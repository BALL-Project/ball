// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Preferences
{
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/preferences.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
	// Preferences(QWidget *parent = NULL, const char *name = NULL, int width = 400, int height = 300) throw();
	Preferences(const Preferences&) throw();
	//virtual ~Preferences() throw();
			
	//bool hasPages() throw();
	//void insertEntry(PreferencesEntry*) throw();
	//void removeEntry(PreferencesEntry *)	throw();
	void fetchPreferences(INIFile&) throw();
	void writePreferences(INIFile&) throw();

	void show();
	//void showEntry(QWidget* page);
	//void entrySelected(QListViewItem* item);
	void setDefaultValues();
	void reject();
	void applyPreferences();
	void showHelp();
};
