// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBFile.sip,v 1.8 2003/10/31 23:15:48 oliver Exp $
//

class PDBFile
	:	GenericPDBFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/PDBFile.h>
	using namespace BALL;
%End
  public:
  PDBFile();
	// default open mode: READ
  PDBFile(const String&, File::OpenMode /openmode/ = std::ios::in)
		throw(FileNotFound);
	~PDBFile() 
		throw();
  virtual void read(Protein& /protein/);
  virtual void read(System& /system/);
  virtual bool write(const Protein& /protein/) throw(CannotWrite);
  virtual bool write(const System& /system/) throw(CannotWrite);
  void operator >> (Protein& /protein/);
  void operator >> (System& /system/);
  void operator << (const Protein& /protein/);
  void operator << (const System& /system/);
};
