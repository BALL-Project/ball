// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControl.sip,v 1.8 2003/11/18 16:54:45 amoll Exp $
//

class MainControl
//   :  QMainWindow,
//    public ConnectionObject
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/mainControl.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	// Q_OBJECT
	public:
	// typedef List<CompositeDescriptor *>::iterator DescriptorIterator;
	enum PopUpID
	{
		FILE = 10001,
		EDIT,
		BUILD,
		DISPLAY,
		TOOLS,
		HELP,
		USER
	};
	//MainControl(QWidget* parent = 0, const char* name = 0, String inifile = ".BALL.preferences") throw();
	MainControl(const MainControl&) throw();
	~MainControl() throw();
	virtual void clear() throw();

	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static MainControl* getInstance(Position /index/);

  CompositeManager& getCompositeManager() throw();
  INIFile& getINIFile() throw();
  PyCompositeList getControlSelection() throw();

	Preferences* getPreferences() throw();
	PrimitiveManager& getPrimitiveManager() throw();
	// QPopupMenu* initPopupMenu(int /ID/) throw();
	System* getSelectedSystem() throw();
	bool compositesAreMuteable();
	bool updateRepresentationsOf(const Composite& /composite/, bool /rebuild/ = true) throw();
	const FragmentDB& getFragmentDB() const throw();
	PyCompositeList getSelection() const throw();
	const INIFile& getINIFile() const throw();
	void show();
	const String& getMenuHint(Index /id/) const throw();


	// int insertMenuEntry(int /ID/, const String& /name/, const QObject* /receiver/ = 0, const char* /slot/ = 0, int /accel/ = 0, int /entry_ID/ = -1, String /hint/ = "") throw();
	// static MainControl* getMainControl(const QObject* object) throw();
	// void dump(std::ostream& /s/ = std::cout, Size /depth/ = 0) const throw();

	void aboutToExit();
	void addModularWidget(ModularWidget* /widget/) throw();


	void applyPreferences(Preferences& /preferences/) throw();
	void applyPreferencesTab();
	void cancelPreferencesTab() throw();
	void fetchPreferences(INIFile& /inifile/) throw();
	void finalizePreferencesTab(Preferences& /preferences/) throw();
	void initializePreferencesTab(Preferences& /preferences/) throw();
	void writePreferences(INIFile& /inifile/) throw();

	void insertPopupMenuSeparator(int /ID/) throw();
	// void removeMenuEntry(int /ID/, const String& /name/, const QObject* /receiver/ = 0, const char* /slot/ = 0, int /accel/ = 0, int /entry_ID/ = -1) throw();
	void menuItemHighlighted(int /id/) throw();
	void checkMenus();

	void deselectCompositeRecursive(Composite* /composite/, bool /first_call/ = false) throw();
	void selectCompositeRecursive(Composite* /composite/, bool /first_call/ = false) throw();
	void setCompositesMuteable(bool /state/);
	void printSelectionInfos() throw();

	// void onNotify(Message* /message/) throw();
	void removeModularWidget(ModularWidget* /widget/) throw();
	void restoreWindows() throw();
	// void setLabel(QLabel* /label/) throw();
	void setMenuHint(Index /id/, const String& /hint/) throw();
	void setStatusbarText(const String& /text/) throw();
	void updateAllRepresentations(bool /rebuild_display_lists/ = false) throw();
	bool update(Composite& /composite/) throw();
	bool insert(Composite& /composite/) throw();
	bool remove(Composite& /composite/) throw();

	bool update(Representation& /composite/) throw();
	bool insert(Representation& /composite/) throw();
	bool remove(Representation& /composite/) throw();
};
