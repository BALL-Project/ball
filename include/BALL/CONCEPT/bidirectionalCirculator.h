// $Id: bidirectionalCirculator.h,v 1.2 1999/12/04 18:34:10 oliver Exp $

#ifndef BALL_CONCEPT_BIDIRECTIONALCIRCULATOR_H
#define BALL_CONCEPT_BIDIRECTIONALCIRCULATOR_H

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

	/**	@name	Bidirectional Circulators 
	*/
	//@{

	/**	Bidirectional Circular Manipulator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class BidirectionalCircularManipulator
		: public BidirectionalManipulator<Container, DataType, Position, Traits>
	{
		public:

		/** @name	Constructors and Destructors
		*/
		//@{

		/**
		*/
		BidirectionalCircularManipulator()
			: BidirectionalManipulator<Container, DataType, Position, Traits>()
		{
		}

		/**
		*/
		BidirectionalCircularManipulator(const BidirectionalCircularManipulator &manipulator)
			: BidirectionalManipulator<Container, DataType, Position, Traits>(manipulator)
		{
		}

		/**
		*/
		BidirectionalCircularManipulator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	BidirectionalManipulator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}
	
		/**	@name	Accessors
		*/
		//@{

		/**
		*/
		bool operator + () const
		{
			return traits_->isValid();
		}

		/**
		*/
		BidirectionalCircularManipulator &operator ++ ()
		{
			if (BidirectionalManipulator<Container, DataType, Position, Traits>::isRBegin() == true)
			{
				BidirectionalManipulator<Container, DataType, Position, Traits>::toBegin();
			} else {
				BidirectionalManipulator<Container, DataType, Position, Traits>::operator++();
			}
			return *this;
		}

		/**
		*/
		BidirectionalCircularManipulator operator ++ (int)
		{
			BidirectionalCircularManipulator manipulator(*this);
			++(*this);
			return manipulator;
		}

		/**
		*/
		BidirectionalCircularManipulator &operator -- ()
		{
			if (BidirectionalManipulator<Container, DataType, Position, Traits>::isBegin() == true)
			{
				BidirectionalManipulator<Container, DataType, Position, Traits>::toRBegin();
			} else {
				BidirectionalManipulator<Container, DataType, Position, Traits>::operator--();
			}
			return *this;
		}

		/**
		*/
		BidirectionalCircularManipulator operator -- (int)
		{
			BidirectionalCircularManipulator manipulator(*this);
			--(*this);
			return manipulator;
		}

		/**
		*/
		DataType* findPrevious(const UnaryPredicate<DataType>& predicate)
		{
			DataType *data = (DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findPrevious(predicate);

			if (data == 0)
			{
				return BidirectionalManipulator<Container, DataType, Position, Traits>::findLast(predicate);
			} else {
				return data;
			}
		}

		/**
		*/
		const DataType* findPrevious(const UnaryPredicate<DataType>& predicate) const
		{
			return ((BidirectionalCircularManipulator *)this)->findPrevious(predicate);
		}

		/**
		*/
		DataType* findNext(const UnaryPredicate<DataType>& predicate)
		{
			DataType *data = (DataType *)BidirectionalManipulator<Container, DataType, Position, Traits>::findNext(predicate);

			if (data == 0)
			{
				return BidirectionalManipulator<Container, DataType, Position, Traits>::findFirst(predicate);
			} else {
				return data;
			}
		}

		/**
		*/
		const DataType* findNext(const UnaryPredicate<DataType>& predicate) const
		{
			return ((BidirectionalCircularManipulator *)this)->findNext(predicate);
		}

		/**
		*/
		static BidirectionalCircularManipulator begin(const Container& container)
		{
			BidirectionalCircularManipulator manipulator(container);
			manipulator.toBegin();
			return manipulator;
		}
		//@}
		
		protected:

		BidirectionalCircularManipulator(const Container &container)
			: BidirectionalManipulator<Container, DataType, Position, Traits>(container)
		{
		}

		private:

		void toEnd();

		bool isEnd() const;

		void toRBegin();

		bool isRBegin() const;

		void toREnd();

		bool isREnd() const;

		DataType *findLast(const UnaryPredicate<DataType> &predicate);

		const DataType *findLast(const UnaryPredicate<DataType> &predicate) const;

		static BidirectionalCircularManipulator end(const Container &container);

		static BidirectionalCircularManipulator rbegin(const Container &container);

		static BidirectionalCircularManipulator rend(const Container &container);
	};



	/**	Bidirectional Circulator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class BidirectionalCirculator
		: public BidirectionalIterator<Container, DataType, Position, Traits>
	{
		public:

			BidirectionalCirculator()
				:	BidirectionalIterator<Container, DataType, Position, Traits>()
			{
			}

			BidirectionalCirculator(const BidirectionalCirculator &circulator)
				:	BidirectionalIterator<Container, DataType, Position, Traits>(circulator)
			{
			}

			BidirectionalCirculator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
				:	BidirectionalIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			bool operator +	() const
			{
				return traits_->isValid();
			}

			BidirectionalCirculator &operator ++ ()
			{
				if (BidirectionalIterator<Container, DataType, Position, Traits>::isRBegin() == true)
				{
					BidirectionalIterator<Container, DataType, Position, Traits>::toBegin();
				} else {
					BidirectionalIterator<Container, DataType, Position, Traits>::operator++();
				}
				return *this;
			}

			BidirectionalCirculator operator ++ (int)
			{
				BidirectionalCirculator circulator(*this);
				++(*this);
				return circulator;
			}

			BidirectionalCirculator &operator --
				()
			{
				if (BidirectionalIterator<Container, DataType, Position, Traits>::isBegin() == true)
				{
					BidirectionalIterator<Container, DataType, Position, Traits>::toRBegin();
				} else {
					BidirectionalIterator<Container, DataType, Position, Traits>::operator--();
				}
				return *this;
			}

			BidirectionalCirculator operator -- (int)
			{
				BidirectionalCirculator circulator(*this);
				--(*this);
				return circulator;
			}

			DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
			{
				DataType *data = (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);

				if (data == 0)
				{
					return BidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
				} else {
					return data;
				}
			}

			const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
			{
				return ((BidirectionalCirculator *)this)->findPrevious(predicate);
			}

			DataType *findNext(const UnaryPredicate<DataType> &predicate)
			{
				DataType *data = (DataType *)BidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);

				if (data == 0)
				{
						return BidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
				} else {
					return data;
				}
			}

			const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
			{
				return ((BidirectionalCirculator *)this)->findNext(predicate);
			}

			static BidirectionalCirculator begin(const Container &container)
			{
				BidirectionalCirculator circulator(container);
				circulator.toBegin();
				return circulator;
			}

		protected:

			BidirectionalCirculator(const Container &container)
				:	BidirectionalIterator<Container, DataType, Position, Traits>(container)
			{
			}

		private:

			void toEnd();

			bool isEnd() const;

			void toRBegin();

			bool isRBegin() const;

			void toREnd();

			bool isREnd() const;

			DataType *findLast(const UnaryPredicate<DataType> &predicate);

			const DataType *findLast(const UnaryPredicate<DataType> &predicate) const;

			static BidirectionalCirculator end(const Container &container);

			static BidirectionalCirculator rbegin(const Container &container);

			static BidirectionalCirculator rend(const Container &container);
	};



	/**	Constant Bidirectional Circulator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ConstBidirectionalCirculator
		: public ConstBidirectionalIterator<Container, DataType, Position, Traits>
	{
		public:

			ConstBidirectionalCirculator()
				:	ConstBidirectionalIterator<Container, DataType, Position, Traits>()
			{
			}

			ConstBidirectionalCirculator(const ConstBidirectionalCirculator &circulator)
				:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(circulator)
			{
			}

			ConstBidirectionalCirculator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
				:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
			{
			}

			bool operator +	() const
			{
				return traits_->isValid();
			}

			ConstBidirectionalCirculator &operator ++ ()
			{
				if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::isRBegin() == true)
				{
					ConstBidirectionalIterator<Container, DataType, Position, Traits>::toBegin();
				} else {
					ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator++();
				}
				return *this;
			}

			ConstBidirectionalCirculator operator ++ (int)
			{
				ConstBidirectionalCirculator circulator(*this);
				++(*this);
				return circulator;
			}

			ConstBidirectionalCirculator &operator -- ()
			{
				if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::isBegin() == true)
				{
					ConstBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin();
				} else {
					ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator--();
				}
				return *this;
			}

			ConstBidirectionalCirculator operator -- (int)
			{
				ConstBidirectionalCirculator circulator(*this);
				--(*this);
				return circulator;
			}

			const DataType *findPrevious(const UnaryPredicate<DataType> &predicate)
			{
				DataType *data = (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findPrevious(predicate);

				if (data == 0)
				{
					return ConstBidirectionalIterator<Container, DataType, Position, Traits>::findLast(predicate);
				} else {
					return data;
				}
			}

			const DataType *findPrevious(const UnaryPredicate<DataType> &predicate) const
			{
				return ((ConstBidirectionalCirculator *)this)->findPrevious(predicate);
			}

			const DataType *findNext(const UnaryPredicate<DataType> &predicate)
			{
				DataType *data = (DataType *)ConstBidirectionalIterator<Container, DataType, Position, Traits>::findNext(predicate);

				if (data == 0)
				{
					return ConstBidirectionalIterator<Container, DataType, Position, Traits>::findFirst(predicate);
				} else {
					return data;
				}
			}

			const DataType *findNext(const UnaryPredicate<DataType> &predicate) const
			{
				return ((ConstBidirectionalCirculator *)this)->findNext(predicate);
			}

			static ConstBidirectionalCirculator begin(const Container &container)
			{
				ConstBidirectionalCirculator circulator(container);
				circulator.toBegin();
				return circulator;
			}

		protected:

			ConstBidirectionalCirculator(const Container &container)
				:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(container)
			{
			}

		private:

			void toEnd();

			bool isEnd() const;

			void toRBegin();

			bool isRBegin() const;

			void toREnd();

			bool isREnd() const;

			const DataType *findLast(const UnaryPredicate<DataType> &predicate);

			const DataType *findLast(const UnaryPredicate<DataType> &predicate) const;

			static ConstBidirectionalCirculator end(const Container &container);

			static ConstBidirectionalCirculator rbegin(const Container &container);

			static ConstBidirectionalCirculator rend(const Container &container);
	};

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_BIDIRECTIONALCIRCULATOR_H
