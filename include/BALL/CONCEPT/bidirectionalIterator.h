// $Id: bidirectionalIterator.h,v 1.5 2000/12/19 12:50:48 amoll Exp $ 

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
	template <typename Container, typename DataType, typename Position, typename Traits>
	class BidirectionalIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

			BidirectionalIterator()
				throw()
				: BaseIterator<Container, DataType, Position, Traits>()
			{
			}
		
			BidirectionalIterator(const BidirectionalIterator& iterator)
				throw()
				:	BaseIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			BidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
				throw()
				:	BaseIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			bool operator + () const
				throw()
			{
				return traits_ptr_->isValid();
			}

			void toBegin()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->toBegin();
			}

			bool isBegin() const
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				return traits_ptr_->isBegin();
			}

			void toEnd()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->toEnd();
			}

			bool isEnd() const
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				return traits_ptr_->isEnd();
			}

			void toRBegin()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->toRBegin();
			}

			bool isRBegin() const
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}

				return traits_ptr_->isRBegin();
			}

			void toREnd()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->toREnd();
			}

			bool isREnd() const
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				return traits_ptr_->isREnd();
			}

			BidirectionalIterator& operator ++ ()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
         throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->forward();
				return *this;
			}

			BidirectionalIterator operator ++ (int)
				throw(Exception::InvalidIterator)
			{
				BidirectionalIterator iterator(*this);
				++(*this);
				return iterator;
			}

			BidirectionalIterator& operator -- ()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
         throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->backward();
				return *this;
			}

			BidirectionalIterator operator -- (int)
				throw(Exception::InvalidIterator)
			{
				BidirectionalIterator iterator(*this);
				--(*this);
				return iterator;
			}

			DataType* findFirst(const UnaryPredicate<DataType>& predicate)
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
		
			const DataType* findFirst(const UnaryPredicate<DataType>& predicate) const
				throw(Exception::InvalidIterator)
			{
				return ((BidirectionalIterator *)this)->findFirst(predicate);
			}

			DataType* findLast(const UnaryPredicate<DataType> &predicate)
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
		
			const DataType* findLast(const UnaryPredicate<DataType>& predicate) const
				throw(Exception::InvalidIterator)
			{
				return ((BidirectionalIterator *)this)->findLast(predicate);
			}

			DataType* findNext(const UnaryPredicate<DataType>& predicate)
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
		
			const DataType* findNext(const UnaryPredicate<DataType>& predicate) const
				throw(Exception::InvalidIterator)
			{
				return ((BidirectionalIterator *)this)->findNext(predicate);
			}

			DataType* findPrevious(const UnaryPredicate<DataType>& predicate)
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
		
			const DataType* findPrevious(const UnaryPredicate<DataType>& predicate) const
				throw(Exception::InvalidIterator)
			{
				return ((BidirectionalIterator *)this)->findPrevious(predicate);
			}

			static BidirectionalIterator begin(const Container& container)
				throw(Exception::InvalidIterator)
			{
				BidirectionalIterator iterator(container);
				iterator.toBegin();
				return iterator;
			}

			static BidirectionalIterator end(const Container& container)
				throw(Exception::InvalidIterator)
			{
				BidirectionalIterator iterator(container);
				iterator.toEnd();
				return iterator;
			}

			static BidirectionalIterator rbegin(const Container& container)
				throw(Exception::InvalidIterator)
			{
				BidirectionalIterator iterator(container);
				iterator.toRBegin();
				return iterator;
			}

			static BidirectionalIterator rend(const Container &container)
				throw(Exception::InvalidIterator)
			{
				BidirectionalIterator iterator(container);
				iterator.toREnd();
				return iterator;
			}

		protected:

			BidirectionalIterator(const Container& container)
				throw()
				:	BaseIterator<Container, DataType, Position, Traits>(container)
			{
			}

	};


	/**	Constant Bidirectional Iterator
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstBidirectionalIterator
		: public ConstBaseIterator<Container, DataType, Position, Traits>
	{
		public:

			ConstBidirectionalIterator()
				throw()
				:	ConstBaseIterator<Container, DataType, Position, Traits>()
			{
			}
		
			ConstBidirectionalIterator(const ConstBidirectionalIterator& iterator)
				throw()
				:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			ConstBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
				throw()
				:	ConstBaseIterator <Container, DataType, Position, Traits>(iterator)
			{
			}

			bool operator + () const
				throw(Exception::InvalidIterator)
			{
				return traits_ptr_->isValid();
			}

			void toBegin()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->toBegin();
			}

			bool isBegin() const
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				return traits_ptr_->isBegin();
			}

			void toEnd()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->toEnd();
			}

			bool isEnd() const
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				return traits_ptr_->isEnd();
			}

			void toRBegin()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->toRBegin();
			}

			bool isRBegin() const
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				return traits_ptr_->isRBegin();
			}

			void toREnd()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->toREnd();
			}

			bool isREnd() const
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				return traits_ptr_->isREnd();
			}

			ConstBidirectionalIterator& operator ++ ()
				throw(Exception::InvalidIterator)
			{
        if (!traits_ptr_->isValid())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
 
				traits_ptr_->forward();
				return *this;
			}

			ConstBidirectionalIterator operator ++ (int)
				throw(Exception::InvalidIterator)
			{
				ConstBidirectionalIterator iterator(*this);
				++(*this);
				return iterator;
			}

			ConstBidirectionalIterator& operator -- ()
				throw(Exception::InvalidIterator)
			{
        if (traits_ptr_->isSingular())
				{
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
				traits_ptr_->backward();
				return *this;
			}

			ConstBidirectionalIterator operator -- (int)
				throw(Exception::InvalidIterator)
			{
				ConstBidirectionalIterator iterator(*this);
				--(*this);
				return iterator;
			}

			const DataType* findFirst(const UnaryPredicate<DataType>& predicate)
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
		
			const DataType* findFirst(const UnaryPredicate<DataType>& predicate) const
				throw(Exception::InvalidIterator)
			{
				return ((ConstBidirectionalIterator *)this)->findFirst(predicate);
			}

			const DataType* findLast(const UnaryPredicate<DataType>& predicate)
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
		
			const DataType* findLast(const UnaryPredicate<DataType>& predicate) const
				throw(Exception::InvalidIterator)
			{
				return ((ConstBidirectionalIterator *)this)->findLast(predicate);
			}

			const DataType* findNext(const UnaryPredicate<DataType>& predicate)
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
		
			const DataType* findNext(const UnaryPredicate<DataType>& predicate) const
				throw(Exception::InvalidIterator)
			{
				return ((ConstBidirectionalIterator *)this)->findNext(predicate);
			}

			const DataType* findPrevious(const UnaryPredicate<DataType>& predicate)
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
		
			const DataType* findPrevious(const UnaryPredicate<DataType>& predicate) const
				throw(Exception::InvalidIterator)
			{
				return ((ConstBidirectionalIterator *)this)->findPrevious(predicate);
			}

			static ConstBidirectionalIterator begin(const Container &container)
				throw(Exception::InvalidIterator)
			{
				ConstBidirectionalIterator iterator(container);
				iterator.toBegin();
				return iterator;
			}

			static ConstBidirectionalIterator end(const Container& container)
				throw(Exception::InvalidIterator)
			{
				ConstBidirectionalIterator iterator(container);
				iterator.toEnd();
				return iterator;
			}

			static ConstBidirectionalIterator rbegin(const Container& container)
				throw(Exception::InvalidIterator)
			{
				ConstBidirectionalIterator iterator(container);
				iterator.toRBegin();
				return iterator;
			}

			static ConstBidirectionalIterator rend(const Container& container)
				throw(Exception::InvalidIterator)
			{
				ConstBidirectionalIterator iterator(container);
				iterator.toREnd();
				return iterator;
			}

		protected:

			ConstBidirectionalIterator(const Container& container)
				throw()
				:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
			{
			}

	};

	//@}

} // namespace BALL 

#endif // BALL_CONCEPT_BIDIRECTIONALITERATOR_H
