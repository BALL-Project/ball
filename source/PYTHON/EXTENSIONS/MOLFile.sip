// $Id: MOLFile.sip,v 1.4 2003/06/19 15:24:34 oliver Exp $

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
  MOLFile(String&, File::OpenMode /openmode/ = std::ios::in)
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
