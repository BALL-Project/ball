// $Id: modularWidget.sip,v 1.1 2001/09/03 16:53:09 oliver Exp $

class ModularWidget
  : Embeddable
{
%HeaderCode
	#include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

  public:
 	// BALL_EMBEDDABLE(ModularWidget)
	// expands to:
	void registerThis() throw();
	void unregisterThis() throw();
	void setIdentifier(const String& /identifier/);
	const String& getIdentifier() const;

	ModularWidget();
 	~ModularWidget() throw();

	static void registerWidget(ModularWidget* /mwidget/);

	virtual void initializeWidget(MainControl& /main_control/);
 	virtual void finalizeWidget(MainControl& /main_control/);
	virtual void checkMenu(MainControl& /main_control/) throw();
}; 
