// $Id: trajectoryFile.sip,v 1.2 2001/07/25 11:32:37 oliver Exp $
class TrajectoryFile
				:  File
{
%HeaderCode
	#include <BALL/FORMAT/trajectoryFile.h>
	using namespace BALL;
%End
				public:
				TrajectoryFile()
								throw();
				TrajectoryFile(const TrajectoryFile& /file/)
								throw();
				TrajectoryFile(const String& /filename/,
												File::OpenMode /open_mode/ = File::IN)
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
