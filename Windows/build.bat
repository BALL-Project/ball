@echo off
echo.

if "%BALL_PATH%"=="" goto Usage1
rem if "%VSINSTALLDIR%"=="" goto Usage4

if not exist Contrib goto Usage5

set OLDDIR=%CD%

if "%1" == "debug" (
	set NMAKE_ARG="DEBUG=true" 
	set RESULT_DIR=Debug
	echo OK, we are working on the debugging version
)else if "%1" == "release" (
	set RESULT_DIR=Release
	echo OK, we are working on the release version
)else (
	goto Usage6
)

if "%2" == "BALL" goto BALL
if "%2" == "clean" goto CLEANUP
if "%2" == "tests" goto TESTS
if "%2" == "benchmarks" goto BENCHMARKS
if "%2" == "tutorial" goto TUTORIAL
if "%2" == "all" goto ALL

if "%QTDIR%"=="" goto Usage2
if "%QTVERSION%"=="" goto Usage22
if "%PYTHONDIR%"=="" goto Usage3

if not exist "%PYTHONDIR%\Lib\site-packages\sip.pyd" goto Create_SIP

goto BUILD

rem ---------------------------------- Create the sip DLL and executeable ----------------------------
:Create_SIP
echo Creating the SIP files ...
cd Contrib\sip
"%PYTHONDIR%\"python configure.py
nmake
nmake install
echo Installed the SIP files.
cd "%OLDDIR%"
goto BUILD

rem ---------------------------------- Build the libs and the application ----------------------------
:BUILD
echo ---------------------------------------------------------------
echo building the libraries and BALLView ...

mkdir Libs\libVIEW\mocfiles 2> NUL

for %%i in ("Libs\libBALL" "Libs\libVIEW" "Libs\Python Module"  "Applications\BALLView") do (
echo ----------------------------------------------------------------
echo running make in %%i
echo ----------------------------------------------------------------
cd %%i
mkdir %RESULT_DIR% > NUL 2> NUL
nmake %NMAKE_ARG% /CS
if errorlevel 1 (
echo Error occured while running make in %%i%, aborting...
goto end
)
cd "%OLDDIR%"
)

goto end

rem ---------------------------------- Build only the BALL DLL --------------------
:BALL
echo building only the BALL dll ...

cd Libs\libBALL
mkdir %RESULT_DIR% > NUL 2> NUL
nmake %NMAKE_ARG% /CS
goto end

rem ---------------------------------- Build the tests ----------------------------
:TESTS
echo building the tests ...

cd Tests
mkdir %RESULT_DIR% > NUL 2> NUL
nmake %NMAKE_ARG% /CS
goto end

rem ---------------------------------- Build the Benchmarks -----------------------
:BENCHMARKS
echo building the benchmarks ...

cd Benchmarks
mkdir %RESULT_DIR% > NUL 2> NUL
nmake %NMAKE_ARG% /CS
goto end

rem ---------------------------------- Build the Tutorials -----------------------
:TUTORIAL
echo building the tutorials ...

cd Tutorial
mkdir %RESULT_DIR% > NUL 2> NUL
nmake %NMAKE_ARG% /CS
goto end

rem ----------------------------------------------- Make clean ---------------------------------------
:CLEANUP
echo Cleaning up ...
for %%i in ("Libs\libBALL" "Libs\libVIEW" "Libs\Python Module"  "Applications\BALLView" Tests Tutorial Benchmarks) do (
echo running make clean in %%i
cd %%i
nmake %NMAKE_ARG% clean /CS
cd "%OLDDIR%"
)

goto end

rem ----------------------------------------------- Make all ---------------------------------------
: ALL
call build.bat %1
call build.bat %1 tests
call build.bat %1 benchmarks
call build.bat %1 tutorial

goto end

rem ---------------------------------------------- Useage hints -----------------------------------------
:Usage1
echo BALL_PATH variable is not set. 
goto Usage

:Usage2
echo QTDIR variable is not set. 
goto Usage

:Usage22
echo QTVERSION variable is not set. 
goto Usage

:Usage3
echo PYTHONDIR variable is not set. 
goto Usage

:Usage4
echo This file is supposed to be run in the Visual Studio Net or the Microsoft Compiler Suite Command prompt!
goto Usage

:Usage5
echo You didnt download the Contrib file and extracted it correctly to this directory!
goto Usage

:Usage6
echo Please specifiy if you want to create a debugging or release version!
goto Usage

:Usage
echo.
echo Useage: build debug^|release [BALL^|tests^|clean^|benchmarks^|tutorial^|all]
echo.
echo You can also have a look at in BALL\Windows\Readme.txt
goto end

:end
cd "%OLDDIR%"
echo.
