// $Id: bidirectionalIterator.h,v 1.7 2001/06/06 15:48:12 anker Exp $ 

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

	/**	@name Bidirectional Iterator
			\\
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

		/** Tag this iterator as a bidirectional iterator according to the STL
				scheme
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
				@return {\bf true} if the iterator is valid
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

		/** Test whether the iterator points to the last element in the
				container.
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

		/** Move the iterator to the "reverse" end of the container.
		 */
		void toREnd()
			throw(Exception::InvalidIterator);

		/** Test whether the iterator points to the "reverse" last element 
				of the container.
		*/
		bool isREnd() const
			throw(Exception::InvalidIterator);

		/** Advance the iterator.
		 */
		BidirectionalIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		/** BAUSTELLE
		 */
		BidirectionalIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		/** Advance the iterator in reverse direction.
		 */
		BidirectionalIterator& operator -- ()
			throw(Exception::InvalidIterator);

		/** BAUSTELLE
		 */
		BidirectionalIterator operator -- (int)
			throw(Exception::InvalidIterator);

		/** Find the first item that matches the predicate {\tt predicate}
				within the container.
				@param predicate the predicate
				@return a mutable pointer to the item
		*/
		DataType* findFirst(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		// BAUSTELLE: 
		// Are the const versions of the followin methods necessary? We have a
		// ConstBidirectionalIterator...

		/** Find the first item that matches the predicate {\tt predicate}
				within the container (const version).
				@param predicate the predicate
				@return a const pointer to the item
		*/
		const DataType* findFirst(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		/** Find the last item that matches the predicate {\tt predicate}
				within the container.
				@param predicate the predicate
				@return a mutable pointer to the item
		*/
		DataType* findLast(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator);
	
		/** Find the last item that matches the predicate {\tt predicate}
				within the container (const version).
				@param predicate the predicate
				@return a const pointer to the item
		*/
		const DataType* findLast(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		/** Find the next item that matches the predicate {\tt predicate}
				within the container.
				@param predicate the predicate
				@return a mutable pointer to the item
		*/
		DataType* findNext(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);

		/** Find the next item that matches the predicate {\tt predicate}
				within the container (const version).
				@param predicate the predicate
				@return a const pointer to the item
		*/
		const DataType* findNext(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		/** Find the previous item that matches the predicate {\tt predicate}
				within the container.
				@param predicate the predicate
				@return a mutable pointer to the item
		*/
		DataType* findPrevious(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		/** Find the previous item that matches the predicate {\tt predicate}
				within the container (const version).
				@param predicate the predicate
				@return a const pointer to the item
		*/
		const DataType* findPrevious(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		/** Return the first item of the container.
		 */
		static BidirectionalIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return the last item of the container.
		 */
		static BidirectionalIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return the first item of the container when acting as a reverse
				iterator.
		 */
		static BidirectionalIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return the last item of the container when acting as a reverse
				iterator.
		 */
		static BidirectionalIterator rend(const Container &container)
			throw(Exception::InvalidIterator);

		//@}


		protected:

		/*_
		*/
		BidirectionalIterator(const Container& container)
			throw();

	};

	// method implementations

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>::BidirectionalIterator()
		throw()
		: BaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>::BidirectionalIterator(const BidirectionalIterator& iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>::BidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
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
	BidirectionalIterator<Container, DataType, Position, Traits>& BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
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
	BidirectionalIterator<Container, DataType, Position, Traits> BidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits>& BidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
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
	BidirectionalIterator<Container, DataType, Position, Traits> BidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* BidirectionalIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->toBegin(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* BidirectionalIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((BidirectionalIterator *)this)->findFirst(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* BidirectionalIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType> &predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->toRBegin(); traits_ptr_->isREnd() == false; traits_ptr_->backward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* BidirectionalIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((BidirectionalIterator *)this)->findLast(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* BidirectionalIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->forward(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* BidirectionalIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((BidirectionalIterator *)this)->findNext(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* BidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->backward(); traits_ptr_->isREnd() == false; traits_ptr_->backward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* BidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((BidirectionalIterator *)this)->findPrevious(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> BidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> BidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> BidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BidirectionalIterator<Container, DataType, Position, Traits> BidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container &container)
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


	/**	Constant Bidirectional Iterator
			\\
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
				@return {\bf true} if the iterator is valid
		*/
		bool operator + () const
			throw(Exception::InvalidIterator);

		/** Move the iterator to the beginning of the container.
		 */
		void toBegin()
			throw(Exception::InvalidIterator);

		bool isBegin() const
			throw(Exception::InvalidIterator);

		void toEnd()
			throw(Exception::InvalidIterator);

		bool isEnd() const
			throw(Exception::InvalidIterator);

		void toRBegin()
			throw(Exception::InvalidIterator);

		bool isRBegin() const
			throw(Exception::InvalidIterator);

		void toREnd()
			throw(Exception::InvalidIterator);

		bool isREnd() const
			throw(Exception::InvalidIterator);

		ConstBidirectionalIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		ConstBidirectionalIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		ConstBidirectionalIterator& operator -- ()
			throw(Exception::InvalidIterator);

		ConstBidirectionalIterator operator -- (int)
			throw(Exception::InvalidIterator);

		const DataType* findFirst(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findFirst(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		const DataType* findLast(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findLast(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		const DataType* findNext(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findNext(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		const DataType* findPrevious(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findPrevious(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		static ConstBidirectionalIterator begin(const Container &container)
			throw(Exception::InvalidIterator);

		static ConstBidirectionalIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		static ConstBidirectionalIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		static ConstBidirectionalIterator rend(const Container& container)
			throw(Exception::InvalidIterator);

		//@}


		protected:

		ConstBidirectionalIterator(const Container& container)
			throw();

	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>::ConstBidirectionalIterator()
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>::ConstBidirectionalIterator(const ConstBidirectionalIterator& iterator)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>::ConstBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
		throw()
		:	ConstBaseIterator <Container, DataType, Position, Traits>(iterator)
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
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
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
	ConstBidirectionalIterator<Container, DataType, Position, Traits> ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>& ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
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
	ConstBidirectionalIterator<Container, DataType, Position, Traits> ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->toBegin(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (const DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((ConstBidirectionalIterator *)this)->findFirst(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->toRBegin(); traits_ptr_->isREnd() == false; traits_ptr_->backward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (const DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((ConstBidirectionalIterator *)this)->findLast(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->forward(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (const DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((ConstBidirectionalIterator *)this)->findNext(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		for(traits_ptr_->backward(); traits_ptr_->isREnd() == false; traits_ptr_->backward())
		{
			if (predicate((const DataType &)traits_ptr_->getData()) == true)
			{
				return (const DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((ConstBidirectionalIterator *)this)->findPrevious(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> ConstBidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> ConstBidirectionalIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> ConstBidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits> ConstBidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBidirectionalIterator<Container, DataType, Position, Traits>::ConstBidirectionalIterator(const Container& container)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
	{
	}

	//@}

} // namespace BALL 

#endif // BALL_CONCEPT_BIDIRECTIONALITERATOR_H
