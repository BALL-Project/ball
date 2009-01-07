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
	class ForceField;
	class Fragment;
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
			 *        prior to using. You can set a new FragmentDB using DNAMutator::setFragmentDB()
			 *
			 * @param mini The minimizer that shall be used for optimization of the mutated
			 *        base. If nothing or NULL is passed optimization is disabled.
			 *        You can pass a new minimizer anytime using DNAMutator::setMinimizer().
			 *
			 * @param ff The forcefield that should be used in conjunction with the minimizer
			 *        You can set a new forcefield anytime using DNAMutator::setForceField().
			 *
			 * @sa    DNAMutator::setup(), DNAMutator::setFragmentDB(), DNAMutator::setMinimizer()
			 *        DNAMutator::setForceField()
			 */
			DNAMutator(EnergyMinimizer* mini = NULL, ForceField* ff = NULL, FragmentDB* frag = NULL);

			/**
			 * The destructor of the the DNAMutator must delete the FragmentDB instance
			 * iff it has been auto generated
			 */
			~DNAMutator();

			/**
			 * Calling this method explicitly circumvents lazy loading of the FragmentDB.
			 * This method has no effect if mutate has already been called or if valid instances
			 * of the fragmentDB has been passed via the constructor.
			 *
			 * @warning If you called setup() and then set the FragmentDB to NULL via setFragmentDB()
			 *          you will need to call setup() again in order to prevent lazy loading.
			 */
			void setup();

			/**
			 * This method changes a given base to another one. Changing the base
			 * on the opposite strand is <br>not</br> handled. Most likely you will
			 * need to localy reoptimize the structure after the insertion.
			 *
			 * @param res A pointer to the Fragment that will be changed
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
			void mutate(Fragment* res, Base base, bool optimize=true) throw(Exception::InvalidOption);

			/*
			 * Set the current minimizer to mini. Passing NULL will disable
			 * minimization.
			 */
			void setMinimizer(EnergyMinimizer* mini);

			/*
			 * Set a new FragmentDB instance that shall be used to obtain
			 * the new bases. If NULL is passed, a default instance will be automatically
			 * created when calling DNAMutator::mutate().
			 */
			void setFragmentDB(FragmentDB* frag);

			/*
			 * Set a new ForceField instance that is used in conjunction with the minimizer to
			 * refine the structure. Even if no minimizer has been passed this force field will
			 * be used to calculate the most favourable rotation of the base using a simple heuristic.
			 * If you do not want this behaviour pass NULL.
			 */
			void setForceField(ForceField* ff);

			/*
			 * Controlls maximum number of steps to be used when
			 * refining the generated structure via a minimizer.
			 */
			void setMaxOptimizationSteps(Size steps);

			/*
			 * The DNAMutator internally uses the unnamed property mechanism of the Atoms.
			 * This defaults to property Atom::NUMBER_OF_PROPERTIES. If you already use this
			 * property in your code you can set another property by passing it to this function.
			 */
			void setUsedProperty(Property p);

		private:
			bool keep_db_;
			bool keep_ff_;

			FragmentDB* db_;
			ForceField* ff_;
			EnergyMinimizer* minimizer_;

			Size num_steps_;
			Property prop_;

			void freeDB_();
			void freeFF_();

			void mark_();
			void unmark_();

			void tryFlip_(Fragment* res, const Vector3& connect_atom, const Vector3& axis) const;

			/**
			 * Reoptimize the given fragment using the minimizer stored in minimizer_.
			 *
			 * @return false on error.
			 */
			bool optimize_(Fragment* frag);

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

