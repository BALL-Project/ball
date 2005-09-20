//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H

#include "dockResult.h"
#include "dockResultDialogData.h"
#include "dockingController.h"

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for showing the docking results.
			* \ingroup ViewDialogs
			*/
		class BALL_EXPORT DockResultDialog : 
				public DockResultDialogData
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
				DockResultDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();

				/** Copy constructor.
					*/
				DockResultDialog(const DockResultDialog& dock_res_dialog)
					throw();
					
				/** Destructor
				*/
				virtual ~DockResultDialog()
					throw();

				//@}
					
				/** Assignment operator
				*/
				const DockResultDialog& operator =(const DockResultDialog& res_dialog)
					throw();

				/** Sets the dock result
					* @param			dock_res dock result
					*/
				void setDockResult(DockResult* dock_res)
					throw();

				/** Sets the docked system
					* @param			system docked system
					*/
				void setDockedSystem(System* system)
					throw();

				/** Adds scoring function to Combobox and its advanced option dialog to HashMap, if it has such a dialog.
					* @param			name the name of the scoring function
					* @param			score_func scoring function (enum)
					* @param			dialog advanced option dialog
					*/
				void addScoringFunction(const QString& name, DockingController::ScoringFunction score_func, QDialog* dialog=0)
					throw();

			public slots:

				/** show and raise result dialog
				*/
				void show();

				/** show snapshot of selected row
				*/
				void showSnapshot();

				/** sorts the result table by a clicked column
				*/
				void sortTable(int column);

				/**
				*/
				void showDockingOptions();

				/** select and show the entry above the current selected entry
				*/
				void upwardClicked();

				/** selects and show the entry below the current selected entry
				*/
				void downwardClicked();

				/** set the advanced button enabled if the selected scoring function has options
				*  otherwise the button is disabled
				*/
				void scoringFuncChosen();

				/** show options dialog of selected scoring function
				*/
				void advancedClicked();

				/** calculate new scores with the chosen scoring function and add a new score column, 
					* the table is sorted by this new column
					*/
				void scoringClicked();
				
				/** Shows a context menu with entries "Delete Score Column", "Scoring Options" and "Redock"
					* @param			row number of the row which the user clicked
					* @param			column number of the row which the user clicked
					* @param			pos
					*/
				void contextMenuRequested(int row, int column, const QPoint& pos);

				/** Closes the dialog.
				*/
				void closeClicked();
				
				
			protected slots:
			
				/** Deletes a score column.
					* @param			column number of the column which should be deleted
					*/
				void deleteColumn_(int column);

				/** Shows options of the scoring function in a small dialog.
					* @param			column number of the column for which the scoring function options should be shown
					*/
				void showScoringOptions_(int column);

				/** 
					* 
					*/
				void redock_(int row);

				
			protected:

				/**
				* nested class Compare_ 
				* This class is needed for the sorting of the table;
				* the rows of the table should be sorted by a certain column
				*/
				class Compare_
				{
					public:

						/** default constructor
						*/
						Compare_() throw();

						/** constructor
						*/
						Compare_(int index) throw();

						/** destructor
						*/
						~Compare_() throw();

						/** operator ()
						*/
						bool operator() (const vector<float>& a, const vector<float>& b) const
							throw();

						/**
						*/
						int index_;
				};

			private:

				/** DockResult contains all information of the performed docking
				*/
				DockResult* dock_res_;

				/** system which contains the two docked partners
					*/
				System* docked_system_;
				
				/**
					*/
				System* redock_partner1_, * redock_partner2_;

				/** key: ScoringFunction(enum), value: advanced options dialog
					*/
				HashMap<int, QDialog*> scoring_dialogs_;
		};
		
} } // Namespaces
#endif
