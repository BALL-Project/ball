// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_SETCLIPPINGPLANE_H
#define BALL_VIEW_DIALOGS_SETCLIPPINGPLANE_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_CLIPPING_PLANE_H
# include <BALL/VIEW/KERNEL/clippingPlane.h>
#endif

#include <BALL/VIEW/UIC/ui_setClippingPlane.h>

#include <QtGui/QDialog>

namespace BALL
{
	namespace VIEW
	{
		/** Dialog to set a clipping plane to a given value
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT SetClippingPlane
			: public QDialog,
				public Ui_SetClippingData
		{ 
				Q_OBJECT

			public:
				SetClippingPlane( QWidget* parent = 0, const char* name = "SetClippingPlane", ClippingPlane* plane = 0);
				~SetClippingPlane();

				ClippingPlane* plane_;
						
			public slots:
				void okPressed();
		};

} } // namespaces
#endif
