// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_ADDHYDROGENPROCESSOR_H
#define BALL_STRUCTURE_ADDHYDROGENPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h> 
#endif

#include <vector>

namespace BALL
{
	class Residue;
	class Atom;
	class Composite;

	/**
	 * Saturate atoms with hydrogen atoms.
	 *
	 * This processor only works for main group elements.
	 * The formal charge of the atoms are taken into account.
	 * The placement of the hydrogen atoms only depends on
	 * the direct neighbour atoms.
	 *
	 * No additional optimization of the atom placement is performed.
	 * The optimal bond lengths are calculated by a modified Schomaker-Stevenson rule
	 * (adapted from the \ref MMFF94). For hydrogen atoms located in a
	 * peptide bond the atom is placed taking the planarity of the bond into account.
	 * In this case bond length is taken from the rules used by DSSP (Kabsch-Sander) as implemented
	 * by the \ref HydrogenBondProcessor.
	 * \par
	 * Usage:
	 * \code
	 * RingPerceptionProcessor rpp;
	 * vector<vector<Atom*> > rings;
	 * rpp.calculateSSSR(rings, system);
	 * rings = rpp.getAllSmallRings();
	 * AddHydrogenProcessor ap;
	 * ap.setRings(rings);
	 * system.apply(ap);
	 * \endcode
	 * 
	 * Keep in mind that the hydrogen positions obtained by using this method are usually not optimal.
   * For better better \ref Atom placment when adding hydrogens to Proteins and DNA you should use
	 * the \ref ReconstructFragmentProcessor. Nevertheless in both cases it is recommended to optimize
	 * the obtained hydrogen atom positions using a force field
	 * (e.g. \ref BALL::AmberFF, \ref BALL::CharmmFF, \ref BALL::MMFF94)
	 *
	 * \ingroup StructureMiscellaneous
	 */
	class BALL_EXPORT AddHydrogenProcessor
		: public UnaryProcessor<Composite>
	{

		public:

			/// Default constructor
			AddHydrogenProcessor();

			/// Destructor
			virtual ~AddHydrogenProcessor();

			/**
			 * Initialize the processor. Resets the number of added hydrogens.
			 *
			 * \see UnaryProcessor::start()
			 *
			 * \return true in all cases
			 */
			virtual bool start();

			/**
			 * Adds hydrogen atoms to the specified composite.
			 * If composite is a \ref Residue it uses the peptide bond rule to place hydrogens.
			 * Otherwise the placement heuristic based on the modified Schomaker-Stevenson rule is used.
			 *
			 * \see UnaryProcessor::operator()
			 *
			 * \param composite The composite that should be saturated with hydrogens
			 * \return Processor::CONTINUE is returned in all cases.
			 */
			virtual Processor::Result operator() (Composite &composite);

			/**
			 * Computes the number of connections of a provided \ref Atom
			 * the computation is solely based on the formal charge of the molecule
			 * as obtained from its group in the PTE. Hence only one connectivity is
			 * computed for every element.
			 *
			 * \param atom The \ref Atom for which the connectivity shoudl be computed
			 * \returns The number of connections the atom most likely will form. 0 if no
			 *          connectivity could be computed.
			 */
			Size getConnectivity(const Atom& atom) const;

			/**
			 * Sets the internal list of ring atoms. This is necessary computing a better
			 * estimate of the hydrogen position.
			 *
			 * \param rings A set of rings. A suitable input for this method can be computed
			 *              by using \ref RingPerceptionProcessor::calculateSSSR
			 */
			void setRings(const vector<vector<Atom*> >& rings);

			/**
			 * Returns the number of added hydrogen atoms. This value will accumulate
			 * over multiple calls of \ref operator()
			 *
			 * \return The number of added hydrogens
			 */
			Size getNumberOfAddedHydrogens() const { return nr_hydrogens_;}

		protected:
			/**
			 * Place peptide bond H-atoms according to Kabsch-Sander
			 * \param res A pointer to a \ref Residue for which a peptide bond hydrogen should be placed
			 * \returns Processor::CONTINUE in all cases.
			 */
			Processor::Result placePeptideBondH_(Residue* res);

			/**
			 * Sums up the bond orders of all bonds the atom participates in.
			 *
			 * @param atom The \ref Atom for which the bond orders should be determined
			 * @returns The bond orders of the \ref Atom.
			 */
			Size countBondOrders(const Atom& atom) const;

			/**
			 * Adds a single hydrogen an atom.
			 *
			 * @param atom The \ref Atom to which a hydrogen should be added
			 * @param position The position of the newly added hydrogen
			 */
			void addHydrogen_(Atom& atom, Vector3 position);

			/**
			 * Returns whether the atom is in the internal list of ring atoms
			 * \see AddHydrogenProcessor::setRings
			 *
			 * \param atom The \ref Atom that should be checked
			 * \returns true if the \ref Atom is in the ring \ref Atom set false otherwise
			 */
			bool isRingAtom_(const Atom& atom) const;

			/**
			 * Returns a list of atoms bound to the specified \ref Atom
			 *
			 * @param atom the atom for which the binding partners should be determined
			 * @returns A std::vector containing all \ref Atoms that have a bond to the specified \ref Atom
			 */
			vector<Atom*> getPartners_(Atom& atom) const;

			/**
			 * Computes a vector orthogonal to the input. It uses the x-axis as reference. If the input
			 * is colinear to the x-axis, the y-axis is used.
			 *
			 * \param v A vector for which an orthogonal vector shoudl be computed
			 * \returns A vector of unit length orthogonal to the input
			 */
			Vector3 getNormal_(const Vector3& v) const;

			/**
			 * Normalizes a vector.
			 *
			 * @param v The vector that should be normalized. Normalization is performed in-place.
			 * @returns true if the vector is non-zero false otherwise.
			 */
			bool normalize_(Vector3& v) const;

			/**
			 * Determines whether an \ref Atom has a bond that is not a single bond.
			 *
			 * @param atom The \ref Atom that should be checked for non-single bonds.
			 * @returns true if the atom has a a non-single bond, false otherwise
			 */
			bool hasMultipleBond_(const Atom& atom) const;

			/**
			 * Get the ideal bond length for the given element with hydrogen atoms. This uses
			 * the Schomaker-Stevenson rule.
			 *
			 * @param element The \ref Element for which to compute the distance
			 * @returns The preferred distance of a hydrogen atom from the input \ref Element
			 */
			float getBondLength_(Position element) const;

		private:
			HashSet<const Atom*> ring_atoms_;
			Position atom_nr_;
			Atom* last_atom_;
			Size nr_hydrogens_;
	}; //class AddHydrogenProcessor

} //namespace BALL

#endif // BALL_STRUCTURE_ADDHYDROGENPROCESSOR_H
