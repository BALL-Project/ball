//   
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_WIDGETS_DOCKINGCONTROLLER_H
# include <BALL/VIEW/WIDGETS/dockingController.h>
#endif

#include <BALL/VIEW/UIC/ui_dockResultDialog.h>

namespace BALL
{
	class DockResult;

	namespace VIEW
	{
		/** Dialog for showing the docking results.
			* \ingroup ViewDialogs
			*/
		class BALL_VIEW_EXPORT DockResultDialog : 
				public QDialog,
				public Ui_DockResultDialogData
		{ 
			Q_OBJECT

			public:

				/**	@name	Constructors and Destructors
				 */
				//@{
			
				/** Constructor
					*	@param      parent the parent widget of the DockResultDialog
				 	*	@param      name the name of the DockResultDialog
				 	*	@param			modal the modal flag
				 	*	@param			fl the widget flags
					*/
				DockResultDialog(QWidget* parent = 0, const char* name = "DockResultDialog");
					
				/** Destructor
				*/
				virtual ~DockResultDialog();
				//@}
					
				/**	@name	Assignment
				 */
				//@{
				
				/** Assignment operator
				*/
				const DockResultDialog& operator =(const DockResultDialog& res_dialog);
				//@}
				
				/**	@name	Accessors: mutators
				 */
				//@{
				
				/** Sets the dock result.
					* @param			dock_res dock result
					*/
				void setDockResult(DockResult* dock_res);

				/** Sets the docked system.
					* @param			system docked system
					*/
				void setDockedSystem(System* system);

				/** Adds scoring function to combobox and its advanced option dialog to hashmap, if it has such a dialog.
					* @param			name the name of the scoring function
					* @param			score_func the value of enum DockingController::ScoringFunction
					* @param			dialog advanced option dialog
					*/
				void addScoringFunction(const QString& name, DockingController::ScoringFunction score_func, QDialog* dialog=0);
				//@}
					
			public slots:

				/** Shows and raises result dialog.
					* Fills the result table in the dialog with the values stored in \link DockResultDialog::dock_res_ dock_res_ \endlink .
				*/
				void show();

				/** Is called when show snapshot button is pressed or when a row of the table is double-clicked.
					* Shows snapshot of selected row.
				*/
				void showSnapshot();

				/** Is called when header of a column in the result table is pressed.
					* Sorts the result table by a clicked column.
					* Uses nested class Compare_ for sorting.
				*/
				void sortTable(int column);

				/** Is called when docking options button is pressed.
					* Shows docking options in a small dialog.
				*/
				void showDockingOptions();

				/** Is called when upward button is pressed.
					* Selects the row above the current selected row and shows its snapshot.
				*/
				void upwardClicked();

				/** Is called when downward button is pressed.
					* Selects the row below the current selected row and shows its snapshot.
				*/
				void downwardClicked();

				/** Is called when a scoring function is chosen in the combobox. 
					* Sets the advanced button enabled if the selected scoring function has options
				*  otherwise the button is disabled.
				*/
				void scoringFuncChosen();

				/** Is called when advanced button for scoring function options is pressed.
			 	 *	Shows corresponding options dialog, if it exits.
			 	 */
				void advancedClicked();

				/** Is called when new scoring button is pressed.
					* Calculates new scores with the chosen scoring function and adds a new score column. 
					* Calls \link DockResultDialog::sortTable sortTable \endlink to sort the table by this new column.
					*/
				void scoringClicked();
				
				/** Is called when close button is pressed.
					* Closes and deletes the dialog.
				*/
				void closeClicked();
				
				
				protected slots:

				void selectionChanged_();
			
				/** Is called when context menu entry <b> Delete Score Column </b> is pressed.
					* Deletes a score column.
					*/
				void deleteColumn_();

				/** Is called when context menu entry <b> Scoring Options </b> is pressed.
					* Shows options of the scoring function in a small dialog.
					* @param			column number of the column for which the scoring function options should be shown
					*/
				void showScoringOptions_();

				/** Is called when context menu entry <b> Redock </b> is pressed.
					* Calls \link DockingController::runDocking DockingController::runDocking \endlink for redocking.
					* @param			row number of the row for which redocking should be started
					*/
				void redock_();

				
			protected:

				/**
				* Nested class in DockResultDialog.
				* This class is needed for the sorting of the table.
				* The rows of the table should be sorted by a certain column.
				*/
				class Compare_
				{
					public:

						/** Default constructor
						*/
						Compare_();

						/** Constructor
						*/
						Compare_(Position index);

						/** Destructor
						*/
						~Compare_();

						/** Operator ()
						*/
						bool operator() (const vector<float>& a, const vector<float>& b) const;

						/** Index of the entry by which the two vectors in \link Compare::operator() operator() \endlink are compared.
						*/
						Position index_;
				};

			private:

				/** Copy constructor
					* Remark: Copy contructor is private because it is not completed. 
					* The copy constuctor of the QT widgets is private and cannot be called.  
					*/
				DockResultDialog(const DockResultDialog& dock_res_dialog);
			
				/** DockResult contains all information of the performed docking.
					* @see DockResult
				*/
				DockResult* dock_res_;

				/** System which contains the two docked partners.
					*/
				System* docked_system_;
				
				/** The two redocking partners.
					*/
				System* redock_partner1_, * redock_partner2_;

				/** key: DockingController::ScoringFunction, value: advanced options dialog
					*/
				HashMap<int, QDialog*> scoring_dialogs_;
		};
		
} } // Namespaces
#endif
