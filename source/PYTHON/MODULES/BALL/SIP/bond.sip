// $Id: bond.sip,v 1.5 2001/07/25 11:32:32 oliver Exp $

class Bond
	: Composite,
    PropertyManager
{
%HeaderCode
	#include <BALL/KERNEL/bond.h>
	#include <BALL/KERNEL/atom.h>
	using namespace BALL;
%End
  public:
  // friend class Atom;
  // BALL_CREATE(Bond)
  // class TooManyBonds
  //  : public Exception::GeneralException
  // {
  //  public:
  //  TooManyBonds(const char* file, int line);
  // };
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
  Bond(const Bond& /bond/, bool /deep/ = true);
  Bond(const String& /name/, Atom& /first/, Atom& /second/, Order /order/ = ORDER__UNKNOWN ,
     Type /type/ = TYPE__UNKNOWN );
  static Bond* createBond(Bond& /bond/, Atom& /first/, Atom& /second/);
  ~Bond() throw();
  virtual void clear() throw();
  virtual void destroy() throw();
  // void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  // void persistentRead(PersistenceManager& pm);
  void finalize();
  // Bond& operator = (const Bond& bond);
  void swap(Bond& /bond/);
  void setFirstAtom(Atom* /atom/);
  const Atom* getFirstAtom() const;
  Atom* getPartner(const Atom& /atom/) const;
  void setSecondAtom(Atom* /atom/);
  const Atom* getSecondAtom() const;
  void setName(const String& /name/);
  const String& getName() const;
  void setOrder(Order /bond_order/);
  Order getOrder() const;
  void setType(Type /bond_type/);
  Type getType() const;
  float getLength() const;
  const Atom* getBoundAtom(const Atom& /atom/);
  const Atom* getBoundAtom(const Atom& /atom/) const;
  bool isBondOf(const Atom& /atom/) const;
  bool isBound() const;
  bool isInterBond() const;
  bool isInterBondOf(const AtomContainer& /atom_container/) const;
  bool isInterBondOf(const System& /system/) const;
  bool isIntraBond() const;
  bool isIntraBondOf(const AtomContainer& /atom_container/) const;
  bool isIntraBondOf(const System& /system/) const;
  virtual bool isValid() const throw();
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // convert the atom to a string representation
	PyMethods:
	__str__
%MemberCode
  Bond* ptr;
  if ((ptr = (Bond*)sipGetCppPtr((sipThisType*)a0,sipClass_Bond)) == NULL)
    return NULL;

	const Atom* a1 = ptr->getFirstAtom();
	const Atom* a2 = ptr->getSecondAtom();
	String tmp("Bond {");
	if ((a1 != 0) && (a2 != 0))
	{
		tmp += a1->getFullName();
		tmp += " - ";
		tmp += a2->getFullName();
		tmp += ", ";
		tmp += String(ptr->getLength());
		tmp += " A";
		switch (ptr->getOrder())
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
