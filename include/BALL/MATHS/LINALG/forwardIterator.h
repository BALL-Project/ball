#ifndef BALL_LINALG_FORWARDITERATOR_H
#define BALL_LINALG_FORWARDITERATOR_H

#ifndef BALL_LINALG_BASEITERATOR_H
#	include <BALL/MATHS/LINALG/baseIterator.h>
#endif

#include <BALL/COMMON/exception.h>
#include <BALL/MATHS/LINALG/linalgException.h>

namespace BALL 
{

	/**	@name	Generic forward iterator classes
	*/
	//@{

	/**	Generic non-mutable forward iterator
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
		ConstForwardIterator() throw() {}
	
		/// Copy constructor
		ConstForwardIterator(const ConstForwardIterator& iterator) throw()
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
			
		///
		~ConstForwardIterator() throw() {}

		//@}

		/** @name Assignment 
		*/
		//@{
		///
		ConstForwardIterator& operator = (const ConstForwardIterator& iterator) throw()
		{
			BaseIterator<Container, DataType, Position, Traits>::operator = (iterator);
			return *this;
		}

		///
		void swap(ConstForwardIterator& iterator) throw() { std::swap(ConstForwardIterator<Container, DataType, Position, Traits>::getTraits(), iterator.getTraits()); }
		//@}

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
		static ConstForwardIterator begin(const Container& container)
			throw(Exception::Precondition);

		/// Return an iterator pointing at the end of the container
		static ConstForwardIterator end(const Container& container) 
			throw(Exception::Precondition);

		/// Increment operator
		ConstForwardIterator& operator ++ () throw(Exception::InvalidIterator)
		{
		  if (!Base::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		  Base::getTraits().forward();
		  return *this;
		}
		
		/// Postfix increment operator
		ConstForwardIterator operator ++ (int) throw(Exception::InvalidIterator)
		{
		  if (!Base::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		  ConstForwardIterator tmp(*this);
		  ++(*this);
		  return tmp;
		}
		
		//@}
		
	protected:
		
		///
		ConstForwardIterator(const Container& container) throw()
		  :	BaseIterator<Container, DataType, Position, Traits>(container)
		{
		}
	};
  //@}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstForwardIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::SingularIterator)
	{
	  if (Base::isSingular())
	  {
	    Exception::SingularIterator e;
	    throw(e);
	  }
		Base::getTraits().toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits> 
		ConstForwardIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::Precondition)
	{
		ConstForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstForwardIterator<Container, DataType, Position, Traits>::isBegin() const
		throw()
	{
		if (Base::getTraits().isSingular())
		{
			return false;
		}
		return Base::getTraits().isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstForwardIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::SingularIterator)
	{
	  if (Base::isSingular())
	  {
	    Exception::SingularIterator e;
	    throw(e);
	  }
		Base::getTraits().toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits> 
		ConstForwardIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::Precondition)
	{
		ConstForwardIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstForwardIterator<Container, DataType, Position, Traits>::isEnd() const
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
		ForwardIterator() throw() {}
	
		///
		ForwardIterator(const ForwardIterator& iterator) throw()
			:	ConstForwardIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		///
		~ForwardIterator() throw() {}			
		//@}

		/** @name Assignemnt
		*/
		//@{
		///
		ForwardIterator& operator = (const ForwardIterator& iterator) throw()
		{
			ConstForwardIterator<Container, DataType, Position, Traits>::operator = (iterator);
			return *this;
		}

		///
		void swap(ForwardIterator& iterator) throw() { std::swap(Base::getTraits(), iterator.getTraits()); }
		//@}

		/** @name Iterator methods
		*/
		//@{

		/// Return an iterator pointing to the beginning of the container
		static ForwardIterator begin(const Container& container) throw(Exception::Precondition);

		/// Return an iterator pointing at the end of the container
		static ForwardIterator end(const Container& container) throw(Exception::Precondition);

		///
		reference operator * () const throw() 
		{ 
			return (reference)Base::getTraits().getData(); 
		}

		///
		pointer operator -> () const throw() 
		{ 
			return (pointer)&Base::getTraits().getData(); 
		}

		/// Increment operator
		ForwardIterator& operator ++ () throw(Exception::InvalidIterator)
		{
		  if (!Base::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		    Base::getTraits().forward();
		  return *this;
		}
		
		/// Postfix increment operator
		ForwardIterator operator ++ (int) throw(Exception::InvalidIterator)
		{
		  if (!Base::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		    ForwardIterator tmp(*this);
		  ++(*this);
		  return tmp;
		}

		//@}

		protected:

		///
		ForwardIterator(const Container& container) throw()	
			:	Base(container)
		{
		}
	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits> 
		ForwardIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::Precondition)
	{
		ForwardIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits> 
		ForwardIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::Precondition)
	{
		ForwardIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}


} // namespace BALL

#endif // BALL_KERNEL_FORWARDITERATOR_H
