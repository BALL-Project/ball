// $Id: forwardCirculator.h,v 1.1 1999/08/26 07:53:16 oliver Exp $

#ifndef BALL_CONCEPT_FORWARDCIRCULATOR_H
#define BALL_CONCEPT_FORWARDCIRCULATOR_H

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL {

	/**	@name	Generic Forward Circulator Classes.
			{\bf Definition:} \URL{BALL/CONCEPT/forwardCirculator.h}
	*/
	//@{
	
	/**	Generic Forward Circular Manipulator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ForwardCircularManipulator
		: public ForwardManipulator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name	Constructors and Destructors
		*/	
		//@{

		/**	
		*/
		ForwardCircularManipulator(void)
			:	ForwardManipulator<Container, DataType, Position, Traits>()
		{
		}

		/**	
		*/
		ForwardCircularManipulator(const ForwardCircularManipulator &manipulator)
			:	ForwardManipulator<Container, DataType, Position, Traits>(manipulator)
		{
		}

		/**	
		*/
		ForwardCircularManipulator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	ForwardManipulator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator +	(void) const
		{
			return traits_->isValid();
		}

		ForwardCircularManipulator &operator ++ (void)
		{
			ForwardManipulator<Container, DataType, Position, Traits>::operator++();

			if (ForwardManipulator<Container, DataType, Position, Traits>::isValid() == false)
				ForwardManipulator<Container, DataType, Position, Traits>::toBegin();

			return *this;
		}

		ForwardCircularManipulator operator ++ (int)
		{
			ForwardCircularManipulator manipulator(*this);
			++(*this);
			return manipulator;
		}

		DataType *findNext(const TUnaryPredicate<DataType> &predicate)
		{
			DataType *data = (DataType *)ForwardManipulator<Container, DataType, Position, Traits>::findNext(predicate);

			if (data == 0)
			{
				return ForwardManipulator<Container, DataType, Position, Traits>::findFirst(predicate);
			} else {
				return data;
			}
		}

		const DataType *findNext(const TUnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)((TUnaryPredicate<DataType> *)this)->findNext(predicate);
		}

		static ForwardCircularManipulator begin(const Container &container)
		{
			ForwardCircularManipulator manipulator(container);
			manipulator.toBegin();
			return manipulator;
		}

		protected:

		ForwardCircularManipulator(const Container &container)
			:	ForwardManipulator<Container, DataType, Position, Traits>(container)
		{
		}

		private:

		void toEnd(void);

		bool isEnd(void) const;

		static ForwardCircularManipulator end(const Container &container);
	};


	/**	Generic Forward Circulator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ForwardCirculator
		: public ForwardIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name	Constructors and Destructors
		*/	
		//@{
		
		/**	
		*/
		ForwardCirculator(void)
			:	ForwardIterator<Container, DataType, Position, Traits>()
		{
		}

		/**	
		*/
		ForwardCirculator(const ForwardCirculator &circulator)
			: ForwardIterator<Container, DataType, Position, Traits>(circulator)
		{
		}

		/**	
		*/
		ForwardCirculator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	ForwardIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + (void) const
		{
			return traits_->isValid();
		}

		bool operator - (void) const
		{
			return (bool)(traits_->isValid() == false);
		}

		ForwardCirculator &operator ++ (void)
		{
			ForwardIterator<Container, DataType, Position, Traits>::operator++();

			if (ForwardIterator<Container, DataType, Position, Traits>::isValid() == false)
				ForwardIterator<Container, DataType, Position, Traits>::toBegin();

			return *this;
		}

		ForwardCirculator operator ++ (int)
		{
			ForwardCirculator circulator(*this);
			++(*this);
			return circulator;
		}

		DataType *findNext(const TUnaryPredicate<DataType> &predicate)
		{
			DataType *data = (DataType *)ForwardIterator<Container, DataType, Position, Traits>::findNext(predicate);

			if (data == 0)
			{
				return ForwardIterator<Container, DataType, Position, Traits>::findFirst(predicate);
			} else {
				return data;
			}
		}

		const DataType *findNext(const TUnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)((ForwardCirculator *)this)->findNext(predicate);
		}

		static ForwardCirculator begin(const Container &container)
		{
			ForwardCirculator circulator(container);
			circulator.toBegin();
			return circulator;
		}

		protected:

		ForwardCirculator(const Container &container)
			:	ForwardIterator<Container, DataType, Position, Traits>(container)
		{
		}

		private:

		void toEnd(void);

		bool isEnd(void) const;

		static ForwardCirculator end(const Container &container);
	};


	/**	Generic Constant Forward Circulator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstForwardCirculator
		: public ConstForwardIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	
		*/
		ConstForwardCirculator(void)
			:	ConstForwardIterator<Container, DataType, Position, Traits>()
		{
		}

		/**	
		*/
		ConstForwardCirculator(const ConstForwardCirculator &circulator)
			:	ConstForwardIterator<Container, DataType, Position, Traits>(circulator)
		{
		}

		/**	
		*/
		ConstForwardCirculator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	ConstForwardIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + (void) const
		{
			return traits_->isValid();
		}

		ConstForwardCirculator &operator ++ (void)
		{
			ConstForwardIterator<Container, DataType, Position, Traits>::operator++();

			if (ConstForwardIterator<Container, DataType, Position, Traits>::isValid() == false)
				ConstForwardIterator<Container, DataType, Position, Traits>::toBegin();

			return *this;
		}

		ConstForwardCirculator operator ++ (int)
		{
			ConstForwardCirculator circulator(*this);
			++(*this);
			return circulator;
		}

		const DataType *findNext(const TUnaryPredicate<DataType> &predicate)
		{
			DataType *data = (DataType *)ConstForwardIterator<Container, DataType, Position, Traits>::findNext(predicate);

			if (data == 0)
			{
				return ConstForwardIterator<Container, DataType, Position, Traits>::findFirst(predicate);
			} else {
				return data;
			}
		}

		const DataType *findNext(const TUnaryPredicate<DataType> &predicate) const
		{
			return (const DataType *)((ConstForwardCirculator *)this)->findNext(predicate);
		}

		static ConstForwardCirculator begin(const Container &container)
		{
			ConstForwardCirculator circulator(container);
			circulator.toBegin();
			return circulator;
		}

		protected:

		ConstForwardCirculator(const Container &container)
			:	ConstForwardIterator<Container, DataType, Position, Traits>(container)
		{
		}

		private:

		void toEnd(void);

		bool isEnd(void) const;

		static ConstForwardCirculator end(const Container &container);
	};
	
	//@}

} // namespace BALL

#endif // BALL_CONCEPT_FORWARDCIRCULATOR_H
