// $Id: PDBFile.sip,v 1.3 2001/08/01 01:28:38 oliver Exp $

class PDBFile
{
%HeaderCode
	#include <BALL/FORMAT/PDBFile.h>
	using namespace BALL;
%End
  public:
  PDBFile();
	// default open mode: READ
  PDBFile(const String&, File::OpenMode /openmode/ = File::IN)
		throw(FileNotFound);
	~PDBFile() 
		throw();
  virtual void read(Protein& /protein/);
  virtual void read(System& /system/);
  virtual void write(const Protein& /protein/);
  virtual void write(const System& /system/);
  virtual void operator >> (Protein& /protein/);
  virtual void operator >> (System& /system/);
  virtual void operator << (const Protein& /protein/);
  virtual void operator << (const System& /system/);
};
