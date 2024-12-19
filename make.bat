@echo off

REM use mkpsxiso for debug purposes, use BUILDCD for final CD
set MAKE_DEBUG_CD=FALSE

REM set the name of the game title here.
REM it is what prepends the the name of
REM .cue or .iso files in the build.xml files
set GAME_NAME=game

REM to be set later
set EXE_NAME=NONE

REM filename for mkpsxiso to read in (excluding ".xml" file extension)
set BUILD_XML_NAME=NONE

REM compiler stuff
set BUILD_DIR=bin
set SOURCE_DIR=source\engine\*.c source\game\*.c source\res\*.c
set INCLUDE_DIR=source
set OPTIONS=-O3 -Xo$80010000

REM region is either american or european
set REGION=NONE

if "%1"=="clean" ( goto clean )

REM ensure the correct region is set
if "%1"=="e"  ( set REGION=e )
if "%1"=="ed" ( set REGION=e )
if "%1"=="a"  ( set REGION=a )
if "%1"=="ad" ( set REGION=a )

REM are we building a debug cd?
if "%1"=="ad" ( set MAKE_DEBUG_CD=TRUE )
if "%1"=="ed" ( set MAKE_DEBUG_CD=TRUE )

if %REGION%==NONE ( goto help )

if %REGION%==e (
	echo Compiling european exe..
	echo.
	set EXE_NAME=e_main
	set BUILD_XML_NAME=e_build
)

if %REGION%==a (
	echo Compiling american exe..
	echo.
	set EXE_NAME=a_main
	set BUILD_XML_NAME=a_build
)

REM build the program
:build

REM run ccpsx
ccpsx %OPTIONS% -I%INCLUDE_DIR% %SOURCE_DIR% -o%EXE_NAME%.cpe,%EXE_NAME%.sym,%EXE_NAME%.map

if %ERRORLEVEL% NEQ 0 (
	echo.
	echo ccpsx failed!
	exit /B
)

REM run cpe2x
cpe2x %EXE_NAME%.cpe /c%REGION%

if %ERRORLEVEL% NEQ 0 (
	echo.
	echo cpe2x failed!
	exit /B
)

if %MAKE_DEBUG_CD%==TRUE (
	echo Building debug CD..
	echo.

	mkpsxiso %BUILD_XML_NAME%.xml

	if %ERRORLEVEL% NEQ 0 (
		echo mkpsxiso failed!
		exit /B
	)
)

echo.
echo All done.
exit /B

REM clean for both american and european
:clean

if not exist %BUILD_DIR%\ (
	mkdir %BUILD_DIR%\
)

del a_main.cpe
del a_main.sym
del a_main.map

move a_main.exe bin

del e_main.cpe
del e_main.sym
del e_main.map

move e_main.exe bin

move %GAME_NAME%_a.cue bin/
move %GAME_NAME%_a.iso bin/
move %GAME_NAME%_e.cue bin/
move %GAME_NAME%_e.iso bin/

echo.
echo All done.
exit /B

:help
echo Usage: make.bat [OPTION]
echo a          compile an american exe
echo ad         build an american debug disc
echo e          compile a european exe
echo ed         build a european debug disc
echo clean      clean the directory of build files

exit /B

