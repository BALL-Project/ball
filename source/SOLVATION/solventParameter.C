// $Id: solventParameter.C,v 1.3 2000/10/17 17:20:45 anker Exp $

#include <BALL/SOLVATION/solventParameter.h>

using namespace std;

namespace BALL
{

	SolventParameter::SolventParameter() throw()
		: ParameterSection(),
			name_(),
			number_density_(0.0),
			solvent_atoms_(),
			solvent_descriptor_()
	{
	}


	SolventParameter::SolventParameter(const SolventParameter& param) throw()
		: ParameterSection(param),
			name_(param.name_),
			number_density_(param.number_density_),
			solvent_atoms_(param.solvent_atoms_),
			solvent_descriptor_(param.solvent_descriptor_)
	{
	}


	SolventParameter::~SolventParameter() throw()
	{
		clear();

		valid_ = false;
	}


	void SolventParameter::clear() throw()
	{
		ParameterSection::clear();
		name_ = "";
		number_density_ = 0.0;
		solvent_descriptor_.clear();
		solvent_atoms_.clear();
	}


	const SolventParameter& SolventParameter::operator = 
		(const SolventParameter& param) throw()
	{
		ParameterSection::operator = (param);
		name_ = param.name_;
		number_density_ = param.number_density_;
		solvent_atoms_ = param.solvent_atoms_;
		solvent_descriptor_ = param.solvent_descriptor_;

		return *this;
	}


	bool SolventParameter::operator == (const SolventParameter& param) const
		throw()
	{
		// BAUSTELLE
		return ((ParameterSection::operator == (param))
			&& (name_ == param.name_)
			&& (number_density_ == param.number_density_));
			// && (solvent_atoms_ == param.solvent_atoms_)
			// && (solvent_descriptor_ == param.solvent_descriptor_));
	}


	SolventDescriptor SolventParameter::getSolventDescriptor() const throw()
	{
		return solvent_descriptor_;
	}

	bool SolventParameter::extractSection(ForceFieldParameters& parameters,
			const String& section_name) throw()
	{
		// BAUSTELLE
		if (!parameters.isValid())
		{
			return false;
		}

		// extract the basis information
		ParameterSection::extractSection(parameters, section_name);

		// check whether all variables we need are defined, terminate otherwi
		if (!hasVariable("radius") || !hasVariable("number_of_atoms") || !hasVariable("element_symbol"))
		{
			Log.error() << "SolventParameter::extractSection(): Variable missing." 
				<< endl;
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
				Log.warn() << "SolventParameter::extractSection(): no name given." 
					<< endl;
			}
			if (options.has("number_density"))
			{
				number_density_ = options.getReal("number_density");
			}
			else
			{
				Log.warn() << "SolventParameter::extractSection(): "
					<< "no number density given." << endl;
			}

			AtomTypes& atom_types = parameters.getAtomTypes();         

			Size number_of_keys = getNumberOfKeys();
			solvent_atoms_.resize(number_of_keys);

			Size index_element_symbol = getColumnIndex("element_symbol");
			Size index_radius = getColumnIndex("radius");
			Size index_number_of_atoms = getColumnIndex("number_of_atoms");
		
			for (Size i = 0; i < number_of_keys; ++i)
			{
				String type_name = getKey(i);
				if (atom_types.has(type_name))
				{
					solvent_atoms_[i].type = atom_types.getType(type_name);
					solvent_atoms_[i].element_symbol = getValue(i, index_element_symbol);
					solvent_atoms_[i].radius = getValue(i, index_radius).toFloat();
					solvent_atoms_[i].number_of_atoms = getValue(i, index_number_of_atoms).toInt();
				}
				else
				{
					Log.error() << "SolventParameter::extractSection(): "
						<< "Cannot assign atom type." << endl;
				}
			}
			// build descriptor 
			solvent_descriptor_ = SolventDescriptor(name_, number_density_,
					solvent_atoms_);
			return true;
		}
		// control flow should not reach this point
		Log.error() << "SolventParameter::extractSection(): "
			<< "reached unreachable part of program" << endl;
		return false;

	}

}
