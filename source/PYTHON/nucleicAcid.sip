# 1 "nucleicAcid.h"
namespace BALL 
{
 class NucleicAcid
  : public Molecule
 {
  public:
  BALL_CREATE(NucleicAcid)
  enum Property
  {
   NUMBER_OF_PROPERTIES = Molecule::NUMBER_OF_PROPERTIES
  };
  NucleicAcid();
  NucleicAcid(const NucleicAcid& nucleic_acid, bool deep = true);
  NucleicAcid(const String& name, const String& id = "" );
  virtual ~NucleicAcid();
  virtual void clear();
  virtual void destroy();
  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  void persistentRead(PersistenceManager& pm);
  void set(const NucleicAcid& nucleic_acid, bool deep = true);
  NucleicAcid& operator = (const NucleicAcid& nucleic_acid);
  void get(NucleicAcid& nucleic_acid, bool deep = true) const;
  void swap(NucleicAcid& nucleic_acid);
  Nucleotide* get3Prime();
  const Nucleotide* get3Prime() const;
  Nucleotide* get5Prime();
  const Nucleotide* get5Prime() const;
  void setID(const String& id);
  String& getID();
  const String& getID() const;
  Size countNucleotides() const;
# 132 "nucleicAcid.h"
  Molecule::prepend;
  Molecule::append;
  Molecule::insert;
  Molecule::insertBefore;
  Molecule::insertAfter;
  Molecule::remove;
  Molecule::splice;
  Molecule::spliceBefore;
  Molecule::spliceAfter;
  virtual bool isValid() const;
  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
  virtual void read(istream& s);
  virtual void write(ostream& s) const;
  BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Nucleotide)
  private:
  void clear_();
  String id_;
 };
}  
