DIRECTORY=VIEW/KERNEL

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	client.C\
	colorExtension2.C\
	colorExtension3.C\
	colorExtension4.C\
	colorExtension5.C\
	compositeManager.C\
	connectionObject.C\
	geometricObject.C\
	message.C\
	QTTimer.C\
	logView.C\
	vertex1.C\
	vertex2.C

MOC_SOURCES=\
	logView_moc.C

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)


logView_moc.C:	$(BALL_PATH)/include/BALL/VIEW/KERNEL/logView.h
	$(MOC) $(BALL_PATH)/include/BALL/VIEW/KERNEL/logView.h -o logView_moc.C

