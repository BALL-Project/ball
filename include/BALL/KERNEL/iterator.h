// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: iterator.h,v 1.22 2003/06/12 15:42:23 oliver Exp $
//

#ifndef BALL_KERNEL_ITERATOR_H
#define BALL_KERNEL_ITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/bidirectionalIterator.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif


/** A macro defining kernel iterators.
		This macro allows th definition of arbitrary bidirectional
		iterators for kernel container classes.
		Multiple iterator definition macros can be included at the same
		time, thus enabling the iteration over different types of 
		kernel objects. Each macro defines beginX()/endX() (const and mutable)
		and rendX()/rbeginX() where X is the type of item to iterate over.
*/
#define BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Type) \
Type##Iterator \
begin##Type () \
{ \
  return Type##Iterator :: begin (*this); \
} \
 \
Type##Iterator \
end##Type () \
{ \
  return Type##Iterator::end(*this); \
} \
 \
Type##ReverseIterator \
rbegin##Type () \
{ \
  return Type##ReverseIterator(end##Type ()); \
} \
 \
Type##ReverseIterator \
rend##Type () \
{ \
  return Type##ReverseIterator(begin##Type ()); \
} \
 \
Type##ConstIterator \
begin##Type () const \
{ \
  return Type##ConstIterator::begin(*this); \
} \
 \
Type##ConstIterator \
end##Type () const \
{ \
  return Type##ConstIterator::end(*this); \
} \
 \
Type##ConstReverseIterator \
rbegin##Type () const \
{ \
  return Type##ConstReverseIterator(end##Type ()); \
} \
 \
Type##ConstReverseIterator \
rend##Type () const \
{ \
  return Type##ConstReverseIterator(begin##Type ()); \
}

namespace BALL 
{
	/**	Composite Iterator Traits
			This class is used in the implementation of the kernel iterators.
			It is intended for internal use only.
			 \par
			
    	\ingroup  KernelIterators
	*/
	class CompositeIteratorTraits
	{
		public:

		/** @name Constructors and Destructor
		*/
		//@{

		/// Default constructor
		inline CompositeIteratorTraits() throw();

		/// Copy constructor
		inline CompositeIteratorTraits(const Composite& composite) throw();

		/// Detailed constructor
		inline CompositeIteratorTraits(const CompositeIteratorTraits& traits) throw();

		/// Destructor
		inline ~CompositeIteratorTraits()	throw() {}

		//@}
		/** @name Assignment
		*/
		//@{

		/// Assignment operator
		inline CompositeIteratorTraits& operator = (const CompositeIteratorTraits& traits) throw();
		//@}

		/** @name Accessors
		*/
		//@{

		/// Return a pointer to the container the iterator is bound to
		Composite* getContainer()	throw() { return bound_; }
	
		/// Return a const pointer to the container the iterator is bound to
		inline const Composite* getContainer() const throw() { return bound_; }
		//@}

		/** @name Predicates
		*/
		//@{

		/// Equality operator.
		inline bool operator == (const CompositeIteratorTraits& traits) const	throw();

		/// Inequality operator.
		inline bool operator != (const CompositeIteratorTraits& traits) const	throw();
		//@}

		/**	@name Predicates
		*/
		//@{
		/** Return the current status of the iterator.
				If the iterator is bound and its subcomposite iterator is valid,
				this predicate returns <tt>true</tt>.
		*/
		inline bool isValid() const throw();

		/// Check whether the iterator is bound to a container.
		inline bool isSingular() const	throw() { return bound_ == 0; }
	
		/** Return true if the iterator is at the first element of the container.
				@exception InvalidIterator if the iterator is {\em singular}
				@see isSingular
		*/
		inline bool isBegin() const throw(Exception::InvalidIterator);

		/** Return true if the iterator is beyond the last element of the container.
				@exception InvalidIterator if the iterator is {\em singular}
				@see isSingular
		*/
		inline bool isEnd() const throw();

		/** Return true if the iterator is at the last element of the container.
				@exception InvalidIterator if the iterator is {\em singular}
				@see isSingular
		*/
		inline bool isRBegin() const throw(Exception::Precondition);
			
		/** Return true if the iterator is beyond the first element of the container.
				@exception InvalidIterator if the iterator is {\em singular}
				@see isSingular
		*/
		inline bool isREnd() const	throw();
		//@}
	
		/// Return the current iterator position
		inline Composite::SubcompositeIterator& getPosition() throw() { return subcomposite_iterator_; }
	
		/// Return the current iterator posittion (const method)
		inline const Composite::SubcompositeIterator& getPosition() const throw() { return subcomposite_iterator_; }
	
		/**	Invalidate the iterator.
				The iterator is separated from its container ( \link isSingular isSingular \endlink  is <b>true</b> afterwards)
				and its  \link SubCompositeIterator SubCompositeIterator \endlink  is invalidated as well.
		*/
		inline void invalidate() throw();

		/**	Reposition the iterator to the first element of the container.
				@exception InvalidIterator if the iterator is {\em singular}.
				@see isSingular
		*/
		inline void toBegin() throw(Exception::InvalidIterator);

		/**	Reposition the iterator after the last element of the container.
				@exception InvalidIterator if the iterator is {\em singular}.
				@see isSingular
		*/
		inline void toEnd() throw();

		/// Return a reference to the current element
		inline Composite& getData() throw();
	
		/// Return a const reference to the current element
		inline const Composite& getData() const throw();
	
		/// Increment the iterator by one element.
		inline void forward() throw();
	
		/// Decrement the iterator one element
		inline void backward() throw();
			
		/// Reposition the (backward) iterator to the last element of the container
		inline void toRBegin() throw(Exception::Precondition);

		/// Reposition the (backward) iterator beyond the first element of the container
		inline void toREnd() throw();

		/// Assign the current predicate associated with the iterator
		inline void setPredicate(const UnaryPredicate<Composite>& predicate) throw() { predicate_ = &predicate; }
			
		/// Return the current predicate associated with the iterator
		inline const UnaryPredicate<Composite>* getPredicate() const	throw() { return predicate_; }


		protected:

		/// The pointer to the container
		Composite* bound_;

		/// The internal iterator to iterate over the current node's children
		Composite::SubcompositeIterator subcomposite_iterator_;

		/// The predicate
		const UnaryPredicate<Composite>* predicate_;
	};

	inline CompositeIteratorTraits::CompositeIteratorTraits()
		throw()
		:	bound_(0),
			subcomposite_iterator_(),
			predicate_(0)
	{
	}
		
	inline CompositeIteratorTraits::CompositeIteratorTraits(const Composite& composite)
		throw()
		:	bound_((Composite *)&composite),
			subcomposite_iterator_(composite.beginSubcomposite()),
			predicate_(0)
	{
	}
		
	inline
	CompositeIteratorTraits::CompositeIteratorTraits(const CompositeIteratorTraits &traits)
		throw()
		:	bound_(traits.bound_),
			subcomposite_iterator_(traits.subcomposite_iterator_),
			predicate_(traits.predicate_)
	{
	}

	inline
	CompositeIteratorTraits& CompositeIteratorTraits::operator = (const CompositeIteratorTraits& traits)
		throw()
	{
		bound_ = traits.bound_;
		subcomposite_iterator_ = traits.subcomposite_iterator_;
		predicate_ = traits.predicate_;
		return *this;
	}

	inline
	bool CompositeIteratorTraits::operator == (const CompositeIteratorTraits &traits) const
		throw()
	{
		return (subcomposite_iterator_ == traits.subcomposite_iterator_);
	}

	inline
	bool CompositeIteratorTraits::operator !=(const CompositeIteratorTraits &traits) const
		throw()
	{
		return (subcomposite_iterator_ != traits.subcomposite_iterator_);
	}

	inline
	bool CompositeIteratorTraits::isValid() const
		throw()
	{
		return (bound_ != 0 && subcomposite_iterator_.isValid() == true);
	}

	inline
	void CompositeIteratorTraits::invalidate()
		throw()
	{
		bound_ = 0;
		subcomposite_iterator_.invalidate();
	}

	inline
	void CompositeIteratorTraits::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		
		subcomposite_iterator_ = bound_->beginSubcomposite();
		while ((predicate_->operator () (*subcomposite_iterator_) == false)
					 && (+subcomposite_iterator_))
		{
			++subcomposite_iterator_;
		}
	}

	inline
	bool CompositeIteratorTraits::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		Composite::SubcompositeIterator sub_iterator = bound_->beginSubcomposite();
		while ((predicate_->operator () (*sub_iterator) == false)
					 && (+sub_iterator))
		{
			++sub_iterator;
		}
		return (subcomposite_iterator_ == sub_iterator);
	}

	inline void CompositeIteratorTraits::toEnd()
		throw()
	{
		subcomposite_iterator_.toEnd();
	}

	inline bool CompositeIteratorTraits::isEnd() const
		throw()
	{
		return subcomposite_iterator_.isEnd();
	}

	inline Composite& CompositeIteratorTraits::getData()	throw()
	{
		return const_cast<Composite&>(*subcomposite_iterator_);
	}

	inline const Composite& CompositeIteratorTraits::getData() const	throw()
	{
		return *subcomposite_iterator_;
	}

	inline void CompositeIteratorTraits::forward()	throw()
	{
		++subcomposite_iterator_;
		while ((predicate_->operator () (*subcomposite_iterator_) == false)
					 && +subcomposite_iterator_)
		{
			++subcomposite_iterator_;
		}
	}

	inline void CompositeIteratorTraits::toRBegin() throw(Exception::Precondition)
	{
		BALL_PRECONDITION_EXCEPTION(isSingular(), "singular iterator")
		subcomposite_iterator_ = --bound_->endSubcomposite();
		while ((predicate_->operator () (*subcomposite_iterator_) == false)
					 && (+subcomposite_iterator_))
		{
			--subcomposite_iterator_;
		}
	}

	inline bool CompositeIteratorTraits::isRBegin() const
		throw(Exception::Precondition)
	{
		BALL_PRECONDITION_EXCEPTION(isSingular(), "singular iterator")
		Composite::SubcompositeIterator sub_iterator = --bound_->endSubcomposite();

		while ((predicate_->operator () (*sub_iterator) == false)
					 && +sub_iterator)
		{
			--sub_iterator;
		}
		return (subcomposite_iterator_ == sub_iterator);
	}

	inline void CompositeIteratorTraits::toREnd() throw()
	{
		subcomposite_iterator_.toREnd();
	}

	inline bool CompositeIteratorTraits::isREnd() const throw()
	{
		return subcomposite_iterator_.isREnd();
	}

	inline void CompositeIteratorTraits::backward() throw()
	{
		--subcomposite_iterator_;
		while ((predicate_->operator () (*subcomposite_iterator_) == false)
					 && +subcomposite_iterator_)
		{
			--subcomposite_iterator_;
		}
	}

} // namespace BALL

#endif // BALL_KERNEL_ITERATOR_H
