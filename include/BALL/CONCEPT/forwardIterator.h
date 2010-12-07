// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#define BALL_CONCEPT_FORWARDITERATOR_H

#ifndef BALL_CONCEPT_BASEITERATOR_H
#	include <BALL/CONCEPT/baseIterator.h>
#endif

namespace BALL 
{

	/**	@name	Generic forward iterator classes
			
	 	 \ingroup  ConceptsIterators
	*/
	//@{

	/**	Generic non-mutable forward itterator
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstForwardIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:
	
		/** @name Typedefs
		 */
		//@{

		///
		typedef std::forward_iterator_tag iterator_category;
    // convenience typedef
    typedef BaseIterator<Container, DataType, Position, Traits> Base;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		/// Default constructor
		BALL_INLINE ConstForwardIterator()  {}
	
		/// Copy constructor
		BALL_INLINE ConstForwardIterator(const ConstForwardIterator& iterator) 
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
			
		///
		BALL_INLINE ~ConstForwardIterator()  {}

		//@}

		/** @name Assignment 
		*/
		//@{
		///
		BALL_INLINE ConstForwardIterator& operator = (const ConstForwardIterator& iterator) 
		{
			BaseIterator<Container, DataType, Position, Traits>::operator = (iterator);
			return *this;
		}

		///
		BALL_INLINE void swap(ConstForwardIterator& iterator)  { std::swap(ConstForwardIterator<Container, DataType, Position, Traits>::getTraits(), iterator.getTraits()); }
		//@}

		/** @name Iterator methods
		*/
		//@{

		/// Move the iterator to the item at the beginning of the container
		BALL_INLINE void toBegin() throw(Exception::Precondition);

		/// Check whether the iterator points to the first item of the container
		BALL_INLINE bool isBegin() const ;

		/// Move the iterator behind the last item of the container
		BALL_INLINE void toEnd() throw(Exception::Precondition);

		/// Check whether the iterator points behind the last item of the container
		BALL_INLINE bool isEnd() const ;

		/// Return an iterator pointing to the beginning of the container
		static ConstForwardIterator begin(const Container& container)
			throw(Exception::Precondition);

		/// Return an iterator pointing at the end of the container
		static ConstForwardIterator end(const Container& container) 
			throw(Exception::Precondition);

    /// Increment operator
    BALL_INLINE ConstForwardIterator& operator ++ () throw(Exception::Precondition)
    {
      BALL_PRECONDITION_EXCEPTION(Base::isValid(), "cannot increment invalid iterator")
      Base::getTraits().forward();
      return *this;
		}

    /// Postfix increment operator
    BALL_INLINE ConstForwardIterator operator ++ (int) throw(Exception::Precondition)
    {
      BALL_PRECONDITION_EXCEPTION(Base::isValid(), "cannot increment invalid iterator")
      ConstForwardIterator tmp(*this);
      ++(*this);
      return tmp;
		}
 
		//@}

		protected:

		///
		BALL_INLINE ConstForwardIterator(const Container& container) 
			:	BaseIterator<Container, DataType, Position, Traits>(container)
		{
		}
	};
	//@}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstForwardIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::Precondition)
	{
    BALL_PRECONDITION_EXCEPTION(!Base::isSingular(),	"cannot move singular iterator to begin")
		Base::getTraits().toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstForwardIterator<Container, DataType, Position, Traits> 
		ConstForwardIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::Precondition)
	{
		ConstForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstForwardIterator<Container, DataType, Position, Traits>::isBegin() const
		
	{
		if (Base::getTraits().isSingular())
		{
			return false;
		}
		return Base::getTraits().isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstForwardIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::Precondition)
	{
    BALL_PRECONDITION_EXCEPTION(!Base::isSingular(), "cannot move singular iterator to end")
		Base::getTraits().toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstForwardIterator<Container, DataType, Position, Traits> 
		ConstForwardIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::Precondition)
	{
		ConstForwardIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstForwardIterator<Container, DataType, Position, Traits>::isEnd() const
		
	{
		if (Base::isSingular())
		{
			return false;
		}
		return Base::getTraits().isEnd();
	}
			
	/**	Generic mutable forward iterator class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ForwardIterator
		: public ConstForwardIterator<Container, DataType, Position, Traits>
	{
		public:

		/** @name Typedefs
		 */
		//@{

		///
		typedef DataType& reference;
		///
		typedef DataType* pointer;
		// convenience typedef		
		typedef ConstForwardIterator<Container, DataType, Position, Traits> Base;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		///
		BALL_INLINE ForwardIterator()  {}
	
		///
		BALL_INLINE ForwardIterator(const ForwardIterator& iterator) 
			:	ConstForwardIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		///
		BALL_INLINE ~ForwardIterator()  {}			
		//@}

		/** @name Assignemnt
		*/
		//@{
		///
		BALL_INLINE ForwardIterator& operator = (const ForwardIterator& iterator) 
		{
			ConstForwardIterator<Container, DataType, Position, Traits>::operator = (iterator);
			return *this;
		}

		///
		BALL_INLINE void swap(ForwardIterator& iterator)  { std::swap(Base::getTraits(), iterator.getTraits()); }
		//@}

		/** @name Iterator methods
		*/
		//@{

		/// Return an iterator pointing to the beginning of the container
		static ForwardIterator begin(const Container& container) throw(Exception::Precondition);

		/// Return an iterator pointing at the end of the container
		static ForwardIterator end(const Container& container) throw(Exception::Precondition);

		///
		BALL_INLINE reference operator * () const  
		{ 
			return (reference)Base::getTraits().getData(); 
		}

		///
		BALL_INLINE pointer operator -> () const  
		{ 
			return (pointer)&Base::getTraits().getData(); 
		}

    /// Increment operator
    BALL_INLINE ForwardIterator& operator ++ () throw(Exception::Precondition)
    {
			BALL_PRECONDITION_EXCEPTION(Base::isValid(), "cannot increment invalid iterator")
			Base::getTraits().forward();
      return *this;
		}

    /// Postfix increment operator
    BALL_INLINE ForwardIterator operator ++ (int) throw(Exception::Precondition)
    {
			BALL_PRECONDITION_EXCEPTION(Base::isValid(), "cannot increment invalid iterator")
      ForwardIterator tmp(*this);
      ++(*this);
      return tmp;
		}

		//@}

		protected:

		///
		BALL_INLINE ForwardIterator(const Container& container) 	
			:	Base(container)
		{
		}
	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ForwardIterator<Container, DataType, Position, Traits> 
		ForwardIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::Precondition)
	{
		ForwardIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ForwardIterator<Container, DataType, Position, Traits> 
		ForwardIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::Precondition)
	{
		ForwardIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}


} // namespace BALL

#endif // BALL_CONCEPT_FORWARDITERATOR_H
