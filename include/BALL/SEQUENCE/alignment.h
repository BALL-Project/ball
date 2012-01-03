// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#ifndef BALL_SEQUENCE_ALIGNMENT_H
#define BALL_SEQUENCE_ALIGNMENT_H

///////////////////////////////////////////////
#ifndef BALL_SEQUENCE_SEQUENCECHARACTER_H
	#include <BALL/SEQUENCE/sequenceCharacter.h>
#endif
/*
#ifndef BALL_SEQUENCE_CLUSTALFILE_H
	#include<BALL/SEQUENCE/clustalFile.h>
#endif
*/
#ifndef BALL_FORMAT_PDBFILE_H
	#include<BALL/FORMAT/PDBFile.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
	#include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
	#include<BALL/KERNEL/system.h>
#endif

#ifndef BALL_SYSTEM_FILE
	#include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_SEQUENCE_SEQUENCE_H
	#include<BALL/SEQUENCE/sequence.h>
#endif

//////////////////////////////////////////////////////

#ifndef EIGEN_DENSE                                                               
	#include <Eigen/Dense>
#endif

///////////////////////////////////////////////



namespace BALL
{

	class AlignAlgorithm;


	typedef Eigen::Array<SequenceCharacter, Eigen::Dynamic, Eigen::Dynamic> AlignmentMatrix;


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
		Alignment(const AlignmentMatrix & alignmentmatrix, const double score, const bool is_aligned=false);

		/**
		 *Copy Constructor
		 *@param alignment the alignment which is to be copied
		 */
		Alignment(Alignment & alignment);

		/**
		 *Destructor
		 */
		//~Alignment();
		//////////////////////////////////////////////////////////////////////////// Getter and Setter //////////////////////////////////////////////

		/**
		 *returns the score of the alignment
		 */
		double getScore() const;

		/**
		 * return the Matrix containing the  alignment
		 */
		AlignmentMatrix& getAlignmentMatrix();

		/**
		 *tells whether the sequences are already aligned
		 */
		bool isAligned() const;
		
	
		/**
		* return the SequnceCharacter stored at given position in Alignment
		* @param row the row out of which character is to be retrieved
		* @param column the column out of which the character is to be retrieved
		* @return  character stored in alignment(row, column)
		*/
		SequenceCharacter getSeqChar(unsigned int row, unsigned int column) const;

		/**
		*@return number of columns in the alignment
		*/
		unsigned int cols() const;
		
		/**
		*@return the number of rows in the alignment
		*/
		unsigned int rows() const;

		/**
		* sets the score of the alignment
		*@param score the scroe to be setted
		*/
		void setScore(double score);
			
	
		/////////////////////////////////////////////////////////////////////// Edit the Alignment ////////////////////////////////////////////

		/**
		 *inserts a GAP in a given Sequence at a given Positon	
		 *@param seq the Sequence where the inserteion should take place
		 *@param pos the position in the Sequence where the insertion should take place

		 */
		bool insertGap(Sequence& seq, int pos);

		/**
		*inserts a Gap in the alignment at a given row and a given column
		*@param row the row where the Gap is to be added
		*@param column the cvolumn where the Gap is to be added
		*/
		bool insertGap(int row, int column);

		/**
		*deletes a GAP in a given Sequence at a given position  
		*does not delete a Gap at the very last position of the sequence if you want to delete a whole column of 		gaps use geleteGapColumn
		*@param seq the Sequence where the insertion should take place
		*@param pos the int in the Sequence where the insertion should take place
		*/
		bool deleteGap(Sequence& seq, int pos);

		/**
		*deletes a Gap at a given row and column
		*does not delete a Gap at the very last position of the sequence if you want to delete a whole column of 		gaps use geleteGapColumn instead
		*@param row the row where the Gap is to be deleted
		*@param column the column where the Gap is to be deleted
		*/
		bool deleteGap(int row, int column);

		/**
		 *adds a sequence at the end of the alignment
		 *@param seq the sequence to be added
		 */
		void addSequence(Sequence& seq);

		/**
		 *deletes a given Sequence
		 *@param seq the sequence to be deleted
		 */
		bool deleteSequence(Sequence& seq);

		/**
		*deletes the Sequence at the given row
		*@param row the row where the sequence should be deleted
		*/
		bool deleteSequence(int row);
		
		/**
		*@param row the row, where the sequence is to be retrieved of
		*@return the Sequence at the given row
		*/
		Sequence& getSequence(int row);


		/**
		 *inserts a given SequenceCharacter into a given sequence at a given position 
		 *@param seq the Sequence where the character is to be inserted
		 *@param c the SequenceCharacter to be added
		 *@param pos the int where the SequenceCharacter is to be added
		 */
		bool insertSeqChar(Sequence& seq, SequenceCharacter& c, int pos=0);
		
		/**
		*inserts a given SequenceCharacter into the alignment at a given row and column
		*@param row the row where the Character is to be added
		*@param column the column where the Character is to be added
		*@param c the SequenceCharacter to be added
		*/
		bool insertSeqChar(int row, int column, SequenceCharacter& c);

	
		/**
		*deletes a whole column that consists only of Gaps
		*@param pos the position where the column is to be deleted
		*@return true if column could be deleted fals else
		*/
		bool deleteGapColumn(unsigned int pos);

		/**
		*resets the Alignment
		*/
		void reset();
		



		/////////////////////////////////////////////////////////////////////// Reading and Writing ///////////////////////////////////////////////////////////////
		
		/**
		 *reads a Clustalfile into an alignment
		 *@param file the file to be read
		 */
//		void read(ClustalFile & file);
		
		
	
		/**
		*reads a PDBFile into the alignment
		*@param file the file to be read
		*/
	//	void read(PDBFile& file);

		/**
		*reads a System into the Alignment
		*/
		void read(System& system);

		/**
		* reads a Protein into the Alignment
		*/
		void read(Protein& protein);

		/**
		*aligns the Sequences contained in the Alignment with a given Algorithm
		*@param algorithm the alignALgorithm witch which the alignment is to to be computed
		*@return true if alignment has been computed correctly, false otherwise
		*/
		bool align(AlignAlgorithm& algorithm);

		
		/**
		*shifts the alignment into the stream, or prints it
		*/
		virtual void dump(std::ostream& s= std::cout) const;

	
		/**
		 *reads a Clustalfile into an alignment
		 *@param file the file to be read
		 */
//		Alignment& operator<<(ClustalFile & file);
		
		
		/**
		*reads a FastaFile into the alignment
		*@param file the file to be read
		*/
//		Alignment& operator<<(FastaFile & file);

		/**
		*reads a PDBFile into the alignment
		*@param file the file to be read
		*/
//		Alignment& operator<<(PDBFile& file);


		
		/**
		 *writes an alignment into a FastaFile
		 *@param file the file to be read
		 */
//		Alignment& operator>>(ClustalFile & file);
		
		
		/**
		*writes an Alignment into a FastaFile 
		*@param file the file to be read
		*/
//		Alignment& operator>>(FastaFile & file);

		/**
		*writes an alignment into a PDBFile
		*@param file the file to be read
		*/
//		Alignment& operator>>(PDBFile& file);

		/**
		 *writes the alignment into a given Clustalfile
		 *@param file the file to be written to
		 */
//		void write(ClustalFile & file);
		

		/**
		*writes the alignment into a given FastaFile
		*@param file the file to be written to
		*/
//		void write(FastaFile & file);

		/**
		*writes the alignment into a given PDBFile
		*@param file the file to be read
		*/
//		void write(PDBFile& file);		
		

	private:
			AlignmentMatrix alignment_;
			double score_;
			bool is_aligned_;
};

}//namespace BALL
#endif //BALL_SEQUENCE_ALIGNMENT_H
