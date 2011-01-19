// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
		: GenericMolFile(),
			molecule_(0),
			factor_(1.)
	{
	}

	/** Todo: What to do with the molecule_ **/
	GAMESSLogFile::GAMESSLogFile(const GAMESSLogFile& file)
		:	GenericMolFile(),
			factor_(file.factor_),
			qmbs_(file.qmbs_)
	{
		if (file.getName() != "")
		{
			try
			{	
				open(file.getName());
			}
			catch (Exception::FileNotFound&)
			{
			}
		}
	}

	GAMESSLogFile::GAMESSLogFile(const String& name, File::OpenMode open_mode)
		: GenericMolFile(),
			molecule_(0),
			factor_(1.)
	{
		open(name, open_mode);
	}

	GAMESSLogFile::~GAMESSLogFile()
	{
	}

	const GAMESSLogFile& GAMESSLogFile::operator = (const GAMESSLogFile& rhs)
	{
		GenericMolFile::operator = (rhs);

		return *this;
	}

	bool GAMESSLogFile::write(const Molecule& molecule)
	{
		System S;
		S.insert(*(Molecule*)molecule.create(true));
		return write(S);
	}

	/** ToDo: Implement! :-) **/
	bool GAMESSLogFile::write(const System& system)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw(File::CannotWrite(__FILE__, __LINE__, name_));
		}

		/** Here goes the code... **/
	
		return true;
	}

	bool GAMESSLogFile::read(System& system)
	{
		return GenericMolFile::read(system);
	}

	Molecule* GAMESSLogFile::read()
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
	{
		if (!molecule_already_defined)
			molecule_ = new Molecule;
		else
			current_atom = 0;
	}

	void GAMESSLogFile::setMoleculeName(char* name)
	{
		if (molecule_)
			molecule_->setName(name);
	}

	void GAMESSLogFile::insertAtom(const String& element, float /*charge*/, float x, float y, float z)
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
	{
		Atom* at1 = molecule_->getAtom(a1);
		Atom* at2 = molecule_->getAtom(a2);

		at1->createBond(*at2);
	}

	void GAMESSLogFile::clearBonds()
	{
		AtomIterator ai;
		for (ai = molecule_->beginAtom(); +ai; ++ai)
			ai->destroyBonds();
	}
	
	void GAMESSLogFile::setCurrentCharge(float charge)
	{
		molecule_->getAtom(current_atom)->setCharge(charge);
	}
	
	void GAMESSLogFile::setUnitConversionFactor(float factor)
	{
		factor_ = factor;
	}

	void GAMESSLogFile::addCoefficient(float coefficient)
	{
		qmbs_.addCoefficient(coefficient);
	}

	void GAMESSLogFile::initializeBasisSet()
	{
		String basisfilename = "basis_";

		/** Construct the correct data file name from the basis set options **/
		if (!basis_options_.has("gbasis"))
		{
			Log.error() << "Cannot initialize basis set: options incomplete!" << std::endl;
			return;
		}

		String basis_type = basis_options_["gbasis"].trim();

		/** TODO: quite a lot!!! **/
		if ( (basis_type == "n31") || (basis_type == "n21") )
		{
				basisfilename += basis_options_["igauss"].trim() + basis_type.getSubstring(1) + "G";

				if (basis_options_["ndfunc"].toInt() > 0)
				{
					basisfilename += "*";
					if (basis_options_["npfunc"].toInt() > 0)
						basisfilename += "*";
				}
		}

		Log.info() << basisfilename << std::endl;
		qmbs_.readBasisSetData("/local/andreas/BALL_main/BALL/source/FORMAT/"+basisfilename);
	}
	
	QMBasisSet& GAMESSLogFile::getBasisSet()
	{
		return qmbs_;
	}

	const QMBasisSet& GAMESSLogFile::getBasisSet() const 
	{
		return qmbs_;
	}

	void GAMESSLogFile::addBasisOption(const String& key, const String& value)
	{
		String new_key = key;
		String new_value = value;
		new_key.toLower();
		new_value.toLower();
		basis_options_[new_key.trim()] = new_value.trim();
	}
	
	String GAMESSLogFile::getBasisOption(const String& key)
	{
		String new_key = key;
		String result;

		new_key.toLower();
		if (basis_options_.has(new_key))
			result = basis_options_[new_key];

		return result;
	}

	struct GAMESSLogFile::State GAMESSLogFile::state;
}
