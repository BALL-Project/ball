// $Id: dockResultDialog.C,v 1.1.2.4 2005/03/22 11:32:38 haid Exp $
//

#include <qtable.h>
#include <qcombobox.h>
#include <qpushbutton.h>

#include "dockResultDialog.h"

#ifndef BALL_STRUCTURE_DOCKING_ENERGETICEVALUATION_H
# include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_RANDOMEVALUATION_H
# include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		
		//Constructor
		DockResultDialog::DockResultDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockResultDialogData(parent, name, modal, fl),
				ModularWidget(name)
		{
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			
			//build HashMap for scoring function advanced option dialogs
			addScoringFunction("Default", DEFAULT);
			addScoringFunction("Random", RANDOM);
		
			hide();
		}
		
		//Destructor	
		DockResultDialog::~DockResultDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockResultDialog" << std::endl;
			#endif 
		}
		
		// add scoring function to HashMap and ComboBox
		void DockResultDialog::addScoringFunction(QString name, int score_func, QDialog* dialog)
			throw()
		{
			if(dialog)
			{
				scoring_dialogs_[score_func] = dialog;
			}
			scoring_functions->insertItem(name, score_func);
		}
		
		// add docked system to BALLView structures 
		void DockResultDialog::displayDockedSystem()
			throw()
		{
			SnapShot best_result = conformation_set_[0];
			
			docked_system_ = new System(conformation_set_.getSystem());
			best_result.applySnapShot(*docked_system_);
			
			Log.info() << "ResultDialog before insert" << std::endl;
			getMainControl()->insert(*docked_system_);
			Log.info() << "ResultDialog after insert" << std::endl;
			//getMainControl()->update(*docked_system_, true);
		
		}

				
		// ------------------------- SLOTS ------------------------------------------------
		// --------------------------------------------------------------------------------

		/// Show and raise
		void DockResultDialog::show()
		{
			// get the conformations (= pair<Index, float> = Snapshot number & energy value) for the result dialog
			std::vector<ConformationSet::Conformation> conformations = conformation_set_.getScoring();
			
			//clear table
			int num_rows = result_table->numRows();
			for(int j = num_rows; j >=0; j--)
			{
				result_table->removeRow(j);
			}
			
			// fill the table of the result dialog
			result_table->insertRows(0,conformations.size());
			
			for(unsigned int i = 0; i < conformations.size() ; i++)
			{
				//1.column = snapshot number; 2.column = energy value 
				QString string;
				result_table->setText(i,0,string.setNum(conformations[i].first));
				result_table->setText(i,1,string.setNum(conformations[i].second));
			}
			
			result_table->horizontalHeader()->setLabel(1, scoring_name_);
			
			//adjust column width
			for(int j = 0; j < result_table->numCols() ; j++)
			{
				result_table->adjustColumn(j);
			}
			//show dialog to user
			DockResultDialogData::show();
		}
		
		///
		void DockResultDialog::showSnapshot()
		{
			int selected_row = result_table->currentRow();
			//Log.info() << "current Row: " << i << std::endl;
			int snapshot = (result_table->text(selected_row,0)).toInt();
					
			SnapShot selected_conformation = conformation_set_[snapshot];
			
			selected_conformation.applySnapShot(*docked_system_);
			getMainControl()->update(*docked_system_, true);
		}
		
		//
		void DockResultDialog::upwardClicked()
		{
			int selected_row = result_table->currentRow();
			if(selected_row > 0)
			{
				result_table->selectRow(selected_row-1);
				showSnapshot();
			}
		}
				
		//
		void DockResultDialog::downwardClicked()
		{
			int selected_row = result_table->currentRow();
			if(selected_row < result_table->numRows()-1)
			{
				result_table->selectRow(selected_row+1);
				showSnapshot();
			}
		}
		
		//
		void DockResultDialog::advancedClicked()
		{
			int index = scoring_functions->currentItem();
			if(index)
			{
				scoring_dialogs_[index]->exec();
			}
		}
					
		//
		void DockResultDialog::scoringClicked()
		{
			// create scoring function object
			EnergeticEvaluation* scoring = 0;
			//check which scoring function is chosen
			int index = scoring_functions->currentItem();
			switch(index)
			{
				case DEFAULT:
					scoring = new EnergeticEvaluation();
					break;
				case RANDOM:
					scoring = new RandomEvaluation();
					break;
			}
			
			// score the results of the docking algorithm
			std::vector<ConformationSet::Conformation> ranked_conformations = (*scoring)(conformation_set_);
			conformation_set_.setScoring(ranked_conformations);
			
			
		}
		
		//
		void DockResultDialog::scoringFuncChosen()
		{
			int index = scoring_functions->currentItem();
			if(scoring_dialogs_.has(index))
			{
				advanced_button->setEnabled(true);
			}
			else
			{
				advanced_button->setEnabled(false);
			}
		}
	}
}
