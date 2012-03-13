#ifndef BALL_STRUCTURE_NUCLEOTIDEMAPPING_H
#define BALL_STRUCTURE_NUCLEOTIDEMAPPING_H

#include <string>

#include <BALL/DATATYPE/hashMap.h>

namespace BALL
{
	class Chain;
	class Residue;

	/**
	 * This class defines the mapping between complementary bases in to strands
	 * of DNA. Ideally this should be tightly coupled with NucleicAcid, however as
	 * this is currently broken this class has to do the job. This also explains
	 * why a Chain is required as an input.
	 *
	 * @todo: Fix these issues once Nucleotide works properly and is supported throughout BALL
	 */
	class BALL_EXPORT NucleotideMapping
	{
		public:
			typedef std::pair<const std::string, const std::string> Alignment;

			NucleotideMapping();

			///@name Factory methods
			///@{

			/**
			 * This method constructs a Nucleotide mapping by simply assuming, that the first base on the 5' end
			 * of a is complementary to the first base on the 3' end of b.
			 *
			 * @param a The first nucleic acid, it is assumed that it is available in 5' -> 3' order.
			 * @param b The second nucleic acid, it is assumed that it is available in 5' -> 3' order.
			 * @param offset_a start matching at the base in strand a with index offset_a instead of 0
			 * @param offset_b start matching at the base in strand b with index offset_b instead of 0
			 */
			static NucleotideMapping assignNaively(Chain& a, Chain& b, unsigned int offset_a = 0, unsigned int offset_b = 0);

			/**
			 * This method tries to reconstruct the base complementarity information by looking at the atom distances between
			 * the two nucleic acids. Äote that this can fail if the strands are strongly deformed.
			 *
			 * @param a The first nucleic acid, it is assumed that it is available in 5' -> 3' order.
			 * @param b The second nucleic acid, it is assumed that it is available in 5' -> 3' order.
			 */
			static NucleotideMapping assignFromDistances(Chain& a, Chain& b);

			/**
			 * Construct a NucleotideMapping given a sequence alignment of the strands. Note that residue names
			 * are not actually checked, but rather any pair of characters different from '-' are considered
			 * as an instruction to align the next bases. E.g: for the sequences ATGC and TCCG this would be a valid input:
			 * <p>
			 * *-***
			 * **-**
			 * </p>
			 * Also the alignment does not need to cover the full length of the sequences. The assignment process simply
			 * stops when the end of the alignment is reached.
			 * @param a The first nucleic acid, it is assumed that it is available in 5' -> 3' order.
			 * @param b The second nucleic acid, it is assumed that it is available in 5' -> 3' order.
			 * @param alignment A pair of strings defining the mapping between the two strands.
			 *        it may only consist of the characters forming the respective abbreviation of the bases
			 *        and '-' for gaps.
			 */
			static NucleotideMapping assignFromAlignment(Chain& a, Chain& b, const Alignment& alignment);

			///@}

			///@name Accessors
			///@{

			/**
			 * Get a pointer to the first DNA strand
			 */
			Chain* getFirstStrand() const;

			/**
			 * Get a pointer to the second DNA strand
			 */
			Chain* getSecondStrand() const;

			/**
			 * Given a nucleotide a located in the first strand. This method returns the
			 * nucleotide complementary to a located in the second strand.
			 * First and second refers to the order the strands were passed in at creation time.
			 *
			 * @return If a is not mapped, either because it is not member of the first strand or 
			 *         because it has no complementary base 0 is returned
			 */
			Residue* firstToSecond(Residue* a) const;

			/**
			 * Given a nucleotide b located in the second strand. This method returns the
			 * nucleotide complementary to b located in the first strand.
			 * First and second refers to the order the strands were passed in at creation time.
			 *
			 * @return If a is not mapped, either because it is not member of the first strand or 
			 *         because it has no complementary base NULL is returned
			 */
			Residue* secondToFirst(Residue* b) const;

			///@}
		private:
			typedef HashMap<Residue*, Residue*> NucleotideMap;
			NucleotideMapping(Chain& a, Chain& b, const NucleotideMap& a_to_b, const NucleotideMap& b_to_a);

			Chain* a_;
			Chain* b_;

			NucleotideMap a_to_b_;
			NucleotideMap b_to_a_;
	};
}

#endif //BALL_STRUCTURE_NUCLEOTIDEMAPPING_H

