class Preferences
{
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/preferences.h>
%End
	public:
	Preferences(const Preferences&) throw();
	void fetchPreferences(INIFile&) throw();
	void writePreferences(INIFile&) throw();
	void show();
	void setDefaultValues();
	void reject();
	void applyPreferences();
};
