// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/threads.h>

// Do nothing, if no thread support
#ifdef BALL_QT_HAS_THREADS

#include <BALL/VIEW/MODELS/modelProcessor.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/DIALOGS/FDPBDialog.h>

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>

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
			if (main_control_ == 0) 
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}

			if (main_control_->stopedSimulation()) return;

			LogEvent* su = new LogEvent;
			su->setMessage(string);
			su->setImportant(important);
			qApp->postEvent(main_control_, su);  // Qt will delete it when done
		}

		void BALLThread::waitForUpdateOfRepresentations_()
		{
			if (main_control_ == 0) 
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}

			while (main_control_->getPrimitiveManager().updatePending())
			{
				main_control_->getPrimitiveManager().getUpdateWaitCondition().wait(100);
			}
		}

		void BALLThread::updateScene_()
		{
			if (main_control_ == 0) 
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}

			main_control_->getPrimitiveManager().setUpdatePending(true);
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
				output_("Invalid Address " + url_ + " in " + String(__FILE__) + __LINE__, true);
				return;
			}
			try
			{
				if (file_name_ != "")
				{
					File f(file_name_, std::ios::out);
					if (!f.isOpen())
					{
						output_(String("Could not open file ") + file_name_ + " for output!");
						return;
					}
					tcp_.set(f, url_);
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
					tcp_.transfer();
				}
			}
			catch(...)
			{
				output_(String("Exception in ") + String(__FILE__) + __LINE__, true);
			}
		}

		// ==========================================
		
		UpdateRepresentationThread::UpdateRepresentationThread()
			throw() 
			: BALLThread(),
				rep_(0)
		{
		}

		void UpdateRepresentationThread::run()
		{
			if (rep_ == 0) return;
 			rep_->update_();
			rep_ = 0;
			FinishedRepresentionUpdateEvent* se = new FinishedRepresentionUpdateEvent;
			qApp->postEvent(getMainControl(), se);
		}

		// ==========================================
		
		SimulationThread::SimulationThread()
			: BALLThread(),
				steps_between_updates_(0),
				dcd_file_(0)
		{
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
				// iterate until done and refresh the screen every "steps" iterations
				while (!main_control_->stopedSimulation() &&
								minimizer_->getNumberOfIterations() < minimizer_->getMaxNumberOfIterations() &&
								ok)
				{
					ok = minimizer_->minimize(steps_between_updates_, true);
			
					updateScene_();

					waitForUpdateOfRepresentations_();

					QString message;
					message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
													minimizer_->getNumberOfIterations(), 
													ff.getEnergy(), ff.getRMSGradient());
					output_(message.ascii());
				}

				updateScene_();

				output_(ff.getResults());
				output_("final RMS gadient    : " + String(ff.getRMSGradient()) + " kJ/(mol A)   after " 
								+ String(minimizer_->getNumberOfIterations()) + " iterations\n",
								true);
				finish_();

				if (!ok)
				{
					output_("Aborted EnergyMinimizer because of strange energy values.", true);
					return;
				}
			}
			catch(Exception::GeneralException e)
			{
				String txt = String("Calculation aborted because of throwed exception: ")
											+ __FILE__ + " " + __LINE__ + " " + e.getMessage();
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
					output_(message.ascii());
					

					if (save_images_) exportSceneToPNG_();
					if (dcd_file_) 		manager.takeSnapShot();
				}

				if (dcd_file_) manager.flushToDisk();

 				output_(ff.getResults());
				output_("final RMS gadient    : " + String(ff.getRMSGradient()) + " kJ/(mol A)   after " 
								+ String(md_->getNumberOfIterations()) + " iterations\n", 
								true);

				if (!ok)
				{
					output_("Aborted MDSimulation because of strange energy values.", true);
				}

				finish_();
			}
			catch(Exception::GeneralException e)
			{
				String txt = String("Calculation aborted because of throwed exception: ")
											+ __FILE__ + " " + __LINE__ + " " + e.getMessage();
				output_(txt, true);
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
			if (md_ != 0)
			{ 
				delete md_;
			}
		}


		void MDSimulationThread::setMolecularDynamics(MolecularDynamics* md)
		{ 
			if (md_ != 0)
			{
				delete md_;
			}
			
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

	} // namespace VIEW
} // namespace BALL

#endif //Thread support
