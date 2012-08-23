// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include<BALL/FORMAT/ClustalFile.h>
#include <BALL/STRUCTURE/peptideBuilder.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#ifndef BALL_STRUCTURE_PEPTIDES_H
#include<BALL/STRUCTURE/peptides.h>
#endif
//defined in the lexer (ClustalParserLexer.l)

extern int  ClustalParserparse();
extern void ClustalParserLexer_reset();

namespace BALL
{


	ClustalFile::ClustalFile()
			: File(),
			blocks_()
	{ }

	ClustalFile::ClustalFile(const String& filename, File::OpenMode open_mode)
			: File(filename,open_mode),
			blocks_()
	{
		File::open(filename, open_mode);
	}


	ClustalFile::~ClustalFile()
	{   }


	///////////////////////////////////////////////////////////// Reading and Writing /////////////////////////////////////////


	bool ClustalFile::hasValidBlocks()
	{
		/*
		 * Check emptyness
		 */

		//check whether there are no blocks, then all blocks conatained are valid
		if (blocks_.empty())
		{
			return true;
		}

		//if the file contains blocks, but the first one is empty all following blocks must be empty, too
		if (blocks_.at(0).seqs.empty())
		{
			for (vector<Block>::iterator it = blocks_.begin(); it != blocks_.end(); it++)
			{
				if (!(it->seqs.empty()))
				{
					//there is a block that is not empty -> return false
					return false;
				}
			}

			//all following blocks are empty -> return true
			return true;
		}


		/*
		 * Now check for the rest
		 */

		// number of lines in the first block and therefore in every block
		unsigned int num_of_lines = blocks_.at(0).seqs.size();


		//Vector of Idents of the first block, must be equal to that of all other blocks
		std::vector<String> idents;

		// fill the idents vector
		for (std::vector<SequenceLine>::iterator it = blocks_.at(0).seqs.begin(); it != blocks_.at(0).seqs.end(); it++)
		{
			idents.push_back(it->ident);
		}

		//denotes the over all number of Amino Acids
		unsigned int num_of_aa = 0;

		//TODO invent better idea!

		//counter to compute the difference later
		unsigned int num_of_aa_old=0;

		//iterate over all blocks of the file
		for (vector<Block>::iterator it = blocks_.begin(); it != blocks_.end(); it++)
		{

			//check whether the block is empty
			if (it->seqs.empty())
			{
				return false;
			}

			//check if the current Block has equal amount of sequences than the others
			if (it->seqs.size() != num_of_lines)
			{
				return false;
			}

			//check if there are only 60 Amino Acids per line
			if ((it->getSequenceLine(0).length - num_of_aa) > 60 )
			{
				return false;
			}

			//increment number of aas if, the length matches the real amount auf aas in the line
			if ((it->getSequenceLine(0).length - it->getSequenceLine(0).sequence.length()) == num_of_aa)
			{
				num_of_aa = it->getSequenceLine(0).length;
			}
			else
			{
				return false;
			}

			//iterate over all sequences of the Block
			for (unsigned int i = 0; i < it->seqs.size(); i++)
			{
				SequenceLine line = it->getSequenceLine(i);

				//check whether idents match
				if (line.ident != idents.at(i))
				{
					return false;
				}

				//check whether given number at end of each line matches the length of the sequence
				if ( (line.length != num_of_aa))
				{
					return false;
				}

				if (line.sequence.length() != num_of_aa - num_of_aa_old)
				{
					return false;
				}
			}

			num_of_aa_old = num_of_aa;

		}


		return true;

	}

	bool ClustalFile::read()
	{
		if (!isValid())
		{
			Log.error() << "Trying to read from invalid ClustalFile '" << getName() << "'" << std::endl;
			return false;
		}

		try
		{
			ClustalParserLexer_reset();
			state.current_parser = this;

			//ClustalParserdebug = 1;
			ClustalParserparse();

		}
		catch (Exception::ParseError& e)
		{
			Log.error() << "ClustalFile: Cannot read " << getName() << std::endl;

		}

		if (!hasValidBlocks())
		{
			Log.error() << "Trying to read from invalid ClustalFile '" <<getName() << "'" << std::endl;
			return false;
		}

		return true;
	}




	bool ClustalFile::write()
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw(File::CannotWrite(__FILE__, __LINE__, name_));
		}

		if (!hasValidBlocks())
		{
			Log.error() << "Trying to write an invalid ClustalFile '" <<getName() << "'" << std::endl;
			return false;
		}

		getFileStream() << "CLUSTAL" <<endl;

		vector<Block>::iterator it;

		for (it = blocks_.begin(); it != blocks_.end(); it++)
		{
			*it >> getFileStream();
		}

		return true;

	}



	void ClustalFile::addBlock(const ClustalFile::Block& block)
	{
		blocks_.push_back(block);
	}




	std::vector<ClustalFile::Block> ClustalFile::getBlocks()
	{
		return blocks_;
	}


	void ClustalFile::clear()
	{

		blocks_.clear();
	}

	void ClustalFile::dump()
	{
		int i=0;

		for (vector<Block>::iterator it = blocks_.begin(); it != blocks_.end(); it++)
		{
			cout<<"Block "<<i<<" contains: "<<endl;
			it->dump();
			++i;
		}
	}


	ClustalFile& ClustalFile::operator = (const ClustalFile& file)
	{
		state = file.state;
		blocks_ = file.blocks_;
		return *this;
	}

	////////////////////////////////// Implementations for nested class Block ///////////////////////////////////


	ClustalFile::Block::Block()
			: seqs(),conserv_line()
	{ }

	ClustalFile::Block::Block(const Block& bl)
			: seqs(bl.seqs),conserv_line(bl.conserv_line)
	{ }


	void ClustalFile::Block::reset()
	{
		seqs.clear();
		conserv_line.clear();
	}

	void ClustalFile::Block::addSequenceLine(SequenceLine& line)
	{
		seqs.push_back(line);
	}

	ClustalFile::SequenceLine& ClustalFile::Block::getSequenceLine(unsigned int i)
	{
		return seqs.at(i);
	}

	std::ostream& ClustalFile::Block::operator >> (std::ostream& os)
	{


		os << endl;

		// shift all sequencelines into the stream
		for (vector<SequenceLine>::iterator it = seqs.begin(); it != seqs.end(); it++)
		{

			*it >> os;

		}

		//shift the conservation line too

		os << conserv_line << endl;

		return os;

	}

	void ClustalFile::Block::dump(std::ostream& s) const
	{
		s<<"The block contains "<< seqs.size() <<" SequenceLines."<< endl <<"These are: "<< endl;

		for (unsigned int i=0; i < seqs.size(); i++)
		{
			s<<"SequenceLine 1:"<<endl;
			seqs.at(i).dump(s);
		}

		if (conserv_line.isEmpty())
		{
			s<<"The Conserv_Line is empty."<<endl;
		}
		else
		{
			s<<"Conserv_Line: "<< conserv_line<<endl;
		}
	}

	ClustalFile::Block& ClustalFile::Block::operator= (const ClustalFile::Block& block)
	{
		seqs = block.seqs;
		conserv_line = block.conserv_line;
		return *this;
	}


	///////////////////////////////////////////// Nested class SequenceLine /////////////////////////////////////////////////////

	ClustalFile::SequenceLine::SequenceLine()
			: ident(),sequence(),length(0)
	{ }

	ClustalFile::SequenceLine::SequenceLine(const SequenceLine& line)
			: ident(line.ident),sequence(line.sequence),length(line.length)
	{ }

	ClustalFile::SequenceLine::SequenceLine(String& id, String& seq, int len)
			:ident(id),sequence(seq),length(len)
	{ }

	void ClustalFile::SequenceLine::reset()
	{
		ident.clear();
		sequence.clear();
		length=0;
	}

	std::ostream& ClustalFile::SequenceLine::operator >> (std::ostream& os)
	{
		os << ident << "\t" << sequence << "\t" << length << endl;

		return os;

	}

	void ClustalFile::SequenceLine::dump(std::ostream& s) const
	{
		s <<"The ident is: "<< ident <<endl << "The sequence is: " << sequence<<endl << "The length is: "<<length<<endl;
	}

	ClustalFile::SequenceLine& ClustalFile::SequenceLine::operator = (const SequenceLine& line)
	{
		ident= line.ident;
		sequence = line.sequence;
		length = line.length;
		return *this;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ClustalFile& ClustalFile::ClustalFile::operator >> (System& system)
	{
		if (!hasValidBlocks())
		{
			throw Exception::InvalidArgument(__FILE__,__LINE__, "ClustalFile Error: Invalid File");
		}

		//vector to store the names of the proteins
		vector<String> names;

		//vector to store the Sequences of the Proteins
		vector<String> sequences_per_line;

		String sequence;

		//iterate over first block to create new Proteins and add them to the System
		for (vector<SequenceLine>::iterator it = blocks_.at(0).seqs.begin(); it != blocks_.at(0).seqs.end() ; it++)
		{
			names.push_back(it->ident);
			//    sequences.push_back("");

			//    Protein* p = new Protein();
			//    p->setName(it->ident);
			//    system.append(*p);
		}

		//iterate over all blocks to retrieve all SequenceLines
		for (unsigned int b_i =0; b_i < blocks_.size(); b_i++)
		{
			//counter that denotes which protein is to be updated
			unsigned int p_counter = system.countProteins()-blocks_.at(0).seqs.size();

			//iterate over all SequenceLines of the Block
			for (unsigned int s_i = 0; s_i < blocks_.at(b_i).seqs.size(); s_i++)
			{
				//check the protein's Name and the current SequenceLines Ident are equal
				//  if(/*p_ptr*/ system.getProtein(p_counter)->getName() != blocks_.at(b_i).seqs.at(s_i).ident)
				//{
				//      throw Exception::InvalidArgument(__FILE__,__LINE__, "ClustalFile Error: Names of Proteins don't match");
				//}

				//iterate through the string to get the single characters
				for (unsigned int j=0; j < (blocks_.at(b_i).seqs.at(s_i).length - b_i* 60); j++)
				{

					//  String three_letter;

					//retrieve the next character
					char check = blocks_.at(b_i).seqs.at(s_i).sequence[j];

					//check whether the next character is a valid AS and if it is, change it to three letter code
					if (check != '-')
					{
						if (Peptides::IsOneLetterCode(check))
						{
							/*three_letter*/
							sequence += /*Peptides::ThreeLetterCode*/(check);

						}
						else
						{
							throw Exception::InvalidArgument(__FILE__,__LINE__, "There is no OneLetterCode in the ClustalFile");
						}

						//for every Amino Acid create a new Residue named with the name of the Amino Acid
						//    Residue* r= new Residue(three_letter);

						//set the Property to Amino Acid
						//  r->setProperty(Residue::Property::PROPERTY__AMINO_ACID);


						//append Residue to the Protein
						//  system.getProtein(p_counter)->append(*r);

					}//end if check != '-'

				}// end of iteration through string

				if (p_counter > 0)
				{
					sequences_per_line.push_back(sequence);
				}

				p_counter++;

				sequence ="";


			}//end of iteration over all sequenceLines


		}//end of iteration over all Blocks

		sequences_per_line.push_back(sequence);

		vector<String> sequences;

		//retrieve sequences from sequences_per_line
		int real_counter = 0;

		while (real_counter < names.size())
		{
			sequence="";

			for (int i = real_counter; i < sequences_per_line.size(); i+=names.size())
			{
				sequence += sequences_per_line.at(i);
			}

			sequences.push_back(sequence);
			real_counter++;

		}


		//************ Build Peptide with Peptidebuilder *********************************

		if (sequences.size() != names.size())
		{
			throw Exception::InvalidArgument(__FILE__,__LINE__, "Not the same number of names and sequences in the ClustalFile!");
		}

		//iterate over all proteins of the ClustalFile
		for (unsigned int i=0; i<sequences.size(); i++)
		{
			// we need to define the peptid's AA sequence as a vector of descriptors
			vector<BALL::Peptides::AminoAcidDescriptor> descriptor_seq;

			for (unsigned int j=0; j<sequences.at(i).length(); j++)
			{
				// each aminoacid is represented by a descriptor
				BALL::Peptides::AminoAcidDescriptor* aad = new BALL::Peptides::AminoAcidDescriptor;


				// define a glycin
				aad->setAminoAcidType(Peptides::ThreeLetterCode(sequences.at(i).c_str() [j]));
				//aad->setPhi(Angle(-47., false));
				//aad->setPsi(Angle(-77., false));

				// add it to the sequence
				descriptor_seq.push_back(*aad);
			}

			// create a builder using the sequence
			BALL::Peptides::PeptideBuilder* pb = new BALL::Peptides::PeptideBuilder(descriptor_seq);

			// "link" the fragment db for adding missing information
			FragmentDB fdb("");
			pb->setFragmentDB(&fdb);

			// now build the peptide
			Protein* prot = pb->construct();

			// give sensible names
			pb->setChainName("Chain A");
			pb->setProteinName(names.at(i));

			system.append(*prot);
		}


		return *this;

	}



	/**
	 *writes an Alignment into a ClustalFile
	 */
	ClustalFile& ClustalFile::operator << (Alignment& alignment)
	{
		if (!alignment.isAligned())
		{
			throw File::CannotWrite(__FILE__, __LINE__, name_);
		}

		AlignmentMatrix matrix = alignment.getAlignmentMatrix();

		//Check whether we have to create new blocks
		if (blocks_.empty())
		{
			blocks_.push_back(* (new Block()));
		}

		//iterate over all Sequences in alignment
		for (unsigned int i = 0; i< matrix.rows(); i++)
		{
			//denotes the number of characters in current line
			unsigned int count = 1;
			//iterate over all blocks to write the SequenceLines each time count mod 60 equals zero the next block is setted
			unsigned int bl_count = 0;

			//iterate over SequenceCharacters of currentline
			for (unsigned int j=0; j< matrix.cols(); j++)
			{
				//CHECK whether we need to go to the next block
				if (count % 60 == 0)
				{
					//CHECK whether we need to create a new Block
					if (bl_count == 0 || bl_count >= blocks_.size())
					{
						addBlock(*(new Block));
					}

					//go to next block
					bl_count++;
				}

				//Check whether we have to newly create a SequenceLine at Position i
				if (blocks_.at(bl_count).seqs.empty() || (blocks_.at(bl_count).seqs.size() <= i))
				{
					SequenceLine* line = new SequenceLine();
					line->ident = matrix(i,0).getOrigin()->getName();
					blocks_.at(bl_count).seqs.push_back(*line);
				}

				//add SequenceCharacter to the sequenceLine
				blocks_.at(bl_count).seqs[i].sequence += matrix(i,j).getChar();

				//store number of AAs in current Sequence and increase counter
				blocks_.at(bl_count).seqs.at(i).length = count++;

			}
		}

		return *this;
	}




	/**
	 *reads a ClustalFile into an Alignment
	 * Note: take care when using this feature, because Origin of all SequenceCharacters will not be setted correctly (will be 0) as well as residue pointer.
	 * better: shift the file into a system and then shift the system into the alignment
	 */
	/*  ClustalFile& ClustalFile::operator >>(Alignment& alignment)
	    {

	    if(!hasValidBlocks())
	    {
	    throw Exception::InvalidArgument(__FILE__,__LINE__, "ClustalFile Error: Invalid File");
	    }

	//first reset Score
	alignment.setScore(0);

	//reset aligned_
	alignment.setAligned(false);

	//Check whether there are already sequences in the alignment
	unsigned int old_width = alignment.getAlignmentMatrix().rows();

	//iterate over all blocks to retrieve all SequenceLines
	for(unsigned int b_it =0; b_it < blocks_.size(); b_it++)
	{
	//iterate over all SequenceLines of the block
	for(unsigned int count = 0; count < blocks_.at(b_it).seqs.size(); count++)
	{
	String sequence = blocks_.at(b_it).seqs.at(count).sequence;

	//if matrix is completley empty
	if(old_width == 0 && count == 0)
	{
	//resize matrix
	alignment.getAlignmentMatrix().conservativeResize(1, 1);

	//intialize all newly created uninitialized entrys with Gaps
	for( int i= 0; i< alignment.getAlignmentMatrix().rows(); i++)
	{
	alignment.getAlignmentMatrix()(i,0)= *(new SequenceCharacter('-', 0, SequenceCharacter::type::GAP, 0));
	}
	}

	//check whether resizing is necessary
	if(old_width + count >= alignment.getAlignmentMatrix().rows())
	{

	//resize matrix
	alignment.getAlignmentMatrix().conservativeResize(old_width + count, Eigen::NoChange);

	//initialize all characters left, with Gaps
	for(int i= sequence.length(); i> alignment.getAlignmentMatrix().cols(); i--)
	{
	alignment.getAlignmentMatrix()(old_width + count, i) = *(new SequenceCharacter('-', 0, SequenceCharacter::type::GAP, 0));
	}
	}


	//iterate over sequence
	for(unsigned int i =0; i<sequence.length();i++)
	{
	char c = sequence[i];
	SequenceCharacter seq_c;
	seq_c.setType(SequenceCharacter::CHAR);
	seq_c.setChar(c);

	//check whether we have to resize the matrix
	if (i >= alignment.getAlignmentMatrix().cols())
	{
	//      alignment.getAlignmentMatrix().conservativeResize(Eigen::NoChange, i);
	//initialize the newly created empty cols with Gaps, except the ith row
	for (unsigned int m =0; m<alignment.getAlignmentMatrix().rows(); m++)
	{
	if(m != old_width + count)
	{
	//alignment.getAlignmentMatrix()(m,i)= *(new SequenceCharacter('-', 0 , SequenceCharacter::type::GAP, 0));
	}
	}
	}

	//einfügen an passender Stelle
	//      alignment.getAlignmentMatrix()(old_width + count, i)= seq_c;
	}

	}
	}

	return *this;
	}
	*/


	struct ClustalFile::State ClustalFile::state;


}

