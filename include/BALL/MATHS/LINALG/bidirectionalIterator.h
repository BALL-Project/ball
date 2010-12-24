#ifndef BALL_LINALG_BIDIRECTIONALITERATOR_H
#define BALL_LINALG_BIDIRECTIONALITERATOR_H

#ifndef BALL_LINALG_FORWARDITERATOR_H
# include <BALL/MATHS/LINALG/forwardIterator.h>
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
		ConstBidirectionalIterator() {}
	
		///
		ConstBidirectionalIterator(const ConstBidirectionalIterator& iterator)
			:	Base(iterator)
		{
		}

		///
		~ConstBidirectionalIterator() {}
		//@}

		/** @name Iterator methods
		 */
		//@{

		/** Move the iterator to the beginning of the container
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		void toBegin();

		/// Check whether the iterator points to the item at the beginning of the container
		bool isBegin() const { return Base::getTraits().isBegin(); }

		/** Move the iterator to the position after the last item of the container
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		void toEnd();

		/// Check whether the iterator points to the position after the last item of the container.
		bool isEnd() const { return Base::getTraits().isEnd(); }

		/** Move the iterator to the "reverse" beginning of the container
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		void toRBegin();

		/// Test whether the iterator points to the "reverse" first element	of the container
		bool isRBegin() const { return Base::getTraits().isRBegin(); }

		/** Move the iterator to the position before the first element
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		void toREnd();

		/// Test wheter the iterator points to the position before the first element
		bool isREnd() const { return Base::getTraits().isREnd(); }

		/** Increment operator
		 *  @exception Exception::InvalidIterator if the interator is invalid
		 */
		ConstBidirectionalIterator& operator ++ ();

		/** Postfix increment operator
		 *  @exception Exception::InvalidIterator if the interator is invalid
		 */
		ConstBidirectionalIterator operator ++ (int);

		/** Decrement operator
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		ConstBidirectionalIterator& operator -- ();

		/** Postfix decrement operator
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		ConstBidirectionalIterator operator -- (int);

		/** Return an iterator pointingto the first item of the container
		 *  @exception Exception::Precondition
		 */
		static ConstBidirectionalIterator begin(const Container& container);

		/** Return an iterator pointing to the position after the last element.
		 *  @exception Exception::Precondition
		 */
		static ConstBidirectionalIterator end(const Container& container);

		/** Return an iterator pointing to the last element
		 *  @exception Exception::Precondition
		 */
		static ConstBidirectionalIterator rbegin(const Container& container);

		/** Return an iterator pointing to the positon before the first element
		 *  @exception Exception::Precondition
		 */
		static ConstBidirectionalIterator rend(const Container& container);
		//@}

		protected:

		/// Construct an iterator bound to a specific container
		ConstBidirectionalIterator(const Container& container)
			:	Base(container)
		{
		}
	};
	//@}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toBegin()
	{
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toEnd()
	{
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin()
	{
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toREnd()
	{	
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
	{
		  if (!Base::getTraits().isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		Base::getTraits().forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
	{
		  if (!Base::getTraits().isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		ConstBidirectionalIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
	{	
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().backward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
	{	
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		ConstBidirectionalIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container& container)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
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
		BidirectionalIterator() {}
	
		/// Copy constructor
		BidirectionalIterator(const BidirectionalIterator& iterator)
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/// Destructor
		~BidirectionalIterator() {}

		//@}

		/** @name Iterator methods
		 */
		//@{

		/// Dereferentiation
		reference operator * () const { return (reference)Base::getTraits().getData(); }

		/// Pointer dereferentiation
		pointer operator -> () const { return (pointer)&Base::getTraits().getData(); }

		/** Increment operator
		 *  @exception Exception::Precondition
		 */
		BidirectionalIterator& operator ++ ();

		/** Postfix increment operator
		 *  @exception Exception::Precondition
		 */
		BidirectionalIterator operator ++ (int);

		/** Decrement operator
		 *  @exception Exception::Precondition
		 */
		BidirectionalIterator& operator -- ();

		/** Postfix decrement operator
		 *  @exception Exception::Precondition
		 */
		BidirectionalIterator operator -- (int);

		/** Return an iterator pointing to the first item of the container
		 *  @exception Exception::Precondition
		 */
		static BidirectionalIterator begin(const Container& container);

		/** Return an iterator pointing to the position after the last element
		 *  @exception Exception::Precondition
		 */
		static BidirectionalIterator end(const Container& container);

		/** Return an iterator pointing to the last element.
		 *  @exception Exception::Precondition
		 */
		static BidirectionalIterator rbegin(const Container& container);

		/** Return an iterator pointing to the positon before the first element
		 *  @exception Exception::Precondition
		 */
		static BidirectionalIterator rend(const Container& container);
		//@}

		protected:

		/// Construct an iterator bound to a specific container
		BidirectionalIterator(const Container& container);
	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>& 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
	{
		Base::operator ++ ();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
	{
		BidirectionalIterator iterator(*this);
		this->operator ++ ();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>& 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
	{
		Base::operator -- ();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
	{
		BidirectionalIterator iterator(*this);
		this->operator -- ();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container& container)
	{
		BidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>
  	BidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
	{
		BidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
	{
		BidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container& container)
	{
		BidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>::BidirectionalIterator(const Container& container)
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(container)
	{
	}


} // namespace BALL 

#endif // BALL_KERNEL_BIDIRECTIONALITERATOR_H
