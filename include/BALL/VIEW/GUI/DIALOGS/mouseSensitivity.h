// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mouseSensitivity.h,v 1.2 2003/08/26 08:04:58 oliver Exp $
// 

#ifndef BALL_VIEW_GUI_DIALOGS_MOUSESENSITIVITY_H
#define BALL_VIEW_GUI_DIALOGS_MOUSESENSITIVITY_H

#include <BALL/VIEW/UIC/mouseSensitivityData.h>

namespace BALL
{
	namespace VIEW
	{

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
