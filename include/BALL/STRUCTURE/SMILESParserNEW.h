// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SMILES_PARSER_H
#define BALL_STRUCTURE_SMILES_PARSER_H

#include <BALL/common.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/std_pair.hpp>


namespace qi = boost::spirit::qi;


namespace BALL 
{
	/*
	 * Struct to store a ring bond member atom
	 * during parsing the SMILES string
	 */
	struct SPRingBond
	{
		unsigned int rb_id = 0;
		unsigned int rb_order = 0;
		BALL::Atom* rb_atom = nullptr;
	};


	/*
	 * Struct to store atom information
	 * during parsing the SMILES string
	 */
	struct SPAtomInfo
	{
			bool is_bracket_atom = false;
			string element_symbol = "";
			bool is_aromatic = false;
			unsigned int isotope = 0;
			unsigned int explicit_hydrogens = 0;
			int formal_charge = 0;
			unsigned int atom_class = 0;

			unsigned int bond_order_in = 0;
			unsigned int bond_order_out = 0;
			std::vector<SPRingBond> ring_bonds;

			std::string chiral_class = "";
	};


	/*
	 * Struct to store parsed SMILES information in a hierarchical tree
	 */
	struct SPMoleculeTree
	{
			std::vector<SPAtomInfo> atom_list;
			std::vector<SPMoleculeTree> children;
	};


	/** @name	Parser for SMILES.
	 *          According to the OpenSMILES specifications v1.0. (http://opensmiles.org)
	 *          The parser is implemented using Boosts parser library Spirit.
	 *
	 * 	\ingroup StructureMatching
	*/
	class BALL_EXPORT SMILESParser
	{
		public:

			struct BALL_EXPORT Option
			{
					static const string SP_ADD_HYDROGENS;

					static const string SP_ADD_EXPLICIT_HYDROGENS;

					static const string SP_GENERATE_3D_COORDINATES;

					static const string SP_GENERATE_3D_COORDINATES_ISOMERIC;
			};

			struct BALL_EXPORT Default
			{
					static const bool SP_ADD_HYDROGENS;

					static const bool SP_ADD_EXPLICIT_HYDROGENS;

					static const bool SP_GENERATE_3D_COORDINATES;

					static const bool SP_GENERATE_3D_COORDINATES_ISOMERIC;
			};

			SMILESParser();

			SMILESParser(const BALL::Options& new_options);

			~SMILESParser();

			/**
			 * Parse given SMILES string and generate Molecule
			 * @param smiles SMILES string
			 * @param molecule pointer to Molecule to be populated
			 * @return
			 */
			bool parse(const std::string& smiles, BALL::Molecule* molecule);

			/**
			 * Set new SMILESParser options
			 * @param new_options Options to use
			 */
			void setOptions(const BALL::Options& new_options);

		private:

			/*
			 * Actual SMILES parser
			 * implementing a Boost spirit grammar.
			 */
			struct SMILESParserGrammar
					: qi::grammar<std::string::const_iterator, SPMoleculeTree()>
			{
				SMILESParserGrammar();

				/**
				 * Initialize boost spirit symbol tables for atoms, bonds, and formal charges.
				 */
				void initSymbolTables();

				void setBracketAtomInfo(SPAtomInfo& ai, boost::fusion::vector<unsigned int, boost::fusion::vector<std::string, bool>, unsigned int, int, unsigned int> args);

				void setAtomInfo(SPAtomInfo& ai, std::string element_symbol, bool is_aromatic);

				void setAtomInfoChiral_TH_AL_(SPAtomInfo& ai, std::string element_symbol, std::string chiral_class);

				/*
				 * Spirit symbol tables to define tokens for the different atom classes,
				 * the bond types, formal charges, and chirality information.
				 */
				qi::symbols<char, std::string> atomsBracket_;
				qi::symbols<char, std::string> atomsOrgAli_;
				qi::symbols<char, std::string> atomsOrgAro_;
				qi::symbols<char, unsigned int> bonds_;
				qi::symbols<char, int> charges_;
				qi::symbols<char, std::string> chiral_TH_;
				qi::symbols<char, std::string> chiral_AL_;
				qi::symbols<char, std::string> chiral_SP_;
				qi::symbols<char, std::string> chiral_TB_;
				qi::symbols<char, std::string> chiral_OH_;

				qi::rule<std::string::const_iterator, void(SPAtomInfo&)> r_atom_chiral_;
				qi::rule<std::string::const_iterator, void(SPAtomInfo&)> r_atom_bracket_;
				qi::rule<std::string::const_iterator, void(SPAtomInfo&)> r_atom_organic_;
				qi::rule<std::string::const_iterator, void(SPAtomInfo&)> r_atom_;
				qi::rule<std::string::const_iterator, unsigned int()> r_ring_number_;
				qi::rule<std::string::const_iterator, SPRingBond()> r_ring_bond_;

				qi::rule<std::string::const_iterator, SPAtomInfo()> r_node_;
				qi::rule<std::string::const_iterator, std::vector<SPAtomInfo>() > r_chain_;
				qi::rule<std::string::const_iterator, std::vector<SPMoleculeTree>()> r_branch_;
				qi::rule<std::string::const_iterator, SPMoleculeTree()> r_smiles_;
				qi::rule<std::string::const_iterator, SPMoleculeTree()> r_start_;

				// Single digit parser for unsigned int
				qi::uint_parser<unsigned, 10, 1, 1> uint_1_digit_;

				// Double digit parser for unsigned int
				qi::uint_parser<unsigned, 10, 2, 2> uint_2_digit_;
			};


			/**
			 * Print given error message
			 * @param error_message message to be printed
			 */
			void errorMessage_(const string& error_message);


			/**
			 * Add explicit hydrogens to the molecule generated from SMILES string
			 *  * @return
			 */
			bool addHydrogens_();


			/**
			 * Return default valence count for a given atom
			 * @param atom the atom for which the default valence count should be returned
			 * @return valence count for specified atom
			 */
			unsigned int getDefaultValence_(const Atom* atom);


			/**
			 * Create new atom and set its parameters
			 * @param ai AtomInfo struct containing the parsed atom information
			 * @return
			 */
			Atom* createAtom_(const SPAtomInfo& ai);


			/**
			 * Create bond with appropriate order between two atoms
			 * @param bond_order bond order if specified, otherwise ORDER_UNKOWN
			 * @param a1 first incident atom
			 * @param a2 second incident atom
			 */
			void createBond_(unsigned int bond_order, BALL::Atom* a1, BALL::Atom* a2);


			/**
			 * Recursively traverse MoleculeTree mol_tree and generate Molecule
			 * @param mol_tree
			 * @return
			 */
			bool iterateMoleculeTree_(const SPMoleculeTree& mol_tree, BALL::Atom* branch_atom);


			/**
			 * Generate specified ring bonds
			 * @return
			 */
			bool generateRingBonds_();


			/**
			 * Generate final Molecule from the parsed SMILES that is stored in MolecularTree struct.
			 * @param mol_tree MolecularTree instance resulting from parsing a SMILES
			 */
			bool finalizeMolecule_(const SPMoleculeTree& mol_tree);


			/**
			 * printMolTree
			 * @param mt
			 * @param depth
			 */
			void dumpMolTree_(SPMoleculeTree& mt, unsigned int depth);


			/**
			 * Set SMILESParser default options
			 */
			void setDefaultOptions_();


			/*
			 * The actual SMILESParser that is derived from boost::spirit::qi::grammar
			 */
			SMILESParserGrammar SMILESParserGrammar_;

			/*
			 * Molecule pointer that is passed to the SMILESParser object to store
			 * the Molecule that is created from a successful SMILES parsing
			 */
			BALL::Molecule* molecule_ = nullptr;

			/*
			 * Map of ring bonds to be formed
			 */
			std::unordered_map<unsigned int, std::vector<SPRingBond> > ring_bonds_;

			/*
			 * Options for the SMILESParser
			 */
			BALL::Options options_;
	};

} // namespace BALL


BOOST_FUSION_ADAPT_STRUCT
(
	BALL::SPMoleculeTree,
	(std::vector<BALL::SPAtomInfo>,     atom_list)
	(std::vector<BALL::SPMoleculeTree>, children)
)

BOOST_FUSION_ADAPT_STRUCT
(
	BALL::SPRingBond,
	(unsigned int, rb_order)
	(unsigned int, rb_id)
)


#endif // BALL_STRUCTURE_SMILES_PARSER_H


