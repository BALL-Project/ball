// $Id: iterator.h,v 1.1 1999/08/26 07:53:19 oliver Exp $

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
  (void) \
{ \
  return Type##Iterator::begin(*this); \
} \
 \
Type##Iterator \
end##Type\
  (void) \
{ \
  return Type##Iterator::end(*this); \
} \
 \
Type##ReverseIterator \
rbegin##Type \
  (void) \
{ \
  return Type##ReverseIterator::begin(*this); \
} \
 \
Type##ReverseIterator \
rend##Type## \
  (void) \
{ \
  return Type##ReverseIterator::end(*this); \
} \
 \
##Type##ConstIterator \
begin##Type## \
  (void) const \
{ \
  return Type##ConstIterator::begin(*this); \
} \
 \
##Type##ConstIterator \
end##Type## \
  (void) const \
{ \
  return Type##ConstIterator::end(*this); \
} \
 \
##Type##ConstReverseIterator \
rbegin##Type## \
  (void) const \
{ \
  return Type##ConstReverseIterator::begin(*this); \
} \
 \
##Type##ConstReverseIterator \
rend##Type## \
  (void) const \
{ \
  return Type##ConstReverseIterator::end(*this); \
}

namespace BALL {

	class CompositeIteratorTraits_
	{
		public:

			CompositeIteratorTraits_(void)
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
				
			virtual ~CompositeIteratorTraits_(void)
			{
			}

			CompositeIteratorTraits_ &operator =(const CompositeIteratorTraits_ &traits)
			{
				bound_ = traits.bound_;
				subcomposite_iterator_ = traits.subcomposite_iterator_;
				predicate_ = traits.predicate_;
				return *this;
			}

			Composite *getContainer(void)
			{
				return bound_;
			}
		
			const Composite* getContainer(void) const
			{
				return bound_;
			}
		
			bool isSingular(void) const
			{
				return (bool)(bound_ == 0);
			}
		
			Composite::SubcompositeIterator& getPosition(void)
			{
				return subcomposite_iterator_;
			}
		
			const Composite::SubcompositeIterator& getPosition(void) const
			{
				return subcomposite_iterator_;
			}
		
			bool operator ==(const CompositeIteratorTraits_ &traits) const
			{
				return (bool)(subcomposite_iterator_ == traits.subcomposite_iterator_);
			}

			bool operator !=(const CompositeIteratorTraits_ &traits) const
			{
				return (bool)(subcomposite_iterator_ != traits.subcomposite_iterator_);
			}
		
			bool isValid(void) const
			{
				return (bool)(bound_ != 0 && subcomposite_iterator_.isValid() == true);
			}

			void invalidate(void)
			{
				bound_ = 0;
				subcomposite_iterator_.invalidate();
			}

			void toBegin
				(void)
			{
				if (isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				subcomposite_iterator_ = bound_->beginSubcomposite();
				if (predicate_->operator()(*subcomposite_iterator_) == false)
				{
					subcomposite_iterator_.findNext(*predicate_);
				}
			}

			bool isBegin(void) const
			{
				if (isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);


				Composite::SubcompositeIterator sub_iterator = bound_->beginSubcomposite();

				if (predicate_->operator()(*subcomposite_iterator_) == false)
				{
					sub_iterator.findNext(*predicate_);
				}
				return (bool)(subcomposite_iterator_ == sub_iterator);
			}

			void toEnd(void)
			{
				subcomposite_iterator_.toEnd();
			}

			bool isEnd(void) const
			{
				return subcomposite_iterator_.isEnd();
			}
		
			Composite& getData(void)
			{
				return *subcomposite_iterator_;
			}
		
			const Composite &getData(void) const
			{
				return *subcomposite_iterator_;
			}
		
			void forward(void)
			{
				subcomposite_iterator_.findNext(*predicate_);
			}
		
			void toRBegin(void)
			{
				if (isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				subcomposite_iterator_ = bound_->rbeginSubcomposite();
				if (predicate_->operator()(*subcomposite_iterator_) == false)
				{
					subcomposite_iterator_.findPrevious(*predicate_);
				}
			}

			bool isRBegin(void) const
			{
				if (isSingular())
					throw Exception::InvalidIterator(__FILE__, __LINE__);

				Composite::SubcompositeIterator sub_iterator = bound_->rbeginSubcomposite();

				if (predicate_->operator()(*subcomposite_iterator_) == false)
				{
					sub_iterator.findPrevious(*predicate_);
				}
				return (bool)(subcomposite_iterator_ == sub_iterator);
			}
				
			void toREnd(void)
			{
				subcomposite_iterator_.toREnd();
			}

			bool isREnd(void) const
			{
				return subcomposite_iterator_.isREnd();
			}
				
			void backward(void)
			{
				subcomposite_iterator_.findPrevious(*predicate_);
			}
				
			void setPredicate(const UnaryPredicate<Composite>& predicate)
			{
				predicate_ = &predicate;
			}
				
			const UnaryPredicate<Composite>* getPredicate(void) const
			{
				return predicate_;
			}

		protected:

			Composite*												bound_;
			Composite::SubcompositeIterator 	subcomposite_iterator_;
			const UnaryPredicate<Composite>* predicate_;

		private:
	};

} // namespace BALL

#endif // BALL_KERNEL_ITERATOR_H
