// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.sip,v 1.1 2004/01/16 16:14:58 amoll Exp $
//

class DisplayProperties
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/displayProperties.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  BALL_EMBEDDABLE(DisplayProperties);
//  DisplayProperties(QWidget *parent, const char*);
  void selectModel(int);
//  void show();
  void applyButtonClicked();
//  void applyPreferences(Preferences&) throw();
//  void checkMenu(MainControl&) throw();
  void coloringOptionsPressed();
  void createRepresentationMode();
//  void defaultPreferences(Preferences&) throw();
  void editColor();
  void editSelectionColor();
//  void fetchPreferences(INIFile &inifile) throw();
//  void finalizePreferencesTab(Preferences &preferences) throw();
//  void finalizeWidget(MainControl&) throw();
//  void initializePreferencesTab(Preferences &preferences) throw();
//  void initializeWidget(MainControl&) throw();
  void modelOptionsPressed();
  void modifyRepresentationMode();
//  void onNotify(Message *message) throw();
  void precisionBoxChanged(int);
  void precisionSliderChanged();
  void selectColoringMethod(int);
  void selectMode(int);
//  void setRepresentation(Representation*) throw();
//  void transparencySliderChanged();
//  void writePreferences(INIFile &inifile) throw();
  ~DisplayProperties() throw();
};
