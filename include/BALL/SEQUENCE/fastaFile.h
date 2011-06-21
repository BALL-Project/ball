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
			 *@param trim_whitespaces sets whether leading and trailing whitespaces shall be removed while reading the file
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

			};

	/////////////////////////////////////////////////////////////////// operators that have to be outside the class ///////////////////////////////////////////////////

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





}//namespace BALL
#endif // BALL_SEQUENCE_FASTAFILE_H
