// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.1.4.1 2007/03/25 21:32:26 oliver Exp $
//

#include "mainframe.h"

#include <BALL/VIEW/WIDGETS/helpViewer.h>

#include <BALL/SYSTEM/path.h>

//   #undef BALL_PYTHON_SUPPORT

#ifdef BALL_PYTHON_SUPPORT
  #	include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif

#include <QtGui/qmenubar.h>
#include <QtGui/qlabel.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qcursor.h>
#include <QtGui/QPixmap>

#include <sstream>

using namespace std;

namespace BALL
{
	using namespace std;
	using namespace BALL::VIEW;

	Mainframe::Mainframe(QWidget* parent, const char* name)
		:	MainControl(parent, name, ".PyBALL")
	{
		#ifdef BALL_VIEW_DEBUG
		Log.error() << "new Mainframe " << this << std::endl;
		#endif

		// ---------------------
		// setup main window
		// ---------------------
		setWindowTitle("PyBALL");
		// make sure submenus are the first 
		initPopupMenu(FILE);
		initPopupMenu(EDIT);
	#ifdef BALL_PYTHON_SUPPORT
		initPopupMenu(TOOLS_PYTHON);
		initPopupMenu(MainControl::USER);
	#endif
		initPopupMenu(WINDOWS);

		// ---------------------
		// Logstream setup -----
		// ---------------------
//   		Log.remove(std::cout);
//   		Log.remove(std::cerr);
		setLoggingFilename("BALLView.log");

		HelpViewer* BALL_docu = new HelpViewer(this, "BALL Docu");
		addDockWidget(Qt::LeftDockWidgetArea, BALL_docu);

		Path path;
		String dirp = path.find(   String("..")
		                         + FileSystem::PATH_SEPARATOR 
														 + "doc" 
														 + FileSystem::PATH_SEPARATOR
														 + "BALL"
														 + FileSystem::PATH_SEPARATOR );

		BALL_docu->setBaseDirectory(dirp);
		BALL_docu->setWhatsThisEnabled(false);
		BALL_docu->setProject("BALL");
		BALL_docu->setDefaultPage("index.htm");
		BALL_docu->show();

		#ifdef BALL_PYTHON_SUPPORT
			PyWidget* py = new PyWidget(this, "Python Interpreter");
 			addDockWidget(Qt::RightDockWidgetArea, py);
			py->show();
		#endif

		setStatusbarText("Ready.");
	}

	Mainframe::~Mainframe()
		throw()
	{
	}

}
