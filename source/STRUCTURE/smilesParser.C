// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/smilesParser.h>
#include <BALL/KERNEL/PTE.h>

#include <algorithm>

// defined in the lexer (smilesParserLexer.l)
extern void SmilesParser_initBuffer(const char* buf);
extern void SmilesParser_delBuffer();
extern void SmilesParser_destroy();
extern int SmilesParserparse();

namespace BALL
{
	SmilesParser::SPAtom::SPAtom(const String& symbol, bool in_brackets)
		:	Atom(), 
			isotope_(0),
			formal_charge_(0),
			chirality_(SmilesParser::NONCHIRAL, 0),
			is_aromatic_(false),
			in_brackets_(in_brackets)
	{
		setAromatic(islower(symbol[0]) != 0);
		String s(symbol);
		s.toUpper(0, 1);
		setElement(PTE[s]);
	}
	
	SmilesParser::SPAtom::~SPAtom()
	{
	}

	Size SmilesParser::SPAtom::getDefaultValence() const
	{
		switch (getElement().getAtomicNumber())
		{
			case  1: return 1; // hydrogen
			case  5: return 3; // boron
			case  6: return 4; // carbon
			case  7: return 3; // nitrogen
			case  8: return 2; // oxygen
			case 15: return 3; // phosphorus
			case 16: return 2; // sulfur
			case  9:
			case 17:
			case 35:
			case 53: return 1; // halogens
			default:
				break;
		};
		return 0;
	}

	Size SmilesParser::SPAtom::countRealValences() const
	{
		Size count_non_aromatic = 0;
		Size count_aromatic = 0;
		for (Position i = 0; i < countBonds(); ++i)
		{
			Size order = getBond(i)->getOrder();

			if (order == Bond::ORDER__AROMATIC)
				count_aromatic += 3;
			else 
				count_non_aromatic += order;
		}
		
		// correct for the missing "half-valence" for each aromatic bond
		Size count = count_non_aromatic + count_aromatic/2;
		
		return count;
	}

	SmilesParser::SmilesParser()
		:	system_(),
			connections_(),
			all_atoms_()
	{
	}

	SmilesParser::SmilesParser(const SmilesParser& parser)
		:	system_(parser.system_),
			connections_(),
			all_atoms_()
	{
	}

	SmilesParser::~SmilesParser()
	{
		for (Position i = 0; i < all_atoms_.size(); i++)
		{
			all_atoms_[i]->destroy();
			delete all_atoms_[i];
			all_atoms_[i] = 0;
		}
		all_atoms_.clear();

		system_.destroy();

		SmilesParser_destroy();
	}

	const System& SmilesParser::getSystem() const
	{
		return system_;
	}

	void SmilesParser::parse(const String& s)
		throw(Exception::ParseError)
	{
		// clear out previous atoms
		for (Position i = 0; i < all_atoms_.size(); i++)
		{
			all_atoms_[i]->destroy();
			delete all_atoms_[i];
		}
		all_atoms_.clear();
		system_.destroy();

		// setup all connections
		connections_.resize(MAX_CONNECTIONS);
		std::fill(connections_.begin(), connections_.end(), (SPAtom*)0);

		// make the internals of this parser available for all
		state.current_parser = this;
		state.buffer = s.c_str();
		state.char_count = 0;
		
		try
		{
			SmilesParser_initBuffer(state.buffer);
			SmilesParserparse();
			SmilesParser_delBuffer();	
		}
		catch (Exception::ParseError& e)
		{
			// Clean up the parser buffer.
			SmilesParser_delBuffer();

			// Clean up allocated memory (atoms).
			for (Position i = 0; i < all_atoms_.size(); i++)
			{
				all_atoms_[i]->destroy();
				delete all_atoms_[i];
			}
			all_atoms_.clear();

			// Propagate the parse error upwards.
			throw e;
		}		

		// fill up empty valences with hydrogens
		addMissingHydrogens();

		// Transfer all atoms into a new molecule.
		Molecule* molecule = new Molecule;
		system_.insert(*molecule);
		for (Position i = 0; i < all_atoms_.size(); i++)
		{
			molecule->insert(*all_atoms_[i]);
		}
		
		// Clean up the pointers to these atoms.
		all_atoms_.clear();
	}
	
	void SmilesParser::addMissingHydrogens()
	{
		for (Position i = 0; i < all_atoms_.size(); i++)
		{
			SPAtom& atom(*(all_atoms_[i]));	
			while (!atom.isInBrackets() && (atom.countRealValences() < (atom.getDefaultValence() + atom.getFormalCharge())))
			{
				new SPBond(&atom, createAtom("H"));
			}
		}
	}

	SmilesParser::SPAtom* SmilesParser::createAtom(const String& symbol, bool in_bracket)
	{
		SPAtom* atom = new SPAtom(symbol, in_bracket);
		all_atoms_.push_back(atom);

		return atom;
	}

	SmilesParser::SPBond::SPBond
		(SmilesParser::SPAtom* left, SmilesParser::SPAtom* right, Index order)
		:	Bond(), 
			ze_type_(SmilesParser::NONE)
	{
		left->createBond(*this, *right);
		if (left->isAromatic() && right->isAromatic())
			setOrder(Bond::ORDER__AROMATIC);
		else
			setOrder(order);
	}

	SmilesParser::SPBond::~SPBond()
	{
		destroy();
	}

	SmilesParser::ZEIsomerType SmilesParser::SPBond::getZEType() const
	{
		return ze_type_;
	}

	void SmilesParser::SPBond::setZEType(SmilesParser::ZEIsomerType type)
	{
		ze_type_ = type;
	}

	struct SmilesParser::State SmilesParser::state;
	
	void SmilesParser::createBonds
		(SmilesParser::SPAtom* atom, const SmilesParser::ConnectionList* conns)
	{
		SmilesParser::ConnectionList::const_iterator it = conns->begin();
		for (; it != conns->end(); ++it)
		{
			if (connections_[*it] == 0)
			{
				connections_[*it] = atom;
			}
			else
			{
				new SPBond(atom, connections_[*it], 1);
				connections_[*it] = 0;
			}
		}
	}

} // namespace BALL
