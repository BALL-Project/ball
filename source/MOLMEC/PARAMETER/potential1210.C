// $Id: potential1210.C,v 1.8 2001/12/14 01:18:12 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/potential1210.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	Potential1210::Potential1210() throw()
		:	ParameterSection(),
			number_of_atom_types_(0),
			A_(0),
			B_(0),
			is_defined_(0),
			names_(0)
	{
	}


	Potential1210::Potential1210(const Potential1210& pot1210) throw()
		:	ParameterSection(pot1210),
			number_of_atom_types_(pot1210.number_of_atom_types_),
			A_(pot1210.A_),
			B_(pot1210.B_),
			is_defined_(pot1210.is_defined_),
			names_(pot1210.names_)
	{
	}


	Potential1210::~Potential1210() throw()
	{
		clear();
		
		valid_ = false;
	}


	void Potential1210::clear() throw()
	{
		// clear parameter fields
		number_of_atom_types_ = 0;
		A_.clear();
		B_.clear();
		is_defined_.clear();
		names_.clear();

		ParameterSection::clear();
	}


	const Potential1210& Potential1210::operator = (const Potential1210& pot1210) 
		throw()
	{
		ParameterSection::operator = (pot1210);

		number_of_atom_types_ = pot1210.number_of_atom_types_;
		A_ = pot1210.A_;
		B_ = pot1210.B_;
		is_defined_ = pot1210.is_defined_;
		names_ = pot1210.names_;

		return *this;
	}


	bool Potential1210::operator == (const Potential1210& pot1210) const throw()
	{
		return (ParameterSection::operator == (pot1210)
			&& (number_of_atom_types_ == pot1210.number_of_atom_types_)
			&& (A_ == pot1210.A_)
			&& (B_ == pot1210.B_)
			&& (is_defined_ == pot1210.is_defined_)
			&& (names_ == pot1210.names_));
	}


	bool Potential1210::extractSection
		(Parameters& parameters, const String& section_name) throw()
	{
		return ParameterSection::extractSection(parameters, section_name);
	}


	bool Potential1210::extractSection
		(ForceFieldParameters& parameters, const String& section_name) throw()
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
		
		// allocate two onedimensional fields for the two parameters
		A_.resize(number_of_atom_types_ * number_of_atom_types_);
		B_.resize(number_of_atom_types_ * number_of_atom_types_);
		is_defined_.resize(number_of_atom_types_ * number_of_atom_types_);

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
				Log.level(LogStream::WARNING) << "unknown unit for parameter A: " << options["unit_A"] << endl;
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
				Log.level(LogStream::WARNING) << "unknown unit for parameter B: " << options["unit_B"] << endl;
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
					A_ [index] = getValue(key, "A").toFloat() * factor_A;
					B_ [index] = getValue(key, "B").toFloat() * factor_B;
					index = (Index)(type_I + number_of_atom_types_ * type_J);
					is_defined_[index] = true;
					A_ [index] = getValue(key, "A").toFloat() * factor_A;
					B_ [index] = getValue(key, "B").toFloat() * factor_B;
				}
			}
		}

		return true;
	}


	bool Potential1210::hasParameters(Atom::Type I, Atom::Type J) const
		throw()
	{
		if ((I < 0) || ((Size)I >= number_of_atom_types_))
			return false;

		if ((J < 0) || ((Size)J >= number_of_atom_types_))
			return false;

		return is_defined_[I * number_of_atom_types_ + J];
	}


	Potential1210::Values Potential1210::getParameters
		(Atom::Type I, Atom::Type J) const throw()
	{
		Potential1210::Values parameters;
		assignParameters(parameters, I, J);
		return parameters;
	}


	bool Potential1210::assignParameters
		(Potential1210::Values& parameters,
		 Atom::Type I, Atom::Type J) const throw()
	{
		if (hasParameters(I, J)) 
		{
			parameters.A = A_[I * number_of_atom_types_ + J];
			parameters.B = B_[I * number_of_atom_types_ + J];

			return true;
		}

		return false;
	}

	 
} // namespace BALL
