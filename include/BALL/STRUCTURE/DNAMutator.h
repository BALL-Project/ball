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
#include <BALL/STRUCTURE/mutator.h>
#include <BALL/STRUCTURE/nucleotideMapping.h>

#include <deque>
#include <map>

namespace BALL
{
	class Chain;
	class Residue;
	class FragmentDB;
	class EnergyMinimizer;
	class ForceField;
	class Fragment;
	class AtomContainer;
	class Atom;

	class BALL_EXPORT DNAMutator
		: public Mutator
	{
		public:
			///This controls how a purine and pyrimidine base are matched onto
			/// each other
			enum MatchingMode
			{
				///Use a minimum angle criterion and try out which conformation fits best
				MINIMUM_ANGLE,
				///Set the torsion angle of the sugar-base connection in the new base
				///to the torsion angle found in the second base
				MATCH_TORSION
			};
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
			 * The destructor of the DNAMutator; Deletes the FragmentDB instance
			 * iff it has been auto generated
			 */
			~DNAMutator();

			/**
			 * Calling this method explicitly circumvents lazy loading of the FragmentDB.
			 * This method has no effect if mutate has already been called or if valid instances
			 * of the FragmentDB has been passed via the constructor.
			 *
			 * @warning If you called setup() and then set the FragmentDB to NULL via setFragmentDB()
			 *          you will need to call setup() again in order to prevent lazy loading.
			 */
			virtual void setup();

			/**
			 * Set the current minimizer to mini. Passing NULL will disable
			 * minimization.
			 */
			void setMinimizer(EnergyMinimizer* mini);

			/**
			 * Set a new ForceField instance that is used in conjunction with the minimizer to
			 * refine the structure. Even if no minimizer has been passed this force field will
			 * be used to calculate the most favourable rotation of the base using a simple heuristic.
			 * If you do not want this behaviour pass NULL.
			 */
			void setForceField(ForceField* ff);

			/**
			 * Controlls maximum number of steps to be used when
			 * refining the generated structure via a minimizer.
			 */
			void setMaxOptimizationSteps(Size steps);

			/**
			 * The DNAMutator internally uses the unnamed property mechanism of the Atoms.
			 * This defaults to property Atom::NUMBER_OF_PROPERTIES. If you already use this
			 * property in your code you can set another property by passing it to this function.
			 */
			void setUsedProperty(Property p);

			/**
			 * This function optimizes the new bases. Inherited from Mutator.
			 * Calling it before mutate will have no effect.
			 *
			 * @return true if the optimization was successful false otherwise.
			 */
			bool optimize();

			/**
			 * Sets the first and the second strand. This will lead to the computation
			 * of a NucleotideMapping using NucleotideMapping::assignNaively
			 *
			 * @sa NucleotideMapping::assignNaively
			 */
			void setStrands(Chain* s1, Chain* s2);

			/**
			 * Sets the first strand. If the second strand is already specified, this will lead
			 * to the computation of a NucleotideMapping using NucleotideMapping::assignNaively
			 *
			 * @sa NucleotideMapping::assignNaively
			 */
			void setFirstStrand(Chain* s1);

			/**
			 * Sets the second strand. If the first strand is already specified, this will lead
			 * to the computation of a NucleotideMapping using NucleotideMapping::assignNaively
			 *
			 * @sa NucleotideMapping::assignNaively
			 */
			void setSecondStrand(Chain* s2);

			/**
			 * Assign a custom NucleotideMapping.
			 */
			void setNucleotideMapping(const NucleotideMapping& bij);

			/**
			 * Implements Mutator::addMutation.
			 *
			 * @sa Mutator::addMutation
			 *
			 * @throw Exception::NotInitialized This exception is thrown when the first DNA
			 *        strand has not been set.
			 * @throw Exception::IndexOverflow If the specified index is larger than the number of
			 *        bases in the first strand, an exception is raised.
			 */
			void addMutation(Index i, const String& new_frag_name);

			/**
			 * Clears the already specified mutations.
			 */
			void clearMutations();

			/**
			 * Sets the matching heuristic used for aligning a pyrimidine to a purine base.
			 * Default is MATCH_TORSION.
			 */
			void setMatchingMode(MatchingMode mmode);

		protected:
			virtual void mutate_impl_(MutatorOptions opt);

		private:
			typedef std::map<Residue*, String> Mutations;
			typedef Mutations::iterator MutIterator;

			bool keep_ff_;

			ForceField* ff_;
			EnergyMinimizer* minimizer_;

			Size num_steps_;
			Property prop_;

			Chain* first_strand_;
			Chain* second_strand_;

			NucleotideMapping mapping_;
			MatchingMode matching_mode_;

			void mutateSingleBase_(Residue* res, const String& basename) const;

			void freeFF_();

			void mark_(AtomContainer* atoms) const;
			void unmark_(AtomContainer* atoms) const;

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
			Atom* getAttachmentAtom(AtomContainer* res) const;

			/**
			 * Selects the atoms in a base. If succesfull it returns
			 * the pointer to the attachment nitrogen.
			 */
			Atom* markBaseAtoms_(AtomContainer* res) const;

			void alignBases_(AtomContainer* from, const Vector3& from_connection, const Vector3& to_connection, Atom* from_at) const;

			Atom* getTorsionDefiningAtom_(Atom* atom) const;
			const Atom* getTorsionDefiningAtom_(const Atom* atom) const;

			void rotateBasesMatchTorsion_(AtomContainer* from, const Atom* to_connection_at, Atom* from_at, const Atom* to_at) const;
			void rotateBasesMinAngle_    (AtomContainer* from, const Vector3& to_connection,   Atom* from_at, const Atom* to_at) const;
			void rotateSameBases_(AtomContainer* from, AtomContainer* to) const;

			Vector3 getNormalVector_(const Atom* at) const;
			Atom* getConnectionAtom_(Atom* at) const;

			String canonizeName_(const String& frag_name) const;

			/**
			 * The methods below decide whether a base is a Purine or a Pyrimidine.
			 * In an ideal distant future this should be stored in the fragment and thus
			 * be not necessary.
			 */
			bool isPurine_(const Atom& baseNitrogen) const;
			bool isPyrimidine_(const Atom& baseNitrogen) const;

			static const char* bases_[];
			static const Size default_num_steps_;

			String getComplement_(const String& s) const;

			std::map<Residue*, String> mutations_;
			std::deque<Atom*> to_optimize_;
	};
}

#endif

