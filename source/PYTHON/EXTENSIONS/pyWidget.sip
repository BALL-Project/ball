// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.sip,v 1.12.16.1 2007/03/25 21:38:50 oliver Exp $
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
	bool openFile(const String&, bool) throw();
	bool runCurrentScript();
	void abortScript();
	void loadScript();
	void execScript();
	void saveScript();
	void clearScript();
	void map(String, String, String);
	void unmap(String, String);
}; 
