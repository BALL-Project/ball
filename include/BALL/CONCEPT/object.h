// $Id: object.h,v 1.15 2001/07/25 11:24:34 oliver Exp $ 

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

	/** Generic object class. This class provides unique handles for every
			object that is an instance of a derived class of object. Together
			with the BALL object manager this class also provides a double linked
			list of all objects. \\
			{\bf Definition:} \URL{BALL/CONCEPT/object.h}
	*/
	class Object
		: public AutoDeletable
	{
		public:

		BALL_CREATE(Object)

		/**	@name	Construction and Destruction */	
		//@{

		/// Default constructor.
		Object()
			throw();
 
		/// Copy constructor
		Object(const Object& object)
			throw();
 
		/// Destructor
		virtual ~Object()
			throw();

		/// Clear function of the OCI, doing nothing here.
		virtual void clear()
			throw();

		//@}
 
 		/**	@name	Assignment */
		//@{
	
		/** Assignment operator.
				This method does not assign anything!
				Implemented just for the OCI.
				@param object an object
				@return this object
		*/
		const Object& operator = (const Object& /* object */)
			throw();
		
		//@}

		/**	@name	Handle Management */
		//@{

		/** Return the global handle of this object.
				@return the handle of this object
		*/
		Handle getHandle() const
			throw();
	
		/** Return the next available handle without changing the global handle
				counter.
				@return the next available handle
		*/
		static Handle getNextHandle()
			throw();
	
		/** Return the next available handle and increase the global handle
				counter.
				@return the next available handle
		*/
		static Handle getNewHandle()
			throw();
		//@}
	
		/**	@name Comparison operators */
		//@{
		/** Equality operator. This operator checks Object features only, i. e.
				the handle.
				@param 	object an Object
				@return	true, if object and this are the same, false ow.
		*/
		bool operator == (const Object& object) const
			throw();

		/** Inequality operator. This operator checks Object features only, i. e.
				the handle.
				@param	object an Object
				@return	true, if object and this are not the same object,
								false ow.
		*/
		bool operator != (const Object& object) const
			throw();

		/** Less than operator. This method checks whether the handle of
				{\bf this} object is less than the handle of {\tt object}
				@param	object an Object
				@return	true, if the handle of {\bf this} is less than the handle
								of {\tt object}
		*/
		bool operator < (const Object& object) const
			throw();

		/** Less than or equal operator. This method checks whether the handle of
				{\bf this} object is less than or equal to the handle of {\tt object}
				@param	object an Object
				@return	true, if the handle of {\bf this} is less than or equal 
								to the handle of {\tt object}
		*/
		bool operator <= (const Object& object) const
			throw();

		/** Greater than or equal operator. This method checks whether the
				handle of {\bf this} object is greater than or equal to the handle
				of {\tt object}
				@param	object an Object
				@return	true, if the handle of {\bf this} is greater than or 
								equal to the handle of {\tt object}
		*/
		bool operator >= (const Object& object) const
			throw();

		/** Greater than operator. This method checks whether the handle of
				{\bf this} object is greater than the handle of {\tt object}
				@param	object an Object
				@return	true, if the handle of {\bf this} is greater than the 
								handle of {\tt object}
		*/
		bool operator > (const Object& object) const
			throw();

		/** Compare the handle of {\bf this} object with the handle of
				another.
				@param	object an Object
				@return 0, if both objects are equal, -1 if {\tt object}'s handle
								is greater and 1 if {\bf this}'s handle is greater.
		*/
		int compare(const Object& object) const
			throw();
		//@}
		/**	@name Debugging and Diagnostics 
		*/
		//@{

		///
		virtual bool isValid() const
			throw();

		///
		virtual void dump(::std::ostream& s = std::cout, Size depth = 0) const
			throw();
		//@}

		private:

		//_ The handle of this instance
		Handle				handle_;

		//_ The global handle
		static Handle global_handle_;
	};


#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/object.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_OBJECT_H
