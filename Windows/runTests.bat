@echo off
set SDIR="Release"
if "%1" == "debug" set SDIR="Debug"

del /Q /S TEST > NUL
mkdir TEST > NUL 2>NUL
cd TEST
xcopy /S /E /I /Q ..\..\source\TEST\data data > NUL

copy ..\..\source\TEST\File_test.C . > NUL
copy ..\..\source\TEST\Path_test.C . > NUL
copy ..\..\source\TEST\DefaultProcessors_test.C . > NUL
copy ..\..\source\TEST\KCFFile_test.C . > NUL
copy ..\..\source\TEST\MOLFile_test.C . > NUL

copy ..\TESTS\%SDIR%\*.exe . > NUL
copy ..\Libs\libBALL\%SDIR%\libBALL.* . > NUL
echo running tests....
FOR %%v IN (*.exe) DO (
	echo running %%v
	%%v -v > %%v".log" 2> NUL
	if errorlevel 1 (
	echo FAILED TEST: %%v
	echo FAILED TEST: %%v >> TEST.log
	) ELSE (
	del %%v".log"
	)
)
echo finished TESTs.
echo finished TESTs. >> TEST.log
echo for results, see *.exe.log files and TEST.log
cd ..