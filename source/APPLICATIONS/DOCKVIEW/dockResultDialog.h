//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include "dockResult.h"
#include "dockResultDialogData.h"

namespace BALL
{
	namespace VIEW
	{
		class BALL_EXPORT DockResultDialog : 
				public DockResultDialogData,
				public ModularWidget
		{ 
				Q_OBJECT
				BALL_EMBEDDABLE(DockResultDialog,ModularWidget)
				
				public:
				
					// if you want to add a new sccoring function extend enum 
					enum ScoringFunction {DEFAULT = 0, AMBER_FF = 1, RANDOM = 2};
				
					//Constructor
					DockResultDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
						throw();
						
					//Destructor	
					~DockResultDialog()
						throw();
					
					//  Assignment operator
					const DockResultDialog& operator =(const DockResultDialog& res_dialog)
						throw();
						
					void setDockResult(DockResult* dock_res)
						throw();
						
					void setDockedSystem(System* system)
						throw();
						
					// Adds scoring function to Combobox and its advanced option dialogs to HashMap, if it has such an dialog.
					void addScoringFunction(const QString& name, const int score_func, QDialog* dialog=0)
						throw();
					
				public slots:
				
					// show and raise result dialog
					void show();
				
					// show snapshot of selected row
					virtual void showSnapshot();
					
					// select and show the entry above the current selected entry
					virtual void upwardClicked();
				
					// selects and show the entry below the current selected entry
					virtual void downwardClicked();
					
					// set the advanced button enabled if the selected scoring function has options
					// otherwise the button is disabled 
					virtual void scoringFuncChosen();
					
					// show options dialog of selected scoring function
					virtual void advancedClicked();
					
					// calculate new scores with the chosen scoring function and add a new score column, 
					// the table is sorted by this new column
					virtual void scoringClicked();
					
					// delete a score column 
					virtual void deleteClicked();
					
					// sorts the result table by a clicked column
					void sortTable(int column);
					
				protected:
				
					/**
					nested class Compare_ 
					This class is needed for the sorting of the table;
					the rows of the table should be sorted by a certain column
      		*/
      		class Compare_
					{
						public:

							// default constructor
							Compare_() throw();
							
							// constructor
							Compare_(int index) throw();
		
							// destructor
							~Compare_() throw();
					
							// operator ()
							bool operator() (const vector<float>& a, const vector<float>& b) const
								throw();
					
							int index_;
					};
					
				private:
				
					// DockResult contains all information of the performed docking
					DockResult* dock_res_;
					
					// system which contains the two docked partners
					System* docked_system_;
					
					// key: ScoringFunction(enum), value: advanced options dialog
					HashMap<int, QDialog*> scoring_dialogs_;
		};
		
} } // Namespaces
#endif
