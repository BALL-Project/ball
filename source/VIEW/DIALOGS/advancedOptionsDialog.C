// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/advancedOptionsDialog.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>


namespace BALL
{
	namespace VIEW
	{

		advancedOptionsDialog::advancedOptionsDialog(QWidget* parent, const char* name)
			:advancedOptions( parent, name )
		{
		}

		advancedOptionsDialog::~advancedOptionsDialog()
		{
		}

		void advancedOptionsDialog::browseParameterFiles()
		{
			// look up the full path of the parameter file
			Path p;
			String filename = p.find(getFilename());
			if (filename == "")
			{
				filename = getFilename();
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getOpenFileName(tmp, "*.ini", 0, "Select an AMBER parameter file");
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field
				setFilename(String(result.ascii()));
			}
		}

		void advancedOptionsDialog::resetOptions()
		{
			nonbonded_cutoff_line_edit->setText("20.000000");
			vdw_cutoff_line_edit->setText("15.000000");
			vdw_cuton_line_edit->setText("13.000000");
			electrostatic_cutoff_line_edit->setText("15.000000");
			electrostatic_cuton_line_edit->setText("13.000000");
			scaling_electrostatic_1_4_line_edit->setText("2.000000");
			scaling_vdw_1_4_line_edit->setText("2.000000");

			distance_button->setChecked(false);
			constant_button->setChecked(true);
			assign_charges_checkBox->setChecked(true);
			assign_typenames_checkBox->setChecked(true);
			assign_types_checkBox->setChecked(true);
			overwrite_charges_checkBox->setChecked(true);
			overwrite_typenames_checkBox->setChecked(false);
		}

		const String& advancedOptionsDialog::getFilename() const
		{
			static String filename;
			filename = parameter_file_edit->text().ascii();
			return filename;
		}
		
		void advancedOptionsDialog::setFilename(const String& filename)
		{
			parameter_file_edit->setText(filename.c_str());
			parameter_file_edit->update();
		}
/*//experimental
		void advancedOptionsDialog::setNonbondedCutoff(float nonbonded_cutoff)
		{
			nonbonded_cutoff_line_edit->setText((String) nonbonded_cutoff);
		}
//experimtental
*/
		bool advancedOptionsDialog::getUseDistanceDependentDC() const
		{
			return distance_button->isChecked();
		}

		float advancedOptionsDialog::getNonbondedCutoff()
		{
			QString nonbondcut = nonbonded_cutoff_line_edit->text().ascii();
			float nonbond = nonbondcut.toFloat();
			return nonbond;
		}
		
		float advancedOptionsDialog::getVdwCutoff()
		{
			QString vdwcut = vdw_cutoff_line_edit->text().ascii();
			float vdw = vdwcut.toFloat();
			return vdw;
		}
		
		float advancedOptionsDialog::getVdwCuton()
		{
			QString vdwcut = vdw_cuton_line_edit->text().ascii();
			float vdw = vdwcut.toFloat();
			return vdw;
		}
		
		float advancedOptionsDialog::getElectrostaticCutoff()
		{
			QString elcut = electrostatic_cutoff_line_edit->text().ascii();
			float el = elcut.toFloat();
			return el;
		}
		
		float advancedOptionsDialog::getElectrostaticCuton()
		{
			QString elcut = electrostatic_cuton_line_edit->text().ascii();
			float el = elcut.toFloat();
			return el;
		}
		
		float advancedOptionsDialog::getScalingElectrostatic_1_4()
		{
			QString scaleel = scaling_electrostatic_1_4_line_edit->text().ascii();
			float scale = scaleel.toFloat();
			return scale;
		}

		float advancedOptionsDialog::getScalingVdw_1_4()
		{
			QString scalevdw = scaling_vdw_1_4_line_edit->text().ascii();
			float scale = scalevdw.toFloat();
			return scale;
		}
		
		bool advancedOptionsDialog::getAssignCharges()
		{
			return assign_charges_checkBox->isChecked();
		}
			
		bool advancedOptionsDialog::getAssignTypenames()
		{
			return assign_typenames_checkBox->isChecked();
		}
			
		bool advancedOptionsDialog::getAssignTypes()
		{
			return assign_types_checkBox->isChecked();
		}
			
		bool advancedOptionsDialog::getOverwriteCharges()
		{
			return overwrite_charges_checkBox->isChecked();
		}
			
		bool advancedOptionsDialog::getOverwriteTypenames()
		{
			return overwrite_typenames_checkBox->isChecked();
		}
		//function to restore previously changed options
		void advancedOptionsDialog::setOptions(float nonbonded_cutoff, float vdw_cutoff, float vdw_cuton, float electrostatic_cutoff, float electrostatic_cuton,
									    float scaling_electrostatic_1_4, float scaling_vdw_1_4, bool use_dddc, bool assign_charges,
									    bool assign_typenames, bool assign_types, bool overwrite_charges, bool overwrite_typenames)
		{
			nonbonded_cutoff_line_edit->setText(String(nonbonded_cutoff).c_str());
			vdw_cutoff_line_edit->setText(String(vdw_cutoff).c_str());
			vdw_cuton_line_edit->setText(String(vdw_cuton).c_str());
			electrostatic_cutoff_line_edit->setText(String(electrostatic_cutoff).c_str());
			electrostatic_cuton_line_edit->setText(String(electrostatic_cuton).c_str());
			scaling_electrostatic_1_4_line_edit->setText(String(scaling_electrostatic_1_4).c_str());
			scaling_vdw_1_4_line_edit->setText(String(scaling_vdw_1_4).c_str());

			distance_button->setChecked(use_dddc);
			assign_charges_checkBox->setChecked(assign_charges);
			assign_typenames_checkBox->setChecked(assign_typenames);
			assign_types_checkBox->setChecked(assign_types);
			overwrite_charges_checkBox->setChecked(overwrite_charges);
			overwrite_typenames_checkBox->setChecked(overwrite_typenames);
		}

	}//namespace View
}//namespace Ball
