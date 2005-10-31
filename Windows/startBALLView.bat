SET BALL_DATA_PATH=..\data
SET LIBSDIR=$(BALL_PATH)\libs\
SET RESULTDIR=Release
SET PATH="%QTDIR%\lib;%LIBSDIR%\libBALL\%RESULT_DIR%;%LIBSDIR%\libView\%RESULT_DIR%;%LIBSDIR%\Python Module\%RESULT_DIR%;%BALL_PATH%\Windows\Applications\BALLVIEW\%RESULT_DIR%"
echo using PATH %PATH%
BALLView.exe 2> ballview.log
