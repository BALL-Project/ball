// $Id: quadraticAngleBend.C,v 1.11 2001/12/14 01:18:12 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/quadraticAngleBend.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	QuadraticAngleBend::QuadraticAngleBend()
		:	ParameterSection(),
			number_of_atom_types_(0)
	{
	}

	QuadraticAngleBend::~QuadraticAngleBend() throw()
	{
		clear();
	}

	void QuadraticAngleBend::clear() throw()
	{
		// clear allocatd parameter fields
		values_.clear();
		value_index_.clear();

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
		
		// clear internal data structures
		values_.clear();
		value_index_.clear();
		value_index_.resize(number_of_entries);

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
