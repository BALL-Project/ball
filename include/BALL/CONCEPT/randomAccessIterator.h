// $Id: randomAccessIterator.h,v 1.3 2000/01/03 15:15:34 oliver Exp $ 

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#define BALL_CONCEPT_RANDOMACCESSITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_BASEITERATOR_H
#	include <BALL/CONCEPT/baseIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL 
{

	/**	@name	Generic Random Access Iterator Classes.
			{\bf Definition:} \URL{BALL/CONCEPT/randomAccessIterator.h}
	*/
	//@{

	/**	Generic Random Access Iterator Class
	*/
	template <class Container, class DataType, class Position, class Traits>
	class RandomAccessIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{

		/**
		*/
		RandomAccessIterator()
			:	BaseIterator<Container, DataType, Position, Traits>()
		{
		}
	
		/**
		*/
		RandomAccessIterator(const RandomAccessIterator &iterator)
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		RandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + () const
		{
			return traits_ptr_->isValid();
		}

		bool operator - () const
		{
			return (bool)(traits_ptr_->isValid() == false);
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

		RandomAccessIterator &operator ++ ()
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			traits_ptr_->forward();
			return *this;
		}

		RandomAccessIterator operator ++ (int)
		{
			RandomAccessIterator iterator(*this);
			++(*this);
			return iterator;
		}

		RandomAccessIterator &operator -- ()
		{
			if (traits_ptr_->isSingular())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			traits_ptr_->backward();
			return *this;
		}

		RandomAccessIterator operator -- (int)
		{
			RandomAccessIterator iterator(*this);
			--(*this);
			return iterator;
		}

		RandomAccessIterator &operator += (Distance distance)
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			if (distance < (Distance)0)
				return (*this -= -distance);

			traits_ptr_->forward(distance);
			return *this;
		}

		RandomAccessIterator &operator -= (Distance distance)
		{
			if (traits_ptr_->isSingular())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			if (distance < (Distance)0)
				return (*this += -distance);

			if (traits_ptr_->isEnd() == true)
			{
				traits_ptr_->toRBegin();
				traits_ptr_->backward(distance - 1);
			} else {
				traits_ptr_->backward(distance);
			}
			return *this;
		}

		RandomAccessIterator operator + (Distance distance) const
		{
			RandomAccessIterator iterator(*this);
			return (iterator += distance);
		}

		friend RandomAccessIterator operator +
			(Distance distance, const RandomAccessIterator &iterator)
		{
			RandomAccessIterator tmp_iterator(iterator);
			return (tmp_iterator += distance);
		}

		RandomAccessIterator operator - (Distance distance) const
		{
			RandomAccessIterator iterator(*this);
			return (iterator -= distance);
		}

		friend Distance operator -
			(const RandomAccessIterator &a, const RandomAccessIterator &b)
		{
			if (!a.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!b.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (a.traits_ptr_->getContainer() != b.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);
 
			return a.traits_ptr_->getDistance
				 (*(b.traits_ptr_));
		}

		bool operator < (const RandomAccessIterator &iterator) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!iterator.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);

			return (bool)(*traits_ptr_ < *iterator.traits_ptr_);
		}

		bool operator <= (const RandomAccessIterator &iterator) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!iterator.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);

			return (bool)!(*iterator.traits_ptr_ < *traits_ptr_);
		}

		bool operator >= (const RandomAccessIterator &iterator) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!iterator.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);

			return (bool)!(*traits_ptr_ < *iterator.traits_ptr_);
		}

		bool operator > (const RandomAccessIterator &iterator) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!iterator.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);

			return (bool)(*iterator.traits_ptr_ < *traits_ptr_);
		}

		DataType* findFirst(const UnaryPredicate<DataType> &predicate)
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			for(traits_ptr_->toBegin(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
				if (predicate((const DataType &)traits_ptr_->getData()) == true)
					return (DataType *)&(traits_ptr_->getData());

			return 0;
		}
	
		const DataType* findFirst(const UnaryPredicate<DataType> &predicate) const
		{
			return ((RandomAccessIterator *)this)->findFirst(predicate);
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
			return ((RandomAccessIterator *)this)->findLast(predicate);
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
			return ((RandomAccessIterator *)this)->findNext(predicate);
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
			return ((RandomAccessIterator *)this)->findPrevious(predicate);
		}

		DataType &operator [] (Index index)
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			return (DataType &)traits_ptr_->getData(index);
		}

		const DataType &operator [] (Index index) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			return (const DataType &)traits_ptr_->getData(index);
		}

		static RandomAccessIterator begin(const Container& container)
		{
			RandomAccessIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static RandomAccessIterator end(const Container& container)
		{
			RandomAccessIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		static RandomAccessIterator rbegin(const Container& container)
		{
			RandomAccessIterator iterator(container);
			iterator.toRBegin();
			return iterator;
		}

		static RandomAccessIterator rend(const Container& container)
		{
			RandomAccessIterator iterator(container);
			iterator.toREnd();
			return iterator;
		}

		protected:

		RandomAccessIterator(const Container& container)
			:	BaseIterator<Container, DataType, Position, Traits>(container)
		{
		}

	};


	/**	Generic Constant Random Access Iterator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ConstRandomAccessIterator
		: public ConstBaseIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**
		*/
		ConstRandomAccessIterator()
			:	ConstBaseIterator<Container, DataType, Position, Traits>()
		{
		}
	
		/**
		*/
		ConstRandomAccessIterator(const ConstRandomAccessIterator &iterator)
			:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		ConstRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + () const
		{
			return traits_ptr_->isValid();
		}

		bool operator - () const
		{
			return (bool)(traits_ptr_->isValid() == false);
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

		ConstRandomAccessIterator &operator ++ ()
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			traits_ptr_->forward();
			return *this;
		}

		ConstRandomAccessIterator operator ++ (int)
		{
			ConstRandomAccessIterator iterator(*this);
			++(*this);
			return iterator;
		}

		ConstRandomAccessIterator &operator -- ()
		{
			if (traits_ptr_->isSingular())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			traits_ptr_->backward();
			return *this;
		}

		ConstRandomAccessIterator operator -- (int)
		{
			ConstRandomAccessIterator iterator(*this);
			--(*this);
			return iterator;
		}

		ConstRandomAccessIterator &operator += (Distance distance)
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			if (distance < (Distance)0)
				return (*this -= -distance);

			traits_ptr_->forward(distance);
			return *this;
		}

		ConstRandomAccessIterator &operator -= (Distance distance)
		{
			if (traits_ptr_->isSingular())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
	
			if (distance < (Distance)0)
				return (*this += -distance);

			if (traits_ptr_->isEnd() == true)
			{
				traits_ptr_->toRBegin();
				traits_ptr_->backward(distance - 1);
			} else {
				traits_ptr_->backward(distance);
			}
			return *this;
		}

		ConstRandomAccessIterator operator + (Distance distance) const
		{
			ConstRandomAccessIterator iterator(*this);
			return (iterator += distance);
		}

		friend ConstRandomAccessIterator operator +
			(Distance distance, const ConstRandomAccessIterator &iterator)
		{
			ConstRandomAccessIterator tmp_iterator(iterator);
			return (tmp_iterator += distance);
		}

		ConstRandomAccessIterator operator - (Distance distance) const
		{
			ConstRandomAccessIterator iterator(*this);
			return (iterator -= distance);
		}

		friend Distance operator -
			(const ConstRandomAccessIterator &a, const ConstRandomAccessIterator &b)
		{
			if (!a.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!b.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (a.traits_ptr_->getContainer() != b.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);

			return a.traits_ptr_->getDistance(*(b.traits_ptr_));
		}

		bool operator < (const ConstRandomAccessIterator &iterator) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!iterator.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);				
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);

			return (bool)(*traits_ptr_ < *iterator.traits_ptr_);
		}

		bool operator <= (const ConstRandomAccessIterator &iterator) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!iterator.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);

			return (bool)!(*iterator.traits_ptr_ < *traits_ptr_);
		}

		bool operator >= (const ConstRandomAccessIterator &iterator) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!iterator.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);

			return (bool)!(*traits_ptr_ < *iterator.traits_ptr_);
		}

		bool operator > (const ConstRandomAccessIterator &iterator) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (!iterator.traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);

			return (bool)(*iterator.traits_ptr_ < *traits_ptr_);
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
			return ((ConstRandomAccessIterator *)this)->findFirst(predicate);
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
			return ((ConstRandomAccessIterator *)this)->findLast(predicate);
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
			return ((ConstRandomAccessIterator *)this)->findNext(predicate);
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
			return ((ConstRandomAccessIterator *)this)->findPrevious(predicate);
		}

		const DataType &operator [] (Index index) const
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			return (const DataType &)traits_ptr_->getData(index);
		}

		static ConstRandomAccessIterator begin(const Container& container)
		{
			ConstRandomAccessIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static ConstRandomAccessIterator end(const Container& container)
		{
			ConstRandomAccessIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		static ConstRandomAccessIterator rbegin(const Container& container)
		{
			ConstRandomAccessIterator iterator(container);
			iterator.toRBegin();
			return iterator;
		}

		static ConstRandomAccessIterator rend(const Container& container)
		{
			ConstRandomAccessIterator iterator(container);
			iterator.toREnd();
			return iterator;
		}

		protected:

		ConstRandomAccessIterator(const Container& container)
			:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
		{
		}

	};

	//@}

} // namespace BALL 

#endif // BALL_CONCEPT_RANDOMACCESSITERATOR_H
