// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.sip,v 1.11 2005/07/16 21:00:43 oliver Exp $
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
	
	void startInterpreter();
	void stopInterpreter();
	virtual void initializeWidget(MainControl& /main_control/) throw();
	virtual void finalizeWidget(MainControl& /main_control/) throw();
	bool toAbortScript() throw();
	void run(const String&) throw();
}; 
