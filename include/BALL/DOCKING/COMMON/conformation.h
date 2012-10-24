// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DOCKING_COMMON_CONFORMATION_H
#define BALL_DOCKING_COMMON_CONFORMATION_H

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/CONCEPT/property.h>

#include <QtCore/QCryptographicHash>

namespace BALL
{

	/**     Conformation class.

	        This class represents conformations of a molecule by simply
	        storing the atom positions (Vector3) of all atoms that deviate significantly 
					from the resp. atom positions of the base conformation ( = parent molecule). 
					Stored conformations can be applied to
	        the parent molecule to reconstruct the conformer of the molecule.
	 */
	class BALL_EXPORT Conformation
	{
		public:

			/* Default Constructor.
			 */
			Conformation();

			/* Constructor.
				 @param pointer to AtomContainer
			 */
			Conformation(AtomContainer* parent, AtomContainer* conformation_molecule);

			/* Copy Constructor.
			 */
			Conformation (const Conformation& conf);

			/* Destructor.
			 */
			virtual ~Conformation();

			/** Generate a Sha1-hash for the given molecule */
			static void generateHash(const AtomContainer* mol, String& hash);

			/** Check whether this conformation is equal to the given Conformation. \n
			If at least one atom-position of this Conformation deviates more than 1e-02 from the resp. position in the given Conformation, or the conformations have an unequal number of atoms, false is returned. Else, true is returned. */
			bool operator == (const Conformation& conf);

			/** Check whether this conformation is unequal to the given Conformation. \n
			If at least one atom-position of this Conformation deviates more than 1e-02 from the resp. position in the given Conformation, or the conformations have an unequal number of atoms, true is returned. Else, false is returned. */
			bool operator != (const Conformation& conf);

			/** Apply the conformation of the supplied AtomContainer.
						@param reference to  AtomContainer
			 */
			void applyConformation(AtomContainer& ac) const;

			/** Get the list of all atom positions.
						@return vector of atom positions
			 */
			const HashMap<Size, Vector3>* getAtomPositions();

			/** Set the list of atom positions.
						@param HashMap of atom-indices and atom positions
			 */
			void setAtomPositions(const AtomContainer* parent, const HashMap<Size, Vector3>& pos);

			const AtomContainer* getParent();

			void setParent(AtomContainer* parent);

			/** Get the ID of the Conformation
					@return ID of Conformation
			 */
			String  getId();

			/** Check whether the hashkey of this Conformation matches the specified key. This function can thus be used to search for file corruptions. */
			bool checkID(String& hash);

			void setID(String& hash);

			/** Add a property to this Conformation. The stored properties are assigned to an AtomContainer when applyConformation() is used.
				@param name the name of the property to be added
				@param value the value of the property to be added */
			void addProperty(const String& name, const String& value);

			const std::vector<std::pair<String, String> >* getProperties();

			Size getNoOfAtoms() const;

		private:

			/** Take the conformation of the supplied AtomContainer.
						@param pointer to AtomContainer
			 */
			void takeConformation(const AtomContainer* parent, AtomContainer* ac);

			/** Take the conformation of the supplied AtomContainer.
						@param const reference to  AtomContainer
			 */
			void takeConformation(const AtomContainer& parent, const AtomContainer& ac);

			/** Convert a given value to String with a precision of 2. */
			static String doubleToString(const double& value);

			/** Generate a Sha1 hashkey for the given molecule and atom positions. This function is intended as a convenience function, so that the atom positions do not need to be applied to 'mol' before calculating the hashkey. \n
			If the number of atoms of given molecule does not match the number of given atom positions, an exception will be thrown. */
			void generateHash(const AtomContainer* mol, const HashMap<Size, Vector3>& positions, String& hash);

			String id_;
			const AtomContainer* source_;
			Size number_of_atoms_;

			/** Map containing pairs of atom-index and atom-positions */
			HashMap<Size, Vector3> atom_positions_;

			std::vector<std::pair<String, String> > properties_;
	};
}

#endif // BALL_DOCKING_COMMON_CONFORMATION_H
