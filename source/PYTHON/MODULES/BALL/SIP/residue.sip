// $Id: residue.sip,v 1.3 2001/07/25 11:32:36 oliver Exp $

class Residue
  : Fragment
{
%HeaderCode
	#include <BALL/KERNEL/residue.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(Residue)
  enum Property
  {
   	PROPERTY__NON_STANDARD = Fragment::Property::NUMBER_OF_PROPERTIES + 1,
	  PROPERTY__AMINO_ACID,
   	PROPERTY__WATER,
  	PROPERTY__HAS_SSBOND,
   	PROPERTY__C_TERMINAL,
   	PROPERTY__N_TERMINAL,
   	PROPERTY__CYCLIC,
   	NUMBER_OF_PROPERTIES
  };
  enum FullNameType
  {
   	NO_VARIANT_EXTENSIONS,
   	ADD_VARIANT_EXTENSIONS
  };
  Residue();
  Residue(const Residue& /residue/, bool /deep/ = true);
  Residue(const String& /name/, const String& /id/ = "" , char /insertion_code/ = ' ' );
  ~Residue() throw();
  virtual void clear() throw();
  virtual void destroy() throw();
  // void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  // void persistentRead(PersistenceManager& pm);
  void set(const Residue& /residue/, bool /deep/ = true);
  // Residue& operator = (const Residue& residue);
  void get(Residue& /residue/, bool /deep/ = true) const;
  void swap(Residue& /residue/);
  String getFullName(FullNameType /type/ = ADD_VARIANT_EXTENSIONS) const;
  bool hasTorsionPhi() const;
  Angle getTorsionPhi() const;
  bool hasTorsionPsi() const;
  Angle getTorsionPsi() const;  
  Protein* getProtein();
  const Protein* getProtein() const;
  Chain* getChain();
  const Chain* getChain() const;
  PDBAtom* getPDBAtom(Position /position/);
  const PDBAtom* getPDBAtom(Position /position/) const;
  void setID(const String& /id/);
  const String& getID() const;
  void setInsertionCode(char /insertion_code/);
  char getInsertionCode() const;
  Size countPDBAtoms() const;
  void prepend(PDBAtom& /atom/);
  void append(PDBAtom& /atom/);
  void insert(PDBAtom& /atom/);
  void insertBefore(PDBAtom& /atom/, Composite& /before/);
  void insertAfter(PDBAtom& /atom/, Composite& /after/);
  bool remove(PDBAtom& /atom/);
  void spliceBefore(Residue& /residue/);
  void spliceAfter(Residue& /residue/);
  void splice(Residue& /residue/);
  bool isAminoAcid() const;
  bool isTerminal() const;
  bool isNTerminal() const;
  bool isCTerminal() const;
  virtual bool isValid() const throw();
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;
  // BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)
	PyMethods:
	__str__
%MemberCode
  Residue* ptr;
  if ((ptr = (Residue*)sipGetCppPtr((sipThisType*)a0,sipClass_Residue)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Residue ") + ptr->getName() + ptr->getID()
        + " { " + String(ptr->countAtoms()) + " atoms }").c_str());
%End
};
