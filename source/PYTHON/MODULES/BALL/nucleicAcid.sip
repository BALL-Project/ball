class NucleicAcid
 : Molecule
{
%HeaderCode
#include <BALL/KERNEL/molecule.h>
using namespace BALL;
%End
  public:
  NucleicAcid();
  NucleicAcid(const NucleicAcid& /nucleic_acid/, bool /deep/ = true);
  NucleicAcid(const String& /name/, const String& /id/ = "" );
  // virtual ~NucleicAcid();

  virtual void clear();
  virtual void destroy();

  // void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  // void persistentRead(PersistenceManager& pm);
  void set(const NucleicAcid& /nucleic_acid/, bool /deep/ = true);
	//  NucleicAcid& operator = (const NucleicAcid& nucleic_acid);
  void get(NucleicAcid& /nucleic_acid/, bool /deep/ = true) const;
  void swap(NucleicAcid& /nucleic_acid/);
  Nucleotide* getNucleotide(int /position/);
  const Nucleotide* getNucleotide(int /position/) const;
  Nucleotide* get3Prime();
  const Nucleotide* get3Prime() const;
  Nucleotide* get5Prime();
  const Nucleotide* get5Prime() const;
  void setID(const String& /id/);
  const String& getID() const;
  int countNucleotides() const;
  virtual bool isValid() const;
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;
};
