// $Id: dockResultDialog.C,v 1.1.2.10 2005/04/01 14:29:13 haid Exp $
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
		
		// Constructor
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
		
		// Destructor
		DockResultDialog::~DockResultDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockResultDialog" << std::endl;
			#endif 
		}
		
		/** Assignment operator
		*/
		const DockResultDialog& DockResultDialog::operator =(const DockResultDialog& res_dialog)
		{
			if (&res_dialog != this)
			{
				dock_res_ = res_dialog.dock_res_;
				scoring_dialogs_ = res_dialog.scoring_dialogs_;
			}
			return *this;
		}
		
		// add scoring function to ComboBox and its options dialog to HashMap
		void DockResultDialog::addScoringFunction(QString name, int score_func, QDialog* dialog)
			throw()
		{
			if(dialog)
			{
				// add dialog to HashMap
				scoring_dialogs_[score_func] = dialog;
			}
			// add to ComboBox
			scoring_functions->insertItem(name, score_func);
		}
		
		// --------------------------------- SLOTS ------------------------------------------------
		// ----------------------------------------------------------------------------------------

		// Show and raise result dialog
		void DockResultDialog::show()
		{
			// get the conformations (= pair<Index, float> = Snapshot number & energy value) for the result dialog
			std::vector<ConformationSet::Conformation> conformations = conformation_set_.getScoring();
			
			// fill the table of the result dialog
			result_table->insertRows(0,conformations.size());
			
			for(unsigned int i = 0; i < conformations.size() ; i++)
			{
				// 1.column = snapshot number; 2.column = energy value 
				QString s;
				result_table->setText(i,0,s.setNum(conformations[i].first));
				result_table->setText(i,1,s.setNum(conformations[i].second));
			}
			// set the scoring function name as label of the column
			result_table->horizontalHeader()->setLabel(1, scoring_name_);
			
			// adjust column width
			for(int j = 0; j < result_table->numCols() ; j++)
			{
				result_table->adjustColumn(j);
			}
			// sort by score column
			sortTable(1);
			
			// adjust table/dialog size
			result_table->adjustSize();
			adjustSize();
			
			// show dialog to user
			DockResultDialogData::show();
		}
		
		// shows snapshot of selected row
		void DockResultDialog::showSnapshot()
		{
			// get index of current row
			int selected_row = result_table->currentRow();
			// get snapshot number
			int snapshot = (result_table->text(selected_row,0)).toInt();
			// apply snapshot
			SnapShot selected_conformation = conformation_set_[snapshot];
			System& s = conformation_set_.getSystem();
			selected_conformation.applySnapShot(s);
			getMainControl()->update(s, true);
		}
		
		// selects and shows the entry above the current selected entry
		void DockResultDialog::upwardClicked()
		{
			int selected_row = result_table->currentRow();
			if(selected_row > 0)
			{
				result_table->selectRow(selected_row-1);
				showSnapshot();
			}
		}
		
		// selects and shows the entry below the current selected entry
		void DockResultDialog::downwardClicked()
		{
			int selected_row = result_table->currentRow();
			if(selected_row < result_table->numRows()-1)
			{
				result_table->selectRow(selected_row+1);
				showSnapshot();
			}
		}
		
		// shows options dialog of selected scoring function
		void DockResultDialog::advancedClicked()
		{
			int index = scoring_functions->currentItem();
			if(index)
			{
				scoring_dialogs_[index]->exec();
			}
		}
		
		// adds a column which contains the sorted scores after reranking
		void DockResultDialog::scoringClicked()
		{
			// create scoring function object
			EnergeticEvaluation* scoring = 0;
			// check which scoring function is chosen
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
			
			// add new score column to score vector scores_
			vector<float> score;
			for (unsigned int i = 0; i < ranked_conformations.size(); i++)
			{
				score.push_back(ranked_conformations[i].second);
			}
			addScore(score);
			
			// before filling the table clear it
			result_table->setSelectionMode(QTable::Multi);
			result_table->selectCells(0,0,result_table->numRows()-1, result_table->numCols()-1);
			result_table->clearSelection();
			result_table->setSelectionMode(QTable::SingleRow);
			
			// add new column to the table of the result dialog, where the new scores are shown
			int num_column = result_table->numCols();
			result_table->insertColumns(num_column,1);
			result_table->horizontalHeader()->setLabel(num_column, scoring_functions->currentText());			
			
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
			
			// sort by new column
			sortTable(num_column);
			
			result_table->adjustColumn(num_column);
			
			// adjust the table/dialog size
			//QSize recommended_size = result_table->sizeHint();
			//result_table->resize(recommended_size);
			result_table->adjustSize();
			adjustSize();
		}
		
		// sets the advanced button enabled if the selected scoring function has options
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
		
		// sorts the result table by clicked column
		void DockResultDialog::sortTable(int column)
		{
			// create vector which contains the rows of the table
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
			
			// sort row-vector by the column which the user clicked
			Compare_ compare_func = Compare_(column);
			sort(rows.begin(), rows.end(), compare_func);
			 
			// fill result table
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
