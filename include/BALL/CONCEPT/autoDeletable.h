// $Id: autoDeletable.h,v 1.8 2000/12/09 20:53:44 amoll Exp $

#ifndef BALL_CONCEPT_AUTODELETABLE_H
#define BALL_CONCEPT_AUTODELETABLE_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_COMMON_H
#	include <BALL/COMMON/debug.h>
#endif

#include <stdlib.h>

namespace BALL 
{

	/**	Auto-deletable concept.
			This class allows the distinction between objects that may be
			deleted automatically (because they are created dynamically on the heap)
			and instances that are static and should not be deleted automatically.\\
			Standard application is the class \Ref{Composite}. Composites may contain
			other composites (tree-like structure). If a composite is deleted, it
			has to decide whether all composites contained therein are to be deleted, too.
			If these composites are static objects, calling their destructor often is fatal!
			The same is true for objects contained in an array.
			Therefore, composite is derived from \Ref{AutoDeletable}.\\
			The AutoDeletable class determines whether it was instantiated via {\bf new}
			and then sets an internal flag to false. The mechanism to determine this is as follows:\\
			AutoDeletable has on overloaded new operator. When invoked, this operator
			allocates storage for an instance of AutoDeletable using the global new operator
			and remembers the address of in a private variable {\bf last\_ptr\_}.
			Each constructor of AutoDeletable checks whether its {\bf this} pointer is
			equal to the address stored in {\bf last\_ptr\_}. If both pointers are equal,
			the object has been created using the new operator and so it is safe to 
			delete it automatically. If the adresses do not match, the object is either
			part of an array or static and should not be deleted automatically.\\
			The state of each object may be changed after it is constructed by a call
			to \Ref{setAutoDeletable}. This might be useful to protect certain instances
			of objects, however usually this should not be neccessary.\\
			{\bf Definition:} \URL{BALL/CONCEPT/autoDeletable.h}
	*/
	class AutoDeletable
	{
		public:

		/**	@name	Constructors and Destructors
				The constructors of this class are protected.
		*/
		//@{

		/**	Destructor
		*/
		virtual ~AutoDeletable()
			throw();

		/**	{\bf new} operator.
				This operator allocates storage for the object and remembers its pointer.
				This pointer is {\bf static} and is evaluated by the constructors.
				As this operator is only invoked for the creation of single dynamic 
				objects, arrays and static objects can be identified.
		*/
		void* operator new(size_t size) 
			throw();
	
		/**	{\bf delete} operator.
				This operator frees the space allocated for an Autodeletable object. It is implemented
				solely to achieve a consistent interface and to avoid warnings issued by some compilers
				if operator new/delete do not appear in pairs.
		*/
		void operator delete(void* ptr) 
			throw();
	
		/**	Placement {\bf new} operator.
				This operator allocates storage for the object and remembers its pointer.
				This pointer is {\bf static} and is evaluated by the constructors.
				As this operator is only invoked for the creation of single dynamic 
				objects, arrays and static objects can be identified.
		*/
		void* operator new(size_t size, void* ptr)
			throw();
	
		/**	Placement {\bf delete} operator.
				This operator frees the space allocated for an Autodeletable object. It is implemented
				solely to achieve a consistent interface and to avoid warnings issued by some compilers
				if operator new/delete do not appear in pairs.
		*/
		void operator delete(void* ptr, void*)
			throw();

	
		//@}
	
		/**	@name	Mutators
		*/
		//@{
	
		/**	Mark the object as auto-deletable.
				Objects can be marked as deletable or not deletable by this method.
				Use this method to protect objects from automatic deletion.
				You should never set static objects to autodeletable, as 
				invoking {\bf delete} on a static object may result in a crash.
		*/
		void setAutoDeletable(bool enable)
			throw();

		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Query the objects status.
				Returns {\bf true} if the object should be automatically
				deleted if the objects it is contained in are deleted.
				Recursive destruction methods should honor this flag and 
				should not call the destructor for objects that return {\bf true}.
		*/
		bool isAutoDeletable() const
			throw();
		//@}


		protected:

		AutoDeletable()
			throw();

		AutoDeletable(const AutoDeletable& auto_deletable, bool deep = false)
			throw();


		private:

		bool 		enabled_;

		static 	void* last_ptr_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/autoDeletable.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_AUTODELETABLE_H
