SET BALL_DATA_PATH=..\data
SET LIBSDIR=%BALL_PATH%\Windows\libs
SET RESULTDIR=Debug
copy %BALL_PATH%\Windows\Applications\BALLVIEW\%RESULTDIR%\BALLView.* %LIBSDIR%\"Python Module"\%RESULTDIR%
SET PATH=%QTDIR%\lib;%LIBSDIR%\libBALL\%RESULTDIR%;%LIBSDIR%\libView\%RESULTDIR%;%LIBSDIR%\"Python Module"\%RESULTDIR%;;%BALL_PATH%\Windows\Contrib\ONC_RPC\bin
echo using PATH %PATH%
BALLView.exe
