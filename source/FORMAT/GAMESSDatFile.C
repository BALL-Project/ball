// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: GAMESSDatFile.C,v 1.1 2005/08/23 18:19:35 anhi Exp $
//

#include <BALL/FORMAT/GAMESSDatFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

// defined in the lexer (GAMESSDatParserLexer.l)
extern void GAMESSDatParser_initBuffer(const char* buf);
extern void GAMESSDatParser_delBuffer();
extern int  GAMESSDatParserparse();

namespace BALL
{
	GAMESSDatFile::GAMESSDatFile()
		throw()
		: GenericMolFile(),
			molecule_(0)
	{
	}

	/** Todo: What to do with the molecule_ **/
	GAMESSDatFile::GAMESSDatFile(const GAMESSDatFile& file)
		throw(Exception::FileNotFound)
		:	GenericMolFile()
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

	GAMESSDatFile::GAMESSDatFile(const String& name, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		: GenericMolFile(),
			molecule_(0)
	{
		open(name, open_mode);
	}

	GAMESSDatFile::~GAMESSDatFile()
		throw()
	{
	}

	const GAMESSDatFile& GAMESSDatFile::operator = (const GAMESSDatFile& rhs)
		throw(Exception::FileNotFound)
	{
		GenericMolFile::operator = (rhs);

		return *this;
	}

	bool GAMESSDatFile::write(const Molecule& molecule)
		throw(File::CannotWrite)
	{
		System S;
		S.insert(*(Molecule*)molecule.create(true));
		return write(S);
	}

	/** ToDo: Implement! :-) **/
	bool GAMESSDatFile::write(const System& system)
		throw(File::CannotWrite)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw(File::CannotWrite(__FILE__, __LINE__, name_));
		}

		/** Here goes the code... **/
	
		return true;
	}

	bool GAMESSDatFile::read(System& system)
		throw(Exception::ParseError)
	{
		return GenericMolFile::read(system);
	}

	Molecule* GAMESSDatFile::read()
		throw(Exception::ParseError)
	{
		if (!isValid())
		{
//			Log.error() << "Trying to read from invalid GAMESSDatFile '" << getName() << "'" << std::endl;
			return false;
		}

		/** Let the parser do the hard work **/
		try {
			state.current_parser = this;
			GAMESSDatParserparse();
		}
		catch (Exception::ParseError& e)
		{
			delete molecule_;
			molecule_ = 0;
		}

		return molecule_;
	}

	void GAMESSDatFile::initRead_()
	{
	}

	void GAMESSDatFile::newMolecule()
		throw()
	{
		molecule_ = new Molecule;
	}

	void GAMESSDatFile::setMoleculeName(char* name)
		throw()
	{
		if (molecule_)
			molecule_->setName(name);
	}

	void GAMESSDatFile::insertAtom(char* element, float charge, float x, float y, float z)
		throw()
	{
		Atom *a = new Atom;
		molecule_->insert(*a);
	
		a->setName(element);
		
		if (PTE[element] != Element::UNKNOWN)
		{
			a->setElement(PTE[element]);
			a->setRadius(PTE[element].getVanDerWaalsRadius());
		}

		/** What to do with the charge given by GAMESSDat??? **/
		// a->setCharge(charge);
		a->setPosition(Vector3(x, y, z));

	}
		
	void GAMESSDatFile::insertBond(Index a1, Index a2)
		throw()
	{
		Atom* at1 = molecule_->getAtom(a1);
		Atom* at2 = molecule_->getAtom(a2);

		at1->createBond(*at2);
	}
	
	struct GAMESSDatFile::State GAMESSDatFile::state;
}
