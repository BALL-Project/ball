// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericMolFile.sip,v 1.6 2003/08/26 09:18:16 oliver Exp $
//

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

	GenericMolFile(String& /filename/, File::OpenMode /open_mode/ = std::ios::in)
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
