// $Id: reverseRandomAccessIterator.h,v 1.4 2000/12/19 12:50:50 amoll Exp $

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
			{\bf Definition:} \URL{BALL/CONCEPT/reverseRandomAccessIterator.h}
	*/
	//@{

	/**	Generic Reverse Random Access Iterator Class
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ReverseRandomAccessIterator
		: public RandomAccessIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{
			
		/**
		*/
		ReverseRandomAccessIterator()
			throw()
			:	RandomAccessIterator<Container, DataType, Position, Traits>()
		{
		}

		/**
		*/
		ReverseRandomAccessIterator(const ReverseRandomAccessIterator &iterator)
			throw()
			:	RandomAccessIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		ReverseRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw()
			:	RandomAccessIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + () const
			throw()
		{
			return traits_->isValid();
		}

		void toBegin()
			throw(Exception::InvalidIterator)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::toRBegin();
		}

		bool isBegin() const
			throw(Exception::InvalidIterator)
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::isRBegin();
		}

		void toEnd()
			throw(Exception::InvalidIterator)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::toREnd();
		}

		bool isEnd() const
			throw(Exception::InvalidIterator)
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::isREnd();
		}

		void toRBegin()
			throw(Exception::InvalidIterator)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::toBegin();
		}

		bool isRBegin() const
			throw(Exception::InvalidIterator)
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::isBegin();
		}

		void toREnd()
			throw(Exception::InvalidIterator)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::toEnd();
		}

		bool isREnd() const
			throw(Exception::InvalidIterator)
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::isEnd();
		}

		ReverseRandomAccessIterator &operator ++ ()
			throw(Exception::InvalidIterator)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::operator--();
			return *this;
		}

		ReverseRandomAccessIterator operator ++ (int)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator iterator(*this);
			RandomAccessIterator<Container, DataType, Position, Traits>::operator--();
			return iterator;
		}

		ReverseRandomAccessIterator &operator -- ()
			throw(Exception::InvalidIterator)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::operator++();
			return *this;
		}

		ReverseRandomAccessIterator operator -- (int)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator iterator(*this);
			RandomAccessIterator<Container, DataType, Position, Traits>::operator++();
			return iterator;
		}

		ReverseRandomAccessIterator &operator += (Distance distance)
			throw(Exception::InvalidIterator)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::operator-=(distance);
			return *this;
		}

		ReverseRandomAccessIterator &operator -= (Distance distance)
			throw(Exception::InvalidIterator)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::operator+=(distance);
			return *this;
		}

		ReverseRandomAccessIterator operator + (Distance distance)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator iterator(*this);
			return (iterator += distance);
		}

		friend ReverseRandomAccessIterator operator + (Distance distance,
			const ReverseRandomAccessIterator &iterator)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator tmp_iterator(iterator);
			return (tmp_iterator += distance);
		}

		ReverseRandomAccessIterator operator -(Distance distance)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator iterator(*this);
			return (iterator -= distance);
		}

		friend Distance operator - (const ReverseRandomAccessIterator &a, const ReverseRandomAccessIterator &b)
			throw(Exception::InvalidIterator)
		{
			return (((RandomAccessIterator<Container, DataType, Position, Traits> &)b) 
							- ((RandomAccessIterator<Container, DataType, Position, Traits> &)a));
		}

		bool operator < (const ReverseRandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator)
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::operator>(iterator);
		}

		bool operator <= (const ReverseRandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator)
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::operator>=(iterator);
		}

		bool operator >= (const ReverseRandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator)
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::operator<=(iterator);
		}

		bool operator > (const ReverseRandomAccessIterator &iterator) const
			throw(Exception::InvalidIterator)
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::operator<(iterator);
		}

		DataType* findFirst(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType* findFirst(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		DataType* findLast(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}
	
		const DataType* findLast(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}

		DataType* findPrevious(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType* findPrevious(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		DataType* findNext(const UnaryPredicate<DataType> &predicate)
			throw(Exception::InvalidIterator)
		{
			return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}
	
		const DataType* findNext(const UnaryPredicate<DataType> &predicate) const
			throw(Exception::InvalidIterator)
		{
			return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}

		static ReverseRandomAccessIterator begin(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static ReverseRandomAccessIterator end(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		static ReverseRandomAccessIterator rbegin(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator iterator(container);
			iterator.toRBegin();
			return iterator;
		}

		static ReverseRandomAccessIterator rend(const Container &container)
			throw(Exception::InvalidIterator)
		{
			ReverseRandomAccessIterator iterator(container);
			iterator.toREnd();
			return iterator;
		}

		protected:

		ReverseRandomAccessIterator(const Container &container)
			throw()
			:	RandomAccessIterator<Container, DataType, Position, Traits>(container)
		{
		}

	};


	/**	Generic Constant Random Access Iterator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ConstReverseRandomAccessIterator
		: public ConstRandomAccessIterator<Container, DataType, Position, Traits>
	{
		public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			/**
			*/
			ConstReverseRandomAccessIterator()
				throw()
				:	ConstRandomAccessIterator<Container, DataType, Position, Traits>()
			{
			}

			/**
			*/
			ConstReverseRandomAccessIterator(const ConstReverseRandomAccessIterator &iterator)
				throw()
				:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			/**
			*/
			ConstReverseRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
				throw()
				:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(iterator)
			{
			}
			//@}

			bool operator + () const
				throw()
			{
				return traits_->isValid();
			}

			void toBegin()
				throw(Exception::InvalidIterator)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::toRBegin();
			}

			bool isBegin() const
				throw(Exception::InvalidIterator)
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isRBegin();
			}

			void toEnd()
				throw(Exception::InvalidIterator)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::toREnd();
			}

			bool isEnd() const
				throw(Exception::InvalidIterator)
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isREnd();
			}

			void toRBegin()
				throw(Exception::InvalidIterator)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::toBegin();
			}

			bool isRBegin() const
				throw(Exception::InvalidIterator)
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isBegin();
			}

			void toREnd()
				throw(Exception::InvalidIterator)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::toEnd();
			}

			bool isREnd() const
				throw(Exception::InvalidIterator)
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isEnd();
			}

			ConstReverseRandomAccessIterator &operator ++ ()
				throw(Exception::InvalidIterator)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator--();
				return *this;
			}

			ConstReverseRandomAccessIterator operator ++ (int)
				throw(Exception::InvalidIterator)
			{
				ConstReverseRandomAccessIterator iterator(*this);
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator--();
				return iterator;
			}

			ConstReverseRandomAccessIterator &operator -- ()
				throw(Exception::InvalidIterator)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator++();
				return *this;
			}

			ConstReverseRandomAccessIterator operator -- (int)
				throw(Exception::InvalidIterator)
			{
				ConstReverseRandomAccessIterator iterator(*this);
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator++();
				return iterator;
			}

			ConstReverseRandomAccessIterator& operator += (Distance distance)
				throw(Exception::InvalidIterator)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator-=(distance);
				return *this;
			}

			ConstReverseRandomAccessIterator& operator -= (Distance distance)
				throw(Exception::InvalidIterator)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator+=(distance);
				return *this;
			}

			ConstReverseRandomAccessIterator operator + (Distance distance)
				throw(Exception::InvalidIterator)
			{
				ConstReverseRandomAccessIterator iterator(*this);
				return (iterator += distance);
			}

			friend ConstReverseRandomAccessIterator operator + (Distance distance, const ConstReverseRandomAccessIterator &iterator)
				throw(Exception::InvalidIterator)
			{
				ConstReverseRandomAccessIterator tmp_iterator(iterator);
				return (tmp_iterator += distance);
			}

			ConstReverseRandomAccessIterator operator - (Distance distance)
				throw(Exception::InvalidIterator)
			{
				ConstReverseRandomAccessIterator iterator(*this);
				return (iterator -= distance);
			}

			friend Distance operator - (const ConstReverseRandomAccessIterator &a,
																	const ConstReverseRandomAccessIterator &b)
				throw(Exception::InvalidIterator)
			{
				return (((ConstRandomAccessIterator<Container, DataType, Position, Traits> &)b) 
								- ((ConstRandomAccessIterator<Container, DataType, Position, Traits> &)a));
			}

			bool operator < (const ConstReverseRandomAccessIterator &iterator) const
				throw(Exception::InvalidIterator)
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator>(iterator);
			}

			bool operator <= (const ConstReverseRandomAccessIterator &iterator) const
				throw(Exception::InvalidIterator)
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator>=(iterator);
			}

			bool operator >= (const ConstReverseRandomAccessIterator &iterator) const
				throw(Exception::InvalidIterator)
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator<=(iterator);
			}

			bool operator > (const ConstReverseRandomAccessIterator &iterator) const
				throw(Exception::InvalidIterator)
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator<(iterator);
			}

			DataType* findFirst(const UnaryPredicate<DataType> &predicate)
				throw(Exception::InvalidIterator)
			{
				return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
			}

			const DataType* findFirst(const UnaryPredicate<DataType> &predicate) const
				throw(Exception::InvalidIterator)
			{
				return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
			}

			DataType* findLast(const UnaryPredicate<DataType> &predicate)
				throw(Exception::InvalidIterator)
			{
				return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
			}
		
			const DataType* findLast(const UnaryPredicate<DataType> &predicate) const
				throw(Exception::InvalidIterator)
			{
				return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
			}

			DataType* findPrevious(const UnaryPredicate<DataType> &predicate)
				throw(Exception::InvalidIterator)
			{
				return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
			}

			const DataType* findPrevious(const UnaryPredicate<DataType> &predicate) const
				throw(Exception::InvalidIterator)
			{
				return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
			}

			DataType* findNext(const UnaryPredicate<DataType> &predicate)
				throw(Exception::InvalidIterator)
			{
				return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
			}
		
			const DataType* findNext(const UnaryPredicate<DataType> &predicate) const
				throw(Exception::InvalidIterator)
			{
				return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
			}

			static ConstReverseRandomAccessIterator begin(const Container &container)
				throw(Exception::InvalidIterator)
			{
				ConstReverseRandomAccessIterator iterator(container);
				iterator.toBegin();
				return iterator;
			}

			static ConstReverseRandomAccessIterator end(const Container &container)
				throw(Exception::InvalidIterator)
			{
				ConstReverseRandomAccessIterator iterator(container);
				iterator.toEnd();
				return iterator;
			}

			static ConstReverseRandomAccessIterator rbegin(const Container &container)
				throw(Exception::InvalidIterator)
			{
				ConstReverseRandomAccessIterator iterator(container);
				iterator.toRBegin();
				return iterator;
			}

			static ConstReverseRandomAccessIterator rend(const Container &container)
				throw(Exception::InvalidIterator)
			{
				ConstReverseRandomAccessIterator iterator(container);
				iterator.toREnd();
				return iterator;
			}

		protected:

			ConstReverseRandomAccessIterator(const Container &container)
				throw()
				:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(container)
			{
			}

	};

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_REVERSERANDOMACCESS_H
