// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.C,v 1.1.2.2 2005/03/22 15:41:11 amoll Exp $
//
// Molecular Mechanics: MMFF94 force field parameters 
//

#include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

namespace BALL 
{
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

	bool MMFF94BondStretchParameters::getParameters(const Bond& bond, float& kb, float& r0) const
	{
		const Atom& atom1 = *bond.getFirstAtom();
		const Atom& atom2 = *bond.getSecondAtom();
		Position atom_type1(atom1.getType());
		Position atom_type2(atom2.getType());

		// make sure atom_type1 is smaller or equal atom_type2
		// because the parameters are not mirrored
		if (atom_type2 < atom_type1)
		{
			Position temp(atom_type1);
			atom_type1 = atom_type2;
			atom_type2 = temp;
		}

		const Position index(getIndex_(atom_type1, atom_type2));

		// take the sbmb value if :

		// is there an optional sbmb value ?
		// is the bond order == 1 ?
		// are both atoms sp or sp2 hypridised?
		StretchMap::ConstIterator it = parameters_optional_sbmb_.find(index);

		const bool get_sbmb = 
			it != parameters_optional_sbmb_.end() 	&&
			bond.getOrder() == 1   									&&
			(isSp_(atom1) || isSp2_(atom1)) 				&&
			(isSp_(atom2) || isSp2_(atom2));

		if (get_sbmb)
		{
			kb = it->second.first;
			r0 = it->second.second;
			return true;
		}

		// take the standard value

		it = parameters_.find(index);
		if (it == parameters_.end()) return false;

		kb = it->second.first;
		r0 = it->second.second;

		return true;
	}

	bool MMFF94BondStretchParameters::readParameters(const String& filename)
		throw(Exception::FileNotFound)
	{
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
		
} // namespace BALL
