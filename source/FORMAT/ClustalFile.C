// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include<BALL/FORMAT/ClustalFile.h>

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
		: File(filename,open_mode)
			{ }

	/*ClustalFile::ClustalFile(ClustalFile& file)
	  : LineBasedFile::LineBasedFile() 
	  {
	  open	
	  blocks(file.getBlocks());
	  state.current_parser = file.state.current_parser;
	  }
	 */

	ClustalFile::~ClustalFile()
	{
		//	LineBasedFile::~LineBasedFile();
	}


	///////////////////////////////////////////////////////////// Reading and Writing /////////////////////////////////////////

	
	bool ClustalFile::hasValidBlocks()
	{
					/* 
					* Check emptyness
					*/

					//check whether there are no blocks, then all blocks conatained are valid
					if(blocks_.empty())
					{
									return true;
					}

					//if the file contains blocks, but the first one is empty all following blocks must be emoty, too
					if(blocks_.at(0).seqs.empty())
					{
									for(vector<Block>::iterator it = blocks_.begin(); it != blocks_.end(); it++)
									{
													if(!(it->seqs.empty()))
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
					for(std::vector<SequenceLine>::iterator it = blocks_.at(0).seqs.begin(); it != blocks_.at(0).seqs.end(); it++)
					{ 
									idents.push_back(it->ident);
					}

					//denotes the over all number of Amino Acids 
					unsigned int num_of_aa = 0;

					//TODO invent better idea!

					//counter to compute the difference later
					unsigned int num_of_aa_old=0;

					//iterate over all blocks of the file
					for(vector<Block>::iterator it = blocks_.begin(); it != blocks_.end(); it++)
					{

									//check whether the block is empty
									if(it->seqs.empty())
									{
													return false;
									}

									//check if the current Block has equal amount of sequences than the others
									if(it->seqs.size() != num_of_lines) 
									{
													return false;
									}

									//check if there are only 60 Amino Acids per line
									if ((it->getSequenceLine(0).length - num_of_aa) > 60 )
									{
													return false;
									}

									//increment number of aas if, the length matches the real amount auf aas in the line
									if((it->getSequenceLine(0).length - it->getSequenceLine(0).sequence.length()) == num_of_aa)
									{
													num_of_aa = it->getSequenceLine(0).length;
									}
									else
									{
													return false;
									}

									//iterate over all sequences of the Block
									for(unsigned int i = 0; i < it->seqs.size(); i++)
									{
													SequenceLine line = it->getSequenceLine(i);

													//check whether idents match
													if (line.ident != idents.at(i))  
													{
																	return false;
													}

													//check whether given number at end of each line matches the length of the sequence
													if( (line.length != num_of_aa))
													{
																	return false;
													}
													if(line.sequence.length() != num_of_aa - num_of_aa_old)
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

		try {
			ClustalParserLexer_reset();
			state.current_parser = this;

			//ClustalParserdebug = 1;
			ClustalParserparse();

		}
		catch (Exception::ParseError& e)
		{
			Log.error() << "ClustalFile: Cannot read " << getName() << std::endl;

		}
		if(!hasValidBlocks()) 
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

			if(!hasValidBlocks()) 
		{ 
		Log.error() << "Trying to write an invalid ClustalFile '" <<getName() << "'" << std::endl;
		return false;
		}


		vector<Block>::iterator it;
		for (it = blocks_.begin(); it != blocks_.end(); it++)
		{
			*it >> getFileStream();
		}

		return true;

	}



	void ClustalFile::addBlock(const ClustalFile::Block& block){
			blocks_.push_back(block);
	}




	std::vector<ClustalFile::Block> ClustalFile::getBlocks(){
		return blocks_;
	}


	void ClustalFile::clear(){

		blocks_.clear();
	}

void ClustalFile::dump()
{	
	int i=0;
	for(vector<Block>::iterator it = blocks_.begin(); it != blocks_.end(); it++)
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

	std::ostream& ClustalFile::Block::operator >> (std::ostream& os){


		os << endl;		

		// shift all sequencelines into the stream
		for(vector<SequenceLine>::iterator it = seqs.begin(); it != seqs.end(); it++) {

			*it >> os;			

		}

		//shift the conservation line too

		os << conserv_line << endl;

		return os;

	}

	void ClustalFile::Block::dump(std::ostream& s) const 
	{
		s<<"The block contains "<< seqs.size() <<" SequenceLines."<< endl <<"These are: "<< endl;

			for(unsigned int i=0; i < seqs.size(); i++) 
			{
				s<<"SequenceLine 1:"<<endl;
				seqs.at(i).dump(s);
			}

		if(conserv_line.isEmpty())
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
					//iterate over first block to create new Proteins and add them to the System
					for(vector<SequenceLine>::iterator it = blocks_.at(0).seqs.begin(); it != blocks_.at(0).seqs.end() ; it++)
					{
									Protein* p = new Protein();
									p->setID(it->ident);
									system.append(*p);
					}

					//iterate over all blocks to retrieve all SequenceLines
					for(vector<Block>::iterator b_it = getBlocks().begin(); b_it!= getBlocks().end(); b_it++)
					{
									//counter that denotes which protein is to be updated
									unsigned int p_counter = 0;

									//iterate over all SequenceLines of the Block
									for(vector<SequenceLine>::iterator s_it = b_it->seqs.begin(); s_it != b_it->seqs.end() ; s_it++)
									{
													//fetch the current protein an increment the counter
													Protein* p_ptr = system.getProtein(p_counter++);

													//check the protein's Name and the current SequenceLines Ident are equal
													if(p_ptr->getName() != s_it->ident) 
													{ 
																	throw Exception::InvalidArgument(__FILE__,__LINE__, "ClustalFile Error: IDs don't match");
													}


													//iterate through the string to get the single characters
													for (unsigned int j=0; j < s_it -> length; j++)						
													{	

																	String three_letter;

																	//retrieve the next character
																	char check = s_it->sequence[j];

																	//check whether the next character is a valid AS and if it is, change it to three letter code						
																	if(Peptides::IsOneLetterCode(check)) 
																	{
																					three_letter = Peptides::ThreeLetterCode(check);
																	}
																	else
																	{ 
																					throw Exception::InvalidArgument(__FILE__,__LINE__, "There is no OneLetterCode in the FASTAFile");
																	}  

																	//for every Amino Acid create a new Residue named with the name of the Amino Acid
																	Residue* r= new Residue(three_letter);

																	//set the Property to Amino Acid
																	r->setProperty(Residue::Property::PROPERTY__AMINO_ACID);

																	//append Residue to the Protein	
																	p_ptr->append(*r); 	


													}// end of iteration through string

									}//end of iteration over all sequenceLines

					}//end of iteration over all Blocks
				
				return *this;

	}

			/**
			*reads a ClustalFile into an Alignment
			* Note: take care when using this feature, because Origin of all SequenceCharacters will not be setted
			*/
			ClustalFile& ClustalFile::operator >>(Alignment& alignment)
			{return *this;}
			

			/**
			*writes a System into a ClustalFile
			*/

			/*
			ClustalFile& ClustalFile::operator << (System& system)
			{

							//first clear everything
							blocks.clear();

							//counter to denote which protein we are in
							int pr_ctr = 0;

							//iterate over all proteins of the system 
							for (MoleculeIterator m_it = S.beginMolecule(); +m_it; ++m_it)
							{

											if (RTTI::isKindOf<Protein>(*(m_it)))
											{
															// cast to Protein
															Protein* protein = RTTI::castTo<Protein>(*(m_it));
											} 
											else 
											{ 
													throw Exception::ParseError(__FILE__,__LINE__, "No Protein in this System");
											}

											//TAKE CARE OF "?"
											//get the protein's sequence
											String tmp = Peptides::GetSequence(*protein);


											//iterator for blocks 
											vector<Block>::iterator bl_it = blocks.begin();

											//initialize a SequenceLine
											ClustalFile::SequenceLine* seqline__ptr = new SequenceLine();
											seqline_ptr->ident= protein->getName();

											//insert the sequenceLine at the desired Position 
											bl_it->seqs.insert(bl_it->seqs.begin() + pr_ctr, *seqline_ptr);

											int seq_ctr = 0;

											//as long as the sequence's end isn't reached
											while(  seq_ctr != tmp.length())
											{												
															//every time the counter reaches a number that is divisible by 60 create a new SequenceLine and go to the next block
															if((seq_ctr + 1) % 60 ==	0)
															{

																			//make new SequenceLine
																			seq_line_ptr = new SequenceLine();
																			seqline_ptr->ident = protein -> getName();

																			//go to the next block and insert the SequenceLine at the desired position
																			if(++bl_it != blocks.end())
																			{
																				bl_it->seqs.insert(bl_it->seq.begin() + pr_ctr, *seqline_ptr);
																			}
																			else
																			{
																				//make a new Block
																				Block* n_bl = new Block();
																				//add it to the existing ones
																				blocks.push_back(*n_bl);
																				
																			}	
															}

															seqline_ptr->sequence += tmp[seq_ctr++];
											}
											pr_ctr++;

							}

							return *this;

			}

		*/


			/**
			*writes an Alignment into a ClustalFile
			*/
			ClustalFile& ClustalFile::operator << (Alignment& alignment)
			{ 
				
				return *this;
			}


	struct ClustalFile::State ClustalFile::state;


}

