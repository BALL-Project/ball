// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.C,v 1.1.2.5 2005/03/24 16:17:34 amoll Exp $
//
// Molecular Mechanics: MMFF94 force field parameters 
//

#include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

namespace BALL 
{

	MMFF94AtomTypeData::MMFF94AtomTypeData()
		: aspec(0),
			crd(0),
			val(0),
			pilp(0),
			mltb(0),
			arom(0),
			lin(0),
			sbmb(0)
	{
	}

	MMFF94AtomTypesContainer::MMFF94AtomTypesContainer()
		: is_initialized_(false)
	{
	}

	MMFF94AtomTypesContainer::~MMFF94AtomTypesContainer()
	{
		data_.clear();
	}


	MMFF94AtomTypesContainer::MMFF94AtomTypesContainer(const MMFF94AtomTypesContainer& to_copy)
	{
		data_ = to_copy.data_;
	}

	bool MMFF94AtomTypesContainer::readDataSet(const String& filename)
	{
		data_.clear();

		LineBasedFile infile(filename);
		vector<String> fields;

		data_.resize(99);

		try
		{
			while (infile.readLine())
			{
				// comments
				if (infile.getLine().hasPrefix("*") || infile.getLine().hasPrefix("$")) 
				{
					continue;
				}
				
				if (infile.getLine().split(fields) != 9)
				{
					Log.error() << "Error in " << filename << " Not 9 fields in one line " << infile.getLine() << std::endl;
					return false;
				}

				Position pos 			= fields[0].toUnsignedInt();

				// shouldnt happen, but just to be sure
				if (pos > data_.size()) data_.resize(pos);

				data_[pos].aspec 	= fields[1].toUnsignedInt();
				data_[pos].crd 		= fields[2].toUnsignedInt();
				data_[pos].val 		= fields[3].toUnsignedInt();
				data_[pos].pilp 	= fields[4].toUnsignedInt();
				data_[pos].mltb 	= fields[5].toUnsignedInt();
				data_[pos].arom 	= fields[6].toUnsignedInt();
				data_[pos].lin 		= fields[7].toUnsignedInt();
				data_[pos].sbmb 	= fields[8].toUnsignedInt();
			}
		}
		catch(...)
		{
			Log.error() << "Error while parsing line " << infile.readLine() << std::endl;
			Log.error() << " in File " << filename << std::endl;
			infile.close();
			return false;
		}

		infile.close();

		is_initialized_ = true;
		return true;
	}


	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	MMFF94BondStretchParameters::MMFF94BondStretchParameters()
		: is_initialized_(false),
			nr_of_atom_types_(100)
	{
	}

	MMFF94BondStretchParameters::~MMFF94BondStretchParameters()
	{
		clear();
	}
	
	void MMFF94BondStretchParameters::clear()
		throw()
	{
		parameters_.clear();
	}
	
	const MMFF94BondStretchParameters& MMFF94BondStretchParameters::operator = (const MMFF94BondStretchParameters& param)
		throw()
	{
		parameters_ = param.parameters_;
		return *this;
	}

	void MMFF94BondStretchParameters::getOptionalSBMBParameters(const Bond& bond, float& kb, float& r0) const
	{
		StretchMap::ConstIterator it = 
		      parameters_optional_sbmb_.find(getIndex_(bond.getFirstAtom()->getType(),
																									 bond.getSecondAtom()->getType()));
		kb = it->second.first;
		r0 = it->second.second;
	}

	bool MMFF94BondStretchParameters::getParameters(const Bond& bond, float& kb, float& r0) const
	{
		// take the standard value
		StretchMap::ConstIterator it = parameters_.find(getIndex_(bond.getFirstAtom()->getType(),
																							 							  bond.getSecondAtom()->getType()));
		if (it == parameters_.end()) return false;

		kb = it->second.first;
		r0 = it->second.second;

		return true;
	}

	bool MMFF94BondStretchParameters::readParameters(const String& filename)
		throw(Exception::FileNotFound)
	{
		parameters_.clear();
		parameters_optional_sbmb_.clear();

		LineBasedFile infile(filename);
		vector<String> fields;

		try
		{
			while (infile.readLine())
			{
				// comments
				if (infile.getLine().hasPrefix("*") || infile.getLine().hasPrefix("$")) 
				{
					continue;
				}
				
				if (infile.getLine().split(fields) != 6)
				{
					Log.error() << "Error in " << filename << " Not 6 fields in one line " << infile.getLine() << std::endl;
					return false;
				}

				const Position type1 = fields[1].toUnsignedInt();
				const Position type2 = fields[2].toUnsignedInt();
				const Position index = getIndex_(type1, type2);

				// the data values r0 and kb
				pair<float, float> values(fields[3].toFloat(), fields[4].toFloat());

				// if the value already exists, put it into the optional sbmb hashmap
				if (parameters_.has(index))
				{
					if (fields[0] != "1")
					{
						Log.error() << "Problem while reading bond parameter file: Invalid SBMB entry: " 
												<< infile.getLine() << std::endl;
						return false;
					}

					parameters_optional_sbmb_[index] = values;
					continue;
				}

				// put the values into the standard values hashmap
				parameters_[index] = values;
			}
		}
		catch(...)
		{
			Log.error() << "Error while parsing line " << infile.readLine() << std::endl;
			Log.error() << " in File " << filename << std::endl;
			infile.close();
			return false;
		}

		infile.close();

		is_initialized_ = true;
		return true;
	}

	Position MMFF94BondStretchParameters::getIndex_(Position atom_type1, Position atom_type2) const
	{ 
		// make sure atom_type1 is smaller or equal atom_type2
		// because the parameters are not mirrored
		if (atom_type2 < atom_type1)
		{
			Position temp(atom_type1);
			atom_type1 = atom_type2;
			atom_type2 = temp;
		}

		return atom_type1 * nr_of_atom_types_ + atom_type2;
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	MMFF94BendParameters::MMFF94BendParameters()
		: is_initialized_(false),
			nr_of_atom_types_(100)
	{
	}

	MMFF94BendParameters::~MMFF94BendParameters()
	{
		clear();
	}
	
	void MMFF94BendParameters::clear()
		throw()
	{
		parameters_.clear();
	}
	
	const MMFF94BendParameters& MMFF94BendParameters::operator = (const MMFF94BendParameters& param)
		throw()
	{
		parameters_ = param.parameters_;
		return *this;
	}

	bool MMFF94BendParameters::getParameters(Position atom_type1, Position atom_type2, Position atom_type3, float& ka, float& angle) const
	{
		// take the standard value
		BendMap::ConstIterator it = parameters_.find(getIndex_(atom_type1, atom_type2, atom_type3));
		if (it == parameters_.end()) return false;

		ka = it->second[1];
		angle = it->second[2];

		return true;
	}

	bool MMFF94BendParameters::readParameters(const String& filename)
		throw(Exception::FileNotFound)
	{
		parameters_.clear();

		LineBasedFile infile(filename);
		vector<String> fields;

		try
		{
			while (infile.readLine())
			{
				// comments
				if (infile.getLine().hasPrefix("*") || infile.getLine().hasPrefix("$")) 
				{
					continue;
				}
				
				if (infile.getLine().split(fields) < 7)
				{
					Log.error() << "Error in " << filename << " Not 6 fields in one line " << infile.getLine() << std::endl;
					return false;
				}

				const Position type1 = fields[1].toUnsignedInt();
				const Position type2 = fields[2].toUnsignedInt();
				const Position type3 = fields[3].toUnsignedInt();
				const Position index = getIndex_(type1, type2, type3);

				parameters_[index] = vector<float>();
				parameters_[index].resize(3);
				parameters_[index][0] = fields[0].toUnsignedInt(); // bond type
				parameters_[index][1] = fields[4].toFloat(); // ka
				parameters_[index][2] = fields[5].toFloat(); // theta
			}
		}
		catch(...)
		{
			Log.error() << "Error while parsing line " << infile.readLine() << std::endl;
			Log.error() << " in File " << filename << std::endl;
			infile.close();
			return false;
		}

		infile.close();

		is_initialized_ = true;
		return true;
	}

	Position MMFF94BendParameters::getIndex_(Position atom_type1, Position atom_type2, Position atom_type3) const
	{ 
		vector<Position> types;
		
		types.push_back(atom_type1);
		types.push_back(atom_type2);
		types.push_back(atom_type3);

		sort(types.begin(), types.end());

		return types[0] * nr_of_atom_types_ * nr_of_atom_types_ + types[1] * nr_of_atom_types_ + types[2];
	}


} // namespace BALL
