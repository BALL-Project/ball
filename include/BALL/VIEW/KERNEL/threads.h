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

#ifndef BALL_SYSTEM_TCPTRANSFER
# include <BALL/SYSTEM/TCPTransfer.h>
#endif

#include <qthread.h>
#include <qevent.h>

namespace BALL
{
	class EnergyMinimizer;
	class MolecularDynamics;
	class Composite;

namespace VIEW
{
	class MainControl;
	class FDPBDialog;

	/** Baseclass for threads in BALL.
			To use multithreading in BALL, there are several problems arising from
			the fact, that QT itself is not threadsafe:
			1. Prevent any output to Log, stdout or sterr.
				 If a simulation has to print some informations, use the method output_,
				 which will send an Event to the GUI-thread to do this.
			2. Dont call any QT-methods directly. If you need some interaction with the
				 GUI, create a new Event-class and send this event to the MainControl.
			3. Especially dont call the refresh methods for the Scene or 
				 GL-methods. Use updateScene_() instead.
			4. Overload the run method to insert the multithreaded code.
			5. Before running a thread, lock the composites with ModularWidget::lockComposites().
			6. After running a thread, unlock them with ModularWidget::unlockComposites().
			If you dont pay attention to these rules, dont wonder if BALLView freezes
			or crashes!
	*/
	class BALL_EXPORT BALLThread
		: public QThread
	{
		public:

			///
			BALLThread();
			
			///
			void setMainControl(MainControl* mf) throw() {main_control_ = mf;}
			
			///
			void setComposite(Composite* composite) throw() {composite_ = composite;}

			///
			Composite* getComposite() throw() { return composite_;}
	
			/** Overloaded from QThread::run(), overload again in derived classes!
			*/
			virtual void run() {};
			
			protected:

			/** Provokes an update of the Representation's and the Scene 
			 		with rebuild of the display-lists.
					This method calls PrimitiveManager::notifyOfPendingingUpdate()
					and sends an UpdateCompositeEvent with a pointer to the Composite.
			*/
			void updateScene_();

			/// Sends the string as outout to Log.info
			void output_(const String& string, bool important = false);

			/** Wait until all Representation's are rebuild, by using
					PrimitiveManager::updatePending() and \\
					PrimitiveManager::getUpdateWaitCondition().wait() .
			*/
			void waitForUpdateOfRepresentations_();
	
			MainControl* 	main_control_;
			Composite* 		composite_;
		};

	
	/** Thread to fetch a file over TCP network traffic.
	 		The result can either be stored in a file or in a stringstream.
			This is the default, if no filename is given.
	*/
	class FetchHTMLThread
		: public BALLThread
	{
		public:
			///
			FetchHTMLThread()
				throw();

			///
			void setURL(const String& text)
				throw();

			///
			virtual void run();

			///
			void setFilename(const String& file_name) { file_name_ = file_name;}

			///
			const String& getFilename() const { return file_name_;}

			///
			TCPTransfer& getTCPTransfer() { return tcp_;}

			///
			std::stringstream& getStream() { return stream_;}

			protected:
			String url_;
			String file_name_;
			TCPTransfer tcp_;
			std::stringstream stream_;
	};
	
	///
	class CalculateFDPBThread
		: public BALLThread
	{
		public:

			///
			CalculateFDPBThread()
				throw();

			///
			virtual void run();

			///
			void setFDPBDialog(FDPBDialog* dialog)
				{dialog_ = dialog;}

		protected:
			FDPBDialog* dialog_;
	};


	///
	class UpdateRepresentationThread
		: public BALLThread
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
			Representation* getRepresentation()
				throw() { return rep_; }

		protected:

			Representation* rep_;
	};

	/** Baseclass for threads, which perform a simulation.
			At the end of the run() method, always call finish_() to notify the 
			main thread to delete the simulation thread, otherwise there will be 
			a memory leak.
	*/
	class BALL_EXPORT SimulationThread
		: public BALLThread
	{
		public:

			///
			SimulationThread();

			///
			void setNumberOfStepsBetweenUpdates(Size steps) throw()
				{ steps_between_updates_ = steps;}

			///
			void setDCDFile(DCDFile* file) throw() {dcd_file_ = file;}

			///
			DCDFile* getDCDFile() throw() { return dcd_file_;}

			protected:
	
			/// Notify the MainControl to export an PNG
			void exportSceneToPNG_();

			/// Notifies the main thread to delete the simulating thread
			void finish_();

			Size 					steps_between_updates_;
			DCDFile*   		dcd_file_;
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
				SimulationOutput();

				///
				void setMessage(const String& msg) {message_ = msg;}

				///
				String getMessage() {return message_;}

				/// will allways be shown in Statusbar or just when no other message shown?
				bool isImportant() { return important_;}

				///
				void setImportant(bool state) { important_ = state;}

			protected:
				String message_;
				bool   important_;
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
		
		///
		class BALL_EXPORT FinishedRepresentionUpdateEvent
			: public QCustomEvent
		{
			public:
				///
				FinishedRepresentionUpdateEvent()
					:QCustomEvent(FINISHED_REPRESENTATION_UPDATE_EVENT)
				{}
		};

	}
}
#endif //BALL_QT_HAS_THREADS
#endif //BALL_VIEW_KERNEL_THREADS_H
