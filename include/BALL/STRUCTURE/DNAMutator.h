#ifndef DNAMUTATOR_H
#define DNAMUTATOR_H

#include <BALL/COMMON/exception.h>
#include <BALL/MATHS/vector3.h>

namespace BALL
{
	class FragmentDB;
	class Residue;
	class AtomContainer;
	class Atom;	

	class DNAMutator 
	{
		public:
			/**
			 * Constructs a DNAMutator instance using the specified FragmentDB
			 * If nothing or NULL is passed a default FragmentDB is used
			 */
			DNAMutator(FragmentDB* frag = NULL);

			/**
			 * This method changes a given base to another one. Changing the base
			 * on the opposite strand is <br>not</br> handled. Most likely you will
			 * need to localy reoptimize the structure after the insertion.
			 *
			 * @param res A pointer to the Residue that will be changed
			 * @param base A string specifiying the base which will be used to replace
			 *             the current one. Can be one of "A", "T", "G", "C" and "U".
			 *
			 * @todo - Add capabilities into the FragmentDB that allows the retrival of Molecule type information
			 *       - Currently only Purine - Purine and Pyrimidine - Pyrimidine conversion is "exact". It is quite
			 *         hard to get Purine - Pyrimidine conversion right without hardcoding
			 */
			void mutate(Residue* res, const String& base) throw(Exception::InvalidOption);

		private:
			FragmentDB* db_;
			bool keep_db_;

			/**
			 * This function returns a pointer to the nitrogen atom that attaches
			 * a base to the sugar backbone
			 */
			Atom* getAttachmentAtom(AtomContainer* res);

			/**
 			 * Selects the atoms in a base. If succesfull it returns
 			 * the pointer to the attachment nitrogen.
 			 */ 
			Atom* selectBaseAtoms(AtomContainer* res);

			void rotateBases(AtomContainer* from, Atom* from_at, 
			                 AtomContainer* to,   Atom* to_at);

			const Atom* getSecondNitro(const std::vector<const Atom*>& ring_atoms, const Atom* base);

			Vector3 getNormalVector(Atom* at);
			Vector3 getConnectionVector(Atom* at);
			Vector3 getOrthogonalVector(const Vector3& n, const Atom* base, const Atom* at);

			bool isPurine(const Atom& baseNitrogen) const;
			bool isPyrimidine(const Atom& baseNitrogen) const;

	};
}

#endif

