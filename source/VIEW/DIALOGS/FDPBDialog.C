// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FDPBDialog.C,v 1.19.4.2 2005/10/14 11:39:42 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/threads.h>

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

		FDPBDialog::FDPBDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			: FDPBDialogData(parent, name, modal, fl),
				ModularWidget("FDPBDialog"),
				system_(0),
				thread_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new FDPBDialog " << this << std::endl;
		#endif
			registerWidget(this);

			setINIFileSectionName("FDPB");

			registerObject_(radii_data_lineedit);
			registerObject_(radii_rules_lineedit);
			registerObject_(charges_data_lineedit);
			registerObject_(charges_rules_lineedit);
			registerObject_(dc_solvent);
			registerObject_(dc_interior);
			registerObject_(probe_radius);
			registerObject_(ionic_strenght);
			registerObject_(ion_radius);
			registerObject_(spacing);
			registerObject_(border);
			registerObject_(temperature);
			registerObject_(max_iterations);
			registerObject_(boundary_group);
			registerObject_(smoothing_group);
			registerObject_(charge_distribution_group);
			registerObject_(radii_group);
			registerObject_(charges_group);
			registerObject_(normalize_names);
			registerObject_(assign_charges);
			registerObject_(assign_radii);
			registerObject_(build_bonds);
			registerObject_(add_hydrogens);
		}

		FDPBDialog::~FDPBDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class FDPBDialog" << std::endl;
			#endif 

			#ifdef BALL_QT_HAS_THREADS
				if (thread_ != 0)
				{
					if (thread_->running()) thread_->terminate();
					if (thread_->running()) thread_->wait();
					delete thread_;
				}
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
			hide();
			calculate();
		}

		void FDPBDialog::resetPressed()
		{
			restoreDefaultValues();
		}

		// ------------------------- helper functions -------------------------------------
		// --------------------------------------------------------------------------------

		void FDPBDialog::selectFile_(QLineEdit& lineedit)
			throw()
		{
			QString s = QFileDialog::getOpenFileName(
										"",
										"",
										getMainControl(),
										"",
										"Choose a file" );

			if (s == QString::null) return;
			lineedit.setText(s);
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

			if (!lockComposites()) return false;
			#ifdef BALL_QT_HAS_THREADS_
				if (thread_ == 0)
				{
					thread_ = new CalculateFDPBThread();
				}
				else
				{
					if (thread_->running())
					{
						Log.error() << "Thread already running in"  << " "  << __FILE__ << "  " << __LINE__<< std::endl;
						return false;
					}
				}

				thread_->setFDPBDialog(this);
				thread_->start();

				Position pos = 3;
				String dots;
				while (thread_->running())
				{
					setStatusbarText("Calculating FDPB grid " + dots, false);
					qApp->wakeUpGuiThread();
					qApp->processEvents();
					if (pos < 40) 
					{
						pos ++;
						dots +="..";
					}
					else 
					{
						pos = 3;
						dots = "...";
					}
					thread_->wait(500); 
				}
					
				setStatusbarText("Finished FDPB grid", true);
			#else
				calculate_();
			#endif
			
			RegularData3DMessage* message = new RegularData3DMessage(RegularData3DMessage::NEW); 
			message->setData(*fdpb_.phi_grid);
			message->setComposite(*system_);
			message->setCompositeName("FDPB_" + system_->getName());
			notify_(message);
			fdpb_.phi_grid = 0;
			system_ = 0;

			unlockComposites();
			return true;
		}

		void FDPBDialog::calculate_()
		{
			applyValues_();
			applyProcessors_();
			fdpb_.setup(*system_, options_);
			fdpb_.solve();
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
			message->setType(CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
			notify_(message);

			return true;
		}

	} // namespace VIEW
} // namespace BALL
