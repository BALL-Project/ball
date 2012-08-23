// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class NucleicAcid
 : Molecule
{
%TypeHeaderCode
#include <BALL/KERNEL/nucleicAcid.h>
using namespace BALL;
%End
  public:
  NucleicAcid();
  NucleicAcid(const NucleicAcid&, bool deep = true);
  NucleicAcid(const String&, const String& id = "" );
  ~NucleicAcid() throw();

  virtual void clear() throw();
  virtual void destroy() throw();

  // void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  // void persistentRead(PersistenceManager& pm);
  void set(const NucleicAcid&, bool deep = true);
	//  NucleicAcid& operator = (const NucleicAcid& nucleic_acid);
  void get(NucleicAcid&, bool deep = true) const;
  void swap(NucleicAcid&);

  Nucleotide* getNucleotide(int);
  // const Nucleotide* getNucleotide(int) const;

  Nucleotide* get3Prime();
  // const Nucleotide* get3Prime() const;

  Nucleotide* get5Prime();
  // const Nucleotide* get5Prime() const;

  void setID(const String&);
  const String& getID() const;
  int countNucleotides() const;
  virtual bool isValid() const throw();
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;
	
	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("NucleicAcid ") + sipCpp->getName() 
							+ " { " + String(sipCpp->countNucleotides()) + " nucleotides,  " 
							+ String(sipCpp->countAtoms()) + " atoms }").c_str());
%End
	
	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(String("NucleicAcid ") + sipCpp->getName() 
							+ " { " + String(sipCpp->countNucleotides()) + " nucleotides,  " 
							+ String(sipCpp->countAtoms()) + " atoms }").c_str());
%End

};
