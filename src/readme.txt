-----------------------------------------------------------------
Ripl---Richard's Image-Processing Library
Version 1.1 (17 May 1998)
Written by Richard A. Cook (Richard.Cook@USA.Net).
Cambridge University Engineering Department, UK.
-----------------------------------------------------------------
SOURCE FILE INSTALLATION INSTRUCTIONS
-----------------------------------------------------------------

Contents
--------

Section 1...................Watcom C/C++ 10.5/Tcl/Tk Instructions
Section 2......................................HP-UX Instructions
Section 3...........................Borland C++ 4.02 Instructions


Section 1---Installation under Watcom C/C++ (Version 10.5)
----------------------------------------------------------

The following are instruction for installing the C source code for Ripl for compiling using the Watcom C compiler (as installed on the Signal Processing Group network at Cambridge University Engineering Department):

1. Copy the file 'ripl.zip' into a _new_ installation directory.

2. Unzip the file into the directory by typing:
		pkunzip -d ripl.zip
This creates several subdirectories containing all the C source and header files.

3. In this same directory type
		watcom/install <full path of directory>
This moves the Watcom project files into the right places.

4. Compile the project by loading the project file 'ripl.wpj' into the IDE.

This creates the command-line version 'ripl.exe' and a dynamic-link library 'ripldll.dll' which is used by the Tcl/Tk version.

PLEASE NOTE THAT YOU NEED THE SOURCE CODE FOR JASHE TO USE THE Tcl/Tk VERSION OF THIS PROGRAMME. I CAN'T PROVIDE THIS BECAUSE I DIDN'T WRITE IT.

Section 2---Installation under HP-UX
------------------------------------

The following are instructions for installing Ripl under the HP-UX Unix-type operating system (as installed on the Cambridge University Engineering Department teaching system). These instructions will probably work on other varieties of Unix, though I've not tried it yet. The makefiles generated are pretty standard.

1. Copy the file 'ripl.zip' into a new installation directory.

2. Unzip 'ripl.zip' by typing
		unzip ripl.zip
This creates several subdirectories containing all the C source and header files.

3. In this same directory type
		hp-ux/install .
It is important to type the full stop '.' at the end of his command. If it's missed out then the installation script will complain. This script generates a series of makefiles in each subdirectory which will allow the Ripl static libraries and the
executable 'ripl' to be automatically compiled.

4. Build the libraries and executable by typing
		make all
This will create four static libraries which can be bound to other object files in order to access the Ripl library functions without the command-line interface. The executable file called 'ripl' is also generated.

5. Other 'make' options include:

---     make depend
This updates the source file dependency lists. This is used when new source files (for example, in the 'oplib' directory, are written). It automatically updates the dependencies in each library makefile to include all files with the extension '.c'.
---     make clean
Removes all object files, makefile backups, library files and the executable.
---     make update
This regenerates individual makefiles (or all of them if used from the Ripl root directory). Useful if the makefile template in the 'makemake' file located in this directory is changed.

Section 3---Installation under Borland C++ 4.02
-----------------------------------------------

The following are instruction for installing the C source code for Ripl for compiling using the Borland C/C++ compiler (version 4.02 and above). These instructions may also work on newer versions of the same compiler.

1. Copy the file 'ripl.zip' into a _new_ installation directory.

2. Unzip the file into the directory by typing:
		pkunzip -d ripl.zip
This creates several subdirectories containing all the C source and header files.

3. In this same directory type
		bc4/install <full path of directory>
This moves the Watcom project files into the right places.

4. Compile the project by loading the project file 'ripl.ide' into the IDE.

This creates the Win32 command-line executable 'ripl.exe'.

------------------------------------------------------------------
HAVE FUN!
------------------------------------------------------------------

E-mail:
		Richard.Cook@USA.Net

Updates and new versions of this programme will be made available at the following URL:

		http://bigfoot.com/~ripl1

------------------------------------------------------------------