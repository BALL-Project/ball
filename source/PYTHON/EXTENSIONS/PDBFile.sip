// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBFile.sip,v 1.7 2003/10/29 22:54:35 oliver Exp $
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
  virtual void write(const Protein& /protein/);
  virtual void write(const System& /system/);
  void operator >> (Protein& /protein/);
  void operator >> (System& /system/);
  void operator << (const Protein& /protein/);
  void operator << (const System& /system/);
};
