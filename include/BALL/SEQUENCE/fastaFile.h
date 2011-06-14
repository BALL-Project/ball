#ifndef BALL_SEQUENCE_FASTAFILE_H
#define BALL_SEQUENCE_FASTAFILE_H

/////////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_LINEBASEDFILE_H
	#include <BALL/FORMAT/lineBasedFile.h>
#endif

#ifndef
	#include<BALL/SEQUENCE/sequence.h>
#endif

/////////////////////////////////////////////////////////



//////////////////////////////////////////////////

namespace BALL{

	class BALL_EXPORT FASTAFile : public BALL::LineBasedFile
	{
		public:
			//////////////////////////////////////////////////// Constructor und Deconstructor //////////////////////////////////////////////////////////////
			/**
			 *Default Constructor
			 */
			FASTAFile();

			/**			
			 *Detailed Constructor
			 *@param trim_whitespaces sets whether leading and trailing whitespaces shall be removed while reading the file
			 */
			FASTAFile(const BALL::String& filename,BALL::File::OpenMode open_mode=std::ios::in, bool trim_whitespaces=false);

			/**
			 *Copy Constructor
			 */ 
			FASTAFile(FASTAFile& file)

			/**
			 *Destructor
			 */
			~FASTAFile();

			///////////////////////////////////////////////////////////// Reading and Writing /////////////////////////////////////////

			/**
			*reads a FastaFile into a protein
			*/
			Protein& operator>>(FASTAFile file,Protein& protein);
		
			/**
			* reads a Fastafile into a Molecule
			*/
			Molecule& operator>>(FASTAFile file, Molecule& molecule);

			/**
			*reads a FastaFile into a System
			*/
			System& operator>> (FASTAFile file, System& system);

			 /**
                        *reads a FastaFile into a protein
                        */
			void read(Protein& protein);

			/**
                        * reads a Fastafile into a Molecule
                        */
			void read(Molecule& molecule);

			/**
                        *reads a FastaFile into a System
                        */
			void read(System& system);

			/**
			* writes a Protein into a Fastafile
			*/
			void write(Protein& protein);

			/**
			*writes a Molecule into a Fastafile
			*/
			void write(Molecule& molecule);
			
			/**
			*writes a System into a Fastafile
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
			Sequence& getSequence();

		private: Sequence sequence;

	};



}//namespace BALL
#endif // BALL_SEQUENCE_FASTAFILE_H
