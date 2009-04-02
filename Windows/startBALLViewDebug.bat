SET BALL_DATA_PATH=..\data
SET RESULTDIR=Debug
SET BALL_BIN_PATH=%BALL_PATH%\Windows\BALL-VisualStudio\%RESULTDIR%
SET PATH=%QTDIR%\lib;%BALL_BIN_PATH%;%BALL_PATH%\Windows\Contrib\bin;%BALL_PATH%\Windows\Contrib\dlls
echo using PATH %PATH%
BALLView.exe
