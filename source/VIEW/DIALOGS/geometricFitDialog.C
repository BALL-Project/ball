
#include <BALL/VIEW/DIALOGS/geometricFitDialog.h>
#include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtGui/qlineedit.h>
#include <QtGui/qcombobox.h>

//#define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{
		// Constructor
		GeometricFitDialog::GeometricFitDialog(QWidget* parent, const char* name)
			throw()
			: Ui_GeometricFitDialogData(),
				DockingAlgorithmDialog(parent)
			{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "new GeometricFitDialog " << this << std::endl;
			#endif

				setupUi(this);
				setObjectName(name);
			
				// register QWidgets of Dialog with PreferenceEntry
				// entries of them will be generated in the INIFile
				setINIFileSectionName("GEOMETRIC_FIT_OPTIONS");
				registerObject_(surface_thickness);
				registerObject_(grid_spacing);
				registerObject_(penalty_static);
				registerObject_(penalty_mobile);
				registerObject_(near_radius);
				registerObject_(deg_interval);
				registerObject_(peak_num);
				registerObject_(surface_type);
		    inifile_section_name_backup_ = "GEOMETRIC_FIT_OPTIONS_REDOCK";
				
				connect(reset_button, SIGNAL(pressed()), this, SLOT(reset()));
			}
		
		// Copy constructor.
		GeometricFitDialog::GeometricFitDialog(const GeometricFitDialog& geo_fit_dialog)
			throw()
			: Ui_GeometricFitDialogData(),
		    DockingAlgorithmDialog(geo_fit_dialog)
		{}
			
		// Destructor
		GeometricFitDialog::~GeometricFitDialog()
		  throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class GeometricFitDialog" << std::endl;
			#endif 
		}
		
		// Assignment operator
		const GeometricFitDialog& GeometricFitDialog::operator =(const GeometricFitDialog& geo_fit_dialog)
			throw()
		{
			if (&geo_fit_dialog != this)
			{
				DockingAlgorithmDialog::operator=(geo_fit_dialog);
			}
			return *this;
		}
		
		// Read the preferences from an INIFile
		// for reading docking preferences call PreferencesEntry::readPreferenceEntries
		// for reading redocking options call fetchPreferences_
	/*	void GeometricFitDialog::fetchPreferences(INIFile& file)
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
		}*/
				
		// Fill options with values of the dialog.
		void GeometricFitDialog::getOptions(Options& options)
					throw()
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
		  catch (Exception::InvalidFormat)
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
	
	} // namespace VIEW
} // namespace BALL
