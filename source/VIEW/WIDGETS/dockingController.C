// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/dockingController.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>

#include <BALL/VIEW/DIALOGS/dockResultDialog.h>
#include <BALL/VIEW/DIALOGS/dockProgressDialog.h>

#include <BALL/VIEW/DATATYPE/standardDatasets.h>

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/DOCKING/COMMON/dockResult.h>
#include <BALL/DOCKING/COMMON/conformationSet.h>
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include <BALL/SCORING/FUNCTIONS/energeticEvaluation.h>
#include <BALL/SCORING/FUNCTIONS/amberEvaluation.h>
#include <BALL/SCORING/FUNCTIONS/randomEvaluation.h>

#ifdef BALL_HAS_FFTW
#include <BALL/VIEW/DIALOGS/geometricFitDialog.h>
#include <BALL/DOCKING/geometricFit.h>
#endif

#include <BALL/VIEW/KERNEL/threads.h>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QComboBox>

//#define BALL_VIEW_DEBUG
//#undef BALL_QT_HAS_THREADS
using namespace std;

namespace BALL
{
	namespace VIEW
	{

		DockingController::DockingController(QWidget* parent, const char* name)
			:	QWidget(parent),
				ModularWidget(name),
				dock_dialog_(this),
				dock_result_dialog_(0),
				progress_dialog_(0),
				dock_alg_(0),
				thread_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "New DockingController " << this << std::endl;
			#endif
			registerWidget(this);
			setObjectName(name);
			hide();
		}

		// Copy constructor.
		DockingController::DockingController(const DockingController& dock_controller)
			: QWidget(),
				ModularWidget(dock_controller),
				dock_dialog_(),
				dock_result_dialog_(0),
				progress_dialog_(0),
				dock_alg_(0),
				thread_(0)
		{}
		
		// Destructor
		DockingController::~DockingController()
		{
			// remark: progress dialog is automatically deleted because its parent is the docking controller
			
			if (thread_ != 0)
			{
				thread_->terminate();
				thread_->wait();
				delete thread_;
			}

			if (dock_alg_ != 0)
			{
				delete dock_alg_;
				dock_alg_ = 0;
			}
		}
		
		// Assignment operator
		const DockingController& DockingController::operator = (const DockingController&)
		{
			return *this;
		}
		
		// Message handling method
		void DockingController::onNotify(Message *message)
		{
			// if (re)docking has finished, start scoring
			// first check if user has aborted (re)docking
            if (RTTI::isKindOf<DockingFinishedMessage>(message))
			{
				DockingFinishedMessage* dfm = RTTI::castTo<DockingFinishedMessage>(*message);
				
				unlockComposites();
				
				progress_dialog_->close();
				delete progress_dialog_;
				progress_dialog_ = 0;
				
				if (dfm->wasAborted())
				{
					if (QMessageBox::question(0, "Request","Do you want to see the current Result?", 
																		 QMessageBox::Yes,
																		 QMessageBox::No,
																		 QMessageBox::NoButton))
					{
						return;
					}
				}
				
				setStatusbarText(tr("Starting scoring..."), true);
				if(!runScoring_((ConformationSet*)(dfm->getConformationSet())))
				{
				 delete dfm->getConformationSet();
				}
				setStatusbarText(tr("Scoring finished."), true);
				return;
			}

			// DatasetControl sends this messages, when user wants to have a look at a DockResult
            if (RTTI::isKindOf<DatasetMessage>(message))
			{
				DatasetMessage* sdrm = RTTI::castTo<DatasetMessage>(*message);
				if (!sdrm->isValid()) return;
				DockResultDataset* set = dynamic_cast<DockResultDataset*>(sdrm->getDataset());
				if (set == 0) return;
				if (set->getType() != DockResultController::type) return;

				if (dock_result_dialog_ == 0) dock_result_dialog_ = new DockResultDialog(this);
					
				dock_result_dialog_->setDockResult(set->getData());
				dock_result_dialog_->setDockedSystem(dynamic_cast<System*>(set->getComposite()));
				dock_result_dialog_->show();
			}
		}
		
		// Initializes the popup menu Molecular Mechanics with its checkable submenu Docking.
		void DockingController::initializeWidget(MainControl& main_control)
		{			
			action_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, tr("&Docking"), this,
													      SLOT(startDocking()), "Shortcut|MolecularMechanics|Dock", QKeySequence("Ctrl + D"), 
																tr("Dock two systems."), UIOperationMode::MODE_ADVANCED);
			dock_dialog_.initializeWidget();
		}
		
		// Fetches the preferences from the INIFile
		void DockingController::fetchPreferences(INIFile& file)
		{
			dock_dialog_.fetchPreferences(file);
		}

		// Writes the preferences to the INIFile.
		void DockingController::writePreferences(INIFile& file)
		{
			dock_dialog_.writePreferences(file);
		}
		
		// Updates the state of menu entry Docking in the popup menu Molecular Mechanics.
		void DockingController::checkMenu(MainControl& main_control)
		{
			// if composites are locked disable menu entry "Docking"
			if (main_control.isBusy())
			{
				action_->setEnabled(false);
				return;
			}

			// get to know how many composites are loaded
			CompositeManager& composite_manager = main_control.getCompositeManager();
			Size num_systems = composite_manager.getNumberOfComposites();

			// if no or only one system loaded, disable menu entry "Docking"
			action_->setEnabled(num_systems > 1);
		}
		
		DockDialog& DockingController::getDockDialog()	
		{
		 return dock_dialog_;
		}			
		
		// is called when user clicks on menu entry "Docking"
		void DockingController::startDocking()
		{
			// Dialog is called in docking-modus, not in redocking-modus
			runDocking(false);
		}
		
		// Show docking dialog, check which algorithm is chosen and create new DockingAlgorithm object.
		// Start new Thread and fill/show ProgressDialog.
		void DockingController::runDocking(bool is_redock)
		{
			// Make sure we run just one instance at a time.
			if (getMainControl()->isBusy())
			{
				Log.error() << "Docking already running! " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
			
			dock_dialog_.isRedock(is_redock);
			
			//if cancel was pressed in DockDialog, don't start docking
			if (!dock_dialog_.exec()) return;

			if (dock_alg_ != 0)
			{
				delete dock_alg_;
				dock_alg_ = 0;
			}
		
			// check which algorithm is chosen and create a DockingAlgorithm object
			Index index = dock_dialog_.algorithms->currentIndex();
			switch(index)
			{
				case GEOMETRIC_FIT:
#ifdef BALL_HAS_FFTW
					dock_alg_ =  new GeometricFit();
#endif
					break;
			}
			
			if (!dock_alg_ || 
					!dock_dialog_.getSystem1() || 
					!dock_dialog_.getSystem2() || 
					dock_dialog_.getAlgorithmOptions().isEmpty())
			{
			 	return;
			}
			
			// Set up the docking algorithm
			setStatusbarText(tr("Setting up docking algorithm..."), true);
			// keep the larger protein in System A and the smaller one in System B
			// and setup the algorithm
			System& s1 = *dock_dialog_.getSystem1();
			System& s2 = *dock_dialog_.getSystem2();
			if (s1.countAtoms() < s2.countAtoms())
			{
				dock_alg_->setup(s2, s1, dock_dialog_.getAlgorithmOptions());
			}
			else
			{
				dock_alg_->setup(s1, s2, dock_dialog_.getAlgorithmOptions());
			}
			
			// ============================= WITH MULTITHREADING ====================================
			#ifdef BALL_QT_HAS_THREADS
				if (!(getMainControl()->lockCompositesFor(this))) return;

				if (thread_ != 0)
				{
					thread_->terminate();
					thread_->wait();
					delete thread_;
				}

				thread_ = new DockingThread;
				thread_->setDockingAlgorithm(dock_alg_);
				thread_->setMainControl(getMainControl());
				
				progress_dialog_ = new DockProgressDialog(this);
				// dialog is deleted by itself when it's closed
				progress_dialog_->fillDialog(dock_dialog_.systems1->currentText(),
																		dock_dialog_.systems2->currentText(),
																		dock_dialog_.algorithms->currentText(),
																		dock_dialog_.scoring_functions->currentText(),
																		dock_dialog_.getAlgorithmOptions(),
																		dock_dialog_.getScoringOptions());
				progress_dialog_->setDockingAlgorithm(dock_alg_);
			
				// start thread
				// function calls DockingThread::run()
				thread_->start();
				progress_dialog_->show();
			// ============================= WITHOUT MULTITHREADING =================================
			#else
				// start docking
				setStatusbarText("Starting docking...", true);
				dock_alg_->start();
				setStatusbarText("Docking finished.", true);
				ConformationSet* cs = new ConformationSet(dock_alg_->getConformationSet());
				if (!runScoring_(cs))
				{
				 delete cs;
				 cs = 0;
				}
				// delete instance 
				// conformationSet is deleted by dockResult
				if (dock_alg_ != 0)
				{
					delete dock_alg_;
					dock_alg_ = 0;
				}
			#endif
		}

		EnergeticEvaluation* DockingController::createEvaluationMethod(Index method)
		{
			MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
			if (!mol_struct)
			{
				Log.error() << "Error while scoring with AMBER_FF! " << __FILE__ << " " << __LINE__ << std::endl;
				return 0;
			}

			switch(method)
			{
				case DEFAULT:
					return new EnergeticEvaluation();

				case AMBER_FF:
					return new AmberEvaluation(mol_struct->getAmberFF());

				case MMFF94_FF:
					return new ForceFieldEvaluation(mol_struct->getMMFF94());

				case SELECTED_FF:
					return new ForceFieldEvaluation(mol_struct->getForceField());

				case RANDOM:
					return new RandomEvaluation;
			}

			return 0;
		}

		
		// Apply scoring function which user has chosen.
		// Then, create new DockResult and add new scoring to it.
		// At the end, add the docked system to BALLView structures
		// and send a NewDockResultMessage to insert the DockResult in DatasetControl.
		bool DockingController::runScoring_(ConformationSet* conformation_set)
		{
			if (!conformation_set) return false;

			if (!conformation_set->size())
			{
				Log.error() << "There are no docking results! " << __FILE__ << " " << __LINE__ << std::endl;
				return false;
			}
		
			//check which scoring function is chosen
			Index index = dock_dialog_.scoring_functions->currentIndex();
			EnergeticEvaluation* scoring = createEvaluationMethod(index);
	
			if (!scoring) return false;
			
			// apply scoring function; set new scores in the conformation set
			vector<ConformationSet::Conformation> ranked_conformations;
			try
	   	{
				ranked_conformations = (*scoring)(*conformation_set);
			}
			catch(...)
			{
				Log.error() << "Scoring of docking results failed! " << __FILE__ << " " << __LINE__ << std::endl;
				// delete instance 
				if (scoring != 0)
				{
					delete scoring;
					scoring = 0;
				}
				return false;
			}
			conformation_set->setScoring(ranked_conformations);

			// create new DockResult and add a new scoring to it;
			// we need the name, options and score vector of the scoring function
			DockResult* dock_res = new DockResult(ascii(dock_dialog_.algorithms->currentText()),
																						conformation_set,
																						dock_dialog_.getAlgorithmOptions()); 
			// dock result is deleted by DatasetControl
		
			// sort vector ranked_conformations by snapshot numbers
			sort(ranked_conformations.begin(), ranked_conformations.end());
			
			dock_res->addScoring(ascii(dock_dialog_.scoring_functions->currentText()), 
													 dock_dialog_.getScoringOptions(), ranked_conformations);

			// add docked system to BALLView structures
			const SnapShot& best_result = (*conformation_set)[0];

			System* docked_system = new System(conformation_set->getSystem());
			// system is deleted by main control, when it is removed from BallView
			best_result.applySnapShot(*docked_system);
			getMainControl()->deselectCompositeRecursive(docked_system, true);
			getMainControl()->insert(*docked_system);
			
			// send a Message
			DockResultDataset* set = new DockResultDataset;
			set->setData(dock_res);
			set->setComposite(docked_system);
			set->setType(DockResultController::type);
			set->setName(dock_res->getDockingAlgorithm() + " result");
			notify_(new DatasetMessage(set, DatasetMessage::ADD));

			// delete instance 
			if (scoring != 0)
			{
				delete scoring;
				scoring = 0;
			}
			return true;
		}
		
	} // end of namespace View
} // end of namespace BALL
