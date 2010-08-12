// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class TrajectoryFile
				:  File
{
%TypeHeaderCode
	#include <BALL/FORMAT/trajectoryFile.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
				public:
				TrajectoryFile();
				TrajectoryFile(const String& /filename/,
											 OpenMode /open_mode/ = std::ios::in) throw(FileNotFound);
				~TrajectoryFile();
				//const TrajectoryFile& operator = (const TrajectoryFile& /file/);
				virtual void clear();
				bool operator == (const TrajectoryFile& /file/) const;

				Size getNumberOfSnapShots() const;
				Size getNumberOfAtoms() const;

				virtual bool readHeader();
				virtual bool writeHeader();
				virtual bool append(const SnapShot& /snapshot/);
				virtual bool read(SnapShot& /snapshot/);
				//virtual bool flushToDisk(const ::std::vector<SnapShot> buffer);
				private:
				TrajectoryFile(const TrajectoryFile& /file/);
};
