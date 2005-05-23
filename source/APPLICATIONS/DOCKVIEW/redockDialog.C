#include "redockDialog.h"
#include "dockResult.h"
#include "geometricFitDialog.h"

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <qlineedit.h>

#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>
#include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/amberEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>

#define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{
		//Default constructor
		RedockDialog::RedockDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: RedockDialogData(parent, name, modal, fl),
				ModularWidget(name),
				PreferencesEntry(),
				dock_alg_(0),
				progress_dialog_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new RedockDialog " << this << std::endl;
		#endif
			
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			
			// register QWidgets of Dialog with PreferenceEntry
			// entries of them in the INIFile will be generated
			setINIFileSectionName("REDOCKING");
			registerObject_(algorithms);
			registerObject_(scoring_functions);
			registerObject_(best_num);
			registerObject_(verbosity);
			registerObject_(phi_min);
			registerObject_(psi_min);
			registerObject_(theta_min);
			registerObject_(phi_max);
			registerObject_(psi_max);
			registerObject_(theta_max);
			registerObject_(delta_phi);
			registerObject_(delta_psi);
			registerObject_(delta_theta);
			
			hide();
		}
		
		RedockDialog::~RedockDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class RedockDialog" << std::endl;
			#endif 
		}
		
		/**  Assignment operator
		 */
		const RedockDialog& RedockDialog::operator =(const RedockDialog& redock_dialog)
			throw()
		{
		 	if (&redock_dialog != this)
			{
				algorithm_dialogs_ = redock_dialog.algorithm_dialogs_;
				scoring_dialogs_ = redock_dialog.scoring_dialogs_;
				algorithm_opt_ = redock_dialog.algorithm_opt_;
				scoring_opt_ = redock_dialog.scoring_opt_;
			}
			return *this;
		}
		
		// Adds docking algorithm to Combobox and its advanced option dialogs to HashMap.
		void RedockDialog::addAlgorithm(const QString& name, const int algorithm, QDialog* dialog)
			throw()
		{
			if(dialog)
			{
				// add dialog to HashMap
				algorithm_dialogs_[algorithm] = dialog;
			}
			// add to ComboBox
			algorithms->insertItem(name, algorithm);
		}
		
		// Adds scoring function to Combobox and its advanced option dialogs to HashMap, if it has such a dialog.
		void RedockDialog::addScoringFunction(const QString& name, const int score_func, QDialog* dialog)
			throw()
		{
			if(dialog)
			{
				// add to HashMap for scoring dialogs
				scoring_dialogs_[score_func] = dialog;
			}
			// add to HashMap for names of scoring functions
			sf_names_[score_func] = name;
		}
	
		
		// Initializes the popup menu Molecular Mechanics with its checkable submenu Docking.
		void RedockDialog::initializeWidget(MainControl& main_control)
			throw()
		{
			//build HashMap for algorithm advanced option dialogs
			//make sure the order of added algorithms is consistent to the enum order
			//because the algorithm with enum value i should be at position i in the combobox
			//otherwise you get the wrong option dialog for an algorithm
			GeometricFitDialog* geo_fit = new GeometricFitDialog(this);
			addAlgorithm("Geometric Fit", GEOMETRIC_FIT, geo_fit);
			
			//build HashMap for scoring function advanced option dialogs
			//make sure the order of added scoring functions is consistent to the enum order
			//because the scoring function with enum value i should be at position i in the Combobox
			//otherwise you get the wrong option dialog for a scoring function
			addScoringFunction("Default", DEFAULT);
			addScoringFunction("Amber Force Field", AMBER_FF, &(MolecularStructure::getInstance(0)->getAmberConfigurationDialog()));
			addScoringFunction("Random", RANDOM);
			
			vector<int> sf;
			sf.push_back(DEFAULT);
			sf.push_back(AMBER_FF);
			allowed_sf_[GEOMETRIC_FIT] = sf;
			
			fetchPreferences(main_control.getINIFile());
		}
		
		// Read the preferences from the INIFile.
		void RedockDialog::fetchPreferences(INIFile& file)
			throw()
		{
			PreferencesEntry::readPreferenceEntries(file);
			// call this function to check which algorithm is the current item in the combobox
			// and set advanced button enabled if necessary
			algorithmChosen();
			scoringFuncChosen();
		}
		
		// Write the preferences to the INIFile.
		void RedockDialog::writePreferences(INIFile& file)
			throw()
		{
			Log.info() << "in writePreferences! " << std::endl;
			PreferencesEntry::writePreferenceEntries(file);
		}
		
		/// Reset the dialog to the standard values
		void RedockDialog::reset()
			throw()
		{
			// comboboxes
			algorithms->setCurrentItem(0);
			scoring_functions->setCurrentItem(0);
			
			// buttons
			scoring_advanced_button->setEnabled(false);
			
			//options
			best_num->setText("100");
			verbosity->setText("1");
			
			// Euler angles
			phi_min->setText("-15");
			psi_min->setText("-15");
			theta_min->setText("-15");
			phi_max->setText("15");
			psi_max->setText("15");
			theta_max->setText("15");
			delta_phi->setText("3");
			delta_psi->setText("3");
			delta_theta->setText("3");
		}
		
		void RedockDialog::setSystems(System& s1, System& s2)
			throw()
		{
		 	docking_partner1_ = s1;
			docking_partner2_ = s2;
		}
		
		
		// set options with values user has chosen 
		void RedockDialog::applyValues_()
			throw()
		{
			// options for all docking algorithms
			/////////////////////////////////////// TODO allgemeine Options ////////////////////////////////////////////
			//options_[DockingAlgorithm::Option::BEST_NUM] = String(best_num->text().ascii()).toInt();
			algorithm_opt_[GeometricFit::Option::BEST_NUM] = String(best_num->text().ascii()).toInt();
			algorithm_opt_[GeometricFit::Option::VERBOSITY] = String(verbosity->text().ascii()).toInt();
			
			// options for chosen algorithm; options are filled by the corresponding dialog
			int index = algorithms->currentItem();
			switch(index)
			{
				case GEOMETRIC_FIT:
					GeometricFitDialog* dialog = RTTI::castTo<GeometricFitDialog>(*(algorithm_dialogs_[index]));
					dialog->getOptions(algorithm_opt_);
					break;
			}
			
			index = scoring_functions->currentItem();
			switch(index)
			{
				case AMBER_FF:
				{
					AmberFF& ff = MolecularStructure::getInstance(0)->getAmberFF();
					AmberConfigurationDialog* dialog = RTTI::castTo<AmberConfigurationDialog>(*(scoring_dialogs_[index]));
					
					//now the Amber force field gets its options
					dialog->applyTo(ff);
					scoring_opt_ = ff.options;
				}
			}
		}
		
		//
		void RedockDialog::okPressed()
		{
			hide();
			
		}
		
		//
		void RedockDialog::cancelPressed()
		{
			writePreferences(getMainControl()->getINIFile());
			close(true);
		}
		
		//
		void RedockDialog::resetPressed()
		{
			reset();
		}
		
		// advanced button for algorithm options pressed
		void RedockDialog::algAdvancedPressed()
		{
			// show corresponding options dialog
			int index = algorithms->currentItem();
			if(index)
			{
				algorithm_dialogs_[index]->exec();
			}
		}
		
		// advanced button for scoring function options pressed
		void RedockDialog::scoringAdvancedPressed()
		{
			// show corresponding options dialog
			int index = scoring_functions->currentItem();
			if(index)
			{
				scoring_dialogs_[index]->exec();
			}
		}
		
		// Indicates an algorithm in the combobox was chosen.
		void RedockDialog::algorithmChosen()
		{
			// if chosen algorithm has advanced options, enable advanced_button
			int index = algorithms->currentItem();
			if(algorithm_dialogs_.has(index))
			{
				alg_advanced_button->setEnabled(true);
			}
			else
			{
				alg_advanced_button->setEnabled(false);
			}
			
			scoring_functions->clear();
			if(algorithm_dialogs_.has(index))
			{
				for(unsigned int j = 0; j < allowed_sf_[index].size(); j++)
				{
				 	scoring_functions->insertItem(sf_names_[allowed_sf_[index][j]]);
				}
			}
			else
			{
			 	HashMap<int, QString>::Iterator name_it = sf_names_.begin();
				for(; +name_it; name_it++)
				{
				 	scoring_functions->insertItem(name_it->second);
				}
			}
		}
		
		// Indicates a scoring function in the combobox was chosen.
		void RedockDialog::scoringFuncChosen()
		{
			// if chosen scoring function has advanced options, enable advanced_button
			int index = scoring_functions->currentItem();
			if(scoring_dialogs_.has(index))
			{
				scoring_advanced_button->setEnabled(true);
			}
			else
			{
				scoring_advanced_button->setEnabled(false);
			}
		}
		
	}
}
