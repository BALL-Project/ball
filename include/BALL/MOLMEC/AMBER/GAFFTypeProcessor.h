// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MOLMEC_AMBER_GAFFTYPEPROCESSOR_H
#define BALL_MOLMEC_AMBER_GAFFTYPEPROCESSOR_H

#ifndef BALL_CONCEPT_COMPOSITE_H
# include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
# include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MOLMEC_AMBER_GAFFCESPARSER_H
# include <BALL/MOLMEC/AMBER/GAFFCESParser.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
# include <BALL/KERNEL/molecule.h>
#endif

#include <vector>
#include <map>

namespace BALL
{
	class BALL_EXPORT GAFFTypeProcessor : public UnaryProcessor<Composite>
	{
		public:
			/// Nested class definitions
			struct TypeDefinition
			{
				String atom_type;
				int atomic_number;
				int connectivity;
				String attached_hydrogens;
				String electron_withdrawal_atoms;
				String atomic_property;
				String chemical_environment;
			};

			struct BALL_EXPORT Option
			{
				/// path to the file containing the atom type definitions
				static const String ATOMTYPE_FILENAME;

				/// switch cleanup of GAFF types (cc=>cd, ...) on or off
				static const String GAFF_ATOMTYPE_POSTPROCESSING;
			};

			struct BALL_EXPORT Default
			{
				/// path to the file containing the atom type definitions
				static const String ATOMTYPE_FILENAME;

				/// switch cleanup of GAFF types (cc=>cd, ...) on or off
				static const bool GAFF_ATOMTYPE_POSTPROCESSING;
			};

			enum BALL_EXPORT BOND_TYPES
			{
				sb,
				SB,
				db,
				DB,
				TB,
				DL,
				AB
			};

			GAFFTypeProcessor();
			GAFFTypeProcessor(const Options& new_options);
			virtual ~GAFFTypeProcessor();
			virtual Processor::Result operator() (Composite &composite);

      std::set<String> getTypeNames() const;

			Options options;

		protected:
			// helper functions used in identifying the correct atom type
			std::vector<TypeDefinition> getCarbonTypes_()   { return atom_types_[6];  }
			std::vector<TypeDefinition> getHydrogenTypes_() { return atom_types_[1];  }
			std::vector<TypeDefinition> getNitrogenTypes_() { return atom_types_[7];  }
			std::vector<TypeDefinition> getOxygenTypes_()   { return atom_types_[8];  }
			std::vector<TypeDefinition> getSulfurTypes_()   { return atom_types_[16]; }

			/** Read file with table of atomtypes and push atomtypes 
			 *  and their TypeDefinition in corresponding vector
			 *  and store a GAFFCESParser for every CESstring.
			 *  The path to the atom type file is taken from the value
			 *  of the option ATOMTYPE_FILENAME.
			 */
			void parseAtomtypeTableFile_()
				throw(Exception::FileNotFound);

			/// compute aromaticity, ring memberships, GAFF bond typization, ...
			void precomputeBondProperties_(Molecule* molecule);

			/** Store connectivity, number of attached H-atoms and 
			 *  number of attached N,O,F,Cl and Br-atoms for every atom in molecule
			 */
			void precomputeAtomProperties_(Molecule* molecule);

			/// Helper for precomputeBondProperties
			void annotateBondTypes_();

			/// Helper for precomputeAtomProperties
			void annotateRingSizes_();
			void annotateAliphaticAndAromaticRingAtoms_();
			void annotatePlanarRingAtoms_();
			bool planarAtom_(const Atom& atom);

			/** Check the atom against each possible atom type and try to assign one.
			 */
			bool assignAtomtype_(Atom& atom);

			/** Postprocessing and cleanup for GAFF types
			 */
			void postProcessAtomTypes_(Molecule* molecule);

			//for every CESstring in atomtable store the "predicate tree"
			//built by GAFFCESParser
			StringHashMap<GAFFCESParser*> ces_parsers_;

			// stores the vectors of atom types indexed by atomic number for fast retrieval
			std::map<Position, std::vector<TypeDefinition> > atom_types_;

			// smallest set of smallest rings used for atomic environment strings
			std::vector<std::vector<Atom*> > sssr_;

			///
			Molecule* current_molecule_;
	};
}

#endif
