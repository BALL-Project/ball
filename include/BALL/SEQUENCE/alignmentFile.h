#ifndef BALL_SEQUENCE_FASTAFILE_H
#define BALL_SEQUENCE_FASTAFILE_H

/////////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_LINEBASEDFILE_H
	#include <BALL/FORMAT/lineBasedFile.h>
#endif

/////////////////////////////////////////////////////////



//////////////////////////////////////////////////



	class BALL_EXPORT AlignmentFile : public BALL::LineBasedFile
	{
		public:

			/**
			*Default Constructor
			*/
			AlignmentFile();

			/**			
			*Detailed Constructor
			*@param trim_whitespaces sets whether leading and trailing whitespaces shall be removed while reading the file
			*/
			AlignmentFile(const BALL::String& filename,BALL::File::OpenMode open_mode=std::ios::in, bool trim_whitespaces=false);
			
			/**
			*Copy Constructor
			*/ 
			AlignmentFile(AlignmentFile& file)

			/**
			*Destructor
			*/
			~AlignmentFile();
			
			/**
			*clear method
			*/
			virtual void clear()
		
				};


#endif // BALL_SEQUENCE_FASTAFILE_H
