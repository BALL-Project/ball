// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: charmmConfigurationDialog.C,v 1.6 2004/04/30 11:47:03 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/charmmConfigurationDialog.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace VIEW
	{

		CharmmConfigurationDialog::CharmmConfigurationDialog(QWidget* parent, const char* name)
			:	CharmmConfigurationDialogData(parent, name),
				charmm_(0)
		{
		}

		CharmmConfigurationDialog::~CharmmConfigurationDialog()
		{
		}

		void CharmmConfigurationDialog::browseParameterFiles()
		{
			// look up the full path of the parameter file
			Path p;
			String filename = p.find(getFilename());
			if (filename == "")
			{
				filename = getFilename();
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getOpenFileName(tmp, "*.ini", 0, "Select a Charmm parameter file");
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field
				setFilename(String(result.ascii()));
			}
		}

		void CharmmConfigurationDialog::resetOptions()
		{
			nonbonded_cutoff_line_edit->setText("20.000000");
			vdw_cutoff_line_edit->setText("9.000000");
			vdw_cuton_line_edit->setText("7.000000");
			electrostatic_cutoff_line_edit->setText("9.000000");
			electrostatic_cuton_line_edit->setText("7.000000");
			solvation_cutoff_line_edit->setText("9.000000");
			solvation_cuton_line_edit->setText("7.000000");
			scaling_electrostatic_1_4_line_edit->setText("1.000000");
			scaling_vdw_1_4_line_edit->setText("2.000000");

			distance_button->setChecked(true);
			constant_button->setChecked(false);
			assign_charges_checkBox->setChecked(true);
			assign_typenames_checkBox->setChecked(true);
			assign_types_checkBox->setChecked(true);
			overwrite_charges_checkBox->setChecked(true);
			overwrite_typenames_checkBox->setChecked(true);
			use_eef1_checkBox->setChecked(true);

			/*
			boundary_box->setChecked(false);
			add_solvent_box->setChecked(false);
			box_size_edit->setText("40.0000");
			solvent_file_edit->setText("");
			*/
		}

		const String& CharmmConfigurationDialog::getFilename() const
		{
			static String filename;
			filename = parameter_file_edit->text().ascii();
			return filename;
		}
		
		void CharmmConfigurationDialog::setFilename(const String& filename)
		{
			parameter_file_edit->setText(filename.c_str());
		}
		
		//function to restore previously changed options
		void CharmmConfigurationDialog::setOptions
					(float nonbonded_cutoff, float vdw_cutoff, 
					 float vdw_cuton, float electrostatic_cutoff, 
					 float electrostatic_cuton, float solvation_cutoff,
					 float solvation_cuton,
					 float scaling_electrostatic_1_4, float scaling_vdw_1_4, 
					 bool use_dddc, bool assign_charges,
					 bool assign_typenames, bool assign_types, 
					 bool overwrite_charges, bool overwrite_typenames,
					 bool use_eef1)
		{
			nonbonded_cutoff_line_edit->setText(String(nonbonded_cutoff).c_str());
			vdw_cutoff_line_edit->setText(String(vdw_cutoff).c_str());
			vdw_cuton_line_edit->setText(String(vdw_cuton).c_str());
			electrostatic_cutoff_line_edit->setText(String(electrostatic_cutoff).c_str());
			electrostatic_cuton_line_edit->setText(String(electrostatic_cuton).c_str());
			solvation_cutoff_line_edit->setText(String(solvation_cutoff).c_str());
			solvation_cuton_line_edit->setText(String(solvation_cuton).c_str());
			scaling_electrostatic_1_4_line_edit->setText(String(scaling_electrostatic_1_4).c_str());
			scaling_vdw_1_4_line_edit->setText(String(scaling_vdw_1_4).c_str());

			distance_button->setChecked(use_dddc);
			assign_charges_checkBox->setChecked(assign_charges);
			assign_typenames_checkBox->setChecked(assign_typenames);
			assign_types_checkBox->setChecked(assign_types);
			overwrite_charges_checkBox->setChecked(overwrite_charges);
			overwrite_typenames_checkBox->setChecked(overwrite_typenames);
			use_eef1_checkBox->setChecked(use_eef1);
			accept();
		}

		void CharmmConfigurationDialog::writePreferences(INIFile& inifile) const
			throw()
		{
			// the CHARMM options
			if (!inifile.hasSection("CHARMM")) inifile.appendSection("CHARMM");
			inifile.insertValue("CHARMM", "Filename", getFilename());
			inifile.insertValue("CHARMM", "NONBONDED_CUTOFF",nonbonded_cutoff_);
			inifile.insertValue("CHARMM", "VDW_CUTOFF",vdw_cutoff_);
			inifile.insertValue("CHARMM", "VDW_CUTON",vdw_cuton_);
			inifile.insertValue("CHARMM", "ELECTROSTATIC_CUTOFF",electrostatic_cutoff_);
			inifile.insertValue("CHARMM", "ELECTROSTATIC_CUTON",electrostatic_cuton_);
			inifile.insertValue("CHARMM", "SOLVATION_CUTOFF",solvation_cutoff_);
			inifile.insertValue("CHARMM", "SOLVATION_CUTON",solvation_cuton_);
			inifile.insertValue("CHARMM", "SCALING_ELECTROSTATIC_1_4",scaling_electrostatic_1_4_);
			inifile.insertValue("CHARMM", "SCALING_VDW_1_4",scaling_vdw_1_4_);
			
			inifile.insertValue("CHARMM", "ASSIGN_CHARGES",assign_charges_);
			inifile.insertValue("CHARMM", "ASSIGN_TYPENAMES",assign_typenames_);
			inifile.insertValue("CHARMM", "ASSIGN_TYPES",assign_types_);
			inifile.insertValue("CHARMM", "OVERWRITE_CHARGES",overwrite_charges_);
			inifile.insertValue("CHARMM", "OVERWRITE_TYPENAMES",overwrite_typenames_);
			inifile.insertValue("CHARMM", "USE_EEF1",use_eef1_);
		}


		void CharmmConfigurationDialog::fetchPreferences(const INIFile& inifile)
			throw()
		{
			// the CHARMM options
			if (inifile.hasEntry("CHARMM", "DistanceDependentDC"))
			{
				use_dddc_ = inifile.getValue("CHARMM", "DistanceDependentDC").toUnsignedInt() == 1;
			}

			if (inifile.hasEntry("CHARMM", "NONBONDED_CUTOFF"))
			{
				nonbonded_cutoff_ = inifile.getValue("CHARMM", "NONBONDED_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("CHARMM", "VDW_CUTOFF"))
			{
				vdw_cutoff_ = inifile.getValue("CHARMM", "VDW_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("CHARMM", "VDW_CUTON"))
			{
				vdw_cuton_ = inifile.getValue("CHARMM", "VDW_CUTON").toFloat();
			}
			
			if (inifile.hasEntry("CHARMM", "ELECTROSTATIC_CUTOFF"))
			{
				electrostatic_cutoff_ = inifile.getValue("CHARMM", "ELECTROSTATIC_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("CHARMM", "ELECTROSTATIC_CUTON"))
			{
				electrostatic_cuton_ = inifile.getValue("CHARMM", "ELECTROSTATIC_CUTON").toFloat();
			}
			
			if (inifile.hasEntry("CHARMM", "SOLVATION_CUTOFF"))
			{
				solvation_cutoff_ = inifile.getValue("CHARMM", "SOLVATION_CUTOFF").toFloat();
			}
			
			if (inifile.hasEntry("CHARMM", "SOLVATION_CUTON"))
			{
				solvation_cuton_ = inifile.getValue("CHARMM", "SOLVATION_CUTON").toFloat();
			}
			
			if (inifile.hasEntry("CHARMM", "SCALING_ELECTROSTATIC_1_4"))
			{
				scaling_electrostatic_1_4_ = inifile.getValue("CHARMM", "SCALING_ELECTROSTATIC_1_4").toFloat();
			}
			
			if (inifile.hasEntry("CHARMM", "SCALING_VDW_1_4"))
			{
				scaling_vdw_1_4_ = inifile.getValue("CHARMM", "SCALING_VDW_1_4").toFloat();
			}
			
			if (inifile.hasEntry("CHARMM", "ASSIGN_CHARGES"))
			{
				assign_charges_ = inifile.getValue("CHARMM", "ASSIGN_CHARGES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("CHARMM", "ASSIGN_TYPENAMES"))
			{
				assign_typenames_ = inifile.getValue("CHARMM", "ASSIGN_TYPENAMES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("CHARMM", "ASSIGN_TYPES"))
			{
				assign_types_ = inifile.getValue("CHARMM", "ASSIGN_TYPES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("CHARMM", "OVERWRITE_CHARGES"))
			{
				overwrite_charges_ = inifile.getValue("CHARMM", "OVERWRITE_CHARGES").toUnsignedInt();
			}
			
			if (inifile.hasEntry("CHARMM", "OVERWRITE_TYPENAMES"))
			{
				overwrite_typenames_ = inifile.getValue("CHARMM", "OVERWRITE_TYPENAMES").toUnsignedInt();
			}

			if (inifile.hasEntry("CHARMM", "USE_EEF1"))
			{
				use_eef1_ = inifile.getValue("CHARMM", "USE_EEF1").toUnsignedInt();
			}

			if (inifile.hasEntry("CHARMM", "Filename"))
			{
				setFilename(inifile.getValue("CHARMM", "Filename"));
			}
		}

		float CharmmConfigurationDialog::getNonbondedCutoff() const
		{
			return nonbonded_cutoff_;
		}

		float CharmmConfigurationDialog::getVdwCutoff() const
		{
			return vdw_cutoff_;
		}

		float CharmmConfigurationDialog::getVdwCuton() const
		{
			return vdw_cuton_;
		}

		float CharmmConfigurationDialog::getElectrostaticCutoff() const
		{
			return electrostatic_cutoff_;
		}

		float CharmmConfigurationDialog::getElectrostaticCuton() const
		{
			return electrostatic_cuton_;
		}

		float CharmmConfigurationDialog::getSolvationCutoff() const
		{
			return solvation_cutoff_;
		}

		float CharmmConfigurationDialog::getSolvationCuton() const
		{
			return solvation_cuton_;
		}

		float CharmmConfigurationDialog::getScalingElectrostatic_1_4() const
		{
			return scaling_electrostatic_1_4_;
		}

		float CharmmConfigurationDialog::getScalingVdw_1_4() const
		{
			return scaling_vdw_1_4_;
		}

		bool CharmmConfigurationDialog::getAssignCharges() const
		{
			return assign_charges_;
		}

		bool CharmmConfigurationDialog::getAssignTypenames() const
		{
			return assign_typenames_;
		}

		bool CharmmConfigurationDialog::getAssignTypes() const
		{
			return assign_types_;
		}

		bool CharmmConfigurationDialog::getOverwriteCharges() const
		{
			return overwrite_charges_;
		}

		bool CharmmConfigurationDialog::getOverwriteTypenames() const
		{
			return overwrite_typenames_;
		}

		bool CharmmConfigurationDialog::getUseDistanceDependentDC() const
		{
			return use_dddc_;
		}

		bool CharmmConfigurationDialog::getUseEEF1() const
		{
			return use_eef1_;
		}

		void CharmmConfigurationDialog::reject()
		{
			hide();

			nonbonded_cutoff_line_edit->setText(String(nonbonded_cutoff_).c_str());
			vdw_cutoff_line_edit->setText(String(vdw_cutoff_).c_str());
			vdw_cuton_line_edit->setText(String(vdw_cuton_).c_str());
			electrostatic_cutoff_line_edit->setText(String(electrostatic_cutoff_).c_str());
			electrostatic_cuton_line_edit->setText(String(electrostatic_cuton_).c_str());
			solvation_cutoff_line_edit->setText(String(solvation_cutoff_).c_str());
			solvation_cuton_line_edit->setText(String(solvation_cuton_).c_str());
			scaling_electrostatic_1_4_line_edit->setText(String(scaling_electrostatic_1_4_).c_str());
			scaling_vdw_1_4_line_edit->setText(String(scaling_vdw_1_4_).c_str());

			distance_button->setChecked(use_dddc_);
			assign_charges_checkBox->setChecked(assign_charges_);
			assign_typenames_checkBox->setChecked(assign_typenames_);
			assign_types_checkBox->setChecked(assign_types_);
			overwrite_charges_checkBox->setChecked(overwrite_charges_);
			overwrite_typenames_checkBox->setChecked(overwrite_typenames_);
			use_eef1_checkBox->setChecked(use_eef1_);
			setResult(0);
		}

		void CharmmConfigurationDialog::accept()
		{
			try
			{
				nonbonded_cutoff_ = String(nonbonded_cutoff_line_edit->text().ascii()).toFloat();
				vdw_cutoff_ = String(vdw_cutoff_line_edit->text().ascii()).toFloat();
				vdw_cuton_ = String(vdw_cuton_line_edit->text().ascii()).toFloat();
				electrostatic_cutoff_ = String(electrostatic_cutoff_line_edit->text().ascii()).toFloat();
				electrostatic_cuton_ = String(electrostatic_cuton_line_edit->text().ascii()).toFloat();
				solvation_cutoff_ = String(solvation_cutoff_line_edit->text().ascii()).toFloat();
				solvation_cuton_ = String(solvation_cuton_line_edit->text().ascii()).toFloat();

				scaling_electrostatic_1_4_ = String(scaling_electrostatic_1_4_line_edit->text().ascii()).toFloat();
				scaling_vdw_1_4_ = String(scaling_vdw_1_4_line_edit->text().ascii()).toFloat();
			}
			catch(Exception::GeneralException e)
			{
				Log.error() << "Invalid value: " << std::endl << e << std::endl;
				return;
			}

			use_dddc_ = distance_button->isChecked();
			assign_charges_ = assign_charges_checkBox->isChecked();
			assign_types_ = assign_types_checkBox->isChecked();
			assign_typenames_ = assign_typenames_checkBox->isChecked();
			overwrite_typenames_ = overwrite_typenames_checkBox->isChecked();
			overwrite_charges_ = overwrite_charges_checkBox->isChecked();
			use_eef1_ = use_eef1_checkBox->isChecked();

			if (charmm_ != 0) applyTo(*charmm_);
			setResult(1);
			hide();
		}


		void CharmmConfigurationDialog::applyTo(CharmmFF& charmm)
			throw()
		{
			charmm.options[CharmmFF::Option::ASSIGN_TYPES] = (getAssignTypes() ? "true" : "false");
			charmm.options[CharmmFF::Option::ASSIGN_CHARGES] = (getAssignCharges() ? "true" : "false");
			charmm.options[CharmmFF::Option::ASSIGN_TYPENAMES] = (getAssignTypenames() ? "true" : "false");
			charmm.options[CharmmFF::Option::OVERWRITE_CHARGES] = (getOverwriteCharges() ? "true" : "false");
			charmm.options[CharmmFF::Option::OVERWRITE_TYPENAMES] = (getOverwriteTypenames() ? "true" : "false");

			charmm.options[CharmmFF::Option::DISTANCE_DEPENDENT_DIELECTRIC] = getUseDistanceDependentDC();
			charmm.options[CharmmFF::Option::USE_EEF1] = getUseEEF1();
			charmm.options[CharmmFF::Option::NONBONDED_CUTOFF] = getNonbondedCutoff();
			charmm.options[CharmmFF::Option::VDW_CUTOFF] = getVdwCutoff();
			charmm.options[CharmmFF::Option::VDW_CUTON] = getVdwCuton();
			charmm.options[CharmmFF::Option::ELECTROSTATIC_CUTOFF] = getElectrostaticCutoff();
			charmm.options[CharmmFF::Option::ELECTROSTATIC_CUTON] = getElectrostaticCuton();
			charmm.options[CharmmFF::Option::SOLVATION_CUTOFF] = getSolvationCutoff();
			charmm.options[CharmmFF::Option::SOLVATION_CUTON] = getSolvationCuton();
			charmm.options[CharmmFF::Option::SCALING_ELECTROSTATIC_1_4] = getScalingElectrostatic_1_4();
			charmm.options[CharmmFF::Option::SCALING_VDW_1_4] = getScalingVdw_1_4();

			charmm.options[CharmmFF::Option::FILENAME] = getFilename();

			/*
			if (boundary_box->isChecked())
			{
				charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "true";
				try
				{
					charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_DISTANCE] = 
						String(box_size_edit->text().ascii()).toFloat();
				}
				catch(...)
				{
					Log.error() << "Invalid distance for Periodic Boundary choosen." << std::endl;
				}
				if (add_solvent_box->isChecked())
				{
					charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "true";

					if (solvent_file_edit->text() != "")
					{
						charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_SOLVENT_FILE] = 
							solvent_file_edit->text().ascii();
					}
				}
			}
			else
			{
				charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_ENABLED] = "false";
				charmm.options[PeriodicBoundary::Option::PERIODIC_BOX_ADD_SOLVENT] = "false";
			}
		*/
		}

		void CharmmConfigurationDialog::setCharmmFF(CharmmFF& charmm)
			throw()
		{
			charmm_ = &charmm;
		}

		void CharmmConfigurationDialog::chooseSolventFile()
			throw()
		{
			/*
			QString result = QFileDialog::getOpenFileName(
					solvent_file_edit->text().ascii(), "", 0, "Select a solvent file");
			if (!result.isEmpty())
			{
				solvent_file_edit->setText(result);
			}
			*/
		}
	
		void CharmmConfigurationDialog::periodicBoundaryClicked()
			throw()
		{
			/*
			if (boundary_box->isChecked())
			{
				add_solvent_box->setEnabled(true);
				solvent_file_edit->setEnabled(true);
				box_size_edit->setEnabled(true);
				browse_solvent_file->setEnabled(true);
			}
			else
			{
				add_solvent_box->setEnabled(false);
				solvent_file_edit->setEnabled(false);
				box_size_edit->setEnabled(false);
				add_solvent_box->setChecked(false);
				browse_solvent_file->setEnabled(false);
			}
			*/
		}
	
	}//namespace VIEW
}//namespace BALL
