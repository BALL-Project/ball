// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: logView.sip,v 1.2 2005/07/03 09:43:25 oliver Exp $
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
