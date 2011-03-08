@echo off
rem Ripl Version 1.1
rem Watcom/Tcl/Tk Install Script
rem Written by Richard A. Cook, 18 May 1998


if not "%1"=="" goto Install
echo Please specify destination directory.
goto End

:Install
echo Installing source files for BC4 version of Ripl...

copy %1\watcom\pkgindex.tcl %1
copy %1\watcom\ripldll.wcl %1
copy %1\watcom\*.tgt %1
copy %1\watcom\ripl.wpj %1
copy %1\watcom\clean.bat %1

echo @echo off > %1\tclripl.bat
echo set jashe_dir=j:/projects/jashe/wish >> %1\tclripl.bat
echo set jashe_libdir=j:/projects/jashe/wattk >> %1\tclripl.bat
echo set ripl_dlldir=%1 >> %1\tclripl.bat
echo set ripl_tcldir=%1/tcldll >> %1\tclripl.bat
echo s:\tcl\bin\wish80 %1/tcldll/riplmain.tcl >> %1\tclripl.bat

echo Use project file 'ripl.wpj'.
echo 'clean.bat' removes intermediate files.
echo 'tclripl.bat' used to run Tcl/Tk version (after you've compiled it!)
echo YOU NEED THE SOURCE CODE FOR JASHE TO RUN THE TCL/TK VERSION.
echo The project file assumes this is located in j:\projects\jashe.
echo If it isn't you'll have to hack the project files yourself.

:End

