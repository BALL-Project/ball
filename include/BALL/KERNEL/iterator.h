// $Id: iterator.h,v 1.8 2001/07/03 20:47:28 anker Exp $

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

		/// BAUSTELLE
		Composite *getContainer()
			throw();
	
		/// BAUSTELLE
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

		/// BAUSTELLE
		bool isValid() const
			throw();

		/// BAUSTELLE
		bool isSingular() const
			throw();
	
		/// BAUSTELLE
		Composite::SubcompositeIterator& getPosition()
			throw();
	
		/// BAUSTELLE
		const Composite::SubcompositeIterator& getPosition() const
			throw();
	
		/// BAUSTELLE
		void invalidate()
			throw();

		/// BAUSTELLE
		void toBegin()
			throw(Exception::InvalidIterator);

		/// BAUSTELLE
		bool isBegin() const
			throw(Exception::InvalidIterator);

		/// BAUSTELLE
		void toEnd()
			throw();

		/// BAUSTELLE
		bool isEnd() const
			throw();
	
		/// BAUSTELLE
		Composite& getData()
			throw();
	
		/// BAUSTELLE
		const Composite &getData() const
			throw();
	
		/// BAUSTELLE
		void forward()
			throw();
	
		/// BAUSTELLE
		void toRBegin()
			throw(Exception::InvalidIterator);

		/// BAUSTELLE
		bool isRBegin() const
			throw(Exception::InvalidIterator);
			
		/// BAUSTELLE
		void toREnd()
			throw();

		/// BAUSTELLE
		bool isREnd() const
			throw();
			
		/// BAUSTELLE
		void backward()
			throw();
			
		/// BAUSTELLE
		void setPredicate(const UnaryPredicate<Composite>& predicate)
			throw();
			
		/// BAUSTELLE
		const UnaryPredicate<Composite>* getPredicate() const
			throw();


		protected:

		/*_ BAUSTELLE
		*/
		Composite*												bound_;

		/*_ BAUSTELLE
		*/
		Composite::SubcompositeIterator 	subcomposite_iterator_;

		/*_ BAUSTELLE
		*/
		const UnaryPredicate<Composite>*	predicate_;

	};

	CompositeIteratorTraits_::CompositeIteratorTraits_()
		throw()
		:	bound_(0),
			subcomposite_iterator_(),
			predicate_(0)
	{
	}
		
	CompositeIteratorTraits_::CompositeIteratorTraits_(const Composite& composite)
		throw()
		:	bound_((Composite *)&composite),
			subcomposite_iterator_(composite.beginSubcomposite()),
			predicate_(0)
	{
	}
		
	CompositeIteratorTraits_::CompositeIteratorTraits_(const CompositeIteratorTraits_ &traits)
		throw()
		:	bound_(traits.bound_),
			subcomposite_iterator_(traits.subcomposite_iterator_),
			predicate_(traits.predicate_)
	{
	}
		
	CompositeIteratorTraits_::~CompositeIteratorTraits_()
		throw()
	{
	}

	CompositeIteratorTraits_& CompositeIteratorTraits_::operator = (const CompositeIteratorTraits_& traits)
		throw()
	{
		bound_ = traits.bound_;
		subcomposite_iterator_ = traits.subcomposite_iterator_;
		predicate_ = traits.predicate_;
		return *this;
	}

	Composite* CompositeIteratorTraits_::getContainer()
		throw()
	{
		return bound_;
	}

	const Composite* CompositeIteratorTraits_::getContainer() const
		throw()
	{
		return bound_;
	}

	bool CompositeIteratorTraits_::isSingular() const
		throw()
	{
		return (bound_ == 0);
	}

	Composite::SubcompositeIterator& CompositeIteratorTraits_::getPosition()
		throw()
	{
		return subcomposite_iterator_;
	}

	const Composite::SubcompositeIterator& CompositeIteratorTraits_::getPosition() const
		throw()
	{
		return subcomposite_iterator_;
	}

	bool CompositeIteratorTraits_::operator == (const CompositeIteratorTraits_ &traits) const
		throw()
	{
		return (subcomposite_iterator_ == traits.subcomposite_iterator_);
	}

	bool CompositeIteratorTraits_::operator !=(const CompositeIteratorTraits_ &traits) const
		throw()
	{
		return (subcomposite_iterator_ != traits.subcomposite_iterator_);
	}

	bool CompositeIteratorTraits_::isValid() const
		throw()
	{
		return (bound_ != 0 && subcomposite_iterator_.isValid() == true);
	}

	void CompositeIteratorTraits_::invalidate()
		throw()
	{
		bound_ = 0;
		subcomposite_iterator_.invalidate();
	}

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

	void CompositeIteratorTraits_::toEnd()
		throw()
	{
		subcomposite_iterator_.toEnd();
	}

	bool CompositeIteratorTraits_::isEnd() const
		throw()
	{
		return subcomposite_iterator_.isEnd();
	}

	Composite& CompositeIteratorTraits_::getData()
		throw()
	{
		return *subcomposite_iterator_;
	}

	const Composite& CompositeIteratorTraits_::getData() const
		throw()
	{
		return *subcomposite_iterator_;
	}

	void CompositeIteratorTraits_::forward()
		throw()
	{
		subcomposite_iterator_.findNext(*predicate_);
	}

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
		
	void CompositeIteratorTraits_::toREnd()
		throw()
	{
		subcomposite_iterator_.toREnd();
	}

	bool CompositeIteratorTraits_::isREnd() const
		throw()
	{
		return subcomposite_iterator_.isREnd();
	}

	void CompositeIteratorTraits_::backward()
		throw()
	{
		subcomposite_iterator_.findPrevious(*predicate_);
	}

	void CompositeIteratorTraits_::setPredicate(const UnaryPredicate<Composite>& predicate)
		throw()
	{
		predicate_ = &predicate;
	}

	const UnaryPredicate<Composite>* CompositeIteratorTraits_::getPredicate() const
		throw()
	{
		return predicate_;
	}

} // namespace BALL

#endif // BALL_KERNEL_ITERATOR_H
