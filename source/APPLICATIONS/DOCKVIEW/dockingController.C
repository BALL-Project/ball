// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockingController.C,v 1.1.2.1 2005/05/23 16:14:32 haid Exp $
//

#include "dockingController.h"
#include "geometricFitDialog.h"
#include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/amberEvaluation.h>
#include <BALL/STRUCTURE/DOCKING/randomEvaluation.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>


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
				dock_alg_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "New DockingController " << this << std::endl;
			#endif
			registerWidget(this);
		}
		
		DockingController::~DockingController()
			throw()
		{}
		
		// Message handling method
		void DockingController::onNotify(Message *message)
			throw()
		{
			if (RTTI::isKindOf<DockingFinishedMessage>(*message))
			{
				DockingFinishedMessage* dfm = RTTI::castTo<DockingFinishedMessage>(*message);
			
				unlockComposites();
				if(dfm->wasAborted())
				{
					QMessageBox request_message(0,0);
					if( request_message.question(0,"Request","Do you want to see the current Result?", 
																			 "Yes", "No", QString::null, 0, 1))
					{
						return;
					}
				}
				runScoring_((ConformationSet*)(dfm->getConformationSet()));
			}
		}
		
		// Initializes the popup menu Molecular Mechanics with its checkable submenu Docking.
		void DockingController::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.initPopupMenu(MainControl::MOLECULARMECHANICS)->setCheckable(true);
			
			String hint = "Dock two systems.";
			id_ = main_control.insertMenuEntry(MainControl::MOLECULARMECHANICS, "&Docking", this,
																				 SLOT(runDocking()), CTRL+Key_D, -1, hint);
			dock_dialog_.initializeWidget(main_control);
		}
		
		//Removes the checkable submenu Docking from the popup menu Molecular Mechanics.
		void DockingController::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "&Docking", this,
																	 SLOT(runDocking()), CTRL+Key_D);
		}   
		
		// Read the preferences from the INIFile.
		void DockingController::fetchPreferences(INIFile& file)
			throw()
		{
			dock_dialog_.fetchPreferences(file);
		}
		
		// Write the preferences to the INIFile.
		void DockingController::writePreferences(INIFile& file)
			throw()
		{
			dock_dialog_.writePreferences(file);
		}
		
		// Updates the state of menu entry Docking in the popup menu Molecular Mechanics.
		void DockingController::checkMenu (MainControl& main_control)
			throw()
		{
			if(main_control.compositesAreLocked())
			{
				menuBar()->setItemEnabled(id_, false);
				return;
			}
		
			CompositeManager& composite_manager = main_control.getCompositeManager();
			
			//iterate over all composites; get to know if there are systems
			HashSet<Composite*>::Iterator composite_it = composite_manager.begin();

			Size num_systems = 0;
			for (; +composite_it; ++composite_it)
			{
				if (RTTI::isKindOf<System>(**composite_it))
				{
					num_systems++;
				}
			}

			//if no or only one system loaded, disable menu entry "Docking"
			if(num_systems > 1)
			{
				menuBar()->setItemEnabled(id_, true);
			}
			else
			{
				menuBar()->setItemEnabled(id_, false);
			}
		}
		
		void DockingController::runDocking()
		{
			// Make sure we run just one instance at a time.
			if (getMainControl()->compositesAreLocked())
			{
				Log.error() << "Docking already running!" << std::endl;
				return;
			}
			
			// if cancel was pressed in DockDialog, don't start docking
			if(!dock_dialog_.exec())
			{
				return;
			}
			
			// check which algorithm is chosen and create a DockingAlgorithm object
			int index = dock_dialog_.algorithms->currentItem();
			switch(index)
			{
				case DockDialog::GEOMETRIC_FIT:
					dock_alg_ =  new GeometricFit();
					break;
			}
			
			if(dock_alg_ == NULL)
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
			Log.info() << "vor thread" << std::endl;
			#ifdef BALL_QT_HAS_THREADS
				DockingThread* thread = new DockingThread;
				thread->setDockingAlgorithm(dock_alg_);
				thread->setMainControl(getMainControl());
				
				progress_dialog_ = new DockProgressDialog(this);
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
		
		void DockingController::runScoring_(ConformationSet* conformation_set)
			throw()
		{
			Log.error() << "in DockingController::runScoring_" << std::endl;
		
		 	// create scoring function object
			EnergeticEvaluation* scoring = 0;
			//check which scoring function is chosen
			int index = dock_dialog_.scoring_functions->currentItem();
			
			switch(index)
			{
				case DockDialog::DEFAULT:
					Log.error() << "in runScoring_::DEFAULT" << std::endl;
					scoring = new EnergeticEvaluation();
					break;

				case DockDialog::AMBER_FF:
				{
					Log.info() << "in DockingController:: Option of Amber FF:" << std::endl;
					AmberFF& ff = MolecularStructure::getInstance(0)->getAmberFF();
					//the force field is given to the AmberEvaluation (scoring function) object
					scoring = new AmberEvaluation(ff);
					break;
				}
				case DockDialog::RANDOM:
					Log.error() << "in runScoring_::RANDOM" << std::endl;
					scoring = new RandomEvaluation();
					break;
			}
		
			// apply scoring function; set new scores in the conformation set
	   	std::vector<ConformationSet::Conformation> ranked_conformations((*scoring)(*conformation_set));
			conformation_set->setScoring(ranked_conformations);

			// add a new scoring to dock_res_; we need the name, options and score vector of the scoring function
			vector<float> scores;
			for(unsigned int i = 0; i < ranked_conformations.size(); i++)
			{
				scores.push_back(ranked_conformations[i].second);
			}

			DockResult* dock_res = new DockResult(String(dock_dialog_.algorithms->currentText().ascii()),
																						conformation_set,
																						dock_dialog_.getAlgorithmOptions()); 
																							
			dock_res->addScoring(String(dock_dialog_.scoring_functions->currentText().ascii()), dock_dialog_.getScoringOptions(), scores);

			// add docked system to BALLView structures 
			SnapShot best_result = (*conformation_set)[0];
			
			System* docked_system = new System(conformation_set->getSystem());
			best_result.applySnapShot(*docked_system);
			getMainControl()->deselectCompositeRecursive(docked_system, true);
			getMainControl()->insert(*docked_system);
			
			// send a DockResultMessage
			NewDockResultMessage* dock_res_m = new NewDockResultMessage();
			dock_res_m->setDockResult(*dock_res);
			dock_res_m->setComposite(*docked_system);
			notify_(dock_res_m);

			// delete instance 
			if (scoring != NULL)
			{
				delete scoring;
				scoring = NULL;
			}
			
			Log.info() << "End of calculate" << std::endl;
		}
		
	}
}
