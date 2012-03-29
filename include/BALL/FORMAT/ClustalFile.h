// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_CLUSTALFILE_H
#define BALL_FORMAT_CLUSTALFILE_H

///////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
	#include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_SEQUENCE_ALIGNMENT_H
	#include <BALL/SEQUENCE/alignment.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
	#include <BALL/COMMON/exception.h>
#endif



#ifndef BALL_KERNEL_PROTEIN_H
	#include<BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
	#include<BALL/KERNEL/system.h>
#endif

#define CLUSTALPARSER_LINE_LENGTH 2550


/////////////////////////////////////////////////////////

//#include <vector.h>

//#include <iostream>

//////////////////////////////////////////////////


namespace BALL
{


	class BALL_EXPORT ClustalFile 
		: public File
	{
		public:

			///////////////////////////////////////////////////Constructors and Destructors ///////////////////////////////////////////////////

			class SequenceLine {

				public:	/*
					 *Constructor
					 */
					SequenceLine();

					/*
					 *Copy Constructor
					 */
					SequenceLine(const SequenceLine& line);

					/**
					 *Detailed Constructor
					 */
					SequenceLine(String& id, String& seq, int len);

					/*
					 *resets the Block
					 */
					void reset();

					/**
					 *operator >>
					 * writes a SequenceLine into the stream buffer
					 */
					std::ostream& operator >> (std::ostream& os);

					/*prints the values contained in this SequenceLine*/
					virtual void dump(std::ostream& s = std::cout) const;

					/**
					 *assignment operator
					 */
					SequenceLine& operator = (const SequenceLine& line);


					//////////////////////////////////////////////

					String ident;
					String  sequence;
					unsigned int length;

			};



			/**
			 *Nested Class for Block
			 */
			class Block {

				public: 
					/*
					 *Constructor
					 */
					Block();

					/**
					 *Copy constructor
					 */
					Block(const Block& bl);

					/**
					 *resets the Block
					 */
					void reset();

					/**
					 *adds a SequenceLine on top of the Block
					 */
					void addSequenceLine(SequenceLine& line);

					/**
					 *returns the Sequence at Position i, starting to count at zero
					 */
					SequenceLine& getSequenceLine(unsigned int i);

					/**
					 * operator >> ostream
					 * writes a block into the stream
					 */
					std::ostream& operator >> (std::ostream& os);

					/**
					 * assignment operator
					 */
					Block& operator = (const Block& block);					

					/*prints the values contained in this SequenceLine*/
					virtual void dump(std::ostream& s = std::cout) const;

					////////////////////////////////////////////////////////////////////////

					/**
					 *contains all the sequence lines of the Block as sequences
					 */
					std::vector<SequenceLine> seqs;

					/**
					 *String that contains the last line of a Block that denotes the degree of conservation of the column
					 */
					String conserv_line;


			};



			/** State of the parser **/
			struct State
			{
				ClustalFile* current_parser;
			};

			///////////////////////////////////////////////////////// Clustal File ///////////////////////////////////////////

			/**
			 *Default Constructor
			 */
			ClustalFile();

			/**			
			 *Detailed Constructor
			 *@param trim_whitespaces sets whether leading and trailing whitespaces shall be removed while reading the file
			 */
			ClustalFile(const String& filename, File::OpenMode open_mode=std::ios::in);

			/**
			 *Copy Constructor
			 */ 
			ClustalFile(ClustalFile& file);

			/**
			 *Destructor
			 */
			~ClustalFile();

			///////////////////////////////////////////////////////////// Reading and Writing /////////////////////////////////////////


			/*prints the values contained in this SequenceLine*/
			void dump();



			/*
			 *checks whether all Blocks have the same idents, and the same number of lines
			 */
			bool hasValidBlocks();

			/**
			 *reads a ClustalFile
			 */
			bool read();

			/**
			 * writes a ClustalFile
			 */
			bool write();

			/**
			 *reads a ClustalFile into a System
			 */
			ClustalFile& operator >> (System& system);

			/**
			 * writes a Clustalfile into an alignment
			 */
	//		ClustalFile& operator >>(Alignment& alignment);


			/**
			 *writes an ALignment into a ClustalFile
			 */
			ClustalFile& operator << (Alignment& alignment);

			ClustalFile& operator = (const ClustalFile& file);

			//@}
			/** @name Accessors
			 */
			//@{

			/**
			 *adds a Block at the top of the vector blocks_
			 */
			void addBlock(const Block& block);

			std::vector<Block> getBlocks();

			////////////////////////////////////////////////// Misc ////////////////////////////////////////////////////

			/**
			 *clear method
			 */
			virtual void clear();

			static State state;

	private:
			std::vector<Block> blocks_;

			//methods to faciliate the writing process



};

}
#endif // BALL_FORMAT_CLUSTALFILE_H
