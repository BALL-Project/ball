// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amberConfigurationDialog.C,v 1.2 2004/01/20 15:07:42 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/MOLMEC/AMBER/amber.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

namespace BALL
{
	namespace VIEW
	{

		AmberConfigurationDialog::AmberConfigurationDialog(QWidget* parent, const char* name)
			:	AmberConfigurationDialogData(parent, name),
				amber_(0)
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
			accept();
		}

		void AmberConfigurationDialog::writePreferences(INIFile& inifile) const
			throw()
		{
			// the AMBER options
			if (!inifile.hasSection("AMBER")) inifile.appendSection("AMBER");
			inifile.insertValue("AMBER", "Filename", getFilename());
			inifile.insertValue("AMBER", "NONBONDED_CUTOFF",nonbonded_cutoff_);
			inifile.insertValue("AMBER", "VDW_CUTOFF",vdw_cutoff_);
			inifile.insertValue("AMBER", "VDW_CUTON",vdw_cuton_);
			inifile.insertValue("AMBER", "ELECTROSTATIC_CUTOFF",electrostatic_cutoff_);
			inifile.insertValue("AMBER", "ELECTROSTATIC_CUTON",electrostatic_cuton_);
			inifile.insertValue("AMBER", "SCALING_ELECTROSTATIC_1_4",scaling_electrostatic_1_4_);
			inifile.insertValue("AMBER", "SCALING_VDW_1_4",scaling_vdw_1_4_);
			
			inifile.insertValue("AMBER", "ASSIGN_CHARGES",assign_charges_);
			inifile.insertValue("AMBER", "ASSIGN_TYPENAMES",assign_typenames_);
			inifile.insertValue("AMBER", "ASSIGN_TYPES",assign_types_);
			inifile.insertValue("AMBER", "OVERWRITE_CHARGES",overwrite_charges_);
			inifile.insertValue("AMBER", "OVERWRITE_TYPENAMES",overwrite_typenames_);
		}


		void AmberConfigurationDialog::fetchPreferences(const INIFile& inifile)
			throw()
		{
			// the AMBER options
			if (inifile.hasEntry("AMBER", "DistanceDependentDC"))
			{
				use_dddc_ = inifile.getValue("AMBER", "DistanceDependentDC").toUnsignedInt() == 1;
			}

			if (inifile.hasEntry("AMBER", "NONBONDED_CUTOFF"))
			{
				nonbonded_cutoff_ = inifile.getValue("AMBER", "NONBONDED_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "VDW_CUTOFF"))
			{
				vdw_cutoff_ = inifile.getValue("AMBER", "VDW_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "VDW_CUTON"))
			{
				vdw_cuton_ = inifile.getValue("AMBER", "VDW_CUTON").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "ELECTROSTATIC_CUTOFF"))
			{
				electrostatic_cutoff_ = inifile.getValue("AMBER", "ELECTROSTATIC_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "ELECTROSTATIC_CUTON"))
			{
				electrostatic_cuton_ = inifile.getValue("AMBER", "ELECTROSTATIC_CUTON").toFloat();
			}
			if (inifile.hasEntry("AMBER", "SCALING_ELECTROSTATIC_1_4"))
			{
				scaling_electrostatic_1_4_ = inifile.getValue("AMBER", "SCALING_ELECTROSTATIC_1_4").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "SCALING_VDW_1_4"))
			{
				scaling_vdw_1_4_ = inifile.getValue("AMBER", "SCALING_VDW_1_4").toFloat();
			}
			
			if (inifile.hasEntry("AMBER", "ASSIGN_CHARGES"))
			{
				assign_charges_ = inifile.getValue("AMBER", "ASSIGN_CHARGES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "ASSIGN_TYPENAMES"))
			{
				assign_typenames_ = inifile.getValue("AMBER", "ASSIGN_TYPENAMES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "ASSIGN_TYPES"))
			{
				assign_types_ = inifile.getValue("AMBER", "ASSIGN_TYPES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "OVERWRITE_CHARGES"))
			{
				overwrite_charges_ = inifile.getValue("AMBER", "OVERWRITE_CHARGES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("AMBER", "OVERWRITE_TYPENAMES"))
			{
				overwrite_typenames_ = inifile.getValue("AMBER", "OVERWRITE_TYPENAMES").toUnsignedInt();
			}

			if (inifile.hasEntry("AMBER", "Filename"))
			{
				setFilename(inifile.getValue("AMBER", "Filename"));
			}
		}

		float AmberConfigurationDialog::getNonbondedCutoff() const
		{
			return nonbonded_cutoff_;
		}

		float AmberConfigurationDialog::getVdwCutoff() const
		{
			return vdw_cutoff_;
		}

		float AmberConfigurationDialog::getVdwCuton() const
		{
			return vdw_cuton_;
		}

		float AmberConfigurationDialog::getElectrostaticCutoff() const
		{
			return electrostatic_cutoff_;
		}

		float AmberConfigurationDialog::getElectrostaticCuton() const
		{
			return electrostatic_cuton_;
		}

		float AmberConfigurationDialog::getScalingElectrostatic_1_4() const
		{
			return scaling_electrostatic_1_4_;
		}

		float AmberConfigurationDialog::getScalingVdw_1_4() const
		{
			return scaling_vdw_1_4_;
		}

		bool AmberConfigurationDialog::getAssignCharges() const
		{
			return assign_charges_;
		}

		bool AmberConfigurationDialog::getAssignTypenames() const
		{
			return assign_typenames_;
		}

		bool AmberConfigurationDialog::getAssignTypes() const
		{
			return assign_types_;
		}

		bool AmberConfigurationDialog::getOverwriteCharges() const
		{
			return overwrite_charges_;
		}

		bool AmberConfigurationDialog::getOverwriteTypenames() const
		{
			return overwrite_typenames_;
		}

		bool AmberConfigurationDialog::getUseDistanceDependentDC() const
		{
			return use_dddc_;
		}

		void AmberConfigurationDialog::reject()
		{
			hide();

			nonbonded_cutoff_line_edit->setText(String(nonbonded_cutoff_).c_str());
			vdw_cutoff_line_edit->setText(String(vdw_cutoff_).c_str());
			vdw_cuton_line_edit->setText(String(vdw_cuton_).c_str());
			electrostatic_cutoff_line_edit->setText(String(electrostatic_cutoff_).c_str());
			electrostatic_cuton_line_edit->setText(String(electrostatic_cuton_).c_str());
			scaling_electrostatic_1_4_line_edit->setText(String(scaling_electrostatic_1_4_).c_str());
			scaling_vdw_1_4_line_edit->setText(String(scaling_vdw_1_4_).c_str());

			distance_button->setChecked(use_dddc_);
			assign_charges_checkBox->setChecked(assign_charges_);
			assign_typenames_checkBox->setChecked(assign_typenames_);
			assign_types_checkBox->setChecked(assign_types_);
			overwrite_charges_checkBox->setChecked(overwrite_charges_);
			overwrite_typenames_checkBox->setChecked(overwrite_typenames_);
		}

		void AmberConfigurationDialog::accept()
		{
			hide();

			try
			{
				nonbonded_cutoff_ = String(nonbonded_cutoff_line_edit->text().ascii()).toFloat();
				vdw_cutoff_ = String(vdw_cutoff_line_edit->text().ascii()).toFloat();
				vdw_cuton_ = String(vdw_cuton_line_edit->text().ascii()).toFloat();
				electrostatic_cutoff_ = String(electrostatic_cutoff_line_edit->text().ascii()).toFloat();
				electrostatic_cuton_ = String(electrostatic_cuton_line_edit->text().ascii()).toFloat();
				scaling_electrostatic_1_4_ = String(scaling_electrostatic_1_4_line_edit->text().ascii()).toFloat();
				scaling_vdw_1_4_ = String(scaling_vdw_1_4_line_edit->text().ascii()).toFloat();
			}
			catch(Exception::GeneralException e)
			{
				Log.error() << "Invalid value: " << std::endl << e << std::endl;
			}

			use_dddc_ = distance_button->isChecked();
			assign_charges_ = assign_charges_checkBox->isChecked();
			assign_types_ = assign_types_checkBox->isChecked();
			assign_typenames_ = assign_typenames_checkBox->isChecked();
			overwrite_typenames_ = overwrite_typenames_checkBox->isChecked();
			overwrite_charges_ = overwrite_charges_checkBox->isChecked();

			if (amber_ != 0) applyTo(*amber_);
		}


		void AmberConfigurationDialog::applyTo(AmberFF& amber)
			throw()
		{
			amber.options[AmberFF::Option::ASSIGN_TYPES] = getAssignTypes();
			amber.options[AmberFF::Option::ASSIGN_CHARGES] = getAssignCharges();
			amber.options[AmberFF::Option::ASSIGN_TYPENAMES] = getAssignTypenames();
			amber.options[AmberFF::Option::OVERWRITE_CHARGES] = getOverwriteCharges();
			amber.options[AmberFF::Option::OVERWRITE_TYPENAMES] = getOverwriteTypenames();
			amber.options[AmberFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = getUseDistanceDependentDC();
			amber.options[AmberFF::Option::NONBONDED_CUTOFF] = getNonbondedCutoff();
			amber.options[AmberFF::Option::VDW_CUTOFF] = getVdwCutoff();
			amber.options[AmberFF::Option::VDW_CUTON] = getVdwCuton();
			amber.options[AmberFF::Option::ELECTROSTATIC_CUTOFF] = getElectrostaticCutoff();
			amber.options[AmberFF::Option::ELECTROSTATIC_CUTON] = getElectrostaticCuton();
			amber.options[AmberFF::Option::SCALING_ELECTROSTATIC_1_4] = getScalingElectrostatic_1_4();
			amber.options[AmberFF::Option::SCALING_VDW_1_4] = getScalingVdw_1_4();

			amber.options[AmberFF::Option::FILENAME] = getFilename();
		}

		void AmberConfigurationDialog::setAmberFF(AmberFF& amber)
			throw()
		{
			amber_ = &amber;
		}
	
	}//namespace VIEW
}//namespace BALL
