class HINFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/HINFile.h>
%End
	public:
	HINFile();
	HINFile(String&, OpenMode openmode = std::ios::in) throw(FileNotFound);
	~HINFile() throw();

	virtual bool write(const System&) throw(File::CannotWrite);
	virtual bool write(const Molecule&) throw(File::CannotWrite);
	virtual bool read(System&) throw(ParseError);
	virtual Molecule* read() throw(ParseError);

	bool hasPeriodicBoundary() const;
	SimpleBox3 getPeriodicBoundary() const;
	float getTemperature() const;
};
