// $Id: forwardManipulator.h,v 1.1 1999/08/26 07:53:16 oliver Exp $

#ifndef BALL_CONCEPT_FORWARDMANIPULATOR_H
#define BALL_CONCEPT_FORWARDMANIPULATOR_H

#ifndef BALL_CONCEPT_BASEITERATOR_H
#	include <BALL/CONCEPT/baseIterator.h>
#endif

namespace BALL {

	
	/**	Generic Forward Manipulator Class.
			{\bf Definition:} \URL{BALL/CONCEPT/forwardManipulator.h}
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ForwardManipulator_
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{

		/**
		*/
		ForwardManipulator_()
			:	BaseIterator<Container, DataType, Position, Traits>()
		{
		}
	
		/**
		*/
		ForwardManipulator_(const ForwardManipulator_ &manipulator)
			:	BaseIterator<Container, DataType, Position, Traits>(manipulator)
		{
		}

		/**
		*/
		ForwardManipulator_(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + () const
		{
			return traits_->isValid();
		}

		bool operator - () const
		{
			return (bool)(traits_->isValid() == false);
		}

		static void setIterators
			(const Traits& traits, const BaseIterator<Container, DataType, 
			 Position, Traits>::Manipulation manipulation)
		{
			const Container* container = traits.getContainer();

			for (register BaseIterator<Container, DataType, Position, Traits>* iterator 
					 = BaseIterator<Container, DataType, Position, Traits>::getFirstManipulator_();
					 iterator != 0; iterator = iterator->next_)
			{
				if (iterator->traits_->getContainer() == container
						&& *iterator->traits_ == traits)
				{
					switch(manipulation)
					{
						case BaseIterator<Container, DataType, Position, Traits>::FORWARD:
							iterator->traits_->forward(traits);
							break;
						
						case BaseIterator<Container, DataType, Position, Traits>::BACKWARD:
						case BaseIterator<Container, DataType, Position, Traits>::INVALIDATE:
						default:
							iterator->traits_->invalidate();
					}
				}		
			}
		}

		static void insertAfter(Traits& traits, const DataType& data)
		{
			traits.insertAfter(data);
		}
	
		void insertAfter(const DataType& data)
		{
			traits_->insertAfter(data);
		}

		static void erase
			(Traits traits,
			 const BaseIterator<Container, DataType, Position, Traits>::Manipulation manipulation 
				= (BaseIterator<Container, DataType, Position, Traits>::INVALIDATE))
		{
			if (manipulation != BaseIterator<Container, DataType, Position, Traits>::STAY)
			{
				ForwardManipulator_::setIterators(traits, manipulation);
			}
			
			traits.erase();
		}
	
		void erase
			(const BaseIterator<Container, DataType, Position, Traits>::Manipulation manipulation 
				= (BaseIterator<Container, DataType, Position, Traits>::INVALIDATE))
		{
			erase(*traits_, manipulation);
		}

		protected:

		ForwardManipulator_(const Container& container)
			:	BaseIterator<Container, DataType, Position, Traits>(container)
		{
		}
	};

} // namespace BALL

#endif // BALL_CONCEPT_FORWARDMANIPULATOR_H
