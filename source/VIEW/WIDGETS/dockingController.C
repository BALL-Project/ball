// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockingController.C,v 1.4.2.7.2.10 2006/07/05 13:16:03 leonhardt Exp $
//

#include <BALL/VIEW/WIDGETS/dockingController.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>

#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>

#include <BALL/VIEW/DIALOGS/dockResultDialog.h>
#include <BALL/VIEW/DIALOGS/dockProgressDialog.h>

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/STRUCTURE/DOCKING/dockResult.h>
#include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>
#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/amberEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/FORMAT/INIFile.h>

#ifdef BALL_HAS_FFTW
#include <BALL/VIEW/DIALOGS/geometricFitDialog.h>
#include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#endif

#include "../../STRUCTURE/DOCKING/evolutionaryDocking.h"
#include <BALL/SYSTEM/path.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/VIEW/MODELS/surfaceModel.h>

#include <BALL/VIEW/KERNEL/threads.h>

#include <QtGui/qmessagebox.h>
#include <QtGui/qcombobox.h>
//#define BALL_VIEW_DEBUG
//#undef BALL_QT_HAS_THREADS
using namespace std;

namespace BALL
{
	namespace VIEW
	{

		DockingController::DockingController(QWidget* parent, const char* name)
			throw()
			:	QWidget(parent),
				ModularWidget(name),
				dock_dialog_(this),
				dock_alg_(0),
				progress_dialog_(0),
				was_called_(false)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "New DockingController " << this << std::endl;
			#endif
			registerWidget(this);
			setObjectName(name);
			
			timer_.setSingleShot(true);
			connect(&timer_, SIGNAL(timeout()), SLOT(updateSystem_()));

			hide();
		}

		// Copy constructor.
		DockingController::DockingController(const DockingController& dock_controller)
			throw()
			: QWidget(),
				ModularWidget(dock_controller),
				dock_dialog_(),
				dock_alg_(dock_controller.dock_alg_),
				progress_dialog_(dock_controller.progress_dialog_),
				was_called_(false)
		{}
		
		// Destructor
		DockingController::~DockingController()
			throw()
		{
			// remark: progress dialog is automatically deleted because its parent is the docking controller
			
			if (dock_alg_ != 0)
			{
				delete dock_alg_;
				dock_alg_ = NULL;
			}
		}
		
		// Assignment operator
		const DockingController& DockingController::operator =(const DockingController& dock_controller)
			throw()
		{
			if (&dock_controller != this)
			{
				dock_dialog_ = dock_controller.dock_dialog_;
				if (dock_alg_ != 0)
				{
					// remark: there might be troubles if doing an assignment when the docking thread is still running
					delete dock_alg_;
				}
				dock_alg_ = dock_controller.dock_alg_;
				if(progress_dialog_ != 0)
				{
					delete progress_dialog_;
				}
				progress_dialog_ = dock_controller.progress_dialog_;
				/////////////////////////////////
				was_called_ = dock_controller.was_called_;
			}
			return *this;
		}
		
		// Message handling method
		void DockingController::onNotify(Message *message)
			throw()
		{
			// if (re)docking has finished, start scoring
			// first check if user has aborted (re)docking
			if (RTTI::isKindOf<DockingFinishedMessage>(*message))
			{
				DockingFinishedMessage* dfm = RTTI::castTo<DockingFinishedMessage>(*message);
				
				unlockComposites();
				
				progress_dialog_->close();
				delete progress_dialog_;
				progress_dialog_ = NULL;
				
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
				
				setStatusbarText("Starting scoring...", true);
				if(!runScoring_((ConformationSet*)(dfm->getConformationSet())))
				{
				 delete dfm->getConformationSet();
				}
				setStatusbarText("Scoring finished.", true);
			}
			// DatasetControl sends this messages, when user wants to have a look at a DockResult
			else if (RTTI::isKindOf<ShowDockResultMessage>(*message))
			{
				ShowDockResultMessage* sdrm = RTTI::castTo<ShowDockResultMessage>(*message);
				
				DockResultDialog* result_dialog = new DockResultDialog(this);
				// dialog deletes itself after close-button is pressed
					
				// setup result_dialog... 
				result_dialog->setDockResult(sdrm->getDockResult());
				result_dialog->setDockedSystem(sdrm->getDockedSystem());
				//...and show it
				result_dialog->show();
			}
		}
		
		// Initializes the popup menu Molecular Mechanics with its checkable submenu Docking.
		void DockingController::initializeWidget(MainControl& main_control)
			throw()
		{			
			action_ = main_control.insertMenuEntry(MainControl::MOLECULARMECHANICS, "&Docking", this,
																				 SLOT(startDocking()), Qt::CTRL + Qt:: Key_D);
			setMenuHint("Dock two systems.");
			dock_dialog_.initializeWidget();
		}
		
		//Fetches the preferences from the INIFile
		void DockingController::fetchPreferences(INIFile& file)
			throw()
		{
			dock_dialog_.fetchPreferences(file);
		}

		//Writes the preferences to the INIFile.
		void DockingController::writePreferences(INIFile& file)
			throw()
		{
			dock_dialog_.writePreferences(file);
		}
		
		// Updates the state of menu entry Docking in the popup menu Molecular Mechanics.
		void DockingController::checkMenu (MainControl& main_control)
			throw()
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
			if (num_systems > 1)
			{
				action_->setEnabled(true);
			}
			else
			{
				action_->setEnabled(false);
			}
		}
		
		DockDialog& DockingController::getDockDialog()	
			throw()
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
			throw()
		{
			// Make sure we run just one instance at a time.
			if (getMainControl()->isBusy())
			{
				Log.error() << "Docking already running! " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
			
			dock_dialog_.isRedock(is_redock);
			
			// QDialog::Accepted = 1; QDialog::Rejected = 0 
			//if cancel was pressed in DockDialog, don't start docking
			if (!dock_dialog_.exec())
			{
				return;
			}
			// check which algorithm is chosen and create a DockingAlgorithm object
			Index index = dock_dialog_.algorithms->currentIndex();
			switch(index)
			{
				case GEOMETRIC_FIT:
					if(dock_alg_ != 0)
					{
						delete dock_alg_;
						dock_alg_ = NULL;
					}
#ifdef BALL_HAS_FFTW
					dock_alg_ =  new GeometricFit();
#endif
					break;
				case EVOLUTION_DOCKING:
					if(dock_alg_ != 0)
					{
						delete dock_alg_;
						dock_alg_ = NULL;
					}
					dock_alg_ =  new EvolutionaryDocking();
					break;
			}

			if (!dock_alg_
					|| !dock_dialog_.getSystem1()
					|| !dock_dialog_.getSystem2()
					|| dock_dialog_.getAlgorithmOptions().isEmpty())
			{
			 	return;
			}

			// Set up the docking algorithm
			setStatusbarText("Setting up docking algorithm...", true);
			///////////////////////// TEMP ///////////////////////////////
			Options::ConstIterator it = dock_dialog_.getAlgorithmOptions().begin();
      for (; +it; ++it)
			{
				Log.error() << it->first << " " << it->second << std::endl;
			}

			// keep the larger protein in System A and the smaller one in System B
			// and setup the algorithm
			if (dock_dialog_.getSystem1()->countAtoms() < dock_dialog_.getSystem2()->countAtoms())
			{
				dock_alg_->setup(*(dock_dialog_.getSystem2()), *(dock_dialog_.getSystem1()), dock_dialog_.getAlgorithmOptions());
			}
			else
			{
				dock_alg_->setup(*(dock_dialog_.getSystem1()), *(dock_dialog_.getSystem2()), dock_dialog_.getAlgorithmOptions());
			}

			// set force field options for the ff used in certain algorithms, e.g. evolutionary docking
			if(RTTI::isKindOf<EvolutionaryDocking>(*dock_alg_))
			{
				EvolutionaryDocking* ed = RTTI::castTo<EvolutionaryDocking>(*dock_alg_);
				//ed->setForceField(dock_dialog_.getForceField());
			}
							
			// ============================= WITH MULTITHREADING ====================================
			#ifdef BALL_QT_HAS_THREADS
				if (!(getMainControl()->lockCompositesFor(this))) return;
				DockingThread* thread = new DockingThread;
				/// ??????? where is thread deleted?
				thread->setDockingAlgorithm(dock_alg_);
				thread->setMainControl(getMainControl());
				
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
				thread->start();
				//progress_dialog_->show();

				switch(index)
				{
					case EVOLUTION_DOCKING:
						// start timer, true -> it is a single shot
      			timer_.start(1000);
						break;
				}
				
			// ============================= WITHOUT MULTITHREADING =================================
			#else
				// start docking
				setStatusbarText("Starting docking...", true);
				dock_alg_->start();
				setStatusbarText("Docking finished.", true);
				ConformationSet* cs = new ConformationSet(dock_alg_->getConformationSet());
				if(!runScoring_(cs))
				{
				 delete cs;
				 cs = NULL;
				}
				// delete instance 
				// conformationSet is deleted by dockResult
				if (dock_alg_ != NULL)
				{
					delete dock_alg_;
					dock_alg_ = NULL;
				}
			#endif
		}
		
		// Apply scoring function which user has chosen.
		// Then, create new DockResult and add new scoring to it.
		// At the end, add the docked system to BALLView structures
		// and send a NewDockResultMessage to insert the DockResult in DatasetControl.
		bool DockingController::runScoring_(ConformationSet* conformation_set)
			throw()
		{
			if (!conformation_set) return false;
			if (!conformation_set->size())
			{
				Log.error() << "There are no docking results! " << __FILE__ << " " << __LINE__ << std::endl;
				return false;
			}
		
		 	// create scoring function object
			EnergeticEvaluation* scoring = 0;
			//check which scoring function is chosen
			Index index = dock_dialog_.scoring_functions->currentIndex();
			
			switch(index)
			{
				case DEFAULT:
					scoring = new EnergeticEvaluation();
					break;
				case AMBER_FF:
				{
					MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
					if (!mol_struct)
					{
						Log.error() << "Error while scoring with AMBER_FF! " << __FILE__ << " " << __LINE__ << std::endl;
						return false;
					}
					AmberFF& ff = mol_struct->getAmberFF();
					//the force field is given to the AmberEvaluation (scoring function) object
					scoring = new AmberEvaluation(ff);
					break;
				}
				case RANDOM:
					scoring = new RandomEvaluation;
					break;
			}
		
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
				if (scoring != NULL)
				{
					delete scoring;
					scoring = NULL;
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
			
			dock_res->addScoring(ascii(dock_dialog_.scoring_functions->currentText()), dock_dialog_.getScoringOptions(), ranked_conformations);

			// add docked system to BALLView structures
			const SnapShot& best_result = (*conformation_set)[0];

			//best_result.applySnapShot(*docked_system_);
			getMainControl()->update(*docked_system_,true);
			//getMainControl()->insert(conformation_set->getSystem());
			
			// send a DockResultMessage
			NewDockResultMessage* dock_res_m = new NewDockResultMessage();
			// message is deleted by ConnectionObject
			dock_res_m->setDockResult(*dock_res);
			dock_res_m->setComposite(*docked_system_);
			notify_(dock_res_m);

			// delete instance 
			if (scoring != NULL)
			{
				delete scoring;
				scoring = NULL;
			}
			return true;
		}
	
		void DockingController::updateSystem_()
		{
			//Log.error() << "in DockingController::updateSystem_()" << std::endl;
			//if(dock_alg_->systemChanged())
			//{
				// if function is called for the first time
				if(!was_called_)
				{
					//Log.error() << "was_called_ true" << std::endl;
					// system is deleted by main control, when it is removed from BallView
					docked_system_ = new System(dock_alg_->getIntermediateResult());
					//getMainControl()->deselectCompositeRecursive(docked_system, true);
					getMainControl()->insert(*docked_system_);
				 //notify_(new CompositeMessage(*docked_system_, CompositeMessage::CENTER_CAMERA));

					was_called_ = true;
				}
				else
				{
					//Log.error() << "was_called_ false" << std::endl;
					SnapShot sn;
					sn.takeSnapShot(dock_alg_->getIntermediateResult());
					sn.applySnapShot(*docked_system_);
					getMainControl()->update(*docked_system_, true);
				}
			//}
			
			// if docking has not finished restart timer
			if (!dock_alg_->hasFinished())
			{
			 	timer_.start(1000);
			}

		}

	} // end of namespace View
} // end of namespace BALL
