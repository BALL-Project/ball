// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
	static DockWidget* getInstance(Position);

	DockWidget();
  ~DockWidget() throw();

	void applyPreferences() throw();				

	void setWidgetVisible(bool);

	private:
		DockWidget(const DockWidget&);
};
