// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/fastaFile.h>
#include <BALL/KERNEL/residueIterator.h>
#include <BALL/STRUCTURE/fragmentDB.h>

namespace BALL
{
	FastaFile::FastaFile()
	{
	}

	FastaFile::FastaFile(const BALL::String& filename, BALL::File::OpenMode open_mode, bool trim_whitespaces)
			: LineBasedFile::LineBasedFile(filename, open_mode, trim_whitespaces)
	{
	}

	bool FastaFile::operator == (const FastaFile& f)
	{
		return LineBasedFile::operator==(f);
	}

	bool FastaFile::operator != (const FastaFile& f)
	{
		return LineBasedFile::operator != (f);
	}


	/*
	* reads a FastaFile into a protein
	* @throws InvalidArgument if a file holding more than one sequence is to be read
	*/
	void FastaFile::read(Protein& protein)
	{
		// read the first line
		if (!readLine())
			throw Exception::InvalidArgument(__FILE__,__LINE__, "could not read the current line");

		// extract the current line
		String& current_str_l = getLine();

		// sequence characters
		String sequence_text;

		// name of the protein
		String name;

		readLine();

		// iterate over all lines of the file
		while (!eof())
		{
			// extract current line of the sequence
			current_str_l = getLine().trim();

			// extract the first character of the current line to check whether it is a comment or a headline
			char c = current_str_l.toChar();

			// skip the comment
			if ((c == ';'))
			{
				// nothing to do 
				readLine();
			}
			// a fasta header line
			else if ((c == '>'))
			{
				vector<String> fields;

				if (current_str_l.countFields(" ")>0)
				{
					fields.clear();
					current_str_l.split(fields);
					name = fields[0]; //TODO: is there a leading "> symbol" ??
				}
				readLine();
			}
			else  // or we have the sequence text
			{
				c = current_str_l.toChar();
				sequence_text = "";
				while (!eof() && ( (c != ';') && (c != '>')))
				{
					// iterate through the string to get the single characters
					for (unsigned int j=0; j<current_str_l.length(); j++)
					{
						char check = current_str_l[j];

						// check whether the next character is a valid AS and if it is change it to three letter code
						if (Peptides::IsOneLetterCode(check))
						{
							sequence_text += check;
						}
						else // TODO handle nucleic acid
						{
							throw Exception::InvalidArgument(__FILE__,__LINE__, "There is no OneLetterCode in the FastaFile");
						}
					}

					readLine();
					if (!eof())
					{
						current_str_l = getLine();
						c = current_str_l.toChar();
					}
					else
					{
						c = ' ';
					}
				}
				// create a chain	
				Sequence sequence(name, sequence_text);
				Chain* chain = (dynamic_cast<Protein*>(sequence.getOrigin())->getChain(0));
				protein.append(*chain);
			}
		}
	}


	/*
	 * reads a FastaFile into a System
	 */
	void FastaFile::read(System& system)
	{
		Protein* new_protein = new Protein;
		read(*new_protein);
		system.append(*new_protein);
	}


	/*
	*  writes a Protein into a Fastafile
	*/
	void FastaFile::write(const Protein& protein)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		ChainConstIterator resit = protein.beginChain();
		for (; +resit ; ++resit)
		{
			// get the protein's sequence
			String seq= Peptides::GetSequence(*resit);
			getFileStream()<< "> " << protein.getName() + ":" + resit->getName() << std::endl << seq << std::endl;
		}

		/*String seq;

		for (; +resit ; ++resit)
		{
			if (resit->isAminoAcid())
			{
				seq += Peptides::OneLetterCode(resit->getName());
			}
		}
		getFileStream() << "> " << protein.getName() << std::endl << seq << std::endl;
		*/
	}

	/* 
	* write a molecule into a Fastafile
	*/
	void FastaFile::write(const Molecule& mol)
	{
		if (RTTI::isKindOf<Protein>(mol))
		{
			Protein* prot = RTTI::castTo<Protein>(mol);
			prot->setName(mol.getName());

			write(*prot);
		}
		else if (RTTI::isKindOf<NucleicAcid>(mol))
		{
			// TODO
			Log.warn() << "FastaFile::write(const Molecule& mol): export of nucleic acids not implemented yet!" << std::endl;
		}
	}

	/* 
	* write a system into a Fastafile
	*/
	void FastaFile::write(const System& system)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		for (ProteinConstIterator it = system.beginProtein(); +it; ++it)
		{
			write(*it);
		}
	}

	/*
	 * clear method
	 */
	void FastaFile::clear()
	{
		LineBasedFile::clear();
	}


# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/FORMAT/fastaFile.iC>
# endif


}//namespace BALL
