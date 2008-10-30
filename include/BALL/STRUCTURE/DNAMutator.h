/*
 * File: BALL/STRUCTURE/DNAMutator.h
 * Created: 23.10.2008
 * 
 * Author: Daniel Stoeckel
 */

#ifndef DNAMUTATOR_H
#define DNAMUTATOR_H

#include <BALL/COMMON/exception.h>
#include <BALL/MATHS/vector3.h>

namespace BALL
{
	class FragmentDB;
	class EnergyMinimizer;
	class Residue;
	class AtomContainer;
	class Atom;

	class BALL_EXPORT DNAMutator
	{
		public:
			/**
			 * Entries of this enum are used for the selection of bases in the <b>mutate</b> function
			 */
			enum Base { ADENINE = 0, THYMINE = 1, GUANINE = 2, CYTOSINE = 3, URACILE = 4 };

			/**
			 * Constructs a DNAMutator 
			 *
			 * @param frag A FragmentDB instance that shall be used to obtain structure
			 *        of the new bases. If nothing or NULL is passed a default 
			 *        FragmentDB is used. The FragmentDB is initialized as needed causing
			 *        a short delay when mutating the first base. If you do not want this
			 *        either pass a valid FragmentDB instance or call DNAMutator::setup()
			 *        prior to using.
			 *
			 * @param mini The minimizer that shall be used for optimization of the mutated
			 *        base. If nothing or NULL is passed optimization is disabled.
			 *        You can pass a new minimizer anytime using DNAMutator::setMinimizer().
			 */
			DNAMutator(EnergyMinimizer* mini = NULL, FragmentDB* frag = NULL);

			/**
			 * The destructor of the the DNAMutator must delete the FragmentDB instance
			 * iff it has been auto generated
			 */
			~DNAMutator();

			/**
			 * Calling this method explicitly circumvents lazy loading of the FragmentDB.
			 * This method has no effect if mutate has already been called or if valid instances
			 * of the fragmentDB has been passed via the constructor.
			 */
			void setup();

			/**
			 * This method changes a given base to another one. Changing the base
			 * on the opposite strand is <br>not</br> handled. Most likely you will
			 * need to localy reoptimize the structure after the insertion.
			 *
			 * @param res A pointer to the Residue that will be changed
			 *
			 * @param base A string specifiying the base which will be used to replace
			 *        the current one. Can be one of "A", "T", "G", "C" and "U".
			 *
			 * @param optimize This flag indicates whether the new base should undergo a
			 *        local optimization. Note that if you decide against a reoptimization
			 *        only geometric properties of the original base are taken into consideration, which
			 *        might be _very_ crude.
			 *        In order to be able to perform an optimization you must provide the mutator
			 *        with a valid instance of an EnergyMinimizer via the constructor or
			 *				DNAMutator::setMinimizer()
			 *
			 * @todo - Add capabilities into the FragmentDB that allows the retrival of Molecule type information
			 */
			void mutate(Residue* res, Base base, bool optimize=true) throw(Exception::InvalidOption);

			void setMinimizer(EnergyMinimizer* mini);
			void setFragmentDB(FragmentDB* frag);

			void setMaxOptimizationSteps(Size steps);

		private:
			bool keep_db_;

			EnergyMinimizer* minimizer_;
			FragmentDB* db_;

			Size num_steps_;

			void freeDB_();

			/**
			 * Reoptimize the given fragment using the minimizer stored in minimizer_.
			 *
			 * @return false on error.
			 */
			bool optimize_(Residue* frag);

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

			void rotateBases(AtomContainer* from, const Atom* from_at, const Atom* to_at,
			                 const Vector3& from_connection, const Vector3& to_connection);
			void rotateSameBases(AtomContainer* from, AtomContainer* to);

			const Atom* getSecondNitro(const std::vector<const Atom*>& ring_atoms, const Atom* base);

			Vector3 getNormalVector(const Atom* at);
			Atom* getConnectionAtom(Atom* at);
			Vector3 getOrthogonalVector(const Vector3& n, const Atom* base, const Atom* at);

			/**
			 * The methods below decide whether a base is a Purine or a Pyrimidine.
			 * In an ideal distant future this should be stored in the fragment and thus 
			 * be not necessary.
			 */
			bool isPurine(const Atom& baseNitrogen) const;
			bool isPyrimidine(const Atom& baseNitrogen) const;

			static const char* bases_[];
			static const Size default_num_steps_;
	};
}

#endif

