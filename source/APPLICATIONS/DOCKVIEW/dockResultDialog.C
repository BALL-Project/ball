// $Id: dockResultDialog.C,v 1.1.2.3 2005/03/21 16:24:28 haid Exp $
//

#include <qtable.h>
#include <qcombobox.h>
#include <qpushbutton.h>

#include "dockResultDialog.h"

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
			SnapShot best_result = ranked_conformations_[0];
			
			docked_system_ = new System(ranked_conformations_.getSystem());
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
			std::vector<ConformationSet::Conformation> conformations = ranked_conformations_.getScoring();
			
			//clear table
			int num_rows = result_table->numRows();
			for(int j = num_rows; j >=0; j--)
			{
				result_table->removeRow(j);
			}
			
			// fill the table of the result dialog
			result_table->insertRows(0,conformations.size());
			Log.info() << "conformation size: " << conformations.size() << std::endl;
			
			for(unsigned int i = 0; i < conformations.size() ; i++)
			{
				//1.column = snapshot number; 2.column = energy value 
				QString string;
				result_table->setText(i,0,string.setNum(conformations[i].first));
				result_table->setText(i,1,string.setNum(conformations[i].second));
			}
			
			result_table->horizontalHeader()->setLabel(0, scoring_name_);
			
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
		/*
			for(int i=0; i < result_table->numRows(); i++)
			{
				if(result_table->isRowSelected(i))
				{
					int snapshot = (result_table->text(i,0)).toInt();
					//Log.info() << "snapshot: " << snapshot << std::endl;
					SnapShot selected_conformation = ranked_conformations_[snapshot];
			
					selected_conformation.applySnapShot(*docked_system_);
					getMainControl()->update(*docked_system_, true);
					return;
				}
			}
			*/
			
			int selected_row = result_table->currentRow();
			//Log.info() << "current Row: " << i << std::endl;
			int snapshot = (result_table->text(selected_row,0)).toInt();
					
			SnapShot selected_conformation = ranked_conformations_[snapshot];
			
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
