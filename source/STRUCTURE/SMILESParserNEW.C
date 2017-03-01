#include <BALL/STRUCTURE/SMILESParserNEW.h>

#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/forEach.h>
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
	const string SMILESParser::Option::SP_ADD_HYDROGENS                    = "add_hydrogens";
	const string SMILESParser::Option::SP_ADD_EXPLICIT_HYDROGENS           = "add_explicit_hydrogens";
	const string SMILESParser::Option::SP_GENERATE_3D_COORDINATES          = "generate_3D_coordinates";
	const string SMILESParser::Option::SP_GENERATE_3D_COORDINATES_ISOMERIC = "generate_3D_coordinates_isomeric";

	const bool SMILESParser::Default::SP_ADD_HYDROGENS                    = false;
	const bool SMILESParser::Default::SP_ADD_EXPLICIT_HYDROGENS           = false;
	const bool SMILESParser::Default::SP_GENERATE_3D_COORDINATES          = false;
	const bool SMILESParser::Default::SP_GENERATE_3D_COORDINATES_ISOMERIC = false;


	SMILESParser::SMILESParser()
	{
		setDefaultOptions_();
	}


	SMILESParser::SMILESParser(const Options& new_options)
		: options_(new_options)
	{
	}


	SMILESParser::~SMILESParser()
	{
	}


	void SMILESParser::setOptions(const Options& new_options)
	{
		options_ = new_options;
	}


	void SMILESParser::setDefaultOptions_()
	{
		options_.setDefault(Option::SP_ADD_HYDROGENS, Default::SP_ADD_HYDROGENS);
		options_.setDefault(Option::SP_ADD_EXPLICIT_HYDROGENS, Default::SP_ADD_EXPLICIT_HYDROGENS);
		options_.setDefault(Option::SP_GENERATE_3D_COORDINATES, Default::SP_GENERATE_3D_COORDINATES);
		options_.setDefault(Option::SP_GENERATE_3D_COORDINATES_ISOMERIC, Default::SP_GENERATE_3D_COORDINATES_ISOMERIC);
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

		if( options_.getBool(Option::SP_ADD_HYDROGENS) )
		{
			success = addHydrogens_();
		}

		return true;
	}


	void SMILESParser::errorMessage_(const string& error_message)
	{
		Log.error() << error_message << endl;
	}


	unsigned int SMILESParser::getDefaultValence_(const Atom* atom)
	{
		switch ( atom->getElement().getAtomicNumber() )
		{
			case 1:  return 1;
			case 5:  return 3;
			case 6:  return 4;
			case 7:  return 3;
			case 8:  return 2;
			case 15: return 3;
			case 16: return 2;
			case 9:  return 1;
			case 17: return 1;
			case 35: return 1;
			case 53: return 1;
			default: return 0;
		};
	}


	bool SMILESParser::addHydrogens_()
	{
#ifdef SMILES_PARSER_DEBUG
		cerr << "\n================= " << endl;
		cerr << "= addHydrogens_" << endl;
#endif

		AtomIterator ai;
		vector<Atom*> added_hydrogens;
		BALL_FOREACH_ATOM(*molecule_, ai)
		{
			cerr << ai->getElement().getSymbol() << endl;

			unsigned int valence_count_aro = 0;
			unsigned int valence_count = 0;

			AtomBondIterator bi;
			BALL_FOREACH_ATOM_BOND(*ai, bi)
			{
				Bond::Order bond_order = bi->getOrder();

				if( bond_order == Bond::ORDER__AROMATIC )
				{
					valence_count_aro += 3;
				}
				else
				{
					valence_count += bond_order;
				}
			}

			unsigned int open_valences = getDefaultValence_(&*ai) + ai->getFormalCharge() - valence_count - valence_count_aro / 2;

			Atom* h;
			Bond* bond;
			for(unsigned int i=0; i!=open_valences; ++i)
			{
				h = new Atom;
				h->setElement( PTE["H"] );

				bond = new Bond;
				ai->createBond(*bond, *h);

				added_hydrogens.push_back(h);
			}
		}

		for( auto h : added_hydrogens)
		{
			molecule_->insert(*h);
		}

		return true;
	}


	Atom* SMILESParser::createAtom_(const SPAtomInfo& ai)
	{
#ifdef SMILES_PARSER_DEBUG
		cerr << "= createAtom_: " << ai.element_symbol << " " << ai.is_aromatic << endl;
#endif

		// Create new atom, set element, and insert it into the molecule
		Atom* a = new Atom;
		molecule_->insert(*a);

		// If not wildcard '*' element symbol
		if( ai.element_symbol != "*" )
		{
			a->setElement( PTE[ai.element_symbol] );
		}

		// If aromatic atom, set property accordingly
		if( ai.is_aromatic)
		{
			a->setProperty("aro");
		}

		// If bracket atom, set necessary properties
		if( ai.is_bracket_atom )
		{
			a->setProperty("isotope", ai.isotope);
			a->setProperty("explicit_h", ai.explicit_hydrogens);
			a->setFormalCharge( ai.formal_charge );
			a->setProperty("atom_class", ai.atom_class);

			// This also includes chiral atoms that have been specified in brackets
			if( !ai.chiral_class.empty() )
			{
				a->setProperty("chiral", ai.chiral_class);
			}
		}


		// If specified, store the ring bonds in the global ring bond map
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


	void SMILESParser::createBond_(unsigned int bond_order, Atom* atom_1, Atom* atom_2)
	{
#ifdef SMILES_PARSER_DEBUG
		cerr << "   = createBond_: " << bond_order << endl;
#endif

		// bond_order = 6: bond order symbol '.'
		// This secifies disconnected atoms
		if(bond_order != 6)
		{
			Bond* bond = new Bond;
			atom_1->createBond(*bond, *atom_2);

			// bond_order = 0: bond not explicitely specified
			if(bond_order == 0)
			{
				if( atom_1->hasProperty("aro") && atom_2->hasProperty("aro") )
				{
					// Thus, either an aromatic bond
					bond->setOrder(Bond::ORDER__AROMATIC);
				}
				else
				{
					// Or a single bind
					bond->setOrder(Bond::ORDER__SINGLE);
				}
			}
			else
			{
				if( bond_order <= 5 )
				{
					// 1 <= bond_order <= 5: bond explicitely specified
					bond->setOrder(bond_order);
				}
				else
				{
					// Two possible cases are left: cis-trans bond order symbols

					if( bond_order == 7)
					{
						// bond_order = 7: cis-trans single bond '/'
						//atom_2->setProperty("cis_trans", "");
					}
					else
					{
						// bond_order = 8: cis-trans single bond '\'
					}

					// Both imply formation of a single bond
					bond->setOrder(Bond::ORDER__SINGLE);
				}

			}
		}
	}


	bool SMILESParser::iterateMoleculeTree_(const SPMoleculeTree& mol_tree, Atom* prev_atom)
	{
		// Sanity check: Bond order suffix is not allowed
		if( mol_tree.atom_list.back().bond_order_out != 0)
		{
			errorMessage_("SMILESParser error: bond order suffix for branch or chain. Example: 'C=', 'C(O=)'");
			return false;
		}

		unsigned int bond_order;


		// Generate first atom of current chain and insert it into the molecule
		// The first atom is treated differently because it can have a predecessor (branch) or not (first atom in SMILES)
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

		// Generate remeining atoms of rcurrent chain and insert them into the molecule
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
			// Recursively call this method for all branches that are attached to this chain
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

		// Iterate over all ring bond indices.
		for( auto rb_id : ring_bonds_ )
		{
			// The map value is a vector that contains all atoms that specified this ring bond index.
			vector<SPRingBond> members = rb_id.second;

			// Sanity check: if the member count is odd, there is an unmatched (non-closed) ring bond.
			//               We do not accept this.
			if(members.size() % 2 != 0)
			{
				errorMessage_("SMILESParser error: unclosed ring bond (ring bond number = " + to_string(rb_id.first) + ")");
				return false;
			}

#ifdef SMILES_PARSER_DEBUG
			cerr << "   = ring bond number: " << rb_id.first << "\t members: " << members.size() << endl;
#endif

			// All ring bonds can be closed.
			// Iterate over all ring bond member pairs and create the bond.
			for(unsigned int i=0; i!=members.size(); i+=2)
			{
				SPRingBond rb1 = members[i];
				SPRingBond rb2 = members[i+1];

				// Sanity check: ring bond orders can be specified explicitely, but they have to match.
				//               Contradicting ring bond orders are not accepted.
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

		// Sanity check: First atom cannot have an in-bond
		if( mol_tree.atom_list[0].bond_order_in != 0 )
		{
			errorMessage_("SMILESParser error: bond order prefix for SMILES. Example: '=C'");
			return false;
		}

		ring_bonds_.clear();

		bool success = iterateMoleculeTree_(mol_tree, nullptr);

		if(success)
		{
			success = generateRingBonds_();
		}

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
		: SMILESParser::SMILESParserGrammar::base_type(r_start_)
	{
		initSymbolTables();

		/*
		 * Spirit rules for single atoms and their bonds, including ring bonds
		 */
		r_ring_number_ = uint_1_digit_ |
						 ( '%' >> uint_2_digit_ );

		r_ring_bond_ = -bonds_ >> r_ring_number_;

		r_atom_chiral_  = ( ascii::string("C") >> ( chiral_TH_ | chiral_AL_ ) ) |
						  ( atomsBracket_      >> ( chiral_SP_ | chiral_TB_ | chiral_OH_ ) ) [ phx::bind(&SMILESParser::SMILESParserGrammar::setAtomInfoChiral_TH_AL_, this, qi::_r1, qi::_1, qi::_2) ];

		r_atom_bracket_ = (
							  ( qi::int_ | qi::attr(0) ) >>
							  ( atomsOrgAro_ >> qi::attr(true) | atomsBracket_ >> qi::attr(false) ) >>
							  ( !ascii::char_('@') ) >>
							  ( ( 'H' >> qi::int_ ) | qi::attr(0) ) >>
							  ( charges_ | qi::attr(0) ) >>
							  ( ( ':' >> qi::int_ ) | qi::attr(0) )
						  )                  [ phx::bind(&SMILESParser::SMILESParserGrammar::setBracketAtomInfo, this, qi::_r1, ::_1) ];

		r_atom_organic_ = atomsOrgAli_       [ phx::bind(&SMILESParser::SMILESParserGrammar::setAtomInfo, this, qi::_r1, qi::_1, false) ] |
						  atomsOrgAro_       [ phx::bind(&SMILESParser::SMILESParserGrammar::setAtomInfo, this, qi::_r1, qi::_1, true) ];

		r_atom_ = r_atom_organic_(qi::_r1) |
				  (
					'[' >>
					(
						  r_atom_bracket_(qi::_r1) |
						  r_atom_chiral_(qi::_r1 )
					 ) >>
					']'
				  );

		r_node_ = -( bonds_ )                [ phx::bind(&SPAtomInfo::bond_order_in, qi::_val) = qi::_1 ] >>
					 r_atom_(qi::_val)  >>
				  *( r_ring_bond_            [ phx::push_back(phx::bind(&SPAtomInfo::ring_bonds, qi::_val), qi::_1 ) ] ) >>
				  -( bonds_ )                [ phx::bind(&SPAtomInfo::bond_order_out, qi::_val) = qi::_1 ];



		/*
		 * Spirit rules that implement the SMILES language
		 */
		r_chain_  = +r_node_                                   [ phx::push_back(qi::_val, qi::_1) ];

		r_branch_ = *( '(' >> r_smiles_ >> ')' | r_smiles_ )   [ phx::push_back(qi::_val, qi::_1) ];

		r_smiles_ = r_chain_                                   [ phx::at_c<0>(qi::_val) = qi::_1 ] >>
					r_branch_                                  [ phx::at_c<1>(qi::_val) = qi::_1 ];

		r_start_  = r_smiles_;
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


	void SMILESParser::SMILESParserGrammar::setAtomInfoChiral_TH_AL_(SPAtomInfo& ai, string element_symbol, string chiral_class)
	{
#ifdef SMILES_PARSER_DEBUG
		cerr << "= setAtomInfoChiral_TH_AL: element, chiral_class" << endl;
#endif

		ai.is_bracket_atom = true;
		ai.element_symbol = element_symbol;
		ai.chiral_class = chiral_class;

#ifdef SMILES_PARSER_DEBUG
			cerr << "                        " << ai.element_symbol << ", " << ai.chiral_class << endl;
#endif
	}


	void SMILESParser::SMILESParserGrammar::initSymbolTables()
	{
		// Populate atom element symbol tables
		vector<string> atoms_bracket = {"*", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P",
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

		vector<string> atoms_org_ali = {"*", "B", "C", "N", "O", "P", "S", "F", "Cl", "Br", "I", "*"};
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


		// Populate bond symbol tables
		bonds_.add
				("-" ,  1)
				("=" ,  2)
				("#" ,  3)
				("$" ,  4)
				(":" ,  5)
				("." ,  6)
				("/" ,  7)
				("\\" , 8)
		;


		// Populate charge symbol tables
		charges_.add
				("+"   , 1)
				("++"  , 2)
				("+++" , 3)
				("-"   , -1)
				("--"  , -2)
				("---" , -3)
		;

		for( unsigned int i=1; i<=15; ++i)
		{
			charges_.add(to_string(i), i);
			charges_.add(to_string(i), -i);
		}


		// Populate chiral symbol tables
		chiral_TH_.add
				("@",     "@")     // Anti-clockwise
				("@H",    "@H")    // Anti-clockwise, implicit hydrogen
				("@@",    "@@")    // Clockwise
				("@@H",   "@@H")   // Clockwise, implicit hydrogen
				("@TH1",  "@")     // Anti-clockwise, tetrahedral
				("@TH1H", "@H")    // Anti-clockwise, tetrahedral, implicit hydrogen
				("@TH2",  "@@")    // Clockwise, tetrahedral
				("@TH2H", "@@H")   // Clockwise, tetrahedral, implicit hydrogen
		;

		chiral_AL_.add
				("@AL1",  "@AL1")  // Anti-clockwise, allenal
				("@AL2",  "@AL2")  // Clockwise, allenal
		;

		for( unsigned int i=1; i<=3; ++i)
		{
			chiral_SP_.add("@SP" + to_string(i),  "@SP" + to_string(i));
		}

		for( unsigned int i=1; i<=20; ++i)
		{
			chiral_TB_.add("@TB" + to_string(i),  "@TB" + to_string(i));
		}

		for( unsigned int i=1; i<=30; ++i)
		{
			chiral_OH_.add("@OH" + to_string(i),  "@OH" + to_string(i));
		}
	}
}
