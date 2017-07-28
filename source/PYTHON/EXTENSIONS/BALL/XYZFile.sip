class XYZFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/XYZFile.h>
%End
	public:
	XYZFile();
	XYZFile(const String&, OpenMode openmode = std::ios::in);
	~XYZFile();
	virtual bool write(const System&);
	virtual bool write(const Molecule&);
	virtual bool read(System&);
	virtual Molecule* read();
	const String& getComment();

	private:
	XYZFile(const XYZFile&);
};
