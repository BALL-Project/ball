#ifndef BALL_SEQUENCE_ALIGNMENT_H
#define BALL_SEQUENCE_ALIGNMENT_H

///////////////////////////////////////////////
#ifndef BALL_SEQUENCE_SEQUENCECHARACTER_H
	#include <Ball/SEQUENCE/sequenceCharacter.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
	#include <BALL/MATHS/MATRIX44.h>
#endif

#ifndef BALL_SYSTEM_FILE
	#include <BALL/SYSTEM/file.h>
#endif

//////////////////////////////////////////////////////

                                                                
#include <Eigen/Dense>

///////////////////////////////////////////////


typedef Eigen::Array<SequenceCharacter, Eigen::Dynamic, Eigen::Dynmaic> AlignmentMatrix;

namespace BALL
{

	class BALL_EXPORT Alignment
{
	public:

		///////////////////////////////////////////////////////////////Constructors and Deconstrucotrs////////////////////////////////////

		/**
		 *Default Constructor
		 */
		Alignment();

		/**
		 *Detailed Constructor
		 *@param alignment the alignment form which to construct the Alignment
		 *@param score the score which the alignment has reached Default=0
		 *@param isaligned tells whether the sequences have already been aligned
		 */
		Alignment(AlignmentMatrix & alignmentmatrix, double score, bool is_aligned=true);

		/**
		 *Copy Constructor
		 *@param alignment the alignment which is to be copied
		 */
		Alignment(Alignment & alignment);

		/**
		 *Destructor
		 */
		~Alignment();
		//////////////////////////////////////////////////////////////////////////// Getter and Setter //////////////////////////////////////////////

		/**
		 *returns the score of the alignment
		 */
		double getScore();

		/**
		 * return the Matrix with the  alignment
		 */
		AlignmentMatrix& getAlignmentMatrix();

		/**
		 *tells whether the sequences are already aligned
		 */
		bool isAligned();

		/////////////////////////////////////////////////////////////////////// Edit the Alignment ////////////////////////////////////////////

		/**
		 *inserts a GAP in a given Sequence at a given Positon	
		 *@param seq the Sequence where the inserteion should take place
		 *@param pos the position in the Sequence where the insertion should take place

		 */
		void insertGap(Sequence& seq, int pos);

		/**
		*inserts a Gap in the alignment at a given row and a given column
		*@param row the row where the Gap is to be added
		*@param column the cvolumn where the Gap is to be added
		*/
		void insertGap(int row, int column);

		/**
		 *deletes a GAP in a given Sequence at a given position   
		 *@param seq the Sequence where the insertion should take place
		 *@param pos the int in the Sequence where the insertion should take place
		 */
		void deleteGap(Sequence& seq, int pos);

		/**
		*deletes a Gap at a given row and column
		*@param row the row where the Gap is to be deleted
		*@param column the column where the Gap is to be deleted
		*/
		void deleteGap(int row, int column);

		/**
		 *adds a sequence at the end of the alignment
		 *@param seq the sequence to be added
		 */
		void addSequence(Sequence& seq);

		/**
		 *deletes a given Sequence
		 *@param seq the sequence to be deleted
		 */
		void deleteSequence(Sequence& seq);

		/**
		*deletes the Sequence at the given row
		*@param row the row where the sequence should be deleted
		*/
		void deleteSequence(int row);
		
		/**
		 *inserts a given SequenceCharacter into a given sequence at a given position 
		 *@param seq the Sequence where the character is to be inserted
		 *@param c the SequenceCharacter to be added
		 *@param pos the int where the SequenceCharacter is to be added
		 */
		void insertSeqChar(Sequence& seq, SequenceCharacter& c; int pos=0);
		
		/**
		*inserts a given SequenceCharacter into the alignment at a given row and column
		*@param row the row where the Character is to be added
		*@param column the column where the Character is to be added
		*@param c the SequenceCharacter to be added
		*/
		void insertSeqChar(int row, int column, SequenceCharacter& c);


		/////////////////////////////////////////////////////////////////////// Reading and Writing ///////////////////////////////////////////////////////////////
		
		/**
		 *reads a Clustalfile into an alignment
		 *@param file the file to be read
		 */
		void read(ClustalFile & file);
		
		
		/**
		*reads a FastaFile into the alignment
		*@param file the file to be read
		*/
		void read(FastaFile & file);

		/**
		*reads a PDBFile into the alignment
		*@param file the file to be read
		*/
		void read(PDBFile& file);

		
		/**
		 *reads a Clustalfile into an alignment
		 *@param file the file to be read
		 */
		Alignment& operator<<(ClustalFile & file);
		
		
		/**
		*reads a FastaFile into the alignment
		*@param file the file to be read
		*/
		Alignment& operator<<(FastaFile & file);

		/**
		*reads a PDBFile into the alignment
		*@param file the file to be read
		*/
		Alignment& operator<<(PDBFile& file);


		
		/**
		 *writes an alignment into a FastaFile
		 *@param file the file to be read
		 */
		Alignment& operator>>(ClustalFile & file);
		
		
		/**
		*writes an Alignment into a FastaFile 
		*@param file the file to be read
		*/
		Alignment& operator>>(FastaFile & file);

		/**
		*writes an alignment into a PDBFile
		*@param file the file to be read
		*/
		Alignment& operator>>(PDBFile& file);

		/**
		 *writes the alignment into a given Clustalfile
		 *@param file the file to be written to
		 */
		void write(ClustalFile & file)
		

		/**
		*writes the alignment into a given FastaFile
		*@param file the file to be written to
		*/
		void write(FastaFile & file);

		/**
		*writes the alignment into a given PDBFile
		*@param file the file to be read
		*/
		void write(PDBFile& file);		
		

	private:
			AlignmentMatrix alignment;
			double score;
			bool is_aligned;
};

}//namespace BALL
#endif //BALL_SEQUENCE_ALIGNMENT_H
