EXECUTABLE=molview
DIRECTORY=APPLICATIONS/MOLVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES)


SOURCES=\
	control.C\
	control_moc.C\
	DlgAbout.C\
	DlgAboutData.C\
	DlgAbout_moc.C\
	DlgAboutData_moc.C\
	DlgDisplayProperties.C\
	DlgDisplayProperties_moc.C\
	DlgDisplayPropertiesData.C\
	DlgDisplayPropertiesData_moc.C\
	DlgPreferences.C\
	DlgPreferencesData.C\
	DlgPreferences_moc.C\
	DlgPreferencesData_moc.C\
	DlgPreferencesGeneral.C\
	DlgPreferencesGeneralData.C\
	DlgPreferencesGeneral_moc.C\
	DlgPreferencesGeneralData_moc.C\
	DlgPreferencesNetwork.C\
	DlgPreferencesNetworkData.C\
	DlgPreferencesNetwork_moc.C\
	DlgPreferencesNetworkData_moc.C\
	DlgPreferencesDisplay.C\
	DlgPreferencesDisplayData.C\
	DlgPreferencesDisplay_moc.C\
	DlgPreferencesDisplayData_moc.C\
	DlgSelector.C\
	DlgSelector_moc.C\
	DlgSelectorData.C\
	DlgSelectorData_moc.C\
	main.C\
	mainframe.C\
	mainframe_moc.C

CPP_SOURCES=$(SOURCES)

molview:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALLVIEW_LIBS)

mainframe_moc.C:	mainframe.h
	moc mainframe.h -o $@

control_moc.C:	control.h
	moc control.h -o $@

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

DlgPreferences_moc.C:	DlgPreferences.h
	moc DlgPreferences.h -o $@

DlgPreferencesData_moc.C:	DlgPreferencesData.h
	moc DlgPreferencesData.h -o $@

DlgPreferencesGeneral_moc.C:	DlgPreferencesGeneral.h
	moc DlgPreferencesGeneral.h -o $@

DlgPreferencesGeneralData_moc.C:	DlgPreferencesGeneralData.h
	moc DlgPreferencesGeneralData.h -o $@

DlgPreferencesNetwork_moc.C:	DlgPreferencesNetwork.h
	moc DlgPreferencesNetwork.h -o $@

DlgPreferencesNetworkData_moc.C:	DlgPreferencesNetworkData.h
	moc DlgPreferencesNetworkData.h -o $@

DlgPreferencesDisplay_moc.C:	DlgPreferencesDisplay.h
	moc DlgPreferencesDisplay.h -o $@

DlgPreferencesDisplayData_moc.C:	DlgPreferencesDisplayData.h
	moc DlgPreferencesDisplayData.h -o $@

