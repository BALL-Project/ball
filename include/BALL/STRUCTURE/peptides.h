// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: peptides.h,v 1.3 2002/12/18 16:00:30 sturm Exp $

#ifndef BALL_STRUCTURE_PEPTIDES_H
#define BALL_STRUCTURE_PEPTIDES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#	include <BALL/KERNEL/protein.h>
#endif

namespace BALL 
{

	namespace Peptides
	{
		
		/**	@name	Type definitions
		*/
		//@{

		/// An amino acid sequence in three-letter code
		typedef std::list<String> ThreeLetterAASequence;

		/// An amino acid sequence in one-letter code
		typedef String OneLetterAASequence;
		//@}

		/**	@name Amino acid sequence conversion
		*/
		//@{

		/**	Convert a three-letter amino acid name to the one-letter abbreviation.
				If the string given is not the three-letter code of one of the twenty
				amino acids, {\tt '?'} is returned. This method is not case sensitive.
		*/
		char OneLetterCode(const String& aa);

		/**	Convert a one-letter amino acid abbreviation to its three-letter code.
				If the character given is not the code for one of the twenty amino acids,
				{\tt 'UNK'} is returned.
				This method is not case sensitive. The strings returned are all upper cases.
		*/
		String ThreeLetterCode(char aa);

		/**	Convert an amino acid sequence from three-letter code to one-letter code.
				Call \Ref{OneLetterCode} for each of the amino acids in the sequence.
		*/
		OneLetterAASequence ThreeLetterToOneLetter(const ThreeLetterAASequence& sequence);
	
		/**	Convert an amino acid sequence from one-letter code to three-letter code.
				Call \Ref{ThreeLetterCode} for each of the characters in the string.
		*/
		ThreeLetterAASequence OneLetterToThreeLetter(const OneLetterAASequence& sequence);

		/**	Get amino acid sequence from an protein.
				This method iterates over all residues of the protein, calls
				\Ref{OneLetterCode} for each of the residue names, and returns
				the concatenation of all results.
		*/
		OneLetterAASequence GetSequence(const Protein& protein);
		//@}
	
	}  // namespace Peptides

} // namespace BALL

#endif // BALL_STRUCTURE_PEPTIDES_H_


