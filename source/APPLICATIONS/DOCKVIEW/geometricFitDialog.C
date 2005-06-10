#include "geometricFitDialog.h"

#include <qlineedit.h>
#include <qcombobox.h>

namespace BALL
{
	namespace VIEW
	{
		// Constructor
		GeometricFitDialog::GeometricFitDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
			throw()
			: GeometricFitDialogData(parent, name, modal, fl),
				ModularWidget(name),
				PreferencesEntry()
			{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new GeometricFitDialog " << this << std::endl;
			#endif
			
				// register the widget with the MainControl
				ModularWidget::registerWidget(this);
			
				// register QWidgets of Dialog with PreferenceEntry
				// entries of them will be generated in the INIFile
				setINIFileSectionName("GEOMETRIC_FIT_OPTIONS");
				registerObject_(surface_thickness);
				registerObject_(grid_spacing);
				registerObject_(penalty_value1);
				registerObject_(penalty_value2);
				registerObject_(near_radius);
				registerObject_(deg_interval);
				registerObject_(peak_num);
				registerObject_(surface_type);
		
				// set flag
				is_redock_ = false;
				
				hide();
			}
	
		// Destructor
		GeometricFitDialog::~GeometricFitDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class GeometricFitDialog" << std::endl;
			#endif 
		}
		
		// Read the preferences from an INIFile
		// for reading docking preferences call PreferencesEntry::readPreferenceEntries
		// for reading redocking options call fetchPreferences_
		void GeometricFitDialog::fetchPreferences(INIFile& file)
					throw()
		{
			PreferencesEntry::readPreferenceEntries(file);
			
			fetchPreferences_(file, "option_entry_0", "1.0");
			fetchPreferences_(file, "option_entry_1", "1.0");
			fetchPreferences_(file, "option_entry_2", "-15");
			fetchPreferences_(file, "option_entry_3", "1");
			fetchPreferences_(file, "option_entry_4", "1.8");
			fetchPreferences_(file, "option_entry_5", "20");
			fetchPreferences_(file, "option_entry_6", "3");
			fetchPreferences_(file, "option_entry_7", "Connolly");
		}
		
		// function to read the redocking options from INIFile into vector backup_
		// if INIFile has not yet a section GEOMETRIC_FIT_OPTIONS_REDOCK, fill backup_ vector with default values
		void GeometricFitDialog::fetchPreferences_(INIFile& file, const String& entry, const QString& default_value)
			throw()
		{
			if (!file.hasEntry("GEOMETRIC_FIT_OPTIONS_REDOCK", entry))
			{
			 	backup_.push_back(default_value);
			}
			else
			{
				backup_.push_back(QString(file.getValue("GEOMETRIC_FIT_OPTIONS_REDOCK", entry).c_str()));
			}
		}
		
		// Write the preferences to an INIFile
		// If redocking was last action, first swap the option values between backup_ vector and dialog
		// Calls  PreferencesEntry::writePreferenceEntries for docking preferences
		// for redocking options: append section and insert the values of backup_ vector as entries
		void GeometricFitDialog::writePreferences(INIFile& file)
			throw()
		{
			if(is_redock_)
			{
				swapValues_();
			}
			PreferencesEntry::writePreferenceEntries(file);
			
			file.appendSection("GEOMETRIC_FIT_OPTIONS_REDOCK");
			
			for(unsigned int i = 0; i < backup_.size(); i++)
			{
				String entry = String("option_entry_") + String(i);
				file.insertValue("GEOMETRIC_FIT_OPTIONS_REDOCK", entry, backup_[i].ascii());
			}
		}

		// Reset the dialog to the standard values
		void GeometricFitDialog::reset()
			throw()
		{
    	surface_thickness->setText("1.0");
    	grid_spacing->setText("1.0"); 
    	penalty_value1->setText("-15");
    	penalty_value2->setText("1");
    	deg_interval->setText("20");
    	near_radius->setText("1.8");
    	peak_num->setText("3");
			surface_type->setCurrentText("Connolly");
		}
		
		// Fill options with values of the dialog.
		void GeometricFitDialog::getOptions(Options& options)
					throw()
		{
			options[GeometricFit::Option::NEAR_RADIUS] = String(near_radius->text().ascii()).toFloat();
			options[GeometricFit::Option::GRID_SPACING] = String(grid_spacing->text().ascii()).toFloat();
			options[GeometricFit::Option::SURFACE_THICKNESS] = String(surface_thickness->text().ascii()).toFloat();
			options[GeometricFit::Option::DEGREE_INTERVAL] = String(deg_interval->text().ascii()).toDouble();
			options[GeometricFit::Option::TOP_N] = String(peak_num->text().ascii()).toInt();
			if(surface_type->currentText() == "Connolly")
			{
				options[GeometricFit::Option::SURFACE_TYPE] = GeometricFit::CONNOLLY;
			}
			else
			{
				options[GeometricFit::Option::SURFACE_TYPE] = GeometricFit::VAN_DER_WAALS;
			}
		}
		
		// Sets the flags 'is_redock_' and 'has_changed_'
		void GeometricFitDialog::setFlag(bool is_redock)
			throw()
		{
			if(is_redock_ == is_redock)
			{
			 	has_changed_ = false;
			}
			else
			{
				has_changed_ = true;
			 	is_redock_ = is_redock;
			}
		}
		
		// Swaps the option values between vector backup_ and dialog
		// Is called in show() if has_changed_ is true
		// and in writePreferences if is_redock_ is true
		void GeometricFitDialog::swapValues_()
			throw()
		{
			QString temp = surface_thickness->text();
			surface_thickness->setText(backup_[0]);
			backup_[0] = temp;

			temp = grid_spacing->text();
			grid_spacing->setText(backup_[1]);
			backup_[1] = temp;

			temp = penalty_value1->text();
			penalty_value1->setText(backup_[2]);
			backup_[2] = temp;

			temp = penalty_value2->text();
			penalty_value2->setText(backup_[3]);
			backup_[3] = temp;

			temp = near_radius->text();
			near_radius->setText(backup_[4]);
			backup_[4] = temp;

			temp = deg_interval->text();
			deg_interval->setText(backup_[5]);
			backup_[5] = temp;

			temp = peak_num->text();
			peak_num->setText(backup_[6]);
			backup_[6] = temp;

			temp = surface_type->currentText();
			surface_type->setCurrentText(backup_[7]);
			backup_[7] = temp;
		}
		
	// --------------------------------- SLOTS ------------------------------------------------
	// ----------------------------------------------------------------------------------------
	
		// Shows dialog to user.
		// Calls swapValues_ if we are now doing docking and did redocking before or otherwise
		void GeometricFitDialog::show()
		{
			if(has_changed_)
			{
				swapValues_();
			}
			GeometricFitDialogData::show();
		}
	
		//
		void GeometricFitDialog::resetPressed()
		{
			reset();
		}
		
		/////// TODO: take the values which were in the fields when dialog was opened
		void GeometricFitDialog::cancelPressed()
		{
			reject();
			//reset();
			//hide();
		}
		
	} // namespace VIEW
} // namespace BALL
