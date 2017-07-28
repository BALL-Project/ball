class TrajectoryFile
	: File
{
%TypeHeaderCode
	#include <BALL/FORMAT/trajectoryFile.h>
%End
	public:

	TrajectoryFile();
	TrajectoryFile(const String&, OpenMode open_mode = std::ios::in)
		throw(FileNotFound);
	~TrajectoryFile();

	virtual void clear();
	bool operator == (const TrajectoryFile&) const;

	Size getNumberOfSnapShots();
	Size getNumberOfAtoms() const;

	virtual bool readHeader();
	virtual bool writeHeader();
	virtual bool append(const SnapShot&);
	virtual bool read(SnapShot&);

	private:
	TrajectoryFile(const TrajectoryFile&);
};
