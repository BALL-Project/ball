// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: GAMESSLogFile.C,v 1.2.16.2 2006/11/22 21:51:02 anhi Exp $
//

#include <BALL/FORMAT/GAMESSLogFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/SYSTEM/path.h>
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
			factor_(1.),
			current_set(1)
	{
	}

	/** Todo: What to do with the molecule_ **/
	GAMESSLogFile::GAMESSLogFile(const GAMESSLogFile& file)
		throw(Exception::FileNotFound)
		:	GenericMolFile(),
			factor_(file.factor_),
			qmbs_(file.qmbs_),
			current_set(file.current_set)
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
			factor_(1.),
			current_set(1)
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

		current_eigenvector_ = 0;
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

	void GAMESSLogFile::insertAtom(const String& element, float /*charge*/, float x, float y, float z)
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

	void GAMESSLogFile::clearBonds()
		throw()
	{
		AtomIterator ai;
		for (ai = molecule_->beginAtom(); +ai; ++ai)
			ai->destroyBonds();
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

	void GAMESSLogFile::setNumberOfAlphaOrbitals(int number)
		throw()
	{
//		qmbs_.setNumberOfAlphaOrbitals(number);
	}

	void GAMESSLogFile::setNumberOfBetaOrbitals(int number)
		throw()
	{
//		qmbs_.setNumberOfBetaOrbitals(number);
	}

	void GAMESSLogFile::initializeBasisSet()
		throw()
	{
		String basisfilename = "QM/basisset_";

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
		Path path;
		String filename = path.find(basisfilename);
		Log.info() << filename << std::endl;
		if (filename != "")
			qmbs_.readBasisSetData(filename);
	}

	QMBasisSet& GAMESSLogFile::getBasisSet()
		throw()
		{
			return qmbs_;
		}

	const QMBasisSet& GAMESSLogFile::getBasisSet() const 
		throw()
	{
		return qmbs_;
	}

	void GAMESSLogFile::addBasisOption(const String& key, const String& value)
		throw()
	{
		String new_key = key;
		String new_value = value;
		new_key.toLower();
		new_value.toLower();
		basis_options_[new_key.trim()] = new_value.trim();
	}
	
	String GAMESSLogFile::getBasisOption(const String& key)
		throw()
	{
		String new_key = key;
		String result;

		new_key.toLower();
		if (basis_options_.has(new_key))
			result = basis_options_[new_key];

		return result;
	}

	void GAMESSLogFile::test()
		throw()
	{
		// test
		std::cout << qmbs_.alpha_eigenvalues.size() << std::endl;
		for (int i=0; i<qmbs_.alpha_eigenvalues.size(); i++)
			std::cout << qmbs_.alpha_eigenvalues[i] << " ";

		// print the first and the 7th eigenvector
		std::cout << "\n\n";
		for (int i=0; i<qmbs_.alpha_eigenvectors[0].size(); i++)
			std::cout << qmbs_.alpha_eigenvectors[0][i] << " ";
		std::cout << "\n\n";
		for (int i=0; i<qmbs_.alpha_eigenvectors[6].size(); i++)
			std::cout << qmbs_.alpha_eigenvectors[6][i] << " ";
		std::cout << "\n";
	}

	void GAMESSLogFile::initEigenData()
		throw()
	{
		// we cruelly overwrite all older data that's possibly there
		current_eigenvector_ = 0;
		if (current_set == 1)
		{
			qmbs_.alpha_eigenvalues.clear();
			qmbs_.alpha_eigenvectors.clear();
		}
		else
		{
			qmbs_.beta_eigenvalues.clear();
			qmbs_.beta_eigenvectors.clear();
		}
	}

	void GAMESSLogFile::nextEigenBlock()
		throw()
	{
		if (current_set == 1)
		{
			current_eigenvector_ = qmbs_.alpha_eigenvalues.size();
		}
		else
		{
			current_eigenvector_ = qmbs_.beta_eigenvalues.size();
		}
	}

	void GAMESSLogFile::addEigenvectors(const std::vector<String>& vectors)
		throw()
	{
		std::vector<std::vector<float> >& eigenvectors = (current_set == 1) ? qmbs_.alpha_eigenvectors
																																				: qmbs_.beta_eigenvectors;
		std::vector<float>& eigenvalues = (current_set == 1) ? qmbs_.alpha_eigenvalues
																												 : qmbs_.beta_eigenvalues;

		// do we need to push_back or did we already do that?
		if (eigenvectors.size() < eigenvalues.size()) // first line of the block
		{
			for (Size i=4; i<vectors.size(); i++)
			{
				eigenvectors.push_back(std::vector<float>());
				eigenvectors[eigenvectors.size()-1].push_back(vectors[i].toFloat());
			}
		}
		else
		{
			for (Size i=4; i<vectors.size(); i++)
			{
				eigenvectors[current_eigenvector_+(i-4)].push_back(vectors[i].toFloat());
			}
		}
	}

	void GAMESSLogFile::addEigenvalues(const std::vector<String>& values)
		throw()
	{
		if (current_set == 1)
		{
			for (Size i=0; i<values.size(); i++)
				qmbs_.alpha_eigenvalues.push_back(values[i].toFloat());
		}
		else
		{
			for (Size i=0; i<values.size(); i++)
				qmbs_.beta_eigenvalues.push_back(values[i].toFloat());
		}
	}

	struct GAMESSLogFile::State GAMESSLogFile::state;
}
