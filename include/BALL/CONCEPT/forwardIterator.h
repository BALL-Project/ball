// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forwardIterator.h,v 1.23 2003/06/11 16:09:22 oliver Exp $
//

#ifndef BALL_CONCEPT_FORWARDITER_H
#define BALL_CONCEPT_FORWARDITER_H

#ifndef BALL_CONCEPT_BASEITERATOR_H
#	include <BALL/CONCEPT/baseIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL 
{

	/**	@name	Generic Forward Iterator Classes.
			
	 	 \ingroup  ConceptsIterators
	*/
	//@{

	/**	Generic Constant Forward Iterator
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
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		/// Default constructor
		ConstForwardIterator() throw() {}
	
		/// Copy constructor
		ConstForwardIterator(const ConstForwardIterator& iterator) throw();

		///
		ConstForwardIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator) 
			throw()
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		//@}

		/** @name Assignment 
		*/
		//@{
		//
		ConstForwardIterator& operator = (const ConstForwardIterator& iterator) throw();

		//
		ConstForwardIterator& operator = (const BaseIterator<Container, DataType, Position, Traits>& iterator) throw();
		//@}

		/** @name Iterator methods
		*/
		//@{

		/** Move the iterator to the item at the beginning of the container.
		 */
		void toBegin()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points to the first item of the container
		*/
		bool isBegin() const
			throw(Exception::InvalidIterator);

		/** Move the iterator behind the last item of the container.
		 */
		void toEnd()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points behind the last item of the container
		*/
		bool isEnd() const
			throw(Exception::InvalidIterator);

		/** Return an iterator pointing to the beginning of the container
		 */
		static ConstForwardIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return an iterator pointing at the end of the container
		 */
		static ConstForwardIterator end(const Container& container) 
			throw(Exception::InvalidIterator);

    /// Increment operator
    ConstForwardIterator& operator ++ () throw(Exception::InvalidIterator)
    {
      #ifdef BALL_DEBUG
        if (!ConstForwardIterator::isValid())
        {
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
      #endif
      BaseIterator<Container, DataType, Position, Traits>::getTraits().forward();
      return *this;
		}

    /// Postfix increment operator
    ConstForwardIterator operator ++ (int) throw(Exception::InvalidIterator)
    {
      #ifdef BALL_DEBUG
        if (!ConstForwardIterator::isValid())
        {
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
      #endif

      ConstForwardIterator tmp(*this);
      ++(*this);
      return tmp;
		}
 
		//@}

		protected:

		///
		ConstForwardIterator(const Container& container) throw();
	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstForwardIterator<Container, DataType, Position, Traits>::ConstForwardIterator
		(const ConstForwardIterator& iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstForwardIterator<Container, DataType, Position, Traits>&
	ConstForwardIterator<Container, DataType, Position, Traits>::operator =
		(const ConstForwardIterator<Container, DataType, Position, Traits>& iterator)
		throw()
	{
		if (this != &iterator)
		{
			BaseIterator<Container, DataType, Position, Traits>::operator = (iterator);
		}
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstForwardIterator<Container, DataType, Position, Traits>&
	ConstForwardIterator<Container, DataType, Position, Traits>::operator =
		(const BaseIterator<Container, DataType, Position, Traits>& iterator)
		throw()
	{
		if (this != &iterator)
		{
			BaseIterator<Container, DataType, Position, Traits>::operator = (iterator);
		}
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstForwardIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		BaseIterator<Container, DataType, Position, Traits>::getTraits().toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstForwardIterator<Container, DataType, Position, Traits> 
		ConstForwardIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstForwardIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		#ifndef BALL_DEBUG
			if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return BaseIterator<Container, DataType, Position, Traits>::getTraits().isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstForwardIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		#ifndef BALL_DEBUG
			if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		BaseIterator<Container, DataType, Position, Traits>::getTraits().toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstForwardIterator<Container, DataType, Position, Traits> 
		ConstForwardIterator<Container, DataType, Position, Traits>::end(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstForwardIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		#ifndef BALL_DEBUG
			if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return BaseIterator<Container, DataType, Position, Traits>::getTraits().isEnd();
	}
			
	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstForwardIterator<Container, DataType, Position, Traits>::ConstForwardIterator(const Container& container)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(container)
	{
	}


	/**	Generic Forward Iterator Class
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

		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		/** Default constructor
		*/
		BALL_INLINE ForwardIterator() throw() {}
	
		/** Copy constructor
		*/
		ForwardIterator(const ForwardIterator& iterator)
			throw();

		/** Detailed constructor
		*/
		ForwardIterator(const ConstForwardIterator<Container, DataType, Position, Traits>& iterator)
			throw()
			:	ConstForwardIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		/** @name Assignemnt
		*/
		//@{
		///
		ForwardIterator& operator = (const ForwardIterator& iterator) throw();
		///
		void swap(ForwardIterator& iterator) throw() { std::swap(traits_, iterator.traits_); }
		//@}

		/** @name Iterator methods
		*/
		//@{

		/// Return an iterator pointing to the beginning of the container
		static ForwardIterator begin(const Container& container) throw(Exception::InvalidIterator);

		/// Return an iterator pointing at the end of the container
		static ForwardIterator end(const Container& container) throw(Exception::InvalidIterator);

		///
		BALL_INLINE reference operator * () const throw() { return (reference)ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().getData(); }

		///
		BALL_INLINE pointer operator -> () const throw() { return (pointer)&ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().getData(); }

    /// Increment operator
    ForwardIterator& operator ++ () throw(Exception::Precondition)
    {
			BALL_PRECONDITION_EXCEPTION((ConstForwardIterator<Container, DataType, Position, Traits>::isValid()), "invalid iterator")
      ConstForwardIterator<Container, DataType, Position, Traits>::operator ++ ();
      return *this;
		}

    /// Postfix increment operator
    ForwardIterator operator ++ (int) throw(Exception::Precondition)
    {
			BALL_PRECONDITION_EXCEPTION((ConstForwardIterator<Container, DataType, Position, Traits>::isValid()), "invalid iterator")

      ForwardIterator tmp(*this);
      ++(*this);
      return tmp;
		}

		//@}

		protected:

		///
		ForwardIterator(const Container& container) throw();
	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ForwardIterator<Container, DataType, Position, Traits>::ForwardIterator(const ForwardIterator& iterator)
		throw()
		:	ConstForwardIterator<Container, DataType, Position, Traits>(iterator)
	{
	}


	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ForwardIterator<Container, DataType, Position, Traits>&
	ForwardIterator<Container, DataType, Position, Traits>::operator =
		(const ForwardIterator<Container, DataType, Position, Traits>& iterator)
		throw()
	{
		if (this != &iterator)
		{
			BaseIterator<Container, DataType, Position, Traits>::operator = (iterator);
		}
		return *this;
	}


	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ForwardIterator<Container, DataType, Position, Traits> 
		ForwardIterator<Container, DataType, Position, Traits>::begin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ForwardIterator<Container, DataType, Position, Traits> 
		ForwardIterator<Container, DataType, Position, Traits>::end(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ForwardIterator<Container, DataType, Position, Traits>::ForwardIterator(const Container& container)
		throw()
		:	ConstForwardIterator<Container, DataType, Position, Traits>(container)
	{
	}

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_FORWARDITER_H
