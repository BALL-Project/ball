// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.h,v 1.17 2004/11/14 22:42:02 amoll Exp $

#ifndef  BALL_VIEW_KERNEL_PRIMITIVEMANAGER_H
#define  BALL_VIEW_KERNEL_PRIMITIVEMANAGER_H

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
#	include <BALL/VIEW/KERNEL/representation.h>
#endif

#ifdef BALL_QT_HAS_THREADS
# include <qmutex.h>
# include <qwaitcondition.h>
#endif

namespace BALL
{
	class Composite;

	namespace VIEW
	{
		class MainControl;
		class UpdateRepresentationThread;

		/** PrimitiveManager manages the graphical Representation objects and all GeometricObject.
		 		All Representation objects which shall be inserted should be created using createRepresentation().
				When the PrimitiveManager is destroyed, all inserted Representation are deleted.
				The PrimitiveManager has also the capability for multithreaded updateing of
				the Representation's:\\
				Representation::update() calls PrimitiveManager::update_(Representation)\\
				if the code is build with support for multithreading. The update itself
				is done in an instance of UpdateRepresentationThread.
				\ingroup ViewKernelGeometricPrimitives
		*/
		class BALL_EXPORT PrimitiveManager
			:	public Object
		{
			friend class Representation;
			friend class UpdateRepresentationThread;
			friend class MainControl;

			public:

			BALL_CREATE(PrimitiveManager)

			/**	@name Type definitions
			*/
			//@{
			///
			typedef List<Representation*> RepresentationList;

			/// Iteration to the Representations
			typedef RepresentationList::Iterator RepresentationsIterator;

			/// ConstIterator to the Representations
			typedef RepresentationList::ConstIterator RepresentationsConstIterator;

			//@}
			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor
			 		The MainControl pointer is needed to send Messages.
			*/
			PrimitiveManager(MainControl* mc = 0)
				throw();

			/** Copy constructor
			*/
			PrimitiveManager(const PrimitiveManager& pm)
				throw();

			/** Destructor
			*/
			virtual ~PrimitiveManager()
				throw();

			//@}
			/**	@name	Accessors
			*/	
			//@{
			
			///
			const PrimitiveManager& operator = (const PrimitiveManager& pm)
				throw();

			///
			bool operator == (const PrimitiveManager& pm) const
				throw();
			
			/// Clears also the representations
			void clear()
				throw();

			/** Delete a representation.
					It will be removed from the list of representations, 
					its GeometricObject will be deleted. 
					If send_message is set to true, a RepresentationMessage with
					type REMOVE is send to notify all ModularWidget's.
			*/
			bool remove(Representation& representation, bool send_message = true)
				throw();

			/** Insert a Representation
					If send_message is set to true, a RepresentationMessage with
					type ADD is send to notify all ModularWidget's.
			*/
			bool insert(Representation& representation, bool send_message = true)
				throw();

			/// Get the list with the Representations
			const RepresentationList& getRepresentations() const
				throw() { return representations_;}

			/// Get the number of the Representations
			Size getNumberOfRepresentations() const
				throw() { return representations_.size();}

			/// Create a Representation and insert it
			Representation* createRepresentation()
				throw();

			/// Test if a Representation is inserted
			bool has(const Representation& representation) const
				throw();
			
			/// Dump the internal state to an output stream
			void dump(std::ostream& s, Size depth) const
				throw();

			/// Iterator to the first Representation
			RepresentationsIterator begin() 
				throw() { return representations_.begin();}

			/// ConstIterator to the first Representation
			RepresentationsConstIterator begin() const
				throw() { return representations_.begin();}
			
			/// Iterator pointing behind the last Representation
			RepresentationsIterator end() 
				throw() { return representations_.end();}

			/// Const Iterator pointing behind the last Representation
			RepresentationsConstIterator end() const
				throw() { return representations_.end();}

			/** Clean up after a Composite was removed.
			 		Method is called in MainControl, after removal of
					a composite (e.g. a System) and it removes all
					representations, which contain the Composite.
					\return a list with the pointers of all removed representations.
			*/
			List<Representation*> removedComposite(const Composite& composite)
				throw();

			/** Get a list of Representation, which were build for a Composite.
			 		Method is called in MainControl, after receiving CompositeChangedMessage.
					\return a list with the pointers of representations, which are to be updated.
			*/
			List<Representation*> getRepresentationsOf(const Composite& composite)
				throw();

			/// Return true if a Representation will be updated
			bool willBeUpdated(const Representation& rep) const
				throw();

			/// Return true, if a Representation is currently beeing updated
			bool updateRunning() const
				throw();

			#ifdef BALL_QT_HAS_THREADS
			/** Get the UpdateRepresentationThread, which updates one Representation.
					(Only used in Multithreaded code.)
			*/
			static UpdateRepresentationThread& getUpdateThread() { return thread_;}

			/** The follow QWaitCondition can be used to let a thread wait until
			 		all Representations have been updated.
					This is e.g. used by SimulationThread.
					(e.g. getMainControl()->getUpdateWaitCondition().wait();)
					(Only used in Multithreaded code.)
			*/
			QWaitCondition& getUpdateWaitCondition() { return update_finished_;}
			#endif

			/** Used by SimulationThread to notify the PrimitiveManager of
			 		an pending update, which will start soon.
					This is needed to sync the main thread and the SimulationThread.
					(Only used in Multithreaded code.)
			*/
			void notifyOfPendingingUpdate() { update_pending_ = true;}

			/** Returns true if an Update will accour soon or an update is running
					(Only used in Multithreaded code.)
			*/
			bool updatePending() { return update_pending_;}
			
			protected:

			/*_ Start the UpdateRepresentationThread with one Representation
					(Only used in Multithreaded code.)
			*/
			void startUpdateThread_()
				throw();

			/*_ Called by Representation::update() to start a multithreaded
			 		Update of the Representation.
					(Only used in Multithreaded code.)
			*/
			void update_(Representation& rep)
				throw();

			/*_ Called by UpdateRepresentationThread, when it is finished.
			 		If an other Representation is waiting to be updated, 
					startUpdateThread_() is called again.
					(Only used in Multithreaded code.)
			*/
			void finishedUpdate_()
				throw();
			
			//_ List with all representations
			RepresentationList representations_;
			
			//_ List with all representations, which will be updated
			RepresentationList representations_to_be_updated_;
			
			#ifdef BALL_QT_HAS_THREADS
			static UpdateRepresentationThread thread_;
			static QMutex 										mutex_;
			QWaitCondition 										update_finished_;
			#endif

			MainControl* 	main_control_;
			bool 					update_running_;
			bool 					update_pending_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_PRIMITIVEMANAGER_H
