// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainWidget.C,v 1.2 2002/02/27 12:25:19 sturm Exp $

#include <BALL/VIEW/GUI/WIDGETS/mainWidget.h>

using std::istream;
using std::ostream;

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

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/mainWidget.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
