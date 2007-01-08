// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef DemoTutorialDialogTUTORIAL_H
#define DemoTutorialDialogTUTORIAL_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
//   # include <BALL/VIEW/KERNEL/message.h>
#endif

#include <BALL/DATATYPE/regularData3D.h>

#include "demoTutorialDialogData.h"

namespace BALL
{
  class System;
	class Composite;

	namespace VIEW
	{
		/**	Dialog for showing a demo and tutorial.
		*/
		class DemoTutorialDialog : 
			public DemoTutorialDialogData,
 			public ModularWidget
		{
			Q_OBJECT
 			BALL_EMBEDDABLE(DemoTutorialDialog,ModularWidget)
				
			public:
			
			/// Default Constructor.
			DemoTutorialDialog(QWidget *parent = NULL, const char *name = NULL )
				throw();

			/// Destructor.
			virtual ~DemoTutorialDialog() throw();
							
			/// Message handling method.
 			virtual void onNotify(Message *message) throw();

			///
			void setDemoMode(bool state) { demo_mode_ = state;}
							
			public slots:
				
			/// Show and raise the dialog
			void show();

			///
			void showDemo();

			///
			void showTutorial();

			/// Next Step
			void nextStepClicked();
						
			protected:

			void nextStepDemo_();
			void nextStepTutorial_();
			
			void initDemo_();
			void initTutorial_();

			void enableNextStep_();

 			virtual void onNotifyTutorial_(Message *message) throw();
 			virtual void onNotifyDemo_(Message *message) throw();

			String getBaseDir_();

			private:

			List<Composite*> composites_;
			RegularData3D* grid_;
			System*  		 	 system_;

			String 					prefix_;
			bool 						demo_mode_;
			Position  			current_step_;
		};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_DemoTutorialDialog_H
