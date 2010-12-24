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
		ConstForwardIterator() {}
	
		/// Copy constructor
		ConstForwardIterator(const ConstForwardIterator& iterator)
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
			
		///
		~ConstForwardIterator() {}

		//@}

		/** @name Assignment 
		*/
		//@{
		///
		ConstForwardIterator& operator = (const ConstForwardIterator& iterator)
		{
			BaseIterator<Container, DataType, Position, Traits>::operator = (iterator);
			return *this;
		}

		///
		void swap(ConstForwardIterator& iterator) { std::swap(ConstForwardIterator<Container, DataType, Position, Traits>::getTraits(), iterator.getTraits()); }
		//@}

		/** @name Iterator methods
		*/
		//@{

		/** Move the iterator to the item at the beginning of the container
		 *  @exception Exception::SingularIterator if the iterator is Singular
		 */
		void toBegin();

		/// Check whether the iterator points to the first item of the container
		bool isBegin() const;

		/** Move the iterator behind the last item of the container
		 *  @exception Exception::SingularIterator if the iterator is Singular
		 */
		void toEnd();

		/// Check whether the iterator points behind the last item of the container
		bool isEnd() const;

		/** Return an iterator pointing to the beginning of the container
		 *  @exception Exception::Precondition
		 */
		static ConstForwardIterator begin(const Container& container);

		/** Return an iterator pointing at the end of the container
		 *  @exception Exception::Precondition
		 */
		static ConstForwardIterator end(const Container& container);

		/** Increment operator
		 *  @exception Exception::InvalidOperator if the iterator is invalid
		 */
		ConstForwardIterator& operator ++ ()
		{
		  if (!Base::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		  Base::getTraits().forward();
		  return *this;
		}
		
		/** Postfix increment operator
		 *  @exception Exception::InvalidOperator if the iterator is invalid
		 */
		ConstForwardIterator operator ++ (int)
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
		ConstForwardIterator(const Container& container)
		  :	BaseIterator<Container, DataType, Position, Traits>(container)
		{
		}
	};
  //@}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstForwardIterator<Container, DataType, Position, Traits>::toBegin()
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
	{
		ConstForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstForwardIterator<Container, DataType, Position, Traits>::isBegin() const
	{
		if (Base::getTraits().isSingular())
		{
			return false;
		}
		return Base::getTraits().isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstForwardIterator<Container, DataType, Position, Traits>::toEnd()
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
	{
		ConstForwardIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
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
		ForwardIterator() {}
	
		///
		ForwardIterator(const ForwardIterator& iterator)
			:	ConstForwardIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		///
		~ForwardIterator() {}			
		//@}

		/** @name Assignemnt
		*/
		//@{
		///
		ForwardIterator& operator = (const ForwardIterator& iterator)
		{
			ConstForwardIterator<Container, DataType, Position, Traits>::operator = (iterator);
			return *this;
		}

		///
		void swap(ForwardIterator& iterator) { std::swap(Base::getTraits(), iterator.getTraits()); }
		//@}

		/** @name Iterator methods
		*/
		//@{

		/** Return an iterator pointing to the beginning of the container
		 *  @exception Exception::Precondition
		 */
		static ForwardIterator begin(const Container& container);

		/** Return an iterator pointing at the end of the container
		 *  @exception Exception::Precondition
		 */
		static ForwardIterator end(const Container& container);

		///
		reference operator * () const
		{ 
			return (reference)Base::getTraits().getData(); 
		}

		///
		pointer operator -> () const
		{ 
			return (pointer)&Base::getTraits().getData(); 
		}

		/** Increment operator
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ForwardIterator& operator ++ ()
		{
		  if (!Base::isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		    Base::getTraits().forward();
		  return *this;
		}
		
		/** Postfix increment operator
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ForwardIterator operator ++ (int)
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
		ForwardIterator(const Container& container)
			:	Base(container)
		{
		}
	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits> 
		ForwardIterator<Container, DataType, Position, Traits>::begin(const Container& container)
	{
		ForwardIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits> 
		ForwardIterator<Container, DataType, Position, Traits>::end(const Container& container)
	{
		ForwardIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}


} // namespace BALL

#endif // BALL_KERNEL_FORWARDITERATOR_H
