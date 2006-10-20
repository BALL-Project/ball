// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockWidget.sip,v 1.2.2.2 2006/10/20 08:32:40 amoll Exp $
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
