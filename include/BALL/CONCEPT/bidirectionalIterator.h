// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: bidirectionalIterator.h,v 1.27 2003/06/11 16:09:22 oliver Exp $ 
//

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#define BALL_CONCEPT_BIDIRECTIONALITERATOR_H

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
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
		
		//@}

		/** @name Constructors and destructor.
		 */
		//@{

		/// Default constructor
		BALL_INLINE ConstBidirectionalIterator() throw() {}
	
		/// Copy constructor
		ConstBidirectionalIterator(const ConstBidirectionalIterator& iterator) throw();

		///
		ConstBidirectionalIterator(const ConstForwardIterator<Container, DataType, Position, Traits>& iterator)
			throw()
			:	ConstForwardIterator <Container, DataType, Position, Traits>(iterator)
		{
		}


		/// Destructor
		BALL_INLINE ~ConstBidirectionalIterator() throw() {}
		//@}

		/** @name Iterator methods
		 */
		//@{

		using BaseIterator<Container, DataType, Position, Traits>::operator +;
		using BaseIterator<Container, DataType, Position, Traits>::operator -;

		/// Move the iterator to the beginning of the container
		void toBegin() throw(Exception::InvalidIterator);

		/// Check whether the iterator points to the item at the beginning of the container
		bool isBegin() const throw(Exception::InvalidIterator);

		/// Move the iterator to the position after the last item of the container
		void toEnd() throw(Exception::InvalidIterator);

		/// Check whether the iterator points to the position after the last item of the container.
		bool isEnd() const throw(Exception::InvalidIterator);

		/// Move the iterator to the "reverse" beginning of the container
		void toRBegin() throw(Exception::InvalidIterator);

		/// Test whether the iterator points to the "reverse" first element	of the container
		bool isRBegin() const	throw(Exception::InvalidIterator);

		/// Move the iterator to the position before the first element
		void toREnd()	throw(Exception::InvalidIterator);

		/// Test wheter the iterator points to the position before the first element
		bool isREnd() const throw(Exception::InvalidIterator);

		/// Increment operator
		ConstBidirectionalIterator& operator ++ () throw(Exception::InvalidIterator);

		/// Postfix increment operator
		ConstBidirectionalIterator operator ++ (int) throw(Exception::InvalidIterator);

		/// Decrement operator
		ConstBidirectionalIterator& operator -- () throw(Exception::InvalidIterator);

		/// Postfix decrement operator
		ConstBidirectionalIterator operator -- (int) throw(Exception::InvalidIterator);

		/** Return a new iterator.
				It will point to the first item of the container.
		*/
		static ConstBidirectionalIterator begin(const Container &container)
			throw(Exception::InvalidIterator);

		/** Return a new iterator.
				It will point to the position after the last element.
		*/
		static ConstBidirectionalIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a new iterator.
				It will point to the last element.
		*/
		static ConstBidirectionalIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a new iterator.
				It will point to the positon before the first element.
		*/
		static ConstBidirectionalIterator rend(const Container& container)
			throw(Exception::InvalidIterator);
		//@}

		protected:

		//
		ConstBidirectionalIterator(const Container& container)
			throw();
	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits>::ConstBidirectionalIterator(const ConstBidirectionalIterator& iterator)
		throw()
		:	ConstForwardIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstBidirectionalIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstBidirectionalIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isEnd();
	}
			
	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstBidirectionalIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{	
		#ifdef BALL_DEBUG
			if (ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstBidirectionalIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (!ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().backward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBidirectionalIterator<Container, DataType, Position, Traits>
	::ConstBidirectionalIterator(const Container& container)
		throw()
		:	ConstForwardIterator<Container, DataType, Position, Traits>(container)
	{
	}

	/**	@name Bidirectional Iterator.
			
	*/
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
		//@}

		/** @name Constructors and Destructor
		 */
		//@{

		/// Default constructor
		BALL_INLINE BidirectionalIterator() throw() {}
	
		/// Copy constructor
		BidirectionalIterator(const BidirectionalIterator& iterator)
			throw()
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/// Copy constructor
		BidirectionalIterator(const ConstBidirectionalIterator<Container, DataType, Position, Traits>& iterator)
			throw()
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/// Destructor
		BALL_INLINE ~BidirectionalIterator() throw() {}

		//@}

		/** @name Iterator methods
		 */
		//@{

		/// Dereferentiation
		BALL_INLINE reference operator * () const throw() { return (reference)ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().getData(); }

		/// Pointer dereferentiation
		BALL_INLINE pointer operator -> () const throw() { return (pointer)&ConstForwardIterator<Container, DataType, Position, Traits>::getTraits().getData(); }

		/// Increment operator
		BidirectionalIterator& operator ++ ()	throw(Exception::InvalidIterator);

		/// Postfix increment operator
		BidirectionalIterator operator ++ (int) throw(Exception::InvalidIterator);

		/// Decrement operator
		BidirectionalIterator& operator -- ()	throw(Exception::InvalidIterator);

		/// Postfix decrement operator
		BidirectionalIterator operator -- (int) throw(Exception::InvalidIterator);

		/** Return a new iterator.
				It will point to the first item of the container. 
		 */
		static BidirectionalIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a new iterator.
				It will point to the position after the last element.
		 */
		static BidirectionalIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a new iterator.
				It will point to the last element.
		 */
		static BidirectionalIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a new iterator.
				It will point to the positon before the first element.
		 */
		static BidirectionalIterator rend(const Container& container)
			throw(Exception::InvalidIterator);
		//@}

		protected:

		//
		BidirectionalIterator(const Container& container)	throw();
	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits>& 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
			 throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits>& 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
			{
			 throw Exception::InvalidIterator(__FILE__, __LINE__);
			}	
		#endif

		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- ();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits>
  	BidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container &container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BidirectionalIterator<Container, DataType, Position, Traits>::BidirectionalIterator(const Container& container)
		throw()
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(container)
	{
	}

	//@}

	/** @} */
} // namespace BALL 

#endif // BALL_CONCEPT_BIDIRECTIONALITERATOR_H
