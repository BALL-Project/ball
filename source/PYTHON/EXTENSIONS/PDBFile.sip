// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBFile.sip,v 1.11 2005/02/25 13:22:39 amoll Exp $
//

class PDBFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/PDBFile.h>
	using namespace BALL;
%End
  public:
  PDBFile();
	// default open mode: READ
  PDBFile(const String&, OpenMode /openmode/ = std::ios::in) throw(FileNotFound);
	~PDBFile() throw();
  virtual bool read(Protein& /protein/) throw (ParseError);
  virtual bool read(Molecule& /molecule/) throw (ParseError);
  virtual bool read(System& /system/) throw (ParseError);
  virtual bool write(const Protein& /protein/) throw(CannotWrite);
  virtual bool write(const Molecule& /protein/) throw(CannotWrite);
  virtual bool write(const System& /system/) throw(CannotWrite);
  void operator >> (Protein& /protein/);
  void operator >> (Molecule& /molecule/);
  void operator >> (System& /system/);
  void operator << (const Protein& /protein/);
  void operator << (const Molecule& /molecule/);
  void operator << (const System& /system/);
};
