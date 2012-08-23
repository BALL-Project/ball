// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class LogView
  : DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/logView.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	static Size countInstances();
	static LogView* getInstance(Position);

	LogView() throw();
  ~LogView() throw();
};
