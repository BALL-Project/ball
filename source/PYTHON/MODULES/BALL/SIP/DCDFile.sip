// $Id: DCDFile.sip,v 1.2 2001/07/25 11:32:31 oliver Exp $

// workaround for nested classes: pull it out
class DCDHeader
{
%HeaderCode
	#include <BALL/FORMAT/DCDFile.h>
	using namespace BALL;
	using namespace BALL::Exception;
	typedef DCDFile::DCDHeader DCDHeader;
%End
				public:
				Size start_info_block;
				//char CORD[4];
				Size number_of_coordinate_sets;
				Size step_number_of_starting_time;
				Size steps_between_saves;
				Size BALL_flag;
				//Size unused_1[5];
				DoubleReal time_step_length;
				//Size unused_2[9];
				Size end_info_block;
				Size start_title_block;
				Size number_of_comments;
				//char title[160];
				Size end_title_block;
				Size start_atomnumber_block;
				Size number_of_atoms;
				Size end_atomnumber_block;
				DCDHeader() throw();
};

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
				const DCDHeader& getHeader() const
								throw();
				void setHeader(const DCDHeader& /header/)
								throw();
				void enableVelocityStorage()
								throw();
				void disableVelocityStorage()
								throw();
};
