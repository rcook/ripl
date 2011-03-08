/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplallc.c
 *		Source file debugging versions of memory-allocation functions.
 *
 *		Version 1.1, last update: 17 May 1998.
 *
 *		History:
 *			17/5/98:	scrapped allocation macros.
 *			20/1/98:		version 1.1.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include "riplallc.h"

#define MALLOC(size)							malloc(size)
#define CALLOC(n, size)						calloc(n, size)
#define REALLOC(block, size)				realloc(block, size)
#define FREE(block)							free(block)

#ifdef _RIPL_ALLOC_DEBUG

/* Debugging versions of allocators. */

enum tagAllocType {
	atMalloc,
	atCalloc,
	atRealloc,
	atFree
};

/* Static objects. */
static int allocations=0;
static unsigned total=0;

/* Static functions. */
static void init_log(void *ptr1,
	void RIPL_PTR *ptr2,
	size_t bytes,
	enum tagAllocType type);
static void log(void *ptr1,
	void RIPL_PTR *ptr2,
	size_t bytes,
	enum tagAllocType type);

void *riplMalloc(size_t size) {
	void *ptr=MALLOC(size);
	if (allocations==0) init_log(NULL, ptr, size, atMalloc);
	else log(NULL, ptr, size, atMalloc);
	allocations++;
	return ptr;
}

void *riplCalloc(size_t n, size_t size) {
	void *ptr=CALLOC(n, size);
	if (allocations==0) init_log(NULL, ptr, n*size, atCalloc);
	else log(NULL, ptr, n*size, atCalloc);
	allocations++;
	return ptr;
}

void *riplRealloc(void *block, size_t size) {
	void *ptr=REALLOC(block, size);
	if (allocations==0) init_log(block, ptr, size, atRealloc);
	else log(block, ptr, size, atRealloc);
	allocations++;
	return ptr;
}

void riplFree(void *block) {
	if (allocations==0) init_log(block, NULL, 0, atFree);
	else log(block, NULL, 0, atFree);
	allocations--;
	FREE(block);
}

static void init_log(void *ptr1,
	void *ptr2,
	size_t bytes,
	enum tagAllocType type) {
	
	FILE *plog_file=fopen("riplallc.log", "wt");
	time_t date;
	
	RIPL_VALIDATE(plog_file)

	date=time(NULL);
	fprintf(plog_file, RIPL_APPNAME " Version " RIPL_VERSION "\n"
		"By " RIPL_AUTHOR "\n"
		"MEMORY ALLOCATIONS LOG FILE\n"
		"---------------------------\n"
		"File created %s\n"
		"Log of all allocation transactions\n"
		"via riplXXalloc and riplFree.\n\n",
		ctime(&date));
	switch (type) {
		case atMalloc:
			fprintf(plog_file,
				"riplMalloc : [%p] %lu bytes\n", ptr2, bytes);
			total+=bytes;
			break;
		case atCalloc:
			fprintf(plog_file,
				"riplCalloc : [%p] %lu bytes\n", ptr2, bytes);
			total+=bytes;
			break;
		case atRealloc:
			fprintf(plog_file,
				"riplRealloc: [%p] (src) [%p] (dest) %lu bytes\n",
				ptr1, ptr2, bytes);
			total+=bytes;
			break;
		case atFree:
			fprintf(plog_file,
				"riplFree   : [%p]\n", ptr1);
			break;
	}
	fclose(plog_file);
}

static void log(void *ptr1,
	void *ptr2,
	size_t bytes,
	enum tagAllocType type) {

	FILE *plog_file=fopen("riplallc.log", "at");
	
	RIPL_VALIDATE(plog_file)
	switch (type) {
		case atMalloc:
			fprintf(plog_file,
				"riplMalloc : [%p] %lu bytes\n", ptr2, bytes);
			total+=bytes;
			break;
		case atCalloc:
			fprintf(plog_file,
				"riplCalloc : [%p] %lu bytes\n", ptr2, bytes);
			total+=bytes;
			break;
		case atRealloc:
			fprintf(plog_file,
				"riplRealloc: [%p] (src) [%p] (dest) %lu bytes\n",
				ptr1, ptr2, bytes);
			total+=bytes;
			break;
		case atFree:
			fprintf(plog_file,
				"riplFree   : [%p]\n", ptr1);
			total+=bytes;
			break;
	}
	fclose(plog_file);
}

#else

/* Non-debugging versions of allocators. */

void *riplMalloc(size_t size) {
	return MALLOC(size);
}

void *riplCalloc(size_t n, size_t size) {
	return CALLOC(n, size);
}

void *riplRealloc(void *block, size_t size) {
	return REALLOC(block, size);
}

void riplFree(void *block) {
	FREE(block);
}

#endif
