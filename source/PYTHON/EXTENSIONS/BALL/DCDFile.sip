class DCDFile
	: TrajectoryFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/DCDFile.h>
%End
	public:

	DCDFile();
	DCDFile(const String&, OpenMode open_mode = std::ios::in) throw(FileNotFound);
	~DCDFile();
	virtual void clear();
	bool operator == (const DCDFile&) const;
	bool isSwappingBytes() const;
	bool hasVelocities() const;

	bool init();
	virtual bool readHeader();
	virtual bool writeHeader();
	virtual bool append(const SnapShot&);
	virtual bool read(SnapShot&);
	void enableVelocityStorage();
	void disableVelocityStorage();
	void close();

	private:
	DCDFile(const DCDFile&);
};
