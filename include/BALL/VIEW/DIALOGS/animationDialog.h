// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: animationDialog.h,v 1.1 2004/08/13 18:05:04 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_ANIMATIONDIALOG_H
#define BALL_VIEW_DIALOGS_ANIMATIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#include <BALL/VIEW/UIC/animationDialogData.h>

namespace BALL
{
	namespace VIEW
	{
		/** Dialog to animate a "flight" in the Scene
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT AnimationDialog 
			: public AnimationDialogData
		{ 
				Q_OBJECT

			public:
				AnimationDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
				~AnimationDialog();
						
			public slots:
				void closePressed();
				void animatePressed();
				void gotoPressed();
				void deletePressed();
				void entrySelected();
				void cancelPressed();
				void addPressed();

			protected:
				List<Camera> cameras_;
		};

} } // namespaces
#endif
