#ifndef BALL_VIEW_KERNEL_THREADS_H
#define BALL_VIEW_KERNEL_THREADS_H

#ifndef BALL_VIEW_KERNEL_COMMON_H_
#include <BALL/VIEW/KERNEL/common.h>
#endif

#include <QtCore/QThread>
#include <QtCore/QEvent>

namespace BALL
{
	class EnergyMinimizer;
	class MolecularDynamics;
	class Composite;
	class DockingAlgorithm;
	class TrajectoryFile;

namespace VIEW
{
	class MainControl;
	class FDPBDialog;
	class Message;
	class Representation;
	class GenerateCrystalDialog; 

	/** A QT Event, that can contain a VIEW message.
	 		Use this class to post events and thus messages over thread boundaries.
			Create the Message on the heap, it will be automatically deleted after sending!
	*/
	class BALL_VIEW_EXPORT MessageEvent
		: public QEvent
	{
		public:

		///
		MessageEvent(Message* msg = 0)
		: QEvent((QEvent::Type)MESSAGE_EVENT),
			message_(msg)
		{}

		///
		void setMessage(Message* msg) { message_ = msg; }

		///
		Message* getMessage() { return message_; }

		protected:

		Message* message_;
	};


	/** Baseclass for threads in BALL.
			To use multithreading in BALL, there are several problems arising from
			the fact, that Qt itself is not threadsafe:
			1. Prevent any output to Log, stdout or sterr.
				 If a simulation has to print some informations, use the method output_,
				 which will send an Event to the GUI-thread.
			2. Dont call any Qt-methods directly. If you need some interaction with the
				 GUI, send a QEvent to the MainControl.
			3. Especially dont call the refresh methods for the Scene or 
				 GL-methods. Use updateStructure_() instead.
			4. Overload the run method to insert the multithreaded code.
			5. Call setMainControl() before running the thread
			6. Before running a thread, lock the composites with ModularWidget::lockComposites().
			7. After running a thread, unlock them with ModularWidget::unlockComposites().
			If you dont pay attention to these rules, dont wonder if BALLView should freeze or crash!
	*/
	class BALL_VIEW_EXPORT BALLThread
		: public virtual QThread
	{
		public:

			///
			BALLThread();
			
			///
			void setMainControl(MainControl* mf) {main_control_ = mf;}
			
			///
			void setComposite(Composite* composite) {composite_ = composite;}

			///
			Composite* getComposite() { return composite_;}
	
			/** Overloaded from QThread::run(), overload again in derived classes!
			*/
			virtual void run() {};
			
			protected:

			/** Send a VIEW Message to the main thread.
			 		Create the message on the heap!
			*/
			void sendMessage_(Message* msg);

			/** Provokes an update of the Representation's and the Scene 
			 		with rebuild of the display-lists.
					This method sends a CompositeMessage with a pointer to the Composite and
					type CHANGED_COMPOSITE.
					As a result, the method MainControl::updateRepresentationsOf(..) will be called.
			*/
			void updateStructure_();

			/// Sends the string as outout to Log.info
			void output_(const String& string, bool important = false);

			/** Wait until all Representation's are rebuild, by using
					RepresentationManager::updatePending() and \\
					RepresentationManager::getUpdateWaitCondition().wait() .
			*/
			void waitForUpdateOfRepresentations_();
	
			MainControl* 	main_control_;
			Composite* 		composite_;
		};

	///
	class BALL_VIEW_EXPORT CalculateFDPBThread
		: public BALLThread
	{
		public:

			///
			CalculateFDPBThread();

			///
			virtual void run();

			///
			void setFDPBDialog(FDPBDialog* dialog) {dialog_ = dialog;}

		protected:
			FDPBDialog* dialog_;
	};

	
	///
	class BALL_VIEW_EXPORT GenerateCrystalThread
		: public BALLThread
	{
		public:

			///
			GenerateCrystalThread();

			///
			virtual void run();

			///
			void setCrystalDialog(GenerateCrystalDialog* dialog) {dialog_ = dialog;}

		protected:
			GenerateCrystalDialog* dialog_;
	};

	///
	class BALL_VIEW_EXPORT UpdateRepresentationThread
		: public BALLThread
	{
		public:

			///
			UpdateRepresentationThread();

			///
			virtual void run();

			///
			void setRepresentation(Representation& rep)
				{ rep_ = &rep;}

			///
			Representation* getRepresentation()
				{ return rep_; }

		protected:

			Representation* rep_;
	};


	/** Baseclass for threads, which perform a simulation.
			At the end of the run() method, always call finish_() to notify the 
			main thread to delete the simulation thread, otherwise there will be 
			a memory leak.
	*/
	class BALL_VIEW_EXPORT SimulationThread
		: public BALLThread
	{
		public:

			///
			SimulationThread();

			///
			void setNumberOfStepsBetweenUpdates(Size steps)
				{ steps_between_updates_ = steps;}

			///
			void setTrajectoryFile(TrajectoryFile* file) {trajectory_file_ = file;}

			///
			TrajectoryFile* getTrajectoryFile() { return trajectory_file_;}

			protected:
	
			/// Notify the MainControl to export an PNG
			void exportSceneToPNG_();

			/// Notifies the main thread to delete the simulating thread
			void finish_();

			Size 					        steps_between_updates_;
			TrajectoryFile*   		trajectory_file_;
		};


		/// Thread for EnergyMinimization
		class BALL_VIEW_EXPORT EnergyMinimizerThread
			: public SimulationThread
		{
			public:

				///
				EnergyMinimizerThread();
				
				///
				virtual ~EnergyMinimizerThread();
				
				///
				virtual void run();

				///
				void setEnergyMinimizer(EnergyMinimizer* minimizer);

			protected:
				EnergyMinimizer* minimizer_;
		};


		/// Thread for MDSimulation
		class BALL_VIEW_EXPORT MDSimulationThread
			: public SimulationThread
		{
			public:

				///
				MDSimulationThread();
				
				///
				virtual ~MDSimulationThread();
				
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


		/// Thread for Docking
		class BALL_VIEW_EXPORT DockingThread
			: public BALLThread
		{
			public:
				///
				DockingThread();
				
				///
				virtual ~DockingThread();
				
				/**  Assignment operator
				 */
				const DockingThread& operator =(const DockingThread& dock_thread);
					
				///
				void setDockingAlgorithm(DockingAlgorithm* dock_alg);
					
				///
				virtual void run()
					throw(Exception::NullPointer);
					
			protected:
				DockingAlgorithm* dock_alg_;
		};
	
	}
}
#endif //BALL_VIEW_KERNEL_THREADS_H
