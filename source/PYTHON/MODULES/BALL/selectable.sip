 class Selectable
 {
%HeaderCode
#include <BALL/CONCEPT/selectable.h>
using namespace BALL;
%End
  public:
  Selectable();
  Selectable(const Selectable&, bool = true);
  // virtual ~Selectable();
  void clear();
  void destroy();
  void set(const Selectable&, bool = true);
  // Selectable& operator = (const Selectable&);
  void get(Selectable&, bool = true) const;
  void swap(Selectable&);
  virtual void select();
  virtual void deselect();
  bool isSelected() const;
  // friend ::std::ostream& operator << (::std::ostream& s, const Selectable& selectable);
  // friend ::std::istream& operator >> (::std::istream& s, Selectable& selectable);
  // void write(PersistenceManager& pm) const;
  // bool read(PersistenceManager& pm);
  // virtual void dump(::std::ostream& s = std::cout, unsigned long depth = 0L) const;
 };
