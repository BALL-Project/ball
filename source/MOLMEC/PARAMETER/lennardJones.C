// $Id: lennardJones.C,v 1.5 2000/02/10 15:16:56 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/lennardJones.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	FFPSLennardJones::FFPSLennardJones()
		:	FFParameterSection(),
			A_(0),
			B_(0),
			Aij_(0),
			Bij_(0),
			is_defined_(0)
	{
	}

	FFPSLennardJones::~FFPSLennardJones()
	{
		destroy();
	}

	void FFPSLennardJones::destroy() 
	{
		// clear allocated parameter fields
		delete [] A_;
		delete [] B_;
		delete [] Aij_;
		delete [] Bij_;
		delete [] is_defined_;

		FFParameterSection::destroy();
	}

	bool FFPSLennardJones::extractSection(ForceFieldParameters& parameters, const String& section_name)
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// extract the basis information
		FFParameterSection::extractSection(parameters, section_name);

		// check whether all variables we need are defined, terminate otherwise
		if ((!hasVariable("A") || !hasVariable("B"))
				&& (!hasVariable("epsilon") || !hasVariable("R")))
		{
			Log.error() << "Lennard Jones parameter section requires two variable columns:"		
				<< "A/B or epsilon/R" << endl;

			return false;

		} else {

			// format_ == A_B_FORMAT:				parameters are in A/B format
			// format_ == EPSILON_R_FORMAT:	parameters are in epsilon/R format
			// format_ == SLATER_KIRKWOOD_FORMAT:	parameters are in epsilon/R format
			if (hasVariable("epsilon") && hasVariable("R"))
			{
				format_ = EPSILON_R_FORMAT;
			} 
			else if (hasVariable("A") && hasVariable("B"))
			{
				format_ = A_B_FORMAT;
			}		
			else if (hasVariable("alpha") && hasVariable("N") && hasVariable("R"))
			{
				// BAUSTELLE
				format_ = SLATER_KIRKWOOD_FORMAT;
				Log.error() << "FFPSLennardJones::extractSection: Slater Kirkwood format not yet supported!" << endl;

				return false;
			}
							 
		}

		// build a two dimensional array of the atom types
		// loop variable
		Size	i;
		const FFPSAtomTypes&	atom_types = parameters.getAtomTypes();
		number_of_atom_types_ = atom_types.getNumberOfTypes();
		
		// allocate two onedimensional fields for the two parameters
		A_ = new float[number_of_atom_types_];
		B_ = new float[number_of_atom_types_];
		Aij_ = new float[number_of_atom_types_ * number_of_atom_types_];
		Bij_ = new float[number_of_atom_types_ * number_of_atom_types_];
		is_defined_ = new bool[number_of_atom_types_];

		for (i = 0; i < number_of_atom_types_; i++) 
		{
			is_defined_[i] = false;
		}

		// the indices of the colums containing the values
		Size					index_A;
		Size					index_B;
		if (format_ == A_B_FORMAT)
		{
			index_A = getColumnIndex("A");
			index_B = getColumnIndex("B");
		} 
		else if (format_ == EPSILON_R_FORMAT) 
		{
			index_A = getColumnIndex("epsilon");
			index_B = getColumnIndex("R");
		}
		else if (format_ == SLATER_KIRKWOOD_FORMAT) 
		{
			index_A = getColumnIndex("epsilon");
			index_B = getColumnIndex("R");
		}

		// try to identify the units of A and B
		// and set the two conversion factors
		double factor_A = 1.0;
		double factor_B = 1.0;
		
		if (format_ == A_B_FORMAT)
		{
			if (options.has("unit_A"))
			{ //BAUSTELLE
			}
		}
		// in EPSILON_R_FORMAT epsilon is in A and R is in B
		if (format_ == EPSILON_R_FORMAT)
		{
			if (options.has("unit_epsilon"))
			{
				if (options["unit_epsilon"] == "kcal/mol")
				{
					factor_A = Constants::JOULE_PER_CAL;
				}
				if (options["unit_epsilon"] == "cal/mol")
				{
					factor_A = Constants::JOULE_PER_CAL * 0.001;					
				}
				if (options["unit_epsilon"] == "J/mol")
				{
					factor_A = 0.001;
				}
			}

			if (options.has("unit_R"))
			{
				if (options["unit_R"] == "pm")
				{
					factor_B = 0.1;
				}
			}
		}

		Atom::Type		atom_type;
		String				key;
		for (i = 1; i <= getNumberOfKeys(); ++i)
		{
			// get the key
			key = getKey(i);
			if (atom_types.hasType(key))
			{
				// get the two parameters
				atom_type = atom_types.getType(key);
				
				// retrieve the two values
				float A = getValue(i, index_A).toFloat() * factor_A;
				float B = getValue(i, index_B).toFloat() * factor_B;

				// store the values
				is_defined_[atom_type] = true;
				A_[atom_type] = A;
				B_[atom_type] = B;

				// check for the sign of the parameters: they have to be positive!
				if ((A < 0) || (B < 0))
				{
					if (format_ == EPSILON_R_FORMAT)
					{
						Log.warn() << "VdW parameter may not be negative: type = " << atom_type << " (" << key << "), eps = " << A 
											 << ", r = " << B << endl;
					} else {
						Log.warn() << "VdW parameter may not be negative: type = " << atom_type << " (" << key << "), A = " << A 
											 << ", B = " << B << endl;
					}
				}

			} else {

				Log.warn() << "unknown atom type in Lennard Jones parameters: " << key << "   i = " << i << endl;
			}
		}

		// now assemble all Lennard Jones parameter for all known atom types
			
		for (i = 0; i < number_of_atom_types_; i++)
		{
			for (Size j = i; j < number_of_atom_types_; j++)
			{
				// calculate the two indices for the Aij/Bij fields
				Index index = (Index)(i + number_of_atom_types_ * j);
				Index sym_index = (Index)(j + number_of_atom_types_ * i);

				if ((is_defined_[j] == true) && (is_defined_[i]))
				{
					// calculate the values for A and B if in eps/R format
					if (format_ == EPSILON_R_FORMAT)
					{
						double R = B_[i] + B_[j];
						double R3 = R * R * R;
						double R6 = R3 * R3;
						double epsilon = sqrt(A_[i] * A_[j]);
						Aij_[index] = epsilon * R6 * R6;
						Bij_[index] = 2.0 * epsilon * R6;
					} else {
						// compute and assign Aij/Bij:
						// Aij = Ai * Aj, Aji = Aij
						// Bij = Bi * Bj, Bji = Bij
						Aij_[index] = A_[i] * A_[j];
						Bij_[index] = B_[i] * B_[j];
					}
					Aij_[sym_index] = Aij_[index];
					Bij_[sym_index] = Bij_[index];
				} else {
					Aij_[index] = 0.0;
					Bij_[index] = 0.0;
					Aij_[sym_index] = 0.0;
					Bij_[sym_index] = 0.0;
				}				
			}
		}

		return true;
	}


	bool FFPSLennardJones::hasParameters(Atom::Type I, Atom::Type J) const 
	{
		if ((I < 0) && ((Size)I >= number_of_atom_types_))
		{
			return false;
		}

		if ((J < 0) && ((Size)J >= number_of_atom_types_))
		{
			return false;
		}

		return (is_defined_[I] && is_defined_[J]);
	}


	FFPSLennardJones::Values FFPSLennardJones::getParameters
		(Atom::Type I, Atom::Type J) const 
	{
		FFPSLennardJones::Values parameters;
		assignParameters(parameters, I, J);
		return parameters;
	}


	bool FFPSLennardJones::assignParameters
		(FFPSLennardJones::Values& parameters,
		 Atom::Type I, Atom::Type J) const 
	{
		if (hasParameters(I, J)) 
		{
			parameters.A = Aij_[I * number_of_atom_types_ + J];
			parameters.B = Bij_[I * number_of_atom_types_ + J];
			
			return true;
		}

		return false;
	}
	 
} // namespace BALL
