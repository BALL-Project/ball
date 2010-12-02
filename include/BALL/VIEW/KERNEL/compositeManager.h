// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_COMPOSITEMANAGER_H
#define BALL_VIEW_KERNEL_COMPOSITEMANAGER_H

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

namespace BALL
{
	class Composite;
	class System;

	namespace VIEW
	{
		/** CompositeManager class.	
		*/
		/** Management of Composites.
		 		This class is a container for all Composite objects, which are used in an application.
				When the CompositeManager is destroyed, all inserted Composites are deleted.
				\ingroup ViewKernelOther
		*/
		class BALL_VIEW_EXPORT CompositeManager
		{
			public:
			/**	@name	Typedefs for STL compliance
			*/	
			//@{
			typedef Composite*					value_type;
			typedef Composite *&				reference;
			typedef Composite**					pointer;
			typedef const Composite*&	  const_reference;
			typedef const Composite**	  const_pointer;
			typedef Index								difference_type;			
			typedef Size								size_type;
			typedef HashSet<Composite*>::Iterator iterator;
				
			///
			typedef HashSet<Composite*>::Iterator CompositeIterator;

			///
			typedef HashSet<Composite*>::ConstIterator CompositeConstIterator;

			//@}
			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			CompositeManager();

			/** Copy Constructor
			*/
			CompositeManager(const CompositeManager& cm);

			/** Destructor.
			*/
			virtual ~CompositeManager();

			/** Explicit default initialization.
			*/
			virtual void clear();

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Test if the CompositeManager has the Composite itself or one of its ancestors.
			 		This method also works if the Composite might have been deleted, but it is quite slow O(n) with n = number of all Composites !
			*/
			bool has(const Composite* composite) const;

			/** Test if the CompositeManager has the Composite itself as root entry (no search for childs or descendents).
			 		Quite Fast with O(log n) with n = number of roots (Systems).
			*/
			bool hasRoot(const Composite* composite) const;

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Insert a Composite.
			 		Composites which are descendents of already inserted Composites can not be inserted.
					\return true, if the Composite could be inserted
			*/
			bool insert(Composite& composite);

			/** Remove a Composite.
			 		The Composite and its descendents will be destroyed if to_delete is true
			*/
			void remove(Composite& composite, bool to_delete = true);

			/** Return the number of inserted Composites
			 */
			Size getNumberOfComposites() const
				{ return composite_set_.size();}

			//@}
			/**	@name	STL Iterator compliance
			*/
			//@{

			///
			CompositeIterator begin()
				{ return composite_set_.begin();}

			///
			CompositeIterator end()
				{ return composite_set_.end();}

			///
			CompositeConstIterator begin() const
				{ return composite_set_.begin();}

			///
			CompositeConstIterator end() const
				{ return  composite_set_.end();}

			///
			HashSet<Composite*>& getComposites()
				{ return composite_set_;}

			///
			const HashSet<Composite*>& getComposites() const
				{ return composite_set_;}

			/** Internal value dump.
					Dumps the current state to a output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			//@}
			
			private:

			HashSet<Composite*> composite_set_;
			HashSet<String> 		name_set_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COMPOSITEMANAGER_H
