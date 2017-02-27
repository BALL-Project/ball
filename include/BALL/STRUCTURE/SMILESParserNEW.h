// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SMILES_PARSER_H
#define BALL_STRUCTURE_SMILES_PARSER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif


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
	struct SPRingBond
	{
		unsigned int rb_id = 0;
		BALL::Bond::Order rb_order = BALL::Bond::ORDER__UNKNOWN;
		BALL::Atom* rb_atom = nullptr;
	};

	struct SPAtomInfo
	{
			bool is_bracket_atom = false;
			string element_symbol = "";
			bool is_aromatic = false;
			unsigned int isotope = 0;
			unsigned int explicit_hydrogens = 0;
			int formal_charge = 0;
			unsigned int atom_class = 0;

			BALL::Bond::Order bond_order_in = BALL::Bond::ORDER__UNKNOWN;
			BALL::Bond::Order bond_order_out = BALL::Bond::ORDER__UNKNOWN;
			std::vector<SPRingBond> ring_bonds;
	};

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
			SMILESParser();

			~SMILESParser();

			bool parse(const std::string& smiles, BALL::Molecule* molecule);


		private:



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

				/*
				 * Spirit symbol tables to define tokens for the different atom classes,
				 * the bond types, and the formal charges (up to +- 15).
				 */
				qi::symbols<char, std::string> atomsBracket_;
				qi::symbols<char, std::string> atomsOrgAli_;
				qi::symbols<char, std::string> atomsOrgAro_;
				qi::symbols<char, BALL::Bond::Order> bonds_;
				qi::symbols<char, int> charges_;


				qi::rule<std::string::const_iterator, void(SPAtomInfo&)> r_atom_bracket_;
				qi::rule<std::string::const_iterator, void(SPAtomInfo&)> r_atom_organic_;
				qi::rule<std::string::const_iterator, unsigned int()> r_ring_number_;
				qi::rule<std::string::const_iterator, SPRingBond()> r_ring_bond_;

				qi::rule<std::string::const_iterator, SPAtomInfo()> r_atom;
				qi::rule<std::string::const_iterator, std::vector<SPAtomInfo>() > r_chain;
				qi::rule<std::string::const_iterator, std::vector<SPMoleculeTree>()> r_branch;
				qi::rule<std::string::const_iterator, SPMoleculeTree()> r_smiles;
				qi::rule<std::string::const_iterator, SPMoleculeTree()> r_start;

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
			void createBond_(const Bond::Order bond_order, BALL::Atom* a1, BALL::Atom* a2);


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
			 * @brief printMolTree
			 * @param mt
			 * @param depth
			 */
			void dumpMolTree_(SPMoleculeTree& mt, unsigned int depth);


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

	};

} // namespace BALL


BOOST_FUSION_ADAPT_STRUCT
(
	BALL::SPMoleculeTree,
	(std::vector<BALL::SPAtomInfo>,       atom_list)
	(std::vector<BALL::SPMoleculeTree>,   children)
)

BOOST_FUSION_ADAPT_STRUCT
(
	BALL::SPRingBond,
	(BALL::Bond::Order, rb_order)
	(unsigned int, rb_id)
)


#endif // BALL_STRUCTURE_SMILES_PARSER_H


