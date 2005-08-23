// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: GAMESSLogFile.C,v 1.1 2005/08/23 18:19:36 anhi Exp $
//

#include <BALL/FORMAT/GAMESSLogFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

// defined in the lexer (GAMESSLogParserLexer.l)
extern void GAMESSLogParser_initBuffer(const char* buf);
extern void GAMESSLogParser_delBuffer();
extern int  GAMESSLogParserparse();

namespace BALL
{
	GAMESSLogFile::GAMESSLogFile()
		throw()
		: GenericMolFile(),
			molecule_(0),
			factor_(1.)
	{
	}

	/** Todo: What to do with the molecule_ **/
	GAMESSLogFile::GAMESSLogFile(const GAMESSLogFile& file)
		throw(Exception::FileNotFound)
		:	GenericMolFile(),
			factor_(file.factor_)
	{
		if (file.getName() != "")
		{
			try
			{	
				open(file.getName());
			}
			catch (Exception::FileNotFound)
			{
			}
		}
	}

	GAMESSLogFile::GAMESSLogFile(const String& name, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		: GenericMolFile(),
			molecule_(0),
			factor_(1.)
	{
		open(name, open_mode);
	}

	GAMESSLogFile::~GAMESSLogFile()
		throw()
	{
	}

	const GAMESSLogFile& GAMESSLogFile::operator = (const GAMESSLogFile& rhs)
		throw(Exception::FileNotFound)
	{
		GenericMolFile::operator = (rhs);

		return *this;
	}

	bool GAMESSLogFile::write(const Molecule& molecule)
		throw(File::CannotWrite)
	{
		System S;
		S.insert(*(Molecule*)molecule.create(true));
		return write(S);
	}

	/** ToDo: Implement! :-) **/
	bool GAMESSLogFile::write(const System& system)
		throw(File::CannotWrite)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw(File::CannotWrite(__FILE__, __LINE__, name_));
		}

		/** Here goes the code... **/
	
		return true;
	}

	bool GAMESSLogFile::read(System& system)
		throw(Exception::ParseError)
	{
		return GenericMolFile::read(system);
	}

	Molecule* GAMESSLogFile::read()
		throw(Exception::ParseError)
	{
		if (!isValid())
		{
//			Log.error() << "Trying to read from invalid GAMESSLogFile '" << getName() << "'" << std::endl;
			return false;
		}

		/** Let the parser do the hard work **/
		try {
			state.current_parser = this;
			molecule_already_defined = false;
			GAMESSLogParserparse();
		}
		catch (Exception::ParseError& e)
		{
			delete molecule_;
			molecule_ = 0;
		}

		return molecule_;
	}

	void GAMESSLogFile::initRead_()
	{
	}

	void GAMESSLogFile::newMolecule()
		throw()
	{
		if (!molecule_already_defined)
			molecule_ = new Molecule;
		else
			current_atom = 0;
	}

	void GAMESSLogFile::setMoleculeName(char* name)
		throw()
	{
		if (molecule_)
			molecule_->setName(name);
	}

	void GAMESSLogFile::insertAtom(const String& element, float charge, float x, float y, float z)
		throw()
	{
		if (!molecule_already_defined)
		{
			Atom *a = new Atom;
			molecule_->insert(*a);

			a->setName(element);

			if (PTE[element] != Element::UNKNOWN)
			{
				a->setElement(PTE[element]);
				a->setRadius(PTE[element].getVanDerWaalsRadius());
			}

			/** What to do with the charge given by GAMESSLog??? **/
			// a->setCharge(charge);
			a->setPosition(Vector3(x*factor_, y*factor_, z*factor_));
		}
		else
		{
			molecule_->getAtom(current_atom)->setPosition(Vector3(x*factor_, y*factor_, z*factor_));
			current_atom++;
		}
	}
		
	void GAMESSLogFile::insertBond(Index a1, Index a2)
		throw()
	{
		Atom* at1 = molecule_->getAtom(a1);
		Atom* at2 = molecule_->getAtom(a2);

		at1->createBond(*at2);
	}
	
	void GAMESSLogFile::setCurrentCharge(float charge)
		throw()
	{
		molecule_->getAtom(current_atom)->setCharge(charge);
	}
	
	void GAMESSLogFile::setUnitConversionFactor(float factor)
		throw()
	{
		factor_ = factor;
	}

	struct GAMESSLogFile::State GAMESSLogFile::state;
}
