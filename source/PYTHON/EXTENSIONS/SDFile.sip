// $Id: SDFile.sip,v 1.5 2003/06/19 15:24:34 oliver Exp $

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
  SDFile(String&, File::OpenMode /openmode/ = std::ios::in)
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
