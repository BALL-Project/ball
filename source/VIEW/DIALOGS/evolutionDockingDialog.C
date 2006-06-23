
#include <BALL/VIEW/DIALOGS/evolutionDockingDialog.h>
#include "../../STRUCTURE/DOCKING/evolutionaryDocking.h"
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>

#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/QFileDialog>

//#define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{
		// Constructor
		EvolutionDockingDialog::EvolutionDockingDialog(QWidget* parent, const char* name)
			throw()
				: Ui_EvolutionDockingDialogData(),
				  DockingAlgorithmDialog(parent)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "new EvolutionDockingDialog " << this << std::endl;
			#endif

				setupUi(this);
				setObjectName(name);
			
				// register QWidgets of Dialog with PreferenceEntry
				// entries of them will be generated in the INIFile
				setINIFileSectionName("EVOLUTION_DOCKKING_OPTIONS");
				registerObject_(grid_group);
				registerObject_(grid_filename);
				registerObject_(trans_box_group);
				registerObject_(trans_box_bottom_x);
				registerObject_(trans_box_bottom_y);
				registerObject_(trans_box_bottom_z);
				registerObject_(trans_box_top_x);
				registerObject_(trans_box_top_y);
				registerObject_(trans_box_top_z);
				registerObject_(max_iterations);
				registerObject_(init_population);	
				registerObject_(population);
				registerObject_(survivors);
				registerObject_(force_field_group);
        inifile_section_name_backup_ = "EVOLUTION_DOCKKING_OPTIONS_REDOCK";
		
				connect(cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
				connect(ok_button, SIGNAL( clicked() ), this, SLOT(accept()));
				connect(reset_button, SIGNAL(pressed()), this, SLOT(reset()));
			  connect(grid_radio_button, SIGNAL(clicked()), this, SLOT(enableFileBrowsing()));
				connect(new_grid_radio_button, SIGNAL(clicked()), this, SLOT(disableFileBrowsing()));
				connect(browse_button, SIGNAL(clicked()), this, SLOT(browseGridFile()));
				connect(force_field_button, SIGNAL(clicked()), this, SLOT(showForceFieldOptions()));
			}
		
		// Copy constructor.
		EvolutionDockingDialog::EvolutionDockingDialog(const EvolutionDockingDialog& ev_dock_dialog)
			throw()
			: Ui_EvolutionDockingDialogData(),
        DockingAlgorithmDialog(ev_dock_dialog)
		{}
			
		// Destructor
		EvolutionDockingDialog::~EvolutionDockingDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class EvolutionDockingDialog" << std::endl;
			#endif 
		}
		
		// Assignment operator
		const EvolutionDockingDialog& EvolutionDockingDialog::operator =(const EvolutionDockingDialog& ev_dock_dialog)
			throw()
		{
			if (&ev_dock_dialog != this)
			{
				DockingAlgorithmDialog::operator=(ev_dock_dialog);
			}
			return *this;
		}
		
		// Read the preferences from an INIFile
		// for reading docking preferences call PreferencesEntry::readPreferenceEntries
		// for reading redocking options call fetchPreferences_
		/*void EvolutionDockingDialog::fetchPreferences(INIFile& file)
					throw()
		{
			Log.error() << "EvolutionDockingDialog::fetchPreferences" << std::endl;
			PreferencesEntry::readPreferenceEntries(file);
			
			fetchPreferences_(file, "option_entry_0", "");
			fetchPreferences_(file, "option_entry_1", "1.0");
			fetchPreferences_(file, "option_entry_2", "1.0");
			fetchPreferences_(file, "option_entry_3", "1.0");
			fetchPreferences_(file, "option_entry_4", "-1.0");
			fetchPreferences_(file, "option_entry_5", "-1.0");
			fetchPreferences_(file, "option_entry_6", "-1.0");
			fetchPreferences_(file, "option_entry_7", "100");
			fetchPreferences_(file, "option_entry_8", "1000");
			fetchPreferences_(file, "option_entry_9", "40");
			fetchPreferences_(file, "option_entry_10", "20");
		}
		
		// function to read the redocking options from INIFile into vector backup_
		// if INIFile has not yet a section EVOLUTION_DOCKING_OPTIONS_REDOCK, fill backup_ vector with default values
		void EvolutionDockingDialog::fetchPreferences_(INIFile& file, const String& entry, const QString& default_value)
			throw()
		{
			if (!file.hasEntry("EVOLUTION_DOCKING_OPTIONS_REDOCK", entry))
			{
			 	backup_.push_back(default_value);
			}
			else
			{
				backup_.push_back(QString(file.getValue("EVOLUTION_DOCKING_OPTIONS_REDOCK", entry).c_str()));
			}
		}
		
		// Write the preferences to an INIFile
		// If redocking was last action, first swap the option values between backup_ vector and dialog
		// Calls  PreferencesEntry::writePreferenceEntries for docking preferences
		// for redocking options: append section and insert the values of backup_ vector as entries
		void EvolutionDockingDialog::writePreferences(INIFile& file)
			throw()
		{
			Log.error() << "EvolutionDockingDialog::writePreferences" << std::endl;
			if (is_redock_)
			{
				swapValues_();
			}
			PreferencesEntry::writePreferenceEntries(file);
			
			file.appendSection("EVOLUTION_DOCKING_OPTIONS_REDOCK");
			
			for (Position i = 0; i < backup_.size(); i++)
			{
				String entry = String("option_entry_") + String(i);
				file.insertValue("EVOLUTION_DOCKING_OPTIONS_REDOCK", entry, ascii(backup_[i]));
			}
		}*/
		
		// Fill options with values of the dialog.
		void EvolutionDockingDialog::getOptions(Options& options)
					throw()
		{
		  try
			{
				options[EvolutionaryDocking::Option::GRID_FILE] = String(ascii(grid_filename->text()));
				options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_X] = ascii(trans_box_bottom_x->text()).toFloat();
				options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Y] = ascii(trans_box_bottom_y->text()).toFloat();
				options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Z] = ascii(trans_box_bottom_z->text()).toFloat();
				options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_X] = ascii(trans_box_top_x->text()).toFloat();
				options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Y] = ascii(trans_box_top_y->text()).toFloat();
				options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Z] = ascii(trans_box_top_z->text()).toFloat();
				options[EvolutionaryDocking::Option::MAX_ITERATIONS] = ascii(max_iterations->text()).toInt();
				options[EvolutionaryDocking::Option::INITIAL_POPULATION] = ascii(init_population->text()).toInt();
				options[EvolutionaryDocking::Option::POPULATION] = ascii(population->text()).toInt();
				options[EvolutionaryDocking::Option::SURVIVORS] = ascii(survivors->text()).toInt();
			}
		  catch (Exception::InvalidFormat)
			{
				Log.error() << "Conversion from String to float, double or int failed: invalid format! " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
		  
		}
	
		// Fill options with values of the force field dialog.
		void EvolutionDockingDialog::getFFOptions(Options& options)
					throw()
		{
			MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
			if (!mol_struct)
			{
				Log.error() << "Error while applying options of AMBER_FF scoring function! " << __FILE__ << " " << __LINE__<< std::endl;
				return;
			}

			if(amber_radio_button->isChecked())
			{
				AmberConfigurationDialog& dialog =	mol_struct->getAmberConfigurationDialog();
				AmberFF& ff = mol_struct->getAmberFF();
				// now the Amber force field gets its options
				dialog.applyTo(ff);
				options = ff.options;
			}
			else
			{
				CharmmConfigurationDialog& dialog = mol_struct->getCharmmConfigurationDialog();
				CharmmFF& ff = mol_struct->getCharmmFF();
				// now the Charmm force field gets its options
				dialog.applyTo(ff);
				options = ff.options;
			}
		}
		  
				
				
	// --------------------------------- SLOTS ------------------------------------------------
	// ----------------------------------------------------------------------------------------

		/** Is called when radio button for using a created grid is pressed. 
		 	* It enables the browse button and to corresponding line edit.
		 	*/
		void EvolutionDockingDialog::enableFileBrowsing()
		{
			browse_button->setEnabled(true);
			grid_filename->setEnabled(true);
			filename_label->setEnabled(true);
		}

		/** Is called when radio button for using a new grid is pressed. 
		 	* It disables the browse button and to corresponding line edit.
		 	*/
		void EvolutionDockingDialog::disableFileBrowsing()
		{
			browse_button->setEnabled(false);
			grid_filename->setEnabled(false);
			grid_filename->clear();
			filename_label->setEnabled(false);
		}

		/** Is called when browse button is pressed. 
			* It shows a file dialog.
		 	*/
		void EvolutionDockingDialog::browseGridFile()
		{
			// look up the full path of the grid file
			Path p;
			String filename = p.find(ascii(grid_filename->text()));

			if (filename == "")
			{
				filename = ascii(grid_filename->text());
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getOpenFileName(0, "Select an grid file", tmp, "*.gr", 0);
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field
				grid_filename->setText(result);
			}
		}

		/** Is called when force field options button is pressed. 
			* It shows the force field option dialog.
		 	*/
		void EvolutionDockingDialog::showForceFieldOptions()
		{
			MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
			if (!mol_struct)
			{
				Log.error() << "Error while opening force field option dialog! " 
										<< __FILE__ << " " << __LINE__ << std::endl;
				return;
			}

			if(amber_radio_button->isChecked())
			{
				mol_struct->getAmberConfigurationDialog().exec();
			}
			else
			{
				mol_struct->getCharmmConfigurationDialog().exec();
			}
		}

	} // namespace VIEW
} // namespace BALL
