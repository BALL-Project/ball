// $Id: forwardIterator.h,v 1.2 2000/01/03 15:15:33 oliver Exp $

#ifndef BALL_CONCEPT_FORWARDITER_H
#define BALL_CONCEPT_FORWARDITER_H

#ifndef BALL_CONCEPT_BASEITERATOR_H
#	include <BALL/CONCEPT/baseIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL 
{

	/**	@name	Generic Forward Iterator Classes.
			{\bf Definition:} \URL{BALL/CONCEPT/forwardIterator.h}
	*/
	//@{

	/**	Generic Forward Iterator Class
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ForwardIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{

		/**
		*/
		ForwardIterator()
			:	BaseIterator<Container, DataType, Position, Traits>()
		{
		}
	
		/**
		*/
		ForwardIterator(const ForwardIterator& iterator)
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		ForwardIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	BaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator + () const
		{
			return traits_ptr_->isValid();
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

		ForwardIterator& operator ++ ()
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			traits_ptr_->forward();
			return *this;
		}

		ForwardIterator operator ++ (int)
		{
			ForwardIterator iterator(*this);
			++(*this);
			return iterator;
		}

		DataType* findFirst(const UnaryPredicate<DataType>& predicate)
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			for(traits_ptr_->toBegin(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
				if (predicate((const DataType &)traits_ptr_->getData()) == true)
					return (DataType *)&(traits_ptr_->getData());

			return 0;
		}
	
		const DataType* findFirst(const UnaryPredicate<DataType>& predicate) const
		{
			return ((ForwardIterator *)this)->findFirst(predicate);
		}

		DataType* findNext(const UnaryPredicate<DataType>& predicate)
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			for(traits_ptr_->forward(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
				if (predicate((const DataType&)traits_ptr_->getData()) == true)
					return (DataType*)&(traits_ptr_->getData());

			return 0;
		}
	
		const DataType* findNext(const UnaryPredicate<DataType>& predicate) const
		{
			return ((ForwardIterator*)this)->findNext(predicate);
		}

		static ForwardIterator begin(const Container &container)
		{
			ForwardIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static ForwardIterator end(const Container &container)
		{
			ForwardIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		protected:

		ForwardIterator(const Container& container)
			:	BaseIterator<Container, DataType, Position, Traits>(container)
		{
		}
	};


	/**	Generic Constant Forward Iterator
	*/
	template <class Container, class DataType, class Position, class Traits>
	class ConstForwardIterator
		: public ConstBaseIterator<Container, DataType, Position, Traits>
	{
		public:
	
		/**	@name Constructors and Destructors
		*/
		//@{

		/**
		*/
		ConstForwardIterator()
			:	ConstBaseIterator<Container, DataType, Position, Traits>()
		{
		}
	
		/**
		*/
		ConstForwardIterator(const ConstForwardIterator &iterator)
			:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		/**
		*/
		ConstForwardIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		bool operator +	() const
		{
			return traits_ptr_->isValid();
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

		ConstForwardIterator& operator ++ ()
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			traits_ptr_->forward();
			return *this;
		}

		ConstForwardIterator operator ++ (int)
		{
			ConstForwardIterator iterator(*this);
			++(*this);
			return iterator;
		}

		const DataType *findFirst(const UnaryPredicate<DataType> &predicate)
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			for(traits_ptr_->toBegin(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
				if (predicate((const DataType &)traits_ptr_->getData()) == true)
					return (const DataType *)&(traits_ptr_->getData());

			return 0;
		}
	
		const DataType* findFirst(const UnaryPredicate<DataType>& predicate) const
		{
			return ((ConstForwardIterator*)this)->findFirst(predicate);
		}

		const DataType* findNext(const UnaryPredicate<DataType>& predicate)
		{
			if (!traits_ptr_->isValid())
				throw Exception::InvalidIterator(__FILE__, __LINE__);

			for(traits_ptr_->forward(); traits_ptr_->isEnd() == false; traits_ptr_->forward())
				if (predicate((const DataType &)traits_ptr_->getData()) == true)
					return (const DataType *)&(traits_ptr_->getData());

			return 0;
		}
	
		const DataType* findNext(const UnaryPredicate<DataType>& predicate) const
		{
			return ((ConstForwardIterator*)this)->findNext(predicate);
		}

		static ConstForwardIterator begin(const Container& container)
		{
			ConstForwardIterator iterator(container);
			iterator.toBegin();
			return iterator;
		}

		static ConstForwardIterator end(const Container &container) 
		{
			ConstForwardIterator iterator(container);
			iterator.toEnd();
			return iterator;
		}

		protected:

		ConstForwardIterator(const Container& container)
			:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
		{
		}

	};
				
	//@}

} // namespace BALL

#endif // BALL_CONCEPT_FORWARDITER_H
