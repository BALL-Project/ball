#include <BALL/VIEW/DIALOGS/dockingAlgorithmDialog.h>

namespace BALL
{
	namespace VIEW
	{
    // Constructor
		DockingAlgorithmDialog::DockingAlgorithmDialog(QWidget* parent)
			throw()
			: QDialog(parent),
				PreferencesEntry()
		{	
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "new DockingAlgorithmDialog " << this << std::endl;
			#endif
	    
				// set flag
				is_redock_ = false;
				inifile_section_name_backup_ = "";

				//setModal(true);
				hide();
		}

		//Copy Constructor.
		DockingAlgorithmDialog::DockingAlgorithmDialog(const DockingAlgorithmDialog& dialog)
			throw()
			: QDialog(),
				PreferencesEntry(),
				has_changed_(dialog.has_changed_),
				is_redock_(dialog.is_redock_),
				backup_(dialog.backup_),
				inifile_section_name_backup_(dialog.inifile_section_name_backup_)
				{}
				
		 // Destructor
		 DockingAlgorithmDialog::~DockingAlgorithmDialog()
		  throw()
		 {
			 #ifdef BALL_VIEW_DEBUG
				 Log.info() << "Destructing object " << this << " of class DockingAlgorithmDialog" << std::endl;
			 #endif 
		 }

		 // Assignment operator
		const DockingAlgorithmDialog& DockingAlgorithmDialog::operator =(const DockingAlgorithmDialog& dialog)
			throw()
		{
			if (&dialog != this)
			{
				backup_ = dialog.backup_;
				has_changed_ = dialog.has_changed_;
				is_redock_ = dialog.is_redock_;
				inifile_section_name_backup_ = dialog.inifile_section_name_backup_;
			}
			return *this;
		}

		// Sets the flags 'is_redock_' and 'has_changed_'
		void DockingAlgorithmDialog::isRedock(bool is_redock)
			throw()
		{
			if (is_redock_ == is_redock)
			{
			 	has_changed_ = false;
			}
			else
			{
				has_changed_ = true;
			 	is_redock_ = is_redock;
			}
		}

		void DockingAlgorithmDialog::getOptions(Options& options)
			throw() 
		{	
			Log.error() << "This is just an interface definition. Check your dynamic binding" << std::endl;
		}

		//Read the preferences from an INIFile
		void DockingAlgorithmDialog::fetchPreferences(INIFile& file)
			throw()
		{
			// first read the docking options
			PreferencesEntry::readPreferenceEntries(file);

			// store docking values of the widgets in ValueMap last_values_
			storeValues();
			// now read redocking options
			// INIFile has not yet a section redocking, fill backup_ with default values
			if (!file.hasSection(inifile_section_name_backup_))
			{
				backup_ = default_values_;
			}
			else
			{
				// make a backup of docking options
				backup_ = last_values_;
				String s = inifile_section_name_backup_;
			  inifile_section_name_backup_ = inifile_section_name_;
				inifile_section_name_ = s;
				// read redocking values into widgets
				PreferencesEntry::readPreferenceEntries(file);
				// store redocking values of the widgets in ValueMap last_values_
				storeValues();
				// swap, s.t. docking values are in widgets and redockng values are in backup_
				swapValues_();
			}
		}

		// Write the preferences to an INIFile
		void DockingAlgorithmDialog::writePreferences(INIFile& file)
			throw()
		{
			// first write the options that are currently in the dialog
			PreferencesEntry::writePreferenceEntries(file);
			// now write the options that are in backup_
	    swapValues_();
      PreferencesEntry::writePreferenceEntries(file);
			swapValues_();
		}
		
	  // Swaps the option values between backup_ and dialog.
	  void DockingAlgorithmDialog::swapValues_() 
		  throw()
		{
			ValueMap temp = last_values_;
			String temp_s = inifile_section_name_;
      restoreValues_(true, backup_);
			last_values_ = backup_;
			inifile_section_name_ = inifile_section_name_backup_;
			backup_ = temp;
      inifile_section_name_backup_ = temp_s;
		}


	// --------------------------------- SLOTS ------------------------------------------------
	// ----------------------------------------------------------------------------------------

		// Shows dialog to user.
		// Calls swapValues_ if we are now doing docking and did redocking before or otherwise
		void DockingAlgorithmDialog::show()
		{
			if (has_changed_)
			{
				swapValues_();
			}
			QDialog::show();
		}

	// Reset the dialog to the standard values
		void DockingAlgorithmDialog::reset()
		{
			restoreDefaultValues();
		}

		void DockingAlgorithmDialog::reject()
		{
			restoreValues();
			QDialog::reject();
		}

		void DockingAlgorithmDialog::accept()
		{
			storeValues();
			QDialog::accept();
		}

	} // namespace VIEW
} // namespace BALL
