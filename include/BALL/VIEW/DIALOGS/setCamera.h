// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: setCamera.h,v 1.4 2004/02/26 08:41:36 anhi Exp $
//

#ifndef BALL_VIEW_DIALOGS_SETCAMERA_H
#define BALL_VIEW_DIALOGS_SETCAMERA_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/UIC/setCameraData.h>

namespace BALL
{
	namespace VIEW
	{
		class Camera;

		/** Dialog to set the camera to a given value
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT SetCamera 
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
