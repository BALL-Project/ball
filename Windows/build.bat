@echo off
echo.

if "%BALL_PATH%"=="" goto Usage1
if "%QTDIR%"=="" goto Usage2
if "%QTVERSION%"=="" goto Usage22
if "%PYTHONDIR%"=="" goto Usage3
if "%VSINSTALLDIR%"=="" goto Usage4

if not exist Contrib goto Usage5

if "%1" == "debug" (
	set NMAKE_ARG="DEBUG=true" 
	set RESULT_DIR=Debug
	echo OK, we are working on the debugging version
)else if "%1" == "release" (
	set RESULT_DIR=Result
	echo OK, we are working on the release version
)else (
	goto Usage6
)

if "%2" == "clean" goto CLEANUP


if not exist "%PYTHONDIR%\Lib\site-packages\sip.pyd" goto Create_SIP

goto BUILD

:Create_SIP
echo Creating the SIP files...
cd Contrib\sip
python configure.py
nmake
nmake install
echo Installed the SIP files.
cd ..\..
goto BUILD

:BUILD
echo building the libraries...

cd Libs\libBALL
mkdir %RESULT_DIR% > NUL 2> NUL
nmake %NMAKE_ARG% /CS
cd ..

cd libVIEW
mkdir %RESULT_DIR% > NUL 2> NUL
mkdir mocfiles > NUL 2> NULL
nmake %NMAKE_ARG% /CS
cd ..

cd "Python Module"
mkdir %RESULT_DIR% > NUL 2> NUL
nmake %NMAKE_ARG% /CS
cd ..

cd ..\Applications\BALLView
mkdir %RESULT_DIR% > NUL 2> NUL
nmake %NMAKE_ARG% /CS
cd ..

cd ..
goto end


:CLEANUP
echo Cleaning up...
cd Libs\libBALL
nmake %NMAKE_ARG% clean /CS
cd ..

cd libVIEW
nmake %NMAKE_ARG% clean /CS
cd ..

cd "Python Module"
nmake %NMAKE_ARG% clean /CS
cd ..

cd ..\Applications\BALLView
nmake %NMAKE_ARG% clean /CS
cd ..

cd ..\Tests
nmake %NMAKE_ARG% clean /CS
cd ..

goto end

:Usage1
echo. BALL_PATH variable is not set. 
goto Usage

:Usage2
echo. QTDIR variable is not set. 
goto Usage

:Usage22
echo. QTVERSION variable is not set. 
goto Usage

:Usage3
echo. PYTHONDIR variable is not set. 
goto Usage

:Usage4
echo. This file is supposed to be run in the Visual Studio Net or the Microsoft Compiler Suite Command prompt!
goto Usage

:Usage5
echo. You didnt download the Contrib file and extracted it correctly to this directory!
goto Usage

:Usage6
echo. You didnt specifiy if you want to create the debugging or release version of the library!
goto Usage

:Usage
echo.
echo. Have a look at the Readme.txt in BALL\Windows!
goto end

:end
echo.
