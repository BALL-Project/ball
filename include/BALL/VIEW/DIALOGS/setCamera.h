// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: setCamera.h,v 1.3 2003/09/04 22:17:48 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_SETCAMERA_H
#define BALL_VIEW_DIALOGS_SETCAMERA_H

#include <BALL/VIEW/UIC/setCameraData.h>

namespace BALL
{
	namespace VIEW
	{
		class Camera;

		/** Dialog to set the camera to a given value
				\ingroup ViewDialogs
		*/
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

} } // namespaces
#endif
