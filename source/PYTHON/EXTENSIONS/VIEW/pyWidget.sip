// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
	static PyWidget* getInstance(Position);
	
  PyWidget() throw();
  ~PyWidget() throw();
	
	virtual void initializeWidget(MainControl&) throw();
	virtual void finalizeWidget(MainControl&) throw();
	bool toAbortScript() throw();
	bool openFile(const String&, bool, bool = false) throw();
	bool runCurrentScript();
	void abortScript();
	void loadScript();
	void execScript();
	void saveScript();
	void clearScript();
	void map(String, String, String);
	void unmap(String, String);

	private:
		PyWidget(const PyWidget&);
}; 
