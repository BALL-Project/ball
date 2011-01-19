#ifndef BALL_LINALG_BIDIRECTIONALFILTERITERATOR_H
#define BALL_LINALG_BIDIRECTIONALFILTERITERATOR_H

#ifndef BALL_LINALG_FORWARDFILTERITERATOR_H
# include <BALL/MATHS/LINALG/forwardFilterIterator.h>
#endif

namespace BALL 
{

	/**	@name	Bidirectional Iterators
	  	\ingroup  ConceptsIterators
	*/
	//@{

	/**	Constant Bidirectional Iterator.
	*/
	template <class Predicate, class IteratorBidirectional>
	class ConstBidirectionalFilterIterator
	  : public ConstForwardFilterIterator<Predicate, IteratorBidirectional>
	{
		public:

		/** @name Typedefs
		 */
		//@{

		
		///
		typedef typename IteratorBidirectional::container_type	Container;
		///
		typedef typename IteratorBidirectional::value_type	DataType;
		///
		typedef typename IteratorBidirectional::difference_type	Position;
		///
		typedef typename IteratorBidirectional::traits_type	Traits;
		///
		typedef typename IteratorBidirectional::value_type	value_type;
		///
		typedef typename IteratorBidirectional::difference_type	difference_type;
		///
		typedef	typename IteratorBidirectional::pointer	pointer;
		///
		typedef typename IteratorBidirectional::reference	reference;
		///
		typedef std::bidirectional_iterator_tag iterator_category;
		// convenience typedef
		typedef ConstForwardFilterIterator<Predicate, IteratorBidirectional> Base;		
		//@}

		/** @name Constructors and destructor.
		 */
		//@{

		///
		ConstBidirectionalFilterIterator() {}

		///
		ConstBidirectionalFilterIterator(Predicate p, IteratorBidirectional it)
		  :     ConstForwardFilterIterator<Predicate, IteratorBidirectional>(p,it)
		{
		}
	
		///
		ConstBidirectionalFilterIterator(const ConstBidirectionalFilterIterator& iterator)
			:	Base(iterator)
		{
		}

		///
		~ConstBidirectionalFilterIterator() {}
		//@}

		/// Set the internal iterator
		void setIterator(const IteratorBidirectional& iterator) 
		{ 
		  Base::iterator_ = iterator; 

		  if (isEnd())
		  {
		    Base::getTraits().backward();

		    while (!isBegin() && !Base::predicate_(Base::getTraits().getData()))
		      Base::getTraits().backward();
		    return;
		  }

		  while (!isEnd() && !Base::predicate_(Base::getTraits().getData()))
		    Base::getTraits().forward();
		}

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
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstBidirectionalFilterIterator& operator ++ ();

		/** Postfix increment operator
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstBidirectionalFilterIterator operator ++ (int);

		/** Decrement operator
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		ConstBidirectionalFilterIterator& operator -- ();

		/** Postfix decrement operator
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		ConstBidirectionalFilterIterator operator -- (int);

		/** Return an iterator pointingto the first item of the container
		 *  @exception Exception::Precondition
		 */
		static ConstBidirectionalFilterIterator begin(const Container& container);

		/** Return an iterator pointing to the position after the last element.
		 *  @exception Exception::Precondition
		 */
		static ConstBidirectionalFilterIterator end(const Container& container);

		/** Return an iterator pointing to the last element
		 *  @exception Exception::Precondition
		 */
		static ConstBidirectionalFilterIterator rbegin(const Container& container);

		/** Return an iterator pointing to the positon before the first element
		 *  @exception Exception::Precondition
		 */
		static ConstBidirectionalFilterIterator rend(const Container& container);
		//@}

		protected:

		/// Construct an iterator bound to a specific container
		ConstBidirectionalFilterIterator(const Container& container)
			:	Base(container)
		{
		}
	};
	//@}

	template <class Predicate, class IteratorBidirectional>
	void ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::toBegin()
	{
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().toBegin();
	}

	template <class Predicate, class IteratorBidirectional>
	void ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::toEnd()
	{
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().toEnd();
	}

	template <class Predicate, class IteratorBidirectional>
	void ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::toRBegin()
	{
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().toRBegin();
	}

	template <class Predicate, class IteratorBidirectional>
	void ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::toREnd()
	{	
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().toREnd();
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>& 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator ++ ()
	{
		  if (!Base::getTraits().isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		Base::getTraits().forward();
		  
		while (!isEnd() && !Base::predicate_(Base::getTraits().getData()))
		  Base::getTraits().forward();
		return *this;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator ++ (int)
	{
		  if (!Base::getTraits().isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		ConstBidirectionalFilterIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>& 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator -- ()
	{	
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().backward();
		  
		  while (!isBegin() && !Base::predicate_(Base::getTraits().getData()))
		    Base::getTraits().backward();
		return *this;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator -- (int)
	{	
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		ConstBidirectionalFilterIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::begin(const Container& container)
	{
		ConstBidirectionalFilterIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::end(const Container& container)
	{
		ConstBidirectionalFilterIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::rbegin(const Container& container)
	{
		ConstBidirectionalFilterIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::rend(const Container& container)
	{
		ConstBidirectionalFilterIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	/// Mutable bidirectional iterator
	template <class Predicate, class IteratorBidirectional>
	class BidirectionalFilterIterator
		: public ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>
	{
		public:

		/** @name Typedefs
		 */
		//@{
		
		///
		typedef typename IteratorBidirectional::container_type	Container;
		///
		typedef typename IteratorBidirectional::value_type	DataType;
		///
		typedef typename IteratorBidirectional::difference_type	Position;
		///
		typedef typename IteratorBidirectional::traits_type	Traits;
		///
		typedef typename IteratorBidirectional::value_type	value_type;
		///
		typedef typename IteratorBidirectional::difference_type	difference_type;
		///
		typedef	typename IteratorBidirectional::pointer	pointer;
		///
		typedef typename IteratorBidirectional::reference	reference;
		// convenience typedef
		typedef ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> Base;
		//@}

		/** @name Constructors and Destructor
		 */
		//@{

		/// Default constructor
		BidirectionalFilterIterator() {}

		///
		BidirectionalFilterIterator(Predicate p, IteratorBidirectional it)
		  : ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>(p,it)
		{
		}
	
		/// Copy constructor
		BidirectionalFilterIterator(const BidirectionalFilterIterator& iterator)
			:	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>(iterator)
		{
		}

		/// Destructor
		~BidirectionalFilterIterator() {}

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
		BidirectionalFilterIterator& operator ++ ();

		/** Postfix increment operator
		 *  @exception Exception::Precondition
		 */
		BidirectionalFilterIterator operator ++ (int);

		/** Decrement operator
		 *  @exception Exception::Precondition
		 */
		BidirectionalFilterIterator& operator -- ();

		/** Postfix decrement operator
		 *  @exception Exception::Precondition
		 */
		BidirectionalFilterIterator operator -- (int);

		/** Return an iterator pointing to the first item of the container
		 *  @exception Exception::Precondition
		 */
		static BidirectionalFilterIterator begin(const Container& container);

		/** Return an iterator pointing to the position after the last element
		 *  @exception Exception::Precondition
		 */
		static BidirectionalFilterIterator end(const Container& container);

		/** Return an iterator pointing to the last element.
		 *  @exception Exception::Precondition
		 */
		static BidirectionalFilterIterator rbegin(const Container& container);

		/** Return an iterator pointing to the positon before the first element
		 *  @exception Exception::Precondition
		 */
		static BidirectionalFilterIterator rend(const Container& container);
		//@}

		protected:

		/// Construct an iterator bound to a specific container
		BidirectionalFilterIterator(const Container& container);
	};


	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional>& 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator ++ ()
	{
		Base::operator ++ ();
		return *this;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator ++ (int)
	{
		BidirectionalFilterIterator iterator(*this);
		this->operator ++ ();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional>& 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator -- ()
	{
		Base::operator -- ();
		return *this;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator -- (int)
	{
		BidirectionalFilterIterator iterator(*this);
		this->operator -- ();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::begin(const Container& container)
	{
		BidirectionalFilterIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional>
  	BidirectionalFilterIterator<Predicate, IteratorBidirectional>::end(const Container& container)
	{
		BidirectionalFilterIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::rbegin(const Container& container)
	{
		BidirectionalFilterIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::rend(const Container& container)
	{
		BidirectionalFilterIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional>::BidirectionalFilterIterator(const Container& container)
		:	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>(container)
	{
	}


} // namespace BALL 

#endif // BALL_KERNEL_BIDIRECTIONALFILTERITERATOR_H
