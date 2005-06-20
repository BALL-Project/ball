// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: logView.sip,v 1.1.2.1 2005/06/20 00:29:19 amoll Exp $
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
