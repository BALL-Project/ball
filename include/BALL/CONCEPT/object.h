// $Id: object.h,v 1.11 2001/01/22 20:50:24 amoll Exp $ 

#ifndef BALL_CONCEPT_OBJECT_H
#define BALL_CONCEPT_OBJECT_H

#ifndef BALL_CONCEPT_AUTODELETABLE_H
#	include <BALL/CONCEPT/autoDeletable.h>
#endif

#ifndef BALL_COMMON_RTTI_H
#	include <BALL/COMMON/rtti.h>
#endif

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/bidirectionalIterator.h>
#endif

#ifndef BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/reverseBidirectionalIterator.h>
#endif

namespace BALL 
{

	/**
	*/
	class Object
		: public AutoDeletable
	{
#		ifdef BALL_SUPPORT_OBJECT_MANAGER

			friend class ObjectManager;

#		endif // BALL_SUPPORT_OBJECT_MANAGER
		
		public:

		BALL_CREATE_DEEP(Object)


		/**	@name	Construction and Destruction */	
		//@{

		/// Default constructor.
		Object()
			throw();
 
		/// Copy constructor
		Object(const Object& object, bool deep = true)
			throw();
 
		/// Destructor
		virtual ~Object()
			throw();

		///
		virtual void clear()
			throw();

		///
		virtual void destroy()
			throw();

		//@}
 
 		/**	@name	Assignment */
		//@{
	
		/** Assignment operator
				This method does not assign anything!
				Implemented just for the OCI.
		*/
		const Object& operator = (const Object& /* object */)
			throw();
		
		//@}

		/**	@name	Handle Management */
		//@{
		///
		Handle getHandle() const
			throw();
	
		///
		static Handle getNextHandle()
			throw();
	
		///
		static Handle getNewHandle()
			throw();
		//@}
	
		/**	@name Comparison operators */
		//@{
		///
		bool operator == (const Object& object) const
			throw();

		///
		bool operator != (const Object& object) const
			throw();

		///
		bool operator < (const Object& object) const
			throw();

		///
		bool operator <= (const Object& object) const
			throw();

		///
		bool operator >= (const Object& object) const
			throw();

		///
		bool operator > (const Object& object) const
			throw();

		///
		int compare(const Object& object) const
			throw();
		//@}

		/**	@name	I/O operations */
		//@{

		///
		virtual void read(::std::istream& s)
			throw();

		///
		virtual void write(::std::ostream& s) const
			throw();

		///
		friend ::std::istream& operator >> (::std::istream& s, Object& object)
			throw();

		///
		friend ::std::ostream& operator << (::std::ostream& s, const Object& object)
			throw();
		//@}
	
		
		/**	@name Debugging and Diagnostics */
		//@{
		///
		virtual bool isValid() const
			throw();

		///
		virtual void dump(::std::ostream& s = std::cout, Size depth = 0) const
			throw();
		//@}

#		ifdef BALL_SUPPORT_OBJECT_MANAGER

		Object* getPrevious()
			throw();
		
		const Object* getPrevious() const
			throw();
		
		Object* getNext()
			throw();
		
		const Object* getNext() const
			throw();

#		endif // BALL_SUPPORT_OBJECT_MANAGER
		


		private:

		Handle				handle_;
		static Handle global_handle_;

#		ifdef BALL_SUPPORT_OBJECT_MANAGER
	
		Object* previous_;
		Object* next_;

#		endif // BALL_SUPPORT_OBJECT_MANAGER
	};


#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/object.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_OBJECT_H
