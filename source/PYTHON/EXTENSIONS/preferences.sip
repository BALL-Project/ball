// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferences.sip,v 1.2 2004/01/18 18:22:11 oliver Exp $
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
	//	bool hasTabs() throw();
	void show();
	void fetchPreferences(INIFile& /inifile/) throw();
	// void insertTab(QWidget* /child/, const QString& /name/) throw();
	// void removeTab(QWidget* /child/) throw();
	void writePreferences(INIFile& /inifile/) throw();
	~Preferences() throw();
};
