// $Id: SDFile.sip,v 1.2 2002/01/12 01:59:49 oliver Exp $

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
	void disableAtoms() throw();
	void enableAtoms() throw();
};
