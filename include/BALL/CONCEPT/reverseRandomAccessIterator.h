// $Id: reverseRandomAccessIterator.h,v 1.1 1999/08/26 07:53:17 oliver Exp $

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


namespace BALL {

	/**	@name	Generic Reverse Random Access Iterator Classes.
			{\bf Definition:} \URL{BALL/CONCEPT/reverseRandomAccessIterator.h}
	*/
	//@{

	/**	Generic Reverse Random Access Manipulator Class
	*/		
	template <class Container, class DataType, class Position, class Traits>
	class ReverseRandomAccessManipulator
		: public RandomAccessManipulator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{

		/**	
		*/
		ReverseRandomAccessManipulator(void)
			: RandomAccessManipulator<Container, DataType, Position, Traits>()
		{
		}

		/**	
		*/
		ReverseRandomAccessManipulator(const ReverseRandomAccessManipulator &manipulator)
			:	RandomAccessManipulator<Container, DataType, Position, Traits>(manipulator)
		{
		}

		/**	
		*/
		ReverseRandomAccessManipulator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	RandomAccessManipulator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + (void) const
		{
			return traits_->isValid();
		}

		void toBegin(void)
		{
			RandomAccessManipulator<Container, DataType, Position, Traits>::toRBegin();
		}

		bool isBegin(void) const
		{
			return RandomAccessManipulator<Container, DataType, Position, Traits>::isRBegin();
		}

		void toEnd(void)
		{
			RandomAccessManipulator<Container, DataType, Position, Traits>::toREnd();
		}

		bool isEnd(void) const
		{
			return RandomAccessManipulator<Container, DataType, Position, Traits>::isREnd();
		}

		void toRBegin(void)
		{
			RandomAccessManipulator<Container, DataType, Position, Traits>::toBegin();
		}

		bool isRBegin(void) const
		{
			return RandomAccessManipulator<Container, DataType, Position, Traits>::isBegin();
		}

		void toREnd(void)
		{
			RandomAccessManipulator<Container, DataType, Position, Traits>::toEnd();
		}

		bool isREnd(void) const
		{
			return RandomAccessManipulator<Container, DataType, Position, Traits>::isEnd();
		}

		ReverseRandomAccessManipulator &operator ++ (void)
		{
			RandomAccessManipulator<Container, DataType, Position, Traits>::operator--();
			return *this;
		}

		ReverseRandomAccessManipulator operator ++ (int)
		{
			ReverseRandomAccessManipulator manipulator(*this);
			RandomAccessManipulator<Container, DataType, Position, Traits>::operator--();
			return manipulator;
		}

		ReverseRandomAccessManipulator &operator -- (void)
		{
			RandomAccessManipulator<Container, DataType, Position, Traits>::operator++();
			return *this;
		}

		ReverseRandomAccessManipulator operator -- (int)
		{
			ReverseRandomAccessManipulator manipulator(*this);
			RandomAccessManipulator<Container, DataType, Position, Traits>::operator++();
			return manipulator;
		}

		ReverseRandomAccessManipulator &operator += (Distance distance)
		{
			RandomAccessManipulator<Container, DataType, Position, Traits>::operator-=(distance);
			return *this;
		}

		ReverseRandomAccessManipulator &operator -= (Distance distance)
		{
			RandomAccessManipulator<Container, DataType, Position, Traits>::operator+=(distance);
			return *this;
		}

		ReverseRandomAccessManipulator operator + (Distance distance)
		{
			ReverseRandomAccessManipulator manipulator(*this);
			return (manipulator += distance);
		}

		friend ReverseRandomAccessManipulator operator + (Distance distance, const ReverseRandomAccessManipulator &manipulator)
		{
			ReverseRandomAccessManipulator manipulator_b(manipulator);
			return (manipulator_b += distance);
		}

		ReverseRandomAccessManipulator operator - (Distance distance)
		{
			ReverseRandomAccessManipulator manipulator(*this);
			return (manipulator -= distance);
		}

		friend Distance operator - (const ReverseRandomAccessManipulator &a, const ReverseRandomAccessManipulator &b)
		{
			return (((RandomAccessManipulator<Container, DataType, Position, Traits> &)b) 
							- ((RandomAccessManipulator<Container, DataType, Position, Traits> &)a));
		}

		bool operator < (const ReverseRandomAccessManipulator &manipulator) const
		{
			return RandomAccessManipulator<Container, DataType, Position, Traits>::operator>(manipulator);
		}

		bool operator <= (const ReverseRandomAccessManipulator &manipulator) const
		{
			return RandomAccessManipulator<Container, DataType, Position, Traits>::operator>=(manipulator);
		}

		bool operator >= (const ReverseRandomAccessManipulator &manipulator) const
		{
			return RandomAccessManipulator<Container, DataType, Position, Traits>::operator<=(manipulator);
		}

		bool operator > (const ReverseRandomAccessManipulator &manipulator) const
		{
			return RandomAccessManipulator<Container, DataType, Position, Traits>::operator<(manipulator);
		}

		DataType *findFirst(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)RandomAccessManipulator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)RandomAccessManipulator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		DataType *findLast(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)RandomAccessManipulator<Container, DataType, Position, Traits>::findFirst(predicate);
		}
	
		const DataType *findLast(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)RandomAccessManipulator<Container, DataType, Position, Traits>::findFirst(predicate);
		}

		DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)RandomAccessManipulator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)RandomAccessManipulator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		DataType *findNext(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)RandomAccessManipulator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}
	
		const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)RandomAccessManipulator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}

		static ReverseRandomAccessManipulator begin(const Container &container)
		{
			ReverseRandomAccessManipulator manipulator(container);
			manipulator.toBegin();
			return manipulator;
		}

		static ReverseRandomAccessManipulator end(const Container &container)
		{
			ReverseRandomAccessManipulator manipulator(container);
			manipulator.toEnd();
			return manipulator;
		}

		static ReverseRandomAccessManipulator rbegin(const Container &container)
		{
			ReverseRandomAccessManipulator manipulator(container);
			manipulator.toRBegin();
			return manipulator;
		}

		static ReverseRandomAccessManipulator rend(const Container &container)
		{
			ReverseRandomAccessManipulator manipulator(container);
			manipulator.toREnd();
			return manipulator;
		}

		protected:

		ReverseRandomAccessManipulator(const Container &container)
			:	RandomAccessManipulator<Container, DataType, Position, Traits>(container)
		{
		}

	};


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
		ReverseRandomAccessIterator(void)
			:	RandomAccessIterator<Container, DataType, Position, Traits>()
		{
		}

		/**
		*/
		ReverseRandomAccessIterator(const ReverseRandomAccessIterator &iterator)
			:	RandomAccessIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		ReverseRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	RandomAccessIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + (void) const
		{
			return traits_->isValid();
		}

		void toBegin(void)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::toRBegin();
		}

		bool isBegin(void) const
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::isRBegin();
		}

		void toEnd(void)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::toREnd();
		}

		bool isEnd(void) const
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::isREnd();
		}

		void toRBegin(void)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::toBegin();
		}

		bool isRBegin(void) const
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::isBegin();
		}

		void toREnd(void)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::toEnd();
		}

		bool isREnd(void) const
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::isEnd();
		}

		ReverseRandomAccessIterator &operator ++ (void)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::operator--();
			return *this;
		}

		ReverseRandomAccessIterator operator ++ (int)
		{
			ReverseRandomAccessIterator iterator(*this);
			RandomAccessIterator<Container, DataType, Position, Traits>::operator--();
			return iterator;
		}

		ReverseRandomAccessIterator &operator -- (void)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::operator++();
			return *this;
		}

		ReverseRandomAccessIterator operator -- (int)
		{
			ReverseRandomAccessIterator iterator(*this);
			RandomAccessIterator<Container, DataType, Position, Traits>::operator++();
			return iterator;
		}

		ReverseRandomAccessIterator &operator += (Distance distance)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::operator-=(distance);
			return *this;
		}

		ReverseRandomAccessIterator &operator -= (Distance distance)
		{
			RandomAccessIterator<Container, DataType, Position, Traits>::operator+=(distance);
			return *this;
		}

		ReverseRandomAccessIterator operator + (Distance distance)
		{
			ReverseRandomAccessIterator iterator(*this);
			return (iterator += distance);
		}

		friend ReverseRandomAccessIterator operator + (Distance distance,
			 const ReverseRandomAccessIterator &iterator)
		{
			ReverseRandomAccessIterator tmp_iterator(iterator);
			return (tmp_iterator += distance);
		}

		ReverseRandomAccessIterator operator -(Distance distance)
		{
			ReverseRandomAccessIterator iterator(*this);
			return (iterator -= distance);
		}

		friend Distance operator - (const ReverseRandomAccessIterator &a, const ReverseRandomAccessIterator &b)
		{
			return (((RandomAccessIterator<Container, DataType, Position, Traits> &)b) 
							- ((RandomAccessIterator<Container, DataType, Position, Traits> &)a));
		}

		bool operator < (const ReverseRandomAccessIterator &iterator) const
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::operator>(iterator);
		}

		bool operator <= (const ReverseRandomAccessIterator &iterator) const
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::operator>=(iterator);
		}

		bool operator >= (const ReverseRandomAccessIterator &iterator) const
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::operator<=(iterator);
		}

		bool operator > (const ReverseRandomAccessIterator &iterator) const
		{
			return RandomAccessIterator<Container, DataType, Position, Traits>::operator<(iterator);
		}

		DataType *findFirst(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
		}

		DataType *findLast(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}
	
		const DataType *findLast(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
		}

		DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
		}

		DataType *findNext(const UnaryPredicate<DataType> &predicate)
		{
			return (DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}
	
		const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)RandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
		}

		static ReverseRandomAccessIterator begin(const Container &container)
		{
			ReverseRandomAccessIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static ReverseRandomAccessIterator end(const Container &container)
		{
			ReverseRandomAccessIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		static ReverseRandomAccessIterator rbegin(const Container &container)
		{
			ReverseRandomAccessIterator iterator(container);
			iterator.toRBegin();
			return iterator;
		}

		static ReverseRandomAccessIterator rend(const Container &container)
		{
			ReverseRandomAccessIterator iterator(container);
			iterator.toREnd();
			return iterator;
		}

		protected:

		ReverseRandomAccessIterator(const Container &container)
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
			ConstReverseRandomAccessIterator(void)
				:	ConstRandomAccessIterator<Container, DataType, Position, Traits>()
			{
			}

			/**
			*/
			ConstReverseRandomAccessIterator(const ConstReverseRandomAccessIterator &iterator)
				:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			/**
			*/
			ConstReverseRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
				:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(iterator)
			{
			}
			//@}

			bool operator + (void) const
			{
				return traits_->isValid();
			}

			void toBegin(void)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::toRBegin();
			}

			bool isBegin(void) const
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isRBegin();
			}

			void toEnd(void)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::toREnd();
			}

			bool isEnd(void) const
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isREnd();
			}

			void toRBegin(void)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::toBegin();
			}

			bool isRBegin(void) const
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isBegin();
			}

			void toREnd(void)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::toEnd();
			}

			bool isREnd(void) const
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::isEnd();
			}

			ConstReverseRandomAccessIterator &operator ++ (void)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator--();
				return *this;
			}

			ConstReverseRandomAccessIterator operator ++ (int)
			{
				ConstReverseRandomAccessIterator iterator(*this);
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator--();
				return iterator;
			}

			ConstReverseRandomAccessIterator &operator -- (void)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator++();
				return *this;
			}

			ConstReverseRandomAccessIterator operator -- (int)
			{
				ConstReverseRandomAccessIterator iterator(*this);
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator++();
				return iterator;
			}

			ConstReverseRandomAccessIterator &operator += (Distance distance)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator-=(distance);
				return *this;
			}

			ConstReverseRandomAccessIterator &operator -= (Distance distance)
			{
				ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator+=(distance);
				return *this;
			}

			ConstReverseRandomAccessIterator operator + (Distance distance)
			{
				ConstReverseRandomAccessIterator iterator(*this);
				return (iterator += distance);
			}

			friend ConstReverseRandomAccessIterator operator + (Distance distance, const ConstReverseRandomAccessIterator &iterator)
			{
				ConstReverseRandomAccessIterator tmp_iterator(iterator);
				return (tmp_iterator += distance);
			}

			ConstReverseRandomAccessIterator operator - (Distance distance)
			{
				ConstReverseRandomAccessIterator iterator(*this);
				return (iterator -= distance);
			}

			friend Distance operator - 
				(const ConstReverseRandomAccessIterator &a,
				 const ConstReverseRandomAccessIterator &b)
			{
				return (((ConstRandomAccessIterator<Container, DataType, Position, Traits> &)b) 
								- ((ConstRandomAccessIterator<Container, DataType, Position, Traits> &)a));
			}

			bool operator < (const ConstReverseRandomAccessIterator &iterator) const
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator>(iterator);
			}

			bool operator <= (const ConstReverseRandomAccessIterator &iterator) const
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator>=(iterator);
			}

			bool operator >= (const ConstReverseRandomAccessIterator &iterator) const
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator<=(iterator);
			}

			bool operator > (const ConstReverseRandomAccessIterator &iterator) const
			{
				return ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator<(iterator);
			}

			DataType *findFirst(const UnaryPredicate<DataType> &predicate)
			{
				return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
			}

			const DataType *findFirst(const UnaryPredicate<DataType> &predicate) const
			{
				return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findLast(predicate);
			}

			DataType *findLast(const UnaryPredicate<DataType> &predicate)
			{
				return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
			}
		
			const DataType *findLast(const UnaryPredicate<DataType> &predicate) const
			{
				return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findFirst(predicate);
			}

			DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
			{
				return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
			}

			const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
			{
				return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findNext(predicate);
			}

			DataType *findNext(const UnaryPredicate<DataType> &predicate)
			{
				return (DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
			}
		
			const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
			{
				return (const DataType *)ConstRandomAccessIterator<Container, DataType, Position, Traits>::findPrevious(predicate);
			}

			static ConstReverseRandomAccessIterator begin(const Container &container)
			{
				ConstReverseRandomAccessIterator iterator(container);
				iterator.toBegin();
				return iterator;
			}

			static ConstReverseRandomAccessIterator end(const Container &container)
			{
				ConstReverseRandomAccessIterator iterator(container);
				iterator.toEnd();
				return iterator;
			}

			static ConstReverseRandomAccessIterator rbegin(const Container &container)
			{
				ConstReverseRandomAccessIterator iterator(container);
				iterator.toRBegin();
				return iterator;
			}

			static ConstReverseRandomAccessIterator rend(const Container &container)
			{
				ConstReverseRandomAccessIterator iterator(container);
				iterator.toREnd();
				return iterator;
			}

		protected:

			ConstReverseRandomAccessIterator(const Container &container)
				:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(container)
			{
			}

		private:
	};

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_REVERSERANDOMACCESS_H
