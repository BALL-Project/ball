// $Id: modularWidget.sip,v 1.1 2000/07/18 21:27:46 oliver Exp $

class ModularWidget
  : Embeddable
{
%HeaderCode
	#include <BALL/VIEW/OPENGL/WIDGETS/modularWidget.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

  public:
 	// BALL_EMBEDDABLE(ModularWidget)
	// expands to:
	void registerThis();
	void unregisterThis();
	void setIdentifier(const String& /identifier/);
	const String& getIdentifier() const;

	ModularWidget();
 	~ModularWidget();

	static void registerWidget(ModularWidget* /mwidget/);

	virtual void initializeWidget(MainControl& /main_control/);
 	virtual void finalizeWidget(MainControl& /main_control/);
	virtual void checkMenu(MainControl& /main_control/);
}; 
