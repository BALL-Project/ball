// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericMolFile.sip,v 1.9 2003/11/01 09:27:37 oliver Exp $
//

class GenericMolFile
	: LineBasedFile
{
%TypeHeaderCode
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

	virtual bool write(const System& /system/) throw(CannotWrite);
	virtual bool write(const Molecule& /molecule/) throw(CannotWrite);
	virtual bool read(System& /system/)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);
	GenericMolFile& operator >> (System& /system/) throw(ParseError);
	GenericMolFile& operator << (const System& /system/);
	GenericMolFile& operator >> (Molecule& /molecule/) throw(ParseError);
	GenericMolFile& operator << (const Molecule& /molecule/);
};
