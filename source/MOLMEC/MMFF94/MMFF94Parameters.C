// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.C,v 1.1.2.1 2005/03/21 16:42:54 amoll Exp $
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
		: is_initialized_(false)
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

	bool MMFF94BondStretchParameters::getParameters(Position atom_type1, Position atom_type2, float& kb, float& r0) const
	{
		if (atom_type2 < atom_type1)
		{
			Position temp(atom_type1);
			atom_type1 = atom_type2;
			atom_type2 = temp;
		}

		StretchMap::ConstIterator it = parameters_.find(atom_type1);
		if (it == parameters_.end()) return false;	

		HashMap<Position, pair<float, float> >::ConstIterator it2 = it->second.find(atom_type2);
		if (it2 == it->second.end()) return false;

		kb = it2->second.first;
		r0 = it2->second.second;

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

				parameters_[fields[1].toUnsignedInt()]
									 [fields[2].toUnsignedInt()] = pair<float, float>(fields[3].toFloat(), 
																																		fields[4].toFloat());
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
