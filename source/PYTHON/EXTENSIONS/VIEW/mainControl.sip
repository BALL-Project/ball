class MainControl
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/mainControl.h>
%End
	public:

	MainControl(const MainControl&) throw();
	~MainControl() throw();
	virtual void clear() throw();
	void clearData() throw();

	static Size countInstances();
	static MainControl* getInstance(Position);

	RepresentationManager& getRepresentationManager() throw();
	CompositeManager& getCompositeManager() throw();
	bool updateRepresentationsOf(const Composite&, bool rebuild = true, bool = false) throw();
	void redrawAllRepresentations(bool rebuild_display_lists = false) throw();
	void update(Composite&, bool changed_hierarchy = true) throw();

	// we get a seg fault under windows if we use default parameters here:
	bool insert(Composite& /Transfer/, String) throw();
	bool insert(Composite& /Transfer/) throw();

	bool remove(Composite&, bool = true, bool = true) throw();
	bool update(Representation&) throw();
	bool insert(Representation& /Transfer/) throw();
	bool remove(Representation&) throw();
	Preferences* getPreferences() throw();
	INIFile& getINIFile() throw();
	void sendMessage(Message& /Transfer/) throw();

	virtual void show();
	virtual void checkMenus();
	void stopSimulation();
	void complementSelection();
	virtual void aboutToExit();
	virtual void quit(int = 0);
	virtual bool isAboutToQuit();
	virtual void resize(int, int);
	void setContentSize(int, int);

	void toggleFullScreen();
	void showFullScreen();

	void insertPopupMenuSeparator(int) throw();
	void applyPreferences() throw();
	void fetchPreferences(INIFile&) throw();
	void writePreferences(INIFile&) throw();
	void restoreWindows(const INIFile&) throw();
	void addModularWidget(ModularWidget*) throw();
	void removeModularWidget(ModularWidget*) throw();

	PyCompositeList getSelection() throw();
	PyCompositeList getMolecularControlSelection() throw();
	System* getSelectedSystem() throw();
	void selectCompositeRecursive(Composite*, bool first_call = false) throw();
	void deselectCompositeRecursive(Composite*, bool first_call = false) throw();
	void clearSelection() throw();
	void printSelectionInfos() throw();
	void setStatusbarText(const String&, bool = false, bool = false);
	String getStatusbarText() const	throw();
	const FragmentDB& getFragmentDB() const throw();

	bool compositesAreLocked() const throw();
	bool lockCompositesFor(ModularWidget*) throw();
	bool unlockCompositesFor(ModularWidget*) throw();
	ModularWidget* getLockingWidget() throw();
	bool updateOfRepresentationRunning() throw();
	bool stopedSimulation();
	bool isBusy();
	void wait();

	void setDeleteEntryEnabled(bool) throw();
	void insertDeleteEntry() throw();
	String getWorkingDir() const throw();
	void setWorkingDir(const String&) throw();
	void enableLoggingToFile() throw();
	void disableLoggingToFile() throw();
	void setLoggingFilename(const String&) throw();
	const String& getLoggingFilename() const throw();
	bool useMultithreading() throw();
	void setMultithreading(bool) throw();
	virtual void openFile(const String&) throw();
	void saveBALLViewProjectFile(const String&);
	void loadBALLViewProjectFile(const String&) throw();
	void quickSave();
	void quickLoad();
	void processEvents(Size);
};
