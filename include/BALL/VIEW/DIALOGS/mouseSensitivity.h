// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mouseSensitivity.h,v 1.3 2003/09/04 22:17:46 amoll Exp $
// 

#ifndef BALL_VIEW_DIALOGS_MOUSESENSITIVITY_H
#define BALL_VIEW_DIALOGS_MOUSESENSITIVITY_H

#include <BALL/VIEW/UIC/mouseSensitivityData.h>

namespace BALL
{
	namespace VIEW
	{

		/** Dialog for setting the mouse sensitivity
				\ingroup ViewDialogs
		*/
		class MouseSensitivity 
			: public MouseSensitivityData
		{ 
				Q_OBJECT

			public:
				MouseSensitivity( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
				~MouseSensitivity();
						
		public slots:
				void okPressed();
				void cancelPressed();
		};

	}
}

#endif
