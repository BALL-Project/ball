#ifndef BALL_APPLICATIONS_MOLVIEW_THREADS_H
#define BALL_APPLICATIONS_MOLVIEW_THREADS_H

#ifndef BALL_COMMON_H
	#include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
 #include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_DCDFILE_H
 #include <BALL/FORMAT/DCDFile.h>
#endif

#include <qthread.h>

namespace BALL
{
	class EnergyMinimizer;
	class MolecularDynamics;
	class Mainframe;
	class AmberFF;

	/** Baseclass for threads in molview.
	 		To use multithreading in molview, there are several problems arising from
			the fact, that QT itself is not threadsafe:
			1. Prevent any output to Log, stdout or sterr.
				 If a simulation has to print some informations, use the method output_,
				 which will send an Event to the GUI-thread to do this.
			2. Dont call any QT-methods directly. If you need some interaction with the
				 GUI, create a new Event-class and send this event to the Mainframe.
			3. Especially dont call the refresh methods for the Scene or 
				 GL-methods. Use updateScene_() instead.
			4. At the end of the run() method, always call finish_() to notify the 
				 main thread to delete the simulation thread, otherwise there will be 
				 a memory leak.
			If you dont pay attention to these points, dont wonder if molview freezes
			or crashes!
	*/
	class SimulationThread
		: public QThread
	{
		public:

			///
			SimulationThread();
			
			///
			virtual void run();

			///
			void setNumberOfStepsBetweenUpdates(Size steps) 
				{ steps_between_updates_ = steps;}

			///
			void setMainframe(Mainframe* mf) {main_frame_ = mf;}

			void setDCDFileName(const String& name) {dcd_file_name_ = name;}

		protected:

			/// Provokes an update of the Scene with rebuild of the display-lists
			void updateScene_();

			/// Sends the string as outout to Log.info
			void output_(const String& string);

			/// Prints the results from an AMBER-forcefield
			void outputAmberResult_(const AmberFF& amber);

			/// Notifies the main thread to delete the simulating thread
			void finish_();

			Size steps_between_updates_;
			Mainframe* main_frame_;
			String dcd_file_name_;
		};


	/// Thread for EnergyMinimization
	class EnergyMinimizerThread
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
	class MDSimulationThread
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

}
#endif
