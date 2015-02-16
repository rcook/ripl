/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplentr.c
 *		Main source file for main Ripl entrypoint.
 *
 *		Version 1.1, last update: 3 February 1998.
 *
 *		History:
 *			3/1/98:			made cosmetic changes to help screens.
 *			24/1/98:		fixed response file bug.
 *			24/1/98:		renamed RIPL_PARSEERROR to RIPL_PARAMERROR.
 *			20/1/98:		version 1.1 (renamed riplentr).
 *			31/12/97:	modified included file list.
 *			16/12/97:	modified execute_arguments.
 *			27/11/97:	last major update.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <string.h>
#include "riplop.h"

/* Prototypes of static functions. */
static riplBool execute_arguments(unsigned argc, char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
static riplBool help_arguments(unsigned argc, char **argv);
static void general_help(void);

/*
 *		riplMain1
 *		Main (command-line) entrypoint for programme.
 *		Function parses command-line arguments and response-file
 *		input as necessary. It also loads and saves the input and
 *		output image files. DO NOT PASS EXECUTABLE NAME AS
 *		FIRST ARGUMENT!
 */
int riplMain1(unsigned argc, char **argv) {
	
	riplGreyMap *pinput_greymap, *poutput_greymap;
	riplBool result, resp_file=false;
	char **resp_argv;

	if (argc>0 && **argv=='@') {
		/* Take command line from a response file. */
		if (argc>1) {
			general_help();
			return EXIT_FAILURE;
		}
		if (!riplFileExists(*argv+1)) {
			riplMessage(itError, "Response file %s does not exist!\n", *argv+1);
			return EXIT_FAILURE;
		}
		resp_argv=riplParseResponseFile(*argv+1, &argc);
		if (!resp_argv) {
			riplMessage(itError,
				"Error reading response file %s!\n"
				"[File error, line too long or file too long]\n", *argv+1);
			return EXIT_FAILURE;
		}
		argv=resp_argv;
		resp_file=true;
	}
	if (argc==0) {
		/* No arguments supplied. */
		general_help();
		if (resp_file) riplFree(resp_argv);
		return EXIT_FAILURE;
	}
	if (!strcmp(argv[0], "?")) {
		/* User is requesting some help. */
		if (argc<2) {
			general_help();
			if (resp_file) riplFree(resp_argv);
			return EXIT_FAILURE;
		}
		else {
			if (!help_arguments(argc-1, argv+1)) {
				if (resp_file) riplFree(resp_argv);
				return EXIT_FAILURE;
			}
		}
		if (resp_file) riplFree(resp_argv);
		return EXIT_SUCCESS;
	}
	if (argc<3) {
		general_help();
		if (resp_file) riplFree(resp_argv);
		return EXIT_FAILURE;
	}
	if (!riplFileExists(argv[0])) {
		riplMessage(itError, "Image file %s does not exist!\n", argv[0]);
		if (resp_file) riplFree(resp_argv);
		return EXIT_FAILURE;
	}

	/* Load input image. */
	pinput_greymap=riplLoadImage(argv[0]);
	if (!pinput_greymap) {
		riplMessage(itError, "Unable to load image file %s!\n"
			"[File error or invalid/unsupported format]\n",
			argv[0]);
		if (resp_file) riplFree(resp_argv);
		return EXIT_FAILURE;
	}
	RIPL_VALIDATE_GREYMAP(pinput_greymap)

	/* Allocate another image of the same size as the input. */
	poutput_greymap=(riplGreyMap *)riplMalloc(sizeof(riplGreyMap));
	RIPL_VALIDATE(poutput_greymap)
	poutput_greymap->cols=pinput_greymap->cols;
	poutput_greymap->rows=pinput_greymap->rows;
	poutput_greymap->size=pinput_greymap->size;
	poutput_greymap->data=
		(riplGrey *)riplCalloc(poutput_greymap->size, sizeof(riplGrey));
	RIPL_VALIDATE(poutput_greymap->data)

	/* Execute command-line arguments. */
	result=execute_arguments(argc-2, argv+2, pinput_greymap, poutput_greymap);
	if (result) {
		result=riplSaveImage(argv[1], gfPGMBinary, poutput_greymap);
		if (!result) riplMessage(itError,
			"Error writing image file %s!\n", argv[1]);
	}

	/* Deallocate images. */
	riplFree(pinput_greymap->data);
	riplFree(poutput_greymap->data);
	riplFree(pinput_greymap);
	riplFree(poutput_greymap);
	if (resp_file) riplFree(resp_argv);
	return result ? EXIT_SUCCESS:EXIT_FAILURE;
}

/*
 *		riplMain2
 *		Alternative entrypoint for programme.
 *		Function parses command-line arguments and response-file
 *		input as necessary. It also loads and saves the input and
 *		output image files. Input and output greymaps are passed
 *		as additional arguments. DO NOT PASS EXECUTABLE NAME AS
 *		FIRST ARGUMENT!
 */
int riplMain2(unsigned argc, char **argv,
	riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {

	riplBool resp_file=false;
	char **resp_argv;

	RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
	
	if (argc>0 && **argv=='@') {
		/* Take command line from a response file. */
		if (argc>1) {
			general_help();
			return EXIT_FAILURE;
		}
		if (!riplFileExists(*argv+1)) {
			riplMessage(itError, "Response file %s does not exist!\n", *argv+1);
			return EXIT_FAILURE;
		}
		resp_argv=argv=riplParseResponseFile(*argv+1, &argc);
		if (!argv) {
			riplMessage(itError,
				"Error reading response file %s!\n"
				"[File error, line too long or file too long]\n", *argv+1);
			return EXIT_FAILURE;
		}
		resp_file=true;
	}

	if (argc==0) {
		/* No arguments supplied. */
		general_help();
		if (resp_file) riplFree(resp_argv);
		return EXIT_FAILURE;
	}
	if (!strcmp(argv[0], "?")) {
		/* User is requesting some help. */
		if (argc<2) {
			general_help();
			if (resp_file) riplFree(resp_argv);
			return EXIT_FAILURE;
		}
		else {
			riplMessage(itInfo,
				RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_DATE "\n"
				RIPL_DESCRIPTION "\n"
				"Written by " RIPL_AUTHOR "\n\n");
			if (!help_arguments(argc-1, argv+1)) {
				if (resp_file) riplFree(resp_argv);
				return EXIT_FAILURE;
			}
		}
		if (resp_file) riplFree(resp_argv);
		return EXIT_SUCCESS;
	}

	/* Execute command-line arguments. */
	if (!execute_arguments(argc, argv, pinputGreyMap, poutputGreyMap)) {
		if (resp_file) riplFree(resp_argv);
		return EXIT_FAILURE;
	}
	if (resp_file) riplFree(resp_argv);
	return EXIT_SUCCESS;
}

/*
 *		Function takes the remaining command-line arguments and
 *		parses them further---executing the relevant operators,
 *		passing pointers to the input and output greymaps.
 *		This function also multiple operators to be specified on
 *		the single command line and passes the output of each operator
 *		as the input to the input of the next, allowing several
 *		transforms to be applied to a single image.
 */
static riplBool execute_arguments(unsigned argc,
	char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap) {

	unsigned args_read;
	riplGrey *temp;

	while (1) {
		args_read=riplOperatorExecute(argc,
			(const char **)argv,
			pinputGreyMap,
			poutputGreyMap);
		if (args_read<1) return false;
		argv+=args_read;
		argc-=args_read;
		if (argc<1) return true;
		temp=pinputGreyMap->data;
		pinputGreyMap->data=poutputGreyMap->data;
		poutputGreyMap->data=temp;
	}
}

/* Displays help screens of operators specified on command line. */
static riplBool help_arguments(unsigned argc, char **argv) {
	unsigned i;
	for (i=0; i<argc; i++) {
		if (!riplOperatorHelp(argv[i])) return false;
	}
	return true;
}

/* Displays general help information. */
static void general_help(void) {
	riplMessage(itInfo,
		RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_DATE "\n"
		RIPL_DESCRIPTION "\n"
		"Written by " RIPL_AUTHOR "\n"
		"Signal Processing and Communications Group\n"
		"Cambridge University Engineering Department\n\n"
		"Usage: " RIPL_EXENAME " <input file> <output file> "
		"<op> (<args>) ...\n"
		"Or:    " RIPL_EXENAME " @<response file>\n\n"
		"Where <op> is one of the following:\n"
		"%s\n"
		"For help on a specific operation:\n\n"
		"Usage: " RIPL_EXENAME " ? <op>\n", riplGetOperatorSummary());
}

