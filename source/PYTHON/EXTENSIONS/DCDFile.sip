// $Id: DCDFile.sip,v 1.3 2003/04/22 21:16:56 oliver Exp $

class DCDFile
	: TrajectoryFile
{
%HeaderCode
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
				DCDFile(const String& /name/, File::OpenMode /open_mode/ = File::IN)
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
