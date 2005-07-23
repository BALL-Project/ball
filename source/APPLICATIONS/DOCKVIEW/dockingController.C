// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockingController.C,v 1.1.2.20 2005/07/23 12:50:56 haid Exp $
//

#include "dockingController.h"
#include "geometricFitDialog.h"
#include "dockResultDialog.h"

#include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/amberEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#include <BALL/KERNEL/system.h>
# include <BALL/DATATYPE/options.h>

#ifdef BALL_QT_HAS_THREADS
#	include <BALL/VIEW/KERNEL/threads.h>
#endif

#include <qmessagebox.h>
#include <qcombobox.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		DockingController::DockingController(QWidget* parent, const char* name)
			throw()
			:	QWidget(parent, name),
				ModularWidget(name),
				dock_dialog_(this),
				dock_alg_(0),
				progress_dialog_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "New DockingController " << this << std::endl;
			#endif
			registerWidget(this);
		}

		// Copy constructor.
		DockingController::DockingController(const DockingController& dock_controller)
			throw()
			: //QWidget(dock_controller),
				ModularWidget(dock_controller),
				dock_dialog_(dock_controller.dock_dialog_),
				dock_alg_(dock_controller.dock_alg_),
				progress_dialog_(dock_controller.progress_dialog_),
				id_(dock_controller.id_)
		{}
		
		// Destructor
		DockingController::~DockingController()
			throw()
		{
			// remark: progress dialog is automatically deleted because its parent is the docking controller
			
			if(dock_alg_ != 0)
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
				// ???? do we have to delete dock_alg_ and progress_dialog_
				dock_alg_ = dock_controller.dock_alg_;
				progress_dialog_ = dock_controller.progress_dialog_;
				id_ = dock_controller.id_;
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
				
				/*if(dock_alg_ != 0)
				{
					Log.info() << "dock_alg_ deleted by docking controller" << std::endl; 
					delete dock_alg_;
					dock_alg_ = NULL;
				}*/
				unlockComposites();
				if (dfm->wasAborted())
				{
					QMessageBox request_message(0,0);
					if ( request_message.question(0,"Request","Do you want to see the current Result?", 
																			 "Yes", "No", QString::null, 0, 1))
					{
						return;
					}
				}
				runScoring_((ConformationSet*)(dfm->getConformationSet()));
			}
			else if (RTTI::isKindOf<ShowDockResultMessage>(*message))
			{
			 	DockResultDialog* result_dialog = new DockResultDialog(this);
				// dialog deletes itself after close-button is pressed
	      ShowDockResultMessage* sdrm = RTTI::castTo<ShowDockResultMessage>(*message);
				
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
			String hint = "Dock two systems.";
			id_ = main_control.insertMenuEntry(MainControl::MOLECULARMECHANICS, "&Docking", this,
																				 SLOT(startDocking()), CTRL+Key_D, -1, hint);
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
			if (main_control.compositesAreLocked())
			{
				menuBar()->setItemEnabled(id_, false);
				return;
			}
			// iterate over all composites; get to know if there are loaded systems
			CompositeManager& composite_manager = main_control.getCompositeManager();
			HashSet<Composite*>::Iterator composite_it = composite_manager.begin();

			Size num_systems = 0;
			for (; +composite_it; ++composite_it)
			{
				if (RTTI::isKindOf<System>(**composite_it))
				{
					num_systems++;
				}
			}
			// if no or only one system loaded, disable menu entry "Docking"
			if (num_systems > 1)
			{
				menuBar()->setItemEnabled(id_, true);
			}
			else
			{
				menuBar()->setItemEnabled(id_, false);
			}
		}
		
		DockDialog& DockingController::getDockDialog()	
			throw()
		{
		 return dock_dialog_;
		}			
		
		void DockingController::startDocking()
		{
		 runDocking(false);
		}
		
		// Check which algorithm is chosen and create new DockingAlgorithm object.
		// Start new Thread and fill/show ProgressDialog.
		void DockingController::runDocking(bool isRedock)
			throw()
		{
			// Make sure we run just one instance at a time.
			if (getMainControl()->compositesAreLocked())
			{
				Log.error() << "Docking already running! " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
			
			dock_dialog_.isRedock(isRedock);
			
			// if cancel was pressed in DockDialog, don't start docking
			if (!dock_dialog_.exec())
			{
				return;
			}
			
			// check which algorithm is chosen and create a DockingAlgorithm object
			Index index = dock_dialog_.algorithms->currentItem();
			switch(index)
				{
				case GEOMETRIC_FIT:
					if(dock_alg_ != 0)
						{
							delete dock_alg_;
							dock_alg_ = NULL;
						}
					dock_alg_ =  new GeometricFit();
					/////// ??? where should we delete it? in this class -> segmenation fault, in thread class???
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
			setStatusbarText("setting up docking algorithm...", true);
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
			
			// ============================= WITH MULTITHREADING ====================================
			if (!(getMainControl()->lockCompositesFor(this))) return;
			#ifdef BALL_QT_HAS_THREADS
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
				
				thread->start();
				progress_dialog_->show();
			// ============================= WITHOUT MULTITHREADING =================================
			#else
				// start docking
				setStatusbarText("starting docking...", true);
				dock_alg_->start();
				setStatusbarText("Docking finished.", true);
				runScoring_(dock_alg_->getConformationSet());
				// delete instance 
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
		void DockingController::runScoring_(ConformationSet* conformation_set)
			throw()
		{
		 	// create scoring function object
			EnergeticEvaluation* scoring = 0;
			//check which scoring function is chosen
			Index index = dock_dialog_.scoring_functions->currentItem();
			
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
							return;
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
		
			if (!scoring) return;
			
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
				return;
			}
			conformation_set->setScoring(ranked_conformations);

			// create new DockResult and add a new scoring to it;
			// we need the name, options and score vector of the scoring function
			DockResult* dock_res = new DockResult(String(dock_dialog_.algorithms->currentText().ascii()),
																						conformation_set,
																						dock_dialog_.getAlgorithmOptions()); 
			// dock result is deleted by DatasetControl
		
			// sort vector ranked_conformations by snapshot numbers
			sort(ranked_conformations.begin(), ranked_conformations.end());
			
			vector<float> scores;
			for (unsigned int i = 0; i < ranked_conformations.size(); i++)
			{
				scores.push_back(ranked_conformations[i].second);
			}

			dock_res->addScoring(String(dock_dialog_.scoring_functions->currentText().ascii()), dock_dialog_.getScoringOptions(), scores);

			// add docked system to BALLView structures
			if (!conformation_set->size())
			{
				Log.error() << "Scoring of docking results failed! " << __FILE__ << " " << __LINE__ << std::endl;
				// delete instance 
				if (scoring != NULL)
				{
					delete scoring;
					scoring = NULL;
				}
				return;
			}
			const SnapShot& best_result = (*conformation_set)[0];

			System* docked_system = new System(conformation_set->getSystem());
			// system is deleted by main control, when it is removed from BallView
			best_result.applySnapShot(*docked_system);
			getMainControl()->deselectCompositeRecursive(docked_system, true);
			getMainControl()->insert(*docked_system);
			
			// send a DockResultMessage
			NewDockResultMessage* dock_res_m = new NewDockResultMessage();
			// message is deleted by ConnectionObject
			dock_res_m->setDockResult(*dock_res);
			dock_res_m->setComposite(*docked_system);
			notify_(dock_res_m);

			// delete instance 
			if (scoring != NULL)
			{
				delete scoring;
				scoring = NULL;
			}
		}
		
	} // end of namespace View
} // end of namespace BALL
