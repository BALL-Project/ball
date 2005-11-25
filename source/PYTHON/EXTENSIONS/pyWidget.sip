// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.sip,v 1.9.4.4 2005/11/25 15:04:56 amoll Exp $
//

class PyWidget
  : DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/pyWidget.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static PyWidget* getInstance(Position /index/);
	
  PyWidget() throw();
	PyWidget(const PyWidget& /widget/) throw();
  ~PyWidget() throw();
	
	virtual void initializeWidget(MainControl& /main_control/) throw();
	virtual void finalizeWidget(MainControl& /main_control/) throw();
	bool toAbortScript() throw();
	bool run(const String&) throw();
	bool runAgain();
}; 
