// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.C,v 1.1.8.1 2007/03/23 12:51:54 oliver Exp $
//
// Molecular Mechanics: MMFF94 force field parameters 
//

#include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/FORMAT/parameters.h>

// #define BALL_DEBUG_MMFF
using namespace std;

namespace BALL 
{

	Size MMFF94_number_atom_types = 100;

	MMFF94ParametersBase::MMFF94ParametersBase()
		: is_initialized_(false),
			number_expected_fields_(0),
			equiv_(0)
	{
	}

	void MMFF94ParametersBase::clear()
		
	{
		is_initialized_ = false;
	}

	bool MMFF94ParametersBase::readParameters(Parameters& p, const String& section)
		throw(Exception::FileNotFound)
	{
		ParameterSection p_sec;
		p_sec.extractSection(p, section);

		vector<vector<String> > lines;

		for(Position p = 0; p < p_sec.getNumberOfKeys(); ++p)
		{
			vector<String> fields;
			String key = p_sec.getKey(p);

			key.split(fields);

			if (fields.size() + p_sec.getNumberOfVariables() != number_expected_fields_)
			{
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : "
										<< section << " Not enough fields in one line "
										<< std::endl;
				return false;
			}

			size_t num_keys = fields.size();
			fields.resize(number_expected_fields_);

			for(size_t i = num_keys; i < number_expected_fields_; ++i) {
				fields[i] = p_sec.getValue(p, i - num_keys);
			}

			lines.push_back(fields);
		}

		if (!setup_(lines))
		{
			Log.error() << "Error while parsing line in section " << section << std::endl;
			return false;
		}

		is_initialized_ = true;
		return true;
	}

/////////////////////////////////////////////////////////////////////////////

MMFF94AtomTypeEquivalences::MMFF94AtomTypeEquivalences()
	: MMFF94ParametersBase()
{
	number_expected_fields_ = 7;
}

MMFF94AtomTypeEquivalences::~MMFF94AtomTypeEquivalences()
{
	clear();
}


MMFF94AtomTypeEquivalences::MMFF94AtomTypeEquivalences(const MMFF94AtomTypeEquivalences& to_copy)
	: MMFF94ParametersBase()
{
	is_initialized_ = to_copy.is_initialized_;
	equivalences_ = to_copy.equivalences_;
	names_ = to_copy.names_;
	exists_ = to_copy.exists_;
}

void MMFF94AtomTypeEquivalences::clear()
	
{
	is_initialized_ = false;
	equivalences_.clear();
	names_.clear();
	exists_.clear();
}

bool MMFF94AtomTypeEquivalences::setup_(const vector<vector<String> >& lines)
{
	equivalences_.resize(MMFF94_number_atom_types);
	names_.resize(MMFF94_number_atom_types);
	exists_.resize(MMFF94_number_atom_types);

	for (Position pos = 0; pos < MMFF94_number_atom_types; pos++)
	{
		exists_[pos] = false;
	}

	try
	{
		for (Position p = 0; p < lines.size(); p++)
		{
			const vector<String>& fields = lines[p];

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
		return false;
	}

	return true;
}


Index MMFF94AtomTypeEquivalences::getEquivalence(Position original, Position number) const
{
	if (number == 0) return original;

	if (original >= equivalences_.size() || !exists_[original] ||
			number < 1 || number > 4) 
	{
		return -1;
	}

	return equivalences_[original][number - 1];
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////


MMFF94AtomType::MMFF94AtomType()
	: aspec(0),
		crd(0),
		val(0),
		pilp(0),
		mltb(0),
		arom(0),
		lin(0),
		sbmb(0),
		valid(false)
{
}

MMFF94AtomTypes::MMFF94AtomTypes()
	: MMFF94ParametersBase()
{
	number_expected_fields_ = 9;
}

MMFF94AtomTypes::MMFF94AtomTypes(const MMFF94AtomTypes& to_copy)
	: MMFF94ParametersBase()
{
	data_ = to_copy.data_;
}

bool MMFF94AtomTypes::setup_(const vector<vector<String> >& lines)
{
	data_.resize(MMFF94_number_atom_types);

	try
	{
		for (Position p = 0; p < lines.size(); p++)
		{
			const vector<String>& fields = lines[p];

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
			data_[pos].valid  = true;
		}
	}
	catch(...)
	{
		return false;
	}

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
		sbmb_exists(0),
		empirical(0)
{
}

MMFF94StretchParameters::MMFF94StretchParameters()
	: MMFF94ParametersBase()
{
	number_expected_fields_ = 6;
}

MMFF94StretchParameters::~MMFF94StretchParameters()
{
}

void MMFF94StretchParameters::clear()
	
{
	parameters_.clear();
}

const MMFF94StretchParameters& MMFF94StretchParameters::operator = 
	(const MMFF94StretchParameters& param)
	
{
	parameters_ = param.parameters_;
	return *this;
}


bool MMFF94StretchParameters::assignParameters(Position type1, Position type2, MMFF94StretchParameters::BondData& data) const
{
	// take the standard value
	MMFF94StretchParameters::StretchMap::ConstIterator it = parameters_.find(getMMFF94Index(type1, type2));
	if (it != parameters_.end())
	{
		data = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

bool MMFF94StretchParameters::setup_(const vector<vector<String> >& lines)	
{
	parameters_.clear();

	try
	{
		for (Position p = 0; p < lines.size(); p++)
		{
			const vector<String>& fields = lines[p];

			const Position type1 = fields[1].toUnsignedInt();
			const Position type2 = fields[2].toUnsignedInt();
			const Position index = getMMFF94Index(type1, type2);

			StretchMap::Iterator it = parameters_.find(index);

			if (it == parameters_.end())
			{
				it = parameters_.insert(pair<Position, BondData>(index, BondData())).first;
			}

			BondData& data = it->second;

			bool is_sbmb = fields[0].toBool();

			if (is_sbmb)
			{
				data.kb_sbmb = fields[3].toDouble(); 
				data.r0_sbmb = fields[4].toDouble();
				data.sbmb_exists = true;
			}
			else
			{
				data.kb_normal = fields[3].toDouble(); 
				data.r0_normal = fields[4].toDouble();
				data.standard_bond_exists = true;
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

Position getMMFF94Index(Position atom_type1, Position atom_type2)
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

bool MMFF94StretchParameters::readEmpiricalParameters(Parameters& p, const String& section)
{
	empirical_parameters_.clear();

	ParameterSection p_sec;
	p_sec.extractSection(p, section);
	vector<String> fields;

	try
	{
		for(Position i = 0; i < p_sec.getNumberOfKeys(); ++i)
		{
			String key = p_sec.getKey(i);
			key.split(fields);

			if (fields.size() + p_sec.getNumberOfVariables() != 5)
			{
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : "
										<< section << " Not 5 fields in one line"
										<< std::endl;
				return false;
			}

			const Position type1 = fields[0].toUnsignedInt();
			const Position type2 = fields[1].toUnsignedInt();
			const Position index = getMMFF94Index(type1, type2);

			EmpiricalStretchMap::Iterator it = empirical_parameters_.find(index);

			if (it == empirical_parameters_.end())
			{
				it = empirical_parameters_.insert(pair<Position, EmpiricalBondData>(index, EmpiricalBondData())).first;
			}

			EmpiricalBondData& data = it->second;

			data.r0 = p_sec.getValue(i, 0).toDouble();
			data.kb = p_sec.getValue(i, 1).toDouble();
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}


// see http://www.ccl.net/cca/data/MMFF94/
double MMFF94StretchParameters::radii[] =
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

double MMFF94StretchParameters::electronegatives[] =
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
	: MMFF94ParametersBase()
{
	number_expected_fields_ = 7;
}

MMFF94BendParameters::~MMFF94BendParameters()
{
}

void MMFF94BendParameters::clear()
	
{
	parameters_.clear();
}

const MMFF94BendParameters& MMFF94BendParameters::operator = (const MMFF94BendParameters& param)
	
{
	parameters_ = param.parameters_;
	return *this;
}

bool MMFF94BendParameters::assignParameters(Position bond_type,
		Position atom_type1, Position atom_type2, Position atom_type3, double& ka, double& angle) const
{
	BendMap::ConstIterator it;

	Position index = getIndex_(bond_type, atom_type1, atom_type2, atom_type3);

	it = parameters_.find(index);
	if (it != parameters_.end())
	{
		ka = it->second.first;
		angle = it->second.second;
		return true;
	}

	// check for parameters in a step down procedure
	for (Position p = 0; p < 5; p++)
	{
		it = parameters_.find(getIndex_(bond_type, 
																		equiv_->getEquivalence(atom_type1, p),
																		atom_type2, 
																		equiv_->getEquivalence(atom_type3, p)));

		if (it != parameters_.end())
		{
			ka = it->second.first;
			angle = it->second.second;
			buffered_parameters_[index] = it->second;
			return true;
		}
	}

	return false;
}

bool MMFF94BendParameters::setup_(const vector<vector<String> >& lines)	
{
	parameters_.clear();

	try
	{
		for (Position p = 0; p < lines.size(); p++)
		{
			const vector<String>& fields = lines[p];

			const Position bend_type = fields[0].toUnsignedInt();
			const Position atom_type1 = fields[1].toUnsignedInt();
			const Position atom_type2 = fields[2].toUnsignedInt();
			const Position atom_type3 = fields[3].toUnsignedInt();
			const Position index = getIndex_(bend_type, atom_type1, atom_type2, atom_type3);

			parameters_[index] = pair<double, double>();
			parameters_[index].first  = fields[4].toDouble(); // ka
			parameters_[index].second = fields[5].toDouble(); // theta
		}
	}
	catch(...)
	{
		return false;
	}

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
	: MMFF94ParametersBase()
{
	number_expected_fields_ = 7;
}

MMFF94StretchBendParameters::~MMFF94StretchBendParameters()
{
}

void MMFF94StretchBendParameters::clear()
	
{
	parameters_.clear();
}

const MMFF94StretchBendParameters& MMFF94StretchBendParameters::operator = (const MMFF94StretchBendParameters& param)
	
{
	parameters_ = param.parameters_;
	return *this;
}

bool MMFF94StretchBendParameters::assignParameters(Position bend_type,
		const Atom& atom1, const Atom& atom2, const Atom& atom3, 
		double& kb_ijk, double& kb_kji) const
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

		if (it == parameters_by_row_.end()) return false;

		if (r1 <= r3)
		{
			kb_ijk = it->second.first;
			kb_kji = it->second.second;
		}
		else
		{
			kb_ijk = it->second.second;
			kb_kji = it->second.first;
		}

		return true;
	}

	kb_ijk = it->second.first;
	kb_kji = it->second.second;

	return true;
}

bool MMFF94StretchBendParameters::setup_(const vector<vector<String> >& lines)
{
	parameters_.clear();
	parameters_by_row_.clear();

	try
	{
		for (Position p = 0; p < lines.size(); p++)
		{
			const vector<String>& fields = lines[p];

			const Position sb = fields[0].toUnsignedInt();
			const Position atom_type1 = fields[1].toUnsignedInt();
			const Position atom_type2 = fields[2].toUnsignedInt();
			const Position atom_type3 = fields[3].toUnsignedInt();
			const Position index = getIndex_(sb, atom_type1, atom_type2, atom_type3);

			parameters_[index] = pair<double, double>();
			parameters_[index].first  = fields[4].toDouble(); // kba_ijk
			parameters_[index].second = fields[5].toDouble(); // kba_kji
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool MMFF94StretchBendParameters::readEmpiricalParameters(Parameters& p, const String& section)
	throw(Exception::FileNotFound)
{
	ParameterSection p_sec;
	p_sec.extractSection(p, section);

	vector<String> fields;

	try
	{
		for(Position i = 0; i < p_sec.getNumberOfKeys(); ++i)
		{
			String key = p_sec.getKey(i);
			key.split(fields);

			if (fields.size() + p_sec.getNumberOfVariables() < 5)
			{
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
										<< section << " Not 5 fields in one line " 
										<< std::endl;
				return false;
			}

			const Position ir = fields[0].toUnsignedInt();
			const Position jr = fields[1].toUnsignedInt();
			const Position kr = fields[2].toUnsignedInt();
			const double    f_ijk = p_sec.getValue(i, 0).toDouble();
			const double    f_kji = p_sec.getValue(i, 1).toDouble();

			parameters_[getIndexByRow_(ir, jr, kr)] = pair<double, double>(f_ijk, f_kji);
		}
	}
	catch(...)
	{
		return false;
	}

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

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Position MMFF94TorsionParameters::ic_[5] = { 0, 1, 2, 4, 4}; // equivalence values for atom type i
Position MMFF94TorsionParameters::lc_[5] = { 0, 1, 4, 2, 4}; // equivalence values for atom type l

MMFF94TorsionParameters::MMFF94TorsionParameters()
	: MMFF94ParametersBase()
{
	number_expected_fields_ = 9;
}

MMFF94TorsionParameters::~MMFF94TorsionParameters()
{
}

void MMFF94TorsionParameters::clear()
	
{
	parameters_.clear();
}

const MMFF94TorsionParameters& MMFF94TorsionParameters::operator = (const MMFF94TorsionParameters& param)
	
{
	parameters_ = param.parameters_;
	return *this;
}

bool MMFF94TorsionParameters::assignParameters(Position torsion_type,
		Index at1, Index at2, Index at3, Index at4,
		double& v1, double& v2, double& v3) const
{
	const String index = getIndex_(torsion_type, at1, at2, at3, at4);

#ifdef BALL_DEBUG_MMFF
	Log.info() << at1 << " " << at2 << " " << at3 << " " << at4  << std::endl;
#endif

	// first we try a buffer for previous hits to speed up things a bit
	// this will prevent searching for combinations further down the step by step approach
	TorsionsMap::ConstIterator it = buffered_parameters_.find(index);

	// did we succeed in a direct hit?
	if (it != buffered_parameters_.end())
	{
		const vector<double>& v = (*it).second;
		v1 = v[0];
		v2 = v[1];
		v3 = v[2];
		return true;
	}

	for (Position p = 0; p < 5; p++)
	{
		String index2 = getIndex_(torsion_type, 
															equiv_->getEquivalence(at1, ic_[p]),
															at2, at3,
															equiv_->getEquivalence(at4, lc_[p]));

		it = parameters_.find(index2);

		if (it != parameters_.end())
		{
			const vector<double>& v = (*it).second;
			v1 = v[0];
			v2 = v[1];
			v3 = v[2];

			// buffer this result 
			buffered_parameters_[index] = v;

			return true;
		}
	}

	return false;
}

bool MMFF94TorsionParameters::setup_(const vector<vector<String> >& lines)	
{
	parameters_.clear();

	try
	{
		for (Position p = 0; p < lines.size(); p++)
		{
			const vector<String>& fields = lines[p];

			const Position type = fields[0].toUnsignedInt();
			const Position atom_type1 = fields[1].toUnsignedInt();
			const Position atom_type2 = fields[2].toUnsignedInt();
			const Position atom_type3 = fields[3].toUnsignedInt();
			const Position atom_type4 = fields[4].toUnsignedInt();
			const String index = getIndex_(type, atom_type1, atom_type2, atom_type3, atom_type4);

			parameters_[index] = vector<double>();
			parameters_[index].push_back(fields[5].toDouble());
			parameters_[index].push_back(fields[6].toDouble());
			parameters_[index].push_back(fields[7].toDouble());
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

String MMFF94TorsionParameters::getIndex_(Position type,
							Position at1, Position at2, Position at3, Position at4) const
{ 
	return String(type) + '|' + String(at1) + '|' + String(at2) + '|' + String(at3) + '|' + String(at4);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

MMFF94PlaneParameters::MMFF94PlaneParameters()
	: MMFF94ParametersBase()
{
	number_expected_fields_ = 6;
}

MMFF94PlaneParameters::~MMFF94PlaneParameters()
{
	clear();
}

void MMFF94PlaneParameters::clear()
	
{
	parameters_.clear();
}

const MMFF94PlaneParameters& MMFF94PlaneParameters::operator = (const MMFF94PlaneParameters& param)
	
{
	parameters_ = param.parameters_;
	return *this;
}

bool MMFF94PlaneParameters::assignParameters(Index at1, Index at2, Index at3, Index at4, double& v) const
{
	String index = getIndex_(at1, at2, at3, at4);

#ifdef BALL_DEBUG_MMFF
	Log.info() << at1 << " " << at2 << " " << at3 << " " << at4  << std::endl;
#endif

	// first we try a buffer for previous hits to speed up things a bit
	// this will prevent searching for combinations further down the step by step approach
	PlaneMap::ConstIterator it = buffered_parameters_.find(index);

	// did we succeed in a direct hit?
	if (it != buffered_parameters_.end())
	{
		v = (*it).second;
		return true;
	}

	for (Position p = 0; p < 5; p++)
	{
		String index2 = getIndex_(equiv_->getEquivalence(at1, p),
															at2,
															equiv_->getEquivalence(at3, p), 
															equiv_->getEquivalence(at4, p));
		{
			it = parameters_.find(index2);

			if (it != parameters_.end())
			{
#ifdef BALL_DEBUG_MMFF
	Log.info() << " Found OOP Parameter " << v << std::endl;
#endif
				v = (*it).second;
				// buffer this result 
				buffered_parameters_[index] = v;
				return true;
			}
		}
	}

	return false;
}

bool MMFF94PlaneParameters::setup_(const vector<vector<String> >& lines)
{
	parameters_.clear();

	try
	{
		for (Position p = 0; p < lines.size(); p++)
		{
			const vector<String>& fields = lines[p];

			const Position atom_type1 = fields[0].toUnsignedInt();
			const Position atom_type2 = fields[1].toUnsignedInt();
			const Position atom_type3 = fields[2].toUnsignedInt();
			const Position atom_type4 = fields[3].toUnsignedInt();
			const String index = getIndex_(atom_type1, atom_type2, atom_type3, atom_type4);

			parameters_[index] = fields[4].toDouble();
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

String MMFF94PlaneParameters::getIndex_(Position at1, Position at2, Position at3, Position at4) const
{ 
	vector<Position> vs;
	vs.push_back(at1);
	vs.push_back(at3);
	vs.push_back(at4);
	sort(vs.begin(), vs.end());

#ifdef BALL_DEBUG_MMFF
Log.error() << at1 << " " << at2 << " " << at3 << " " << at4 << std::endl;
#endif

	return String(vs[0]) + '|' + String(at2) + '|' + String(vs[1]) + '|' + String(vs[2]);
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

MMFF94VDWParameters::MMFF94VDWParameters()
	: MMFF94ParametersBase()
{
	number_expected_fields_ = 8;
}

MMFF94VDWParameters::~MMFF94VDWParameters()
{
	clear();
}

void MMFF94VDWParameters::clear()
	
{
	parameters_.clear();
}

const MMFF94VDWParameters& MMFF94VDWParameters::operator = (const MMFF94VDWParameters& param)
	
{
	parameters_ = param.parameters_;
	return *this;
}

const MMFF94VDWParameters::VDWEntry& MMFF94VDWParameters::getParameters(Index at) const
{
	if (at > (Index) MMFF94_number_atom_types || at < 0) return parameters_[0];
	return parameters_[at];
}


bool MMFF94VDWParameters::setup_(const vector<vector<String> >& lines)
{
	parameters_.clear();
	parameters_.resize(MMFF94_number_atom_types);

	for (Position p = 0; p < MMFF94_number_atom_types; p++)
	{
		parameters_[p].valid = false;
	}

	try
	{
		for (Position p = 0; p < lines.size(); p++)
		{
			const vector<String>& fields = lines[p];

			Position type = fields[0].toUnsignedInt();
			parameters_[type] = VDWEntry();
			VDWEntry& e = parameters_[type];

			e.alpha_i = fields[1].toDouble();
			e.ni = fields[2].toDouble();
			e.ai = fields[3].toDouble();
			e.gi = fields[4].toDouble();
			e.valid = true;
			if      (fields[5] == "-") e.donor_acceptor = 0;
			else if (fields[5] == "D") e.donor_acceptor = 1;
			else if (fields[5] == "A") e.donor_acceptor = 2;
		}
	}
	catch(...)
	{
		return false;
	}

	rs_.resize(MMFF94_number_atom_types);

	for (Position p = 0; p < MMFF94_number_atom_types; p++)
	{
		rs_[p] = -1;
	}

	const Size max = MMFF94_number_atom_types * MMFF94_number_atom_types;
	rij_.resize(max);
	eij_.resize(max);
	rij_7_.resize(max);
	calculated_.resize(max);

	for (Position p = 0; p < max; p++)
	{
		calculated_[p] = false;
	}

	return true;
}

double MMFF94VDWParameters::getR(Position t) const
{
	// first try buffered value
	if (rs_[t] > 0) return rs_[t];

	const VDWEntry& entry = parameters_[t];
	if (!entry.valid) return -1;

	const double r = entry.ai * pow(entry.alpha_i, 0.25);
	rs_[t] = r;
	return r;
}

bool MMFF94VDWParameters::assignParameters(Position at1, Position at2, double& rij, double& rij_7, double& eij) const
{
	const Position index = at1 * MMFF94_number_atom_types + at2;

	// first try buffered value
	if (calculated_[index]) 
	{
		rij = rij_[index];
		rij_7 = rij_7_[index];
		eij = eij_[index];
		return true;
	}

	const double ri = getR(at1);
	const double rj = getR(at2);

	// no parameters for one of the two types?
	if (ri < 0 || rj < 0) return false;

	// equation 4
	const double l = (ri - rj) / (ri + rj);

	// zero pointers are caught above
	const VDWEntry& e1 = getParameters(at1);
	const VDWEntry& e2 = getParameters(at2);
	
	// 2 identical atom types? 
	if (at1 != at2) 
	{
		// equation 3
		// either one of both is a donor?
		if (e1.donor_acceptor == 1 || e2.donor_acceptor == 1)
		{
			rij = 0.5 * (ri + rj);
		}
		else
		{
			rij = 0.5 * (ri + rj) * (1. + 0.2 * (1. - exp(-12. * l * l)));
		}
	}
	else
	{
		rij = ri;
	}

	// equation 5, upper part
	const double up = 181.16 * e1.gi * e2.gi * e1.alpha_i * e2.alpha_i;

	// equation 5, lower part
	const double lo = sqrt(e1.alpha_i / e1.ni) + sqrt(e2.alpha_i / e2.ni);

	// equation 5
	eij = (up / lo) / pow(rij, 6.);

	if (at1 != at2 &&
			e1.donor_acceptor * e2.donor_acceptor == 2)
	{
		rij *= 0.8;
		eij *= 0.5;
	}

	rij_[index] = rij;
	rij_7_[index] = rij_7 = pow(rij, 7.);
	eij_[index] = eij;
	calculated_[index] = true;

	return true;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

MMFF94ESParameters::MMFF94ESParameters()
	: MMFF94ParametersBase()
{
	number_expected_fields_ = 5;
}

MMFF94ESParameters::~MMFF94ESParameters()
{
	clear();
}

void MMFF94ESParameters::clear()
	
{
	parameters_.clear();
	phis_.clear();
	pbcis_.clear();
}

const MMFF94ESParameters& MMFF94ESParameters::operator = (const MMFF94ESParameters& param)
	
{
	parameters_ = param.parameters_;
	pbcis_ 			= param.pbcis_;
	phis_ 			= param.phis_;
	return *this;
}

bool MMFF94ESParameters::setup_(const vector<vector<String> >& lines)
{
	parameters_.clear();
	parameters_.resize(MMFF94_number_atom_types * MMFF94_number_atom_types * 5);

	for (Position p = 0; p < MMFF94_number_atom_types * MMFF94_number_atom_types * 5; p++)
	{
		parameters_[p] = MMFF94_INVALID_VALUE;
	}

	try
	{
		for (Position p = 0; p < lines.size(); p++)
		{
			const vector<String>& fields = lines[p];

			const Position btype = fields[0].toUnsignedInt();
			const Position at1 = fields[1].toUnsignedInt();
			const Position at2 = fields[2].toUnsignedInt();
			parameters_[getIndex_(at1, at2, btype)] = fields[3].toDouble();
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

Position MMFF94ESParameters::getIndex_(Position at1, Position at2, Position bt) const
{
	return at1 * MMFF94_number_atom_types + at2 + bt * MMFF94_number_atom_types * MMFF94_number_atom_types;
}

double MMFF94ESParameters::getPartialCharge(Position at1, Position at2, Position bt) const
{
	if (at1 == at2) return 0;

	const Position index = getIndex_(at1, at2, bt);

	if (index > parameters_.size()) return MMFF94_INVALID_VALUE;

	double r = parameters_[index];

	if (r != MMFF94_INVALID_VALUE) return r;

	///////////////////////////
	// heuristic value:
	///////////////////////////
	double p1 = getPBCI(at1);
	double p2 = getPBCI(at2);

	if (p1 != MMFF94_INVALID_VALUE &&
			p2 != MMFF94_INVALID_VALUE)
	{
		r = p2 - p1;
	}

	if (r == MMFF94_INVALID_VALUE)
	{
		Log.error() << "No ES parameters: "  << bt << " " << at1 << " " << at2 << std::endl;
	}

//    	Log.error() << "ES "  << bt << " " << at1 << " " << at2 << " r " << r << std::endl;

	return r;
}

bool MMFF94ESParameters::readEmpiricalParameters(Parameters& p, const String& section)
	throw(Exception::FileNotFound)
{
	phis_.resize(MMFF94_number_atom_types);
	pbcis_.resize(MMFF94_number_atom_types);

	for (Position k = 0; k < MMFF94_number_atom_types; k++)
	{
		phis_[k] = MMFF94_INVALID_VALUE;
		pbcis_[k] = MMFF94_INVALID_VALUE;
	}

	ParameterSection p_sec;
	p_sec.extractSection(p, section);

	vector<String> fields;

	try
	{
		for(Position i = 0; i < p_sec.getNumberOfKeys(); ++i)
		{
			String key = p_sec.getKey(i);
			if (1 + p_sec.getNumberOfVariables() < 4)
			{
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
										<< section << " Not 4 fields in one line " 
										<< std::endl;
				return false;
			}

			const Position t = key.toUnsignedInt();
			pbcis_[t] = p_sec.getValue(i, 0).toDouble();
			phis_[t]  = p_sec.getValue(i, 1).toDouble();
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

double MMFF94ESParameters::getPhi(Index atom_type) const
{
	if (atom_type < 1 || atom_type > (Index) MMFF94_number_atom_types - 1) return MMFF94_INVALID_VALUE;

	return phis_[atom_type];
}

double MMFF94ESParameters::getPBCI(Index atom_type) const
{
	if (atom_type < 1 || atom_type > (Index) MMFF94_number_atom_types - 1) return MMFF94_INVALID_VALUE;

	return pbcis_[atom_type];
}


} // namespace BALL
