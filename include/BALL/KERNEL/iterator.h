// $Id: iterator.h,v 1.11 2001/07/15 20:52:55 oliver Exp $

#ifndef BALL_KERNEL_ITERATOR_H
#define BALL_KERNEL_ITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/reverseBidirectionalIterator.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

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
  return Type##ReverseIterator::begin(*this); \
} \
 \
Type##ReverseIterator \
rend##Type () \
{ \
  return Type##ReverseIterator::end(*this); \
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
  return Type##ConstReverseIterator::begin(*this); \
} \
 \
Type##ConstReverseIterator \
rend##Type () const \
{ \
  return Type##ConstReverseIterator::end(*this); \
}

namespace BALL 
{

	/**	Composite Iterator Traits.	
			This class is used in the implementation of the kernel iterators.
			It is intended for internal use only.
			\\
			{\bf Definition:} \URL{BALL/kernel/iterator.h}
			
	*/
	class CompositeIteratorTraits_
	{
		public:

		/** @name Constructors and Destructor
		*/
		//@{

		/// Default constructor
		CompositeIteratorTraits_()
			throw();

		/// Copy constructor
		CompositeIteratorTraits_(const Composite& composite)
			throw();

		/// Detailed constructor
		CompositeIteratorTraits_(const CompositeIteratorTraits_& traits)
			throw();

		/// Destructor
		virtual ~CompositeIteratorTraits_()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/// Assignment operator
		CompositeIteratorTraits_ &operator = (const CompositeIteratorTraits_& traits)
			throw();

		//@}
		/** @name Accessors
		*/
		//@{

		/// Return a pointer to the container the iterator is bound to
		Composite *getContainer()
			throw();
	
		/// Return a const pointer to the container the iterator is bound to
		const Composite* getContainer() const
			throw();
	
		//@}
		/** @name Predicates
		*/
		//@{

		/// Equality operator.
		bool operator == (const CompositeIteratorTraits_& traits) const
			throw();

		/// Inequality operator.
		bool operator != (const CompositeIteratorTraits_& traits) const
			throw();
	
		//@}

		/**	@name Predicates
		*/
		//@{
		/** Return the current status of the iterator.
				If the iterator is bound and its subcomposite iterator is valid,
				this predicate returns {\tt true}.
		*/
		bool isValid() const
			throw();

		/// Check whether the iterator is bound to a container.
		bool isSingular() const
			throw();
	
		/** Return true if the iterator is at the first element of the container.
				@exception InvalidIterator if the iterator is {\em singular}
				@see isSingular
		*/
		bool isBegin() const
			throw(Exception::InvalidIterator);

		/** Return true if the iterator is beyond the last element of the container.
				@exception InvalidIterator if the iterator is {\em singular}
				@see isSingular
		*/
		bool isEnd() const
			throw();

		/** Return true if the iterator is at the last element of the container.
				@exception InvalidIterator if the iterator is {\em singular}
				@see isSingular
		*/
		bool isRBegin() const
			throw(Exception::InvalidIterator);
			
		/** Return true if the iterator is beyond the first element of the container.
				@exception InvalidIterator if the iterator is {\em singular}
				@see isSingular
		*/
		bool isREnd() const
			throw();
		//@}
	
		/// Return the current iterator position
		Composite::SubcompositeIterator& getPosition()
			throw();
	
		/// Return the current iterator posittion (const method)
		const Composite::SubcompositeIterator& getPosition() const
			throw();
	
		/**	Invalidate the iterator.
				The iterator is separated from its container (\Ref{isSingular} is {\bf true} afterwards)
				and its \Ref{SubCompositeIterator} is invalidated as well.
		*/
		void invalidate()
			throw();

		/**	Reposition the iterator to the first element of the container.
				@exception InvalidIterator if the iterator is {\em singular}.
				@see isSingular
		*/
		void toBegin()
			throw(Exception::InvalidIterator);

		/**	Reposition the iterator after the last element of the container.
				@exception InvalidIterator if the iterator is {\em singular}.
				@see isSingular
		*/
		void toEnd()
			throw();

		/// Return a reference to the current element
		Composite& getData()
			throw();
	
		/// Return a const reference to the current element
		const Composite& getData() const
			throw();
	
		/// Increment the iterator by one element.
		void forward()
			throw();
	
		/// Decrement the iterator one element
		void backward()
			throw();
			
		/// Reposition the (backward) iterator to the last element of the container
		void toRBegin()
			throw(Exception::InvalidIterator);

		/// Reposition the (backward) iterator beyond the first element of the container
		void toREnd()
			throw();

		/// Assign the current predicate associated with the iterator
		void setPredicate(const UnaryPredicate<Composite>& predicate)
			throw();
			
		/// Return the current predicate associated with the iterator
		const UnaryPredicate<Composite>* getPredicate() const
			throw();


		protected:

		/*_ The pointer to the container
		*/
		Composite*												bound_;

		/*_ The internal iterator to iterate over the current node's children
		*/
		Composite::SubcompositeIterator 	subcomposite_iterator_;

		/*_ The predicate
		*/
		const UnaryPredicate<Composite>*	predicate_;

	};

	inline
	CompositeIteratorTraits_::CompositeIteratorTraits_()
		throw()
		:	bound_(0),
			subcomposite_iterator_(),
			predicate_(0)
	{
	}
		
	inline
	CompositeIteratorTraits_::CompositeIteratorTraits_(const Composite& composite)
		throw()
		:	bound_((Composite *)&composite),
			subcomposite_iterator_(composite.beginSubcomposite()),
			predicate_(0)
	{
	}
		
	inline
	CompositeIteratorTraits_::CompositeIteratorTraits_(const CompositeIteratorTraits_ &traits)
		throw()
		:	bound_(traits.bound_),
			subcomposite_iterator_(traits.subcomposite_iterator_),
			predicate_(traits.predicate_)
	{
	}

	inline
	CompositeIteratorTraits_::~CompositeIteratorTraits_()
		throw()
	{
	}

	inline
	CompositeIteratorTraits_& CompositeIteratorTraits_::operator = (const CompositeIteratorTraits_& traits)
		throw()
	{
		bound_ = traits.bound_;
		subcomposite_iterator_ = traits.subcomposite_iterator_;
		predicate_ = traits.predicate_;
		return *this;
	}

	inline
	Composite* CompositeIteratorTraits_::getContainer()
		throw()
	{
		return bound_;
	}

	inline
	const Composite* CompositeIteratorTraits_::getContainer() const
		throw()
	{
		return bound_;
	}

	inline
	bool CompositeIteratorTraits_::isSingular() const
		throw()
	{
		return (bound_ == 0);
	}

	inline
	Composite::SubcompositeIterator& CompositeIteratorTraits_::getPosition()
		throw()
	{
		return subcomposite_iterator_;
	}

	inline
	const Composite::SubcompositeIterator& CompositeIteratorTraits_::getPosition() const
		throw()
	{
		return subcomposite_iterator_;
	}

	inline
	bool CompositeIteratorTraits_::operator == (const CompositeIteratorTraits_ &traits) const
		throw()
	{
		return (subcomposite_iterator_ == traits.subcomposite_iterator_);
	}

	inline
	bool CompositeIteratorTraits_::operator !=(const CompositeIteratorTraits_ &traits) const
		throw()
	{
		return (subcomposite_iterator_ != traits.subcomposite_iterator_);
	}

	inline
	bool CompositeIteratorTraits_::isValid() const
		throw()
	{
		return (bound_ != 0 && subcomposite_iterator_.isValid() == true);
	}

	inline
	void CompositeIteratorTraits_::invalidate()
		throw()
	{
		bound_ = 0;
		subcomposite_iterator_.invalidate();
	}

	inline
	void CompositeIteratorTraits_::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		
		subcomposite_iterator_ = bound_->beginSubcomposite();
		if (predicate_->operator()(*subcomposite_iterator_) == false)
		{
			subcomposite_iterator_.findNext(*predicate_);
		}
	}

	inline
	bool CompositeIteratorTraits_::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		Composite::SubcompositeIterator sub_iterator = bound_->beginSubcomposite();

		if (predicate_->operator()(*subcomposite_iterator_) == false)
		{
			sub_iterator.findNext(*predicate_);
		}
		return (subcomposite_iterator_ == sub_iterator);
	}

	inline
	void CompositeIteratorTraits_::toEnd()
		throw()
	{
		subcomposite_iterator_.toEnd();
	}

	inline
	bool CompositeIteratorTraits_::isEnd() const
		throw()
	{
		return subcomposite_iterator_.isEnd();
	}

	inline
	Composite& CompositeIteratorTraits_::getData()
		throw()
	{
		return *subcomposite_iterator_;
	}

	inline
	const Composite& CompositeIteratorTraits_::getData() const
		throw()
	{
		return *subcomposite_iterator_;
	}

	inline
	void CompositeIteratorTraits_::forward()
		throw()
	{
		subcomposite_iterator_.findNext(*predicate_);
	}

	inline
	void CompositeIteratorTraits_::toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		subcomposite_iterator_ = bound_->rbeginSubcomposite();
		if (predicate_->operator()(*subcomposite_iterator_) == false)
		{
			subcomposite_iterator_.findPrevious(*predicate_);
		}
	}

	inline
	bool CompositeIteratorTraits_::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		if (isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		Composite::SubcompositeIterator sub_iterator = bound_->rbeginSubcomposite();

		if (predicate_->operator()(*subcomposite_iterator_) == false)
		{
			sub_iterator.findPrevious(*predicate_);
		}
		return (subcomposite_iterator_ == sub_iterator);
	}

	inline
	void CompositeIteratorTraits_::toREnd()
		throw()
	{
		subcomposite_iterator_.toREnd();
	}

	inline
	bool CompositeIteratorTraits_::isREnd() const
		throw()
	{
		return subcomposite_iterator_.isREnd();
	}

	inline
	void CompositeIteratorTraits_::backward()
		throw()
	{
		subcomposite_iterator_.findPrevious(*predicate_);
	}

	inline
	void CompositeIteratorTraits_::setPredicate(const UnaryPredicate<Composite>& predicate)
		throw()
	{
		predicate_ = &predicate;
	}

	inline
	const UnaryPredicate<Composite>* CompositeIteratorTraits_::getPredicate() const
		throw()
	{
		return predicate_;
	}

} // namespace BALL

#endif // BALL_KERNEL_ITERATOR_H
