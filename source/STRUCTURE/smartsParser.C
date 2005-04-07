// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsParser.C,v 1.1 2005/04/07 15:57:22 bertsch Exp $
//

#include <BALL/STRUCTURE/smartsParser.h>
#include <BALL/KERNEL/PTE.h>

#include <algorithm>

// defined in the lexer (smartsarserLexer.l)
extern void SmartsParser_initBuffer(const char* buf);
extern void SmartsParser_delBuffer();
extern int SmartsParserparse();

namespace BALL
{
	SmartsParser::SPAtom::SPAtom(const String& symbol, bool in_brackets)
		:	Atom(), 
			isotope_(0),
			formal_charge_(0),
			chirality_(SmartsParser::NONCHIRAL, 0),
			is_aromatic_(false),
			in_brackets_(in_brackets)
	{
		setAromatic(islower(symbol[0]) != 0);
		String s(symbol);
		s.toUpper(0, 1);
		setElement(PTE[s]);
	}
	
	SmartsParser::SPAtom::~SPAtom()
		throw()
	{
	}

	Size SmartsParser::SPAtom::getDefaultValence() const
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

	Size SmartsParser::SPAtom::countRealValences() const
	{
		Size count = 0;
		for (Position i = 0; i < countBonds(); ++i)
		{
			count += abs(getBond(i)->getOrder());
		}
		
		// if the atom is aromatic, we asume that
		// two of the bonds were aromatic ("order" is still 1)
		// and correct for the missing two "half-valences"
		if (isAromatic())
		{
			count++;
		}
		
		return count;
	}

	SmartsParser::SmartsParser()
		:	system_(),
			connections_(),
			all_atoms_()
	{
	}

	SmartsParser::SmartsParser(const SmartsParser& parser)
		:	system_(parser.system_),
			connections_(),
			all_atoms_()
	{
	}

	SmartsParser::~SmartsParser()
	{
		for (Position i = 0; i < all_atoms_.size(); i++)
		{
			all_atoms_[i]->destroy();
			delete all_atoms_[i];
			all_atoms_[i] = 0;
		}
		all_atoms_.clear();

		system_.destroy();
	}

	const System& SmartsParser::getSystem() const
	{
		return system_;
	}

	void SmartsParser::parse(const String& s)
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
			SmartsParser_initBuffer(state.buffer);
			SmartsParserparse();
			SmartsParser_delBuffer();	
		}
		catch (Exception::ParseError& e)
		{
			// Clean up the parser buffer.
			SmartsParser_delBuffer();

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
	
	void SmartsParser::addMissingHydrogens()
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

	SmartsParser::SPAtom* SmartsParser::createAtom(const String& symbol, bool in_bracket)
	{
		SPAtom* atom = new SPAtom(symbol, in_bracket);
		all_atoms_.push_back(atom);

		return atom;
	}

	SmartsParser::SPBond::SPBond
		(SmartsParser::SPAtom* left, SmartsParser::SPAtom* right, Index order)
		:	Bond(), 
			ze_type_(SmartsParser::NONE)
	{
		left->createBond(*this, *right);
		setOrder(order);
	}

	SmartsParser::SPBond::~SPBond()
		throw()
	{
		destroy();
	}

	SmartsParser::ZEIsomerType SmartsParser::SPBond::getZEType() const
	{
		return ze_type_;
	}

	void SmartsParser::SPBond::setZEType(SmartsParser::ZEIsomerType type)
	{
		ze_type_ = type;
	}

	struct SmartsParser::State SmartsParser::state;
	
	void SmartsParser::createBonds
		(SmartsParser::SPAtom* atom, const SmartsParser::ConnectionList* conns)
	{
		SmartsParser::ConnectionList::const_iterator it = conns->begin();
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
