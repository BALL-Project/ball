// $Id: randomAccessIterator.h,v 1.5 2001/06/07 12:45:35 anker Exp $ 

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
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/randomAccessIterator.h}
	*/
	//@{

	/**	Generic Random Access Iterator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class RandomAccessIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(RandomAccessIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a random access iterator according to the STL
				scheme.
		 */
		typedef ::std::random_access_iterator_tag iterator_category;

		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		/** Default constructor
		*/
		RandomAccessIterator()
			throw();
	
		/**
		*/
		RandomAccessIterator(const RandomAccessIterator &iterator)
			throw();

		/**
		*/
		RandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw();

		/** Destructor
		 */
		~RandomAccessIterator()
			throw();

		//@}

		/** @name Iterator methods
		 */
		//@{

		bool operator + () const
			throw();

		bool operator - () const
			throw();

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

		RandomAccessIterator &operator ++ ()
			throw(Exception::InvalidIterator);

		RandomAccessIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		RandomAccessIterator &operator -- ()
			throw(Exception::InvalidIterator);

		RandomAccessIterator operator -- (int)
			throw(Exception::InvalidIterator);

		RandomAccessIterator &operator += (Distance distance)
			throw(Exception::InvalidIterator);

		RandomAccessIterator &operator -= (Distance distance)
			throw(Exception::InvalidIterator);

		RandomAccessIterator operator + (Distance distance) const
			throw(Exception::InvalidIterator);

		friend RandomAccessIterator operator +
			(Distance distance, const RandomAccessIterator &iterator)
			throw(Exception::InvalidIterator)
		{
			RandomAccessIterator tmp_iterator(iterator);
			return (tmp_iterator += distance);
		}

		RandomAccessIterator operator - (Distance distance) const
			throw(Exception::InvalidIterator);

		friend Distance operator -
			(const RandomAccessIterator &a, const RandomAccessIterator &b)
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
		{
			if (!a.traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
			if (!b.traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
			if (a.traits_ptr_->getContainer() != b.traits_ptr_->getContainer())
			{
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);
			}
			return a.traits_ptr_->getDistance(*(b.traits_ptr_));
		}

		bool operator < (const RandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		bool operator <= (const RandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		bool operator >= (const RandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		bool operator > (const RandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		DataType* findFirst(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findFirst(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator);

		DataType* findLast(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findLast(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator);

		DataType* findNext(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findNext(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator);

		DataType* findPrevious(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findPrevious(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator);

		DataType &operator [] (Index index)
			throw(Exception::InvalidIterator);

		const DataType &operator [] (Index index) const
			throw(Exception::InvalidIterator);

		static RandomAccessIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		static RandomAccessIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		static RandomAccessIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		static RandomAccessIterator rend(const Container& container)
			throw(Exception::InvalidIterator);

		//@}

		protected:

			/*_
			 */
		RandomAccessIterator(const Container& container)
			throw();

	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::RandomAccessIterator()
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::RandomAccessIterator(const RandomAccessIterator &iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::RandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::~RandomAccessIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return traits_ptr_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator - () const
		throw()
	{
		return (traits_ptr_->isValid() == false);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void RandomAccessIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void RandomAccessIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void RandomAccessIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void RandomAccessIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		traits_ptr_->toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>& RandomAccessIterator<Container, DataType, Position, Traits>::operator ++ ()
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
	RandomAccessIterator<Container, DataType, Position, Traits> RandomAccessIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>& RandomAccessIterator<Container, DataType, Position, Traits>::operator -- ()
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
	RandomAccessIterator<Container, DataType, Position, Traits> RandomAccessIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>& RandomAccessIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		if (distance < (Distance)0)
		{
			return (*this -= -distance);
		}

		traits_ptr_->forward(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>& RandomAccessIterator<Container, DataType, Position, Traits>::operator -= (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		if (distance < (Distance)0)
		{
			return (*this += -distance);
		}

		if (traits_ptr_->isEnd() == true)
		{
			traits_ptr_->toRBegin();
			traits_ptr_->backward(distance - 1);
		}
		else 
		{
			traits_ptr_->backward(distance);
		}
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> RandomAccessIterator<Container, DataType, Position, Traits>::operator + (Distance distance) const
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(*this);
		return (iterator += distance);
	}

	/* BAUSTELLE
	template <typename Container, typename DataType, typename Position, typename Traits>
	friend RandomAccessIterator<Container, DataType, Position, Traits> RandomAccessIterator<Container, DataType, Position, Traits>::operator + (Distance distance, const RandomAccessIterator &iterator)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator tmp_iterator(iterator);
		return (tmp_iterator += distance);
	}
	*/

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> RandomAccessIterator<Container, DataType, Position, Traits>::operator - (Distance distance) const
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(*this);
		return (iterator -= distance);
	}

	/* BAUSTELLE
	template <typename Container, typename DataType, typename Position, typename Traits>
	friend Distance RandomAccessIterator<Container, DataType, Position, Traits>::operator - (const RandomAccessIterator &a, const RandomAccessIterator &b)
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!a.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!b.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (a.traits_ptr_->getContainer() != b.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}
		return a.traits_ptr_->getDistance(*(b.traits_ptr_));
	}
	*/

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator < (const RandomAccessIterator &iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (*traits_ptr_ < *iterator.traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator <= (const RandomAccessIterator &iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return !(*iterator.traits_ptr_ > *traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator >= (const RandomAccessIterator &iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return !(*traits_ptr_ < *iterator.traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator > (const RandomAccessIterator &iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (*iterator.traits_ptr_ < *traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* RandomAccessIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType> &predicate)
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
				return (DataType *)&(traits_ptr_->getData());
			}
		}

		return 0;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* RandomAccessIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType> &predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((RandomAccessIterator *)this)->findFirst(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* RandomAccessIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType> &predicate)
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
	const DataType* RandomAccessIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType> &predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((RandomAccessIterator *)this)->findLast(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* RandomAccessIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType> &predicate)
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
	const DataType* RandomAccessIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType> &predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((RandomAccessIterator *)this)->findNext(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* RandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType> &predicate)
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
	const DataType* RandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType> &predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((RandomAccessIterator *)this)->findPrevious(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType& RandomAccessIterator<Container, DataType, Position, Traits>::operator [] (Index index)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (DataType &)traits_ptr_->getData(index);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType& RandomAccessIterator<Container, DataType, Position, Traits>::operator [] (Index index) const
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (const DataType &)traits_ptr_->getData(index);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> RandomAccessIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> RandomAccessIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> RandomAccessIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> RandomAccessIterator<Container, DataType, Position, Traits>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::RandomAccessIterator(const Container& container)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(container)
	{
	}


	/**	Generic Constant Random Access Iterator
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/randomAccessIterator.h}
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstRandomAccessIterator
		: public ConstBaseIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(ConstRandomAccessIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a random access iterator according to the STL
				scheme.
		 */
		typedef ::std::random_access_iterator_tag iterator_category;

		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**
		*/
		ConstRandomAccessIterator()
			throw();
	
		/**
		*/
		ConstRandomAccessIterator(const ConstRandomAccessIterator &iterator)
			throw();

		/**
		*/
		ConstRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw();
		
		/**
		 */
		~ConstRandomAccessIterator()
			throw();

		//@}

		bool operator + () const
			throw();

		bool operator - () const
			throw();

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

		ConstRandomAccessIterator &operator ++ ()
			throw(Exception::InvalidIterator);

		ConstRandomAccessIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		ConstRandomAccessIterator &operator -- ()
			throw(Exception::InvalidIterator);

		ConstRandomAccessIterator operator -- (int)
			throw(Exception::InvalidIterator);

		ConstRandomAccessIterator &operator += (Distance distance)
			throw(Exception::InvalidIterator);

		ConstRandomAccessIterator &operator -= (Distance distance)
			throw(Exception::InvalidIterator);

		ConstRandomAccessIterator operator + (Distance distance) const
			throw(Exception::InvalidIterator);

		friend ConstRandomAccessIterator operator +
			(Distance distance, const ConstRandomAccessIterator &iterator)
			throw(Exception::InvalidIterator)
		{
			ConstRandomAccessIterator tmp_iterator(iterator);
			return (tmp_iterator += distance);
		}

		ConstRandomAccessIterator operator - (Distance distance) const
			throw(Exception::InvalidIterator);

		friend Distance operator -
			(const ConstRandomAccessIterator &a, const ConstRandomAccessIterator &b)
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
		{
			if (!a.traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
			if (!b.traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
			if (a.traits_ptr_->getContainer() != b.traits_ptr_->getContainer())
			{
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);
			}
			return a.traits_ptr_->getDistance(*(b.traits_ptr_));
		}

		bool operator < (const ConstRandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		bool operator <= (const ConstRandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		bool operator >= (const ConstRandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		bool operator > (const ConstRandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		const DataType* findFirst(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findFirst(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator);

		const DataType* findLast(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findLast(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator);

		const DataType* findNext(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findNext(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator);

		const DataType* findPrevious(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findPrevious(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator);

		const DataType &operator [] (Index index) const
			throw(Exception::InvalidIterator);

		static ConstRandomAccessIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		static ConstRandomAccessIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		static ConstRandomAccessIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		static ConstRandomAccessIterator rend(const Container& container)
			throw(Exception::InvalidIterator);

		protected:

		ConstRandomAccessIterator(const Container& container)
			throw();

	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator()
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator(const ConstRandomAccessIterator &iterator)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::~ConstRandomAccessIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return traits_ptr_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator - () const
		throw()
	{
		return (traits_ptr_->isValid() == false);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		traits_ptr_->toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		return traits_ptr_->isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		traits_ptr_->toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		return traits_ptr_->isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		traits_ptr_->toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return traits_ptr_->isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}	

		traits_ptr_->toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		return traits_ptr_->isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>& ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator ++ ()
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
	ConstRandomAccessIterator<Container, DataType, Position, Traits> ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>& ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -- ()
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
	ConstRandomAccessIterator<Container, DataType, Position, Traits> ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>& ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (distance < (Distance)0)
		{
			return (*this -= -distance);
		}
		traits_ptr_->forward(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>& ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -= (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (traits_ptr_->isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (distance < (Distance)0)
		{
			return (*this += -distance);
		}
		if (traits_ptr_->isEnd() == true)
		{
			traits_ptr_->toRBegin();
			traits_ptr_->backward(distance - 1);
		}
		else 
		{
			traits_ptr_->backward(distance);
		}
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator + (Distance distance) const
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(*this);
		return (iterator += distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator - (Distance distance) const
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(*this);
		return (iterator -= distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator < (const ConstRandomAccessIterator &iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);				
		}
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (*traits_ptr_ < *iterator.traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator <= (const ConstRandomAccessIterator &iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}
		return !(*iterator.traits_ptr_ > *traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator >= (const ConstRandomAccessIterator &iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return !(*traits_ptr_ < *iterator.traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator > (const ConstRandomAccessIterator &iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (*iterator.traits_ptr_ < *traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType> &predicate)
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
	const DataType* ConstRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType> &predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((ConstRandomAccessIterator *)this)->findFirst(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstRandomAccessIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType> &predicate)
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
	const DataType* ConstRandomAccessIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType> &predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((ConstRandomAccessIterator *)this)->findLast(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstRandomAccessIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType> &predicate)
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
	const DataType* ConstRandomAccessIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType> &predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((ConstRandomAccessIterator *)this)->findNext(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType> &predicate)
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
	const DataType* ConstRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType> &predicate) const
		throw(Exception::InvalidIterator)
	{
		return ((ConstRandomAccessIterator *)this)->findPrevious(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType& ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator [] (Index index) const
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (const DataType &)traits_ptr_->getData(index);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> ConstRandomAccessIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> ConstRandomAccessIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> ConstRandomAccessIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> ConstRandomAccessIterator<Container, DataType, Position, Traits>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator(const Container& container)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
	{
	}

	//@}

} // namespace BALL 

#endif // BALL_CONCEPT_RANDOMACCESSITERATOR_H
