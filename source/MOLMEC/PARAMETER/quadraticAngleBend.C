// $Id: quadraticAngleBend.C,v 1.4 1999/12/28 17:52:38 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/quadraticAngleBend.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	FFPSQuadraticAngleBend::FFPSQuadraticAngleBend()
		:	FFParameterSection(),
			k_(0),
			theta0_(0),
			is_defined_(0)
	{
	}

	FFPSQuadraticAngleBend::~FFPSQuadraticAngleBend()
	{
		destroy();
	}

	void FFPSQuadraticAngleBend::destroy() 
	{
		// clear allocatd parameter fields
		delete [] k_;
		delete [] theta0_;
		delete [] is_defined_;

		FFParameterSection::destroy();
	}

	bool FFPSQuadraticAngleBend::extractSection
		(ForceFieldParameters& parameters, const String& section_name)
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// extract the basis information
		FFParameterSection::extractSection(parameters, section_name);
		
		// check whether all variables we need are defined, terminate otherwise
		if (!hasVariable("theta0") || !hasVariable("k"))
		{
			Log.level(LogStream::ERROR) << "quadratic angle bend section needs two variable columns: theta0, and k" << endl;
			return false;
		}

		// build a two dimensional array of the atom types
		// loop variable
		Size	i;
		FFPSAtomTypes& atom_types = parameters.getAtomTypes();
		number_of_atom_types_ = atom_types.getNumberOfTypes();
		Size number_of_entries = number_of_atom_types_ * number_of_atom_types_ 
                                                                    * number_of_atom_types_;
		
		// allocate two one-dimensional fields for the two parameters
		k_  = new float[number_of_entries];
		theta0_ = new float[number_of_entries];
		is_defined_ = new bool[number_of_entries];

		for (i = 0; i < number_of_entries_; i++) 
		{
			is_defined_[i] = false;
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

		// start with line 1: skip the format line!
		for (i = 1; i < getNumberOfKeys(); i++)
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

					// calculate the indices in the parameter array
					index = (Index)(type_I + number_of_atom_types_ * type_J + number_of_atom_types_ * number_of_atom_types_ * type_K);
					sym_index = (Index)(type_K + number_of_atom_types_ * type_J + number_of_atom_types_ * number_of_atom_types_ * type_I);

					// set the values
					is_defined_[index] = true;
					k_[index] = getValue(i, index_k).toFloat() * factor_k;
					theta0_[index] = getValue(i, index_theta0).toFloat() * factor_theta0;

					is_defined_[sym_index] = true;
					k_[sym_index] = getValue(i, index_k).toFloat() * factor_k;
					theta0_[sym_index] = getValue(i, index_theta0).toFloat() * factor_theta0;
				} else {
					Log.level(LogStream::WARNING) << "could not identify atom types for key " << key << endl;
				}
			} else {
				Log.level(LogStream::ERROR) << "Wrong number of fields in the key: " << key << endl;
			}
		}

		return true;
	}


	bool FFPSQuadraticAngleBend::hasParameters(Atom::Type I, Atom::Type J, Atom::Type K) const 
	{
		if ((I < 0) || ((Size)I >= number_of_atom_types_))
			return false;

		if ((J < 0) || ((Size)J >= number_of_atom_types_))
			return false;

		if ((K < 0) || ((Size)K >= number_of_atom_types_))
			return false;


		return is_defined_[I + number_of_atom_types_ * J + number_of_atom_types_ * number_of_atom_types_ * K];
	}


	FFPSQuadraticAngleBend::Values FFPSQuadraticAngleBend::getParameters
		(Atom::Type I, Atom::Type J, Atom::Type K) const 
	{
		FFPSQuadraticAngleBend::Values parameters;
		assignParameters(parameters, I, J, K);
		return parameters;
	}


	bool FFPSQuadraticAngleBend::assignParameters
		(FFPSQuadraticAngleBend::Values& parameters,
		 Atom::Type I, Atom::Type J, Atom::Type K) const 
	{
		Index index = (Index)(I + number_of_atom_types_ * J + number_of_atom_types_ * number_of_atom_types_ * K);
		if (is_defined_[index]) 
		{
			parameters.k = k_[index];
			parameters.theta0 = theta0_[index];
			return true;
		}

		return false;
	}
	 
} // namespace BALL
