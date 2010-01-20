#ifndef BALL_LINALG_FORWARDFILTERITERATOR_H
#define BALL_LINALG_FORWARDFILTERITERATOR_H

#ifndef BALL_LINALG_BASEFILTERITERATOR_H
#	include <BALL/MATHS/LINALG/baseFilterIterator.h>
#endif

namespace BALL 
{

	/**	@name	Generic forward filter iterator classes
	*/
	//@{

	/**	Generic non-mutable forward iterator
	*/
	template <class Predicate, class IteratorForward>
	class ConstForwardFilterIterator
		: public BaseFilterIterator<Predicate, IteratorForward>
	{
		public:
	
		/** @name Typedefs
		 */
		//@{

		
		///
		typedef typename IteratorForward::container_type	Container;
		///
		typedef typename IteratorForward::value_type	DataType;
		///
		typedef typename IteratorForward::difference_type	Position;
		///
		typedef typename IteratorForward::traits_type	Traits;
		///
		typedef typename IteratorForward::value_type	value_type;
		///
		typedef typename IteratorForward::difference_type	difference_type;
		///
		typedef	typename IteratorForward::pointer	pointer;
		///
		typedef typename IteratorForward::reference	reference;
		///
		typedef std::forward_iterator_tag iterator_category;
		///
		typedef BaseFilterIterator<Predicate, IteratorForward> Base;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		/// Default constructor
		ConstForwardFilterIterator() throw() {}

		///
		ConstForwardFilterIterator(Predicate p, IteratorForward it) throw()
		  :     BaseFilterIterator<Predicate, IteratorForward>(p,it)
		{
		  while (!isEnd() && !Base::predicate_(Base::getTraits().getData()))
		    Base::getTraits().forward();
		}
	
		/// Copy constructor
		ConstForwardFilterIterator(const ConstForwardFilterIterator& iterator) throw()
		  :	BaseFilterIterator<Predicate, IteratorForward>(iterator)
		{
		}
			
		///
		~ConstForwardFilterIterator() throw() {}

		//@}

		/** @name Assignment 
		*/
		//@{
		///
		ConstForwardFilterIterator& operator = (const ConstForwardFilterIterator& iterator) throw()
		{
		        Base::operator = (iterator);
			return *this;
		}

		///
		void swap(ConstForwardFilterIterator& iterator) throw() 
		{ 
		  std::swap(Base::predicate_, iterator.predicate_); 
		  std::swap(Base::iterator_, iterator.iterator_);
		}
		//@}

		/// Set the internal iterator
		void setIterator(const IteratorForward& iterator) throw() 
		{ 
		  Base::iterator_ = iterator; 

		  while (!isEnd() && !Base::predicate_(Base::getTraits().getData()))
		    Base::getTraits().forward();
		}

		/// Get a constant reference to the internal iterator.
		const IteratorForward& getIterator() const throw() { return Base::iterator_; }

		/// Get a constant reference to the internal iterator.
		IteratorForward& getIterator() throw() { return Base::iterator_; }

		/** @name Iterator methods
		*/
		//@{

		/// Move the iterator to the item at the beginning of the container
		void toBegin() throw(Exception::SingularIterator);

		/// Check whether the iterator points to the first item of the container
		bool isBegin() const throw();

		/// Move the iterator behind the last item of the container
		void toEnd() throw(Exception::SingularIterator);

		/// Check whether the iterator points behind the last item of the container
		bool isEnd() const throw();

		/// Return an iterator pointing to the beginning of the container
		static ConstForwardFilterIterator begin(const Container& container)
			throw(Exception::Precondition);

		/// Return an iterator pointing at the end of the container
		static ConstForwardFilterIterator end(const Container& container) 
			throw(Exception::Precondition);

		/// Increment operator
		ConstForwardFilterIterator& operator ++ () throw(Exception::InvalidIterator)
		{
		  if (!Base::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		  Base::getTraits().forward();
		  
		  while (!isEnd() && !Base::predicate_(Base::getTraits().getData()))
		    Base::getTraits().forward();
		  return *this;
		}
		
		/// Postfix increment operator
		ConstForwardFilterIterator operator ++ (int) throw(Exception::InvalidIterator)
		{
		  if (!IteratorForward::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		  ConstForwardFilterIterator tmp(*this);
		  ++(*this);
		  return tmp;
		}
		
		//@}
		
	protected:
		
		///
		template <typename Container>
		ConstForwardFilterIterator(const Container& container) throw()
		  :	BaseFilterIterator<Predicate, IteratorForward>(container)
		{
		}
		
	};
  //@}

        template <class Predicate, class IteratorForward>
	void ConstForwardFilterIterator<Predicate, IteratorForward>::toBegin()
		throw(Exception::SingularIterator)
	{
	  if (IteratorForward::isSingular())
	  {
	    Exception::SingularIterator e;
	    throw(e);
	  }
		Base::getTraits().toBegin();
	}

	template <class Predicate, class IteratorForward>
	ConstForwardFilterIterator<Predicate, IteratorForward> 
		ConstForwardFilterIterator<Predicate, IteratorForward>::begin(const Container& container)
		throw(Exception::Precondition)
	{
		ConstForwardFilterIterator<Predicate, IteratorForward> iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <class Predicate, class IteratorForward>
	bool ConstForwardFilterIterator<Predicate, IteratorForward>::isBegin() const
		throw()
	{
		if (Base::getTraits().isSingular())
		{
			return false;
		}
		return Base::getTraits().isBegin();
	}

	template <class Predicate, class IteratorForward>
	void ConstForwardFilterIterator<Predicate, IteratorForward>::toEnd()
		throw(Exception::SingularIterator)
	{
	  if (Base::isSingular())
	  {
	    Exception::SingularIterator e;
	    throw(e);
	  }
	  Base::getTraits().toEnd();
	}

	template <class Predicate, class IteratorForward>
	ConstForwardFilterIterator<Predicate, IteratorForward> 
		ConstForwardFilterIterator<Predicate, IteratorForward>::end(const Container& container)
		throw(Exception::Precondition)
	{
		ConstForwardFilterIterator<Predicate, IteratorForward> iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <class Predicate, class IteratorForward>
	bool ConstForwardFilterIterator<Predicate, IteratorForward>::isEnd() const
		throw()
	{
		if (Base::isSingular())
		{
			return false;
		}
		return Base::getTraits().isEnd();
	}
			
	/**	Generic mutable forward iterator class
	*/
	template <class Predicate, class IteratorForward>
	class ForwardFilterIterator
		: public ConstForwardFilterIterator<Predicate, IteratorForward>
	{
		public:

		/** @name Typedefs
		 */
		//@{
		
		///
		typedef typename IteratorForward::container_type	Container;
		///
		typedef typename IteratorForward::value_type	DataType;
		///
		typedef typename IteratorForward::difference_type	Position;
		///
		typedef typename IteratorForward::traits_type	Traits;
		///
		typedef typename IteratorForward::value_type	value_type;
		///
		typedef typename IteratorForward::difference_type	difference_type;
		///
		typedef	typename IteratorForward::pointer	pointer;
		///
		typedef typename IteratorForward::reference	reference;
	        ///
		typedef ConstForwardFilterIterator<Predicate, IteratorForward> Base;

		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		///
		ForwardFilterIterator() throw() {}

		///
		ForwardFilterIterator(Predicate p, IteratorForward it) throw()
		  :     ConstForwardFilterIterator<Predicate, IteratorForward>(p,it)
		{
		}
	
		///
		ForwardFilterIterator(const ForwardFilterIterator& iterator) throw()
			:	ConstForwardFilterIterator<Predicate, IteratorForward>(iterator)
		{
		}

		///
		~ForwardFilterIterator() throw() {}			
		//@}

		/** @name Assignemnt
		*/
		//@{
		///
		ForwardFilterIterator& operator = (const ForwardFilterIterator& iterator) throw()
		{
			Base::operator = (iterator);
			return *this;
		}

		///

		///
		void swap(ForwardFilterIterator& iterator) throw() 
		{ 
		  std::swap(Base::predicate_, iterator.predicate_); 
		  std::swap(Base::iterator_, iterator.iterator_);
		}
		//@}

		/** @name Iterator methods
		*/
		//@{

		/// Return an iterator pointing to the beginning of the container
		static ForwardFilterIterator begin(const Container& container) throw(Exception::Precondition);

		/// Return an iterator pointing at the end of the container
		static ForwardFilterIterator end(const Container& container) throw(Exception::Precondition);

		///
		reference operator * () const throw() 
		{ 
			return Base::getTraits().getData(); 
		}

		///
		pointer operator -> () const throw() 
		{ 
			return &Base::getTraits().getData(); 
		}

		/// Increment operator
		ForwardFilterIterator& operator ++ () throw(Exception::InvalidIterator)
		{
		  if (!Base::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		  
		  while (!Base::isEnd() && !Base::predicate_(Base::getTraits().getData()))
		    Base::getTraits().forward();
		  return *this;
		}
		
		/// Postfix increment operator
		ForwardFilterIterator operator ++ (int) throw(Exception::InvalidIterator)
		{
		  if (!Base::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		    ForwardFilterIterator tmp(*this);
		  ++(*this);
		  return tmp;
		}

		//@}

		protected:

		///
		ForwardFilterIterator(const Container& container) throw()	
		  :	ConstForwardFilterIterator<Predicate, IteratorForward>(container)
		{
		}
	};


	template <class Predicate, class IteratorForward>
	ForwardFilterIterator<Predicate, IteratorForward> 
		ForwardFilterIterator<Predicate, IteratorForward>::begin(const Container& container)
		throw(Exception::Precondition)
	{
		ForwardFilterIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <class Predicate, class IteratorForward>
	ForwardFilterIterator<Predicate, IteratorForward> 
		ForwardFilterIterator<Predicate, IteratorForward>::end(const Container& container)
		throw(Exception::Precondition)
	{
		ForwardFilterIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}


} // namespace BALL

#endif // BALL_KERNEL_FORWARDFILTERITERATOR_H
