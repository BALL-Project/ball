// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: trajectoryFile.sip,v 1.7 2003/11/12 18:20:29 oliver Exp $
//

class TrajectoryFile
				:  File
{
%TypeHeaderCode
	#include <BALL/FORMAT/trajectoryFile.h>
	using namespace BALL;
%End
				public:
				TrajectoryFile()
								throw();
				TrajectoryFile(const TrajectoryFile& /file/)
								throw();
				TrajectoryFile(const String& /filename/,
											 OpenMode /open_mode/ = std::ios::in)
								throw();
				~TrajectoryFile()
								throw();
				//const TrajectoryFile& operator = (const TrajectoryFile& /file/)
				//				throw();
				virtual void clear()
								throw();
				bool operator == (const TrajectoryFile& /file/) const
								throw();

				Size getNumberOfSnapShots() const
								throw();
				Size getNumberOfAtoms() const
								throw();

				virtual bool readHeader()
								throw();
				virtual bool writeHeader()
								throw();
				virtual bool append(const SnapShot& /snapshot/)
								throw();
				virtual bool read(SnapShot& /snapshot/)
								throw();
				//virtual bool flushToDisk(const ::std::vector<SnapShot> buffer)
				//				throw();
};
