// $Id: PDBFile.sip,v 1.4 2000/07/12 19:35:13 oliver Exp $

class PDBFile
{
%HeaderCode
	#include <BALL/FORMAT/PDBFile.h>
	using namespace BALL;
%End
  public:
  PDBFile();
	// default open mode: READ
  PDBFile(const String&, File::OpenMode /openmode/ = File::IN);
	~PDBFile();
  virtual void read(Protein& /protein/);
  virtual void read(System& /system/);
  virtual void write(const Protein& /protein/);
  virtual void write(const System& /system/);
};
