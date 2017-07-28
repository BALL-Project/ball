class MOLFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/MOLFile.h>
%End
	public:
	MOLFile();
	MOLFile(String&, OpenMode openmode = std::ios::in)
		throw(FileNotFound);
	~MOLFile() throw();

	virtual bool write(const System&) throw(File::CannotWrite);
	virtual bool write(const Molecule&) throw(File::CannotWrite);
	virtual bool read(System&)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);
};
