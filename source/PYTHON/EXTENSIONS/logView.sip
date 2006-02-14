// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: logView.sip,v 1.2.6.1 2006/02/14 15:02:40 amoll Exp $
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
	static LogView* getInstance(Position /index/);

	LogView() throw();
  ~LogView() throw();
};
