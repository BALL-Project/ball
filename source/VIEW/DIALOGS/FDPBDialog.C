// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FDPBDialog.C,v 1.20.2.3 2006/02/01 14:15:03 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/threads.h>

#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		FDPBDialog::FDPBDialog(QWidget* parent,  const char* name, bool modal, Qt::WFlags fl)
			: QDialog(parent, fl),
				Ui_FDPBDialogData(),
				ModularWidget("FDPBDialog"),
				system_(0),
				thread_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new FDPBDialog " << this << std::endl;
		#endif
			registerWidget(this);

			setINIFileSectionName("FDPB");

			setupUi(this);
			setObjectName(name);
			setModal(modal);
			
			// signals and slots connections
			connect( radii_data_browse, SIGNAL( clicked() ), this, SLOT( browseRadiiData() ) );
			connect( radii_rules_browse, SIGNAL( clicked() ), this, SLOT( browseRadiiRules() ) );
			connect( ok_button, SIGNAL( clicked() ), this, SLOT( okPressed() ) );
			connect( cancel_button, SIGNAL( clicked() ), this, SLOT( cancelPressed() ) );
			connect( reset_button, SIGNAL( clicked() ), this, SLOT( resetPressed() ) );
			connect( charges_rules_browse, SIGNAL( pressed() ), this, SLOT( browseChargesRules() ) );
			connect( charges_data_browse, SIGNAL( clicked() ), this, SLOT( browseChargesData() ) );

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

				if (thread_ != 0)
				{
					if (thread_->isRunning()) thread_->terminate();
					if (thread_->isRunning()) thread_->wait();
					delete thread_;
				}
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
										0,
										"Choose a file",
										"",
										"");

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
			// currently doesnt work:
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
			options_[FDPB::Option::SOLVENT_DC] 						= ascii(dc_solvent->text()).toFloat();
			options_[FDPB::Option::SOLUTE_DC] 						= ascii(dc_interior->text()).toFloat();
			options_[FDPB::Option::PROBE_RADIUS] 					= ascii(probe_radius->text()).toFloat();
			options_[FDPB::Option::IONIC_STRENGTH] 				= ascii(ionic_strenght->text()).toFloat();
			options_[FDPB::Option::ION_RADIUS] 						= ascii(ion_radius->text()).toFloat();
			options_[FDPB::Option::SPACING] 							= ascii(spacing->text()).toFloat();
			options_[FDPB::Option::BORDER]								= ascii(border->text()).toFloat();
			options_[FDPB::Option::TEMPERATURE]						= ascii(temperature->text()).toFloat();
			options_[FDPB::Option::MAX_ITERATIONS]				= ascii(max_iterations->text()).toFloat();

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
						charge_processor_.setFilename(ascii(charges_data_lineedit->text()));
						if (!system_->apply(charge_processor_)) return false;
					}
					else
					{
						INIFile inifile(ascii(charges_rules_lineedit->text()));
						charge_rule_processor_ = ChargeRuleProcessor(inifile);
						if (!system_->apply(charge_rule_processor_)) return false;
					}
				}
				
				if (assign_radii->isChecked())
				{
					if (radii_data_button->isChecked())
					{
						radius_processor_.setFilename(ascii(radii_data_lineedit->text()));
						if (!system_->apply(radius_processor_)) return false;
					}
					else
					{
						INIFile inifile(ascii(radii_rules_lineedit->text()));
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
