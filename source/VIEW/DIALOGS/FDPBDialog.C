// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FDPBDialog.C,v 1.10 2004/04/21 15:06:13 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

namespace BALL
{
	namespace VIEW
	{

		FDPBDialog::FDPBDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
			: FDPBDialogData(parent, name, modal, fl),
				ModularWidget("FDPBDialog")
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new FDPBDialog " << this << std::endl;
		#endif
			registerWidget(this);
		}

		FDPBDialog::~FDPBDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class FDPBDialog" << std::endl;
			#endif 
		}

		// ------------------------- SLOTS ------------------------------------------------
		// --------------------------------------------------------------------------------

		void FDPBDialog::browseChargesData()
		{
			selectFile_(*charges_data_lineedit);
		}

		void FDPBDialog::browseChargesRules()
		{
			selectFile_(*charges_rules_lineedit);
		}

		void FDPBDialog::browseRadiiData()
		{
			selectFile_(*radii_data_lineedit);
		}

		void FDPBDialog::browseRadiiRules()
		{
			selectFile_(*radii_rules_lineedit);
		}

		void FDPBDialog::cancelPressed()
		{
			hide();
		}

		void FDPBDialog::okPressed()
		{
			calculate();
			hide();
		}

		void FDPBDialog::resetPressed()
		{
			reset();
		}

		// ------------------------- helper functions -------------------------------------
		// --------------------------------------------------------------------------------

		void FDPBDialog::selectFile_(QLineEdit& lineedit)
			throw()
		{
			QString s = QFileDialog::getOpenFileName(
										getWorkingDir().c_str(),
										"",
										getMainControl(),
										"",
										"Choose a file" );

			if (s == QString::null) return;
			setWorkingDirFromFilename_(s.ascii());
			lineedit.setText(s);

			QWidget::update();
		}

		void FDPBDialog::fetchPreferences(INIFile& file)
			throw()
		{
			fetchPreference_(file, "radii_data_file", *radii_data_lineedit);
			fetchPreference_(file, "radii_rules_file", *radii_rules_lineedit);
			fetchPreference_(file, "charges_data_file", *charges_data_lineedit);
			fetchPreference_(file, "charges_rules_file", *charges_rules_lineedit);
			
			fetchPreference_(file, "dc_solvent", *dc_solvent);
			fetchPreference_(file, "dc_interior", *dc_interior);
			fetchPreference_(file, "probe_radius", *probe_radius);
			fetchPreference_(file, "ionic_strenght", *ionic_strenght);
			fetchPreference_(file, "ion_radius", *ion_radius);
			fetchPreference_(file, "spacing", *spacing);
			fetchPreference_(file, "border", *border);
			fetchPreference_(file, "temperature", *temperature);
			fetchPreference_(file, "max_iterations", *max_iterations);

			charges_rules_button->setChecked(file.hasEntry("FDPB", "charges_from_rules"));
			radii_rules_button->setChecked(file.hasEntry("FDPB", "radii_from_rules"));

			if (file.hasEntry("FDPB", "boundary"))
			{
				boundary_group->setButton(file.getValue("FDPB", "boundary").toUnsignedInt());
			}

			if (file.hasEntry("FDPB", "smoothing"))
			{
				smoothing_group->setButton(file.getValue("FDPB", "smoothing").toUnsignedInt());
			}
			if (file.hasEntry("FDPB", "charge_distribution"))
			{
				charge_distribution_group->setButton(file.getValue("FDPB", "charge_distribution").toUnsignedInt());
			}
			
			normalize_names->setChecked(file.hasEntry("FDPB", "normalize_names"));
			assign_charges->setChecked(file.hasEntry("FDPB", "assign_charges"));
			assign_radii->setChecked(file.hasEntry("FDPB", "assign_radii"));
			build_bonds->setChecked(file.hasEntry("FDPB", "build_bonds"));
			add_hydrogens->setChecked(file.hasEntry("FDPB", "add_hydrogens"));
		}

		void FDPBDialog::fetchPreference_(INIFile& inifile, const String& entry, 
																			QLineEdit& lineedit)
			throw()
		{
			if (!inifile.hasEntry("FDPB", entry)) return;
			lineedit.setText(inifile.getValue("FDPB", entry).c_str());
		}

		void FDPBDialog::writePreference_(INIFile& inifile, const String& entry, 
																			const QLineEdit& lineedit) const
			throw()
		{
			inifile.insertValue("FDPB", entry, lineedit.text().ascii());
		}


		void FDPBDialog::writePreferences(INIFile& file)
			throw()
		{
			file.appendSection("FDPB");
			writePreference_(file, "radii_data_file", *radii_data_lineedit);
			writePreference_(file, "radii_rules_file", *radii_rules_lineedit);
			writePreference_(file, "charges_data_file", *charges_data_lineedit);
			writePreference_(file, "charges_rules_file", *charges_rules_lineedit);
			
			writePreference_(file, "dc_solvent", *dc_solvent);
			writePreference_(file, "dc_interior", *dc_interior);
			writePreference_(file, "probe_radius", *probe_radius);
			writePreference_(file, "ionic_strenght", *ionic_strenght);
			writePreference_(file, "ion_radius", *ion_radius);
			writePreference_(file, "spacing", *spacing);
			writePreference_(file, "border", *border);
			writePreference_(file, "temperature", *temperature);
			writePreference_(file, "max_iterations", *max_iterations);

			if (charges_rules_button->isChecked())
			{
				file.insertValue("FDPB", "charges_from_rules", "1");
			}
			if (radii_rules_button->isChecked())
			{
				file.insertValue("FDPB", "radii_from_rules", "1");
			}

			Position pos = 0;
			if (boundary_zero->isChecked())
			{
				pos = 0;
			}
			else if (boundary_debye->isChecked())
			{
				pos = 1;
			}
			else if (boundary_coulomb->isChecked())
			{
				pos = 2;
			}
			else 
			{
				pos = 3;
			}
			file.insertValue("FDPB", "boundary", String(pos));

			// ----------- smoothing ---------------------------
			pos = 0;
			if (smoothing_none->isChecked())
			{
				pos = 0;
			}
			else if (smoothing_uniform->isChecked())
			{
				pos = 1;
			}
			else 
			{
				pos = 2;
			}
			file.insertValue("FDPB", "smoothing", String(pos));

			// ----------- charge distribution -----------------
			pos = 1;
			if (charge_uniform->isChecked())
			{
				pos = 0;
			}
			else if (charge_trilinear->isChecked())
			{
				pos = 1;
			}
			file.insertValue("FDPB", "charge_distribution", String(pos));

			// ----------- processors ---------------------------
			if (normalize_names->isChecked()) file.insertValue("FDPB", "normalize_names", "1");
			if (assign_charges->isChecked()) file.insertValue("FDPB", "assign_charges", "1");
			if (assign_radii->isChecked()) file.insertValue("FDPB", "assign_radii", "1");
			if (build_bonds->isChecked()) file.insertValue("FDPB", "build_bonds", "1");
			if (add_hydrogens->isChecked()) file.insertValue("FDPB", "add_hydrogens", "1");
		}

		bool FDPBDialog::calculate()
			throw()
		{
			if (system_ == 0) system_ = getMainControl()->getSelectedSystem();
			if (system_ == 0)
			{
				Log.error() << "No system given! Aborting..." << std::endl;
				return false;
			}
			
			applyValues_();
			applyProcessors_();
			fdpb_.setup(*system_, options_);
			fdpb_.solve();
			RegularData3DMessage* message = new RegularData3DMessage(RegularData3DMessage::NEW); 
			message->setRegularData3D(*fdpb_.phi_grid);
			message->setComposite(*system_);
			message->setCompositeName("FDPB_" + system_->getName());
			notify_(message);
			fdpb_.phi_grid = 0;
			system_ = 0;
			return true;
		}

		void FDPBDialog::reset()
			throw()
		{
			radii_data_lineedit->setText("radii/PARSE.siz");
			radii_rules_lineedit->setText("solvation/PARSE.rul");
			charges_data_lineedit->setText("charges/PARSE.crg");
			charges_rules_lineedit->setText("solvation/PARSE.rul");

			dc_solvent->setText("78.0");
			dc_interior->setText("2.0");
			probe_radius->setText("1.4");
			ionic_strenght->setText("0.0");
			ion_radius->setText("2.0");
			spacing->setText("1.0");
			border->setText("5.0");
			temperature->setText("298");
			max_iterations->setText("1000");

			boundary_zero->setChecked(true);
			smoothing_none->setChecked(true);
			charge_trilinear->setChecked(true);

			normalize_names->setChecked(false);
			assign_charges->setChecked(true);
			assign_radii->setChecked(true);
			build_bonds->setChecked(false);
			add_hydrogens->setChecked(false);
		}

		void FDPBDialog::applyValues_()
			throw()
		{
			/// ------------------------------
			options_[FDPB::Option::SOLVENT_DC] 						= String(dc_solvent->text().ascii()).toFloat();
			options_[FDPB::Option::SOLUTE_DC] 						= String(dc_interior->text().ascii()).toFloat();
			options_[FDPB::Option::PROBE_RADIUS] 					= String(probe_radius->text().ascii()).toFloat();
			options_[FDPB::Option::IONIC_STRENGTH] 				= String(ionic_strenght->text().ascii()).toFloat();
			options_[FDPB::Option::ION_RADIUS] 						= String(ion_radius->text().ascii()).toFloat();
			options_[FDPB::Option::SPACING] 							= String(spacing->text().ascii()).toFloat();
			options_[FDPB::Option::BORDER]								= String(border->text().ascii()).toFloat();
			options_[FDPB::Option::TEMPERATURE]						= String(temperature->text().ascii()).toFloat();
			options_[FDPB::Option::MAX_ITERATIONS]				= String(max_iterations->text().ascii()).toFloat();

			/// ------------------------------
			if (boundary_zero->isChecked())
			{
				options_[FDPB::Option::BOUNDARY] = FDPB::Boundary::ZERO;
			}
			else if (boundary_debye->isChecked())
			{
				options_[FDPB::Option::BOUNDARY] = FDPB::Boundary::DEBYE;
			}
			else if (boundary_coulomb->isChecked())
			{
				options_[FDPB::Option::BOUNDARY] = FDPB::Boundary::COULOMB;
			}
			else if (boundary_dipole->isChecked())
			{
				options_[FDPB::Option::BOUNDARY] = FDPB::Boundary::DIPOLE;
			}
			else if (boundary_focusing->isChecked())
			{
				options_[FDPB::Option::BOUNDARY] = FDPB::Boundary::FOCUSING;
			}
			
			/// ------------------------------
			if (smoothing_none->isChecked())
			{
				options_[FDPB::Option::DIELECTRIC_SMOOTHING] = FDPB::DielectricSmoothing::NONE;
			}
			else if (smoothing_uniform->isChecked())
			{
				options_[FDPB::Option::DIELECTRIC_SMOOTHING] = FDPB::DielectricSmoothing::UNIFORM;
			}
			else if (smoothing_harmonic->isChecked())
			{
				options_[FDPB::Option::DIELECTRIC_SMOOTHING] = FDPB::DielectricSmoothing::HARMONIC;
			}

			/// ------------------------------
			if (charge_uniform->isChecked())
			{
				options_[FDPB::Option::CHARGE_DISTRIBUTION] = FDPB::ChargeDistribution::UNIFORM;
			}
			else if (charge_trilinear->isChecked())
			{
				options_[FDPB::Option::CHARGE_DISTRIBUTION] = FDPB::ChargeDistribution::TRILINEAR;
			}

			options_[FDPB::Option::VERBOSITY] = 100;
		}

		bool FDPBDialog::applyProcessors_()
			throw()
		{
			if (system_ == 0)
			{
				Log.error() << "No system given! Aborting..." << std::endl;
				return false;
			}

			if (add_hydrogens->isChecked())
			{
				if (!system_->apply(getFragmentDB().add_hydrogens)) return false;
				if (!system_->apply(getFragmentDB().normalize_names)) return false;
			}
			else if (normalize_names->isChecked())
			{
				if (!system_->apply(getFragmentDB().normalize_names)) return false;
			}

			if (build_bonds->isChecked())
			{
				if (!system_->apply(getFragmentDB().build_bonds)) return false;
			}

			try
			{
				if (assign_charges->isChecked())
				{
					if (charges_data_button->isChecked())
					{
						charge_processor_.setFilename(charges_data_lineedit->text().ascii());
						if (!system_->apply(charge_processor_)) return false;
					}
					else
					{
						INIFile inifile(String(charges_rules_lineedit->text().ascii()));
						charge_rule_processor_ = ChargeRuleProcessor(inifile);
						if (!system_->apply(charge_rule_processor_)) return false;
					}
				}
				
				if (assign_radii->isChecked())
				{
					if (radii_data_button->isChecked())
					{
						radius_processor_.setFilename(radii_data_lineedit->text().ascii());
						if (!system_->apply(radius_processor_)) return false;
					}
					else
					{
						INIFile inifile(String(radii_rules_lineedit->text().ascii()));
						radius_rule_processor_ = RadiusRuleProcessor(inifile);
						if (!system_->apply(radius_rule_processor_)) return false;
					}
				}
			}
			catch (Exception::FileNotFound e)
			{
				Log.error() << "Invalid file " << e.getFilename() << std::endl;
				return false;
			}

			CompositeMessage* message = new CompositeMessage;
			message->setComposite(*system_);
			message->setType(CompositeMessage::CHANGED_COMPOSITE);
			notify_(message);

			return true;
		}

	} // namespace VIEW
} // namespace BALL
