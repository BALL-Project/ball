// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBFile.sip,v 1.10 2005/02/11 15:27:13 oliver Exp $
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
  virtual void read(Protein& /protein/);
  virtual void read(Molecule& /molecule/);
  virtual void read(System& /system/);
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
