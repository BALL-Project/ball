// $Id: bidirectionalIterator.h,v 1.1 1999/08/26 07:53:16 oliver Exp $ 

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#define BALL_CONCEPT_BIDIRECTIONALITERATOR_H

#ifndef BALL_CONCEPT_BASEITERATOR_H
#	include <BALL/CONCEPT/baseIterator.h>
#endif

#ifndef BALL_CONCEPT_BIDIRECTIONALMANIPULATOR_H
#	include <BALL/CONCEPT/bidirectionalManipulator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL {

	/**	@name	Bidirectional Iterators
	*/
	//@{

	/**	Bidirectional Manipulator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class BidirectionalManipulator
		: public BidirectionalManipulator_<Container, DataType, Position, Traits>
	{
		public:

			BidirectionalManipulator(void)
				:	BidirectionalManipulator_<Container, DataType, Position, Traits>()
			{
			}
		
			BidirectionalManipulator(const BidirectionalManipulator &manipulator)
				:	BidirectionalManipulator_<Container, DataType, Position, Traits>(manipulator)
			{
			}

			BidirectionalManipulator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
				:	BidirectionalManipulator_<Container, DataType, Position, Traits>(iterator)
			{
			}

			bool operator + (void) const
			{
				return traits_ptr_->isValid();
			}

			bool operator -(void) const
			{
				return (bool)(traits_ptr_->isValid() == false);
			}

			void toBegin(void)
			{
				if (traits_ptr_->isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				traits_ptr_->toBegin();
			}

			bool isBegin(void) const
			{	
				if (traits_ptr_->isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				return traits_ptr_->isBegin();
			}

			void toEnd(void)
			{
				if (traits_ptr_->isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				traits_ptr_->toEnd();
			}

			bool isEnd(void) const
			{
				if (traits_ptr_->isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				return traits_ptr_->isEnd();
			}

			void toRBegin(void)
			{
				if (traits_ptr_->isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				traits_ptr_->toRBegin();
			}

			bool isRBegin(void) const
			{
				if (traits_ptr_->isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				return traits_ptr_->isRBegin();
			}

			void toREnd(void)
			{
				if (traits_ptr_->isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				traits_ptr_->toREnd();
			}

			bool isREnd(void) const
			{
				if (traits_ptr_->isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				return traits_ptr_->isREnd();
			}

			BidirectionalManipulator &operator ++ (void)
			{
				if (!traits_ptr_->isValid())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				traits_ptr_->forward();
				return *this;
			}

			BidirectionalManipulator operator ++ (int)
			{
				BidirectionalManipulator manipulator(*this);
				++(*this);
				return manipulator;
			}

			BidirectionalManipulator &operator -- (void)
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->backward();
				return *this;
			}

			BidirectionalManipulator operator -- (int)
			{
				BidirectionalManipulator manipulator(*this);
				--(*this);
				return manipulator;
			}

			DataType* findFirst(const UnaryPredicate<DataType> &predicate)
			{
        if (!traits_ptr_->isValid())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				for(traits_ptr_->toBegin(); traits_ptr_->isEnd() == false; traits_ptr_->forward()) {
					if (predicate((const DataType &)traits_ptr_->getData()) == true) {
						return (DataType *)&(traits_ptr_->getData());
					}
				}

				return 0;
			}
		
			const DataType* findFirst(const UnaryPredicate<DataType> &predicate) const
			{
				return ((BidirectionalManipulator *)this)->findFirst(predicate);
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
				return ((BidirectionalManipulator *)this)->findLast(predicate);
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
				return ((BidirectionalManipulator *)this)->findNext(predicate);
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
				return ((BidirectionalManipulator *)this)->findPrevious(predicate);
			}

			static BidirectionalManipulator begin(const Container &container)
			{
				BidirectionalManipulator manipulator(container);
				manipulator.toBegin();
				return manipulator;
			}

			static BidirectionalManipulator end(const Container &container)
			{
				BidirectionalManipulator manipulator(container);
				manipulator.toEnd();
				return manipulator;
			}

			static BidirectionalManipulator rbegin(const Container &container)
			{
				BidirectionalManipulator manipulator(container);
				manipulator.toRBegin();
				return manipulator;
			}

			static BidirectionalManipulator rend(const Container &container)
			{
				BidirectionalManipulator manipulator(container);
				manipulator.toREnd();
				return manipulator;
			}

		protected:

			BidirectionalManipulator(const Container &container)
				:	BidirectionalManipulator_<Container, DataType, Position, Traits>(container)
			{
			}

		private:
	};



	/**	@name Bidirectional Iterator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class BidirectionalIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

			BidirectionalIterator(void)
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

			bool operator + (void) const
			{
				return traits_ptr_->isValid();
			}

			void toBegin(void)
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toBegin();
			}

			bool isBegin(void) const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isBegin();
			}

			void toEnd(void)
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toEnd();
			}

			bool isEnd(void) const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isEnd();
			}

			void toRBegin(void)
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toRBegin();
			}

			bool isRBegin(void) const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);

				return traits_ptr_->isRBegin();
			}

			void toREnd(void)
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toREnd();
			}

			bool isREnd(void) const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isREnd();
			}

			BidirectionalIterator& operator ++ (void)
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
				(void)
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

			ConstBidirectionalIterator(void)
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

			bool operator +(void) const
			{
				return traits_ptr_->isValid();
			}

			void toBegin(void)
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toBegin();
			}

			bool isBegin(void) const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isBegin();
			}

			void toEnd(void)
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toEnd();
			}

			bool isEnd(void) const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isEnd();
			}

			void toRBegin(void)
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toRBegin();
			}

			bool isRBegin(void) const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isRBegin();
			}

			void toREnd(void)
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				traits_ptr_->toREnd();
			}

			bool isREnd(void) const
			{
        if (traits_ptr_->isSingular())
          throw Exception::InvalidIterator(__FILE__, __LINE__);
 
				return traits_ptr_->isREnd();
			}

			ConstBidirectionalIterator &operator ++ (void)
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

			ConstBidirectionalIterator &operator -- (void)
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
