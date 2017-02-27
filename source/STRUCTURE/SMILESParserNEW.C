#include <BALL/STRUCTURE/SMILESParserNEW.h>

#include <BALL/KERNEL/PTE.h>

#include <locale>

#include <boost/bind.hpp>
#include <boost/spirit/include/phoenix.hpp>


#define SMILES_PARSER_DEBUG


using namespace std;
using namespace BALL;
using namespace boost;

namespace ascii = spirit::ascii;
namespace fsn   = boost::fusion;
namespace phx   = boost::phoenix;
namespace qi    = spirit::qi;


namespace BALL
{

	SMILESParser::SMILESParser()
	{
	}


	SMILESParser::~SMILESParser()
	{
	}


	bool SMILESParser::parse(const string& smiles, Molecule* molecule)
	{
		bool success = false;
		molecule_ = molecule;
		SPMoleculeTree mol_tree;

		string::const_iterator smiles_begin = smiles.begin();
		string::const_iterator smiles_end = smiles.end();

		bool parse_success = qi::parse(smiles_begin, smiles_end, SMILESParserGrammar_, mol_tree);

		if( parse_success && (smiles_begin == smiles_end) )
		{

#ifdef SMILES_PARSER_DEBUG
			cerr << "\n================= " << endl;
			cerr << "=== printMolTree_ " << endl;
			dumpMolTree_(mol_tree, 0);
#endif

			success = finalizeMolecule_(mol_tree);
		}

		if( !success )
		{
			errorMessage_("SMILESParser failed for: " + smiles);

			molecule_->clear();

			return false;
		}

		return true;
	}


	void SMILESParser::errorMessage_(const string& error_message)
	{
		Log.info() << error_message << endl;
	}


	Atom* SMILESParser::createAtom_(const SPAtomInfo& ai)
	{
#ifdef SMILES_PARSER_DEBUG
		cerr << "= createAtom_: " << ai.element_symbol << " " << ai.is_aromatic << endl;
#endif

		Atom* a = new Atom;
		a->setElement( PTE[ai.element_symbol] );

		if( ai.is_aromatic)
		{
			a->setProperty("aro");
		}

		if(ai.is_bracket_atom)
		{
			a->setProperty("isotope", ai.isotope);
			a->setProperty("explicit_h", ai.explicit_hydrogens);
			a->setFormalCharge( ai.formal_charge );
			a->setProperty("atom_class", ai.atom_class);

		}

		molecule_->insert(*a);


		// Processing of the atom's ring bonds
		if( !ai.ring_bonds.empty() )
		{
			std::unordered_map<unsigned int, vector<SPRingBond> >::iterator rb_iter;

			for( auto ring_bond_member : ai.ring_bonds )
			{
				rb_iter = ring_bonds_.insert( ring_bonds_.begin(), make_pair(ring_bond_member.rb_id, vector<SPRingBond>()) );
				(rb_iter->second).push_back(ring_bond_member);
				(rb_iter->second).back().rb_atom = a;
			}
		}

		return a;
	}


	void SMILESParser::createBond_(const Bond::Order bond_order, Atom* atom_1, Atom* atom_2)
	{
#ifdef SMILES_PARSER_DEBUG
		cerr << "   = createBond_: " << bond_order << endl;
#endif

		// Check for '.', which has bond order value 10 and represents disconnected atoms
		if(bond_order != 10)
		{
			Bond* bond = new Bond;
			atom_1->createBond(*bond, *atom_2);

			if(bond_order == Bond::ORDER__UNKNOWN)
			{
				if( atom_1->hasProperty("aro") && atom_2->hasProperty("aro") )
				{
					bond->setOrder(Bond::ORDER__AROMATIC);
				}
				else
				{
					bond->setOrder(Bond::ORDER__SINGLE);
				}
			}
			else
			{
				bond->setOrder(bond_order);
			}
		}
	}


	bool SMILESParser::iterateMoleculeTree_(const SPMoleculeTree& mol_tree, Atom* prev_atom)
	{
		// Check 2: Bond order suffix is not allowed
		if( mol_tree.atom_list.back().bond_order_out != Bond::ORDER__UNKNOWN)
		{
			errorMessage_("SMILESParser error: bond order suffix for branch or chain. Example: 'C=', 'C(O=)'");
			return false;
		}

		Bond::Order bond_order;

		vector<SPAtomInfo>::const_iterator ai_iter = mol_tree.atom_list.cbegin();
		Atom* curr_atom = createAtom_(*ai_iter);

		if( prev_atom != nullptr)
		{
			// If this is a branch, connect it to the preceeding chain
			// This is only necessary for the first atom
			createBond_(ai_iter->bond_order_in, prev_atom, curr_atom);
		}

		prev_atom = curr_atom;
		bond_order = ai_iter->bond_order_out;


		// Generate atoms of current chain and insert them into the molecule
		for(++ai_iter; ai_iter!= mol_tree.atom_list.cend(); ++ai_iter)
		{
			curr_atom = createAtom_(*ai_iter);
			createBond_(bond_order, prev_atom, curr_atom);

			prev_atom = curr_atom;
			bond_order = ai_iter->bond_order_out;
		}

		bool success;
		for( SPMoleculeTree mt : mol_tree.children )
		{
			success = iterateMoleculeTree_(mt, prev_atom);

			if(!success)
			{
				return false;
			}
		}

		return true;
	}


	bool SMILESParser::generateRingBonds_()
	{
#ifdef SMILES_PARSER_DEBUG
		cerr << "\n= generateRingBonds_: " << ring_bonds_.size() << endl;
#endif

		Bond* rb;

		for( auto rb_id : ring_bonds_ )
		{
			vector<SPRingBond> members = rb_id.second;

			if(members.size() % 2 != 0)
			{
				errorMessage_("SMILESParser error: unclosed ring bond (ring bond number = " + to_string(rb_id.first) + ")");
				return false;
			}

#ifdef SMILES_PARSER_DEBUG
			cerr << "   = ring bond number: " << rb_id.first << "\t members: " << members.size() << endl;
#endif

			for(unsigned int i=0; i!=members.size(); i+=2)
			{
				SPRingBond rb1 = members[i];
				SPRingBond rb2 = members[i+1];

				if( rb1.rb_order != rb2.rb_order )
				{
					errorMessage_("SMILESParser error: contradicting ring bond order (ring bond number =" + to_string(rb1.rb_id) + ")");
					return false;
				}

				createBond_(rb1.rb_order, rb1.rb_atom, rb2.rb_atom);
			}
		}

		return true;
	}


	bool SMILESParser::finalizeMolecule_(const SPMoleculeTree& mol_tree)
	{
#ifdef SMILES_PARSER_DEBUG
			cerr << "\n===================== " << endl;
			cerr << "=== finalizeMolecule_ " << endl;
#endif

		// Check 1: First atom cannot have an in-bond
		if( mol_tree.atom_list[0].bond_order_in != Bond::ORDER__UNKNOWN )
		{
			errorMessage_("SMILESParser error: bond order prefix for SMILES. Example: '=C'");
			return false;
		}

		ring_bonds_.clear();

		bool success = iterateMoleculeTree_(mol_tree, nullptr);
		success = generateRingBonds_();

		return success;
	}


	void SMILESParser::dumpMolTree_(SPMoleculeTree& mt, unsigned int depth)
	{
		cout << "======== " << depth << endl;

		for( SPAtomInfo ai : mt.atom_list )
		{
			cout << string(depth, ' ') << ai.element_symbol << " ";
		}
		cout << endl;


		++depth;

		for (unsigned int i=0; i!=mt.children.size(); ++i)
		{
			dumpMolTree_(mt.children[i], depth);
		}
	}



	SMILESParser::SMILESParserGrammar::SMILESParserGrammar()
		: SMILESParser::SMILESParserGrammar::base_type(r_start)
	{
		initSymbolTables();

		/*
		 * Spirit rules for single atoms and their bonds, including ring bonds
		 */
		r_atom_bracket_ = (
							  ( qi::int_ | qi::attr(0) ) >>
							  ( atomsOrgAro_ >> qi::attr(true) | atomsBracket_ >> qi::attr(false) ) >>
							  ( ( 'H' >> qi::int_ ) | qi::attr(0) ) >>
							  ( charges_ | qi::attr(0) ) >>
							  ( ( ':' >> qi::int_ ) | qi::attr(0) )
						  ) [ phx::bind(&SMILESParser::SMILESParserGrammar::setBracketAtomInfo, this, qi::_r1, ::_1) ];

		r_atom_organic_ = ( atomsOrgAli_ >> qi::attr(false) ) [ phx::bind(&SMILESParser::SMILESParserGrammar::setAtomInfo, this, qi::_r1, qi::_1, qi::_2) ] |
						  ( atomsOrgAro_ >> qi::attr(true) )  [ phx::bind(&SMILESParser::SMILESParserGrammar::setAtomInfo, this, qi::_r1, qi::_1, qi::_2) ];

		r_ring_number_ = uint_1_digit_ |
						 ( '%' >> uint_2_digit_ );

		r_ring_bond_ = -bonds_ >> r_ring_number_;

		r_atom = -( bonds_ )       [ phx::bind(&SPAtomInfo::bond_order_in, qi::_val) = qi::_1 ] >>
				  ( r_atom_organic_(qi::_val) | ( '[' >> r_atom_bracket_(qi::_val) >> ']' ) ) >>
				 *( r_ring_bond_   [ phx::push_back(phx::bind(&SPAtomInfo::ring_bonds, qi::_val), qi::_1 ) ] ) >>
				 -( bonds_ )       [ phx::bind(&SPAtomInfo::bond_order_out, qi::_val) = qi::_1 ];



		/*
		 * Spirit rules that implement the SMILES language
		 */
		r_chain  = +r_atom                                    [ phx::push_back(qi::_val, qi::_1) ];

		r_branch = *( '(' >> r_smiles >> ')' | r_smiles )     [ phx::push_back(qi::_val, qi::_1) ];


		r_smiles = r_chain                                    [ phx::at_c<0>(qi::_val) = qi::_1 ] >>
				   r_branch                                   [ phx::at_c<1>(qi::_val) = qi::_1 ];

		r_start  = r_smiles;
	}


	void SMILESParser::SMILESParserGrammar::setBracketAtomInfo(SPAtomInfo& ai, fsn::vector<unsigned int, fsn::vector<string, bool>, unsigned int, int, unsigned int> args)
	{
#ifdef SMILES_PARSER_DEBUG
			cerr << "= setBracketAtomInfo: isotope, element, aromatic, explicit_h, formal_charge, atom_class" << endl;
#endif

		ai.is_bracket_atom = true;
		ai.isotope =            fsn::at_c<0>(args);
		ai.element_symbol =     fsn::at_c<0>( fsn::at_c<1>(args) );
		ai.is_aromatic =        fsn::at_c<1>( fsn::at_c<1>(args) );
		ai.explicit_hydrogens = fsn::at_c<2>(args);
		ai.formal_charge =      fsn::at_c<3>(args);
		ai.atom_class =         fsn::at_c<4>(args);

#ifdef SMILES_PARSER_DEBUG
			cerr << "                        " << ai.isotope << ", '" << ai.element_symbol << "', " << ai.is_aromatic << ", " << ai.explicit_hydrogens << ", " << ai.formal_charge << ", " <<  ai.atom_class << endl;
#endif
	}


	void SMILESParser::SMILESParserGrammar::setAtomInfo(SPAtomInfo& ai, string element_symbol, bool is_aromatic)
	{
#ifdef SMILES_PARSER_DEBUG
		cerr << "= setAtomInfo: element, aromatic" << endl;
#endif

		ai.element_symbol = element_symbol;
		ai.is_aromatic = is_aromatic;

#ifdef SMILES_PARSER_DEBUG
			cerr << "                        " << ai.element_symbol << ", " << ai.is_aromatic << endl;
#endif
	}


	void SMILESParser::SMILESParserGrammar::initSymbolTables()
	{

		vector<string> atoms_bracket = {"H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P",
										"S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
										"Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh",
										"Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "Hf", "Ta", "W", "Re",
										"Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Rf", "Db",
										"Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og", "La", "Ce",
										"Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Ac", "Th",
										"Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr"};
		for( auto s : atoms_bracket )
		{
			atomsBracket_.add(s, s);
		}

		vector<string> atoms_org_ali = {"B", "C", "N", "O", "P", "S", "F", "Cl", "Br", "I", "*"};
		for ( auto s : atoms_org_ali )
		{
			atomsOrgAli_.add(s, s);
		}

		vector<string> atoms_org_aro       = {"b", "c", "n", "o", "p", "s"};
		vector<string> atoms_org_aro_value = {"B", "C", "N", "O", "P", "S"};
		for( unsigned int i=0; i!=atoms_org_aro.size(); ++i )
		{
			atomsOrgAro_.add(atoms_org_aro[i], atoms_org_aro_value[i]);
		}

		bonds_.add
				("-" , Bond::BondOrder::ORDER__SINGLE)
				("=" , Bond::BondOrder::ORDER__DOUBLE)
				("#" , Bond::BondOrder::ORDER__TRIPLE)
				("$" , Bond::BondOrder::ORDER__QUADRUPLE)
				(":" , Bond::BondOrder::ORDER__AROMATIC)
				("." , 10)
		;

		charges_.add
				("+"   , 1)
				("++"  , 2)
				("+++" , 3)
				("+1"  , 1)
				("+2"  , 2)
				("+3"  , 3)
				("+4"  , 4)
				("+5"  , 5)
				("+6"  , 6)
				("+7"  , 7)
				("+8"  , 8)
				("+9"  , 9)
				("+10" , 10)
				("+11" , 11)
				("+12" , 12)
				("+13" , 13)
				("+14" , 14)
				("+15" , 15)
				("-"   , -1)
				("--"  , -2)
				("---" , -3)
				("-1"  , -1)
				("-2"  , -2)
				("-3"  , -3)
				("-4"  , -4)
				("-5"  , -5)
				("-6"  , -6)
				("-7"  , -7)
				("-8"  , -8)
				("-9"  , -9)
				("-10" , -10)
				("-11" , -11)
				("-12" , -12)
				("-13" , -13)
				("-14" , -14)
				("-15" , -15)
		;
	}
}
