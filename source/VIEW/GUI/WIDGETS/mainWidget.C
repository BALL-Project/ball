// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainWidget.C,v 1.1.2.1 2003/01/07 13:23:30 anker Exp $

#include <BALL/VIEW/GUI/WIDGETS/mainWidget.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{
	  MainWidget::MainWidget(string options_filename, QWidget* parent, const char* name)
			:	QMainWindow(parent, name),
				MainControl(options_filename, parent, name)
		{
		}
			
		MainWidget::~MainWidget()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MainWidget>() << endl;
			#endif 
		}

	} // namespace VIEW
} // namespace BALL
