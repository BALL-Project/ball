class HelpViewer
	: DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/helpViewer.h>
%End
	public:

	virtual ~HelpViewer() throw();
	virtual void initializeWidget(MainControl&) throw();
	virtual void onNotify(Message *) throw();
	virtual void showHelp(const String&);
	void setDefaultPage(const String&);
	const String& getDefaultPage() const;
	void setBaseDirectory(const String&);
	const String& getBaseDirectory() const;
	virtual void registerMenuEntryForHelpSystem(Index, const String&);
	void unregisterMenuEntryForHelpSystem(Index);
	bool hasHelpFor(Index) const;
	bool showDocumentationForObject();

	virtual void showHelp();
	void enterWhatsThisMode();
	void exitWhatsThisMode();
};
