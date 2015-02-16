/*
 *		riplwin.c
 *		Standard front-end for Windows applications.
 *		Parses command line and calls standard C 'main' function.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:	renamed file.
 *			19/1/98:	included fix for Watcom compiler
 *			6/12/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#if defined(__WIN32__) || defined(__NT__)

#	ifndef __WATCOMC__
/*
 * Watcom compiler doesn't like WinMain for some reason.
 */

#		include <ctype.h>
#		include <malloc.h>
#		include <windows.h>

/* Prototype for main (define it elsewhere) called by WinMain. */
int main(int argc, char **argv);

#		ifdef __BORLANDC__
#			pragma argsused
#		endif
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine,
    int nCmdShow) {

    char *ptr1, *ptr2, *ptr3;
    unsigned argc, i;
    char **argv;
    int return_val;

    /* Count number of command-line arguments. */
    argc=0;
    ptr1=lpszCmdLine;
    while (*ptr1) {
        if (isspace(*ptr1)) {
            while (isspace(*ptr1)) ptr1++;
            continue;
        }
        argc++;
        while (*ptr1 && !isspace(*ptr1)) ptr1++;
    }

    /* Allocate memory for pointers to them. */
    argv=(char **)calloc(argc, sizeof(char *));

    /* Parse 'em! */
    ptr1=lpszCmdLine;
    for (i=0; i<argc; i++) {
        if (isspace(*ptr1)) {
            while (isspace(*ptr1)) ptr1++;
        }
        ptr2=ptr1;
        while (*ptr2 && !isspace(*ptr2)) ptr2++;
        ptr3=argv[i]=(char *)calloc(ptr2-ptr1+1, sizeof(char));
        while (ptr1<ptr2) {
            *ptr3++=*ptr1++;
        }
        *ptr3=0;
    }

    /* Execute main function recording return value. */
    return_val=main(argc, argv);

    /* Deallocate memory used for parsed arguments. */
    for (i=0; i<argc; i++) free(argv[i]);
    free(argv);

    /* Return to operating system. */
    return return_val;
}
#	endif
#else
#	error WinMain has only been tested with Win32/NT.
#endif
