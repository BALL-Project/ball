// $Id: DCDFile.sip,v 1.1 2001/01/16 17:27:48 anker Exp $

class DCDFile
	:	TrajectoryFile
{
%HeaderCode
	#include <BALL/FORMAT/DCDFile.h>
	using namespace BALL;
%End
	public:
	DCDFile();
	DCDFile(const DCDFile& /file/);
	DCDFile(const String& /name/, File::OpenMode /open_mode/);
	// const DCDFile& operator = (const DCDFile& /file/);
	virtual void clear();
	// bool operator == (const DCDFile& /file/);
	virtual bool readHeader();
	virtual bool updateHeader(const SnapShotManager& /manager/);
	virtual bool writeHeader();
	virtual bool read(SnapShotManager& /manager/);
	virtual bool write(const SnapShotManager& /manager/);
	virtual bool append(const SnapShot& /snapshot/);
};
