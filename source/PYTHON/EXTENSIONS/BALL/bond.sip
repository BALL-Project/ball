// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Bond
	: Composite,
    PropertyManager
{
%TypeHeaderCode
	#include <BALL/KERNEL/bond.h>
	#include <BALL/KERNEL/atom.h>
	using namespace BALL;
%End
  public:
  // friend class Atom;
  // BALL_CREATE(Bond)
  typedef short Order;
  typedef short Type;
  enum BondOrder
  {
   ORDER__UNKNOWN        = 0,
   ORDER__SINGLE         = 1,
   ORDER__DOUBLE         = 2,
   ORDER__TRIPLE         = 3,
   ORDER__QUADRUPLE      = 4,
   ORDER__AROMATIC       = 5,
   NUMBER_OF_BOND_ORDERS
  };
  enum BondType
  {
   TYPE__UNKNOWN           = 0,
   TYPE__COVALENT          = 1,
   TYPE__HYDROGEN          = 2,
   TYPE__DISULPHIDE_BRIDGE = 3,
   TYPE__SALT_BRIDGE       = 4,
   TYPE__PEPTIDE           = 5,
   NUMBER_OF_BOND_TYPES
  };
  enum Property
  {
   NUMBER_OF_PROPERTIES
  };
  Bond();
  Bond(const Bond&, bool deep = true);
  Bond(const String&, Atom&, Atom&, Order order = ORDER__UNKNOWN ,
     Type type = TYPE__UNKNOWN );
  static Bond* createBond(Bond&, Atom&, Atom&);
  ~Bond() throw();
  virtual void clear() throw();
  virtual void destroy() throw();
  // void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  // void persistentRead(PersistenceManager& pm);
  void finalize();
  // Bond& operator = (const Bond& bond);
  void swap(Bond&);
  void setFirstAtom(Atom*);
  const Atom* getFirstAtom() const;
  Atom* getPartner(const Atom&) const;
  void setSecondAtom(Atom*);
  const Atom* getSecondAtom() const;
  void setName(const String&);
  const String& getName() const;
  void setOrder(Order);
  Order getOrder() const;
	bool isAromatic() const;
  void setType(Type);
  Type getType() const;
  float getLength() const;
  // const Atom* getBoundAtom(const Atom&);
  const Atom* getBoundAtom(const Atom&) const;
  bool isBondOf(const Atom&) const;
  bool isBound() const;
  bool isInterBond() const;
  bool isInterBondOf(const AtomContainer&) const;
  bool isInterBondOf(const System&) const;
  bool isIntraBond() const;
  bool isIntraBondOf(const AtomContainer&) const;
  bool isIntraBondOf(const System&) const;
  virtual bool isValid() const throw();
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // convert the atom to a string representation


  SIP_PYOBJECT __str__();
%MethodCode
	const Atom* a1 = sipCpp->getFirstAtom();
	const Atom* a2 = sipCpp->getSecondAtom();
	String tmp("Bond {");
	if ((a1 != 0) && (a2 != 0))
	{
		tmp += a1->getFullName();
		tmp += " - ";
		tmp += a2->getFullName();
		tmp += ", ";
		tmp += String(sipCpp->getLength());
		tmp += " A";
		switch (sipCpp->getOrder())
		{
			case Bond::ORDER__SINGLE:
				tmp += ", single";
				break;
			case Bond::ORDER__DOUBLE:
				tmp += ", double";
				break;
			case Bond::ORDER__AROMATIC:
				tmp += ", aromatic";
				break;
			case Bond::ORDER__TRIPLE:
				tmp += ", triple";
				break;
			default:
				tmp += ", unknown";
		}
		tmp += " }";
	}
	return PyString_FromString(tmp.c_str());
%End
};
