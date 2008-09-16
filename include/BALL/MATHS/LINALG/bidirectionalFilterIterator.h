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
		ConstBidirectionalFilterIterator() throw() {}

		///
		ConstBidirectionalFilterIterator(Predicate p, IteratorBidirectional it) throw()
		  :     ConstForwardFilterIterator<Predicate, IteratorBidirectional>(p,it)
		{
		}
	
		///
		ConstBidirectionalFilterIterator(const ConstBidirectionalFilterIterator& iterator) throw()
			:	Base(iterator)
		{
		}

		///
		~ConstBidirectionalFilterIterator() throw() {}
		//@}

		/// Set the internal iterator
		void setIterator(const IteratorBidirectional& iterator) throw() 
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

		/// Move the iterator to the beginning of the container
		void toBegin() throw(Exception::SingularIterator);

		/// Check whether the iterator points to the item at the beginning of the container
		bool isBegin() const throw() { return Base::getTraits().isBegin(); }

		/// Move the iterator to the position after the last item of the container
		void toEnd() throw(Exception::SingularIterator);

		/// Check whether the iterator points to the position after the last item of the container.
		bool isEnd() const throw() { return Base::getTraits().isEnd(); }

		/// Move the iterator to the "reverse" beginning of the container
		void toRBegin() throw(Exception::SingularIterator);

		/// Test whether the iterator points to the "reverse" first element	of the container
		bool isRBegin() const throw() { return Base::getTraits().isRBegin(); }

		/// Move the iterator to the position before the first element
		void toREnd()	throw(Exception::SingularIterator);

		/// Test wheter the iterator points to the position before the first element
		bool isREnd() const throw() { return Base::getTraits().isREnd(); }

		/// Increment operator
		ConstBidirectionalFilterIterator& operator ++ () throw(Exception::InvalidIterator);

		/// Postfix increment operator
		ConstBidirectionalFilterIterator operator ++ (int) throw(Exception::InvalidIterator);

		/// Decrement operator
		ConstBidirectionalFilterIterator& operator -- () throw(Exception::SingularIterator);

		/// Postfix decrement operator
		ConstBidirectionalFilterIterator operator -- (int) throw(Exception::SingularIterator);

		/// Return an iterator pointingto the first item of the container
		static ConstBidirectionalFilterIterator begin(const Container& container) throw(Exception::Precondition);

		/// Return an iterator pointing to the position after the last element.
		static ConstBidirectionalFilterIterator end(const Container& container) throw(Exception::Precondition);

		/// Return an iterator pointing to the last element
		static ConstBidirectionalFilterIterator rbegin(const Container& container) throw(Exception::Precondition);

		/// Return an iterator pointing to the positon before the first element
		static ConstBidirectionalFilterIterator rend(const Container& container) throw(Exception::Precondition);
		//@}

		protected:

		/// Construct an iterator bound to a specific container
		ConstBidirectionalFilterIterator(const Container& container) throw()
			:	Base(container)
		{
		}
	};
	//@}

	template <class Predicate, class IteratorBidirectional>
	void ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::toBegin()
		throw(Exception::SingularIterator)
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
		throw(Exception::SingularIterator)
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
		throw(Exception::SingularIterator)
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
		throw(Exception::SingularIterator)
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
		throw(Exception::InvalidIterator)
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
		throw(Exception::InvalidIterator)
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
		throw(Exception::SingularIterator)
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
		throw(Exception::SingularIterator)
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
		throw(Exception::Precondition)
	{
		ConstBidirectionalFilterIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::end(const Container& container)
		throw(Exception::Precondition)
	{
		ConstBidirectionalFilterIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::rbegin(const Container& container)
		throw(Exception::Precondition)
	{
		ConstBidirectionalFilterIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>::rend(const Container& container)
		throw(Exception::Precondition)
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
		BidirectionalFilterIterator() throw() {}

		///
		BidirectionalFilterIterator(Predicate p, IteratorBidirectional it) throw()
		  :     ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>(p,it)
		{
		}
	
		/// Copy constructor
		BidirectionalFilterIterator(const BidirectionalFilterIterator& iterator)
			throw()
			:	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>(iterator)
		{
		}

		/// Destructor
		~BidirectionalFilterIterator() throw() {}

		//@}

		/** @name Iterator methods
		 */
		//@{

		/// Dereferentiation
		reference operator * () const throw() { return (reference)Base::getTraits().getData(); }

		/// Pointer dereferentiation
		pointer operator -> () const throw() { return (pointer)&Base::getTraits().getData(); }

		/// Increment operator
		BidirectionalFilterIterator& operator ++ ()	throw(Exception::Precondition);

		/// Postfix increment operator
		BidirectionalFilterIterator operator ++ (int) throw(Exception::Precondition);

		/// Decrement operator
		BidirectionalFilterIterator& operator -- ()	throw(Exception::Precondition);

		/// Postfix decrement operator
		BidirectionalFilterIterator operator -- (int) throw(Exception::Precondition);

		/// Return an iterator pointing to the first item of the container
		static BidirectionalFilterIterator begin(const Container& container)
			throw(Exception::Precondition);

		/// Return an iterator pointing to the position after the last element
		static BidirectionalFilterIterator end(const Container& container)
			throw(Exception::Precondition);

		/// Return an iterator pointing to the last element.
		static BidirectionalFilterIterator rbegin(const Container& container)
			throw(Exception::Precondition);

		/// Return an iterator pointing to the positon before the first element
		static BidirectionalFilterIterator rend(const Container& container)
			throw(Exception::Precondition);
		//@}

		protected:

		/// Construct an iterator bound to a specific container
		BidirectionalFilterIterator(const Container& container)	throw();
	};


	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional>& 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator ++ ()
		throw(Exception::Precondition)
	{
		Base::operator ++ ();
		return *this;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator ++ (int)
		throw(Exception::Precondition)
	{
		BidirectionalFilterIterator iterator(*this);
		this->operator ++ ();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional>& 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator -- ()
		throw(Exception::Precondition)
	{
		Base::operator -- ();
		return *this;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::operator -- (int)
		throw(Exception::Precondition)
	{
		BidirectionalFilterIterator iterator(*this);
		this->operator -- ();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::begin(const Container& container)
		throw(Exception::Precondition)
	{
		BidirectionalFilterIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional>
  	BidirectionalFilterIterator<Predicate, IteratorBidirectional>::end(const Container& container)
		throw(Exception::Precondition)
	{
		BidirectionalFilterIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::rbegin(const Container& container)
		throw(Exception::Precondition)
	{
		BidirectionalFilterIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional> 
		BidirectionalFilterIterator<Predicate, IteratorBidirectional>::rend(const Container& container)
		throw(Exception::Precondition)
	{
		BidirectionalFilterIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <class Predicate, class IteratorBidirectional>
	BidirectionalFilterIterator<Predicate, IteratorBidirectional>::BidirectionalFilterIterator(const Container& container)
		throw()
		:	ConstBidirectionalFilterIterator<Predicate, IteratorBidirectional>(container)
	{
	}


} // namespace BALL 

#endif // BALL_KERNEL_BIDIRECTIONALFILTERITERATOR_H
