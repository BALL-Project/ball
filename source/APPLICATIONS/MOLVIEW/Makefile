EXECUTABLE=molview

all:	$(EXECUTABLE)

include ../../common.mak

BALL_INCLUDES=$(BALLVIEW_INCLUDES)

OBJECTS=\
	control.o\
	control_moc.o\
	displayProperties.o\
	displayProperties_moc.o\
	main.o\
	mainframe.o\
	mainframe_moc.o

molview:	$(OBJECTS) 
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(BALLVIEW_LIBS)

