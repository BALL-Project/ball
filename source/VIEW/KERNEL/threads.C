// Do nothing, if no thread support

#include <BALL/VIEW/KERNEL/threads.h>
#ifdef BALL_QT_HAS_THREADS

#include <BALL/VIEW/MODELS/modelProcessor.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <qapplication.h>

namespace BALL
{
	namespace VIEW
	{

		UpdateRepresentationThread::UpdateRepresentationThread()
			throw() 
			: QThread(),
				rep_(0)
		{
		}


		void UpdateRepresentationThread::run()
		{
			if (rep_ == 0) return;
			rep_->update_(rebuild_);
		}


		SimulationThread::SimulationThread()
			: QThread(),
				main_control_(0),
				dcd_file_(0),
				composite_(0)
		{
		}

		void SimulationThread::run()
		{
			// overloaded in derived classes
		}

		void SimulationThread::updateScene_()
		{
			if (main_control_->stopedSimulation()) return;

			// notify MainControl to update all Representations for the Composite
			UpdateCompositeEvent* se = new UpdateCompositeEvent;
			se->setComposite(composite_);
			qApp->postEvent(main_control_, se);

			// wait until MainControl tell us we can continue
			mutex_.lock();
		}

		void SimulationThread::output_(const String& string)
		{
			if (main_control_->stopedSimulation()) return;

			SimulationOutput* su = new SimulationOutput;
			su->setMessage(string);
			qApp->postEvent(main_control_, su);  // Qt will delete it when done
		}

		void SimulationThread::finish_()
		{
			SimulationThreadFinished* su = new SimulationThreadFinished;
			qApp->postEvent(main_control_, su);  // Qt will delete it when done
		}

		void EnergyMinimizerThread::run()
		{
			try
			{
				if (minimizer_ == 0 ||
						minimizer_->getForceField() == 0 || 
						minimizer_->getForceField()->getSystem() == 0 ||
						main_control_ == 0)
				{
					throw Exception::NullPointer(__FILE__, __LINE__);
				}
				ForceField& ff = *minimizer_->getForceField();

				// iterate until done and refresh the screen every "steps" iterations
				while (!main_control_->stopedSimulation() &&
								minimizer_->getNumberOfIterations() < minimizer_->getMaxNumberOfIterations() &&
							 !minimizer_->minimize(steps_between_updates_, true))
				{
					updateScene_();

					QString message;
					message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
													minimizer_->getNumberOfIterations(), 
													ff.getEnergy(), ff.getRMSGradient());
					output_(message.ascii());
				}

				output_(ff.getResults());
				output_("final RMS gadient    : " + String(ff.getRMSGradient()) + " kJ/(mol A)   after " 
								+ String(minimizer_->getNumberOfIterations()) + " iterations\n");
				finish_();
			}
			catch(Exception::GeneralException e)
			{
				String txt = String("Calculation aborted because of throwed exception: ")
											+ __FILE__ + " " + __LINE__ + " " + e.getMessage();
				output_(txt);
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
				// iterate until done and refresh the screen every "steps" iterations
				
				while (md_->getNumberOfIterations() < steps_ &&
							 !main_control_->stopedSimulation())
				{
					md_->simulateIterations(steps_between_updates_, true);
					updateScene_();

					QString message;
					message.sprintf("Iteration %d: energy = %f kJ/mol, RMS gradient = %f kJ/mol A", 
													md_->getNumberOfIterations(), ff.getEnergy(),
													ff.getRMSGradient());
					output_(message.ascii());
					
					if (save_images_) 
					{
						Scene* scene= (Scene*) Scene::getInstance(0);
						scene->exportPNG();
					}

					if (dcd_file_) manager.takeSnapShot();
				}

				if (dcd_file_) manager.flushToDisk();

 				output_(ff.getResults());
				output_("final RMS gadient    : " + String(ff.getRMSGradient()) + " kJ/(mol A)   after " 
								+ String(md_->getNumberOfIterations()) + " iterations\n");
				finish_();
			}
			catch(Exception::GeneralException e)
			{
				String txt = String("Calculation aborted because of throwed exception: ")
											+ __FILE__ + " " + __LINE__ + " " + e.getMessage();
				output_(txt);
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

	} // namespace VIEW

} // namespace BALL

#endif //Thread support
