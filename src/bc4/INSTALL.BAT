@echo off
rem BC4 Ripl Install Script
rem Ripl Version 1.1
rem Written by Richard A. Cook, 17/5/98

if not "%1"=="" goto Install
echo Please specify destination directory.
goto End

:Install
echo Installing source files for BC4 version of Ripl...
copy %1\bc4\ripl.ide %1
copy %1\bc4\clean.bat %1
echo Use project file 'ripl.ide'.
echo 'clean.bat' removes intermediate files.

:End
