// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.sip,v 1.3 2003/10/29 22:54:41 oliver Exp $
//

class ModularWidget
  : Embeddable
{
%TypeHeaderCode
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
