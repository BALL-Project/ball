// $Id: forwardIterator.h,v 1.8 2001/06/26 08:58:21 anker Exp $

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
			{\bf Definition:} \URL{BALL/CONCEPT/forwardIterator.h}
	*/
	//@{

	/**	Generic Constant Forward Iterator
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstForwardIterator
		: public ConstBaseIterator<Container, DataType, Position, Traits>
	{
		public:
	
		BALL_CREATE(ConstForwardIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a forward iterator according to the STL scheme
		 */
		typedef ::std::forward_iterator_tag iterator_category;

		//@}
		/**	@name Constructors and Destructors
		*/
		//@{

		/** Default constructor
		*/
		ConstForwardIterator()
			throw();
	
		/** Copy constructor
		*/
		ConstForwardIterator(const ConstForwardIterator &iterator)
			throw();

		/** Detailed constructor
		*/
		ConstForwardIterator(const ConstBaseIterator<Container, DataType, Position, Traits> &iterator)
			throw();
		
		//@}
		/** @name Iterator methods
		*/
		//@{

		/** BALL specific shortcut for validity checking.
		 */
		bool operator +	() const
			throw();

		/** Move the iterator to the item at the beginning of the container.
		 */
		void toBegin()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points to the item at the beginning 
				of the container
		*/
		bool isBegin() const
			throw(Exception::InvalidIterator);

		/** Move the iterator to the item at the end of the container.
		 */
		void toEnd()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points to the item at the end of the
				container
		*/
		bool isEnd() const
			throw(Exception::InvalidIterator);

		/** Advance the iterator.
		 */
		ConstForwardIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		/** BAUSTELLE
		 */
		ConstForwardIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		/** Return an iterator at the beginning of the container
		 */
		static ConstForwardIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return an iterator at the end of the container
		 */
		static ConstForwardIterator end(const Container &container) 
			throw(Exception::InvalidIterator);

		//@}

		protected:

		/*_
		 */
		ConstForwardIterator(const Container& container)
			throw();

	};

	// method implementations

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits>::ConstForwardIterator()
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits>::ConstForwardIterator(const ConstForwardIterator &iterator)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits>::ConstForwardIterator(const ConstBaseIterator<Container, DataType, Position, Traits> &iterator)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstForwardIterator<Container, DataType, Position, Traits>::operator +	() const
		throw()
	{
		return traits_ptr_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstForwardIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		traits_ptr_->toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits> ConstForwardIterator<Container, DataType, Position, Traits>::begin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstForwardIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		return traits_ptr_->isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstForwardIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		traits_ptr_->toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits> ConstForwardIterator<Container, DataType, Position, Traits>::end(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstForwardIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		return traits_ptr_->isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits>& ConstForwardIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		traits_ptr_->forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits> ConstForwardIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ConstForwardIterator iterator(*this);
		++(*this);
		return iterator;
	}


	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstForwardIterator<Container, DataType, Position, Traits>::ConstForwardIterator(const Container& container)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
	{
	}



	/**	Generic Forward Iterator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ForwardIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(ForwardIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a forward iterator according to the STL scheme
		 */
		typedef ::std::forward_iterator_tag iterator_category;

		//@}
		/**	@name Constructors and Destructors
		*/
		//@{

		/** Default constructor
		*/
		ForwardIterator()
			throw();
	
		/** Copy constructor
		*/
		ForwardIterator(const ForwardIterator& iterator)
			throw();

		/** Detailed constructor
		*/
		ForwardIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw();

		//@}
		/** @name Iterator methods
		*/
		//@{

		/** BALL specific shortcut for validity checking.
		 */
		bool operator + () const
			throw();

		/** Move the iterator to the item at the beginning of the container.
		 */
		void toBegin()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points to the item at the beginning 
				of the container
		*/
		bool isBegin() const
			throw(Exception::InvalidIterator);

		/** Move the iterator to the item at the end of the container.
		 */
		void toEnd()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points to the item at the end of the
				container
		*/
		bool isEnd() const
			throw(Exception::InvalidIterator);

		/** Advance the iterator.
		 */
		ForwardIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		/** BAUSTELLE
		 */
		ForwardIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		/** Return an iterator at the beginning of the container
		 */
		static ForwardIterator begin(const Container &container)
			throw(Exception::InvalidIterator);

		/** Return an iterator at the end of the container
		 */
		static ForwardIterator end(const Container &container)
			throw(Exception::InvalidIterator);

		//@}

		protected:

		/*_
		 */
		ForwardIterator(const Container& container)
			throw();

	};

	// AB HIER BAUSTELLE
	// method implementations

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits>::ForwardIterator()
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits>::ForwardIterator(const ForwardIterator& iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits>::ForwardIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ForwardIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return traits_ptr_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ForwardIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		traits_ptr_->toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ForwardIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		return traits_ptr_->isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ForwardIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		traits_ptr_->toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ForwardIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		return traits_ptr_->isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits>& ForwardIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
			throw Exception::InvalidIterator(__FILE__, __LINE__);

		traits_ptr_->forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits> ForwardIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ForwardIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits> ForwardIterator<Container, DataType, Position, Traits>::begin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits> ForwardIterator<Container, DataType, Position, Traits>::end(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ForwardIterator<Container, DataType, Position, Traits> iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ForwardIterator<Container, DataType, Position, Traits>::ForwardIterator(const Container& container)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(container)
	{
	}

//@}

} // namespace BALL

#endif // BALL_CONCEPT_FORWARDITER_H
