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

#include <QtWidgets/QDialog>

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
			void setDemoMode(bool) { tutorial_type_ = DEMO; }
			
			///
			//void setTutorialType(int type) { tutorial_type_ = type;}

			public slots:
				
			/// Show and raise the dialog
			void show();

			///
			void showDemo();

			///
			void showTutorial();
			
			///
			void showRaytracingTutorial();

			/// Next Step
			void nextStepClicked();
						
			virtual void checkMenu(MainControl& main_control);
						
			protected:

			void nextStepDemo_();
			void nextStepTutorial_();
			
			void initDemo_();
			void initTutorials_();
		
			void enableNextStep_();

 			virtual void onNotifyTutorial_(Message *message);
 			virtual void onNotifyRaytracingTutorial_(Message *message);
 			virtual void onNotifyDemo_(Message *message);

			void addPlane_(char plane_specifier, int height, int boundary, bool bottom = true);

			String getBaseDir_();

			private:
			
			enum TUTORIAL_TYPE
			{
				DEMO,
				TUTORIAL,
				RAYTRACING_TUTORIAL
			};

			TUTORIAL_TYPE    tutorial_type_;

			std::list<Composite*> composites_;
			
			RegularData3D*   grid_;
			System*  		 	   system_;

			String 					 prefix_;
			Position  			 current_step_;
			Mesh* 					 surface_;
			QAction* 				 demo_action_, *tutorial_action_, *raytracing_tutorial_action_;
		};

} } // namespaces

#endif // BALL_DEMOTUTORIALDIALOG_H
