// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.sip,v 1.7 2004/04/19 17:08:19 amoll Exp $
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
}; 
