// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.sip,v 1.8 2004/09/08 11:53:21 amoll Exp $
//

class PyWidget
  : ModularWidget
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
	void initializeWidget(MainControl& /main_control/) throw();
	void finalizeWidget(MainControl& /main_control/) throw();
	bool toAbortScript() throw();
	void run(const String&) throw();
}; 
