// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Nucleotide
  : Fragment
{
%TypeHeaderCode
#include <BALL/KERNEL/nucleotide.h>
using namespace BALL;
%End
  public:
  Nucleotide();
  Nucleotide(const Nucleotide&, bool deep = true);
  Nucleotide
   (const String&,
    const String& id = "" ,
    char insertion_code = ' ' );
  ~Nucleotide() throw();

  virtual void clear() throw();
  virtual void destroy() throw();

  // void persistentWrite(PersistenceManager&, const char* name = 0) const;
  // void persistentRead(PersistenceManager&);

  void set(const Nucleotide&, bool deep = true);
  // Nucleotide& operator = (const Nucleotide&);
  void get(Nucleotide&, bool deep = true) const;
  void swap(Nucleotide&);

  NucleicAcid* getNucleicAcid();
  // const NucleicAcid* getNucleicAcid() const;

  void setID(const String&);
  const String& getID() const;
  void setInsertionCode(char);
  void unsetInsertionCode();
  char getInsertionCode() const;
  void prepend(Atom& /Transfer/);
  void append(Atom& /Transfer/);
  void insert(Atom& /Transfer/);
  void insertBefore(Atom& /Transfer/, Composite&);
  void insertAfter(Atom& /Transfer/, Composite&);
  bool remove(Atom&);
  void spliceBefore(Nucleotide&);
  void spliceAfter(Nucleotide&);
  void splice(Nucleotide&);
  bool isTerminal() const;
  bool is3Prime() const;
  bool is5Prime() const;
  virtual bool isValid() const throw();
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;
};
