class ModularWidget
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/modularWidget.h>
%End
	public:

	static Size countInstances();
	static ModularWidget* getInstance(Position);

	ModularWidget(const char* = "<ModularWidget>");
	ModularWidget(const ModularWidget&);
	~ModularWidget() throw();

	virtual void clear();
	static void registerWidget(ModularWidget*) throw(NullPointer);
	virtual void checkMenu(MainControl&) throw();
	virtual void applyPreferences() throw();
	virtual void fetchPreferences(INIFile&) throw();
	virtual void writePreferences(INIFile&)	throw();
	virtual void setStatusbarText(String , bool = false) throw();
	MainControl* getMainControl() const	throw();
	String getWorkingDir() throw();
	void setWorkingDir(const String&) throw();
	FragmentDB& getFragmentDB() const throw();
	bool lockComposites() throw();
	bool unlockComposites()	throw();
	void setWorkingDirFromFilename_(String) throw();
	virtual bool canHandle(const String&) const;
	virtual bool openFile(const String&);
};
