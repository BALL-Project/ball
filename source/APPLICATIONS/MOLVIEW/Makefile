EXECUTABLE=molview
DIRECTORY=APPLICATIONS/MOLVIEW

all:	$(EXECUTABLE)

clean:	default_clean

include ../../common.mak
include .Dependencies

BALL_INCLUDES=$(BALLVIEW_INCLUDES) $(PYTHON_INCLUDES)

# PYTHON_LIBS=/opt/misc/lib/python1.5/config/libpython1.5.a -lsocket -lnsl -ldl  -lpthread -lthread -lreadline -ltermcap

SOURCES=\
	DIALOGS/DlgAbout.C\
	DIALOGS/DlgAboutData.C\
	DIALOGS/DlgDisplayProperties.C\
	DIALOGS/DlgDisplayPropertiesData.C\
	DIALOGS/DlgAmberMinimization.C\
	DIALOGS/DlgAmberMinimizationData.C\
	DIALOGS/DlgLabelProperties.C\
	DIALOGS/DlgLabelPropertiesData.C\
	DIALOGS/DlgPreferences.C\
	DIALOGS/DlgPreferencesData.C\
	DIALOGS/DlgPreferencesGeneral.C\
	DIALOGS/DlgPreferencesGeneralData.C\
	DIALOGS/DlgPreferencesDisplay.C\
	DIALOGS/DlgPreferencesDisplayData.C\
	DIALOGS/DlgPreferencesNetwork.C\
	DIALOGS/DlgPreferencesNetworkData.C\
	icons.C\
	main.C\
	mainframe.C

MOC_SOURCES=\
	DIALOGS/DlgAbout_moc.C\
	DIALOGS/DlgAboutData_moc.C\
	DIALOGS/DlgDisplayProperties_moc.C\
	DIALOGS/DlgDisplayPropertiesData_moc.C\
	DIALOGS/DlgAmberMinimization_moc.C\
	DIALOGS/DlgAmberMinimizationData_moc.C\
	DIALOGS/DlgLabelProperties_moc.C\
	DIALOGS/DlgLabelPropertiesData_moc.C\
	DIALOGS/DlgPreferences_moc.C\
	DIALOGS/DlgPreferencesData_moc.C\
	DIALOGS/DlgPreferencesGeneral_moc.C\
	DIALOGS/DlgPreferencesGeneralData_moc.C\
	DIALOGS/DlgPreferencesDisplay_moc.C\
	DIALOGS/DlgPreferencesDisplayData_moc.C\
	DIALOGS/DlgPreferencesNetwork_moc.C\
	DIALOGS/DlgPreferencesNetworkData_moc.C\
	mainframe_moc.C


CPP_SOURCES=$(SOURCES)

SPECIAL_OBJECTS=

molview:	$(OBJECTS) $(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALLVIEW_LIBS) $(PYTHON_LIBS)

DIALOGS/DlgAbout_moc.C:     DIALOGS/DlgAbout.h
				$(MOC) DIALOGS/DlgAbout.h -o $@

DIALOGS/DlgAboutData_moc.C: DIALOGS/DlgAboutData.h
				$(MOC) DIALOGS/DlgAboutData.h -o $@

DIALOGS/DlgDisplayProperties_moc.C:     DIALOGS/DlgDisplayProperties.h
				$(MOC) DIALOGS/DlgDisplayProperties.h -o $@

DIALOGS/DlgDisplayPropertiesData_moc.C: DIALOGS/DlgDisplayPropertiesData.h
				$(MOC) DIALOGS/DlgDisplayPropertiesData.h -o $@

DIALOGS/DlgPreferences_moc.C:     DIALOGS/DlgPreferences.h
				$(MOC) DIALOGS/DlgPreferences.h -o $@

DIALOGS/DlgPreferencesData_moc.C: DIALOGS/DlgPreferencesData.h
				$(MOC) DIALOGS/DlgPreferencesData.h -o $@

DIALOGS/DlgPreferencesDisplay_moc.C:     DIALOGS/DlgPreferencesDisplay.h
				$(MOC) DIALOGS/DlgPreferencesDisplay.h -o $@

DIALOGS/DlgPreferencesDisplayData_moc.C: DIALOGS/DlgPreferencesDisplayData.h
				$(MOC) DIALOGS/DlgPreferencesDisplayData.h -o $@

DIALOGS/DlgPreferencesGeneral_moc.C:     DIALOGS/DlgPreferencesGeneral.h
				$(MOC) DIALOGS/DlgPreferencesGeneral.h -o $@

DIALOGS/DlgPreferencesGeneralData_moc.C: DIALOGS/DlgPreferencesGeneralData.h
				$(MOC) DIALOGS/DlgPreferencesGeneralData.h -o $@

DIALOGS/DlgPreferencesNetwork_moc.C:     DIALOGS/DlgPreferencesNetwork.h
				$(MOC) DIALOGS/DlgPreferencesNetwork.h -o $@

DIALOGS/DlgPreferencesNetworkData_moc.C: DIALOGS/DlgPreferencesNetworkData.h
				$(MOC) DIALOGS/DlgPreferencesNetworkData.h -o $@

DIALOGS/DlgAmberMinimization_moc.C:     DIALOGS/DlgAmberMinimization.h
				$(MOC) DIALOGS/DlgAmberMinimization.h -o $@

DIALOGS/DlgAmberMinimizationData_moc.C: DIALOGS/DlgAmberMinimizationData.h
				$(MOC) DIALOGS/DlgAmberMinimizationData.h -o $@

DIALOGS/DlgLabelProperties_moc.C:     DIALOGS/DlgLabelProperties.h
				$(MOC) DIALOGS/DlgLabelProperties.h -o $@

DIALOGS/DlgLabelPropertiesData_moc.C: DIALOGS/DlgLabelPropertiesData.h
				$(MOC) DIALOGS/DlgLabelPropertiesData.h -o $@

mainframe_moc.C:	mainframe.h
					$(MOC) mainframe.h -o $@
