EXECUTABLE=molview

all:	$(EXECUTABLE)

include ../../common.mak

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

DlgAbout_moc.C:	DlgAbout.h
	moc DlgAbout.h -o $@

DlgDisplayProperties_moc.C:	DlgDisplayProperties.h
	moc DlgDisplayProperties.h -o $@

OBJECTS=\
	control.o\
	control_moc.o\
	displayProperties.o\
	displayProperties_moc.o\
	DlgAbout.o\
	DlgAboutData.o\
	DlgAbout_moc.o\
	DlgAboutData_moc.o\
	DlgDisplayProperties.o\
	DlgDisplayProperties_moc.o\
	DlgDisplayPropertiesData.o\
	DlgDisplayPropertiesData_moc.o\
	main.o\
	mainframe.o\
	mainframe_moc.o

molview:	$(OBJECTS) ../../VIEW/KERNEL/timerTextView.o
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) ../../VIEW/KERNEL/timerTextView.o $(BALLVIEW_LIBS)

