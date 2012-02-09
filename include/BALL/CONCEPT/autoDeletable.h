// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#ifndef BALL_CONCEPT_AUTODELETABLE_H
#define BALL_CONCEPT_AUTODELETABLE_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_COMMON_H
#	include <BALL/COMMON/debug.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <cstdlib>
#include <new>
#include <iostream>

namespace BALL 
{

	/**	Auto-deletable concept.
			This class allows the distinction between objects that may be deleted
			automatically (because they are created dynamically on the heap) and
			instances that are static and should not be deleted automatically. \par
			Standard application is the class  \link Composite Composite \endlink . Composites may
			contain other composites (tree-like structure). If a composite is
			deleted, it has to decide whether all composites contained therein
			are to be deleted, too.  If these composites are static objects,
			calling their destructor often is fatal!  The same is true for
			objects contained in an array.  Therefore, composite is derived from
			 \link AutoDeletable AutoDeletable \endlink . \par
			The AutoDeletable class determines whether it was instantiated via
			<b>new</b> and then sets an internal flag to false. The mechanism to
			determine this is as follows: \par
			AutoDeletable has on overloaded new operator. When invoked, this
			operator allocates storage for an instance of AutoDeletable using the
			global new operator and remembers the address of in a private
			variable <b>last_ptr_</b>.  Each constructor of AutoDeletable checks
			whether its <b>this</b> pointer is equal to the address stored in 
			<b>last_ptr_</b>. If both pointers are equal, the object has been created
			using the new operator and so it is safe to delete it automatically.
			If the adresses do not match, the object is either part of an array
			or static and should not be deleted automatically. \par
			The state of each object may be changed after it is constructed by a
			call to  \link setAutoDeletable setAutoDeletable \endlink . This might be useful to protect
			certain instances of objects, however usually this should not be
			neccessary. \par
			
	 	 \ingroup ConceptsMiscellaneous
	*/
	class BALL_EXPORT AutoDeletable
	{
		public:

		/**	@name	Constructors and Destructors
				The constructors of this class are protected.
		*/
		//@{

		/**	Destructor
		*/
		virtual ~AutoDeletable()
			;

		/**	<b>new</b> operator.
				This operator allocates storage for the object and remembers its
				pointer. This pointer is <b>static</b> and is evaluated by the
				constructors. As this operator is only invoked for the creation of
				single dynamic objects, arrays and static objects can be
				identified.
		*/
		void* operator new(size_t size) 
			;
	
		/**	<b>delete</b> operator.
				This operator frees the space allocated for an Autodeletable
				object. It is implemented solely to achieve a consistent interface
				and to avoid warnings issued by some compilers if operator
				new/delete do not appear in pairs.
		*/
		void operator delete(void* ptr) 
			;
	
		/**	Placement <b>new</b> operator.
				This operator allocates storage for the object and remembers its
				pointer.  This pointer is <b>static</b> and is evaluated by the
				constructors.  As this operator is only invoked for the creation of
				single dynamic objects, arrays and static objects can be
				identified.
		*/
		void* operator new(size_t size, void* ptr)
			;
	
		/**	Placement <b>delete</b> operator.
				This operator frees the space allocated for an Autodeletable
				object. It is implemented solely to achieve a consistent interface
				and to avoid warnings issued by some compilers if operator
				new/delete do not appear in pairs.
		*/
		void operator delete(void* ptr, void*)
			;
	
		//@}
	
		/**	@name	Mutators
		*/
		//@{
	
		/**	Mark the object as auto-deletable.
				Objects can be marked as deletable or not deletable by this method.
				Use this method to protect objects from automatic deletion.
				You should never set static objects to autodeletable, as 
				invoking <b>delete</b> on a static object may result in a crash.
		*/
		void setAutoDeletable(bool enable)
			;

		/* This is required for cleaning up in case we want to
			 valgrind our stuff -- otherwise we get "still reachable"
       blocks through this ptr.
		*/
		static void clearLastPtr() { last_ptr_ = 0; }
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Query the objects status.
				Returns <b>true</b> if the object should be automatically
				deleted if the objects it is contained in are deleted.
				Recursive destruction methods should honor this flag and 
				should not call the destructor for objects that return <b>true</b>.
		*/
		bool isAutoDeletable() const
			;
		//@}


		protected:

		/*_	Default constructor.
				Should be called by subclasses only. There's no need to construct
				AutoDeletable objects by themselves.
		*/
		AutoDeletable()
			;

		/*_ Copy constructor.
		*/
		AutoDeletable(const AutoDeletable& auto_deletable, bool deep = false)
			;


		private:

		/*_	Flag describing whether the object may be deleted automatically.
		*/
		bool enabled_;

		/*_ The last new pointer.
				This pointe ris used internally to determine whether a given 
				instance of AutoDeletable was constructed statically or dynamically.
		*/
		static 	void* last_ptr_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/autoDeletable.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_AUTODELETABLE_H
