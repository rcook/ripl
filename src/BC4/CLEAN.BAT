@echo off
rem BC4 Clean File
rem Ripl Version 1.1
rem By Richard A. Cook, 17/5/98

cd oplib
del *.obj
cd ..\riplcmd
del *.obj
cd ..\riplmain
del *.obj
cd ..\riplop
del *.obj
cd ..\ripltool
del *.obj
cd ..\riplwin
del *.obj
cd ..
del *.obj
del *.lib

