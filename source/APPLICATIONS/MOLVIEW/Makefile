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
	main.C\
	mainframe.C\
	mainframe_moc.C\
	icons.C\
	DIALOGS/DlgAbout.C\
	DIALOGS/DlgAboutData.C\
	DIALOGS/DlgAbout_moc.C\
	DIALOGS/DlgAboutData_moc.C\
	DIALOGS/DlgDisplayProperties.C\
	DIALOGS/DlgDisplayProperties_moc.C\
	DIALOGS/DlgDisplayPropertiesData.C\
	DIALOGS/DlgDisplayPropertiesData_moc.C\
	DIALOGS/DlgPreferences.C\
	DIALOGS/DlgPreferencesData.C\
	DIALOGS/DlgPreferences_moc.C\
	DIALOGS/DlgPreferencesData_moc.C\
	DIALOGS/DlgPreferencesGeneral.C\
	DIALOGS/DlgPreferencesGeneralData.C\
	DIALOGS/DlgPreferencesGeneral_moc.C\
	DIALOGS/DlgPreferencesGeneralData_moc.C\
	DIALOGS/DlgPreferencesNetwork.C\
	DIALOGS/DlgPreferencesNetworkData.C\
	DIALOGS/DlgPreferencesNetwork_moc.C\
	DIALOGS/DlgPreferencesNetworkData_moc.C\
	DIALOGS/DlgPreferencesDisplay.C\
	DIALOGS/DlgPreferencesDisplayData.C\
	DIALOGS/DlgPreferencesDisplay_moc.C\
	DIALOGS/DlgPreferencesDisplayData_moc.C\
	DIALOGS/DlgSelector.C\
	DIALOGS/DlgSelector_moc.C\
	DIALOGS/DlgSelectorData.C\
	DIALOGS/DlgSelectorData_moc.C


CPP_SOURCES=$(SOURCES)

molview:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALLVIEW_LIBS)

mainframe_moc.C:	mainframe.h
	$(MOC) mainframe.h -o $@

control_moc.C:	control.h
	$(MOC) control.h -o $@

DIALOGS/DlgAbout_moc.C:	DIALOGS/DlgAbout.h
	$(MOC) DIALOGS/DlgAbout.h -o $@

DIALOGS/DlgAboutData_moc.C:	DIALOGS/DlgAboutData.h
	$(MOC) DIALOGS/DlgAboutData.h -o $@

DIALOGS/DlgSelector_moc.C:	DIALOGS/DlgSelector.h
	$(MOC) DIALOGS/DlgSelector.h -o $@

DIALOGS/DlgSelectorData_moc.C:	DIALOGS/DlgSelectorData.h
	$(MOC) DIALOGS/DlgSelectorData.h -o $@

DIALOGS/DlgDisplayProperties_moc.C:	DIALOGS/DlgDisplayProperties.h
	$(MOC) DIALOGS/DlgDisplayProperties.h -o $@

DIALOGS/DlgDisplayPropertiesData_moc.C:	DIALOGS/DlgDisplayPropertiesData.h
	$(MOC) DIALOGS/DlgDisplayPropertiesData.h -o $@

DIALOGS/DlgPreferences_moc.C:	DIALOGS/DlgPreferences.h
	$(MOC) DIALOGS/DlgPreferences.h -o $@

DIALOGS/DlgPreferencesData_moc.C:	DIALOGS/DlgPreferencesData.h
	$(MOC) DIALOGS/DlgPreferencesData.h -o $@

DIALOGS/DlgPreferencesGeneral_moc.C:	DIALOGS/DlgPreferencesGeneral.h
	$(MOC) DIALOGS/DlgPreferencesGeneral.h -o $@

DIALOGS/DlgPreferencesGeneralData_moc.C:	DIALOGS/DlgPreferencesGeneralData.h
	$(MOC) DIALOGS/DlgPreferencesGeneralData.h -o $@

DIALOGS/DlgPreferencesNetwork_moc.C:	DIALOGS/DlgPreferencesNetwork.h
	$(MOC) DIALOGS/DlgPreferencesNetwork.h -o $@

DIALOGS/DlgPreferencesNetworkData_moc.C:	DIALOGS/DlgPreferencesNetworkData.h
	$(MOC) DIALOGS/DlgPreferencesNetworkData.h -o $@

DIALOGS/DlgPreferencesDisplay_moc.C:	DIALOGS/DlgPreferencesDisplay.h
	$(MOC) DIALOGS/DlgPreferencesDisplay.h -o $@

DIALOGS/DlgPreferencesDisplayData_moc.C:	DIALOGS/DlgPreferencesDisplayData.h
	$(MOC) DIALOGS/DlgPreferencesDisplayData.h -o $@

