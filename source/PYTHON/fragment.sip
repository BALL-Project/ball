 class Fragment
  : BaseFragment
 {
%HeaderCode
#include <BALL/KERNEL/fragment.h>
using namespace BALL;
%End
   public:
  Fragment();
  Fragment(const Fragment&, bool = true);
  Fragment(const String&);
//  virtual ~Fragment();
  virtual void clear();
  virtual void destroy();
//  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
//  void persistentRead(PersistenceManager& pm);
//  Fragment& operator = (const Fragment& fragment);
  virtual bool isValid() const;
//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
//  virtual void read(istream& s);
//  virtual void write(ostream& s) const;
 };  
