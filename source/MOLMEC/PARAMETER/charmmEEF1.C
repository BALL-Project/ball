// $Id: charmmEEF1.C,v 1.1 2000/01/30 12:58:12 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/charmmEEF1.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	FFPSCharmmEEF1::FFPSCharmmEEF1()
		:	FFParameterSection(),
			V_(0),
			dG_ref_(0),
			dG_free_(0),
			dH_ref_(0),
			Cp_ref_(0),
			sig_w_(0),
			is_defined_(0)
	{
	}

	FFPSCharmmEEF1::~FFPSCharmmEEF1()
	{
		destroy();
	}

	void FFPSCharmmEEF1::destroy() 
	{
		// clear allocated parameter fields
		delete [] V_;
		delete [] dG_free_;
		delete [] dG_ref_;
		delete [] Cp_ref_;
		delete [] dH_ref_;
		delete [] sig_w_;

		FFParameterSection::destroy();
	}

	bool FFPSCharmmEEF1::extractSection(ForceFieldParameters& parameters, const String& section_name)
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// extract the basis information
		FFParameterSection::extractSection(parameters, section_name);

		// check whether all variables we need are defined, terminate otherwise
		if (!(hasVariable("V") && !hasVariable("dG_free")
				&& hasVariable("dH_ref") && hasVariable("Cp_ref") && hasVariable("sig_w")))
		{
			Log.level(LogStream::ERROR) << "CHARMm EEF1 parameter section requires five variable columns:"		
				<< "V/dG_ref/dG_free/dH_ref/Cp_ref/sig_w" << endl;

			return false;

		}

		// determine the number of atom types
		const FFPSAtomTypes&	atom_types = parameters.getAtomTypes();
		number_of_atom_types_ = atom_types.getNumberOfTypes();
		
		// allocate two onedimensional fields for the two parameters
		V_					= new float[number_of_atom_types_];
		dG_ref_			= new float[number_of_atom_types_];
		dG_free_		= new float[number_of_atom_types_];
		dH_ref_			= new float[number_of_atom_types_];
		Cp_ref_			= new float[number_of_atom_types_];
		sig_w_			= new float[number_of_atom_types_];
		is_defined_ = new bool[number_of_atom_types_];

		// loop variable
		Size	i;

		// clear the field of defined atom_types
		for (i = 0; i < number_of_atom_types_; i++) 
		{
			is_defined_[i] = false;
		}

		// the indices of the colums containing the values
		Size index_V				= getColumnIndex("V");
		Size index_dG_ref		= getColumnIndex("dG_ref");
		Size index_dG_free	= getColumnIndex("dG_free");
		Size index_dH_ref		= getColumnIndex("dH_ref");
		Size index_Cp_ref		= getColumnIndex("Cp_ref");
		Size index_sig_w		= getColumnIndex("sig_w");

		// try to identify the units of all parameters
		// and set the conversion factors
		double factor_dG_ref = 1.0;
		if (options.has("unit_dG_ref"))
		{
			if (options["unit_dG_ref"] == "kcal/mol")
			{
				factor_dG_ref = Constants::JOULE_PER_CAL;
			}
			else if (options["unit_dG_ref"] == "cal/mol")
			{
				factor_dG_ref = Constants::JOULE_PER_CAL * 0.001;					
			}
			else if (options["unit_dG_ref"] == "J/mol")
			{
				factor_dG_ref = 0.001;
			}
			else if (options["unit_dG_ref"] != "kJ/mol")
			{
				Log.error() << "FFPSCharmmEEF1: unknown unit for parameter column dG_ref: " 
										<< options["unit_dG_ref"] << ". Assuming kJ/mol as default unit." << endl;
			}
		}

		double factor_dG_free = 1.0;
		if (options.has("unit_dG_free"))
		{
			if (options["unit_dG_free"] == "kcal/mol")
			{
				factor_dG_free = Constants::JOULE_PER_CAL;
			}
			else if (options["unit_dG_free"] == "cal/mol")
			{
				factor_dG_free = Constants::JOULE_PER_CAL * 0.001;					
			}
			else if (options["unit_dG_free"] == "J/mol")
			{
				factor_dG_free = 0.001;
			}
			else if (options["unit_dG_free"] != "kJ/mol")
			{
				Log.error() << "FFPSCharmmEEF1: unknown unit for parameter column dG_free: " 
										<< options["unit_dG_free"] << ". Assuming kJ/mol as default unit." << endl;
			}
		}

		double factor_dH_ref = 1.0;
		if (options.has("unit_dH_ref"))
		{
			if (options["unit_dH_ref"] == "kcal/mol")
			{
				factor_dH_ref = Constants::JOULE_PER_CAL;
			}
			else if (options["unit_dH_ref"] == "cal/mol")
			{
				factor_dH_ref = Constants::JOULE_PER_CAL * 0.001;					
			}
			else if (options["unit_dH_ref"] == "J/mol")
			{
				factor_dH_ref = 0.001;
			}
			else if (options["unit_dH_ref"] != "kJ/mol")
			{
				Log.error() << "FFPSCharmmEEF1: unknown unit for parameter column dH_ref: " 
										<< options["unit_dH_ref"] << ". Assuming kJ/mol as default unit." << endl;
			}
		}

		double factor_Cp_ref = 1.0;
		if (options.has("unit_Cp_ref"))
		{
			if (options["unit_Cp_ref"] == "kcal/mol")
			{
				factor_Cp_ref = Constants::JOULE_PER_CAL;
			}
			else if (options["unit_Cp_ref"] == "cal/mol")
			{
				factor_Cp_ref = Constants::JOULE_PER_CAL * 0.001;					
			}
			else if (options["unit_Cp_ref"] == "J/mol")
			{
				factor_Cp_ref = 0.001;
			}
			else if (options["unit_Cp_ref"] != "kJ/mol")
			{
				Log.error() << "FFPSCharmmEEF1: unknown unit for parameter column Cp_ref: " 
										<< options["unit_Cp_ref"] << ". Assuming kJ/mol as default unit." << endl;
			}
		}

		double factor_sig_w = 1.0;
		if (options.has("unit_sig_w"))
		{
			if (options["unit_sig_w"] == "pm")
			{
				factor_sig_w = 0.1;
			}
			else if (options["unit_sig_w"] != "A"
							 && options["unit_sig_w"] != "Angstrom")
			{
				Log.error() << "FFPSCharmmEEF1: unknown unit for parameter column sig_w: " 
										<< options["unit_sig_w"] << ". Assuming Angstrom as default unit." << endl;
			}


		}

		double factor_V = 1.0;
		if (options.has("unit_sig_w"))
		{
			if (options["unit_V"] != "A^3"
							 && options["unit_V"] != "Angstrom^3")
			{
				Log.error() << "FFPSCharmmEEF1: unknown unit for parameter column V: " 
										<< options["unit_V"] << ". Assuming Angstrom^3 as default unit." << endl;
			}
		}

		String			key;
		for (i = 1; i <= getNumberOfKeys(); ++i)
		{
			// get the key
			key = getKey(i);
			if (atom_types.hasType(key))
			{
				// retrieve the values
				V_[i]				= getValue(i, index_V).toFloat() * factor_V;
				dG_ref_[i]	= getValue(i, index_dG_ref).toFloat() * factor_dG_ref;
				dG_free_[i] = getValue(i, index_dG_free).toFloat() * factor_dG_free;
				dH_ref_[i]	= getValue(i, index_dH_ref).toFloat() * factor_dH_ref;
				Cp_ref_[i]	= getValue(i, index_Cp_ref).toFloat() * factor_Cp_ref;
				sig_w_[i]		= getValue(i, index_sig_w).toFloat() * factor_sig_w;
				is_defined_[i] = true;

			} else {
				Log.level(LogStream::WARNING) << "unknown atom type in Charmm EEF1 parameters: " << key << "   i = " << i << endl;
			}
		}

		return true;
	}


	bool FFPSCharmmEEF1::hasParameters(Atom::Type I) const 
	{
		if ((I < 0) && ((Size)I >= number_of_atom_types_))
		{
			return false;
		}

		return (is_defined_[I]);
	}


	FFPSCharmmEEF1::Values FFPSCharmmEEF1::getParameters
		(Atom::Type I) const 
	{
		FFPSCharmmEEF1::Values parameters;
		assignParameters(parameters, I);
		return parameters;
	}


	bool FFPSCharmmEEF1::assignParameters
		(FFPSCharmmEEF1::Values& parameters, Atom::Type I) const 
	{
		if (hasParameters(I)) 
		{
			parameters.V				= V_[I];
			parameters.dG_ref		= dG_ref_[I];
			parameters.dG_free	= dG_free_[I];
			parameters.r_min		= sig_w_[I];
			parameters.dH_ref		= dH_ref_[I];
			parameters.Cp_ref		= Cp_ref_[I];

			return true;
		}

		return false;
	}
	 
} // namespace BALL
