// $Id: bidirectionalIterator.h,v 1.13 2001/07/04 18:31:41 amoll Exp $ 

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#define BALL_CONCEPT_BIDIRECTIONALITERATOR_H

#ifndef BALL_CONCEPT_BASEITERATOR_H
#	include <BALL/CONCEPT/baseIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL 
{

	/**	@name	Bidirectional Iterators
	*/
	//@{

	/**	@name Bidirectional Iterator.
			{\bf Definition:} \URL{BALL/CONCEPT/bidirectionalIterator.h}
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class BidirectionalIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(BidirectionalIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a bidirectional iterator according to the STL scheme
		 */
		typedef ::std::bidirectional_iterator_tag iterator_category;
		
		//@}
		/** @name Constructors and Destructor
		 */
		//@{

		/// Default constructor
		BidirectionalIterator()
			throw();
	
		/// Copy constructor
		BidirectionalIterator(const BidirectionalIterator& iterator)
			throw();

		/// Detailed constructor
		BidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw();

		/// Destructor
		~BidirectionalIterator()
			throw();

		//@}
		/** @name Iterator methods
		 */
		//@{

		/** BALL iterator validity check.
				@return true if the iterator is valid
		*/
		bool operator + () const
			throw();

		/** Move the iterator to the beginning of the container.
		 */
		void toBegin()
			throw(Exception::InvalidIterator);

		/** Test whether the iterator points to the first element in the
				container.
		*/
		bool isBegin() const
			throw(Exception::InvalidIterator);

		/** Move the iterator to the end of the container.
		 */
		void toEnd()
			throw(Exception::InvalidIterator);

		/** Test whether the iterator points behind the last element in the container.
		*/
		bool isEnd() const
			throw(Exception::InvalidIterator);

		/** Move the iterator to the "reverse" beginning of the container.
		 */
		void toRBegin()
			throw(Exception::InvalidIterator);

		/** Test whether the iterator points before the first element of the container.
		*/
		bool isRBegin() const
			throw(Exception::InvalidIterator);

		/** Move the iterator before the first element of the container.
		 */
		void toREnd()
			throw(Exception::InvalidIterator);

		/** Test whether the iterator points before the first element of the container. 
		*/
		bool isREnd() const
			throw(Exception::InvalidIterator);

		/** Advance the iterator.
		 */
		BidirectionalIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		/** Advance the iterator (postfix notation).
		*/
		BidirectionalIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		/** Advance the iterator in reverse direction.
		 */
		BidirectionalIterator& operator -- ()
			throw(Exception::InvalidIterator);

		/** Advance the iterator in reverse direction (postfix notation).
		 */
		BidirectionalIterator operator -- (int)
			throw(Exception::InvalidIterator);

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
		static BidirectionalIterator rend(const Container &container)
			throw(Exception::InvalidIterator);

		/** Find the previous item that is satisfying the predicate
		*/
		DataType* findPrevious(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);

		/** Find the next position that is satisfying the predicate
		*/
		DataType* findNext(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);

		//@}

		protected:

		/** @name Protected methods
		*/
		//@{

		/*_ Detailed ctor which is for internal use only.
		*/
		BidirectionalIterator(const Container& container)
			throw();

		//@}

	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>::BidirectionalIterator()
		throw()
		: BaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>
	::BidirectionalIterator(const BidirectionalIterator& iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>
	::BidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>::~BidirectionalIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool BidirectionalIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return traits_ptr_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void BidirectionalIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool BidirectionalIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void BidirectionalIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool BidirectionalIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void BidirectionalIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool BidirectionalIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void BidirectionalIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool BidirectionalIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>& 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
		 throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>& 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
		 throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->backward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>
  	BidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> 
		BidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container &container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>::BidirectionalIterator(const Container& container)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(container)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* BidirectionalIterator<Container, DataType, Position, Traits>
		::findPrevious(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->backward(); traits_ptr_->isREnd() == false;
				traits_ptr_->backward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* BidirectionalIterator<Container, DataType, Position, Traits>
		::findNext(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->forward(); traits_ptr_->isEnd() == false;
				traits_ptr_->forward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}



	/**	Constant Bidirectional Iterator.
			{\bf Definition:} \URL{BALL/CONCEPT/bidirectionalIterator.h}
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstBidirectionalIterator
		: public ConstBaseIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(ConstBidirectionalIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a bidirectional iterator according to the STL
				scheme
		 */
		typedef ::std::bidirectional_iterator_tag iterator_category;

		//@}

		/** @name Constructors and destructor.
		 */
		//@{

		/** Constructor
		 */
		ConstBidirectionalIterator()
			throw();
	
		/** Copy constructor
		 */
		ConstBidirectionalIterator(const ConstBidirectionalIterator& iterator)
			throw();

		/** Detailed constructor
		 */
		ConstBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
			throw();

		/** Destructor
		 */
		~ConstBidirectionalIterator()
			throw();

		//@}
		/** @name Iterator methods
		 */
		//@{

		/** BALL iterator validity check.
				@return true if the iterator is valid
		*/
		bool operator + () const
			throw(Exception::InvalidIterator);

		/** Move the iterator to the beginning of the container.
		 */
		void toBegin()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points to the item at the beginning of the container.
		*/
		bool isBegin() const
			throw(Exception::InvalidIterator);

		/** Move the iterator to the position after the last item of the container.
		*/
		void toEnd()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points to the position after the last item of the container.
		*/
		bool isEnd() const
			throw(Exception::InvalidIterator);

		/** Move the iterator to the "reverse" beginning of the container.
		*/
		void toRBegin()
			throw(Exception::InvalidIterator);

		/** Test whether the iterator points to the "reverse" first element
				of the container.
		*/
		bool isRBegin() const
			throw(Exception::InvalidIterator);

		/** Move the iterator to the position before the first element.
		*/
		void toREnd()
			throw(Exception::InvalidIterator);

		/** Test wheter the iterator points to the position before the first element.
		*/
		bool isREnd() const
			throw(Exception::InvalidIterator);

		/** Advance the iterator.
		*/
		ConstBidirectionalIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		/** Advance the iterator (postfix notation).
		*/
		ConstBidirectionalIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		/** Advance the iterator in reverse direction.
		*/
		ConstBidirectionalIterator& operator -- ()
			throw(Exception::InvalidIterator);

		/** Advance the iterator in reverse direction (postfix notation).
		*/
		ConstBidirectionalIterator operator -- (int)
			throw(Exception::InvalidIterator);

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

		/** Find the previous position that is satisfying the predicate
		*/
		const DataType* findPrevious(const UnaryPredicate<DataType>& predicate)
					throw(Exception::InvalidIterator);

		/** Find the next position that is satisfying the predicate
		*/
		const DataType* findNext(const UnaryPredicate<DataType>& predicate)
					throw(Exception::InvalidIterator);

		//@}

		protected:

		/** @name Protected methods
		*/
		//@{

		/*_ Detailed ctor which is for internal use only
		*/
		ConstBidirectionalIterator(const Container& container)
			throw();

		//@}

	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>::ConstBidirectionalIterator()
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>
	::ConstBidirectionalIterator(const ConstBidirectionalIterator& iterator)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>
	::ConstBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
		throw()
		:	ConstBaseIterator <Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>::~ConstBidirectionalIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator + () const
		throw(Exception::InvalidIterator)
	{
		return traits_ptr_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstBidirectionalIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstBidirectionalIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstBidirectionalIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstBidirectionalIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstBidirectionalIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		traits_ptr_->backward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> 
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>
	::ConstBidirectionalIterator(const Container& container)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>
	::findPrevious(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->backward(); traits_ptr_->isREnd() == false;
				traits_ptr_->backward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (const DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>
	::findNext(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->forward(); traits_ptr_->isEnd() == false;
				traits_ptr_->forward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (const DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	//@}

} // namespace BALL 

#endif // BALL_CONCEPT_BIDIRECTIONALITERATOR_H
