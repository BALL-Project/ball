// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: quadraticAngleBend.C,v 1.13 2002/12/12 10:44:37 oliver Exp $

#include <BALL/MOLMEC/PARAMETER/quadraticAngleBend.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	QuadraticAngleBend::QuadraticAngleBend()
		:	ParameterSection(),
			number_of_atom_types_(0),
			values_(),
			value_index_(),
			comment_(),
			force_field_parameters_(0)
	{
	}

	QuadraticAngleBend::~QuadraticAngleBend() 
	{
		clear();
	}

	void QuadraticAngleBend::clear() 
	{
		// clear allocated parameter fields
		number_of_atom_types_ = 0;
		values_.clear();
		value_index_.clear();
		comment_.clear();
		force_field_parameters_ = 0;
		ParameterSection::clear();
	}

	bool QuadraticAngleBend::extractSection
		(Parameters& parameters, const String& section_name)
	{
		return ParameterSection::extractSection(parameters, section_name);
	}

	bool QuadraticAngleBend::extractSection
		(ForceFieldParameters& parameters, const String& section_name)
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// extract the basis information
		ParameterSection::extractSection(parameters, section_name);
		
		// check whether all variables we need are defined, terminate otherwise
		if (!hasVariable("theta0") || !hasVariable("k"))
		{
			Log.error() << "QuadraticAngleBend::extractSection: quadratic angle bend "
				<< "section needs two variable columns: theta0, and k" << endl;
			return false;
		}

		// build a two dimensional array of the atom types
		AtomTypes& atom_types = parameters.getAtomTypes();
		number_of_atom_types_ = atom_types.getNumberOfTypes();
		Size number_of_entries = number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
		
		// store a pointer to the corresponding atom types
		force_field_parameters_ = &parameters;

		// clear internal data structures
		values_.clear();
		value_index_.clear();
		value_index_.resize(number_of_entries);
		comment_.clear();

		// set the value_index_ array to -1 (= parameter undefined)
		Size	i;
		for (i = 0; i < number_of_entries; i++) 
		{
			value_index_[i] = -1;
		}

		// try to determine the factors to convert the units
		double factor_theta0 = Constants::RAD_PER_DEG;
		double factor_k = 1.0;
		if (options.has("unit_k"))
		{
			if (options["unit_k"] == "kcal/mol")
			{
				factor_k = Constants::JOULE_PER_CAL;
			}
			if (options["unit_k"] == "cal/mol")
			{
				factor_k = Constants::JOULE_PER_CAL * 0.001;
			}
			if (options["unit_k"] == "J/mol")
			{
				factor_k = 0.001;
			}
		}

		// theta0 is expected in degrees and returned in rad
		if (options.has("unit_theta0"))
		{
			if (options["unit_theta0"] == "rad")
			{
				factor_theta0 = 1.0;
			}
		}

		Atom::Type		type_I;
		Atom::Type		type_J;
		Atom::Type		type_K;
		String				key;
		
		// two indices: index for the combination I-J-K
		// and sym_index for the symmetric parameters K-J-I
		Index					index = 0;
		Index					sym_index = 0;

		// get the column indices of the different table variables
		Size index_k			= getColumnIndex("k");
		Size index_theta0	= getColumnIndex("theta0");
		Size index_comment	= getColumnIndex("comment");

		String	fields[4];
		Values	values;
		// retrieve all keys from the section and interpret their values
		for (i = 0; i < getNumberOfKeys(); i++)
		{
			key = getKey(i);

			// split the key into the three type names
			if (key.split(fields, 3) == 3)
			{
				// check whether the three type names are valid
				if ((atom_types.hasType(fields[0]))
						&& (atom_types.hasType(fields[1])) 
						&& (atom_types.hasType(fields[2]))) 
				{
					// extract the three atom types
					type_I = atom_types.getType(fields[0]);
					type_J = atom_types.getType(fields[1]);
					type_K = atom_types.getType(fields[2]);

					// calculate the index in the in value_index_ array
					index = (Index)(type_I + number_of_atom_types_ * type_J + number_of_atom_types_ * number_of_atom_types_ * type_K);
					sym_index = (Index)(type_K + number_of_atom_types_ * type_J + number_of_atom_types_ * number_of_atom_types_ * type_I);


					// set the indices
					value_index_[index] = (Size)values_.size();
					value_index_[sym_index] = (Size)values_.size();

					// store the values 
					values.k = getValue(i, index_k).toFloat() * factor_k;
					values.theta0 = getValue(i, index_theta0).toFloat() * factor_theta0;
					values_.push_back(values);
					comment_.push_back(getValue(i, index_comment));
				} 
				else 
				{
					Log.error() << "QuadraticAngleBend::extractSection: could not identify atom types for key " << key << endl;
				}
			} 
			else 
			{
				Log.error() << "QuadraticAngleBend::extractSection: wrong number of fields in the key: " << key << endl;
			}
		}

		return true;
	}
	
	bool QuadraticAngleBend::exportParmFile(File& outfile) const
	{	
		
		if (!force_field_parameters_)
			return false;

		AtomTypes& atom_types = force_field_parameters_->getAtomTypes();

		// Amber requires kcal/mol/rad^2 and degree
		// while BALL uses kjoule/mol/degree^2 and degree
		double factor_k      = Constants::CAL_PER_JOULE; 
		double factor_theta0 = Constants::DEG_PER_RAD; 
		
		Index	 index     = 0;
		Values  value;

		// a string buffer for snprintf
		char buffer[1024];
		
		for (Size i=1; i < number_of_atom_types_; i++)
		{	
			String atom_I =  atom_types.getTypeName(i);

			for (Size j=0; j < number_of_atom_types_; j++)
			{
				String atom_J =  atom_types.getTypeName(j);

				for (Size k=i; k < number_of_atom_types_; k++)
				{
					index =(Index)(i + number_of_atom_types_ * j + number_of_atom_types_ * number_of_atom_types_ * k);
					if (value_index_[index] > -1)
					{
						String atom_K = atom_types.getTypeName(k);

						// calculate the index into the in value_index_ array
						index = value_index_[(Index)(i + number_of_atom_types_ * j + number_of_atom_types_ * number_of_atom_types_ * k)];
						value = values_[index];

						//emulate format: O -C -O     80.0      126.00    AA COO- terminal residues
						snprintf(buffer, 1024, "%-2s-%-2s-%-2s  %-3.1f    %-3.2f    %s",	
								atom_I.c_str(), atom_J.c_str(), atom_K.c_str(),  
								value.k * factor_k, value.theta0 * factor_theta0, 
								comment_[index].c_str());
						outfile << buffer << endl;
					}
				}
			}	
		}

		// Terminate by blank card
		outfile << endl;
		return true;
	}

	bool QuadraticAngleBend::hasParameters(Atom::Type I, Atom::Type J, Atom::Type K) const 
	{
		return 
			// check the indices
			(((I >= 0) || ((Size)I < number_of_atom_types_))
			 && ((J >= 0) || ((Size)J < number_of_atom_types_))
			 && ((K >= 0) || ((Size)K < number_of_atom_types_))
			// and the availability of parameters
			 && (value_index_[I + number_of_atom_types_ * J 
											+ number_of_atom_types_ * number_of_atom_types_ * K] >= 0));
	}


	QuadraticAngleBend::Values QuadraticAngleBend::getParameters
		(Atom::Type I, Atom::Type J, Atom::Type K) const 
	{
		QuadraticAngleBend::Values parameters;
		assignParameters(parameters, I, J, K);
		return parameters;
	}


	bool QuadraticAngleBend::assignParameters
		(QuadraticAngleBend::Values& parameters,
		 Atom::Type I, Atom::Type J, Atom::Type K) const 
	{
		// check whether the parameters are defined
		Index index = (Index)(I + number_of_atom_types_ * J + number_of_atom_types_ * number_of_atom_types_ * K);
		if (value_index_[index] >= 0) 
		{	
			// retrieve and assign the parameters
			parameters = values_[value_index_[index]];

			return true;
		}

		return false;
	}
	 
} // namespace BALL
