#include "geometricFitDialog.h"

#include <qlineedit.h>
#include <qcombobox.h>

namespace BALL
{
	namespace VIEW
	{
		/// Constructor
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
				// entries of them in the INIFile will be generated
				setINIFileSectionName("GEOMETRIC_FIT_OPTIONS");
				registerObject_(surface_thickness);
				registerObject_(grid_spacing);
				registerObject_(penalty_value1);
				registerObject_(penalty_value2);
				registerObject_(near_radius);
				registerObject_(deg_interval);
				registerObject_(peak_num);
				registerObject_(surface_type);
		
				hide();
			}
	
		/// Destructor
		GeometricFitDialog::~GeometricFitDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class GeometricFitDialog" << std::endl;
			#endif 
		}
		
		// ------------------------- helper functions -------------------------------------
		// --------------------------------------------------------------------------------
		
		/// Read the preferences from a INIFile
		void GeometricFitDialog::fetchPreferences(INIFile& file)
					throw()
		{
			PreferencesEntry::readPreferenceEntries(file);
		}
				
		/// Write the preferences to a INIFile
		void GeometricFitDialog::writePreferences(INIFile& file)
					throw()
		{
			PreferencesEntry::writePreferenceEntries(file);
		}

		/// Reset the dialog to the standard values
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
		
		/// Set options
		void GeometricFitDialog::setOptions(Options& options)
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
	
	// ------------------------- SLOTS ------------------------------------------------
	// --------------------------------------------------------------------------------
	
		///
		void GeometricFitDialog::resetPressed()
		{
			reset();
		}
		
		//
		void GeometricFitDialog::cancelPressed()
		{
			reset();
			hide();
		}
		
	} // namespace VIEW
} // namespace BALL
