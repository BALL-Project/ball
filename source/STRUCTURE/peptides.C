// $Id: peptides.C,v 1.1.2.1 2002/05/14 10:54:16 oliver Exp $

#include <BALL/STRUCTURE/peptides.h>

#include <list>
#include <vector>

	
using namespace std;

namespace BALL 
{
	namespace Peptides
	{
		const String one_letter_codes = "ARNDCQQGHILKMFPSTWYV";
		const String three_letter_codes = "ALA@ARG@ASN@ASP@CYS@GLN@GLU@GLY@HIS@ILE@LEU@LYS@MET@PHE@PRO@SER@THR@TRP@TYR@VAL@";

		char OneLetterCode(const String& aa)
		{
			if (aa.size() == 3)
			{
				aa.toUpper();
				aa += "@";
				Index idx = three_letter_codes.find(aa);
				if (idx != String::EndPos)
				{
					return one_letter_codes[idx / 3];
				}
			}
			
			return "?";
		}

		String ThreeLetterCode(char aa)
		{
			aa = toupper(aa);
			Index idx = one_letter_codes.find(aa);
			return ((idx == String::EndPos) ? "UNK" : String(three_letter_codes, idx * 3, 3));
		}

		OneLetterAASequence ThreeLetterToOneLetter(const ThreeLetterAASequence& sequence)
		{
			OneLetterAASequence tmp;
			for (Position i = 0; i < (Size)sequence.size(); i++)
			{
				tmp += OneLetterCode(sequence[i]);
			}
			return tmp;
		}

		ThreeLetterAASequence OneLetterToThreeLetter(const OneLetterAASequence& sequence)
		{
			ThreeLetterAASequence tmp;
			for (Position i = 0; i < (Size)sequence.size(); i++)
			{
				tmp.append(ThreeLetterCode(sequence[i]));
			}
			return tmp;
		}

		OneLetterSequence GetSequence(const Protein& protein)
		{
			// iterate over all residues in the protein
			OneLetterSequence tmp;
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

