#ifndef BALL_VIEW_KERNEL_THREADS_H
#define BALL_VIEW_KERNEL_THREADS_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifdef BALL_QT_HAS_THREADS

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
#include <BALL/VIEW/KERNEL/representation.h>
#endif

#ifndef BALL_FORMAT_DCDFILE_H
#include <BALL/FORMAT/DCDFile.h>
#endif

#include <qthread.h>
#include <qmutex.h>
#include <qevent.h>

namespace BALL
{
class EnergyMinimizer;
class MolecularDynamics;
class Composite;

namespace VIEW
{
	class MainControl;

	///
	class UpdateRepresentationThread
		: public QThread
	{
		public:

			///
			UpdateRepresentationThread()
				throw();

			///
			virtual void run();

			///
			void setRepresentation(Representation& rep)
				throw() { rep_ = &rep;}

			///
			void setRebuild(bool rebuild)
				throw(){ rebuild_ = rebuild;}

		protected:

			Representation* rep_;

			bool rebuild_;
	};

	/** Baseclass for threads, which perform a simulation.
			To use multithreading in BALL, there are several problems arising from
			the fact, that QT itself is not threadsafe:
			1. Prevent any output to Log, stdout or sterr.
				 If a simulation has to print some informations, use the method output_,
				 which will send an Event to the GUI-thread to do this.
			2. Dont call any QT-methods directly. If you need some interaction with the
				 GUI, create a new Event-class and send this event to the MainControl.
			3. Especially dont call the refresh methods for the Scene or 
				 GL-methods. Use updateScene_() instead.
			4. At the end of the run() method, always call finish_() to notify the 
				 main thread to delete the simulation thread, otherwise there will be 
				 a memory leak.
			If you dont pay attention to these rules, dont wonder if molview freezes
			or crashes!
			This class already has a mutex as member, so use it.
	*/
	class BALL_EXPORT SimulationThread
		: public QThread
	{
		public:

			///
			SimulationThread();
			
			/// Overloaded method QThread::run()
			virtual void run();

			///
			void setNumberOfStepsBetweenUpdates(Size steps) throw()
				{ steps_between_updates_ = steps;}

			///
			void setMainControl(MainControl* mf) throw() {main_control_ = mf;}

			///
			void setDCDFile(DCDFile* file) throw() {dcd_file_ = file;}

			///
			DCDFile* getDCDFile() throw() { return dcd_file_;}

			///
			void setComposite(Composite* composite) throw() {composite_ = composite;}

			///
			Composite* getComposite() throw() { return composite_;}

			///
			QMutex& getMutex() throw() { return mutex_;}

			protected:

			/// Provokes an update of the Scene with rebuild of the display-lists
			void updateScene_();

			/// Sends the string as outout to Log.info
			void output_(const String& string);

			/// Notifies the main thread to delete the simulating thread
			void finish_();

			QMutex mutex_;

			Size steps_between_updates_;
			MainControl* main_control_;
			DCDFile* dcd_file_;
			Composite* composite_;
		};


	/// Thread for EnergyMinimization
	class BALL_EXPORT EnergyMinimizerThread
		: public SimulationThread
	{
		public:

			///
			EnergyMinimizerThread();
			
			///
			~EnergyMinimizerThread();
			
			///
			virtual void run();

			///
			void setEnergyMinimizer(EnergyMinimizer* minimizer);

		protected:
			EnergyMinimizer* minimizer_;
	};


	/// Thread for MDSimulation
	class BALL_EXPORT MDSimulationThread
		: public SimulationThread
	{
		public:

			///
			MDSimulationThread();
			
			///
			~MDSimulationThread();
			
			///
			virtual void run()
				throw(Exception::NullPointer);

			///
			void setMolecularDynamics(MolecularDynamics* md);

			///
			void setNumberOfSteps(Size steps) { steps_ = steps;}

			///
			void setSaveImages(bool state) { save_images_ = state;}

		protected:
			MolecularDynamics* md_;
			Size steps_;
			bool save_images_;
	};


	/** This class is only intended for usage with multithreading.
			It notifies the MainControl, that the thread for simulations has finished and can be deleted.
			This should only be used internaly.
	*/
	class BALL_EXPORT SimulationThreadFinished
		: public QCustomEvent
	{
		public:
			SimulationThreadFinished()
				: QCustomEvent( SIMULATION_THREAD_FINISHED_EVENT ){}
	};

	///
	class BALL_EXPORT SimulationOutput
		: public QCustomEvent
	{
		public:
			///
			SimulationOutput()
				: QCustomEvent( SIMULATION_OUTPUT_EVENT ){}

			///
			void setMessage(const String& msg) {message_ = msg;}

			///
			String getMessage() {return message_;}

		protected:
			String message_;
	};

	///
	class BALL_EXPORT UpdateCompositeEvent
		: public QCustomEvent
	{
		public:
			///
			UpdateCompositeEvent()
				:QCustomEvent(UPDATE_COMPOSITE_EVENT),
				 composite_(0){}

			///
			void setComposite(const Composite* composite) { composite_ = composite;}
			
			///
			const Composite* getComposite() const { return composite_;}

		protected:
			const Composite* composite_;
	};


	}
}
#endif //BALL_QT_HAS_THREADS
#endif //BALL_VIEW_KERNEL_THREADS_H
