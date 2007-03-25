// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockWidget.sip,v 1.2.20.1 2007/03/25 21:37:07 oliver Exp $
//

class DockWidget
  : ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/dockWidget.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	static Size countInstances();
	static DockWidget* getInstance(Position /index/);

	DockWidget();
  ~DockWidget() throw();

	void applyPreferences() throw();				

	void setWidgetVisible(bool);
};
