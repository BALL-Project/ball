// $Id: mainWidget.C,v 1.1.4.1 2002/10/29 13:20:46 amoll Exp $

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
