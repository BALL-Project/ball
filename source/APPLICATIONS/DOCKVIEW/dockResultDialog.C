// $Id: dockResultDialog.C,v 1.1.2.18 2005/05/05 13:22:16 haid Exp $
//

#include "dockResultDialog.h"

#include <qtable.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtextedit.h>

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/amberEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>

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
			//setWFlags(Qt::WStyle_MinMax | Qt::WStyle_SysMenu);
		
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			
			//build HashMap and ComboBox for scoring function and its advanced option dialog
			//make sure the order of added scoring functions is consistent to the enum order
			//because the scoring function with enum value i should be at position i in the Combobox
			//otherwise you get the wrong option dialog for a scoring function
			addScoringFunction("Default", DEFAULT);
			addScoringFunction("Amber Force Field", AMBER_FF, &(MolecularStructure::getInstance(0)->getAmberConfigurationDialog()));
			addScoringFunction("Random", RANDOM);
		
			// signals and slots connections
    	QHeader* columns = result_table->horizontalHeader();
			//the table should be sorted by a column, when this column is clicked
			connect( columns, SIGNAL( clicked(int) ), this, SLOT( sortTable(int) ) );
			
			info_dialog_ = new InfoDialog(this);
			
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
		
		// Assignment operator
		const DockResultDialog& DockResultDialog::operator =(const DockResultDialog& res_dialog)
			throw()
		{
			if (&res_dialog != this)
			{
				dock_res_ = res_dialog.dock_res_;
				docked_system_ = res_dialog.docked_system_;
				scoring_dialogs_ = res_dialog.scoring_dialogs_;
			}
			return *this;
		}
		
		void DockResultDialog::setDockResult(DockResult* dock_res)
			throw()
		{
			dock_res_ = dock_res;
		}
		
		void DockResultDialog::setDockedSystem(System* system)
			throw()
		{
			docked_system_ = system;
		}
		
		// Adds scoring function to Combobox and its advanced option dialogs to HashMap, if it has such an dialog.
		void DockResultDialog::addScoringFunction(const QString& name, const int score_func, QDialog* dialog)
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

		// show and raise result dialog
		void DockResultDialog::show()
		{
			Log.info() << "in DockeResultDialog::show()" << std::endl;
			// before showing the dialog the result table has to be build and filled 
			// first get the number of conformations, to know how many rows the table needs
			int conformation_num = dock_res_->getConformationSet()->size();
			// insert rows in table
			result_table->insertRows(0,conformation_num);
		
			// fill the first column with snapshot numbers
			for(int i=0; i < conformation_num; i++)
			{
				QString s;
				result_table->setText(i,0,s.setNum(i));
			}
			
			// fill table with scores
			for(unsigned int i = 0; i < dock_res_->numberOfScorings(); i++)
			{
				// insert new score column in table; i+1, because first column contains snapshot number
				result_table->insertColumns(i+1, 1);
				// set the scoring function name as label of the column
				result_table->horizontalHeader()->setLabel(i+1, dock_res_->getScoringName(i));
				// the scores in the vector are sorted by snapshot number!
				// the score with snapshot number i is at position i in the vector
				vector<float> scores = dock_res_->getScores(i);
				for(unsigned int j = 0; j < scores.size(); j++)
				{
					QString s;
					result_table->setText(j, i+1, s.setNum(scores[j]));
				}
			}
			
			// adjust column width
			for(int j = 0; j < result_table->numCols() ; j++)
			{
				result_table->adjustColumn(j);
			}
			// sort by first score column
			sortTable(1);
			// adjust table/dialog size
			result_table->adjustSize();
			adjustSize();
			// show dialog to user
			DockResultDialogData::show();
			Log.info() << "DockeResultDialog::show() finished" << std::endl;
		}
		
		// show snapshot of selected row
		void DockResultDialog::showSnapshot()
		{
			// get index of current row
			int selected_row = result_table->currentRow();
			// get snapshot number of this row
			int snapshot = (result_table->text(selected_row,0)).toInt();
			// apply snapshot
			const ConformationSet* conformation_set = dock_res_->getConformationSet();
			SnapShot selected_conformation = (*conformation_set)[snapshot];
			selected_conformation.applySnapShot(*docked_system_);
			//inform main control that system has changed
			getMainControl()->update(*docked_system_, true);
		}
				
		// select and show the entry above the current selected entry
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
		
		// sets the advanced button enabled if the selected scoring function has options
		// otherwise the button is disabled 
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
		
		// show options dialog of selected scoring function
		void DockResultDialog::advancedClicked()
		{
			int index = scoring_functions->currentItem();
			if(index)
			{
				scoring_dialogs_[index]->exec();
			}
		}
		
		// calculate new scores with the chosen scoring function and add a new score column, 
		// the table is sorted by this new column
		void DockResultDialog::scoringClicked()
		{
			// create scoring function object
			EnergeticEvaluation* scoring = 0;
			Options scoring_options;  ///////////////////////////////TODO mit new anlegen? ///////////////////////////////
			
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
				case AMBER_FF:
				{
					AmberFF& ff = MolecularStructure::getInstance(0)->getAmberFF();
					AmberConfigurationDialog* dialog = RTTI::castTo<AmberConfigurationDialog>(*(scoring_dialogs_[index]));
					//now the Amber force field gets its options
					dialog->applyTo(ff);
					Log.info() << "in DockResultDialog:: Option of Amber FF:" << std::endl;
					Options::Iterator it = ff.options.begin();
					for(; +it; ++it)
					{
						Log.info() << it->first << " : " << it->second << std::endl;
					}
					scoring_options = ff.options;
					//the force field is given to the AmberEvaluation (scoring function) object
					scoring = new AmberEvaluation(ff);
					break;
				}	
			}
			
			// apply scoring function; set new scores in the conformation set
			ConformationSet* conformation_set = dock_res_->getConformationSet();
			std::vector<ConformationSet::Conformation> ranked_conformations = (*scoring)(*conformation_set);
			Log.info() << "in DockResultDialog:: " << std::endl;
			conformation_set->setScoring(ranked_conformations);
//   			dock_res_->setConformationSet(conformation_set);
			
			// add a new scoring to dock_res_; we need the name, options and score vector of the scoring function
			vector<float> scores;
			for (unsigned int i = 0; i < ranked_conformations.size(); i++)
			{
				scores.push_back(ranked_conformations[i].second);
			}
			dock_res_->addScoring(String(scoring_functions->currentText().ascii()), scoring_options, scores);
			
			/*
			// before filling the table clear it
			result_table->setSelectionMode(QTable::Multi);
			result_table->selectCells(0,0,result_table->numRows()-1, result_table->numCols()-1);
			result_table->clearSelection();
			result_table->setSelectionMode(QTable::SingleRow);
			*/
			
			// before filling the table with a new score column, sort table by snapshot number
			// because the scores in the vector are also sorted by them
			sortTable(0);
			
			// add new column to the table of the result dialog, where the new scores are shown
			int num_column = result_table->numCols();
			result_table->insertColumns(num_column,1);
			result_table->horizontalHeader()->setLabel(num_column, scoring_functions->currentText());
			
			// fill new column
			for(unsigned int i = 0; i < scores.size(); i++)
			{
				QString s;
				result_table->setText(i, num_column, s.setNum(scores[i]));
			}
			
			// sort by new column
			sortTable(num_column);
			
			// adjust column width
			result_table->adjustColumn(num_column);
			
			// adjust the table/dialog size
			result_table->adjustSize();
			adjustSize();
			
			// delete instances 
			if (scoring != NULL)
			{
				delete scoring;
				scoring = NULL;
			}
			
		}
		
		// sort the result table by clicked column
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
			//now the current selected row can be different,
			//so call showSnapshot() of current selected row
			showSnapshot();
		}
		
		void DockResultDialog::showDockingOptions()
		{
			// clear and refill info dialog
			info_dialog_->info_box->clear();
			QString s = "Algorithm: ";
			info_dialog_->info_box->append(s.append(dock_res_->getDockingAlgorithm()));
			info_dialog_->info_box->append("*** Options of algorithm ***");
			s = "number of best docked structures: ";
			const Options& alg_opt = dock_res_->getDockingOptions();
			info_dialog_->info_box->append(s.append(alg_opt.get(GeometricFit::Option::BEST_NUM)));
			Options::ConstIterator it = alg_opt.begin();
			for(; +it; ++it)
			{
				s = it->first;
				s.append(" : ");
				info_dialog_->info_box->append(s.append(it->second));
			}
			//show dialog
			info_dialog_->show();
		}
		
		void DockResultDialog::contextMenuRequested(int row, int column, const QPoint& pos)
		{
				QPopupMenu context_menu;
				Position menu_entry_pos; 
				menu_entry_pos = context_menu.insertItem("Delete Score Column", this, SLOT(deleteColumn_(int)));
				if (!column) context_menu.setItemEnabled(menu_entry_pos, false);
				menu_entry_pos = context_menu.insertItem("Scoring Options", this, SLOT(showScoringOptions_(int)));
				if (!column) context_menu.setItemEnabled(menu_entry_pos, false);
				context_menu.insertItem("Redock", this, SLOT(redock_(int)));
				context_menu.exec(pos);
		}
		
		void DockResultDialog::deleteColumn_(int column)
		{
			result_table->removeColumn(column);
			dock_res_->deleteScoring(column-1);
				
			// adjust the table/dialog size
			result_table->adjustSize();
			adjustSize();
		}
		
		void DockResultDialog::showScoringOptions_(int column)
		{
			Log.info() << "in showScoringOptions " << column << std::endl;
			// clear and refill info dialog
			info_dialog_->info_box->clear();
			QString s = "Scoring function: ";
			info_dialog_->info_box->append(s.append(dock_res_->getScoringName(column-1)));
			info_dialog_->info_box->append("*** Options of scoring function ***");
			const Options& scoring_opt = dock_res_->getScoringOptions(column-1);
			Options::ConstIterator it = scoring_opt.begin();
			for(; +it; ++it)
			{
				s = it->first;
				s.append(" : ");
				info_dialog_->info_box->append(s.append(it->second));
			}
			//show dialog
			info_dialog_->show();
		}
		
		void DockResultDialog::redock_(int row)
		{
		
		}
		
		/*implementation of nested class Compare_		
		*/
		// default constructor
		DockResultDialog::Compare_::Compare_() throw()
		{}
		
		// constructor
		DockResultDialog::Compare_::Compare_(int index) throw()
		{ index_ = index; }
		
		// destructor
		DockResultDialog::Compare_::~Compare_() throw()
		{}
			
		// operator ()
		bool DockResultDialog::Compare_::operator() (const vector<float>& a, const vector<float>& b) const
			throw()
		{ return a[index_] < b[index_]; }
	
	} // end of namespace VIEW
} // end of namespace BALL
