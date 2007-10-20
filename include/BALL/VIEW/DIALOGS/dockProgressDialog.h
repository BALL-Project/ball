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

#include <QtCore/qtimer.h>
#include <QtCore/qdatetime.h>

#include <BALL/VIEW/UIC/dockProgressDialogData.h>

namespace BALL
{
	namespace VIEW
	{
		/**	Dialog for showing the progress during a docking calculation.
    		\ingroup  ViewDialogs
		 */
		class BALL_VIEW_EXPORT DockProgressDialog : 
			public QDialog,
			public Ui_DockProgressDialogData
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
				DockProgressDialog(QWidget* parent = 0, const char* name = "DockProgressDialog")
					throw();
					
				/** Destructor
					*/
				virtual ~DockProgressDialog()
					throw();
				//@}
					
				/**	@name	Accessors: inspectors and mutators
				 */
				//@{
				
				/** Sets docking algorithm.
					* @param      alg docking algorithm
					*/
				void setDockingAlgorithm(DockingAlgorithm* alg)
					throw();
				
				/** Get docking algorithm.
					*/
				const DockingAlgorithm* getDockingAlgorithm() const
					throw();

				/** Fills ProgressDialog with information about the chosen
					* docking partners, algorithm, scoring function and options.
					* @param      p1 first docking partner 
					* @param      p2 second docking partner
					* @param      alg docking algorithm
					* @param      sf scoring function
					* @param      alg_opt algorithm options
					* @param      sf_opt scoring function options
					*/
				void fillDialog(const QString& p1, const QString& p2, const QString& alg, const QString& sf, const Options& alg_opt, const Options& sf_opt)
					throw();
				//@}
				
			public slots:
			
				/** Starts timer and shows dialog to user.
					*/
				void show();
			
				// TODO: pause algorithm!!!
				/** Is called when pause button / continue button is pressed.
					* Pauses / continues docking calculations.
					*/
				void pauseClicked();
			
				/** Is called when abort button is pressed.
					* Aborts docking calculations.
					* Closes and deletes the dialog.
				*/
				void abortClicked();
				
				
			protected slots:
			
				/** Is called when timer elapses.
					* Gets current progress of docking algorithm.
					* Sets progress bar to current progress and calculates remaining time.
					* Closes and deletes the dialog when calculations have finished.
					*/
				void updateProgress_();
			
			
			private:
			
				/** Copy constructor
					* Remark: Copy contructor is private because it is not completed. 
					* The copy constuctor of the QT widgets is private and cannot be called.  
					*/
				DockProgressDialog(const DockProgressDialog& dock_prog_dialog)
					throw();
				
				/**	@name	Assignment
					* Remark: Operator is private because it is not completed. 
					* The assignment operator of the QT widgets is private and cannot be called.  
				 */
				//@{
				
				/** Assignment operator
				 	*/
				const DockProgressDialog& operator =(const DockProgressDialog& dock_prog_dialog)
					throw();
				//@}
					
				/** Pointer to docking algorithm.
					*/
				DockingAlgorithm* alg_;
			
				/** Timer to call \link DockProgressDialog::updateProgress_ updateProgress_ \endlink regularly while calculation is running.
					*/
				QTimer timer_;
				
				/** Start time of docking calculations.
					*/
				QDateTime start_time_;
		};
		
} } // Namespaces
#endif
