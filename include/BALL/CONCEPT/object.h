// $Id: object.h,v 1.12 2001/05/28 11:51:15 anker Exp $ 

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
				@return	{\bf true}, if object and this are the same, false ow.
		*/
		bool operator == (const Object& object) const
			throw();

		/** Inequality operator. This operator checks Object features only, i. e.
				the handle.
				@param	object an Object
				@return	{\bf true}, if object and this are not the same object,
								false ow.
		*/
		bool operator != (const Object& object) const
			throw();

		/** Less than operator. This method checks whether the handle of
				{\bf this} object is less than the handle of {\tt object}
				@param	object an Object
				@return	{bf true}, if the handle of {\bf this} is less than the handle
								of {\tt object}
		*/
		bool operator < (const Object& object) const
			throw();

		/** Less than or equal operator. This method checks whether the handle of
				{\bf this} object is less than or equal to the handle of {\tt object}
				@param	object an Object
				@return	{bf true}, if the handle of {\bf this} is less than or equal 
								to the handle of {\tt object}
		*/
		bool operator <= (const Object& object) const
			throw();

		/** Greater than or equal operator. This method checks whether the
				handle of {\bf this} object is greater than or equal to the handle
				of {\tt object}
				@param	object an Object
				@return	{bf true}, if the handle of {\bf this} is greater than or 
								equal to the handle of {\tt object}
		*/
		bool operator >= (const Object& object) const
			throw();

		/** Greater than operator. This method checks whether the handle of
				{\bf this} object is greater than the handle of {\tt object}
				@param	object an Object
				@return	{bf true}, if the handle of {\bf this} is greater than the 
								handle of {\tt object}
		*/
		bool operator > (const Object& object) const
			throw();

		/** Compare the handle of {\bf this} object with the handle of
				another.
				@param	{\tt object} an Object
				@return 0, if both objects are equal, -1 if {\tt object}'s handle
								is greater and 1 if {\bf this}'s handle is greater.
		*/
		int compare(const Object& object) const
			throw();
		//@}

		/**	@name	I/O operations */
		//@{

		/** Read an object from a stream. {\bf Note:} This method is not yet
				implemented.
				@param istream a stream from which the object will be read
		*/
		virtual void read(::std::istream& s)
			throw();

		/** Write an object to a stream. {\bf Note:} This method is not yet
				implemented.
				@param ostream a stream to which the object will be written
		*/
		virtual void write(::std::ostream& s) const
			throw();

		// BAUSTELLE:
		// Shouldn't friends be avoided?

		///
		friend ::std::istream& operator >> (::std::istream& s, Object& object)
			throw();

		///
		friend ::std::ostream& operator << (::std::ostream& s, const Object& object)
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

#		ifdef BALL_SUPPORT_OBJECT_MANAGER

		/** @name Methods related to the BALL object manager
		*/
		//@{

		/** Return the previous object in the list.
				@return a pointer to the previous object
		*/
		Object* getPrevious()
			throw();
		
		/** Return the previous object in the list.
				@return a const pointer to the previous object
		*/
		const Object* getPrevious() const
			throw();
		
		/** Return the next object in the list.
				@return a pointer to the next object
		*/
		Object* getNext()
			throw();
		
		/** Return the next object in the list.
				@return a const pointer to the next object
		*/
		const Object* getNext() const
			throw();
		//@}

#		endif // BALL_SUPPORT_OBJECT_MANAGER
		


		private:

		//_ The handle of this instance
		Handle				handle_;

		//_ The global handle
		static Handle global_handle_;

#		ifdef BALL_SUPPORT_OBJECT_MANAGER
	
		//_ A pointer to the previous object in the managed list
		Object* previous_;
		//
		//_ A pointer to the next object in the managed list
		Object* next_;

#		endif // BALL_SUPPORT_OBJECT_MANAGER
	};


#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/object.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_OBJECT_H
