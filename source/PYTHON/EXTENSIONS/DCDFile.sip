// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: DCDFile.sip,v 1.7 2003/11/12 18:20:27 oliver Exp $
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
				DCDFile()
								throw();
				DCDFile(const DCDFile& /file/)
								throw(FileNotFound);
				DCDFile(const String& /name/, OpenMode /open_mode/ = std::ios::in)
								throw();
				~DCDFile()
								throw();
				//const DCDFile& operator = (const DCDFile& /file/)
				//				throw();
				virtual void clear()
								throw();
				bool operator == (const DCDFile& /file/) const
								throw();
				bool isSwappingBytes() const
								throw();
				bool hasVelocities() const
								throw();

				bool init()
								throw();
				virtual bool readHeader()
								throw();
				virtual bool writeHeader()
								throw();
				virtual bool append(const SnapShot& /snapshot/)
								throw();
				virtual bool read(SnapShot& /snapshot/)
								throw();
				//virtual bool flushToDisk(const vector<SnapShot> buffer)
				//				throw();
				void enableVelocityStorage()
								throw();
				void disableVelocityStorage()
								throw();
};
