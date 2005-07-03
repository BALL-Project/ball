// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.sip,v 1.6 2005/07/16 21:00:42 oliver Exp $
//

class ModularWidget
  : Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/modularWidget.h>
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

	virtual void initializeWidget(MainControl& /main_control/) throw();
 	virtual void finalizeWidget(MainControl& /main_control/) throw();
	virtual void checkMenu(MainControl& /main_control/) throw();
}; 
