// $Id: baseIterator.h,v 1.4 2000/03/05 11:02:53 oliver Exp $

#ifndef BALL_CONCEPT_BASEITERATOR_H
#define BALL_CONCEPT_BASEITERATOR_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_CREATE_H
#	include <BALL/COMMON/create.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

namespace BALL 
{

	/**	Generic Iterator Class.
			This template class implements the basic behaviour of 
			an iterator.\\
			{\bf Definition:} \URL{BALL/CONCEPT/baseIterator.h}
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class BaseIterator
	{
		public:

		BALL_CREATE(BaseIterator)

		/**	@name	Constructors and Destructors 
		*/
		//@{

		/**	Default constructor.
				This constructor creates a new iterator and registers it.
		*/
		BaseIterator()
			:	previous_(0),
				next_(BaseIterator::getFirstIterator_())
		{
			traits_ptr_ = &traits_;

			if (next_ != 0)
			{
				next_->previous_ = this;
			}

			BaseIterator::getFirstIterator_() = this;
			++BaseIterator::countIterators_();
		}
	
		/**	Copy constructor.
				This constructor creates a new iterator from an existing one.
				@param iterator the iterator to be copied
		*/
		BaseIterator(const BaseIterator& iterator, bool /* deep */ = true)
			:	previous_(0),
				next_(BaseIterator::getFirstIterator_())
		{
			traits_ptr_ = &traits_;
			*traits_ptr_ = *(iterator.traits_ptr_);

			if (next_ != 0)
			{
				next_->previous_ = this;
			}

			BaseIterator::getFirstIterator_() = this;
			++BaseIterator::countIterators_();
		}

		/**	Destructor.
				Deregisters and destructs the iterator.
		*/
		virtual ~BaseIterator()
		{

			if (BaseIterator::getFirstIterator_() == this)
			{
				BaseIterator::getFirstIterator_() = next_;
			}

			if (previous_ != 0)
			{
				previous_->next_ = next_;
			}

			if (next_ != 0)
			{
				next_->previous_ = previous_;
			}

			--BaseIterator::countIterators_();
		}

		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Assigns the contents of an iterator to another iterator.
				@param	iterator the iterator to be copied
		*/
		BaseIterator& operator = (const BaseIterator &iterator)
		{
			if (this != &iterator)
			{
				*traits_ptr_ = *(iterator.traits_ptr_);
			}

			return *this;
		}

		/**	Swap two iterators.
				This method swaps the contents of two iterators of the same
				type.
		*/
		void swap(BaseIterator &iterator)
		{
			Traits *tempiteratorTraits = traits_ptr_;
			traits_ptr_ = iterator.traits_ptr_;
			iterator.traits_ptr_ = tempiteratorTraits;
		}
		//@}

		/**	@name	 Accessors 
		*/
		//@{

		/**
		*/
		static Size countIterators()
		{
			return BaseIterator::countIterators_();
		}

		/**
		*/
		static Size countIterators(const Container &container)
		{
			Size size = 0;

			for (BaseIterator* iterator = BaseIterator::getFirstIterator_(); 
					 iterator != 0; iterator = iterator->next_)
			{
				if (iterator->traits_ptr_->getContainer() == &container)
				{
					++size;
				}
			}

			return size;
		}

		/**
		*/
		static Size countIterators(const Container& container, const Position& position)
		{
			Size size = 0;

			for (BaseIterator *iterator = BaseIterator::getFirstIterator_();
					 iterator != 0; iterator = iterator->next_)
			{
				if (iterator->traits_ptr_->getContainer() == &container
						&& iterator->traits_ptr_->getPosition() == position)
				{
					++size;
				}
			}

			return size;
		}

		/**
		*/
		Size countCollisions() const
		{
			if (traits_ptr_->isSingular() == true)
			{
				return 0;
			} 
			else 
			{
				return (BaseIterator::countIterators(*traits_ptr_->getContainer(), traits_ptr_->getPosition()) - 1);
			}
		}
	
		/**
		*/
		static Size countCollisions(const Container &container, const Position &iteratorPosition)
		{
			return (BaseIterator::countIterators(container, iteratorPosition) - 1);
		}
	
		/**
		*/
		void invalidate()
		{
			traits_ptr_->invalidate();
		}

		/**
		*/
		void setTraits(const Traits &iteratorTraits)
		{
			*traits_ptr_ = iteratorTraits;
		}

		/**
		*/
		Traits& getTraits()
		{
			return *traits_ptr_;
		}

		/**
		*/
		const Traits& getTraits() const
		{
			return *traits_ptr_;
		}

		/**
		*/
		Container* getContainer()
		{
			return traits_ptr_->getContainer();
		}

		/**
		*/
		const Container* getContainer() const
		{
			return traits_ptr_->getContainer();
		}

		/**
		*/
		operator Position& ()
		{
			return traits_ptr_->getPosition();
		}

		/**
		*/
		DataType& operator * ()
		{
			if (!traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}

			return (DataType &)traits_ptr_->getData();
		}

		/**
		*/
		const DataType& operator *() const
		{
			if (!traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
 
			return (const DataType &)traits_ptr_->getData();
		}

		/**
		*/
		DataType* operator -> ()
		{
			if (!traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
 
			return (DataType *)&(traits_ptr_->getData());
		}

		/**
		*/
		const DataType* operator -> () const
		{
			if (!traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
 
			return (const DataType *)&(traits_ptr_->getData());
		}
		//@}

	
		/**	@name	Predicates
		*/
		//@{

		/**
		*/
		bool operator == (const BaseIterator& iterator) const
		{
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
			{
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);
			}

			return (bool)(*traits_ptr_ == *iterator.traits_ptr_);
		}

		/**
		*/
		bool operator != (const BaseIterator &iterator) const
		{
			if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
			{
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);
			}

			return (bool)(*traits_ptr_ != *iterator.traits_ptr_);
		}

		/**
		*/
		bool isSingular() const
		{
			return traits_ptr_->isSingular();
		}

		/**
		*/
		bool isUnique() const
		{
			if (traits_ptr_->isValid() == false)
			{
				return true;
			}	
			else 
			{
				return (bool)(BaseIterator::countIterators(*traits_ptr_->getContainer(), traits_ptr_->getPosition()) == 1);
			}
		}
	
		/**
		*/
		static bool isUnique(const Container &container, const Position &iteratorPosition)
		{
			return (bool)(BaseIterator::countIterators(container, iteratorPosition) == 1);
		}
	
		/**
		*/
		bool isValid() const
		{
			return traits_ptr_->isValid();
		}
		//@}


		protected:

		BaseIterator(const Container& container)
			:	traits_(container),
				previous_(0),
				next_(0)
		{
			traits_ptr_ = &traits_;

			++BaseIterator::countIterators_();
		}

		static BaseIterator*& getFirstIterator_();

		static Size& countIterators_();


		private:

		Traits					traits_;

		
		public:

		Traits*					traits_ptr_;

		BaseIterator* 	previous_;

		BaseIterator* 	next_;
	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	BaseIterator<Container, DataType, Position, Traits>*&
	BaseIterator<Container, DataType, Position, Traits>::getFirstIterator_()
	{
		static BaseIterator* first = 0;
		
		return first;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	Size& BaseIterator<Container, DataType, Position, Traits>::countIterators_()
	{
		static Size size = 0;
		
		return size;
	}

	/**	Constant Basic Iterator.
			{\bf Definition:} \URL{BALL/CONCEPT/baseIterator.h}
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstBaseIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		ConstBaseIterator()
			: BaseIterator<Container, DataType, Position, Traits>()
		{
		}
	
		/**	Default constructor
		*/
		ConstBaseIterator(const ConstBaseIterator &iterator)
			:	BaseIterator<Container,  DataType,  Position,  Traits>(iterator)
		{
		}

		/**	Default constructor
		*/
		ConstBaseIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**
		*/
		const Traits& getTraits()
		{
			return *traits_ptr_;
		}

		/**
		*/
		const Container* getContainer()
		{
			return traits_ptr_->getContainer();
		}

		/**
		*/
		operator const Position & ()
		{
			return traits_ptr_->getPosition();
		}

		/**
		*/
		const DataType& operator * ()
		{
			if (!traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
 
			return (const DataType &)traits_ptr_->getData();
		}

		/**
		*/
		const DataType* operator -> ()
		{
			if (!traits_ptr_->isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
 
			return (const DataType *)&(traits_ptr_->getData());
		}
		//@}

		protected:

		ConstBaseIterator(const Container& container)
			:	BaseIterator<Container, DataType, Position, Traits>(container)
		{
		}

		private:

	};

} // namespace BALL

#endif // BALL_CONCEPT_BASEITERATOR_H
