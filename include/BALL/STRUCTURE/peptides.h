// $Id: peptides.h,v 1.1.2.1 2002/05/14 10:55:06 oliver Exp $

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
		typedef String OneLetterAASequence
		//@}

		/**	@name Amino acid sequence conversion
		*/
		//@{

		/**	Convert an amino acid sequence from three-letter code to one-letter code.
		*/
		OneLetterAASequence ThreeLetterToOneLetter(const ThreeLetterAASequence& sequence);
	
		/**	Convert an amino acid sequence from one-letter code to three-letter code.
		*/
		ThreeLetterToOneLetter(const OneLetterAASequence& sequence);

		/**	Get amino acid sequence from an protein
		*/
		OneLetterAASequence GetSequence(const Protein& protein);
		//@}
	
	}  // namespace Peptides

} // namespace BALL

#endif // BALL_STRUCTURE_PEPTIDES_H_


