# 1 "predicate.h"
namespace BALL 
{
 class Composite;
 typedef UnaryPredicate<Composite> KernelPredicateType;
 template <class T>
 class KernelPredicate
  : public UnaryPredicate<Composite>
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 template <class T>
 bool KernelPredicate<T>::operator () (const Composite& composite) const
 {
  return RTTI<T>::isKindOf(composite);
 }
}  
