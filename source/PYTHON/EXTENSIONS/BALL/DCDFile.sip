// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class DCDFile
	: TrajectoryFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/DCDFile.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
				public:

				// BALL_CREATE(DCDFile)
				DCDFile();
				DCDFile(const String& /name/, OpenMode /open_mode/ = std::ios::in) throw(FileNotFound);
				~DCDFile();
				//const DCDFile& operator = (const DCDFile& /file/);
				virtual void clear();
				bool operator == (const DCDFile& /file/) const;
				bool isSwappingBytes() const;
				bool hasVelocities() const;

				bool init();
				virtual bool readHeader();
				virtual bool writeHeader();
				virtual bool append(const SnapShot& /snapshot/);
				virtual bool read(SnapShot& /snapshot/);
				//virtual bool flushToDisk(const vector<SnapShot> buffer);
				void enableVelocityStorage();
				void disableVelocityStorage();
				void close();

				private:
				DCDFile(const DCDFile& /file/);
};
