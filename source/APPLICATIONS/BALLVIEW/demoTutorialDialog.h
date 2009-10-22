// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALL_DEMOTUTORIALDIALOG_H
#define BALL_DEMOTUTORIALDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
# include <BALL/VIEW/PRIMITIVES/mesh.h>
#endif

#include <BALL/DATATYPE/regularData3D.h>

#include "ui_demoTutorialDialog.h"

#include <QtGui/QDialog>

namespace BALL
{
  class System;
	class Composite;

	namespace VIEW
	{
		/**	Dialog for showing a demo and tutorial.
		*/
		class DemoTutorialDialog : 
			public QDialog,
			public Ui_DemoTutorialDialogData,
 			public ModularWidget
		{
			Q_OBJECT
 			BALL_EMBEDDABLE(DemoTutorialDialog,ModularWidget)
				
			public:
			
			/// Default Constructor.
			DemoTutorialDialog(QWidget *parent = NULL, const char *name = NULL );

			/// Destructor.
			virtual ~DemoTutorialDialog();
							
			/// Message handling method.
 			virtual void onNotify(Message *message);

			virtual void initializeWidget(MainControl& main_control);

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

			virtual void checkMenu(MainControl& main_control);
						
			protected:

			void nextStepDemo_();
			void nextStepTutorial_();
			
			void initDemo_();
			void initTutorial_();

			void enableNextStep_();

 			virtual void onNotifyTutorial_(Message *message);
 			virtual void onNotifyDemo_(Message *message);

			String getBaseDir_();

			private:

			List<Composite*> composites_;
			RegularData3D* grid_;
			System*  		 	 system_;

			String 					prefix_;
			bool 						demo_mode_;
			Position  			current_step_;
			Mesh* 					surface_;
			QAction* 				demo_action_, *tutorial_action_;
		};

} } // namespaces

#endif // BALL_DEMOTUTORIALDIALOG_H
