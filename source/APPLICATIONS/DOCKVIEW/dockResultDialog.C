// $Id: dockResultDialog.C,v 1.1.2.12 2005/04/06 15:25:57 leonhardt Exp $
//

#include <qtable.h>
#include <qcombobox.h>
#include <qpushbutton.h>

#include "dockResultDialog.h"

#ifndef BALL_STRUCTURE_DOCKING_ENERGETICEVALUATION_H
# include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_AMBEREVALUATION_H
# include <BALL/STRUCTURE/DOCKING/amberEvaluation.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_RANDOMEVALUATION_H
# include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#endif

#ifndef BALL_VIEW_DIALOGS_AMBERCONFIGURATIONDIALOG_H
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_GEOMETRICFIT_H
# include <BALL/STRUCTURE/DOCKING/geometricFit.h>
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
			//setWFlags(Qt::WStyle_MinMax | Qt::WStyle_SysMenu);
		
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			
			//build HashMap for scoring function advanced option dialogs
			addScoringFunction("Default", DEFAULT);
			addScoringFunction("Random", RANDOM);
			AmberConfigurationDialog* amber = new AmberConfigurationDialog(this); 
			addScoringFunction("Amber Force Field", AMBER_FF, amber);
			
		
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
		{dock_res_ = dock_res;}
		
		void DockResultDialog::setDockedSystem(System* system)
			throw()
		{docked_system_ = system;}
		
		// add scoring function to ComboBox and its options dialog to HashMap
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

		// Show and raise result dialog
		void DockResultDialog::show()
		{
			// get the number of conformations, to know how many rows the table needs
			Options dock_options = dock_res_->getDockingOptions();
			int conformation_num = dock_options.getInteger(GeometricFit::Option::BEST_NUM);
			// insert rows in table
			result_table->insertRows(0,conformation_num);
			
			// fill the first column with snapshot numbers
			for(int i=0; i < conformation_num; i++)
			{
				QString s;
				result_table->setText(i,0,s.setNum(i));
			}
			
			// fill table with scores
			// 
			for(unsigned int i = 0; i < dock_res_->numberOfScorings(); i++)
			{
				// insert new score column in table; i+1, because first column contains snapshot number
				result_table->insertColumns(i+1, 1);
				
				// set the scoring function name as label of the column
				result_table->horizontalHeader()->setLabel(i+1, dock_res_->getScoringName(i));
				
				/* the scores in the vector are sorted by snapshot number!
					 the score with snapshot number i is at position i in the vector
				*/
				vector<float> scores = dock_res_->getScores(i);
				for(unsigned int j = 0; j < scores.size(); j++)
				{
					QString s;
					result_table->setText(j, i+1, s.setNum(scores[j]));
				}
			}
			
			/*
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
			*/
			
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
		}
		
		// shows snapshot of selected row
		void DockResultDialog::showSnapshot()
		{
			// get index of current row
			int selected_row = result_table->currentRow();
			// get snapshot number
			int snapshot = (result_table->text(selected_row,0)).toInt();
			// apply snapshot
			ConformationSet conformation_set = dock_res_->getConformationSet();
			SnapShot selected_conformation = conformation_set[snapshot];
			//System& s = conformation_set.getSystem();
			selected_conformation.applySnapShot(*docked_system_);
			getMainControl()->update(*docked_system_, true);
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
			AmberFF* ff = 0;
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
					ff = new AmberFF();
					AmberConfigurationDialog* dialog = RTTI::castTo<AmberConfigurationDialog>(*(scoring_dialogs_[index]));
					dialog->applyTo(*ff);
					Log.info() << "in DockDialog:: Option of Amber FF:" << std::endl;
					Options::Iterator it = ff->options.begin();
					for(; +it; ++it)
					{
						Log.info() << it->first << " : " << it->second << std::endl;
					}
					scoring = new AmberEvaluation(*ff);
					break;
				}	
			}
			
			// apply scoring function; set new scores in the conformation set
			ConformationSet conformation_set = dock_res_->getConformationSet();
			std::vector<ConformationSet::Conformation> ranked_conformations = (*scoring)(conformation_set);
			conformation_set.setScoring(ranked_conformations);
			dock_res_->setConformationSet(conformation_set);
			
			// to add a new scoring to dock_res_ we need the name, options and score vector of the scoring function
			vector<float> scores;
			for (unsigned int i = 0; i < ranked_conformations.size(); i++)
			{
				scores.push_back(ranked_conformations[i].second);
			}
			Options options; ////////// !!!!!!!!!!!!!
			dock_res_->addScoring(scoring_functions->currentText(), options, scores);
			
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
			
			/*
			// fill table
			for(int row = 0 ; row < result_table->numRows(); row++)
			{
				QString s;
				result_table->setText(row,0,s.setNum(row));
				for(unsigned int column = 0; column < scores.size(); column++)
				{
				 result_table->setText(row,column+1,s.setNum(scores_[column][row]));
				}
			}
			*/
			
			// fill new column
			for(unsigned int i = 0; i < scores.size(); i++)
			{
				QString s;
				result_table->setText(i, num_column, s.setNum(scores[i]));
			}
			
			// sort by new column
			sortTable(num_column);
			
			result_table->adjustColumn(num_column);
			
			// adjust the table/dialog size
			//QSize recommended_size = result_table->sizeHint();
			//result_table->resize(recommended_size);
			result_table->adjustSize();
			adjustSize();
			
			delete scoring;
			delete ff;
			
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
	}
}
