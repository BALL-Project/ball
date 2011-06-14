#ifndef BALL_SEQUENCE_CLUSTALFILE_H
#define BALL_SEQUENCE_CLUSTALFILE_H

///////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_LINEBASEDFILE_H
	#include <BALL/FORMAT/lineBasedFile.h>
#endif

#ifndef BALL_SEQUENCE_ALIGNMENT_H
	#include <BALL/SEQUENCE/alignment.h>
#endif

/////////////////////////////////////////////////////////



//////////////////////////////////////////////////


namespace BALL
{

	class BALL_EXPORT ClustalFile : public BALL::LineBasedFile
	{
		public:

			///////////////////////////////////////////////////Constructors and Destructors ///////////////////////////////////////////////////

			/**
			 *Default Constructor
			 */
			ClustalFile();

			/**			
			 *Detailed Constructor
			 *@param trim_whitespaces sets whether leading and trailing whitespaces shall be removed while reading the file
			 */
			ClustalFile(const String& filename, File::OpenMode open_mode=std::ios::in, bool trim_whitespaces=false);

			/**
			 *Copy Constructor
			 */ 
			ClustalFile(ClustalFile& file)

			/**
			 *Destructor
			 */
			~ClustalFile();

			/**
			 *clear method
			 */
			virtual void clear()

			///////////////////////////////////////////////////////////// Reading and Writing /////////////////////////////////////////

		
			/**
			 *reads a ClustalFile into a System
			 */
			System& operator>> (ClustalFile file, System& system;

			/**
			 *reads a ClustalFile into a System
			 */
			void read(System& system);
			
			//Soll es das wirklich können???
			/**
			 * writes a Protein into a ClustalFile
			 */
			void write(Protein& protein);

			/**
			 *writes a Molecule into a ClustalFile
			 */
			void write(Molecule& molecule);

			/**
			 *writes a System into a ClustalFile
			 */
			void write(System& system);

			////////////////////////////////////////////////// Misc ////////////////////////////////////////////////////

			/**
			 *clear method
			 */
			virtual void clear();

			/**
			 *@return the Sequence stored in the file
			 */
			Alignment& getAlignment()

		private: Alignment alignment;

	};

}
#endif // BALL_SEQUENCE_CLUSTALFILE_H
