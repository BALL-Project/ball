// $Id: nucleotide.sip,v 1.1 2000/07/18 17:02:35 oliver Exp $
class Nucleotide
  : Fragment
{
%HeaderCode
#include <BALL/KERNEL/fragment.h>
using namespace BALL;
%End
  public:
  Nucleotide();
  Nucleotide(const Nucleotide& /nucleotide/, bool /deep/ = true);
  Nucleotide
   (const String& /name/,
    const String& /id/ = "" ,
    char /insertion_code/ = ' ' );
  // virtual ~Nucleotide();

  virtual void clear();
  virtual void destroy();

  // void persistentWrite(PersistenceManager& /pm/, const char* /name/ = 0) const;
  // void persistentRead(PersistenceManager& /pm/);

  void set(const Nucleotide& /nucleotide/, bool /deep/ = true);
  // Nucleotide& operator = (const Nucleotide& /nucleotide/);
  void get(Nucleotide& /nucleotide/, bool /deep/ = true) const;
  void swap(Nucleotide& /nucleotide/);
  NucleicAcid* getNucleicAcid();
  const NucleicAcid* getNucleicAcid() const;
  void setID(const String& /id/);
  const String& getID() const;
  void setInsertionCode(char /insertion_code/);
  char getInsertionCode() const;
  void prepend(Atom& /atom/);
  void append(Atom& /atom/);
  void insert(Atom& /atom/);
  void insertBefore(Atom& /atom/, Composite& /before/);
  void insertAfter(Atom& /atom/, Composite& /after/);
  bool remove(Atom& /atom/);
  void spliceBefore(Nucleotide& /nucleotide/);
  void spliceAfter(Nucleotide& /nucleotide/);
  void splice(Nucleotide& /nucleotide/);
  bool isTerminal() const;
  bool is3Prime() const;
  bool is5Prime() const;
  virtual bool isValid() const;
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;
};
