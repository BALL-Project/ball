# 1 "iterator.h"
# 74 "iterator.h"
namespace BALL {
 class CompositeIteratorTraits_
 {
  public:
   CompositeIteratorTraits_(void)
    : bound_(0),
     subcomposite_iterator_(),
     predicate_(0)
   {
   }
   CompositeIteratorTraits_(const Composite& composite)
    : bound_((Composite *)&composite),
     subcomposite_iterator_(composite.beginSubcomposite()),
     predicate_(0)
   {
   }
   CompositeIteratorTraits_(const CompositeIteratorTraits_ &traits)
    : bound_(traits.bound_),
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
     throw Exception::InvalidIterator("iterator.h", 164);
    subcomposite_iterator_ = bound_->beginSubcomposite();
    if (predicate_->operator()(*subcomposite_iterator_) == false)
    {
     subcomposite_iterator_.findNext(*predicate_);
    }
   }
   bool isBegin(void) const
   {
    if (isSingular())
     throw Exception::InvalidIterator("iterator.h", 176);
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
     throw Exception::InvalidIterator("iterator.h", 216);
    subcomposite_iterator_ = bound_->rbeginSubcomposite();
    if (predicate_->operator()(*subcomposite_iterator_) == false)
    {
     subcomposite_iterator_.findPrevious(*predicate_);
    }
   }
   bool isRBegin(void) const
   {
    if (isSingular())
     throw Exception::InvalidIterator("iterator.h", 228);
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
   Composite*            bound_;
   Composite::SubcompositeIterator  subcomposite_iterator_;
   const UnaryPredicate<Composite>* predicate_;
  private:
 };
}  
