// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: potential1210.C,v 1.10 2005/01/18 21:33:52 amoll Exp $
//

#include <BALL/MOLMEC/PARAMETER/potential1210.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	Potential1210::Potential1210() 
		:	ParameterSection(),
			number_of_atom_types_(0),
			A_(0),
			B_(0),
			is_defined_(0),	
			comment_(),
			force_field_parameters_(0)
	{
	}


	Potential1210::Potential1210(const Potential1210& pot1210) 
		:	ParameterSection(pot1210),
			number_of_atom_types_(pot1210.number_of_atom_types_),
			A_(pot1210.A_),
			B_(pot1210.B_),
			is_defined_(pot1210.is_defined_),	
			comment_(pot1210.comment_),
			force_field_parameters_(pot1210.force_field_parameters_)
	{
	}


	Potential1210::~Potential1210() 
	{
		clear();
		
		valid_ = false;
	}


	void Potential1210::clear() 
	{
		// clear parameter fields
		number_of_atom_types_ = 0;
		A_.clear();
		B_.clear();
		is_defined_.clear();
		comment_.clear();
		force_field_parameters_ = 0;
		ParameterSection::clear();
	}


	const Potential1210& Potential1210::operator = (const Potential1210& pot1210) 
		
	{
		ParameterSection::operator = (pot1210);

		number_of_atom_types_ = pot1210.number_of_atom_types_;
		A_ = pot1210.A_;
		B_ = pot1210.B_;
		is_defined_ = pot1210.is_defined_;

		return *this;
	}


	bool Potential1210::operator == (const Potential1210& pot1210) const 
	{
		return (ParameterSection::operator == (pot1210)
			&& (number_of_atom_types_ == pot1210.number_of_atom_types_)
			&& (A_ == pot1210.A_)
			&& (B_ == pot1210.B_)
			&& (is_defined_ == pot1210.is_defined_));
	}


	bool Potential1210::extractSection
		(Parameters& parameters, const String& section_name) 
	{
		return ParameterSection::extractSection(parameters, section_name);
	}


	bool Potential1210::extractSection
		(ForceFieldParameters& parameters, const String& section_name) 
	{
		// clear the fields first
		clear();

		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}

		// extract the basis information
		ParameterSection::extractSection(parameters, section_name);
		
		// check whether all variables we need are defined, terminate otherwise
		if (!hasVariable("A") || !hasVariable("B"))
		{
			return false;
		}

		// build a two dimensional array of the atom types
		// loop variable
		Size	i;

		AtomTypes& atom_types = parameters.getAtomTypes();
		number_of_atom_types_ = atom_types.getNumberOfTypes();
		
		// store a pointer to the corresponding atom types
		force_field_parameters_ = &parameters;

		// allocate two onedimensional fields for the two parameters
		A_.resize(number_of_atom_types_ * number_of_atom_types_);
		B_.resize(number_of_atom_types_ * number_of_atom_types_);
		is_defined_.resize(number_of_atom_types_ * number_of_atom_types_);
		comment_.resize(number_of_atom_types_ * number_of_atom_types_);

		for (i = 0; i < number_of_atom_types_ * number_of_atom_types_; i++) 
		{
			is_defined_[i] = false;
		}

		StringHashMap<Index>::Iterator it;

		// determine the factor to convert the parameters to the standard units used
		// as a default, energies are assumend to be in kJ/mol and distances in Angstrom
		double factor_A = 1.0;
		double factor_B = 1.0;
		if (options.has("unit_A"))
		{
			if (options["unit_A"] == "kcal/mol*A^12")
			{
				factor_A = Constants::JOULE_PER_CAL;
			} 
			else 
			{
				Log.warn() << "unknown unit for parameter A: " << options["unit_A"] << endl;
			}
		}	
		
		if (options.has("unit_B"))
		{
			if (options["unit_B"] == "kcal/mol*A^10")
			{
				factor_B = Constants::JOULE_PER_CAL;
			} 
			else 
			{
				Log.warn() << "unknown unit for parameter B: " << options["unit_B"] << endl;
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
					index = (Index)(type_I * number_of_atom_types_ + type_J);
					is_defined_[index] = true;
					A_[index] = getValue(key, "A").toFloat() * factor_A;
					B_[index] = getValue(key, "B").toFloat() * factor_B;
					comment_[index] = getValue(key, "comment");
					index = (Index)(type_I + number_of_atom_types_ * type_J);
					is_defined_[index] = true;
					A_[index] = getValue(key, "A").toFloat() * factor_A;
					B_[index] = getValue(key, "B").toFloat() * factor_B;
					comment_[index] = getValue(key, "comment");
				}
			}
		}

		return true;
	}


	bool Potential1210::hasParameters(Atom::Type I, Atom::Type J) const
		
	{
		if ((I < 0) || ((Size)I >= number_of_atom_types_))
			return false;

		if ((J < 0) || ((Size)J >= number_of_atom_types_))
			return false;

		return is_defined_[I * number_of_atom_types_ + J];
	}


	Potential1210::Values Potential1210::getParameters
		(Atom::Type I, Atom::Type J) const 
	{
		Potential1210::Values parameters;
		assignParameters(parameters, I, J);
		return parameters;
	}


	bool Potential1210::assignParameters
		(Potential1210::Values& parameters,
		 Atom::Type I, Atom::Type J) const 
	{
		if (hasParameters(I, J)) 
		{
			parameters.A = A_[I * number_of_atom_types_ + J];
			parameters.B = B_[I * number_of_atom_types_ + J];

			return true;
		}

		return false;
	}

	bool  Potential1210::exportParmFile(File& outfile) const
	{
		if (!force_field_parameters_)
			return false;

		AtomTypes& atom_types = force_field_parameters_->getAtomTypes();

		String atom_I;
		String atom_J;
		
		double factor_A = Constants::CAL_PER_JOULE;
		double factor_B = Constants::CAL_PER_JOULE;
		
		Index	 index     = 0;

		// a string buffer for snprintf
		char buffer[1024];

		for (Size i=0; i < number_of_atom_types_; i++)
		{	
			atom_I = atom_types.getTypeName(i);
			atom_I = (atom_I =="?") ? "X " : atom_I;

			for (Size j=i; j < number_of_atom_types_; j++)
			{
				index = (Index)(i* number_of_atom_types_ + j);
				if (is_defined_[index])
				{
					atom_J = atom_types.getTypeName(j);
					atom_J = (atom_J=="?") ? "X " : atom_J;
					
			  	//emulate format: HW  OW  0000.     0000.                                4.  flag for fast water
					snprintf(buffer, 1024, "  %-2s  %-2s   %4.1f     %4.1f                       %s",
										atom_I.c_str(), atom_J.c_str(), 
										A_[index] * factor_A,  B_[index] * factor_B, 
										comment_[index].c_str());

					outfile << buffer << endl;
				}
			}
		}
		
		// Terminate by blank card
		outfile << endl;
		return true;
	}

	 
} // namespace BALL
