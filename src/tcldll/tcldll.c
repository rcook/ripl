/*
 *		RIPL---Richard's Image-Processing Library.
 *		Tcl/Tk Dynamic-Link Library Version.
 *		Written by Richard Cook.
 *
 *		tcldll.h
 *		Main source file.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:	modified to version 1.1.
 *			26/11/97:	first version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <stdlib.h>
#include <string.h>

#include "ripl.h"
#include "tcldll.h"

/* Static functions. */
static int create(ClientData notused,
	Tcl_Interp *interp,
	int argc,
	char **argv);
static int command(ClientData clientData,
	Tcl_Interp *interp,
	int argc,
	char **argv);
static int output(ClientData clientData);
static int setup(ClientData clientData,
	Tcl_Interp *pInterp,
	const char *pinputImageName,
	const char *poutputImageName);
static int invoke_jashe(ClientData clientData,
	Tcl_Interp *pInterp,
	const char *pcommandLine);

/* Package initialization function. */
int Ripldll_Init(Tcl_Interp *interp) {
	
	Tcl_CreateCommand(interp,
		"riplcreate",
		create,
		(ClientData)NULL,
		(void (*)())NULL);
	return Tcl_PkgProvide(interp, "ripldll", "1.0");
}

/*
 * First argument should be of form '$riplBase' and
 * second '$jasheBase'---names of ripl and jashe commands.
 */
static int create(ClientData notused,
	Tcl_Interp *interp,
	int argc,
	char **argv) {

	ripldllData *data=(ripldllData *)riplMalloc(sizeof(ripldllData));
	RIPL_VALIDATE(data)

	data->jashe_name=(char *)riplCalloc(strlen(argv[2])+1, sizeof(char));
	RIPL_VALIDATE(data->jashe_name)
	
	strcpy(data->jashe_name, argv[2]);

	Tcl_GetCommandInfo(interp,
		argv[2],
		&data->jashe);
	Tcl_CreateCommand(interp,
		argv[1],
		command,
		(ClientData)data,
		&riplFree);
	return TCL_OK;
}

/*
 * Handles all commands to this package.
 *
 *	Commands are:
 *		output					update output
 *		setup					set up internal data structures
 *		ripl <op>				submit command to Ripl
 *								command-line processor
 */
static int command(ClientData clientData,
	Tcl_Interp *pInterp,
	int argc,
	char **argv) {
	
	Jashe_ImgArrays *pimage_arrays;
	riplGreyMap input_greymap, output_greymap;
	riplGrey *pmy_buffer;
	int result;

	if (strlen(argv[1])==6 && riplMatch(argv[1], "output", 0)) {
		return output(clientData);
	}
	if (strlen(argv[1])==5 && riplMatch(argv[1], "setup", 0)) {
		return setup(clientData, pInterp, argv[2], argv[3]);
	}
	RIPL_VALIDATE(argc>2)
	if (!riplMatch(argv[1], "ripl", 0)) {
		riplMessage(itError, "Unrecognized Racdll command '%s'!\n", argv[1]);
		return TCL_OK;
	}
	pimage_arrays=((Jashe_ClientData *)
		((ripldllData *)clientData)->jashe.clientData)->imgArraysPtr;
	output_greymap.cols=input_greymap.cols=pimage_arrays->cols;
	output_greymap.rows=input_greymap.rows=pimage_arrays->rows;
	output_greymap.size=input_greymap.size=
		input_greymap.cols*input_greymap.rows;

	/* Copy input array. */
	pmy_buffer=
		(riplGrey *)riplCalloc(input_greymap.size, sizeof(riplGrey));
	/* Validate the memory allocation. */
	RIPL_VALIDATE(pmy_buffer)
	memcpy(pmy_buffer,
		pimage_arrays->inImgPtr, input_greymap.size);

	/* Assign the temporary array. */
	output_greymap.data=(riplGrey *)pimage_arrays->tmpImgPtr;
	/* Validate it. */
	RIPL_VALIDATE(output_greymap.data)

	/*
	 * Execute the parameters.
	 *
	 * Note that some operators may return the input and output
	 * greymap objects with their data pointers swapped around.
	 * To fix this, a copy of the input pointer is retained and
	 * the situation remedied.
	 */
	input_greymap.data=pmy_buffer;
	result=riplMain2(argc-2, argv+2, &input_greymap, &output_greymap);
	/* The situation, if any, is remedied here. */
	if (pmy_buffer!=input_greymap.data) {
		RIPL_VALIDATE(input_greymap.data==(riplGrey *)pimage_arrays->tmpImgPtr)
		RIPL_VALIDATE(output_greymap.data==pmy_buffer)
		memcpy(pimage_arrays->tmpImgPtr, pmy_buffer, output_greymap.size);
	}
	else {
		RIPL_VALIDATE(output_greymap.data=pimage_arrays->tmpImgPtr)
	}
		
	/* Deallocate the temporary buffer. */
	riplFree(pmy_buffer);

	/*
	 * Don't return TCL_ERROR so user doesn't have to quit
	 * jashe and restart all the time.
	 */
/*	if (result==EXIT_FAILURE) {
		Tcl_AppendResult(pInterp, "ripldll error!", (char *)NULL);
		return TCL_ERROR;
	}*/
	return TCL_OK;
}

/* Updates output. */
static int output(ClientData clientData) {
	
	Jashe_WData w_data;
	Jashe_ImgArrays *pimage_arrays;
	unsigned row, col;
	Jashe_Gray *inP, *outP;
	
	w_data=((Jashe_ClientData *)
		((ripldllData *)clientData)->jashe.clientData)->wData;
	pimage_arrays=((Jashe_ClientData *)
		((ripldllData *)clientData)->jashe.clientData)->imgArraysPtr;

	/* Write the temporary image to the output. */
	Jashe_WPutImgStart(w_data, pimage_arrays);
	inP=pimage_arrays->tmpImgPtr;
	for (row=0; row<pimage_arrays->rows; row++) {
		Jashe_WPutImgRowStart(w_data, pimage_arrays, row);
		outP=pimage_arrays->putImgRowPtr;
		for (col=0; col<pimage_arrays->cols; col++) *outP++=*inP++;
		Jashe_WPutImgRowFinish(w_data, pimage_arrays, row);
	}
	Jashe_WPutImgFinish(w_data, pimage_arrays);
	return Jashe_WErrorValue(pimage_arrays->wData);
}

#define MAX_COMMAND_LEN		100

/* Set up internal data structures. */
static int setup(ClientData clientData,
	Tcl_Interp *pInterp,
	const char *pinputImageName,
	const char *poutputImageName) {
	
	static char command[MAX_COMMAND_LEN];

	invoke_jashe(clientData, pInterp, "conf -reset 0 0 0");
	sprintf(command, "conf -input %s -output %s", pinputImageName, poutputImageName);
	invoke_jashe(clientData, pInterp, command);
	Jashe_SetTmpImg(
		((Jashe_ClientData *)
			((ripldllData *)clientData)->jashe.clientData)->imgArraysPtr,
		sizeof(Jashe_Gray), 0, 0);
	invoke_jashe(clientData, pInterp, "setupprocess");
	return TCL_OK;
}

/* Executes a jashe command line. */
static int invoke_jashe(ClientData clientData,
	Tcl_Interp *pInterp,
	const char *pcommandLine) {
	
	static char command[MAX_COMMAND_LEN];

	sprintf(command, "%s %s",
		((ripldllData *)clientData)->jashe_name,
		pcommandLine);
	return Tcl_Eval(pInterp, command);
}
	
/* Memory-allocation functions. */

void *riplMalloc(size_t size) {
	return Tcl_Alloc(size);
}
void *riplCalloc(size_t n, size_t size) {
	return Tcl_Alloc(n*size);
}
void *riplRealloc(void *block, size_t size) {
	return Tcl_Realloc(block, size);
}
void riplFree(void *block) {
	Tcl_Free(block);
}
