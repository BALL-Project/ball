// $Id: reverseBidirectionalIterator.h,v 1.2 1999/12/04 18:34:12 oliver Exp $

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

namespace BALL {

	/**	@name Generic Reverse Bidirectional Iterator Classes.
			{\bf Definition:} \URL{BALL/CONCEPT/reverseBidirectionalIterator.h}
	*/
	//@{

	/**	Generic Reverse Bidirectional Manipulator Class
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ReverseBidirectionalManipulator
		: public BidirectionalManipulator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**
		*/
		ReverseBidirectionalManipulator()
			:	BidirectionalManipulator<Container, DataType, Position, Traits>()
		{
		}

		/**
		*/
		ReverseBidirectionalManipulator(const ReverseBidirectionalManipulator &manipulator)
			:	BidirectionalManipulator<Container, DataType, Position, Traits>(manipulator)
		{
		}

		/**
		*/
		ReverseBidirectionalManipulator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	BidirectionalManipulator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + () const
		{
			return traits_->isValid();
		}

		bool operator - () const
		{
			return (bool)(traits_->isValid() == false);
		}

		void toBegin()
		{
			BidirectionalManipulator<Container, DataType, Position, Traits>::toRBegin();
		}

		bool isBegin() const
		{
			return BidirectionalManipulator<Container, DataType, Position, Traits>::isRBegin();
		}

		void toEnd()
		{
			BidirectionalManipulator<Container, DataType, Position, Traits>::toREnd();
		}

		bool isEnd() const
		{
			return BidirectionalManipulator<Container, DataType, Position, Traits>::isREnd();
		}

		void toRBegin()
		{
			BidirectionalManipulator<Container, DataType, Position, Traits>::toBegin();
		}

		bool isRBegin() const
		{
			return BidirectionalManipulator<Container, DataType, Position, Traits>::isBegin();
		}

		void toREnd()
		{
			BidirectionalManipulator<Container, DataType, Position, Traits>::toEnd();
		}

		bool isREnd() const
		{
			return BidirectionalManipulator<Container, DataType, Position, Traits>::isEnd();
		}

		ReverseBidirectionalManipulator &operator ++ ()
		{
			BidirectionalManipulator<Container, DataType, Position, Traits>::operator--();
			return *this;
		}

		ReverseBidirectionalManipulator operator ++ (int)
		{
			ReverseBidirectionalManipulator manipulator(*this);
			BidirectionalManipulator<Container, DataType, Position, Traits>::operator--();
			return manipulator;
		}

		ReverseBidirectionalManipulator &operator -- ()
		{
			BidirectionalManipulator<Container, DataType, Position, Traits>::operator++();
			return *this;
		}

		ReverseBidirectionalManipulator operator -- (int)
		{
			ReverseBidirectionalManipulator manipulator(*this);
			BidirectionalManipulator<Container, DataType, Position, Traits>::operator++();
			return manipulator;
		}

		DataType *findFirst(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		DataType *findLast(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findFirst(predicate);
		}
	
		const DataType *findLast(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findFirst(predicate);
		}

		DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		DataType *findNext(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}
	
		const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}

		static ReverseBidirectionalManipulator begin(const Container &container)
		{
			ReverseBidirectionalManipulator manipulator(container);
			manipulator.toBegin();
			return manipulator;
		}

		static ReverseBidirectionalManipulator end(const Container &container)
		{
			ReverseBidirectionalManipulator manipulator(container);
			manipulator.toEnd();
			return manipulator;
		}

		static ReverseBidirectionalManipulator rbegin(const Container &container)
		{
			ReverseBidirectionalManipulator manipulator(container);
			manipulator.toRBegin();
			return manipulator;
		}

		static ReverseBidirectionalManipulator rend(const Container &container)
		{
			ReverseBidirectionalManipulator manipulator(container);
			manipulator.toREnd();
			return manipulator;
		}
		
		protected:

		ReverseBidirectionalManipulator(const Container &container)
			:	BidirectionalManipulator<Container, DataType, Position, Traits>(container)
		{
		}

	};


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
			:	BidirectionalIterator<Container, DataType, Position, Traits>()
		{
		}

		/**
		*/
		ReverseBidirectionalIterator(const ReverseBidirectionalIterator &iterator)
			:	BidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		ReverseBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	BidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{	
		}
		//@}

		bool operator + () const
		{
			return traits_->isValid();
		}

		bool operator - () const
		{
			return (bool)(traits_->isValid() == false);
		}

		void toBegin()
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::toRBegin();
		}

		bool isBegin() const
		{
			return BidirectionalIterator<Container, DataType, Position, Traits>::isRBegin();
		}

		void toEnd()
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::toREnd();
		}

		bool isEnd() const
		{
			return BidirectionalIterator<Container, DataType, Position, Traits>::isREnd();
		}

		void toRBegin()
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::toBegin();
		}

		bool isRBegin() const
		{
			return BidirectionalIterator<Container, DataType, Position, Traits>::isBegin();
		}

		void toREnd()
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::toEnd();
		}

		bool isREnd() const
		{
			return BidirectionalIterator<Container, DataType, Position, Traits>::isEnd();
		}

		ReverseBidirectionalIterator &operator ++ ()
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::operator--();
			return *this;
		}

		ReverseBidirectionalIterator operator ++ (int)
		{
			ReverseBidirectionalIterator iterator(*this);
			BidirectionalIterator<Container, DataType, Position, Traits>::operator--();
			return iterator;
		}

		ReverseBidirectionalIterator &operator -- ()
		{
			BidirectionalIterator<Container, DataType, Position, Traits>::operator++();
			return *this;
		}

		ReverseBidirectionalIterator operator -- (int)
		{
			ReverseBidirectionalIterator iterator(*this);
			BidirectionalIterator<Container, DataType, Position, Traits>::operator++();
			return iterator;
		}

		DataType *findFirst(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		DataType *findLast(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}
	
		const DataType *findLast(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}

		DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		DataType *findNext(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}
	
		const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}

		static ReverseBidirectionalIterator begin(const Container &container)
		{
			ReverseBidirectionalIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static ReverseBidirectionalIterator end(const Container &container)
		{
			ReverseBidirectionalIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		static ReverseBidirectionalIterator rbegin(const Container &container)
		{
			ReverseBidirectionalIterator iterator(container);
			iterator.toRBegin();
			return iterator;
		}

		static ReverseBidirectionalIterator rend(const Container &container)
		{
			ReverseBidirectionalIterator iterator(container);
			iterator.toREnd();
			return iterator;
		}

		protected:

		ReverseBidirectionalIterator(const Container &container)
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
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>()
		{
		}

		/**
		*/
		ConstReverseBidirectionalIterator(const ConstReverseBidirectionalIterator &iterator)
			: ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		ConstReverseBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + () const
		{
			return traits_->isValid();
		}

		bool operator - () const
		{
			return (bool)(traits_->isValid() == false);
		}

		void toBegin()
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin();
		}

		bool isBegin() const
		{
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isRBegin();
		}

		void toEnd()
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::toREnd();
		}

		bool isEnd() const
		{
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isREnd();
		}

		void toRBegin()
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::toBegin();
		}

		bool isRBegin() const
		{
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isBegin();
		}

		void toREnd()
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::toEnd();
		}

		bool isREnd() const
		{
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isEnd();
		}

		ConstReverseBidirectionalIterator &operator ++ ()
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator--();
			return *this;
		}

		ConstReverseBidirectionalIterator operator ++ (int)
		{
			ConstReverseBidirectionalIterator iterator(*this);
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator--();
			return iterator;
		}

		ConstReverseBidirectionalIterator &operator -- ()
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator++();
			return *this;
		}

		ConstReverseBidirectionalIterator operator -- (int)
		{
			ConstReverseBidirectionalIterator iterator(*this);
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator++();
			return iterator;
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType *findLast(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}
	
		const DataType *findLast(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}

		const DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType *findNext(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}
	
		const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}

		static ConstReverseBidirectionalIterator begin(const Container &container)
		{
			ConstReverseBidirectionalIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static ConstReverseBidirectionalIterator end(const Container &container)
		{
			ConstReverseBidirectionalIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		static ConstReverseBidirectionalIterator rbegin(const Container &container)
		{
			ConstReverseBidirectionalIterator iterator(container);
			iterator.toRBegin();
			return iterator;
		}

		static ConstReverseBidirectionalIterator rend(const Container &container)
		{
			ConstReverseBidirectionalIterator iterator(container);
			iterator.toREnd();
			return iterator;
		}

		protected:

		ConstReverseBidirectionalIterator(const Container &container)
			:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(container)
		{
		}

	};

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H
