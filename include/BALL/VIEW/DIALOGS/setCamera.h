// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: setCamera.h,v 1.1 2003/08/26 12:11:04 oliver Exp $
//

#ifndef BALL_VIEW_GUI_DIALOGS_SETCAMERA_H
#define BALL_VIEW_GUI_DIALOGS_SETCAMERA_H

#include <BALL/VIEW/UIC/setCameraData.h>

namespace BALL
{
	namespace VIEW
	{
		class Camera;

		class SetCamera 
			: public SetCameraData
		{ 
				Q_OBJECT

			public:
				SetCamera( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
				~SetCamera();

				Camera* camera;
						
			public slots:
				void okPressed();
		};

	}
}

#endif
