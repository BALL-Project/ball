// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.h,v 1.12 2004/11/12 15:40:31 amoll Exp $

#ifndef  BALL_VIEW_KERNEL_PRIMITIVEMANAGER_H
#define  BALL_VIEW_KERNEL_PRIMITIVEMANAGER_H

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
#	include <BALL/VIEW/KERNEL/representation.h>
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
					its GeometricObject will be deleted, but it will not be removed from the screen.
					To do this, send a SceneMessage or a RepresentationMessage.\par
					Calls Representation::clear.
			*/
			bool remove(Representation& representation)
				throw();

			/// Insert a Representation
			bool insert(Representation& representation)
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

			///
			void setMainControl(MainControl& mc)
				throw() { main_control_ = &mc;}

			/// Return true, if a Representation is currently beeing updated
			bool updateRunning() const
				throw();

			#ifdef BALL_QT_HAS_THREADS
			///
			static UpdateRepresentationThread& getUpdateThread() { return thread_;}
			#endif

			/// used by SimulationThread to know when UpdateThread will be finished
			void willUpdateSoon()
				throw();

			///
			bool updatePending() const
				throw();
			
			protected:

			void startUpdateThread_(Representation& rep)
				throw();

			void update_(Representation& rep)
				throw();

			void finishedUpdate_()
				throw();

			//_ List with all representations
			RepresentationList representations_;
			
			//_ List with all representations, which will be updated
			RepresentationList representations_to_be_updated_;
			
			//_ List with all representations, which are to be deleted
			HashSet<Representation*> representations_to_be_deleted_;

			#ifdef BALL_QT_HAS_THREADS
			static UpdateRepresentationThread thread_;
			#endif

			MainControl* 	main_control_;
			bool 					update_still_to_be_started_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_PRIMITIVEMANAGER_H
