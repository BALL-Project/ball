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
				TrajectoryFile(const String&,
											 OpenMode open_mode = std::ios::in) throw(FileNotFound);
				~TrajectoryFile();
				//const TrajectoryFile& operator = (const TrajectoryFile&);
				virtual void clear();
				bool operator == (const TrajectoryFile&) const;

				Size getNumberOfSnapShots();
				Size getNumberOfAtoms() const;

				virtual bool readHeader();
				virtual bool writeHeader();
				virtual bool append(const SnapShot&);
				virtual bool read(SnapShot&);
				//virtual bool flushToDisk(const ::std::vector<SnapShot> buffer);
				private:
				TrajectoryFile(const TrajectoryFile&);
};
