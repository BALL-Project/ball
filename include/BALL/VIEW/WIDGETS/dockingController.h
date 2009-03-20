// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockingController.h,v 1.2.12.2 2007/03/26 07:26:29 amoll Exp $
//

#ifndef BALL_WIDGETS_DOCKINGCONTROLLER_H
#define BALL_WIDGETS_DOCKINGCONTROLLER_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_DIALOGS_DOCKDIALOG_H
# include <BALL/VIEW/DIALOGS/dockDialog.h>
#endif

namespace BALL
{
	class ConformationSet;
	class DockingAlgorithm;
	class EnergeticEvaluation;

	namespace VIEW
	{
		class DockProgressDialog;
		class DockResultDialog;
		class DockingThread;

		/**	Class for docking two systems.
			 It is responsible to show the docking dialog and start a docking caluclation with the options 
			 of the dialog. 
			 If BALLView was compiled with multithreading support it simultaneously shows the progress dialog.
			 Finally it shows the result dialog.
			 \ingroup	ViewWidgets
		 */
		class BALL_VIEW_EXPORT DockingController
			: public QWidget, 
				public ModularWidget
		{
			Q_OBJECT
			
			public:
			
			BALL_EMBEDDABLE(DockingController, ModularWidget)
			
				////// TODO: enum Algorithm und ScoringFunction in DockingAlgorithm bzw. EnergeticEvaluation ///////
				
				/**	@name	Enumerations
				 */
				//@{
				
				/** Enumeration which contains the docking algorithms.
						The numbering corresponds to the order in the combobox of 
						\link DockingController::dock_dialog_ dock_dialog_ \endlink.
				 		If you want to add a new docking algorithm extend enumeration 
				 		(0 corresponds to <select> item in ComboBox)
				 */
				enum Algorithm 
				{
					GEOMETRIC_FIT = 1
				};
				
				/** Enumeration which contains the scoring functions.
						The numbering corresponds to the order in the combobox of 
						\link DockingController::dock_dialog_ dock_dialog_ \endlink. 
						If you want to add a new scoring function extend enumeration 
				 */
				enum ScoringFunction 
				{
					DEFAULT = 0, 
					AMBER_FF, 
					MMFF94_FF,
					SELECTED_FF,
					RANDOM
				};

				//@}
				/**	@name	Constructors, Destructors, and Assignment
				 */
				//@{

				/// Default Constructor.
				DockingController(QWidget* parent = 0, const char* name = 0);

				/// Destructor
				virtual ~DockingController();
				
				///	Assignment operator
				const DockingController& operator =(const DockingController& dock_controller);
				
				//@}
				/**	@name ModularWidget methods
				*/
				//@{
				
				/**	Initializes the popup menu <b>	Molecular Mechanics </b> with its checkable submenu <b>	Docking </b>.
						Calls \link DockDialog::initializeWidget DockDialog::initializeWidget \endlink.
						It is called automatically	immediately before the main application is started.
					  @param main_control the	MainControl object to be initialized
					  @see	 openDialog
					  @see	 finalizeWidget
					  @see	 insertMenuEntry
				*/
				virtual void initializeWidget(MainControl& main_control);
				
				/** Fetches the preferences from the INIFile.
				 		Calls \link DockDialog::fetchPreferences DockDialog::fetchPreferences \endlink.
						This method will be called inside the method \link MainControl::show MainControl::show \endlink. 
				 		@see		writePreferences
				*/
				virtual void fetchPreferences(INIFile& file);

				/** Writes the preferences to the INIFile.
				 		Calls \link DockDialog::writePreferences DockDialog::writePreferences \endlink.
						This method will be called inside the method \link MainControl::aboutToExit MainControl::aboutToExit \endlink. 
						@see		fetchPreferences
				*/
				virtual void writePreferences(INIFile& file);

				/** Updates the state of menu entry Docking in the popup menu <b>	Molecular Mechanics </b>.
					  Enables the menu entry if more than two composites are loaded.
					  Disables the menu entry if less composites are loaded or if a simulation / calculation is running.
				*/
				virtual void checkMenu (MainControl& main_control);

				/** Message handling method.
				  	Catches <b> DockingFinishedMessage </b> and <b> ShowDockResultMessage </b>.
					 	@param message the pointer to the message that should be processed
				*/
				virtual void onNotify(Message *message);
					
				//@}	
			
				/** Shows docking dialog, checks which algorithm is chosen and creates new DockingAlgorithm object.
						Starts new Thread and fills/shows \link DockingController::progress_dialog_ progress_dialog_ \endlink.
						@param			is_redock flag that indicates if a docking or redocking should be run
				 */
				virtual void runDocking(bool is_redock);

				///	Get docking dialog. 
				DockDialog& getDockDialog()	;

				///
				virtual EnergeticEvaluation* createEvaluationMethod(Index method);

			public slots:
				
				/** Is called when docking is started by clicking on menu entry	<b> Docking </b>.
						Calls \link DockingController::runDocking runDocking(false) \endlink.
				 */
				void startDocking();
				
			protected:
			
				/** Applies scoring function which user has chosen.
						Then, creates new DockResult and adds new scoring to it.
						At the end, adds the docked system to BALLView structures
						and sends a <b> NewDockResultMessage </b> to insert the DockResult in DatasetControl.
						Is called in \link DockingController::onNotify onNotify \endlink.
					  @param		conformation_set conformation set that contains the result of the docking algorithm
					  @return		true if scoring function was succesfully applied
					  @return		false if scoring function was not succesfully applied
			 */
			 bool runScoring_(ConformationSet* conformation_set);
				
			private:
				
				/** Copy constructor
					 	Remark: Copy contructor is private because it is not completed. 
					 	The copy constuctor of the QT widgets is private and cannot be called.	
				*/
				DockingController(const DockingController& dock_controller);
			
				/** Dialog for docking and redocking
				 */
				DockDialog dock_dialog_;
				DockResultDialog* dock_result_dialog_;
				DockProgressDialog* progress_dialog_;
				
				/// Pointer to docking algorithm
				DockingAlgorithm* dock_alg_;
				
				/// Menu entry 
				QAction* action_;

				DockingThread* thread_;
		};
	} // end of namespace View
} // end of namespace BALL
#endif
