// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#define BALL_CONCEPT_BIDIRECTIONALITERATOR_H

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
#endif

namespace BALL 
{

	/**	@name	Bidirectional Iterators
	  	\ingroup  ConceptsIterators
	*/
	//@{

	/**	Constant Bidirectional Iterator.
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstBidirectionalIterator
		: public ConstForwardIterator<Container, DataType, Position, Traits>
	{
		public:

		/** @name Typedefs
		 */
		//@{

		///
		typedef std::bidirectional_iterator_tag iterator_category;
		// convenience typedef
		typedef ConstForwardIterator<Container, DataType, Position, Traits> Base;		
		//@}

		/** @name Constructors and destructor.
		 */
		//@{

		///
		BALL_INLINE ConstBidirectionalIterator()  {}
	
		///
		BALL_INLINE ConstBidirectionalIterator(const ConstBidirectionalIterator& iterator) 
			:	Base(iterator)
		{
		}

		///
		BALL_INLINE ~ConstBidirectionalIterator()  {}
		//@}

		/** @name Iterator methods
		 */
		//@{

		/** Move the iterator to the beginning of the container
		 *  \throws Exception::Precondition
		 */
		BALL_INLINE void toBegin();

		/// Check whether the iterator points to the item at the beginning of the container
		BALL_INLINE bool isBegin() const  { return Base::getTraits().isBegin(); }

		/** Move the iterator to the position after the last item of the container
		 *  \throws Exception::Precondition
		 */
		void toEnd();

		/// Check whether the iterator points to the position after the last item of the container.
		BALL_INLINE bool isEnd() const  { return Base::getTraits().isEnd(); }

		/** Move the iterator to the "reverse" beginning of the container
		 *  \throws Exception::Precondition
		 */
		void toRBegin();

		/// Test whether the iterator points to the "reverse" first element	of the container
		BALL_INLINE bool isRBegin() const  { return Base::getTraits().isRBegin(); }

		/** Move the iterator to the position before the first element
		 *  \throws Exception::Precondition
		 */
		void toREnd();

		/// Test wheter the iterator points to the position before the first element
		BALL_INLINE bool isREnd() const  { return Base::getTraits().isREnd(); }

		/** Increment operator
		 *  \throws Exception::Precondition
		 */
		BALL_INLINE ConstBidirectionalIterator& operator ++ ();

		/** Postfix increment operator
		 *  \throws Exception::Precondition
		 */
		BALL_INLINE ConstBidirectionalIterator operator ++ (int);

		/** Decrement operator
		 *  \throws Exception::Precondition
		 */
		BALL_INLINE ConstBidirectionalIterator& operator -- ();

		/** Postfix decrement operator
		 *  \throws Exception::Precondition
		 */
		BALL_INLINE ConstBidirectionalIterator operator -- (int);

		/** Return an iterator pointing to the first item of the container
		 *  \throws Exception::Precondition
		 */
		static ConstBidirectionalIterator begin(const Container& container);

		/** Return an iterator pointing to the position after the last element.
		 *  \throws Exception::Precondition
		 */
		static ConstBidirectionalIterator end(const Container& container);

		/** Return an iterator pointing to the last element
		 *  \throws Exception::Precondition
		 */
		static ConstBidirectionalIterator rbegin(const Container& container);

		/** Return an iterator pointing to the positon before the first element
		 *  \throws Exception::Precondition
		 */
		static ConstBidirectionalIterator rend(const Container& container);
		//@}

		protected:

		/// Construct an iterator bound to a specific container
		BALL_INLINE ConstBidirectionalIterator(const Container& container) 
			:	Base(container)
		{
		}
	};
	//@}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toBegin()
	{
		BALL_PRECONDITION_EXCEPTION(!Base::getTraits().isSingular(), "cannot set unbound iterator to begin")
		Base::getTraits().toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toEnd()
	{
		BALL_PRECONDITION_EXCEPTION(!Base::getTraits().isSingular(), "cannot set unbound iterator to end")
		Base::getTraits().toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin()
	{
		BALL_PRECONDITION_EXCEPTION(!Base::getTraits().isSingular(), "cannot set unbound iterator to reverse begin")
		Base::getTraits().toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toREnd()
	{	
		BALL_PRECONDITION_EXCEPTION(!Base::getTraits().isSingular(), "cannot set unbound iterator to reverse end")
		Base::getTraits().toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
	{
		BALL_PRECONDITION_EXCEPTION(Base::getTraits().isValid(), "cannot increment an invalid iterator")
		Base::getTraits().forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
	{
		BALL_PRECONDITION_EXCEPTION(Base::getTraits().isValid(), "cannot increment an invalid iterator")
		ConstBidirectionalIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
	{
		BALL_PRECONDITION_EXCEPTION(!Base::getTraits().isSingular(), "cannot decrement unbound iterator")
		Base::getTraits().backward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
	{
		BALL_PRECONDITION_EXCEPTION(!Base::getTraits().isSingular(), "cannot decrement an unbound iterator")
		ConstBidirectionalIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container& container)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container& container)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	/// Mutable bidirectional iterator
	template <typename Container, typename DataType, typename Position, typename Traits>
	class BidirectionalIterator
		: public ConstBidirectionalIterator<Container, DataType, Position, Traits>
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
		typedef ConstBidirectionalIterator<Container, DataType, Position, Traits> Base;
		//@}

		/** @name Constructors and Destructor
		 */
		//@{

		/// Default constructor
		BALL_INLINE BidirectionalIterator()  {}
	
		/// Copy constructor
		BALL_INLINE BidirectionalIterator(const BidirectionalIterator& iterator)
			
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/// Destructor
		BALL_INLINE ~BidirectionalIterator()  {}

		//@}

		/** @name Iterator methods
		 */
		//@{

		/// Dereferentiation
		BALL_INLINE reference operator * () const  { return (reference)Base::getTraits().getData(); }

		/// Pointer dereferentiation
		BALL_INLINE pointer operator -> () const  { return (pointer)&Base::getTraits().getData(); }

		/** Increment operator
		 *  \throws Exception::Precondition
		 */
		BALL_INLINE BidirectionalIterator& operator ++ ();

		/** Postfix increment operator
		 *  \throws Exception::Precondition
		 */
		BALL_INLINE BidirectionalIterator operator ++ (int);

		/** Decrement operator
		 *  \throws Exception::Precondition
		 */
		BALL_INLINE BidirectionalIterator& operator -- ();

		/** Postfix decrement operator
		 *  \throws Exception::Precondition
		 */
		BALL_INLINE BidirectionalIterator operator -- (int);

		/** Return an iterator pointing to the first item of the container
		 *  \throws Exception::Precondition
		 */
		static BidirectionalIterator begin(const Container& container);

		/** Return an iterator pointing to the position after the last element
		 *  \throws Exception::Precondition
		 */
		static BidirectionalIterator end(const Container& container);

		/** Return an iterator pointing to the last element.
		 *  \throws Exception::Precondition
		 */
		static BidirectionalIterator rbegin(const Container& container);

		/** Return an iterator pointing to the positon before the first element
		 *  \throws Exception::Precondition
		 */
		static BidirectionalIterator rend(const Container& container);
		//@}

		protected:

		/// Construct an iterator bound to a specific container
		BALL_INLINE BidirectionalIterator(const Container& container)	;
	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits>& 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
	{
		Base::operator ++ ();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
	{
		BidirectionalIterator iterator(*this);
		this->operator ++ ();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits>& 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
	{
		Base::operator -- ();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
	{
		BidirectionalIterator iterator(*this);
		this->operator -- ();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container& container)
	{
		BidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits>
  	BidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
	{
		BidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
	{
		BidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container& container)
	{
		BidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits>::BidirectionalIterator(const Container& container)
		
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(container)
	{
	}


} // namespace BALL 

#endif // BALL_CONCEPT_BIDIRECTIONALITERATOR_H
