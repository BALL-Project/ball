// $Id: iterator.h,v 1.5 2001/01/26 01:36:52 amoll Exp $

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
begin##Type \
  () \
{ \
  return Type##Iterator::begin(*this); \
} \
 \
Type##Iterator \
end##Type\
  () \
{ \
  return Type##Iterator::end(*this); \
} \
 \
Type##ReverseIterator \
rbegin##Type \
  () \
{ \
  return Type##ReverseIterator::begin(*this); \
} \
 \
Type##ReverseIterator \
rend##Type## \
  () \
{ \
  return Type##ReverseIterator::end(*this); \
} \
 \
##Type##ConstIterator \
begin##Type## \
  () const \
{ \
  return Type##ConstIterator::begin(*this); \
} \
 \
##Type##ConstIterator \
end##Type## \
  () const \
{ \
  return Type##ConstIterator::end(*this); \
} \
 \
##Type##ConstReverseIterator \
rbegin##Type## \
  () const \
{ \
  return Type##ConstReverseIterator::begin(*this); \
} \
 \
##Type##ConstReverseIterator \
rend##Type## \
  () const \
{ \
  return Type##ConstReverseIterator::end(*this); \
}

namespace BALL 
{

	/**	Composite Iterator Traits.	
			This class is used in the implementation of the kernel iterators.
			It is intended for internal use only.
			{\bf Definition:} \URL{BALL/kernel/iterator.h}
			
	*/
	class CompositeIteratorTraits_
	{
		public:

		CompositeIteratorTraits_()
			:	bound_(0),
				subcomposite_iterator_(),
				predicate_(0)
		{
		}
			
		CompositeIteratorTraits_(const Composite& composite)
			:	bound_((Composite *)&composite),
				subcomposite_iterator_(composite.beginSubcomposite()),
				predicate_(0)
		{
		}
			
		CompositeIteratorTraits_(const CompositeIteratorTraits_ &traits)
			:	bound_(traits.bound_),
				subcomposite_iterator_(traits.subcomposite_iterator_),
				predicate_(traits.predicate_)
		{
		}
			
		virtual ~CompositeIteratorTraits_()
		{
		}

		CompositeIteratorTraits_ &operator =(const CompositeIteratorTraits_ &traits)
		{
			bound_ = traits.bound_;
			subcomposite_iterator_ = traits.subcomposite_iterator_;
			predicate_ = traits.predicate_;
			return *this;
		}

		Composite *getContainer()
		{
			return bound_;
		}
	
		const Composite* getContainer() const
		{
			return bound_;
		}
	
		bool isSingular() const
		{
			return (bound_ == 0);
		}
	
		Composite::SubcompositeIterator& getPosition()
		{
			return subcomposite_iterator_;
		}
	
		const Composite::SubcompositeIterator& getPosition() const
		{
			return subcomposite_iterator_;
		}
	
		bool operator ==(const CompositeIteratorTraits_ &traits) const
		{
			return (subcomposite_iterator_ == traits.subcomposite_iterator_);
		}

		bool operator !=(const CompositeIteratorTraits_ &traits) const
		{
			return (subcomposite_iterator_ != traits.subcomposite_iterator_);
		}
	
		bool isValid() const
		{
			return (bound_ != 0 && subcomposite_iterator_.isValid() == true);
		}

		void invalidate()
		{
			bound_ = 0;
			subcomposite_iterator_.invalidate();
		}

		void toBegin
			()
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

		bool isBegin() const
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

		void toEnd()
		{
			subcomposite_iterator_.toEnd();
		}

		bool isEnd() const
		{
			return subcomposite_iterator_.isEnd();
		}
	
		Composite& getData()
		{
			return *subcomposite_iterator_;
		}
	
		const Composite &getData() const
		{
			return *subcomposite_iterator_;
		}
	
		void forward()
		{
			subcomposite_iterator_.findNext(*predicate_);
		}
	
		void toRBegin()
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

		bool isRBegin() const
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
			
		void toREnd()
		{
			subcomposite_iterator_.toREnd();
		}

		bool isREnd() const
		{
			return subcomposite_iterator_.isREnd();
		}
			
		void backward()
		{
			subcomposite_iterator_.findPrevious(*predicate_);
		}
			
		void setPredicate(const UnaryPredicate<Composite>& predicate)
		{
			predicate_ = &predicate;
		}
			
		const UnaryPredicate<Composite>* getPredicate() const
		{
			return predicate_;
		}

		protected:

		Composite*												bound_;
		Composite::SubcompositeIterator 	subcomposite_iterator_;
		const UnaryPredicate<Composite>*	predicate_;
	};

} // namespace BALL

#endif // BALL_KERNEL_ITERATOR_H
