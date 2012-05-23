// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/dockResultDialog.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>

#include <BALL/VIEW/WIDGETS/molecularStructure.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/SCORING/FUNCTIONS/energeticEvaluation.h>
#include <BALL/SCORING/FUNCTIONS/amberEvaluation.h>
#include <BALL/SCORING/FUNCTIONS/randomEvaluation.h>
#include <BALL/DOCKING/COMMON/dockResult.h>

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QMessageBox>


//#define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{
		
		// Constructor
		DockResultDialog::DockResultDialog(QWidget* parent,  const char* name)
			: QDialog(parent),
				Ui_DockResultDialogData(),
				dock_res_(0),
				docked_system_(0),
				redock_partner1_(0),
				redock_partner2_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.info() << "new DockResultDialog " << this << std::endl;
		#endif

			setupUi(this);
			setWindowTitle(name);
		
			//build HashMap and ComboBox for scoring function and its advanced option dialog
			//make sure the order of added scoring functions is consistent to the enum order
			//because the scoring function with enum value i should be at position i in the Combobox
			//otherwise you get the wrong option dialog for a scoring function
			addScoringFunction("Default", DockingController::DEFAULT);
			MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
			if (!mol_struct)
		  {
		    Log.error() << "Error while filling HashMap for scoring function advanced option dialogs! " 
										<< __FILE__ << " " << __LINE__ << std::endl;
		    return;
		  }
			addScoringFunction("Amber Force Field", DockingController::AMBER_FF, &(mol_struct->getAmberConfigurationDialog()));
			addScoringFunction("Random", DockingController::RANDOM);
		
			result_table->setSortingEnabled(false);
			result_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
			
			hide();

			connect( scoring_functions, SIGNAL( activated(const QString&) ), this, SLOT( scoringFuncChosen() ) );
			connect( advanced_button, SIGNAL( clicked() ), this, SLOT( advancedClicked() ) );
			connect( scoring_button, SIGNAL( clicked() ), this, SLOT( scoringClicked() ) );
			connect( scoring_opt, SIGNAL( clicked()), this, SLOT(showScoringOptions_()));
			connect( delete_score, SIGNAL( clicked()), this, SLOT(deleteColumn_()));
			connect( dock_opt_button, SIGNAL( clicked() ), this, SLOT( showDockingOptions() ) );
			connect( redock, SIGNAL( clicked() ), this, SLOT( redock_() ) );
			connect( show_button, SIGNAL( clicked() ), this, SLOT( showSnapshot() ) );
			connect( upwardButton, SIGNAL( clicked() ), this, SLOT( upwardClicked() ) );
			connect( downwardButton, SIGNAL( clicked() ), this, SLOT( downwardClicked() ) );
			connect( close_button, SIGNAL( clicked() ), this, SLOT( closeClicked() ) );

			connect(result_table->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortTable(int)));
			connect(result_table, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged_()));
		}
		
		// Copy constructor.
		DockResultDialog::DockResultDialog(const DockResultDialog& dock_res_dialog)
			: QDialog(),
				Ui_DockResultDialogData(),
				dock_res_(dock_res_dialog.dock_res_),
				docked_system_(dock_res_dialog.docked_system_),
				redock_partner1_(dock_res_dialog.redock_partner1_),
				redock_partner2_(dock_res_dialog.redock_partner2_),
				scoring_dialogs_(dock_res_dialog.scoring_dialogs_)
		{}
		
		// Destructor
		DockResultDialog::~DockResultDialog()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockResultDialog" << std::endl;
			#endif 
			
			// remark: dockResult is deleted by DatasetControl
			//				 docked system is deleted by MainControl
			if (redock_partner1_) delete redock_partner1_;
			if (redock_partner2_) delete redock_partner2_;
		}
		
		// Assignment operator
		const DockResultDialog& DockResultDialog::operator =(const DockResultDialog& res_dialog)
		{
			if (&res_dialog == this) return *this;
			
			dock_res_ = res_dialog.dock_res_;
			docked_system_ = res_dialog.docked_system_;
			scoring_dialogs_ = res_dialog.scoring_dialogs_;
			
			if (redock_partner1_) delete redock_partner1_;
			redock_partner1_ = res_dialog.redock_partner1_;
			
			if (redock_partner2_) delete redock_partner2_;
			redock_partner2_ = res_dialog.redock_partner2_;
		
			return *this;
		}
		
		void DockResultDialog::setDockResult(DockResult* dock_res)
		{
			dock_res_ = dock_res;
			if (dock_res == 0) hide();
		}
		
		void DockResultDialog::setDockedSystem(System* system)
		{
			docked_system_ = system;
		}
		
		// Adds scoring function to Combobox and its advanced option dialogs to HashMap, if it has such an dialog.
		void DockResultDialog::addScoringFunction(const QString& name, DockingController::ScoringFunction score_func, 
																							QDialog* dialog)
		{
			if (dialog)
			{
				// add dialog to HashMap
				scoring_dialogs_[score_func] = dialog;
			}
			// add to ComboBox
			scoring_functions->addItem(name, score_func);
		}
		
		// --------------------------------- SLOTS ------------------------------------------------
		// ----------------------------------------------------------------------------------------

		// show and raise result dialog
		void DockResultDialog::show()
		{
			MainControl* main_control = VIEW::getMainControl();
			if (!main_control || main_control->isBusy())
			{
				BALLVIEW_DEBUG;
				return;
			}

			if (!dock_res_) return;
			
			// before showing the dialog the result table has to be build and filled 
			// first get the number of conformations, to know how many rows the table needs
			Size conformation_num = dock_res_->getConformationSet()->size();
			// insert rows in table
			result_table->clear();
			result_table->setRowCount(conformation_num);
			result_table->setColumnCount(dock_res_->numberOfScorings() + 1);

			result_table->setHorizontalHeaderItem(0, new QTableWidgetItem("Snapshot"));
		
			// fill the first column with snapshot numbers
			for (Position i=0; i < conformation_num; i++)
			{
				QString s;
				result_table->setItem(i, 0, new QTableWidgetItem(s.setNum(i)));
			}
			
			// fill table with scores
			for (Position i = 0; i < dock_res_->numberOfScorings(); i++)
			{
				result_table->setHorizontalHeaderItem(i + 1, new QTableWidgetItem(dock_res_->getScoringName(i).c_str()));
				// the scores in the vector are sorted by snapshot number!
				// the score with snapshot number i is at position i in the vector
				vector<ConformationSet::Conformation> scores = dock_res_->getScores(i);
				for(Position j = 0; j < scores.size(); j++)
				{
					QString s;
					result_table->setItem(j, i + 1, new QTableWidgetItem(s.setNum(scores[j].second)));
				}
			}
			
			// sort by first score column
			sortTable(1);

			QDialog::show();
		}

		
		// show snapshot of selected row
		void DockResultDialog::showSnapshot()
		{
			// get index of current row
			Index selected_row = result_table->currentRow();
			if (selected_row == -1) return;

			// get snapshot number of this row
			Index snapshot = (result_table->item(selected_row, 0)->text()).toInt();

			// apply snapshot
			const ConformationSet* conformation_set = dock_res_->getConformationSet();
			const SnapShot& selected_conformation = (*conformation_set)[snapshot];
			selected_conformation.applySnapShot(*docked_system_);

			//inform main control that system has changed
			VIEW::getMainControl()->update(*docked_system_, true);
		}
				
		// select and show the entry above the current selected entry
		void DockResultDialog::upwardClicked()
		{
			Index selected_row = result_table->currentRow();
			if (selected_row > 0)
			{
				result_table->selectRow(selected_row-1);
				showSnapshot();
			}
		}
		
		// selects and shows the entry below the current selected entry
		void DockResultDialog::downwardClicked()
		{
			Index selected_row = result_table->currentRow();
			if (selected_row < result_table->rowCount() - 1)
			{
				result_table->selectRow(selected_row + 1);
				showSnapshot();
			}
		}
		
		// sets the advanced button enabled if the selected scoring function has options
		// otherwise the button is disabled 
		void DockResultDialog::scoringFuncChosen()
		{
			Index index = scoring_functions->currentIndex();
			if (scoring_dialogs_.has(index))
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
			Index index = scoring_functions->currentIndex();
			if (index)
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
			Options scoring_options;
			
			// check which scoring function is chosen
			Index index = scoring_functions->currentIndex();
			switch(index)
			{
			  case DockingController::DEFAULT:
			    scoring = new EnergeticEvaluation();
			    break;
			  case DockingController::RANDOM:
			    scoring = new RandomEvaluation();
			    break;
			  case DockingController::AMBER_FF:
				{
					MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
					if (!mol_struct)
					{
						Log.error() << "Error while rescoring with AMBER_FF! " << __FILE__ << " " << __LINE__ << std::endl;
						return;
					}
					AmberFF& ff = mol_struct->getAmberFF();
					AmberConfigurationDialog* dialog = RTTI::castTo<AmberConfigurationDialog>(*(scoring_dialogs_[index]));
					// now the Amber force field gets its options
					dialog->applyTo(ff);
					scoring_options = ff.options;
					// the force field is given to the AmberEvaluation (scoring function) object
					scoring = new AmberEvaluation(ff);
					break;
				}
			}
			
			if (!scoring) return;
			
			// apply scoring function; set new scores in the conformation set
			ConformationSet* conformation_set = dock_res_->getConformationSet();
			vector<ConformationSet::Conformation> ranked_conformations = (*scoring)(*conformation_set);
			conformation_set->setScoring(ranked_conformations);
			
			// sort vector ranked_conformations by snapshot numbers
			sort(ranked_conformations.begin(), ranked_conformations.end());
			
			// add a new scoring to dock_res_; we need the name, options and score vector of the scoring function
			dock_res_->addScoring(ascii(scoring_functions->currentText()), scoring_options, ranked_conformations);
			
			// before filling the table with a new score column, sort table by snapshot number
			// because the scores in the vector are also sorted by them
			sortTable(0);
			
			// add new column to the table of the result dialog, where the new scores are shown
			Size num_column = result_table->columnCount();
			result_table->setColumnCount(num_column + 1);
			result_table->setHorizontalHeaderItem(num_column, new QTableWidgetItem());
 			result_table->horizontalHeaderItem(num_column)->setText(scoring_functions->currentText());
			
			// fill new column
			for (Position i = 0; i < ranked_conformations.size(); i++)
			{
				QString s;
				result_table->setItem(i, num_column, new QTableWidgetItem(s.setNum(ranked_conformations[i].second)));
			}
			
			// sort by new column
			sortTable(num_column);
			
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
			for(Index row_it = 0; row_it < result_table->rowCount(); row_it++)
			{
				vector<float> row;
				for(Index column_it = 0; column_it < result_table->columnCount(); column_it++)
				{
					QString s = result_table->item(row_it, column_it)->text();
					row.push_back(s.toFloat());
				}
				rows.push_back(row);
			}
			// sort row-vector by the column which the user clicked
			Compare_ compare_func = Compare_(column);
			sort(rows.begin(), rows.end(), compare_func);
			// fill result table
			for (Index row_it = 0; row_it < result_table->rowCount(); row_it++)
			{
				QString s;
				// snapshot number isn't a float!
				int index = (int) rows[row_it][0];
				result_table->item(row_it, 0)->setText(s.setNum(index));
				for(Index column_it = 1; column_it < result_table->columnCount(); column_it++)
				{
					result_table->item(row_it, column_it)->setText(s.setNum(rows[row_it][column_it]));
				}
			}
			//now the current selected row can be different,
			//so call showSnapshot() of current selected row
			showSnapshot();
		}
		
		void DockResultDialog::showDockingOptions()
		{
			QString text = "Algorithm: ";
			text.append(dock_res_->getDockingAlgorithm().c_str());
			text.append("\n\n*** Options of algorithm ***\n");
			const Options& alg_opt = dock_res_->getDockingOptions();
			Options::ConstIterator it = alg_opt.begin();
			for(; it != alg_opt.end(); ++it)
			{
				text.append(it->first.c_str());
				text.append(" : ");
				text.append(it->second.c_str());
				text.append("\n");
			}
			
			QMessageBox mb("Docking Options", text, 
										QMessageBox::NoIcon, 
										QMessageBox::Ok, 
										QMessageBox::NoButton, 
										QMessageBox::NoButton);
			mb.exec();
		}
		

		// closes the dialog
		void DockResultDialog::closeClicked()
		{
			// close dialog and destroy it
			close();
		}
		
		// deletes chosen score column
		void DockResultDialog::deleteColumn_()
		{
			Index column = result_table->currentColumn();
			if (column <= 0) return;

			result_table->removeColumn(column);
			dock_res_->deleteScoring(column-1);
		}
		
		// opens new dialog with scoring options
		void DockResultDialog::showScoringOptions_()
		{
			Index column = result_table->currentColumn();
			if (column <= 0) return;

			QString text = "Scoring function: ";
			text.append(dock_res_->getScoringName(column - 1).c_str());
			const Options& scoring_opt = dock_res_->getScoringOptions(column-1);
			if(scoring_opt.isEmpty())
			{
			 	text.append("\n\nThere are no options.");
			}
			else
			{
				text.append("\n\n*** Options of scoring function ***\n");
				Options::ConstIterator it = scoring_opt.begin();
				for(; it != scoring_opt.end(); ++it)
				{
					text.append(it->first.c_str());
					text.append(" : ");
					text.append(it->second.c_str());
					text.append("\n");
				}
			}
			
			QMessageBox mb("Scoring Options", text, 
										QMessageBox::NoIcon, 
										QMessageBox::Ok, 
										QMessageBox::NoButton, 
										QMessageBox::NoButton);
			mb.exec();
		}
		 
		// 
		void DockResultDialog::redock_()
		{
			Index row = result_table->currentRow();
			if (row < 0) return;

			// get snapshot number of this row
			int snapshot = result_table->item(row, 0)->text().toInt();
			// apply snapshot
			const ConformationSet* conformation_set = dock_res_->getConformationSet();
			SnapShot selected_conformation = (*conformation_set)[snapshot];
			selected_conformation.applySnapShot(*docked_system_);
			
			// split docked system into its original docking partners
			if (redock_partner1_) delete redock_partner1_;
			if (redock_partner2_) delete redock_partner2_;
			redock_partner1_ = new System(*docked_system_);
			redock_partner2_ = new System(*docked_system_);
			
			redock_partner1_->setName(docked_system_->getName());
			redock_partner2_->setName("rd");
			
			AtomContainerIterator it = redock_partner1_->beginAtomContainer();
			++it;
			for ( ; +it; ++it)
			{
				if (!it->hasProperty("DOCKING_PARTNER_1"))
				{
					it->getParent()->removeChild(*it);
					delete &*it;
				}
			}
			
			it = redock_partner2_->beginAtomContainer();
			++it;
			for ( ; +it; ++it)
			{
				if (!it->hasProperty("DOCKING_PARTNER_2"))
				{
					it->getParent()->removeChild(*it);
					delete &*it;
				}
			}
			DockingController* dock_control = DockingController::getInstance(0);
			if (!dock_control)
			  {
			    Log.error() << "Error while getting instance of DockingController! " << __FILE__ << " " << __LINE__ << std::endl;
			    return;
			  }
			DockDialog& dialog = dock_control->getDockDialog(); 
			dialog.setSystems(redock_partner1_, redock_partner2_);
			
			// run redocking
			dock_control->runDocking(true);
		}
		
		/*implementation of nested class Compare_		
		*/
		// default constructor
		DockResultDialog::Compare_::Compare_()
		{}
		
		// constructor
		DockResultDialog::Compare_::Compare_(Position index)
		{ index_ = index; }
		
		// destructor
		DockResultDialog::Compare_::~Compare_()
		{}
			
		// operator ()
		bool DockResultDialog::Compare_::operator() (const vector<float>& a, const vector<float>& b) const
		{ return a[index_] < b[index_]; }
	

		void DockResultDialog::selectionChanged_()
		{
			scoring_opt->setEnabled(true);
			delete_score->setEnabled(true);
			redock->setEnabled(true);
			show_button->setEnabled(true);

			QTableWidgetItem* item = result_table->currentItem();
			if (!item)
			{
				scoring_opt->setEnabled(false);
				delete_score->setEnabled(false);
				redock->setEnabled(false);
				show_button->setEnabled(false);
				return;
			}

			Index col = result_table->column(item);
			if (col <= 0)
			{
				scoring_opt->setEnabled(false);
				delete_score->setEnabled(false);
			}

			Index row = result_table->row(item);
			if (row < 0)
			{
				redock->setEnabled(false);
				show_button->setEnabled(false);
			}
		}

	} // end of namespace VIEW
} // end of namespace BALL
