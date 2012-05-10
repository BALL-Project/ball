// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/geometricFitDialog.h>
#include <BALL/DOCKING/geometricFit.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

//#define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{
		// Constructor
		GeometricFitDialog::GeometricFitDialog(QWidget* parent, const char* name)
			: QDialog(parent),
				Ui_GeometricFitDialogData(),
				PreferencesEntry()
			{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "new GeometricFitDialog " << this << std::endl;
			#endif

				setupUi(this);
				setObjectName(name);
				setINIFileSectionName("GEOMETRIC_FIT_OPTIONS");
				registerWidgets_();
		
				// set flag
				is_redock_ = false;
				
				hide();
				connect(reset_button, SIGNAL(pressed()), this, SLOT(reset()));
			}
		
		// Copy constructor.
		GeometricFitDialog::GeometricFitDialog(const GeometricFitDialog& geo_fit_dialog)
			: QDialog(),
				Ui_GeometricFitDialogData(),
				PreferencesEntry(),
				has_changed_(geo_fit_dialog.has_changed_),
				is_redock_(geo_fit_dialog.is_redock_),
				backup_(geo_fit_dialog.backup_)
		{}
			
		// Destructor
		GeometricFitDialog::~GeometricFitDialog()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class GeometricFitDialog" << std::endl;
			#endif 
		}
		
		// Assignment operator
		const GeometricFitDialog& GeometricFitDialog::operator =(const GeometricFitDialog& geo_fit_dialog)
		{
			if (&geo_fit_dialog != this)
			{
				backup_ = geo_fit_dialog.backup_;
				has_changed_ = geo_fit_dialog.has_changed_;
				is_redock_ = geo_fit_dialog.is_redock_;
			}
			return *this;
		}
		
		// Read the preferences from an INIFile
		// for reading docking preferences call PreferencesEntry::readPreferenceEntries
		// for reading redocking options call fetchPreferences_
		void GeometricFitDialog::fetchPreferences(INIFile& file)
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
		{
			if (is_redock_)
			{
				swapValues_();
			}
			PreferencesEntry::writePreferenceEntries(file);
			
			file.appendSection("GEOMETRIC_FIT_OPTIONS_REDOCK");
			
			for (Position i = 0; i < backup_.size(); i++)
			{
				String entry = String("option_entry_") + String(i);
				file.insertValue("GEOMETRIC_FIT_OPTIONS_REDOCK", entry, ascii(backup_[i]));
			}
		}

		// Reset the dialog to the standard values
		void GeometricFitDialog::reset()
		{
			restoreDefaultValues();
		}

		void GeometricFitDialog::reject()
		{
			restoreValues();
			QDialog::reject();
		}

		void GeometricFitDialog::accept()
		{
			storeValues();
			QDialog::accept();
		}
		
		// Fill options with values of the dialog.
		void GeometricFitDialog::getOptions(Options& options)
		{
		  try
			{
				options[GeometricFit::Option::NEAR_RADIUS] = ascii(near_radius->text()).toFloat();
				options[GeometricFit::Option::GRID_SPACING] = ascii(grid_spacing->text()).toFloat();
				options[GeometricFit::Option::SURFACE_THICKNESS] = ascii(surface_thickness->text()).toFloat();
				options[GeometricFit::Option::DEGREE_INTERVAL] = ascii(deg_interval->text()).toDouble();
				options[GeometricFit::Option::TOP_N] = ascii(peak_num->text()).toInt();
				options[GeometricFit::Option::PENALTY_STATIC] = ascii(penalty_static->text()).toInt();
				options[GeometricFit::Option::PENALTY_MOBILE] = ascii(penalty_mobile->text()).toInt();
			}
		  catch (Exception::InvalidFormat&)
			{
				Log.error() << "Conversion from String to float, double or int failed: invalid format! " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
		  
			if (surface_type->currentText() == "Connolly")
			{
			  options[GeometricFit::Option::SURFACE_TYPE] = GeometricFit::CONNOLLY;
			}
		  else if (surface_type->currentText() == "van der Waals")
			{
			  options[GeometricFit::Option::SURFACE_TYPE] = GeometricFit::VAN_DER_WAALS;
			}
		  else
			{
			  options[GeometricFit::Option::SURFACE_TYPE] = GeometricFit::FTDOCK;
			}
		}
		  
		// Sets the flags 'is_redock_' and 'has_changed_'
		void GeometricFitDialog::isRedock(bool is_redock)
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
		
		// Swaps the option values between vector backup_ and dialog
		// Is called in show() if has_changed_ is true
		// and in writePreferences if is_redock_ is true
		void GeometricFitDialog::swapValues_()
		{
			QString temp = surface_thickness->text();
			surface_thickness->setText(backup_[0]);
			backup_[0] = temp;

			temp = grid_spacing->text();
			grid_spacing->setText(backup_[1]);
			backup_[1] = temp;

			temp = penalty_static->text();
			penalty_static->setText(backup_[2]);
			backup_[2] = temp;

			temp = penalty_mobile->text();
			penalty_mobile->setText(backup_[3]);
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
			surface_type->setCurrentIndex(surface_type->findText(backup_[7]));
			backup_[7] = temp;
		}
		
	// --------------------------------- SLOTS ------------------------------------------------
	// ----------------------------------------------------------------------------------------
	
		// Shows dialog to user.
		// Calls swapValues_ if we are now doing docking and did redocking before or otherwise
		void GeometricFitDialog::show()
		{
			if (has_changed_)
			{
				swapValues_();
			}
			QDialog::show();
		}
	
	} // namespace VIEW
} // namespace BALL
