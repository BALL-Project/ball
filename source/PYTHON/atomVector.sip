class AtomVector
{
%HeaderCode
#include <BALL/MOLMEC/COMMON/atomVector.h>
using namespace BALL;
%End
  public:
  AtomVector();
  AtomVector(Composite&, bool);
  AtomVector(AtomVector&, bool);

  void clear();
  // AtomVector& operator = (const AtomVector& rhs);
  void set(const AtomVector&);
  // AtomVector& operator = (const Composite& rhs);
  void set(const Composite&, bool);
  int size() const;
  // using vector<Atom*>::operator [];
  void savePositions();
  void resetPositions();
  void moveTo(const Gradient&, float);
	void push_back(Atom*);
	void resize(int);
	//  using vector<Atom*>::begin;
	//  using vector<Atom*>::end;
};
