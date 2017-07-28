class SDFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/SDFile.h>
%End
	public:
	SDFile();
	SDFile(String&, OpenMode openmode = std::ios::in) throw(FileNotFound);
	~SDFile()	throw();

	virtual bool write(const System&) throw(File::CannotWrite);
	virtual bool write(const Molecule&) throw(File::CannotWrite);
	virtual bool read(System&)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);
	void disableAtoms() throw();
	void enableAtoms() throw();
};
