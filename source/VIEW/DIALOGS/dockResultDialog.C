// $Id: dockResultDialog.C,v 1.4 2006/02/01 13:08:17 leonhardt Exp $
//

#include <BALL/VIEW/DIALOGS/dockResultDialog.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>

#include <BALL/VIEW/WIDGETS/molecularStructure.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/amberEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/dockResult.h>

#include <qtable.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qmessagebox.h>


//#define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{
		
		// Constructor
		DockResultDialog::DockResultDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockResultDialogData(parent, name, modal, fl),
				dock_res_(0),
				docked_system_(0),
				redock_partner1_(0),
				redock_partner2_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.info() << "new DockResultDialog " << this << std::endl;
		#endif
		
			//build HashMap and ComboBox for scoring function and its advanced option dialog
			//make sure the order of added scoring functions is consistent to the enum order
			//because the scoring function with enum value i should be at position i in the Combobox
			//otherwise you get the wrong option dialog for a scoring function
			addScoringFunction("Default", DockingController::DEFAULT);
			MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
			if (!mol_struct)
			  {
			    Log.error() << "Error while filling HashMap for scoring function advanced option dialogs! " << __FILE__ << " " << __LINE__ << std::endl;
			    return;
			  }
			addScoringFunction("Amber Force Field", DockingController::AMBER_FF, &(mol_struct->getAmberConfigurationDialog()));
			addScoringFunction("Random", DockingController::RANDOM);
		
			// signals and slots connections
    	QHeader* columns = result_table->horizontalHeader();
			//the table should be sorted by a column, when this column is clicked
			connect( columns, SIGNAL( clicked(int) ), this, SLOT( sortTable(int) ) );
			
			hide();
		}
		
		// Copy constructor.
		DockResultDialog::DockResultDialog(const DockResultDialog& dock_res_dialog)
			throw()
			: DockResultDialogData(),
				dock_res_(dock_res_dialog.dock_res_),
				docked_system_(dock_res_dialog.docked_system_),
				redock_partner1_(dock_res_dialog.redock_partner1_),
				redock_partner2_(dock_res_dialog.redock_partner2_),
				scoring_dialogs_(dock_res_dialog.scoring_dialogs_)
		{}
		
		// Destructor
		DockResultDialog::~DockResultDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockResultDialog" << std::endl;
			#endif 
			
			// remark: dockResult is deleted by DatasetControl
			//				 docked system is deleted by MainControl
			if(redock_partner1_)
			{
				delete redock_partner1_;
			}
			if(redock_partner2_)
			{
				delete redock_partner2_;
			}
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
				if(redock_partner1_)
				{
					delete redock_partner1_;
				}
				redock_partner1_ = res_dialog.redock_partner1_;
				if(redock_partner2_)
				{
					delete redock_partner2_;
				}
				redock_partner2_ = res_dialog.redock_partner2_;
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
		void DockResultDialog::addScoringFunction(const QString& name, DockingController::ScoringFunction score_func, QDialog* dialog)
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
			if(!dock_res_) return;
			
			// before showing the dialog the result table has to be build and filled 
			// first get the number of conformations, to know how many rows the table needs
			Size conformation_num = dock_res_->getConformationSet()->size();
			// insert rows in table
			result_table->insertRows(0,conformation_num);
			
			// we want table sorted by scoring 0
			dock_res_->sortBy(0);
			// first get order of snapshot indices of scoring 0
			vector<ConformationSet::Conformation> conf = dock_res_->getScores(0);
			
			// fill the first column with snapshot indices
			for(Position i=0; i < conformation_num; i++)
			{
				QString s;
				result_table->setText(i,0,s.setNum(conf[i].first));
			}
		
			// second fill table with scores
			for(Position i = 0; i < dock_res_->numberOfScorings(); i++)
			{
				// insert new score column in table; i+1, because first column contains snapshot number
				result_table->insertColumns(i+1, 1);
				// set the scoring function name as label of the column
				result_table->horizontalHeader()->setLabel(i+1, dock_res_->getScoringName(i));
				// 
				for(Position j = 0; j < conformation_num; j++)
				{
					QString s;
					result_table->setText(j, i+1, s.setNum((*dock_res_)(j,i)));
				}
			}	
			
			// adjust column width
			for(Index j = 0; j < result_table->numCols() ; j++)
			{
				result_table->adjustColumn(j);
			}
			// adjust table/dialog size
			result_table->adjustSize();
			adjustSize();
			// show dialog to user
			DockResultDialogData::show();
		}
		
		// show snapshot of selected row
		void DockResultDialog::showSnapshot()
		{
			if(!dock_res_) return;
		
			// get index of current row
			Index selected_row = result_table->currentRow();
			// get snapshot number of this row
			Index snapshot;
			snapshot = (result_table->text(selected_row, 0)).toInt();
			// apply snapshot
			const ConformationSet* conformation_set = dock_res_->getConformationSet();
			SnapShot selected_conformation = (*conformation_set)[snapshot];
			selected_conformation.applySnapShot(*docked_system_);
			//inform main control that system has changed
			MainControl* main_control = MainControl::getInstance(0);
			if (!main_control)
			{
				Log.error() << "Error while informing MainControl about changed system! " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
			main_control->update(*docked_system_, true);
		}
				
		// select and show the entry above the current selected entry
		void DockResultDialog::upwardClicked()
		{
			Index selected_row = result_table->currentRow();
			if(selected_row > 0)
			{
				result_table->selectRow(selected_row-1);
				showSnapshot();
			}
		}
		
		// selects and shows the entry below the current selected entry
		void DockResultDialog::downwardClicked()
		{
			Index selected_row = result_table->currentRow();
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
			Index index = scoring_functions->currentItem();
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
			Index index = scoring_functions->currentItem();
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
			Options scoring_options;
			
			// check which scoring function is chosen
			Index index = scoring_functions->currentItem();
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
			
			if(!scoring || !dock_res_) return;
			
			// apply scoring function
			ConformationSet* conformation_set = dock_res_->getConformationSet();
			vector<ConformationSet::Conformation> ranked_conformations;
			try
	   	{
				ranked_conformations = (*scoring)(*conformation_set);
			}
			catch(...)
			{
				Log.error() << "Scoring of docking results failed! " << __FILE__ << " " << __LINE__ << std::endl;
				// delete instance 
				if (scoring != NULL)
				{
					delete scoring;
					scoring = NULL;
				}
				return;
			}
			
			// add a new scoring to dock_res_
			dock_res_->addScoring(String(scoring_functions->currentText().ascii()), scoring_options, ranked_conformations);
			
			// add new column to the table of the result dialog, where the new scores are shown
			Size num_column = result_table->numCols();
			result_table->insertColumns(num_column,1);
			result_table->horizontalHeader()->setLabel(num_column, scoring_functions->currentText());
			
			// fill table sorted by the new scoring
			dock_res_->sortBy(dock_res_->numberOfScorings()-1);
			// first get order of snapshot indices of new added scoring 
			vector<ConformationSet::Conformation> conf = dock_res_->getScores(dock_res_->numberOfScorings()-1);
			
			// fill the first column with snapshot indices
			for(Position i=0; i < dock_res_->getConformationSet()->size(); i++)
			{
				QString s;
				result_table->setText(i,0,s.setNum(conf[i].first));
			}
			
			// second fill table with scores
			for(Position i = 0; i < dock_res_->numberOfScorings(); i++)
			{
				for(Position j = 0; j < dock_res_->getConformationSet()->size(); j++)
				{
					QString s;
					result_table->setText(j, i+1, s.setNum((*dock_res_)(j,i)));
				}
			}	
						
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
			// scoring 0 ist is column 1...
			// index -1 corresponds to an ordering by snapshot number
			dock_res_->sortBy(column-1);
		
			// first get order of snapshot indices
			// and fill the first column with snapshot indices
			if(column != 0)
			{
				vector<ConformationSet::Conformation> conf = dock_res_->getScores(column-1);
			
				for(Position i=0; i < dock_res_->getConformationSet()->size(); i++)
				{
					QString s;
					result_table->setText(i,0,s.setNum(conf[i].first));
				}
			}
			else
			{
				for(Position i=0; i < dock_res_->getConformationSet()->size(); i++)
				{
					QString s;
					result_table->setText(i,0,s.setNum(i));
				}
			}
			
			// second fill table with scores
			for(Position i = 0; i < dock_res_->numberOfScorings(); i++)
			{
				for(Position j = 0; j < dock_res_->getConformationSet()->size(); j++)
				{
					QString s;
					result_table->setText(j, i+1, s.setNum((*dock_res_)(j,i)));
				}
			}	
			
			//now the current selected row can be different,
			//so call showSnapshot() of current selected row
			showSnapshot();
		}
		
		void DockResultDialog::showDockingOptions()
		{
			if(!dock_res_) return;
			
			QString text = "Algorithm: ";
			text.append(dock_res_->getDockingAlgorithm());
			text.append("\n\n*** Options of algorithm ***\n");
			const Options& alg_opt = dock_res_->getDockingOptions();
			Options::ConstIterator it = alg_opt.begin();
			for(; +it; ++it)
			{
				text.append(it->first);
				text.append(" : ");
				text.append(it->second);
				text.append("\n");
			}
			
			QMessageBox* info_message = new QMessageBox("Docking Options", text, QMessageBox::NoIcon, 
																									QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton,
																									0,0, false,WDestructiveClose);
			info_message->show();
		}
		
		void DockResultDialog::contextMenuRequested(int row, int column, const QPoint& pos)
		{
				QPopupMenu context_menu;
				Position menu_entry_pos; 
				
				menu_entry_pos = context_menu.insertItem("Delete Score Column", this, SLOT(deleteColumn_(int)));
				context_menu.setItemParameter(menu_entry_pos, column);
				if (!column || result_table->numCols() < 3)
				{
					context_menu.setItemEnabled(menu_entry_pos, false);
				}
				
				menu_entry_pos = context_menu.insertItem("Scoring Options", this, SLOT(showScoringOptions_(int)));
				context_menu.setItemParameter(menu_entry_pos, column);
				if (!column) context_menu.setItemEnabled(menu_entry_pos, false);
				
				menu_entry_pos = context_menu.insertItem("Redock", this, SLOT(redock_(int)));
				context_menu.setItemParameter(menu_entry_pos, row);
				MainControl* main_control = MainControl::getInstance(0);
				if (!main_control)
				{
					Log.error() << "Error while showing context menu! " << __FILE__ << " " << __LINE__ << std::endl;
					return;
				}
				if (main_control->compositesAreLocked())
				{
					context_menu.setItemEnabled(menu_entry_pos, false);
				}
				
				context_menu.exec(pos);
		}
		
		// closes the dialog
		void DockResultDialog::closeClicked()
		{
			// close dialog and destroy it
			close(true);
		}
		
		// deletes chosen score column
		void DockResultDialog::deleteColumn_(int column)
		{
			if(!dock_res_) return;
			result_table->removeColumn(column);
			dock_res_->deleteScoring(column-1);
				
			// adjust the table/dialog size
			result_table->adjustSize();
			adjustSize();
		}
		
		// opens new dialog with scoring options
		void DockResultDialog::showScoringOptions_(int column)
		{
			if(!dock_res_) return;
			
			QString text = "Scoring function: ";
			text.append(dock_res_->getScoringName(column-1));
			const Options& scoring_opt = dock_res_->getScoringOptions(column-1);
			if(scoring_opt.isEmpty())
			{
			 	text.append("\n\nThere are no options.");
			}
			else
			{
				text.append("\n\n*** Options of scoring function ***\n");
				Options::ConstIterator it = scoring_opt.begin();
				for(; +it; ++it)
				{
					text.append(it->first);
					text.append(" : ");
					text.append(it->second);
					text.append("\n");
				}
			}
			
			QMessageBox* info_message = new QMessageBox("Scoring Options", text, QMessageBox::NoIcon, 
																									QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton,
																									0,0, false,WDestructiveClose);
			info_message->show();
		}
		 
		// 
		void DockResultDialog::redock_(int row)
		{
			if(!dock_res_) return;
			
			// get snapshot number of this row
			int snapshot = (result_table->text(row, 0)).toInt();
			// apply snapshot
			const ConformationSet* conformation_set = dock_res_->getConformationSet();
			SnapShot selected_conformation = (*conformation_set)[snapshot];
			selected_conformation.applySnapShot(*docked_system_);
			
			// split docked system into its original docking partners
			if (redock_partner1_)
			{
				delete redock_partner1_;
			}
			if (redock_partner2_)
			{
				delete redock_partner2_;
			}
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
		
	} // end of namespace VIEW
} // end of namespace BALL
