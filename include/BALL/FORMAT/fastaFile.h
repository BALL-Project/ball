#ifndef BALL_SEQUENCE_FASTAFILE_H
#define BALL_SEQUENCE_FASTAFILE_H

/////////////////////////////////////////
#ifndef BALL_KERNEL_SYSTEM_H
	#include<BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_LINEBASEDFILE_H
	#include <BALL/FORMAT/lineBasedFile.h>
#endif

#ifndef BALL_STRUCTURE_PEPTIDES_H
	#include<BALL/STRUCTURE/peptides.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
	#include<BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
	#include<BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
	#include<BALL/KERNEL/residue.h>
#endif

#ifndef BALL_SEQUENCE_SEQUENCE_H
	#include<BALL/SEQUENCE/sequence.h>
#endif

#ifndef BALL_SEQUENCE_ALIGNMENT_H
	#include<BALL/SEQUENCE/alignment.h>
#endif
/////////////////////////////////////////////////////////


#include<string>
//////////////////////////////////////////////////

namespace BALL{

	class FASTAFile : public LineBasedFile { 

			public: 
			/**
			 *Default Constructor
			 */
			FASTAFile();

			/**			
			 *Detailed Constructor
			 *@param trim_whitespaces sets whether leading and trailing whitespaces shall be removed while reading the file default is
			 */
			FASTAFile(const BALL::String& filename,BALL::File::OpenMode open_mode=std::ios::in, bool trim_whitespaces=false);

			/**
			 *Destructor
			 */
			~FASTAFile();
			

			///////////////////////////////////////// Operators ///////////////////////////////////////
			bool operator == (const FASTAFile& f);  
			
			bool operator != (const FASTAFile& f);
						
			
		//	const FASTAFile& operator = (const FASTAFile& f);

			
			
			
			///////////////////////////////////////////////////////////// Reading and Writing /////////////////////////////////////////

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
			* read a FastaFile into an Alignment
			*/
			void read(Alignment& align);
			
			/**
			* writes a Protein into a Fastafile
			*/
			void write(const Protein& protein);

			/**
			*writes a Molecule into a Fastafile
			*/
			void write(const Molecule& molecule);
			
			/**
			*writes a System into a Fastafile
			*/
			void write(const System& system);

			
			////////////////////////////////////////////////// Misc ////////////////////////////////////////////////////

			/**
			 *clear method
			 */
			virtual void clear();

		/** Read a protein from the file
		 *  @throw Exception::ParseError if a syntax error was encountered
	 	*/
		FASTAFile& operator >> (Protein& protein);

		/** Read a molecule from the file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		FASTAFile& operator >> (Molecule& molecule);

		/** Read system from the file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		FASTAFile& operator >> (System& system);

		/** Write a protein to the file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		FASTAFile& operator << (const Protein& protein);

		/** Write a system to the file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		FASTAFile& operator << (const System& system);

		/** Write molecule to the file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		FASTAFile& operator << (const Molecule& molecule);

			};

	/////////////////////////////////////////////////////////////////// operators that have to be outside the class ///////////////////////////////////////////////////

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/fastaFile.iC>
#	endif
		

			


}//namespace BALL
#endif // BALL_SEQUENCE_FASTAFILE_H
