// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
#define BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#include <QtWidgets/QWidget>

namespace BALL
{
	using namespace BALL::VIEW;

	class Mainframe	
		: public BALL::VIEW::MainControl
	{
		Q_OBJECT

		public:

		///
		BALL_EMBEDDABLE(Mainframe, MainControl)

		///
		Mainframe(QWidget* parent = 0, const char* name = 0);

		///
		virtual ~Mainframe() throw();
	};

} // namespace BALL

#endif // BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
