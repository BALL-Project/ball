// $Id: claverieParameter.C,v 1.2 2000/09/02 14:34:12 oliver Exp $

#include <BALL/SOLVATION/claverieParameter.h>

using namespace std;

namespace BALL
{

	ClaverieParameter::ClaverieParameter()
		:	ParameterSection(),
			parameters_(),
			indices_()
	{
	}


	ClaverieParameter::ClaverieParameter(const ClaverieParameter& param)
		:	ParameterSection(param),
			parameters_(param.parameters_),
			indices_(param.indices_)
	{
	}


	ClaverieParameter::~ClaverieParameter()
	{
		destroy();
	}

	
	void ClaverieParameter::destroy()
	{
		clear();
	}


	void ClaverieParameter::clear()
	{
		// ParameterSection::clear();
		parameters_.clear();
		indices_.clear();
	}


	void ClaverieParameter::set(const ClaverieParameter& param)
	{
		// ParameterSection::set(param);
		parameters_ = param.parameters_;
		indices_ = param.indices_;
	}


	bool ClaverieParameter::hasParameters(Atom::Type solvent_type,
			Atom::Type solute_type) const
	{
		// BAUSTELLE!
		return true;
	}


	std::pair<float, float> ClaverieParameter::getParameters(
			Atom::Type solvent_type, Atom::Type solute_type) const
	{
		float K_m;
		float R_m;
		float K_s;
		float R_s;
		std::pair<float, float> params;

		K_s = parameters_[indices_[solvent_type]].first;
		K_m = parameters_[indices_[solute_type]].first;
		R_s = parameters_[indices_[solvent_type]].second;
		R_m = parameters_[indices_[solute_type]].second;

		/*
		// DEBUG
		Log.info() << "solvent_type = " << solvent_type << endl;
		Log.info() << "solute_type = " << solute_type << endl;
		Log.info() << "K_s = " << K_s << " R_s = " << R_s << endl;
		Log.info() << "K_m = " << K_m << " R_m = " << R_m << endl;
		*/

		params.first = K_m * K_s;
		params.second = 2 * sqrt( R_s * R_m );

		return params;
	}


	bool ClaverieParameter::extractSection(ForceFieldParameters& parameters,
		const String& section_name)
	{

		if (!parameters.isValid())
		{
			return false;
		}

		ParameterSection::extractSection(parameters, section_name);

		if (!hasVariable("K") || !hasVariable("R"))
		{
			Log.error() 
				<< "Variable missing." << endl;
			return false;
		}
		else
		{
			AtomTypes& atom_types = parameters.getAtomTypes();         

			Size number_of_keys = getNumberOfKeys();
			parameters_.resize(number_of_keys);

			Size index_K = getColumnIndex("K");
			Size index_R = getColumnIndex("R");

			for (Index i = 0; i < (Index) number_of_keys; ++i)
			{
				String type_name = getKey(i);
				if (atom_types.has(type_name))
				{
					parameters_[i].first = getValue(i, index_K).toFloat();
					parameters_[i].second = getValue(i, index_R).toFloat();
					indices_.insert(std::pair<Atom::Type, Index>(atom_types.getType(type_name), i));
					/*
					// DEBUG
					Log.info() << type_name << " (" << atom_types.getType(type_name)
						<< "): " << parameters_[i].first << " " << parameters_[i].second
						<< endl;
					*/

				}
				else
				{
					Log.error() << "Cannot assign atom type " << type_name << endl;
					return false;
				}
			}
		}
		return true;
	}

}
