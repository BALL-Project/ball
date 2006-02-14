// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockWidget.sip,v 1.2.6.1 2006/02/14 15:02:37 amoll Exp $
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

	DockWidget() throw();
  ~DockWidget() throw();

	void applyPreferences() throw();				

	void setVisible(bool);
};
