// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: threads.C,v 1.41.2.1 2006/01/13 15:36:02 amoll Exp $
//

#include <BALL/VIEW/KERNEL/threads.h>

// Do nothing, if no thread support
#ifdef BALL_QT_HAS_THREADS

#include <BALL/VIEW/MODELS/modelProcessor.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/DIALOGS/FDPBDialog.h>

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>

#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>

#include <BALL/SYSTEM/directory.h>

#include <qapplication.h>

namespace BALL
{
	namespace VIEW
	{

		BALLThread::BALLThread()
			: QThread(),
				main_control_(0),
				composite_(0)
		{
		}

		void BALLThread::output_(const String& string, bool important)
		{
			LogEvent* su = new LogEvent;
			su->setMessage(string);
			su->setImportant(important);
			qApp->postEvent(main_control_, su);  // Qt will delete it when done
		}

		void BALLThread::waitForUpdateOfRepresentations_()
		{
//   Log.error() << "#~~#   1 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;
			PrimitiveManager& pm = main_control_->getPrimitiveManager();
			while (pm.updateRunning())
			{
				msleep(50);
			}
//   Log.error() << "#~~#   2 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;
		}

		void BALLThread::updateScene_()
		{
			// notify MainControl to update all Representations for the Composite
			UpdateCompositeEvent* se = new UpdateCompositeEvent;
			se->setComposite(composite_);
			qApp->postEvent(main_control_, se);
		}

		// ================================== FetchHTMLThread ===============================

		FetchHTMLThread::FetchHTMLThread()
			throw()
			: BALLThread(),
				file_name_("")
		{
		}

		void FetchHTMLThread::setURL(const String& url)
			throw()
		{
			url_ = url;
		}

		void FetchHTMLThread::run()
		{
			if (url_ == "")
			{
				output_("Invalid Address " + url_ + " in " + String(__FILE__) + ":" + String(__LINE__), true);
				return;
			}
			try
			{
				MainControl* mc = getMainControl();
				if (mc != 0)
				{
					tcp_.setProxy(mc->getProxy(), mc->getProxyPort());
				}

				if (file_name_ != "")
				{
					File f(file_name_, std::ios::out);
					if (!f.isOpen())
					{
						output_(String("Could not open file ") + file_name_ + " for output!");
						return;
					}
					tcp_.set(f, url_);
					// dont move the transfer() call away from here!
					tcp_.transfer();
				}
				else
				{
					char c;
					stream_.get(c);

					while (stream_.gcount() > 0)
					{
						stream_.get(c);
					}

					stream_.clear();

					tcp_.set(stream_, url_);
					// dont move the transfer() call away from here!
					tcp_.transfer();
				}
			}
			catch(...)
			{
				output_(String("Exception in ") + String(__FILE__) + ": " + String(__LINE__), true);
			}
		}

		// ==========================================
		
		UpdateRepresentationThread::UpdateRepresentationThread()
			throw() 
			: BALLThread(),
				rep_(0)
		{
			setTerminationEnabled(true);
		}

		void UpdateRepresentationThread::run()
		{
			PrimitiveManager& pm = main_control_->getPrimitiveManager();
			Representation* rep = 0;
			while (true)
			{
				msleep(10);

				if (!main_control_->useMultithreading()) continue;

				rep = pm.popRepresentationToUpdate();
				if (rep == 0) continue;
				
				rep->update_();

				FinishedRepresentionUpdateEvent* se = new FinishedRepresentionUpdateEvent;
				se->setRepresentation(rep);
				qApp->postEvent(getMainControl(), se);
			}
		}

		// ==========================================
		
		SimulationThread::SimulationThread()
			: BALLThread(),
				steps_between_updates_(0),
				dcd_file_(0)
		{
			setTerminationEnabled(true);
		}
		
		void SimulationThread::exportSceneToPNG_()
		{
			if (main_control_->stopedSimulation()) return;

			Scene* scene = Scene::getInstance(0);
			if (scene == 0) return;

			Scene::SceneExportPNGEvent* e = new Scene::SceneExportPNGEvent();
			qApp->postEvent(scene, e);
		}

		void SimulationThread::finish_()
		{
			SimulationThreadFinished* su = new SimulationThreadFinished;
			qApp->postEvent(main_control_, su);  // Qt will delete it when done

			main_control_->getCompositesLockedWaitCondition().wakeAll();
		}

		// =====================================================================
		void EnergyMinimizerThread::run()
		{
			try
			{
				if (minimizer_ == 0 															||
						minimizer_->getForceField() == 0 							|| 
						minimizer_->getForceField()->getSystem() == 0 ||
						main_control_ == 0)
				{
					throw Exception::NullPointer(__FILE__, __LINE__);
				}

				ForceField& ff = *minimizer_->getForceField();
				bool ok = true;
				bool converged = false;
				// iterate until done and refresh the screen every "steps" iterations
				while (!main_control_->stopedSimulation() &&
								minimizer_->getNumberOfIterations() < minimizer_->getMaxNumberOfIterations() &&
								!converged && ok)
				{
					converged = minimizer_->minimize(steps_between_updates_, true);
					ok = !minimizer_->wasAborted();

					updateScene_();
					waitForUpdateOfRepresentations_();

					QString message;
					message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
													minimizer_->getNumberOfIterations(), 
													ff.getEnergy(), ff.getRMSGradient());
					output_(ascii(message));
				}

				updateScene_();

				output_(ff.getResults());
				output_("final RMS gradient    : " + String(ff.getRMSGradient()) + " kJ/(mol A)   after " 
								+ String(minimizer_->getNumberOfIterations()) + " iterations\n",
								true);
				finish_();

				if (!ok)
				{
					output_("Aborted minimization because convergence could not be reached. Try to restart the minimization.", true);
					return;
				}
			}
			catch(Exception::GeneralException e)
			{
				delete dcd_file_;
				dcd_file_ = 0;

				String txt = String("Exception was thrown during minimization: ") + __FILE__ + ": " + String(__LINE__) + " :\n" 
											+ e.getMessage();
				output_(txt, true);

				finish_();
			}
		}

		EnergyMinimizerThread::EnergyMinimizerThread()
			: SimulationThread(),
				minimizer_(0)
		{
		}

		EnergyMinimizerThread::~EnergyMinimizerThread()
		{
			if (minimizer_ != 0)
			{
				delete minimizer_;
			}
		}

		void EnergyMinimizerThread::setEnergyMinimizer(EnergyMinimizer* minimizer) 
		{ 
			if (minimizer_ != 0)
			{
				delete minimizer_;
			}
			minimizer_ = minimizer;
		}

		// =====================================================
		void MDSimulationThread::run()
			throw(Exception::NullPointer)
		{
			try
			{
				if (md_ == 0 ||
						md_->getForceField() == 0 || 
						md_->getForceField()->getSystem() == 0 || 
						main_control_ == 0)
				{
					throw Exception::NullPointer(__FILE__, __LINE__);
				}
				ForceField& ff = *md_->getForceField();

				SnapShotManager manager(ff.getSystem(), &ff, dcd_file_);
				manager.setFlushToDiskFrequency(10);
				bool ok = true;

				// iterate until done and refresh the screen every "steps" iterations
				while (ok &&
							 md_->getNumberOfIterations() < steps_ &&
							 !main_control_->stopedSimulation())
				{
					ok = md_->simulateIterations(steps_between_updates_, true);
					updateScene_();

					waitForUpdateOfRepresentations_();
					
					QString message;
					message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
													md_->getNumberOfIterations(), ff.getEnergy(),
													ff.getRMSGradient());
					output_(ascii(message));
					

					if (save_images_) exportSceneToPNG_();
					if (dcd_file_) 		manager.takeSnapShot();
				}

				if (dcd_file_) manager.flushToDisk();

 				output_(ff.getResults());
				output_("final RMS gradient    : " + String(ff.getRMSGradient()) + " kJ/(mol A)   after " 
								+ String(md_->getNumberOfIterations()) + " iterations\n", 
								true);

				if (!ok)
				{
					output_("Simulation aborted to to strange energy values.", true);
				}

				finish_();
			}
			catch(Exception::GeneralException e)
			{
				String txt = String("Exception was thrown during MD simulation: ")
											+ __FILE__ + ": " + String(__LINE__) + " \n" + e.getMessage();
				output_(txt, true);

				if (dcd_file_ != 0)
				{
					dcd_file_->close();
					delete dcd_file_;
					dcd_file_ = 0;
				}

				finish_();
			}
		}


		MDSimulationThread::MDSimulationThread()
			: SimulationThread(),
				md_(0),
				save_images_(false)
		{
		}

		MDSimulationThread::~MDSimulationThread()
		{
			if (md_ != 0) delete md_;
		}


		void MDSimulationThread::setMolecularDynamics(MolecularDynamics* md)
		{ 
			if (md_ != 0) delete md_;
			
			md_ = md;
		}

		// =================================================0
		
		CalculateFDPBThread::CalculateFDPBThread()
			throw()
			: BALLThread(),
				dialog_(0)
		{}

		void CalculateFDPBThread::run()
		{
			if (dialog_ == 0) return;
			dialog_->calculate_();
		}


		// =========================== implementation of class DockingThread ================
		
		///
		DockingThread::DockingThread()
			throw()
			: BALLThread(),
				dock_alg_(0)
		{} 
		
		// Copy constructor.
		DockingThread::DockingThread(const DockingThread& dock_thread)
			throw()
			: BALLThread(),
				dock_alg_(dock_thread.dock_alg_)
		{}
		
		///
		DockingThread::~DockingThread()
			throw()
		{
			output_("delete thread", true);

			if (dock_alg_ != 0)
			{
				delete dock_alg_;
				dock_alg_ = NULL;
			}
		}
		
		// Assignment operator
		const DockingThread& DockingThread::operator =(const DockingThread& dock_thread)
			throw()
		{
			if (&dock_thread != this)
			{
				dock_alg_ = dock_thread.dock_alg_;
			}
			return *this;
		}
		
		//
		void DockingThread::setDockingAlgorithm(DockingAlgorithm* dock_alg)
			throw()
		{
			dock_alg_ = dock_alg;
		}
		
		/// 
		void DockingThread::run()
			throw(Exception::NullPointer)
		{
				if (dock_alg_ == 0 ||
						main_control_ == 0)
				{
					throw Exception::NullPointer(__FILE__, __LINE__);
				}
				
				output_("starting docking...", true);

				dock_alg_->start();
				
		 		DockingFinishedEvent* finished = new DockingFinishedEvent(dock_alg_->wasAborted());
				// Qt will delete event when done
				ConformationSet* cs = new ConformationSet(dock_alg_->getConformationSet());
				// conformation set is deleted in DockResult
				finished->setConformationSet(cs);
				qApp->postEvent(getMainControl(), finished);
				
				output_("Docking finished.", true);
		}
		
		// =================================================0
		
	} // namespace VIEW
} // namespace BALL

#endif //Thread support
