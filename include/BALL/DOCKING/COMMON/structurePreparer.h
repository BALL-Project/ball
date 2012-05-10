/* structurePreparer.h
*
* Copyright (C) 2011 Marcel Schumann
*
* This program free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_DOCKING_COMMON_STRUCTUREPREPARER_H
#define BALL_DOCKING_COMMON_STRUCTUREPREPARER_H

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/STRUCTURE/addHydrogenProcessor.h>
#include <set>

namespace BALL
{
	class BALL_EXPORT StructurePreparer
	{
		public:

			StructurePreparer(String type="", bool ignore = 0);

			/** tries to assign the exact atom types, as defined in 'iniFile', to all atoms of 'at' */
			void assignAtomTypes(AtomContainer* at, String iniFile, String AtomType_file="");

			/** tries to assign simplied atom types based mainly on element symbols. \n @param 'iniFile' is needed to obtain the corresponding IDs (force field dependend AtomType of type int) for each of these simple type names  */
			void assignSimpleAtomTypes(AtomContainer* at, String iniFile);

			/** assign  Piecewise Linear Potential AtomTypes to all atoms of the given AtomContainer */
			void assignPLPAtomTypes(AtomContainer* at);

			void assignMMFF94AtomTypes(AtomContainer* at);

			void setScoringType(String type);

			/** assign all partial charges#
			@param overwrite_mode 0 = no overwriting of charges, 1 = overwrite all charges, 2 = automatic (overwrite all charges only if most atoms do not have a charge yet) */
			void assignCharges(AtomContainer* at, int overwrite_mode);

			/** assigns charges to a protein based on the charges template section of the specified parameter file
			@param overwrite_mode 0 = no overwriting of charges, 1 = overwrite all charges, 2 = automatic (overwrite all charges only if most atoms do not have a charge yet) */
			bool assignProteinCharges(AtomContainer* at, String parameterFile, int overwrite_mode);

			void buildBonds(AtomContainer* at);

			void saturate(AtomContainer* at);

			void selectHeavyAtoms(AtomContainer* at);

			/** run all of the above */
			void prepare(AtomContainer* at, String parameterFile, String AtomType_file="");

			void setSimpleAtomTypes(set<String>& new_types);

			const set<String>* getSimpleAtomTypes();

		private:

			bool setIonStandardCharge(Atom* atom);

			bool use_mmff94_;

			/** if == 1, unassigned atom types are ignored.\n
			else an exception is thrown if an atom type could not be assigned for at least one atom. */
			bool ignore_unassigned_;

			/** determines whether Piecewise Linear Potential AtomTypes are to be used */
			bool use_PLP_;

			set<String> simple_atom_types_;
	};
}

#endif // BALL_DOCKING_COMMON_STRUCTUREPREPARER_H
