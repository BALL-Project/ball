class HelpViewer
	: DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/helpViewer.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	//HelpViewer(QWidget* parent, const char* name = 0) throw();
	virtual ~HelpViewer() throw();
  virtual void initializeWidget(MainControl&) throw();
	virtual void onNotify(Message *) throw();
	virtual void showHelp(const String&);
	void setDefaultPage(const String&);
	const String& getDefaultPage() const;
	void setBaseDirectory(const String&);
	const String& getBaseDirectory() const;
	//virtual void registerWidgetForHelpSystem(const QWidget*, const String&);
	//void unregisterWidgetForHelpSystem(const QWidget* widget);
	virtual void registerMenuEntryForHelpSystem(Index, const String&);
	void unregisterMenuEntryForHelpSystem(Index);
	bool hasHelpFor(Index) const;
	//bool showHelpFor(const QWidget* widget);
	bool showDocumentationForObject();
	//bool hasHelpFor(const QWidget* widget) const;
			
	virtual void showHelp();
	void enterWhatsThisMode();
	void exitWhatsThisMode();
	//bool eventFilter(QObject*, QEvent*);
};
