// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: quadraticBondStretch.C,v 1.10 2003/08/26 09:17:54 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/quadraticBondStretch.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	QuadraticBondStretch::QuadraticBondStretch()
		:	ParameterSection(),
			k_(0),
			r0_(0),
			is_defined_(0),
			force_field_parameters_(0),
			comment_(0)
	{
	}

	QuadraticBondStretch::~QuadraticBondStretch() 
	{
		clear();
	}

	void QuadraticBondStretch::clear() 
	{
		// clear allocated parameter fields
		delete [] k_;
		delete [] r0_;
		delete [] is_defined_;
		delete [] comment_;
		
		k_ = 0;
		r0_ = 0;
		is_defined_ = 0;
		force_field_parameters_ = 0;
		comment_ = 0;
		
		ParameterSection::clear();
	}

	bool QuadraticBondStretch::extractSection
		(Parameters& parameters, const String& section_name)
	{
		return ParameterSection::extractSection(parameters, section_name);
	}

	bool QuadraticBondStretch::extractSection
		(ForceFieldParameters& parameters, const String& section_name)
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// store a pointer to the corresponding atom types
		force_field_parameters_ = &parameters;

		// extract the basis information
		ParameterSection::extractSection(parameters, section_name);
		
		// check whether all variables we need are defined, terminate otherwise
		if (!hasVariable("r0") || !hasVariable("k"))
		{
			return false;
		}

		// build a two dimensional array of the atom types
		// loop variable
		Size	i;

		AtomTypes& atom_types = parameters.getAtomTypes();
		number_of_atom_types_ = atom_types.getNumberOfTypes();
		
		// allocate two onedimensional fields for the two parameters
		// and a two dimensional field of boolean variables.
		// we might have to delete old stuff lying around
		if (k_)
			delete [] k_;
		if (r0_)
			delete [] r0_;
		if (is_defined_)
			delete [] is_defined_;
		if (comment_)
			delete [] comment_;

		k_  = new float[number_of_atom_types_ * number_of_atom_types_];
		r0_ = new float[number_of_atom_types_ * number_of_atom_types_];
		is_defined_ = new bool[number_of_atom_types_ * number_of_atom_types_];
		comment_ = new String[number_of_atom_types_ * number_of_atom_types_];

		for (i = 0; i < number_of_atom_types_ * number_of_atom_types_; i++) 
		{
			is_defined_[i] = false;
		}

		StringHashMap<Index>::Iterator it;

		// determine the factor to convert the parameters to the standard units used
		// as a default, energies are assumend to be in kJ/mol and distances in Angstrom
		double factor_k = 1.0;
		double factor_r0 = 1.0;
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
		
		if (options.has("unit_r0"))
		{
			if (options["unit_r0"] == "pm")
			{
				factor_r0 = 0.1;
			}
		}	
		

		Atom::Type		type_I;
		Atom::Type		type_J;
		String				type_name_I;
		String				type_name_J;
		String				key;
		Index					index = 0;

		for (it = section_entries_.begin(); !(it == section_entries_.end()); ++it)
		{
			key = (*it).first;
			if ((key.size() > 0) && (key.find_first_of(" ", 0) > 0)) 
			{
				type_name_I = key.before(" ", 0);
				type_name_J = key.after(" ", 0);
				if ((atom_types.hasType(type_name_I)) && (atom_types.hasType(type_name_J))) 
				{
					type_I = atom_types.getType(type_name_I);
					type_J = atom_types.getType(type_name_J);
					index = (Index)((type_I * number_of_atom_types_) + type_J);
					is_defined_[index] = true;
					k_ [index] = getValue(key, "k").toFloat() * factor_k;
					r0_ [index] = getValue(key, "r0").toFloat() * factor_r0;
					comment_[index] = getValue(key, "comment");
					index = (Index)(type_I + number_of_atom_types_ * type_J);
					is_defined_[index] = true;
					k_ [index] = getValue(key, "k").toFloat() * factor_k;
					r0_ [index] = getValue(key, "r0").toFloat() * factor_r0;	
					comment_[index] = getValue(key, "comment");
				}
			}
		}

		return true;
	}
	
	bool QuadraticBondStretch::exportParmFile(File& outfile) const
	{
		if (!force_field_parameters_)
			return false;
		
		// get the atom_types
		AtomTypes& atom_types = force_field_parameters_->getAtomTypes();
		
		// Amber requires kcal/mol and Angstroem
		double factor_k = Constants::CAL_PER_JOULE;
	
		// a string buffer for snprintf
		char buffer[1024];

		// the first line has to be handled differently
		bool first_line = true;

		for (Size i=1; i < number_of_atom_types_; i++)
		{	
			String atom_A =  atom_types.getTypeName(i);

			for (Size j=i; j < number_of_atom_types_; j++)
			{
				Size index = (i*number_of_atom_types_)+j;
				if (is_defined_[index])
				{	
					String atom_B = atom_types.getTypeName(j);

					if (first_line)
					{	
						// emulate the format: C -CA  469.0    1.409       JCC,7,(1986),230; TYR
						snprintf(buffer, 1024, "%-2s-%-2s  %-3.1f    %-1.4f     ! %s", 
										atom_A.c_str(), atom_B.c_str(), 
										(k_[index] * factor_k), r0_[index], 
										comment_[index].c_str()); 
						first_line = false;
					}
					else
					{
						// emulate the format: C -CA  469.0    1.409       JCC,7,(1986),230; TYR
						snprintf(buffer, 1024, "%-2s-%-2s  %-3.1f    %-1.4f       %s", 
										atom_A.c_str(), atom_B.c_str(), 
										(k_[index] * factor_k), r0_[index], 
										comment_[index].c_str()); 
					}
					outfile << buffer << endl;
				}
			}
		}
		outfile << endl;
		return true;
	}

	bool QuadraticBondStretch::hasParameters(Atom::Type I, Atom::Type J) const 
	{
		if ((I < 0) || ((Size)I >= number_of_atom_types_))
		{
			return false;
		}

		if ((J < 0) || ((Size)J >= number_of_atom_types_))
		{
			return false;
		}

		return is_defined_[I * number_of_atom_types_ + J];
	}


	QuadraticBondStretch::Values QuadraticBondStretch::getParameters
		(Atom::Type I, Atom::Type J) const 
	{
		QuadraticBondStretch::Values parameters;
		assignParameters(parameters, I, J);
		return parameters;
	}


	bool QuadraticBondStretch::assignParameters
		(QuadraticBondStretch::Values& parameters,
		 Atom::Type I, Atom::Type J) const 
	{
		if (hasParameters(I, J)) 
		{
			parameters.k = k_[I * number_of_atom_types_ + J];
			parameters.r0 = r0_[I * number_of_atom_types_ + J];

			return true;
		}

		return false;
	}
	 
} // namespace BALL
