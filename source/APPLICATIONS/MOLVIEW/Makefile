EXECUTABLE=molview

all:	$(EXECUTABLE)

include .Dependencies
include ../../common.mak

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

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
	DlgSelector.o\
	DlgSelector_moc.o\
	DlgSelectorData.o\
	DlgSelectorData_moc.o\
	main.o\
	mainframe.o\
	mainframe_moc.o

molview:	$(OBJECTS) ../../VIEW/KERNEL/timerTextView.o
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) ../../VIEW/KERNEL/timerTextView.o $(BALLVIEW_LIBS)

mainframe_moc.C:	mainframe.h
	moc mainframe.h -o $@

control_moc.C:	control.h
	moc control.h -o $@

displayProperties_moc.C:	displayProperties.h
	moc displayProperties.h -o $@

DlgAbout_moc.C:	DlgAbout.h
	moc DlgAbout.h -o $@

DlgAboutData_moc.C:	DlgAboutData.h
	moc DlgAboutData.h -o $@

DlgSelector_moc.C:	DlgSelector.h
	moc DlgSelector.h -o $@

DlgSelectorData_moc.C:	DlgSelectorData.h
	moc DlgSelectorData.h -o $@

DlgDisplayProperties_moc.C:	DlgDisplayProperties.h
	moc DlgDisplayProperties.h -o $@

DlgDisplayPropertiesData_moc.C:	DlgDisplayPropertiesData.h
	moc DlgDisplayPropertiesData.h -o $@

