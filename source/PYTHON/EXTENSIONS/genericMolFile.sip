// $Id: genericMolFile.sip,v 1.2 2001/12/20 01:20:41 oliver Exp $

class GenericMolFile
	: LineBasedFile
{
%HeaderCode
	#include <BALL/FORMAT/genericMolFile.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End

	public:

	GenericMolFile()
					throw();

	GenericMolFile(String& /filename/, File::OpenMode /open_mode/ = File::IN)
					throw(FileNotFound);

	GenericMolFile(const GenericMolFile& /file/);

	~GenericMolFile()
					throw();

	virtual void initRead();
	virtual void initWrite();
	virtual void write(const System& /system/);
	virtual void write(const Molecule& /molecule/);
	virtual bool read(System& /system/)
					throw(ParseError);
	virtual Molecule* read()
					throw(ParseError);
	GenericMolFile& operator >> (System& /system/)
					throw(ParseError);
	GenericMolFile& operator << (const System& /system/);
	GenericMolFile& operator >> (Molecule& /molecule/)
					throw(ParseError);
	GenericMolFile& operator << (const Molecule& /molecule/);
};
