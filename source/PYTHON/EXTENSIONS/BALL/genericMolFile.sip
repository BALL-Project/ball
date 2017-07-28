class GenericMolFile
	: LineBasedFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/genericMolFile.h>
%End
	public:
	GenericMolFile() throw();
	GenericMolFile(const String&, OpenMode open_mode = std::ios::in)
		throw(FileNotFound);
	virtual ~GenericMolFile()	throw();

	virtual bool write(const System&) throw(File::CannotWrite);
	virtual bool write(const Molecule&) throw(File::CannotWrite);
	virtual bool read(System&) throw(ParseError);
	virtual Molecule* read() throw(ParseError);
	virtual GenericMolFile& operator >> (System&) throw(ParseError);
	virtual GenericMolFile& operator << (const System&) throw(File::CannotWrite);
	virtual GenericMolFile& operator >> (Molecule&) throw(ParseError);
	virtual GenericMolFile& operator << (const Molecule&) throw(File::CannotWrite);
};
