#ifndef BALL_FORMAT_FASTAFILE_H
#define BALL_FORMAT_FASTAFILE_H

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

	class FastaFile : public LineBasedFile { 

			public: 
			/**
			 *Default Constructor
			 */
			FastaFile();

			/**			
			 *Detailed Constructor
			 *@param trim_whitespaces sets whether leading and trailing whitespaces shall be removed while reading the file default is
			 */
			FastaFile(const BALL::String& filename,BALL::File::OpenMode open_mode=std::ios::in, bool trim_whitespaces=false);

			/**
			 *Destructor
			 */
			~FastaFile();
			

			///////////////////////////////////////// Operators ///////////////////////////////////////
			bool operator == (const FastaFile& f);  
			
			bool operator != (const FastaFile& f);
						
			
		//	const FastaFile& operator = (const FastaFile& f);

			
			
			
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
		FastaFile& operator >> (Protein& protein);

		/** Read a molecule from the file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		FastaFile& operator >> (Molecule& molecule);

		/** Read system from the file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		FastaFile& operator >> (System& system);

		/** Write a protein to the file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		FastaFile& operator << (const Protein& protein);

		/** Write a system to the file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		FastaFile& operator << (const System& system);

		/** Write molecule to the file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		FastaFile& operator << (const Molecule& molecule);

			};

	/////////////////////////////////////////////////////////////////// operators that have to be outside the class ///////////////////////////////////////////////////

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/fastaFile.iC>
#	endif
		

			


}//namespace BALL
#endif // BALL_SEQUENCE_FASTAFILE_H
