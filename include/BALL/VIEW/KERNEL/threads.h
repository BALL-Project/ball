#ifndef BALL_VIEW_KERNEL_THREADS_H
#define BALL_VIEW_KERNEL_THREADS_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
#include <BALL/VIEW/KERNEL/representation.h>
#endif

#ifndef BALL_FORMAT_DCDFILE_H
#include <BALL/FORMAT/DCDFile.h>
#endif

#ifndef BALL_SYSTEM_TCPTRANSFER
# include <BALL/SYSTEM/TCPTransfer.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#include <QtCore/qthread.h>
#include <QtGui/qevent.h>
#include <QtCore/QCustomEvent>

namespace BALL
{
	class EnergyMinimizer;
	class MolecularDynamics;
	class Composite;
	class DockingAlgorithm;
	class ForceField;
	class ConformationSet;

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
			5. Call setMainControl() before running the thread
			6. Before running a thread, lock the composites with ModularWidget::lockComposites().
			7. After running a thread, unlock them with ModularWidget::unlockComposites().
			If you dont pay attention to these rules, dont wonder if BALLView freezes
			or crashes!
	*/
	class BALL_VIEW_EXPORT BALLThread
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
					This method calls RepresentationManager::notifyOfPendingingUpdate()
					and sends an UpdateCompositeEvent with a pointer to the Composite.
			*/
			void updateScene_();

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

	
	/** Thread to fetch a file over TCP network traffic.
	 		The result can either be stored in a file or in a stringstream.
			This is the default, if no filename is given.
	*/
	class BALL_VIEW_EXPORT FetchHTMLThread
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
			virtual void abort() { tcp_.abort();}

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
	class BALL_VIEW_EXPORT CalculateFDPBThread
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
	class BALL_VIEW_EXPORT UpdateRepresentationThread
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
	class BALL_VIEW_EXPORT SimulationThread
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


		/** This class is only intended for usage with multithreading.
				It notifies the MainControl, that the thread for simulations has finished and can be deleted.
				This should only be used internaly.
		*/
		class BALL_VIEW_EXPORT SimulationThreadFinished
			: public QEvent
		{
			public:
				SimulationThreadFinished()
					: QEvent((QEvent::Type)SIMULATION_THREAD_FINISHED_EVENT ){}
		};

		///
		class BALL_VIEW_EXPORT UpdateCompositeEvent
			: public QEvent
		{
			public:
				///
				UpdateCompositeEvent()
					:QEvent((QEvent::Type)UPDATE_COMPOSITE_EVENT),
					 composite_(0){}

				///
				void setComposite(const Composite* composite) { composite_ = composite;}
				
				///
				const Composite* getComposite() const { return composite_;}

			protected:
				const Composite* composite_;
		};
		
		///
		class BALL_VIEW_EXPORT FinishedRepresentionUpdateEvent
			: public QEvent
		{
			public:
				///
				FinishedRepresentionUpdateEvent()
					:QEvent((QEvent::Type)FINISHED_REPRESENTATION_UPDATE_EVENT),
					 rep_(0)
				{}

				///
				void setRepresentation(Representation* rep) { rep_ = rep;}

				///
				Representation* getRepresentation() { return rep_;}

			protected:

				Representation* rep_;
		};

		/// Thread for Docking
		class BALL_VIEW_EXPORT DockingThread
			: public BALLThread
		{
			public:
				///
				DockingThread()
					throw();
				
				/** Copy constructor.
					*/
				DockingThread(const DockingThread& dock_thread)
					throw();
					
				///
				virtual ~DockingThread()
					throw();
				
				/**  Assignment operator
				 */
				const DockingThread& operator =(const DockingThread& dock_thread)
					throw();
					
				///
				void setDockingAlgorithm(DockingAlgorithm* dock_alg)
					throw();

				///
				void setDockingPartner1(System* s)
					throw();

				///
				void setDockingPartner2(System* s)
					throw();

				///
				void setDockingOptions(Options& opt)
					throw();

				///
				void setForceField(ForceField* ff)
					throw();
					
				///
				virtual void run()
					throw(Exception::NullPointer);
					
			protected:

				DockingAlgorithm* dock_alg_;
				System* docking_partner1_;
				System* docking_partner2_;	
				Options algorithm_opt_;
				ForceField* ff_;

		};


		///
		class BALL_VIEW_EXPORT DockingFinishedEvent
			: public QEvent
		{
			public:
				
				///
				DockingFinishedEvent()
					:QEvent((QEvent::Type)DOCKING_FINISHED_EVENT),
					 conformation_set_(0)
				{}

				///
				DockingFinishedEvent(bool abort)
					:QEvent((QEvent::Type)DOCKING_FINISHED_EVENT),
					 conformation_set_(0)
				{
				 	abort_ = abort;
				}
				 
				///
				void setConformationSet(const ConformationSet* conformation_set)
				{
					conformation_set_ = conformation_set;
				}
				
				//
				const ConformationSet* getConformationSet() const
				{
					return conformation_set_;
				}
				
				///
				bool wasAborted()
				{
		 			return abort_;
				}
				
				protected:

				/// this conformation set is deleted in DockResult
				const ConformationSet* conformation_set_;
				bool abort_;
		};
		
	}
}
#endif //BALL_VIEW_KERNEL_THREADS_H
