// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.C,v 1.1.2.9 2005/03/27 14:12:35 amoll Exp $
//
// Molecular Mechanics: MMFF94 force field parameters 
//

#include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

namespace BALL 
{

	Size MMFF94_number_atom_types = 100;

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

	bool MMFF94AtomTypesContainer::readParameters(const String& filename)
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
				
				if (infile.getLine().split(fields) < 9)
				{
					Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
										  << filename << " Not 9 fields in one line " 
											<< infile.getLine() << std::endl;
					return false;
				}

				Position pos 			= fields[0].toUnsignedInt();

				// shouldnt happen, but just to be sure
				if (pos >= data_.size()) data_.resize(pos + 10);

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


		is_initialized_ = true;
		return true;
	}


	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	
	MMFF94BondStretchParameters::BondData::BondData()
		: kb_normal(0),
			r0_normal(0),
			standard_bond_exists(0),
			kb_sbmb(0),
			r0_sbmb(0),
			sbmb_exists(0)
	{
	}

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
	
	const MMFF94BondStretchParameters& MMFF94BondStretchParameters::operator = 
		(const MMFF94BondStretchParameters& param)
		throw()
	{
		parameters_ = param.parameters_;
		return *this;
	}


	MMFF94BondStretchParameters::StretchMap::ConstIterator 
		MMFF94BondStretchParameters::getParameters(const Bond& bond) const
	{
		// take the standard value
		StretchMap::ConstIterator it = parameters_.find(getIndex_(bond.getFirstAtom()->getType(),
																							 							  bond.getSecondAtom()->getType()));
		return it;
	}

	bool MMFF94BondStretchParameters::readParameters(const String& filename)
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
				
				if (infile.getLine().split(fields) < 6)
				{
					Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
										  << filename << " Not 6 fields in one line " 
											<< infile.getLine() << std::endl;
					return false;
				}

				const Position type1 = fields[1].toUnsignedInt();
				const Position type2 = fields[2].toUnsignedInt();
				const Position index = getIndex_(type1, type2);

				StretchMap::Iterator it = parameters_.find(index);

				if (it == parameters_.end())
				{
					it = parameters_.insert(pair<Position, BondData>(index, BondData())).first;
				}

				BondData& data = it->second;

				bool is_sbmb = fields[0].toBool();

				if (is_sbmb)
				{
					data.kb_sbmb = fields[3].toFloat(); 
					data.r0_sbmb = fields[4].toFloat();
					data.sbmb_exists = true;
				}
				else
				{
					data.kb_normal = fields[3].toFloat(); 
					data.r0_normal = fields[4].toFloat();
					data.standard_bond_exists = true;
				}
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

		return atom_type1 * MMFF94_number_atom_types + atom_type2;
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	MMFF94BendParameters::MMFF94BendParameters()
		: is_initialized_(false)
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

	bool MMFF94BendParameters::getParameters(Position bond_type,
			Position atom_type1, Position atom_type2, Position atom_type3, float& ka, float& angle) const
	{
		// take the standard value
		BendMap::ConstIterator it = parameters_.find(
				getIndex_(bond_type, atom_type1, atom_type2, atom_type3));

		if (it == parameters_.end()) return false;

		ka = it->second.first;
		angle = it->second.second;

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
					Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
										  << filename << " Not 6 fields in one line " 
											<< infile.getLine() << std::endl;
					return false;
				}

				const Position bend_type = fields[0].toUnsignedInt();
				const Position atom_type1 = fields[1].toUnsignedInt();
				const Position atom_type2 = fields[2].toUnsignedInt();
				const Position atom_type3 = fields[3].toUnsignedInt();
				const Position index = getIndex_(bend_type, atom_type1, atom_type2, atom_type3);

				parameters_[index] = pair<float, float>();
				parameters_[index].first  = fields[4].toFloat(); // ka
				parameters_[index].second = fields[5].toFloat(); // theta
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

	Position MMFF94BendParameters::getIndex_(Position bend_type,
						Position atom_type1, Position atom_type2, Position atom_type3) const
	{ 
		// Atom type I is always less than or equal to K
		if (atom_type1 > atom_type3)
		{
			Position temp(atom_type1);
			atom_type1 = atom_type3;
			atom_type3 = temp;
		}

		// The canonical-order index, CXA, is computed as:
		// CXA = MC*(J*MA**2 + I*MA + K) + ATIJK
		// where MA is again the maximum permissible atom type +1, and MC is at least 
		// one greater than the maximum permissible angle-type index.
		return 10 * (atom_type2 * MMFF94_number_atom_types * MMFF94_number_atom_types +
								 atom_type1 * MMFF94_number_atom_types +
								 atom_type3) +
					 bend_type;
	}


} // namespace BALL
