// $Id: dockResultDialog.C,v 1.1.2.6 2005/03/24 10:39:23 haid Exp $
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
		
			// signals and slots connections
    	QHeader* columns = result_table->horizontalHeader();
			connect( columns, SIGNAL( clicked(int) ), this, SLOT( sortTable(int) ) );
			
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
			/*int num_rows = result_table->numRows();
			for(int j = num_rows; j >=0; j--)
			{
				result_table->removeRow(j);
			}*/
			
			// fill the table of the result dialog
			result_table->insertRows(0,conformations.size());
			
			for(unsigned int i = 0; i < conformations.size() ; i++)
			{
				//1.column = snapshot number; 2.column = energy value 
				QString s;
				result_table->setText(i,0,s.setNum(conformations[i].first));
				result_table->setText(i,1,s.setNum(conformations[i].second));
			}
			// set the scoring function name as lable of the column
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
			
			// add new score column to score vector
			vector<float> score;
			for (unsigned int i = 0; i < ranked_conformations.size(); i++)
			{
				score.push_back(ranked_conformations[i].second);
			}
			addScore(score);
			
			//before filling the table clear it
			result_table->setSelectionMode(QTable::Multi);
			result_table->selectCells(0,0,result_table->numRows()-1, result_table->numCols()-1);
			result_table->clearSelection();
			result_table->setSelectionMode(QTable::SingleRow);
			
			// add new column to the table of the result dialog, where the new scores are shown
			int num_column = result_table->numCols();
			result_table->insertColumns(num_column,1);
			result_table->horizontalHeader()->setLabel(num_column, scoring_functions->currentText());
			
			
			//adjust the table size
			
			
			// fill table
			for(int row = 0 ; row < result_table->numRows(); row++)
			{
				QString s;
				result_table->setText(row,0,s.setNum(row));
				for(unsigned int column = 0; column < scores_.size(); column++)
				{
				 result_table->setText(row,column+1,s.setNum(scores_[column][row]));
				}
			}
			
			//sort by new column
			sortTable(num_column);
			
			result_table->adjustColumn(num_column);
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
		
		void DockResultDialog::sortTable(int section)
		{
			//create vector which contains the rows of the table
			vector<vector<float> > rows;
			for(int row_it = 0; row_it < result_table->numRows(); row_it++)
			{
				vector<float> row;
				for(int column_it = 0; column_it < result_table->numCols(); column_it++)
				{
					QString s = result_table->text(row_it,column_it);
					row.push_back(s.toFloat());
				}
				rows.push_back(row);
			}
			
			//sort vector by column section which the user clicked
			Compare_ compare_func = Compare_(section);
			sort(rows.begin(), rows.end(), compare_func);
			 
			//fill table
			for(int row_it = 0; row_it < result_table->numRows(); row_it++)
			{
				QString s;
				int index = (int) rows[row_it][0];
				result_table->setText(row_it,0,s.setNum(index));
				for(int column_it = 1; column_it < result_table->numCols(); column_it++)
				{
					result_table->setText(row_it,column_it,s.setNum(rows[row_it][column_it]));
				}
			}
		}
	}
}
