// $Id: SDFile.sip,v 1.1 2001/12/28 02:25:52 oliver Exp $

class SDFile
	: GenericMolFile
{
%HeaderCode
	#include <BALL/FORMAT/SDFile.h>
	using namespace BALL;
%End
  public:
  SDFile();
	// default open mode is "IN"
  SDFile(String&, File::OpenMode /openmode/ = File::IN)
		throw(FileNotFound);
	~SDFile()
		throw();

  virtual void write(const System& /system/);
  virtual bool read(System& /system/)
		throw(ParseError);
	virtual void write(const Molecule& /molecule/);
	virtual Molecule* read()
		throw(ParseError);
};
