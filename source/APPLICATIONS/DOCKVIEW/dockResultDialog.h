//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKRESULTDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_CONFORMATIONSET_H
# include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#endif

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
				
					/** if you want to add a new sccoring function extend enum 
					*/
					enum ScoringFunction {DEFAULT = 0, RANDOM = 1};
				
					//Constructor
					DockResultDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
						throw();
						
					//Destructor	
					~DockResultDialog()
						throw();
					
					/**  Assignment operator
					*/
					const DockResultDialog& operator =(const DockResultDialog& res_dialog);
						
					void setConformationSet(ConformationSet& conformations)
						throw() {conformation_set_ = conformations;}
						
					ConformationSet getConformationSet()
						throw() {return conformation_set_;}
						
					void setScoringName(QString name)
						throw() {scoring_name_ = name;}
					
					// add score of a scoring function to score vector scores_
					void addScore(vector<float> new_score)
						throw() {scores_.push_back(new_score);}
						
					// add scoring function to ComboBox and its options dialog to HashMap
					void addScoringFunction(QString name, int score_func, QDialog* dialog=0)
						throw();
						
					// add docked system to BALLView structures 
					void displayDockedSystem()
						throw();
					
				public slots:
				
					// Show and raise result dialog
					void show();
				
					// shows snapshot of selected row
					virtual void showSnapshot();
					
					// selects and shows the entry above the current selected entry
					virtual void upwardClicked();
				
					// selects and shows the entry below the current selected entry
					virtual void downwardClicked();
					
					// shows options dialog of selected scoring function
					virtual void advancedClicked();
					
					// adds a column which contains the sorted scores after reranking
					virtual void scoringClicked();
					
					// sets the advanced button enabled if the selected scoring function has options 
					virtual void scoringFuncChosen();
					
					// sorts the result table by clicked column
					virtual void sortTable(int column);
					
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
							Compare_() throw()
							{}
							
							// constructor
							Compare_(int index) throw()
							{ index_ = index; }
		
							// destructor
							~Compare_() throw()
							{}
					
							// operator ()
							bool operator() (const vector<float> a, const vector<float> b) const
								{ return a[index_] < b[index_]; }
					
							int index_;
					};
					
				private:
				
					ConformationSet conformation_set_;
					System* docked_system_;
					QString scoring_name_;
					
					// vector contains scores of different scoring functions
					vector<vector<float> > scores_;
					
					// key: ScoringFunction(enum), value: advanced options dialog
					HashMap<int, QDialog*> scoring_dialogs_;
		};
		
} } // Namespaces
#endif
