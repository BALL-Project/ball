// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: solventParameter.C,v 1.11 2003/08/26 09:18:26 oliver Exp $
//

#include <BALL/SOLVATION/solventParameter.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

namespace BALL
{

	SolventParameter::SolventParameter()
		: ParameterSection(),
			name_(),
			number_density_(0.0),
			solvent_atoms_(),
			solvent_descriptor_(),
			atom_types_()
	{
	}


	SolventParameter::SolventParameter(const SolventParameter& param)
		: ParameterSection(param),
			name_(param.name_),
			number_density_(param.number_density_),
			solvent_atoms_(param.solvent_atoms_),
			solvent_descriptor_(param.solvent_descriptor_),
			atom_types_(param.atom_types_)
	{
	}


	SolventParameter::SolventParameter(const AtomTypes& atom_types)
		: ParameterSection(),
			name_(),
			number_density_(),
			solvent_atoms_(),
			solvent_descriptor_(),
			atom_types_(atom_types)
	{
	}


	SolventParameter::~SolventParameter()
	{
		clear();
		valid_ = false;
	}


	void SolventParameter::clear()
	{
		ParameterSection::clear();
		name_ = "";
		number_density_ = 0.0;
		solvent_descriptor_.clear();
		solvent_atoms_.clear();
		atom_types_.clear();
	}


	const SolventParameter& SolventParameter::operator = 
		(const SolventParameter& param)
	{
		ParameterSection::operator = (param);
		name_ = param.name_;
		number_density_ = param.number_density_;
		solvent_atoms_ = param.solvent_atoms_;
		solvent_descriptor_ = param.solvent_descriptor_;
		atom_types_ = param.atom_types_;

		return *this;
	}


	bool SolventParameter::operator == (const SolventParameter& param) const
	{
		// ?????
		return ((ParameterSection::operator == (param))
			&& (name_ == param.name_)
			&& (number_density_ == param.number_density_));
			// && (solvent_atoms_ == param.solvent_atoms_)
			// && (solvent_descriptor_ == param.solvent_descriptor_));
	}


	SolventDescriptor& SolventParameter::getSolventDescriptor()
	{
		return solvent_descriptor_;
	}

	const SolventDescriptor& SolventParameter::getSolventDescriptor() const
	{
		return solvent_descriptor_;
	}

	const String& SolventParameter::getSolventName() const
	{
		return name_;
	}

	const std::vector<SolventAtomDescriptor>& SolventParameter::getSolventDescription() const
	{
		return solvent_atoms_;
	}  

	const float& SolventParameter::getSolventDensity() const
	{
		return number_density_;
	}

	bool SolventParameter::extractSection(Parameters& parameters,
			const String& section_name)
	{
		// ?????
		if (!parameters.isValid())
		{
			return false;
		}

		// extract the basis information
		ParameterSection::extractSection(parameters, section_name);

		// check whether all variables we need are defined, terminate otherwi
		if (!hasVariable("radius") || !hasVariable("number_of_atoms") || !hasVariable("element_symbol"))
		{
			Log.error() << "SolventParameter::extractSection(): Variable missing." << endl;
			return false;
		}
		else
		{
			if (options.has("name"))
			{
				name_ = options.get("name");
			}
			else
			{
				Log.warn() << "SolventParameter::extractSection(): no name given." << endl;
			}

			if (options.has("number_density"))
			{
				number_density_ = options.getReal("number_density");
			}
			else
			{
				Log.warn() << "SolventParameter::extractSection(): " << "no number density given." << endl;
			}

			if (!atom_types_.isValid())
			{
				if (options.has("force_field"))
				{
					Path path;
					String force_field_file = path.find(options.get("force_field"));
					if (force_field_file == "") 
					{
						force_field_file = options.get("force_field");
					}
					Parameters tmp_ffp(force_field_file);
					tmp_ffp.init();
					atom_types_.extractSection(tmp_ffp, "AtomTypes");
				}
				else
				{
					Log.error() << "SolventParameter::extractSection(): "
						<< "did not find atom types, aborting." << endl;
					return(false);
				}
			}

			Size number_of_keys = getNumberOfKeys();
			solvent_atoms_.resize(number_of_keys);

			Size index_element_symbol = getColumnIndex("element_symbol");
			Size index_radius = getColumnIndex("radius");
			Size index_number_of_atoms = getColumnIndex("number_of_atoms");
		
			for (Size i = 0; i < number_of_keys; ++i)
			{
				String type_name = getKey(i);
				if (atom_types_.has(type_name))
				{
					solvent_atoms_[i].type = atom_types_.getType(type_name);
					solvent_atoms_[i].element_symbol = getValue(i, index_element_symbol);
					solvent_atoms_[i].radius = getValue(i, index_radius).toFloat();
					solvent_atoms_[i].number_of_atoms = getValue(i, index_number_of_atoms).toInt();
				}
				else
				{
					Log.error() << "SolventParameter::extractSection(): " << "Cannot assign atom type." << endl;
					return(false);
				}
			}
			// build descriptor 
			solvent_descriptor_ = SolventDescriptor(name_, number_density_, solvent_atoms_);

			return true;
		}
	}
}
