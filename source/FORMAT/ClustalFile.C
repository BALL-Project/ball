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
		: LineBasedFile(),
		blocks()	
	{ }

	ClustalFile::ClustalFile(const String& filename, File::OpenMode open_mode, bool trim_whitespaces)
		: LineBasedFile(filename,open_mode, trim_whitespaces)
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

	bool ClustalFile::hasValidBlocks(){

		// number of lines in the first block and therefore in every block
		unsigned int num = blocks.at(0).seqs.size();

		//Vector of Idents of the first block, must be equal to that of all other blocks
		std::vector<String> idents;

		// fill the idents vector
		for(std::vector<SequenceLine>::iterator it = blocks.at(0).seqs.begin(); it != blocks.at(0).seqs.end(); it++)
		{ idents.push_back(it->ident);}

		//iterate over all blocks of the file
		for(vector<Block>::iterator it = blocks.begin(); it != blocks.end(); it++)
		{ 
			//check if the current Block has equal amount of sequences than the others
			if(it->seqs.size() != num) {return false;}

			//iterate over all sequences of the Block
			for(unsigned int i = 0; i < it->seqs.size(); i++)
			{
				if (it->getSequenceLine(i).ident != idents.at(i)) return false;
			}
		}

		return true;

	}

	bool ClustalFile::read() {

		if (!isValid())
		{
			Log.error() << "Trying to read from invalid ClustalFile '" << getName() << "'" << std::endl;
			return false;
		}

		try {
			ClustalParserLexer_reset();
			state.current_parser = this;
			//CIFParserdebug = 1;
			ClustalParserparse();
		}
		catch (Exception::ParseError& e)
		{
			Log.error() << "ClustalFile: Cannot read " << getName() << std::endl;
		}

		return true;
	}




	bool ClustalFile::write(){

		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw(File::CannotWrite(__FILE__, __LINE__, name_));
		}	

		vector<Block>::iterator it;
		for (it = blocks.begin(); it != blocks.end(); it++)
		{
			*it >> getFileStream();
		}

		return true;

	}



	void ClustalFile::addBlock(const ClustalFile::Block& block){
		blocks.insert(blocks.begin(), block);
	}




	std::vector<ClustalFile::Block> ClustalFile::getBlocks(){
		return blocks;
	}


	void ClustalFile::clear(){

		blocks.clear();
		//TODO Überklasse clear aufrufen???	
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

	bool ClustalFile::Block::addSequenceLine(SequenceLine& line)
	{

		//check whether the number at the end of the line equals the number of AminoAcids in the line
		if(line.length != line.sequence.length()) 
		{ 
			return false;
		}

		//insert the line at the beginning of the block
		seqs.insert(seqs.begin(),line);

		return true;

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

	struct ClustalFile::State ClustalFile::state;


}

