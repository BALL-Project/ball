// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_ITERATOR_H
#define BALL_KERNEL_ITERATOR_H

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
  return Type##Iterator::begin(*this); \
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
	class BALL_EXPORT CompositeIteratorTraits
	{
		public:

		/** @name Constructors and Destructor
		*/
		//@{

		/// Default constructor
		inline CompositeIteratorTraits();

		/// Copy constructor
		inline CompositeIteratorTraits(const Composite& composite);

		/// Detailed constructor
		inline CompositeIteratorTraits(const CompositeIteratorTraits& traits);

		/// Destructor
		inline ~CompositeIteratorTraits() {}

		//@}
		/** @name Assignment
		*/
		//@{

		/// Assignment operator
		inline CompositeIteratorTraits& operator = (const CompositeIteratorTraits& traits);
		//@}

		/** @name Accessors
		*/
		//@{

		/// Return a pointer to the container the iterator is bound to
		Composite* getContainer() { return bound_; }
	
		/// Return a const pointer to the container the iterator is bound to
		inline const Composite* getContainer() const { return bound_; }
		//@}

		/** @name Predicates
		*/
		//@{

		/// Equality operator.
		inline bool operator == (const CompositeIteratorTraits& traits) const;

		/// Inequality operator.
		inline bool operator != (const CompositeIteratorTraits& traits) const;
		//@}

		/**	@name Predicates
		*/
		//@{
		/** Return the current status of the iterator.
				If the iterator is bound and its subcomposite iterator is valid,
				this predicate returns <tt>true</tt>.
		*/
		inline bool isValid() const { return ((bound_ != 0) && composite_iterator_.isValid()); }

		/// Check whether the iterator is bound to a container.
		inline bool isSingular() const { return (bound_ == 0); }
	
		/** Return true if the iterator is at the first element of the container.
				@see isSingular
		*/
		inline bool isBegin() const;

		/** Return true if the iterator is beyond the last element of the container.
				@exception InvalidIterator if the iterator is <b>  singular </b>
				@see isSingular
		*/
		inline bool isEnd() const 
		{ 
			return composite_iterator_.isEnd(); 
		}

		/** Return true if the iterator is at the last element of the container.
				@exception InvalidIterator if the iterator is <b>  singular </b>
				@see isSingular
		*/
		inline bool isRBegin() const;
			
		/** Return true if the iterator is beyond the first element of the container.
				@exception InvalidIterator if the iterator is <b>  singular </b>
				@see isSingular
		*/
		inline bool isREnd() const;
		//@}
	
		/// Return the current iterator position
		inline Composite::CompositeIterator& getPosition() { return composite_iterator_; }
	
		/// Return the current iterator posittion (const method)
		inline const Composite::CompositeIterator& getPosition() const { return composite_iterator_; }
	
		/**	Invalidate the iterator.
				The iterator is separated from its container ( \link isSingular isSingular \endlink  is <b>true</b> afterwards)
				and its  \link SubCompositeIterator SubCompositeIterator \endlink  is invalidated as well.
		*/
		inline void invalidate();

		/**	Reposition the iterator to the first element of the container.
		 *	@throw Exception::Precondition if the iterator is unbound
		 *	@see isSingular
		 */
		inline void toBegin();

		/**	Reposition the iterator after the last element of the container.
		 *	@throw Exception::Precondition if the iterator is unbound
		 *	@see isSingular
		 */
		inline void toEnd();

		/// Return a reference to the current element
		inline Composite& getData();
	
		/// Return a const reference to the current element
		inline const Composite& getData() const;
	
		/// Increment the iterator by one element.
		inline void forward();
	
		/// Decrement the iterator one element
		inline void backward();
			
		/** Reposition the (backward) iterator to the last element of the container
		 *  @throw Exception::Precondition if the iterator is unbound
		 */
		inline void toRBegin();

		/** Reposition the (backward) iterator beyond the first element of the container
		 *  @throw Exception::Precondition if the iterator is unbound
		 */
		inline void toREnd();

		/// Assign the current predicate associated with the iterator
		inline void setPredicate(const UnaryPredicate<Composite>& predicate) { predicate_ = &predicate; }
			
		/// Return the current predicate associated with the iterator
		inline const UnaryPredicate<Composite>* getPredicate() const { return predicate_; }


		protected:

		/// The pointer to the container
		Composite* bound_;

		/// The internal iterator to iterate over the current node's children
		Composite::CompositeIterator composite_iterator_;

		/// The predicate
		const UnaryPredicate<Composite>* predicate_;
	};

	inline CompositeIteratorTraits::CompositeIteratorTraits()
		:	bound_(0),
			composite_iterator_(),
			predicate_(0)
	{
	}
		
	inline CompositeIteratorTraits::CompositeIteratorTraits(const Composite& composite)
		:	bound_(const_cast<Composite*>(&composite)),
			composite_iterator_(const_cast<Composite&>(composite).beginComposite()),
			predicate_(0)
	{
	}
		
	inline CompositeIteratorTraits::CompositeIteratorTraits(const CompositeIteratorTraits& traits)
		:	bound_(traits.bound_),
			composite_iterator_(traits.composite_iterator_),
			predicate_(traits.predicate_)
	{
	}

	inline CompositeIteratorTraits& CompositeIteratorTraits::operator = (const CompositeIteratorTraits& traits)
	{
		bound_ = traits.bound_;
		composite_iterator_ = traits.composite_iterator_;
		predicate_ = traits.predicate_;
		return *this;
	}

	inline bool CompositeIteratorTraits::operator == (const CompositeIteratorTraits& traits) const
	{
		return ((composite_iterator_ == traits.composite_iterator_) && (bound_ == traits.bound_));
	}

	inline bool CompositeIteratorTraits::operator != (const CompositeIteratorTraits& traits) const
	{
		return !this->operator == (traits);
	}

	inline void CompositeIteratorTraits::invalidate()
	{
		bound_ = 0;
		composite_iterator_.invalidate();
	}

	inline void CompositeIteratorTraits::toBegin()
	{
		BALL_PRECONDITION_EXCEPTION((bound_ != 0), "cannot move unbound iterator to begin")
		composite_iterator_ = bound_->beginComposite();
		while (+composite_iterator_ && (predicate_->operator () (*composite_iterator_) == false))
		{
			++composite_iterator_;
		}
	}

	inline bool CompositeIteratorTraits::isBegin() const
	{
		if (isSingular())
		{
			return false;
		}

		try
		{
			Composite::CompositeIterator sub_iterator(bound_->beginComposite());
			while (+sub_iterator && (predicate_->operator () (*sub_iterator) == false))
			{
				++sub_iterator;
			}
			return (composite_iterator_ == sub_iterator);
		}
		catch (Exception::Precondition&)
		{
		}

		return false;
	}

	inline void CompositeIteratorTraits::toEnd()
	{
		composite_iterator_.toEnd();
	}

	inline Composite& CompositeIteratorTraits::getData()
	{
		return const_cast<Composite&>(*composite_iterator_);
	}

	inline const Composite& CompositeIteratorTraits::getData() const
	{
		return *composite_iterator_;
	}

	inline void CompositeIteratorTraits::forward()
	{
		++composite_iterator_;
		while (+composite_iterator_ && (predicate_->operator () (*composite_iterator_) == false))
		{
			++composite_iterator_;
		}
	}

	inline void CompositeIteratorTraits::toRBegin()
	{
		BALL_PRECONDITION_EXCEPTION(!isSingular(), "cannot move singular iterator to reverse begin")
		composite_iterator_ = --bound_->endComposite();
		while (+composite_iterator_ && (predicate_->operator () (*composite_iterator_) == false))
		{
			--composite_iterator_;
		}
	}

	inline bool CompositeIteratorTraits::isRBegin() const
	{
		if (isSingular())
		{
			return false;
		}
		Composite::CompositeIterator sub_iterator = --bound_->endComposite();

		while (+sub_iterator && (predicate_->operator () (*sub_iterator) == false))
		{
			--sub_iterator;
		}
		return (composite_iterator_ == sub_iterator);
	}

	inline void CompositeIteratorTraits::toREnd()
	{
		composite_iterator_.toREnd();
	}

	inline bool CompositeIteratorTraits::isREnd() const
	{
		if (isSingular())
		{
			return false;
		}
		return composite_iterator_.isREnd();
	}

	inline void CompositeIteratorTraits::backward()
	{
		--composite_iterator_;
		while (+composite_iterator_ && (predicate_->operator () (*composite_iterator_) == false))
		{
			--composite_iterator_;
		}
	}
} // namespace BALL

#endif // BALL_KERNEL_ITERATOR_H
