//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKPROGRESSDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKPROGRESSDIALOG_H

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
			public DockProgressDialogData
		{ 
			Q_OBJECT
				
			public:
			
				/**	@name	Constructors and Destructors
				 */
				//@{
			
				/** Constructor
					* @param      parent the parent widget of the DockProgressDialog
					*	@param      name the name of the DockProgressDialog
				 	*	@param			modal the modal flag
				 	*	@param			fl the widget flags
					*/
				DockProgressDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();
					
				/** Copy constructor.
					*/
				DockProgressDialog(const DockProgressDialog& dock_prog_dialog)
					throw();
					
				/** Destructor
					*/
				virtual ~DockProgressDialog()
					throw();
		
				//@}
					
				/** Assignment operator
				 	*/
				const DockProgressDialog& operator =(const DockProgressDialog& dock_prog_dialog)
					throw();
					
				/** Set docking algorithm.
					* @param      alg docking algorithm
					*/
				void setDockingAlgorithm(DockingAlgorithm* alg)
					throw();
				
				/** Get docking algorithm
					*/
				const DockingAlgorithm* getDockingAlgorithm() const
					throw();

				/** Fill ProgressDialog with information about the chosen
					* docking partners, algorithm, scoring function and options
					* @param      p1 first docking partner 
					* @param      p2 second docking partner
					* @param      alg docking algorithm
					* @param      sf scoring function
					* @param      alg_opt algorithm options
					* @param      sf_opt scoring function options
					*/
				void fillDialog(const QString& p1, const QString& p2, const QString& alg, const QString& sf, const Options& alg_opt, const Options& sf_opt)
					throw();
					
				
			public slots:
			
				/** Start timer and remember start time.
					* Then show dialog to user.
					*/
				void show();
			
				/**
				*/
				void pauseClicked();
			
				/**
				*/
				void abortClicked();
				
				
			protected slots:
			
				/** Get progress of docking algorithm.
					* Set progress bar to current progress and calculate remaining time
					*/
				void updateProgress_();
			
			
			protected:
				
				DockingAlgorithm* alg_;
			
				QTimer timer_;
				
				QDateTime start_time_;
		};
		
} } // Namespaces
#endif
