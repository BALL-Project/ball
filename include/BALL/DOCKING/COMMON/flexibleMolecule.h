// ----------------------------------------------------
// $Maintainer: Marc Röttig $
// $Authors: Marc Röttig, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_DOCKING_COMMON_FLEXIBLEMOLECULE_H
#define BALL_DOCKING_COMMON_FLEXIBLEMOLECULE_H

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/DOCKING/COMMON/conformation.h>
#include <BALL/COMMON/exception.h>
#include <iostream>

namespace BALL
{
		/**     FlexibleMolecule class.
		        This class represents any molecule having
		        multiple conformers.
		 */
		class BALL_EXPORT FlexibleMolecule
		{
			public:

				/* Constructor.
				 */
				FlexibleMolecule(Molecule* mol);

				/** Copy constructor. */
				FlexibleMolecule(const FlexibleMolecule& flexmol);

				/** Destructor.
				 */
				virtual ~FlexibleMolecule();

				/** Add a conformation
						@param conformation pointer
				 */
				virtual void addConformation(Conformation* conf);

				/** Removes and destroyes all conformations of this FlexibleMolecule */
				void clearConformations();

				/** Generate a hashkey for the topology of the given molecule. \n
				This hashkey uses information about all atoms, bonds and bond-orders and generates a cryptographic hashkey from this data. \n
				Note that the atom positions are not taken into account by this function; if you need a hashkey for a conformation, please you Conformation::generateHash() instead.
				@param ignore_hydrogen if set to true, hydrogen atom are ignore during creation of the hashkey. This is useful for searching molecules regardless of the exact protonation state. */
				static void generateTopologyHash(const AtomContainer* mol, String& hash, bool ignore_hydrogen = false);


				/** Get the number of stored conformations.
						@return number of conformations
				 */
				Size getNumberOfConformations();

				/** Get a vector of all  stored conformations.
						@return vector of conformation pointers
				 */
				const vector < Conformation* > & getConformations() const;

				/** Get the ID  of the i-th conformation.
						@return ID of i-th conformation
				 */
				String getConformationId(Position i);

				/** Get a pointer to the i-th conformation.
						@return pointer to i-th conformation
				 */
				Conformation*  getConformation(Position i);

				/** Get a pointer to the i-th conformer ( = conformation applied to parent molecule).
						@return pointer to i-th conformer
				 */
				Molecule* getConformer(Position i);

				/** Get a pointer to the conformer by ID ( = conformation applied to parent molecule).
						@return pointer to conformer having the specified ID
				 */
				Molecule* getConformerById(String id);

				/** Get a pointer to a conformation by specifying its ID.
						@return pointer to Conformation having the specified ID
				 */
				Conformation* getConformationById(String id);

				/** Has the Flexible Molecule an associated conformation with given ID.
						@return bool
				 */
				bool hasConformation(String id);

				/** Get a pointer to the parent molecule supplied in the constructor
						@return pointer to parent molecule
				 */
				Molecule* getParent();

				/** Get the ID of the Flexible Molecule
						@return ID of Flexible Molecule
				 */
				String  getId() const;

				/** Set the ID of the Flexible Molecule
						@param id of Flexible Molecule
				 */
				void setId(String id);

				void removeMoleculeOwnership();

			protected:
				String id;
				bool checkCompatibility(Conformation* conf);
				Molecule* molecule;
				vector < Conformation* > conformations;
				HashMap < String, Conformation* > conf_ids_;
				bool molecule_ownership_;

			private:
				static void calculatePseudoSmile(const Atom* atom, String& smile, HashSet<const Atom*>& visited_atoms, bool ignore_hydrogen);
		};

		typedef FlexibleMolecule Ligand;
}

#endif /* BALL_DOCKING_COMMON_FLEXIBLEMOLECULE_H */
