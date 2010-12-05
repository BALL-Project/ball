// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representationManager.h,v 1.1.4.2 2007/03/28 13:11:43 amoll Exp $

#ifndef  BALL_VIEW_KERNEL_REPRESENTATIONMANAGER_H
#define  BALL_VIEW_KERNEL_REPRESENTATIONMANAGER_H

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
#	include <BALL/VIEW/KERNEL/representation.h>
#endif

#ifndef BALL_SYSTEM_MUTEX_H
#include <BALL/SYSTEM/mutex.h>
#endif

#include <QtCore/QCoreApplication>
#include <QtCore/QWaitCondition>

#include <vector>

using std::vector;

namespace BALL
{
	class Composite;
	class INIFile;

	namespace VIEW
	{
		class MainControl;
		class UpdateRepresentationThread;
		class ClippingPlane;

		/** RepresentationManager manages the graphical Representation objects and all GeometricObject.
		 		All Representation objects which shall be inserted should be created using createRepresentation().
				When the RepresentationManager is destroyed, all inserted Representation are deleted.
				The RepresentationManager has also the capability for multithreaded updateing of
				the Representation's:\\
				Representation::update() calls RepresentationManager::update_(Representation)\\
				if the code is build with support for multithreading. The update itself
				is done in an instance of UpdateRepresentationThread.
				\ingroup ViewKernelGeometricPrimitives
		*/
		class BALL_VIEW_EXPORT RepresentationManager
			:	public Object
		{
			friend class Representation;
			friend class UpdateRepresentationThread;
			friend class BALLThread;
			friend class MainControl;

			public:

			BALL_CREATE(RepresentationManager)

			/**	@name Type definitions
			*/
			//@{
			///
			///
			typedef HashSet<Representation*> RepresentationSet;

			/// Iteration to the Representations
			typedef RepresentationList::iterator RepresentationsIterator;

			/// ConstIterator to the Representations
			typedef RepresentationList::const_iterator RepresentationsConstIterator;

			//@}
			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor
			 		The MainControl is needed for sending Messages.
			*/
			RepresentationManager(MainControl* mc = 0);

			/** Copy constructor
			*/
			RepresentationManager(const RepresentationManager& pm);

			/** Destructor
			*/
			virtual ~RepresentationManager();

			//@}
			/**	@name	Accessors
			*/	
			//@{
			
			///
			const RepresentationManager& operator = (const RepresentationManager& pm);

			///
			bool operator == (const RepresentationManager& pm) const;
			
			/// Clears also the representations
			void clear();

			/** Delete a representation.
					It will be removed from the list of representations, 
					its GeometricObject will be deleted. 
					If send_message is set to true, a RepresentationMessage with
					type REMOVE is send to notify all ModularWidget's.
			*/
			bool remove(Representation& representation, bool send_message = true);

			/** Insert a Representation
					If send_message is set to true, a RepresentationMessage with
					type ADD is send to notify all ModularWidget's.
			*/
			bool insert(Representation& representation, bool send_message = true);

			/// Get the list with the Representations
			const RepresentationList& getRepresentations() const
				{ return representations_;}

			/// Get the number of the Representations
			Size getNumberOfRepresentations() const
				{ return representations_.size();}

			/// Create a Representation and insert it
			Representation* createRepresentation();

			/// Test if a Representation is inserted
			bool has(const Representation& representation) const;
			
			/// Dump the internal state to an output stream
			void dump(std::ostream& s, Size depth = 0) const;

			/// Iterator to the first Representation
			RepresentationsIterator begin() 
				{ return representations_.begin();}

			/// ConstIterator to the first Representation
			RepresentationsConstIterator begin() const
				{ return representations_.begin();}
			
			/// Iterator pointing behind the last Representation
			RepresentationsIterator end() 
				{ return representations_.end();}

			/// Const Iterator pointing behind the last Representation
			RepresentationsConstIterator end() const
				{ return representations_.end();}

			/** Clean up after a Composite was removed.
			 		Method is called in MainControl, after removal of
					a composite (e.g. a System) and it removes all
					representations, which contain the Composite.
					It can also update all Representations, which have still have other Composites than the one to be deleted.
					\return a list with the pointers of all removed representations.
			*/
			RepresentationList removedComposite(const Composite& composite, bool update = true);

			/** Get a list of Representation, which were build for a Composite.
			 		Method is called in MainControl, after receiving CompositeChangedMessage.
					\return a list with the pointers of representations, which are to be updated.
			*/
			RepresentationList getRepresentationsOf(const Composite& composite);

			///
			void rebuildAllRepresentations();

			///
			const vector<ClippingPlane*>& getClippingPlanes() const { return clipping_planes_;}

			///
			bool removeClippingPlane(ClippingPlane* plane);

			///
			void insertClippingPlane(ClippingPlane* plane);

			///
			void storeRepresentations(INIFile& out);
			
			///
			void restoreRepresentations(const INIFile& in, const vector<const Composite*>& new_systems);

			///
			void focusRepresentation(const Representation& rep);
			
			/// Return true if a Representation will be updated
			bool willBeUpdated(const Representation& rep) const;

			/// Return true, if a Representation is currently beeing updated
			bool updateRunning() const ;

			///
			bool startRendering(Representation* rep);

			///
			void finishedRendering(Representation* rep);
			
			/// Used by UpdateRepresentationThread
			bool isBeeingRendered(const Representation* rep) const;

			protected:
		
			///
			Representation* popRepresentationToUpdate();

			void finishedUpdate_(Representation* rep);

			/*_ Called by Representation::update() to start a multithreaded
			 		Update of the Representation.
					(Only used in Multithreaded code.)
			*/
			void update_(Representation& rep);

			//_ List with all representations
			RepresentationList representations_;
			
			HashSet<Representation*> beeing_rendered_;
			HashSet<Representation*> beeing_updated_;
			HashSet<Representation*> to_update_;

			vector<ClippingPlane*> clipping_planes_;
			
			// we need to ensure that the mutex is always created correctly before
			// the thread that tries to access it!
			mutable Mutex 							update_mutex_;

			UpdateRepresentationThread* thread_;
			bool 												no_update_;
			bool 												still_to_notify_;

			MainControl* 	main_control_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_REPRESENTATIONMANAGER_H
