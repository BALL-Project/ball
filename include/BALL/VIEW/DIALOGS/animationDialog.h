// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: animationDialog.h,v 1.3 2004/08/15 23:21:45 amoll Exp $
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

#ifdef BALL_QT_HAS_THREADS
 #include <qthread.h>
 #include <qevent.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class AnimationDialog;


		/** Dialog to animate a "flight" in the Scene
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT AnimationDialog 
			: public AnimationDialogData
		{ 
			friend class AnimationThread;

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

// 			protected:
				void animate_();

				List<Camera> cameras_;
				bool stop_;
				AnimationThread* animation_thread_;
		};

#ifdef BALL_QT_HAS_THREADS
		///
		class AnimationThread
			: public QThread
		{
			public:
				
				///
				AnimationThread()
					throw(): ani_(0) {};

				///
				void setAnimationDialog(AnimationDialog* ani)
					throw() { ani_ = ani;}

				///
				virtual void run();// {ani_->animate_();}

				protected:
				AnimationDialog* ani_;
		};
#endif


} } // namespaces
#endif
