// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberConfigurationDialog.C,v 1.1 2004/01/20 13:21:33 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/AmberConfigurationDialog.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

namespace BALL
{
	namespace VIEW
	{

		AmberConfigurationDialog::AmberConfigurationDialog(QWidget* parent, const char* name)
			:	AdvancedOptions(parent, name)
		{
		}

		AmberConfigurationDialog::~AmberConfigurationDialog()
		{
		}

		void AmberConfigurationDialog::browseParameterFiles()
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

		void AmberConfigurationDialog::resetOptions()
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

		const String& AmberConfigurationDialog::getFilename() const
		{
			static String filename;
			filename = parameter_file_edit->text().ascii();
			return filename;
		}
		
		void AmberConfigurationDialog::setFilename(const String& filename)
		{
			parameter_file_edit->setText(filename.c_str());
			parameter_file_edit->update();
		}
		
		bool AmberConfigurationDialog::getUseDistanceDependentDC() const
		{
			return distance_button->isChecked();
		}

		float AmberConfigurationDialog::getNonbondedCutoff()
		{
			String nonbondcut = nonbonded_cutoff_line_edit->text().ascii();
			float nonbond = nonbondcut.toFloat();
			return nonbond;
		}
		
		float AmberConfigurationDialog::getVdwCutoff()
		{
			String vdwcut = vdw_cutoff_line_edit->text().ascii();
			float vdw = vdwcut.toFloat();
			return vdw;
		}
		
		float AmberConfigurationDialog::getVdwCuton()
		{
			String vdwcut = vdw_cuton_line_edit->text().ascii();
			float vdw = vdwcut.toFloat();
			return vdw;
		}
		
		float AmberConfigurationDialog::getElectrostaticCutoff()
		{
			String elcut = electrostatic_cutoff_line_edit->text().ascii();
			float el = elcut.toFloat();
			return el;
		}
		
		float AmberConfigurationDialog::getElectrostaticCuton()
		{
			String elcut = electrostatic_cuton_line_edit->text().ascii();
			float el = elcut.toFloat();
			return el;
		}
		
		float AmberConfigurationDialog::getScalingElectrostatic_1_4()
		{
			String scaleel = scaling_electrostatic_1_4_line_edit->text().ascii();
			float scale = scaleel.toFloat();
			return scale;
		}

		float AmberConfigurationDialog::getScalingVdw_1_4()
		{
			String scalevdw = scaling_vdw_1_4_line_edit->text().ascii();
			float scale = scalevdw.toFloat();
			return scale;
		}
		
		bool AmberConfigurationDialog::getAssignCharges()
		{
			return assign_charges_checkBox->isChecked();
		}
			
		bool AmberConfigurationDialog::getAssignTypenames()
		{
			return assign_typenames_checkBox->isChecked();
		}
			
		bool AmberConfigurationDialog::getAssignTypes()
		{
			return assign_types_checkBox->isChecked();
		}
			
		bool AmberConfigurationDialog::getOverwriteCharges()
		{
			return overwrite_charges_checkBox->isChecked();
		}
			
		bool AmberConfigurationDialog::getOverwriteTypenames()
		{
			return overwrite_typenames_checkBox->isChecked();
		}

		//function to restore previously changed options
		void AmberConfigurationDialog::setOptions
					(float nonbonded_cutoff, float vdw_cutoff, 
					 float vdw_cuton, float electrostatic_cutoff, 
					 float electrostatic_cuton,
					 float scaling_electrostatic_1_4, float scaling_vdw_1_4, 
					 bool use_dddc, bool assign_charges,
					 bool assign_typenames, bool assign_types, 
					 bool overwrite_charges, bool overwrite_typenames)
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

		void AmberConfigurationDialog::writePreferences(INIFile& inifile) const
			throw()
		{
			// the AMBER options
			if (!inifile.hasSection("AMBER")) inifile.appendSection("AMBER");
			inifile.insertValue("AMBER", "Filename", getFilename());
			inifile.insertValue("AMBER", "NONBONDED_CUTOFF",nonbonded_cutoff);
			inifile.insertValue("AMBER", "VDW_CUTOFF",vdw_cutoff);
			inifile.insertValue("AMBER", "VDW_CUTON",vdw_cuton);
			inifile.insertValue("AMBER", "ELECTROSTATIC_CUTOFF",electrostatic_cutoff);
			inifile.insertValue("AMBER", "ELECTROSTATIC_CUTON",electrostatic_cuton);
			inifile.insertValue("AMBER", "SCALING_ELECTROSTATIC_1_4",scaling_electrostatic_1_4);
			inifile.insertValue("AMBER", "SCALING_VDW_1_4",scaling_vdw_1_4);
			
			inifile.insertValue("AMBER", "ASSIGN_CHARGES",assign_charges);
			inifile.insertValue("AMBER", "ASSIGN_TYPENAMES",assign_typenames);
			inifile.insertValue("AMBER", "ASSIGN_TYPES",assign_types);
			inifile.insertValue("AMBER", "OVERWRITE_CHARGES",overwrite_charges);
			inifile.insertValue("AMBER", "OVERWRITE_TYPENAMES",overwrite_typenames);
		}


		void AmberConfigurationDialog::readPreferences(const INIFile& inifile)
			throw()
		{
			// the AMBER options
			if (inifile.hasEntry("AMBER", "DistanceDependentDC"))
			{
				use_dddc = inifile.getValue("AMBER", "DistanceDependentDC").toUnsignedInt() == 1;
			}

			if (inifile.hasEntry("AMBER", "NONBONDED_CUTOFF"))
			{
				nonbonded_cutoff = inifile.getValue("AMBER", "NONBONDED_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "VDW_CUTOFF"))
			{
				vdw_cutoff = inifile.getValue("AMBER", "VDW_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "VDW_CUTON"))
			{
				vdw_cuton = inifile.getValue("AMBER", "VDW_CUTON").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "ELECTROSTATIC_CUTOFF"))
			{
				electrostatic_cutoff = inifile.getValue("AMBER", "ELECTROSTATIC_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "ELECTROSTATIC_CUTON"))
			{
				electrostatic_cuton = inifile.getValue("AMBER", "ELECTROSTATIC_CUTON").toFloat();
			}
			if (inifile.hasEntry("AMBER", "SCALING_ELECTROSTATIC_1_4"))
			{
				scaling_electrostatic_1_4 = inifile.getValue("AMBER", "SCALING_ELECTROSTATIC_1_4").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "SCALING_VDW_1_4"))
			{
				scaling_vdw_1_4 = inifile.getValue("AMBER", "SCALING_VDW_1_4").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "ASSIGN_CHARGES"))
			{
				assign_charges = inifile.getValue("AMBER", "ASSIGN_CHARGES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "ASSIGN_TYPENAMES"))
			{
				assign_typenames = inifile.getValue("AMBER", "ASSIGN_TYPENAMES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "ASSIGN_TYPES"))
			{
				assign_types = inifile.getValue("AMBER", "ASSIGN_TYPES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "OVERWRITE_CHARGES"))
			{
				overwrite_charges = inifile.getValue("AMBER", "OVERWRITE_CHARGES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "OVERWRITE_TYPENAMES"))
			{
				overwrite_typenames = inifile.getValue("AMBER", "OVERWRITE_TYPENAMES").toUnsignedInt();
			}

			if (inifile.hasEntry("AMBER", "Filename"))
			{
				setFilename(inifile.getValue("AMBER", "Filename"));
			}
		}

		float AmberConfigurationDialog::getNonbondedCutoff() const
		{
			return nonbonded_cutoff;
		}

		float AmberConfigurationDialog::getVdwCutoff() const
		{
			return vdw_cutoff;
		}

		float AmberConfigurationDialog::getVdwCuton() const
		{
			return vdw_cuton;
		}

		float AmberConfigurationDialog::getElectrostaticCutoff() const
		{
			return electrostatic_cutoff;
		}

		float AmberConfigurationDialog::getElectrostaticCuton() const
		{
			return electrostatic_cuton;
		}

		float AmberConfigurationDialog::getScalingElectrostatic_1_4() const
		{
			return scaling_electrostatic_1_4;;
		}

		float AmberConfigurationDialog::getScalingVdw_1_4() const
		{
			return scaling_vdw_1_4;;
		}

		bool AmberConfigurationDialog::getAssignCharges() const
		{
			return assign_charges;
		}

		bool AmberConfigurationDialog::getAssignTypenames() const
		{
			return assign_typenames;
		}

		bool AmberConfigurationDialog::getAssignTypes() const
		{
			return assign_types;
		}

		bool AmberConfigurationDialog::getOverwriteCharges() const
		{
			return overwrite_charges;
		}

		bool AmberConfigurationDialog::getOverwriteTypenames() const
		{
			return overwrite_typenames;
		}

		bool AmberConfigurationDialog::getUseDistanceDependentDC() const
		{
			return use_dddc;
		}
	
	}//namespace VIEW
}//namespace BALL
