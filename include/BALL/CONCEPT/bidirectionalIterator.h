// $Id: bidirectionalIterator.h,v 1.3 2000/01/03 15:15:32 oliver Exp $ 

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
	*/
	template <class Container, class DataType, class Position, class Traits>
	class BidirectionalIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

			BidirectionalIterator()
				: BaseIterator<Container, DataType, Position, Traits>()
			{
			}
		
			BidirectionalIterator(const BidirectionalIterator &iterator)
				:	BaseIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			BidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
				:	BaseIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			bool operator + () const
			{
				return traits_ptr_->isValid();
			}

			void toBegin()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toBegin();
			}

			bool isBegin() const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isBegin();
			}

			void toEnd()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toEnd();
			}

			bool isEnd() const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isEnd();
			}

			void toRBegin()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toRBegin();
			}

			bool isRBegin() const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);

				return traits_ptr_->isRBegin();
			}

			void toREnd()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toREnd();
			}

			bool isREnd() const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isREnd();
			}

			BidirectionalIterator& operator ++ ()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->forward();
				return *this;
			}

			BidirectionalIterator operator ++ (int)
			{
				BidirectionalIterator iterator(*this);
				++(*this);
				return iterator;
			}

			BidirectionalIterator &operator --
				()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->backward();
				return *this;
			}

			BidirectionalIterator operator --
				(int)
			{
				BidirectionalIterator iterator(*this);
				--(*this);
				return iterator;
			}

			DataType* findFirst
				(const UnaryPredicate<DataType> &predicate)
			{
        if (!traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				for(traits_ptr_->toBegin(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
					if (predicate((const DataType &)traits_ptr_->getData()) == true)
						return (DataType *)&(traits_ptr_->getData());

				return 0;
			}
		
			const DataType* findFirst(const UnaryPredicate<DataType> &predicate) const
			{
				return ((BidirectionalIterator *)this)->findFirst(predicate);
			}

			DataType* findLast(const UnaryPredicate<DataType> &predicate)
			{
        if (!traits_ptr_->isValid())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				for(traits_ptr_->toRBegin(); traits_ptr_->isREnd() == false; traits_ptr_->backward())
					if (predicate((const DataType &)traits_ptr_->getData()) == true)
						return (DataType *)&(traits_ptr_->getData());

				return 0;
			}
		
			const DataType* findLast(const UnaryPredicate<DataType> &predicate) const
			{
				return ((BidirectionalIterator *)this)->findLast(predicate);
			}

			DataType* findNext(const UnaryPredicate<DataType> &predicate)
			{
				if (!traits_ptr_->isValid())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				for(traits_ptr_->forward(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
					if (predicate((const DataType &)traits_ptr_->getData()) == true)
						return (DataType *)&(traits_ptr_->getData());

				return 0;
			}
		
			const DataType* findNext(const UnaryPredicate<DataType> &predicate) const
			{
				return ((BidirectionalIterator *)this)->findNext(predicate);
			}

			DataType* findPrevious(const UnaryPredicate<DataType> &predicate)
			{
        if (!traits_ptr_->isValid())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				for(traits_ptr_->backward(); traits_ptr_->isREnd() == false; traits_ptr_->backward())
					if (predicate((const DataType &)traits_ptr_->getData()) == true)
						return (DataType *)&(traits_ptr_->getData());

				return 0;
			}
		
			const DataType* findPrevious(const UnaryPredicate<DataType> &predicate) const
			{
				return ((BidirectionalIterator *)this)->findPrevious(predicate);
			}

			static BidirectionalIterator begin(const Container &container)
			{
				BidirectionalIterator iterator(container);
				iterator.toBegin();
				return iterator;
			}

			static BidirectionalIterator end(const Container &container)
			{
				BidirectionalIterator iterator(container);
				iterator.toEnd();
				return iterator;
			}

			static BidirectionalIterator rbegin(const Container &container)
			{
				BidirectionalIterator iterator(container);
				iterator.toRBegin();
				return iterator;
			}

			static BidirectionalIterator rend(const Container &container)
			{
				BidirectionalIterator iterator(container);
				iterator.toREnd();
				return iterator;
			}

		protected:

			BidirectionalIterator(const Container &container)
				:	BaseIterator<Container, DataType, Position, Traits>(container)
			{
			}

		private:
	};


	/**	Constant Bidirectional Iterator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ConstBidirectionalIterator
		: public ConstBaseIterator<Container, DataType, Position, Traits>
	{
		public:

			ConstBidirectionalIterator()
				:	ConstBaseIterator<Container, DataType, Position, Traits>()
			{
			}
		
			ConstBidirectionalIterator(const ConstBidirectionalIterator &iterator)
				:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			ConstBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
				:	ConstBaseIterator <Container, DataType, Position, Traits>(iterator)
			{
			}

			bool operator +() const
			{
				return traits_ptr_->isValid();
			}

			void toBegin()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toBegin();
			}

			bool isBegin() const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isBegin();
			}

			void toEnd()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toEnd();
			}

			bool isEnd() const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isEnd();
			}

			void toRBegin()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toRBegin();
			}

			bool isRBegin() const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isRBegin();
			}

			void toREnd()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toREnd();
			}

			bool isREnd() const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isREnd();
			}

			ConstBidirectionalIterator &operator ++ ()
			{
        if (!traits_ptr_->isValid())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->forward();
				return *this;
			}

			ConstBidirectionalIterator operator ++ (int)
			{
				ConstBidirectionalIterator iterator(*this);
				++(*this);
				return iterator;
			}

			ConstBidirectionalIterator &operator -- ()
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->backward();
				return *this;
			}

			ConstBidirectionalIterator operator -- (int)
			{
				ConstBidirectionalIterator iterator(*this);
				--(*this);
				return iterator;
			}

			const DataType* findFirst(const UnaryPredicate<DataType> &predicate)
			{
        if (!traits_ptr_->isValid())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				for(traits_ptr_->toBegin(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
					if (predicate((const DataType &)traits_ptr_->getData()) == true)
						return (const DataType *)&(traits_ptr_->getData());

				return 0;
			}
		
			const DataType* findFirst(const UnaryPredicate<DataType> &predicate) const
			{
				return ((ConstBidirectionalIterator *)this)->findFirst(predicate);
			}

			const DataType* findLast(const UnaryPredicate<DataType> &predicate)
			{
        if (!traits_ptr_->isValid())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				for(traits_ptr_->toRBegin(); traits_ptr_->isREnd() == false; traits_ptr_->backward())
					if (predicate((const DataType &)traits_ptr_->getData()) == true)
						return (const DataType *)&(traits_ptr_->getData());

				return 0;
			}
		
			const DataType* findLast(const UnaryPredicate<DataType> &predicate) const
			{
				return ((ConstBidirectionalIterator *)this)->findLast(predicate);
			}

			const DataType* findNext(const UnaryPredicate<DataType> &predicate)
			{
        if (!traits_ptr_->isValid())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				for(traits_ptr_->forward(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
					if (predicate((const DataType &)traits_ptr_->getData()) == true)
						return (const DataType *)&(traits_ptr_->getData());

				return 0;
			}
		
			const DataType* findNext(const UnaryPredicate<DataType> &predicate) const
			{
				return ((ConstBidirectionalIterator *)this)->findNext(predicate);
			}

			const DataType* findPrevious(const UnaryPredicate<DataType> &predicate)
			{
        if (!traits_ptr_->isValid())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				for(traits_ptr_->backward(); traits_ptr_->isREnd() == false; traits_ptr_->backward())
					if (predicate((const DataType &)traits_ptr_->getData()) == true)
						return (const DataType *)&(traits_ptr_->getData());

				return 0;
			}
		
			const DataType* findPrevious(const UnaryPredicate<DataType> &predicate) const
			{
				return ((ConstBidirectionalIterator *)this)->findPrevious(predicate);
			}

			static ConstBidirectionalIterator begin(const Container &container)
			{
				ConstBidirectionalIterator iterator(container);
				iterator.toBegin();
				return iterator;
			}

			static ConstBidirectionalIterator end(const Container& container)
			{
				ConstBidirectionalIterator iterator(container);
				iterator.toEnd();
				return iterator;
			}

			static ConstBidirectionalIterator rbegin(const Container& container)
			{
				ConstBidirectionalIterator iterator(container);
				iterator.toRBegin();
				return iterator;
			}

			static ConstBidirectionalIterator rend(const Container& container)
			{
				ConstBidirectionalIterator iterator(container);
				iterator.toREnd();
				return iterator;
			}

		protected:

			ConstBidirectionalIterator(const Container& container)
				:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
			{
			}

		private:
	};

	//@}

} // namespace BALL 

#endif // BALL_CONCEPT_BIDIRECTIONALITERATOR_H
