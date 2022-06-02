@echo off
rem --------------------------------------------------------
rem _cleat.bat version 2009.08.23
rem Copyright (c) NetworkLab Corporation All rights reserved
rem --------------------------------------------------------

rem --------------------------------------------------------
rem borland
rem --------------------------------------------------------
del /s *.~*
del /s *.dcu
del /s *.dsk
del /s *.cfg
del /s *.dsm
del /s *.dcp
del /s *.tds
del /s *.#*
del /s *.local
del /s *.cgl
del /s *.ilc
del /s *.ild
del /s *.ilf
del /s *.ils
del /s *.map

rem --------------------------------------------------------
rem microsoft
rem --------------------------------------------------------
del /s *.obj
del /s *.pch
del /s *.idb
del /s *.pdb
del /s *.exp
del /s *.ilk
del /s *.ncb
del /s *.aps
del /s *.plg
del /s *.opt
del /s *.bsc
del /s *.sbr
del /s *.user
del /s *.dep
del /s *.manifest
del /s *.user
del /s *.manifest.res
del /s BuildLog.htm
del /s /ah *.suo

rem --------------------------------------------------------
rem other
rem --------------------------------------------------------
rem del /s *.exe
rem del /s *.lib
del /s *.ini
del /s *.bak
del /s *.log
del /s *.xml
del /s *.sdf

rem --------------------------------------------------------
rem folder
rem --------------------------------------------------------
rm_folder /q log
rm_folder /q Debug
rm_folder /q Release
rm_folder /q /h __history
rmdir /q /s ipch
rmdir /q /s debug
rmdir /q /s .vs