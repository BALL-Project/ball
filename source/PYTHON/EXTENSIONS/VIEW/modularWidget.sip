// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ModularWidget
  : Embeddable
//		ConnectionObject
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/modularWidget.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::VIEW;
	using namespace BALL::Exception;
%End

  public:
 		
	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static ModularWidget* getInstance(Position);

  ModularWidget(const char* = "<ModularWidget>");
	ModularWidget(const ModularWidget&);
 	~ModularWidget() throw();
				
//	virtual void destroy();
	virtual void clear();

	static void registerWidget(ModularWidget*) throw(NullPointer);
//	virtual void initializeWidget(MainControl&);
// 	virtual void finalizeWidget(MainControl&);
	virtual void checkMenu(MainControl&) throw();
//	virtual void initializePreferencesTab(Preferences&);
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
	//void setMenuHint(const String&);
	//void setMenuHelp(const String&);
	//virtual void registerForHelpSystem(const QObject*, const String&);
//	virtual void dump(std::ostream& = std::cout, Size = 0) const throw();
	void setWorkingDirFromFilename_(String) throw();
	virtual void showHelp(const String&);
	virtual bool canHandle(const String&) const;
	virtual bool openFile(const String&);
}; 
