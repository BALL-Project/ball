// $Id: reverseBidirectionalIterator.h,v 1.5 2000/12/19 12:50:50 amoll Exp $

#ifndef BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H
#define BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/bidirectionalIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL 
{

	/**	@name Generic Reverse Bidirectional Iterator Classes.
			{\bf Definition:} \URL{BALL/CONCEPT/reverseBidirectionalIterator.h}
	*/
	//@{

	/**	Generic Reverse Bidirectional Iterator Class
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ReverseBidirectionalIterator
		: public BidirectionalIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**
		*/
		ReverseBidirectionalIterator()
			throw()
			:	BidirectionalIterator<Container, DataType, Position, Traits>()
		{
		}

		/**
		*/
		ReverseBidirectionalIterator(const ReverseBidirectionalIterator &iterator)
			throw()
			:	BidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		ReverseBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw()
			:	BidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{	
		}
		//@}

		bool operator + () const
			throw()
		{
			return traits_ptr_->isValid();
		}

		bool operator - () const
			throw()
		{
			return (traits_ptr_->isValid() == false);
		}

		void toBegin()
			throw(Exception::InvalidIterator)
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::toRBegin();
		}

		bool isBegin() const
			throw(Exception::InvalidIterator)
		{
			return BidirectionalIterator<Container, DataType, Position, Traits>::isRBegin();
		}

		void toEnd()
			throw(Exception::InvalidIterator)
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::toREnd();
		}

		bool isEnd() const
			throw(Exception::InvalidIterator)
		{
			return BidirectionalIterator<Container, DataType, Position, Traits>::isREnd();
		}

		void toRBegin()
			throw(Exception::InvalidIterator)
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::toBegin();
		}

		bool isRBegin() const
			throw(Exception::InvalidIterator)
		{
			return BidirectionalIterator<Container, DataType, Position, Traits>::isBegin();
		}

		void toREnd()
			throw(Exception::InvalidIterator)
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::toEnd();
		}

		bool isREnd() const
			throw(Exception::InvalidIterator)
		{
			return BidirectionalIterator<Container, DataType, Position, Traits>::isEnd();
		}

		ReverseBidirectionalIterator &operator ++ ()
			throw(Exception::InvalidIterator)
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::operator--();
			return *this;
		}

		ReverseBidirectionalIterator operator ++ (int)
			throw(Exception::InvalidIterator)
		{
			ReverseBidirectionalIterator iterator(*this);
			BidirectionalIterator<Container, DataType, Position, Traits>::operator--();
			return iterator;
		}

		ReverseBidirectionalIterator &operator -- ()
			throw(Exception::InvalidIterator)
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::operator++();
			return *this;
		}

		ReverseBidirectionalIterator operator -- (int)
			throw(Exception::InvalidIterator)
		{
			ReverseBidirectionalIterator iterator(*this);
			BidirectionalIterator<Container, DataType, Position, Traits>::operator++();
			return iterator;
		}

		DataType *findFirst(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		DataType *findLast(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}
	
		const DataType *findLast(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}

		DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		DataType *findNext(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}
	
		const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}

		static ReverseBidirectionalIterator begin(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ReverseBidirectionalIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static ReverseBidirectionalIterator end(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ReverseBidirectionalIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		static ReverseBidirectionalIterator rbegin(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ReverseBidirectionalIterator iterator(container);
			iterator.toRBegin();
			return iterator;
		}

		static ReverseBidirectionalIterator rend(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ReverseBidirectionalIterator iterator(container);
			iterator.toREnd();
			return iterator;
		}

		protected:

		ReverseBidirectionalIterator(const Container &container)
			throw()
			: BidirectionalIterator<Container, DataType, Position, Traits>(container)
		{
		}

	};


	/**	Generic Constant Reverse Bidirectional Iterator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ConstReverseBidirectionalIterator
		: public ConstBidirectionalIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{

		/**
		*/
		ConstReverseBidirectionalIterator()
			throw()
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>()
		{
		}

		/**
		*/
		ConstReverseBidirectionalIterator(const ConstReverseBidirectionalIterator &iterator)
			throw()
			: ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		ConstReverseBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw()
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + () const
			throw()
		{
			return traits_ptr_->isValid();
		}

		bool operator - () const
			throw()
		{
			return (traits_ptr_->isValid() == false);
		}

		void toBegin()
			throw(Exception::InvalidIterator)
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin();
		}

		bool isBegin() const
			throw(Exception::InvalidIterator)
		{
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isRBegin();
		}

		void toEnd()
			throw(Exception::InvalidIterator)
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::toREnd();
		}

		bool isEnd() const
			throw(Exception::InvalidIterator)
		{
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isREnd();
		}

		void toRBegin()
			throw(Exception::InvalidIterator)
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::toBegin();
		}

		bool isRBegin() const
			throw(Exception::InvalidIterator)
		{
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isBegin();
		}

		void toREnd()
			throw(Exception::InvalidIterator)
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::toEnd();
		}

		bool isREnd() const
			throw(Exception::InvalidIterator)
		{
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isEnd();
		}

		ConstReverseBidirectionalIterator &operator ++ ()
			throw(Exception::InvalidIterator)
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator--();
			return *this;
		}

		ConstReverseBidirectionalIterator operator ++ (int)
			throw(Exception::InvalidIterator)
		{
			ConstReverseBidirectionalIterator iterator(*this);
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator--();
			return iterator;
		}

		ConstReverseBidirectionalIterator &operator -- ()
			throw(Exception::InvalidIterator)
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator++();
			return *this;
		}

		ConstReverseBidirectionalIterator operator -- (int)
			throw(Exception::InvalidIterator)
		{
			ConstReverseBidirectionalIterator iterator(*this);
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator++();
			return iterator;
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType *findLast(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}
	
		const DataType *findLast(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}

		const DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType *findNext(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}
	
		const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}

		static ConstReverseBidirectionalIterator begin(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ConstReverseBidirectionalIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static ConstReverseBidirectionalIterator end(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ConstReverseBidirectionalIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		static ConstReverseBidirectionalIterator rbegin(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ConstReverseBidirectionalIterator iterator(container);
			iterator.toRBegin();
			return iterator;
		}

		static ConstReverseBidirectionalIterator rend(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ConstReverseBidirectionalIterator iterator(container);
			iterator.toREnd();
			return iterator;
		}

		protected:

		ConstReverseBidirectionalIterator(const Container &container)
			throw()
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(container)
		{
		}

	};

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H
