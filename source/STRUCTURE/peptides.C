// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/peptides.h>

#include <BALL/KERNEL/protein.h>

#include <list>
#include <vector>

	
using namespace std;

namespace BALL 
{
	namespace Peptides
	{

		const String one_letter_codes = "ARNDCQEGHILKMFPSTWYV";
		const String three_letter_codes = "ALA@ARG@ASN@ASP@CYS@GLN@GLU@GLY@HIS@ILE@LEU@LYS@MET@PHE@PRO@SER@THR@TRP@TYR@VAL@";

		char OneLetterCode(const String& aa)
		{
			if (aa.size() == 3)
			{
				String tmp = aa + "@";
				tmp.toUpper();
				Position idx = three_letter_codes.find(tmp);
				if (idx != String::EndPos)
				{
					return one_letter_codes[idx / 4];
				}
			}
			
			return '?';
		}

		String ThreeLetterCode(char aa)
		{
			aa = toupper(aa);
			Position idx = one_letter_codes.find(aa);
			return ((idx == String::EndPos) ? String("UNK") : String(three_letter_codes, idx * 4, 3));
		}

		OneLetterAASequence ThreeLetterToOneLetter(const ThreeLetterAASequence& sequence)
		{
			OneLetterAASequence tmp;
			ThreeLetterAASequence::const_iterator it(sequence.begin());
			for (; it != sequence.end(); ++it)
			{
				tmp += OneLetterCode(*it);
			}
			return tmp;
		}

		ThreeLetterAASequence OneLetterToThreeLetter(const OneLetterAASequence& sequence)
		{
			ThreeLetterAASequence tmp;
			for (Position i = 0; i < (Size)sequence.size(); i++)
			{
				tmp.push_back(ThreeLetterCode(sequence[i]));
			}
			return tmp;
		}

		OneLetterAASequence GetSequence(const Protein& protein)
		{
			// iterate over all residues in the protein
			OneLetterAASequence tmp;
			ResidueConstIterator it(protein.beginResidue());
			for (; +it; ++it)
			{
				//...and add the corresponding one-letter code to the sequence
				tmp += OneLetterCode(it->getName());
			}

			// return the sequence
			return tmp;
		}
		

	} // namespace Peptides

} // namespace BALL

