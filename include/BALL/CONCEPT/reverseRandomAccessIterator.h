// $Id: reverseRandomAccessIterator.h,v 1.5 2001/06/08 12:21:28 anker Exp $

#ifndef BALL_CONCEPT_REVERSERANDOMACCESS_H
#define BALL_CONCEPT_REVERSERANDOMACCESS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#	include <BALL/CONCEPT/randomAccessIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif


namespace BALL 
{

	/**	@name	Generic Reverse Random Access Iterator Classes.
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/reverseRandomAccessIterator.h}
	*/
	//@{

	/**	Generic Reverse Random Access Iterator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ReverseRandomAccessIterator
		: public RandomAccessIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(ReverseRandomAccessIterator)

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
			
		/**
		*/
		ReverseRandomAccessIterator()
			throw();

		/**
		*/
		ReverseRandomAccessIterator(const ReverseRandomAccessIterator& iterator)
			throw();

		/**
		*/
		ReverseRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
			throw();
		
		/**
		*/
		~ReverseRandomAccessIterator()
			throw();

		//@}

		bool operator + () const
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

		ReverseRandomAccessIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		ReverseRandomAccessIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		ReverseRandomAccessIterator& operator -- ()
			throw(Exception::InvalidIterator);

		ReverseRandomAccessIterator operator -- (int)
			throw(Exception::InvalidIterator);

		ReverseRandomAccessIterator& operator += (Distance distance)
			throw(Exception::InvalidIterator);

		ReverseRandomAccessIterator& operator -= (Distance distance)
			throw(Exception::InvalidIterator);

		ReverseRandomAccessIterator operator + (Distance distance)
			throw(Exception::InvalidIterator);

		friend ReverseRandomAccessIterator operator + (Distance distance,
			const ReverseRandomAccessIterator& iterator)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator tmp_iterator(iterator);
			return (tmp_iterator += distance);
		}

		ReverseRandomAccessIterator operator - (Distance distance)
			throw(Exception::InvalidIterator);

		friend Distance operator - (const ReverseRandomAccessIterator& a, const ReverseRandomAccessIterator& b)
			throw(Exception::InvalidIterator)
		{
			return (((RandomAccessIterator<Container, DataType, Position, Traits>&)b) 
							- ((RandomAccessIterator<Container, DataType, Position, Traits>&)a));
		}

		bool operator < (const ReverseRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator);

		bool operator <= (const ReverseRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator);

		bool operator >= (const ReverseRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator);

		bool operator > (const ReverseRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator);

		DataType* findFirst(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);

		const DataType* findFirst(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		DataType* findLast(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findLast(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		DataType* findPrevious(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);

		const DataType* findPrevious(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		DataType* findNext(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findNext(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		static ReverseRandomAccessIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		static ReverseRandomAccessIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		static ReverseRandomAccessIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		static ReverseRandomAccessIterator rend(const Container& container)
			throw(Exception::InvalidIterator);

		protected:

		ReverseRandomAccessIterator(const Container& container)
			throw();

	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits>::ReverseRandomAccessIterator()
		throw()
		:	RandomAccessIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits>::ReverseRandomAccessIterator(const ReverseRandomAccessIterator& iterator)
		throw()
		:	RandomAccessIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits>::ReverseRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
		throw()
		:	RandomAccessIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits>::~ReverseRandomAccessIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return traits_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ReverseRandomAccessIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator<Container, DataType, Position, Traits>::toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseRandomAccessIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		return RandomAccessIterator<Container, DataType, Position, Traits>::isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ReverseRandomAccessIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator<Container, DataType, Position, Traits>::toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseRandomAccessIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		return RandomAccessIterator<Container, DataType, Position, Traits>::isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ReverseRandomAccessIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator<Container, DataType, Position, Traits>::toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseRandomAccessIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		return RandomAccessIterator<Container, DataType, Position, Traits>::isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ReverseRandomAccessIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator<Container, DataType, Position, Traits>::toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseRandomAccessIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		return RandomAccessIterator<Container, DataType, Position, Traits>::isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits>& ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator<Container, DataType, Position, Traits>::operator--();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits> ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ReverseRandomAccessIterator iterator(*this);
		RandomAccessIterator<Container, DataType, Position, Traits>::operator--();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits>& ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator<Container, DataType, Position, Traits>::operator++();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits> ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		ReverseRandomAccessIterator iterator(*this);
		RandomAccessIterator<Container, DataType, Position, Traits>::operator++();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits>& ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator<Container, DataType, Position, Traits>::operator-=(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits>& ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator -= (Distance distance)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator<Container, DataType, Position, Traits>::operator+=(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits> ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator + (Distance distance)
		throw(Exception::InvalidIterator)
	{
		ReverseRandomAccessIterator iterator(*this);
		return (iterator += distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits> ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator -(Distance distance)
		throw(Exception::InvalidIterator)
	{
		ReverseRandomAccessIterator iterator(*this);
		return (iterator -= distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator < (const ReverseRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator)
	{
		return RandomAccessIterator<Container, DataType, Position, Traits>::operator>(iterator);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator <= (const ReverseRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator)
	{
		return RandomAccessIterator<Container, DataType, Position, Traits>::operator>=(iterator);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator >= (const ReverseRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator)
	{
		return RandomAccessIterator<Container, DataType, Position, Traits>::operator<=(iterator);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator > (const ReverseRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator)
	{
		return RandomAccessIterator<Container, DataType, Position, Traits>::operator<(iterator);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* ReverseRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ReverseRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* ReverseRandomAccessIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ReverseRandomAccessIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* ReverseRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ReverseRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* ReverseRandomAccessIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ReverseRandomAccessIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits> ReverseRandomAccessIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ReverseRandomAccessIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits> ReverseRandomAccessIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ReverseRandomAccessIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits> ReverseRandomAccessIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ReverseRandomAccessIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits> ReverseRandomAccessIterator<Container, DataType, Position, Traits>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ReverseRandomAccessIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseRandomAccessIterator<Container, DataType, Position, Traits>::ReverseRandomAccessIterator(const Container& container)
		throw()
		:	RandomAccessIterator<Container, DataType, Position, Traits>(container)
	{
	}


	/**	Generic Constant Random Access Iterator
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstReverseRandomAccessIterator
		: public ConstRandomAccessIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(ConstReverseRandomAccessIterator)

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
		ConstReverseRandomAccessIterator()
			throw();

		/**
		*/
		ConstReverseRandomAccessIterator(const ConstReverseRandomAccessIterator& iterator)
			throw();

		/**
		*/
		ConstReverseRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
			throw();

		/**
		 */
		~ConstReverseRandomAccessIterator()
			throw();

		//@}

		bool operator + () const
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

		ConstReverseRandomAccessIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		ConstReverseRandomAccessIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		ConstReverseRandomAccessIterator& operator -- ()
			throw(Exception::InvalidIterator);

		ConstReverseRandomAccessIterator operator -- (int)
			throw(Exception::InvalidIterator);

		ConstReverseRandomAccessIterator& operator += (Distance distance)
			throw(Exception::InvalidIterator);

		ConstReverseRandomAccessIterator& operator -= (Distance distance)
			throw(Exception::InvalidIterator);

		ConstReverseRandomAccessIterator operator + (Distance distance)
			throw(Exception::InvalidIterator);

		friend ConstReverseRandomAccessIterator operator + (Distance distance, const ConstReverseRandomAccessIterator& iterator)
			throw(Exception::InvalidIterator)
		{
			ConstReverseRandomAccessIterator tmp_iterator(iterator);
			return (tmp_iterator += distance);
		}

		ConstReverseRandomAccessIterator operator - (Distance distance)
			throw(Exception::InvalidIterator);

		friend Distance operator - (const ConstReverseRandomAccessIterator& a,
																const ConstReverseRandomAccessIterator& b)
			throw(Exception::InvalidIterator)
		{
			return (((ConstRandomAccessIterator<Container, DataType, Position, Traits>& )b) 
							- ((ConstRandomAccessIterator<Container, DataType, Position, Traits>& )a));
		}

		bool operator < (const ConstReverseRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator);

		bool operator <= (const ConstReverseRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator);

		bool operator >= (const ConstReverseRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator);

		bool operator > (const ConstReverseRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator);

		DataType* findFirst(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);

		const DataType* findFirst(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		DataType* findLast(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findLast(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		DataType* findPrevious(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);

		const DataType* findPrevious(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		DataType* findNext(const UnaryPredicate<DataType>& predicate)
			throw(Exception::InvalidIterator);
	
		const DataType* findNext(const UnaryPredicate<DataType>& predicate) const
			throw(Exception::InvalidIterator);

		static ConstReverseRandomAccessIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		static ConstReverseRandomAccessIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		static ConstReverseRandomAccessIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		static ConstReverseRandomAccessIterator rend(const Container& container)
			throw(Exception::InvalidIterator);

	protected:

		ConstReverseRandomAccessIterator(const Container& container)
			throw();

	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::ConstReverseRandomAccessIterator()
		throw()
		:	ConstRandomAccessIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::ConstReverseRandomAccessIterator(const ConstReverseRandomAccessIterator& iterator)
		throw()
		:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::ConstReverseRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
		throw()
		:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::~ConstReverseRandomAccessIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return traits_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>& ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator--();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits> ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ConstReverseRandomAccessIterator iterator(*this);
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator--();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>& ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator++();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits> ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		ConstReverseRandomAccessIterator iterator(*this);
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator++();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>& ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator-=(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>& ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator -= (Distance distance)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator+=(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits> ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator + (Distance distance)
		throw(Exception::InvalidIterator)
	{
		ConstReverseRandomAccessIterator iterator(*this);
		return (iterator += distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits> ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator - (Distance distance)
		throw(Exception::InvalidIterator)
	{
		ConstReverseRandomAccessIterator iterator(*this);
		return (iterator -= distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator < (const ConstReverseRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator)
	{
		return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator>(iterator);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator <= (const ConstReverseRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator)
	{
		return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator>=(iterator);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator >= (const ConstReverseRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator)
	{
		return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator<=(iterator);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::operator > (const ConstReverseRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator)
	{
		return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator<(iterator);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::findLast(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType>& predicate)
		throw(Exception::InvalidIterator)
	{
		return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::findNext(const UnaryPredicate<DataType>& predicate) const
		throw(Exception::InvalidIterator)
	{
		return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits> ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstReverseRandomAccessIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits> ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstReverseRandomAccessIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits> ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstReverseRandomAccessIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits> ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstReverseRandomAccessIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseRandomAccessIterator<Container, DataType, Position, Traits>::ConstReverseRandomAccessIterator(const Container& container)
		throw()
		:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(container)
	{
	}

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_REVERSERANDOMACCESS_H
