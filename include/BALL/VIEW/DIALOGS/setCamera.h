// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_SETCAMERA_H
#define BALL_VIEW_DIALOGS_SETCAMERA_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/UIC/ui_setCamera.h>

#include <QtWidgets/QDialog>

namespace BALL
{
	namespace VIEW
	{
		class Camera;

		/** Dialog to set the camera to a given value
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT SetCamera 
			: public QDialog,
				public Ui_SetCameraData
		{ 
				Q_OBJECT

			public:
				SetCamera( QWidget* parent = 0, const char* name = "SetCamera", bool modal = false, Qt::WindowFlags fl = 0 );
				~SetCamera();

				Camera* camera;
						
			public slots:
				void okPressed();
		};

} } // namespaces
#endif
