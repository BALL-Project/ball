// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#ifndef BALL_DOCKING_COMMON_STRUCTUREPREPARER_H
#define BALL_DOCKING_COMMON_STRUCTUREPREPARER_H

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_STRUCTURE_ADDHYDROGENSPROCESSOR_H
#include <BALL/STRUCTURE/addHydrogenProcessor.h>
#endif

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

			void setSimpleAtomTypes(std::set<String>& new_types);

			const std::set<String>* getSimpleAtomTypes();

		private:

			bool setIonStandardCharge(Atom* atom);

			bool use_mmff94_;

			/** if == 1, unassigned atom types are ignored.\n
			else an exception is thrown if an atom type could not be assigned for at least one atom. */
			bool ignore_unassigned_;

			/** determines whether Piecewise Linear Potential AtomTypes are to be used */
			bool use_PLP_;

			std::set<String> simple_atom_types_;
	};
}

#endif // BALL_DOCKING_COMMON_STRUCTUREPREPARER_H
