// $Id: bidirectionalManipulator.h,v 1.3 1999/12/04 18:34:10 oliver Exp $

#ifndef BALL_CONCEPT_BIDIRECTIONALMANIPULATOR_H
#define BALL_CONCEPT_BIDIRECTIONALMANIPULATOR_H

#ifndef BALL_CONCEPT_BASEITERATOR_H
#	include <BALL/CONCEPT/baseIterator.h>
#endif

namespace BALL {

	/** Bidirectional Manipulator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class BidirectionalManipulator_
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**
		*/
		BidirectionalManipulator_()
			: BaseIterator<Container, DataType, Position, Traits>()
		{
		}
	
		/**
		*/
		BidirectionalManipulator_(const BidirectionalManipulator_& manipulator)
			:	BaseIterator<Container, DataType, Position, Traits>(manipulator)
		{
		}

		/**
		*/
		BidirectionalManipulator_(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		/**
		*/
		bool operator + () const
		{
			return traits_->isValid();
		}

		/**
		*/
		bool operator - () const
		{
			return (bool)(traits_->isValid() == false);
		}

		/**
		*/
		static void setIterators
			(const Traits& traits,
			 const typename BaseIterator<Container, DataType, Position, Traits>::Manipulation manipulation)
		{
			const Container* container = traits.getContainer();

			for (register BaseIterator<Container, DataType, Position, Traits> *iterator 
						= BaseIterator<Container, DataType, Position, Traits>::getFirstIterator_();
					 iterator != 0; iterator = iterator->next_)
			{
				if (iterator->traits_->getContainer() == container && *iterator->traits_ == traits)
				{
					switch(manipulation)
					{
						case BaseIterator<Container, DataType, Position, Traits>::BACKWARD:

							iterator->traits_->backward(traits);
							break;

						case BaseIterator<Container, DataType, Position, Traits>::FORWARD:

							iterator->traits_->forward(traits);
							break;

						case BaseIterator<Container, DataType, Position, Traits>::INVALIDATE:
						default:

							iterator->traits_->invalidate();
					}
				}
			}
		}

		/**
		*/
		static void insertBefore(Traits& traits, const DataType& data)
		{
			traits.insertBefore(data);
		}
	
		/**
		*/
		void insertBefore(const DataType& data)
		{
			traits_->insertBefore(data);
		}

		/**
		*/
		static void insertAfter(Traits& traits, const DataType& data)
		{
			traits.insertAfter(data);
		}
	
		/**
		*/
		void insertAfter(const DataType& data)
		{
			traits_->insertAfter(data);
		}

		/**
		*/
		static void remove
			(Traits traits,
			 const typename BaseIterator<Container, DataType, Position, Traits>::Manipulation manipulation 
						= (BaseIterator<Container, DataType, Position, Traits>::INVALIDATE))
		{
			if (manipulation != BaseIterator<Container, DataType, Position, Traits>::STAY)
				BidirectionalManipulator_::setIterators(traits,manipulation);
			
			traits.remove();
		}
	
		/**
		*/
		void remove
			(const typename BaseIterator<Container, DataType, Position, Traits>::Manipulation manipulation 
						= (BaseIterator<Container, DataType, Position, Traits>::INVALIDATE))
		{
			remove(*traits_, manipulation);
		}

		protected:

		BidirectionalManipulator_(const Container& container)
			:	BaseIterator<Container, DataType, Position, Traits>(container)
		{
		}

	};

} // namespace BALL

#endif // BALL_CONCEPT_BIDIRECTIONALMANIPULATOR_H
