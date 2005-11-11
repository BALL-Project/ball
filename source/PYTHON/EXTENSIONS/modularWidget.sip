// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.sip,v 1.4.6.2 2005/11/11 17:21:24 leonhardt Exp $
//

class ModularWidget
  : Embeddable
//		ConnectionObject
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/modularWidget.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

  public:
 		
	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static ModularWidget* getInstance(Position /index/);

  ModularWidget(const char* = "<ModularWidget>")	throw();
	ModularWidget(const ModularWidget&) throw();
 	~ModularWidget() throw();
				
	virtual void destroy() throw();
	virtual void clear() throw();

	
	static void registerWidget(ModularWidget*) throw(NullPointer);
//	virtual void initializeWidget(MainControl& /main_control/);
// 	virtual void finalizeWidget(MainControl& /main_control/);
	virtual void checkMenu(MainControl& /main_control/) throw();
//	virtual void initializePreferencesTab(Preferences&)	throw();
//	virtual void finalizePreferencesTab(Preferences&)	throw();
	virtual void applyPreferences() throw();
  virtual void fetchPreferences(INIFile&) throw();
	virtual void writePreferences(INIFile&)	throw();
	virtual void setStatusbarText(String , bool = false)
	throw();
	MainControl* getMainControl() const	throw();
	String getWorkingDir() throw();
	void setWorkingDir(const String&) throw();
	FragmentDB& getFragmentDB() const throw();
	bool lockComposites() throw();
	bool unlockComposites()	throw();
	//QMenuBar* menuBar() throw();
	//Index insertMenuEntry (Index, const String&, const QObject*= 0, const char* = 0, Index = 0, Index = -1) throw();
	void setMenuHint(const String&);
	void setMenuHelp(const String&);
	//virtual void registerWidgetForHelpSystem(const QWidget*, const String&);
	virtual void registerMenuEntryForHelpSystem(Index, const String&);
//	virtual void dump(std::ostream& = std::cout, Size = 0) const throw();
	void setWorkingDirFromFilename_(String) throw();
	void removeMenuEntries();
	virtual void showHelp(const String&);
}; 
