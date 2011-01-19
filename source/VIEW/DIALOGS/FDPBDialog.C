// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/VIEW/DATATYPE/standardDatasets.h>

#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		FDPBDialog::FDPBDialog(QWidget* parent,  const char* name, bool modal, Qt::WFlags fl)
			: QDialog(parent, fl),
				Ui_FDPBDialogData(),
				ModularWidget("FDPBDialog"),
				PreferencesEntry(),
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
			connect( charges_rules_browse, SIGNAL( pressed() ), this, SLOT( browseChargesRules() ) );
			connect( charges_data_browse, SIGNAL( clicked() ), this, SLOT( browseChargesData() ) );
			registerWidgets_();
		}

		FDPBDialog::~FDPBDialog()
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

		void FDPBDialog::accept()
		{
			QDialog::accept();
			calculate();
		}

		void FDPBDialog::clicked(QAbstractButton* button)
		{
			if(buttonBox->buttonRole(button) == QDialogButtonBox::ResetRole)
			{
				resetPressed();
			}
		}

		void FDPBDialog::resetPressed()
		{
			restoreDefaultValues();
		}

		// ------------------------- helper functions -------------------------------------
		// --------------------------------------------------------------------------------

		void FDPBDialog::selectFile_(QLineEdit& lineedit)
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
		{
			if (system_ == 0) system_ = getMainControl()->getSelectedSystem();
			if (system_ == 0)
			{
				Log.error() << "No system given! Aborting..." << std::endl;
				return false;
			}

			if (!lockComposites()) 
			{
				setStatusbarText(tr("Can not calculate FDPB, since I can not lock the molecular data. Is a simulation running?"), true);
				return false;
			}

			bool use_mt = false;
			// currently doesnt work:
			#ifdef BALL_QT_HAS_THREADS
			if (getMainControl()->useMultithreading())
			{
				use_mt = true;

				if (thread_ == 0)
				{
					thread_ = new CalculateFDPBThread();
				}
				else
				{
					if (thread_->isRunning())
					{
						Log.error() << "Thread already running in"  << " "  << __FILE__ << "  " << __LINE__<< std::endl;
						return false;
					}
				}

				thread_->setFDPBDialog(this);
				thread_->start();

				Position pos = 3;
				QString dots;
				Position i = 0;
				while (thread_->isRunning())
				{
					setStatusbarText(tr("Calculating FDPB grid ") + dots, false);
					qApp->processEvents();
					if (i > 10)
					{
						if (pos < 40) 
						{
							pos ++;
							dots +=".";
						}
						else 
						{
							pos = 3;
							dots = "...";
						}
						i = 0;
					}

					i++;
					thread_->wait(10); 
				}
			}

			#endif
			if (!use_mt) calculate_();
			
			setStatusbarText(tr("Finished FDPB grid"), true);
			RegularData3DDataset* set = new RegularData3DDataset();
			set->setData(fdpb_.phi_grid);
			set->setComposite(system_);
			set->setName("FDPB_" + system_->getName());
			set->setType(RegularData3DController::type);
			DatasetMessage* message = new DatasetMessage(set, DatasetMessage::ADD);
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
		{
			/// ------------------------------
			options_[FDPB::Option::SOLVENT_DC] 						= dc_solvent->text().toFloat();
			options_[FDPB::Option::SOLUTE_DC] 						= dc_interior->text().toFloat();
			options_[FDPB::Option::PROBE_RADIUS] 					= probe_radius->text().toFloat();
			options_[FDPB::Option::IONIC_STRENGTH] 				= ionic_strenght->text().toFloat();
			options_[FDPB::Option::ION_RADIUS] 						= ion_radius->text().toFloat();
			options_[FDPB::Option::SPACING] 							= spacing->text().toFloat();
			options_[FDPB::Option::BORDER]								= border->text().toFloat();
			options_[FDPB::Option::TEMPERATURE]						= temperature->text().toFloat();
			options_[FDPB::Option::MAX_ITERATIONS]				= max_iterations->text().toFloat();

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
			catch (Exception::FileNotFound& e)
			{
				Log.error() << "Invalid file " << e.getFilename() << std::endl;
				return false;
			}

			// notify MainControl to update all Representations for the Composite
			CompositeMessage* msg = new CompositeMessage(*system_,
																									 CompositeMessage::CHANGED_COMPOSITE,
																									 true);;
			qApp->postEvent(getMainControl(), new MessageEvent(msg));

			return true;
		}

	} // namespace VIEW
} // namespace BALL
