// $Id: MOLFile.sip,v 1.1 2001/12/23 22:03:59 oliver Exp $

class MOLFile
	: GenericMolFile
{
%HeaderCode
	#include <BALL/FORMAT/MOLFile.h>
	using namespace BALL;
%End
  public:
  MOLFile();
	// default open mode is "IN"
  MOLFile(String&, File::OpenMode /openmode/ = File::IN)
		throw(FileNotFound);
	~MOLFile()
		throw();

  virtual void write(const System& /system/);
  virtual bool read(System& /system/)
		throw(ParseError);
	virtual void write(const Molecule& /molecule/);
	virtual Molecule* read()
		throw(ParseError);
};
