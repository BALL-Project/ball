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
				
					void setConformationSet(ConformationSet& conformations)
						throw() {conformation_set_ = conformations;}
						
					ConformationSet getConformationSet()
						throw() {return conformation_set_;}
						
					void setScoringName(QString name)
						throw() {scoring_name_ = name;}
					
					void addScore(vector<float> new_score)
						throw() { scores_.push_back(new_score); }
						
						
					// add scoring function to HashMap and ComboBox
					void addScoringFunction(QString name, int score_func, QDialog* dialog=0)
						throw();
						
					// add docked system to BALLView structures 
					void displayDockedSystem()
						throw();
							
				public slots:
				
					/// Show and raise
					void show();
				
					//
					virtual void showSnapshot();
					
					//
					virtual void upwardClicked();
				
					//
					virtual void downwardClicked();
					
					//
					virtual void advancedClicked();
					
					//
					virtual void scoringClicked();
					
					//
					virtual void scoringFuncChosen();
					
					//
					virtual void sortTable(int section);
					
				protected:
							
				/**
				nested class Compare_ 
				This class is needed for the sorting of the table;
				the rows of the table should be sorted by a certain column
      */
      class Compare_
			{
				public:

					// constructor
					Compare_() throw()
					{}
					
					Compare_(int index) throw()
					{ index_ = index; }
		
					// destructor
					~Compare_() throw()
					{}
					
					// Operator ()
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
