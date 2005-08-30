// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockWidget.sip,v 1.1.2.1 2005/06/20 00:29:18 amoll Exp $
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
