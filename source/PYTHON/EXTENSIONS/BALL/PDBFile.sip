class PDBFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/PDBFile.h>
%End
	public:
	PDBFile();
	PDBFile(const String&, OpenMode openmode = std::ios::in) throw(FileNotFound);
	~PDBFile();
	void selectAltLocationIndicator(char ch);
	bool read(Protein&) throw (ParseError);
	bool read(Molecule&) throw (ParseError);
	bool read(System&) throw (ParseError);
	bool write(const Protein&) throw(File::CannotWrite);
	bool write(const Molecule&) throw(File::CannotWrite);
	bool write(const System&) throw(File::CannotWrite);
	void close();

	//Actually all of the following methods are non virtual
	//However we need to declare them as virtual to trick sip into
	//producing correct code
	PDBFile& operator >> (Protein&) throw (ParseError);
	virtual PDBFile& operator >> (Molecule&) throw (ParseError);
	virtual PDBFile& operator >> (System&) throw (ParseError);
	PDBFile& operator << (const Protein&) throw(File::CannotWrite);
	virtual PDBFile& operator << (const Molecule&) throw(File::CannotWrite);
	virtual PDBFile& operator << (const System&) throw(File::CannotWrite);

	private:
	PDBFile(const PDBFile&);
};
