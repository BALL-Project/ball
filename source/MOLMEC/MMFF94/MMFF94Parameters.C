// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.C,v 1.1.2.15 2006/01/30 16:11:46 amoll Exp $
//
// Molecular Mechanics: MMFF94 force field parameters 
//

#include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>

#define BALL_DEBUG_MMFF
using namespace std;

namespace BALL 
{

	Size MMFF94_number_atom_types = 100;


	MMFF94AtomTypeEquivalences::MMFF94AtomTypeEquivalences()
		: is_initialized_(false)
	{
	}

	MMFF94AtomTypeEquivalences::~MMFF94AtomTypeEquivalences()
	{
		equivalences_.clear();
		names_.clear();
		exists_.clear();
	}


	MMFF94AtomTypeEquivalences::MMFF94AtomTypeEquivalences(const MMFF94AtomTypeEquivalences& to_copy)
	{
		equivalences_ = to_copy.equivalences_;
		names_ = to_copy.names_;
		exists_ = to_copy.exists_;
	}

	bool MMFF94AtomTypeEquivalences::readParameters(const String& filename)
	{
		equivalences_.clear();
		names_.clear();
		exists_.clear();

		LineBasedFile infile(filename);
		vector<String> fields;

		equivalences_.resize(MMFF94_number_atom_types);
		names_.resize(MMFF94_number_atom_types);
		exists_.resize(MMFF94_number_atom_types);

		for (Position pos = 0; pos < MMFF94_number_atom_types; pos++)
		{
			exists_[pos] = false;
		}

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
										  << filename << " Not 7 fields in one line " 
											<< infile.getLine() << std::endl;
					return false;
				}

				Position primary = fields[1].toUnsignedInt();
				
				// shouldnt happen, but just to be sure
				if (primary >= exists_.size())
				{
					names_.resize(primary + 10);
					exists_.resize(primary + 10);
					equivalences_.resize(primary + 10);
				}

				// ok, get the data
				names_[primary] = fields[0];
				equivalences_[primary].resize(4);

				equivalences_[primary][0] = fields[2].toUnsignedInt();
				equivalences_[primary][1] = fields[3].toUnsignedInt();
				equivalences_[primary][2] = fields[4].toUnsignedInt();
				equivalences_[primary][3] = fields[5].toUnsignedInt();

				exists_[primary] = true;
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


	Index MMFF94AtomTypeEquivalences::getEquivalence(Position original, Position number) const
	{
		if (original >= equivalences_.size() || !exists_[original] ||
				number < 1 || number > 4) 
		{
			return -1;
		}

		return equivalences_[original][number - 1];
	}


	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	

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

		data_.resize(MMFF94_number_atom_types);

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
	
	MMFF94StretchParameters::BondData::BondData()
		: kb_normal(0),
			r0_normal(0),
			standard_bond_exists(0),
			kb_sbmb(0),
			r0_sbmb(0),
			sbmb_exists(0)
	{
	}

	MMFF94StretchParameters::MMFF94StretchParameters()
		: is_initialized_(false)
	{
	}

	MMFF94StretchParameters::~MMFF94StretchParameters()
	{
		clear();
	}
	
	void MMFF94StretchParameters::clear()
		throw()
	{
		parameters_.clear();
	}
	
	const MMFF94StretchParameters& MMFF94StretchParameters::operator = 
		(const MMFF94StretchParameters& param)
		throw()
	{
		parameters_ = param.parameters_;
		return *this;
	}


	MMFF94StretchParameters::StretchMap::ConstIterator 
		MMFF94StretchParameters::getParameters(const Bond& bond) const
	{
		// take the standard value
		StretchMap::ConstIterator it = parameters_.find(getIndex_(bond.getFirstAtom()->getType(),
																							 							  bond.getSecondAtom()->getType()));
		return it;
	}

	bool MMFF94StretchParameters::readParameters(const String& filename)
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

	Position MMFF94StretchParameters::getIndex_(Position atom_type1, Position atom_type2) const
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

	MMFF94StretchParameters::StretchMap::ConstIterator 
	  MMFF94StretchParameters::calculateValues(const Bond& bond)
	{
		const Atom& atom1 = *bond.getFirstAtom();
		const Atom& atom2 = *bond.getSecondAtom();

		const Position e1 = atom1.getElement().getAtomicNumber();
		const Position e2 = atom2.getElement().getAtomicNumber();

		// only atoms up to Xenon
		if (e1 > 54 || e2 > 54 ||
				e1 == 0 || e2 || 0) 
		{
			return parameters_.end();
		}

		// Calculate Hybridization
		Position h1 = 0;
		Position h2 = 0;

		if      (sp1_.operator() (atom1)) h1 = 1;
		else if (sp2_.operator() (atom1)) h1 = 2;
		else if (sp3_.operator() (atom1)) h1 = 3;

		if      (sp1_.operator() (atom2)) h2 = 1;
		else if (sp2_.operator() (atom2)) h2 = 2;
		else if (sp3_.operator() (atom2)) h2 = 3;


		// only SP hypridized atoms: 
		if (h1 == 0 || h2 == 0)
		{
			return parameters_.end();
		}

		// only singe, double, tripple, quadruple  and aromatic bonds
		Bond::BondOrder bo = (Bond::BondOrder) bond.getOrder();
		if (bo == Bond::ORDER__UNKNOWN || 
				bo == Bond::ORDER__ANY)
		{
			return parameters_.end();
		}

		float r1 = radii_[e1];
		float r2 = radii_[e2];

		// only for stored radii
		if (r1 == 0.0 ||
				r2 == 0.0)
		{
			return parameters_.end();
		}

		// calculate corrected radii
		// no correction for H atoms
		if (e1 != 1 && e2 != 1)
		{
			if (bo == Bond::ORDER__AROMATIC)
			{
				r1 -= 0.035;
				r2 -= 0.035;
			}
			else if (bo == Bond::ORDER__QUADRUPLE)
			{
				r1 -= 0.070;
				r2 -= 0.070;
			}
			else if (bo == Bond::ORDER__TRIPLE)
			{
				r1 -= 0.17;
				r2 -= 0.17;
			}
			else if (bo == Bond::ORDER__DOUBLE)
			{
				r1 -= 0.1;
				r2 -= 0.1;
			}
			else 
			{
				if (h1 == 1) r1 -= 0.08;
				if (h2 == 1) r2 -= 0.08;

				if (h1 == 2) r1 -= 0.035;
				if (h2 == 2) r2 -= 0.035;
			}
		}

		// calculate shrink factor
		float d = 0.008; // SHRINK FACTOR

		// for hyrogen atoms no shrinking
		if (e1 == 1 || e2 == 1) d = 0.0;

		// for atoms > neon no shrinking
		if (e1 > 10 || e2 > 10) d = 0.0;

		// calculate SENS c
		float c = 0.08;

		// for hyrogen atoms
		if (e1 == 1 || e2 == 1) c = 0.05;

		// POWER
		const float n = 1.4;

		// FORMULA from CHARMM docu:
		float r0 = radii_[e1] + radii_[e2] - c * pow(fabs(electronegatives_[e1] - electronegatives_[e2]), n) - d;
		
		if (r0 > 0.0)
		{
			// do something!
			return parameters_.end();
		}

		// requisite data is not available, use relationship developed by Badger
		// parameters are those described in: D. L. Herschbach and V. W. Laurie, J. Chem.  Phys. 1961, 35, 458-463.

		// to change:
return parameters_.end();
	}

	float MMFF94StretchParameters::radii_[] =
	{
     0.33, 0.0,
     1.34, 0.90, 0.81, 0.77, 0.73, 0.72, 0.74, 0.0,
     1.54, 1.30, 1.22, 1.15, 1.09, 1.03, 1.01, 0.0,
     1.96, 1.74,
     1.44, 1.36, 0.00, 0.00, 0.00,
     0.00, 0.00, 0.00, 1.38, 1.31,
     1.19, 1.20, 1.20, 1.16, 1.15, 0.0,
     2.11, 1.92,
     1.62, 1.48, 0.00, 0.00, 0.00,
     0.00, 0.00, 0.00, 1.53, 1.48,
     1.46, 1.40, 1.41, 1.35, 1.33, 0.0
	};

	float MMFF94StretchParameters::electronegatives_[] =
	{
     2.20, 0.0,
     0.97, 1.47, 2.01, 2.5, 3.07, 3.5, 4.10, 0.0,
     1.01, 1.23, 1.47, 1.74, 2.06, 2.44, 2.83, 0.0,
     0.91, 1.04,
     1.3, 1.5, 1.6, 1.6, 1.5,
     1.8, 1.8, 1.8, 1.9, 1.6,
     1.82, 2.02, 2.20, 2.48, 2.74, 0.0,
     0.89, 0.99,
     1.3, 1.4, 1.6, 1.8, 1.9,
     2.2, 2.2, 2.2, 1.9, 1.7,
     1.49, 1.72, 1.82, 2.01, 2.21, 0.0
	};
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

		if (it == parameters_.end())
		{
#ifdef BALL_DEBUG_MMFF
			Log.info() << "MMFF94 Bend "  << bond_type 
								 << " " << atom_type1 << " " << atom_type2 << " " << atom_type3 
								 << "  ->  ???" << std::endl;
#endif

			return false;
		}


		ka = it->second.first;
		angle = it->second.second;

#ifdef BALL_DEBUG_MMFF
		Log.info() << "MMFF94 Bend "  << bond_type 
					     << " " << atom_type1 << " " << atom_type2 << " " << atom_type3 
							 << "  -> " << ka << "  " << angle << std::endl;
#endif

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


	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	MMFF94StretchBendParameters::MMFF94StretchBendParameters()
		: is_initialized_(false)
	{
	}

	MMFF94StretchBendParameters::~MMFF94StretchBendParameters()
	{
		clear();
	}
	
	void MMFF94StretchBendParameters::clear()
		throw()
	{
		parameters_.clear();
	}
	
	const MMFF94StretchBendParameters& MMFF94StretchBendParameters::operator 
		= (const MMFF94StretchBendParameters& param)
		throw()
	{
		parameters_ = param.parameters_;
		return *this;
	}

	bool MMFF94StretchBendParameters::getParameters(Position bend_type,
			const Atom& atom1, const Atom& atom2, const Atom& atom3, 
			float& kb_ijk, float& kb_kji) const
	{
		// take the standard value
		StretchBendMap::ConstIterator it = parameters_.find(
				getIndex_(bend_type, atom1.getType(), atom2.getType(), atom3.getType()));

		if (it == parameters_.end())
		{
			Position r1 = atom1.getElement().getPeriod() - 1;
			Position r2 = atom2.getElement().getPeriod() - 1;
			Position r3 = atom3.getElement().getPeriod() - 1;

			it = parameters_.find(getIndexByRow_(r1, r2, r3));

			if (it == parameters_by_row_.end())
			{
				return false;
			}
#ifdef BALL_DEBUG_MMFF
Log.info() << "MMFF94 StretchBend: from row: " << atom1.getName() << " " << atom2.getName() << " " << atom3.getName() << " " <<
																									atom1.getType() << " " << atom2.getType() << " " << atom3.getType() << 
																									"    b: " << bend_type << std::endl;
#endif
		}

		kb_ijk = it->second.first;
		kb_kji = it->second.second;

		return true;
	}

	bool MMFF94StretchBendParameters::readParameters(const String& filename, const String& by_row_filename)
		throw(Exception::FileNotFound)
	{
		parameters_.clear();
 		parameters_by_row_.clear();

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

				const Position sb = fields[0].toUnsignedInt();
				const Position atom_type1 = fields[1].toUnsignedInt();
				const Position atom_type2 = fields[2].toUnsignedInt();
				const Position atom_type3 = fields[3].toUnsignedInt();
				const Position index = getIndex_(sb, atom_type1, atom_type2, atom_type3);

				parameters_[index] = pair<float, float>();
				parameters_[index].first  = fields[4].toFloat(); // kba_ijk
				parameters_[index].second = fields[5].toFloat(); // kba_kji
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

		infile = LineBasedFile(by_row_filename);

		try
		{
			while (infile.readLine())
			{
				// comments
				if (infile.getLine().hasPrefix("*") || infile.getLine().hasPrefix("$")) 
				{
					continue;
				}
				
				if (infile.getLine().split(fields) < 5)
				{
					Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
										  << filename << " Not 5 fields in one line " 
											<< infile.getLine() << std::endl;
					return false;
				}

				const Position ir = fields[0].toUnsignedInt();
				const Position jr = fields[1].toUnsignedInt();
				const Position kr = fields[2].toUnsignedInt();
				const float    f_ijk = fields[3].toFloat();
				const float    f_kji = fields[4].toFloat();
				const Position index = getIndexByRow_(ir, jr, kr);

				parameters_[index] = pair<float, float>();
				parameters_[index].first  = f_ijk;
				parameters_[index].second = f_kji;
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

	Position MMFF94StretchBendParameters::getIndex_(Position stretch_bend_type,
						Position atom_type1, Position atom_type2, Position atom_type3) const
	{ 
		// Atom type I is always less than or equal to K
		if (atom_type1 > atom_type3)
		{
			Position temp(atom_type1);
			atom_type1 = atom_type3;
			atom_type3 = temp;
		}

		// The canonical-order index, CXBA
		// CXBA = MC*(J*MA**2 + I*MA + K) + ATIJK
		// where MA is again the maximum permissible atom type +1, and MC is at least 
		// one greater than the maximum permissible -type index.
		return 13 * (atom_type2 * MMFF94_number_atom_types * MMFF94_number_atom_types +
								 atom_type1 * MMFF94_number_atom_types +
								 atom_type3) +
					 stretch_bend_type;
	}


	Position MMFF94StretchBendParameters::getIndexByRow_(Position r1, Position r2, Position r3) const
	{
		// row 1 is always less or equal row 3
		if (r1 > r3)
		{
			Position temp(r1);
			r1 = r3;
			r3 = temp;
		}

		return r1 * 10 * 10 + 
					 r2 * 10      +
					 r3;
	}

} // namespace BALL
