SET BALL_DATA_PATH=..\data
SET LIBSDIR=$(BALL_PATH)\libs\
SET PATH="%QTDIR%\lib;$(LIBSDIR)\libBALL\Release;$(LIBSDIR)\libView\Release;$(LIBSDIR)\Python Module\Release;$(BALL_PATH)\Windows\Applications\BALLVIEW\Release"
echo using PATH %PATH%
BALLView.exe 2> ballview.log
