// $Id: trajectoryFile.sip,v 1.1 2001/01/16 17:28:13 anker Exp $

class TrajectoryFile
	:	File
{
%HeaderCode
	#include <BALL/MOLMEC/COMMON/snapShot.h>
	#include <BALL/FORMAT/trajectoryFile.h>
	using namespace BALL;
%End
	public:
	TrajectoryFile();
	TrajectoryFile(const TrajectoryFile& /file/);
	TrajectoryFile(const String& /filename/,
			File::OpenMode /open_mode/ = File::IN);
	// const TrajectoryFile& operator = (const TrajectoryFile& file);
	virtual void clear();
	// bool operator == (const TrajectoryFile& file) const;
	virtual bool readHeader();
	virtual bool updateHeader(const SnapShotManager& /manager/);
	virtual bool writeHeader();
	virtual bool read(SnapShotManager& /manager/);
	virtual bool write(const SnapShotManager& /manager/);
	virtual bool append(const SnapShot& /snapshot/);
};
