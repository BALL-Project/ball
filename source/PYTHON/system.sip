 class System
  : BaseFragment,
  	PropertyManager
 {
%HeaderCode
#include <BALL/KERNEL/system.h>
using namespace BALL;
%End
  public:
  System();
  System(const System&, bool = true);
  System(const String&);
//  virtual ~System;
  virtual void clear();
  virtual void destroy();
//  void persistentWrite(PersistenceManager& pm, const char* = 0) const;
//  void persistentRead(PersistenceManager& pm);
  void set(const System&, bool = true);
//  System& operator = (const System&);
  void get(System&, bool = true) const;
  void swap(System&);
  void setName(const String&);
  String& getName();
  const String& getName() const;
  Size countMolecules() const;
  Size countFragments() const;
  Size countAtoms() const;
  void prepend(Molecule&);
  void append(Molecule&);
  void insert(Molecule&);
  void insertBefore(Molecule&, Composite&);
  void insertAfter(Molecule&, Composite&);
  bool remove(Molecule&);
  void spliceBefore(System&);
  void spliceAfter(System&);
  void splice(System&);  
  void destroyBonds();
  virtual bool isValid() const;
//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
//  virtual void read(istream& s);
//  virtual void write(ostream& s) const;
//  template <typename T>
//  bool apply(UnaryProcessor<T>& processor)
//  {
// if (processor.start() == false)
//    return false;
//   Processor::Result  result;
//   SubcompositeIterator it = beginSubcomposite();
//	 for (; it != endSubcomposite(); ++it)
//   {
//    T*  object = dynamic_cast<T*>(&(*it));
  //  if (object != 0)
//    {
//     result = processor(*object);
//     if (result <= Processor::BREAK)
//      return (result == Processor::BREAK) ? true : false;
//    }
//   }
//   return processor.finish();
//  }
//  BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)
  //BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)
//  BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Fragment)
//  BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Molecule)
//  BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Protein)
//  BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
//  BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Chain)
//  BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
 };
