//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKPROGRESSDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKPROGRESSDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_DOCKINGALGORITHM_H
# include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>
#endif

#include <qtimer.h>
#include <qdatetime.h>

#include "dockProgressDialogData.h"

namespace BALL
{
	namespace VIEW
	{
		/**	Dialog for showing the progress during a docking calculation.
    		\ingroup  ViewDialogs
		 */
		class BALL_EXPORT DockProgressDialog : 
			public DockProgressDialogData,
			public ModularWidget
		{ 
			Q_OBJECT
				
			public:
			
			BALL_EMBEDDABLE(DockProgressDialog, ModularWidget)
			
				/** Constructor
				*/
				DockProgressDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();
					
				/** Destructor
				*/
				~DockProgressDialog()
					throw();
			
				/**
				*/
				void fillDialog(const QString& p1, const QString& p2, const QString& alg, const QString& sf, const Options& alg_opt, const Options& sf_opt)
					throw();
					
				/**
				*/
				void setDockingAlgorithm(DockingAlgorithm* alg)
					throw();	
				
				/**
				*/
				const DockingAlgorithm* getDockingAlgorithm() const
					throw();
			
				
			public slots:
			
				/**
				*/
				virtual void show();
			
				/**
				*/
				virtual void pauseClicked();
			
				/**
				*/
				virtual void abortClicked();
			
				
			protected slots:
			
				/**
				*/
				virtual void updateProgress_();
			
			
			protected:
				
				DockingAlgorithm* alg_;
			
				QTimer timer_;
				
				QDateTime start_time_;
		};
		
} } // Namespaces
#endif
