// $Id: charmmEEF1.C,v 1.8 2001/07/11 00:10:47 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/charmmEEF1.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#include <string.h>

using namespace std;

namespace BALL 
{

	CharmmEEF1::CharmmEEF1() 
		throw()
		:	ParameterSection(),
			V_(0),
			dG_ref_(0),
			dG_free_(0),
			dH_ref_(0),
			Cp_ref_(0),
			sig_w_(0),
			R_min_(0),
			is_defined_(0)
	{
	}


	CharmmEEF1::CharmmEEF1(const CharmmEEF1& charmm_EEF1) throw()
		:	ParameterSection(charmm_EEF1),
			V_(0),
			dG_ref_(0),
			dG_free_(0),
			dH_ref_(0),
			Cp_ref_(0),
			sig_w_(0),
			R_min_(0),
			is_defined_(0)
	{
		// avoid allocation of zero-length blocks -- this is usually trouble...
		if (number_of_atom_types_ > 0)
		{
			// copy attributes
			number_of_atom_types_ = charmm_EEF1.number_of_atom_types_;

			V_ = new float [number_of_atom_types_];
			memcpy(V_, charmm_EEF1.V_, sizeof(float) * number_of_atom_types_);

			dG_ref_ = new float [number_of_atom_types_];
			memcpy(dG_ref_, charmm_EEF1.dG_ref_, sizeof(float) * number_of_atom_types_);

			dG_free_ = new float [number_of_atom_types_];
			memcpy(dG_free_, charmm_EEF1.dG_free_, sizeof(float) * number_of_atom_types_);

			dH_ref_ = new float [number_of_atom_types_];
			memcpy(dH_ref_, charmm_EEF1.dH_ref_, sizeof(float) * number_of_atom_types_);

			Cp_ref_ = new float [number_of_atom_types_];
			memcpy(Cp_ref_, charmm_EEF1.Cp_ref_, sizeof(float) * number_of_atom_types_);

			sig_w_ = new float [number_of_atom_types_];
			memcpy(sig_w_, charmm_EEF1.sig_w_, sizeof(float) * number_of_atom_types_);

			R_min_ = new float [number_of_atom_types_];
			memcpy(R_min_, charmm_EEF1.R_min_, sizeof(float) * number_of_atom_types_);
			
			is_defined_ = new bool [number_of_atom_types_];
			memcpy(is_defined_, charmm_EEF1.is_defined_, sizeof(bool) * number_of_atom_types_);
		}
	}


	CharmmEEF1::~CharmmEEF1()
		throw()
	{
		clear();

		valid_ = false;
	}
	

	void CharmmEEF1::clear()  
		throw()
	{
		// clear allocated parameter fields
		delete [] V_;
		V_ = 0;
		delete [] dG_free_;
		dG_free_ = 0;
		delete [] dG_ref_;
		dG_ref_ = 0;
		delete [] Cp_ref_;
		Cp_ref_ = 0;
		delete [] dH_ref_;
		dH_ref_ = 0;
		delete [] sig_w_;
		sig_w_ = 0;
		delete [] R_min_;
		R_min_ = 0;
		
		ParameterSection::clear();
	}


	bool CharmmEEF1::extractSection(Parameters& parameters, 
			const String& section_name) throw()
	{
		return ParameterSection::extractSection(parameters, section_name);
	}


	bool CharmmEEF1::extractSection(ForceFieldParameters& parameters, 
			const String& section_name) throw()
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// release previous contents
		clear();

		// extract the basis information
		ParameterSection::extractSection(parameters, section_name);

		// check whether all variables we need are defined, terminate otherwise
		if (!hasVariable("V") || !hasVariable("dG_free") || !hasVariable("R_min")
				|| !hasVariable("dH_ref") || !hasVariable("Cp_ref") || !hasVariable("sig_w"))
		{
			Log.error() << "CHARMm EEF1 parameter section requires six variable columns:"		
				<< "V, dG_ref, dG_free, dH_ref, Cp_ref, sig_w, and R_min." << endl;

			return false;

		}

		// determine the number of atom types
		const AtomTypes&	atom_types = parameters.getAtomTypes();
		number_of_atom_types_ = atom_types.getNumberOfTypes();
		
		// allocate two onedimensional fields for the two parameters
		V_					= new float[number_of_atom_types_];
		dG_ref_			= new float[number_of_atom_types_];
		dG_free_		= new float[number_of_atom_types_];
		dH_ref_			= new float[number_of_atom_types_];
		Cp_ref_			= new float[number_of_atom_types_];
		sig_w_			= new float[number_of_atom_types_];
		R_min_			= new float[number_of_atom_types_];
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
		Size index_R_min		= getColumnIndex("R_min");

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
				Log.error() << "CharmmEEF1: unknown unit for parameter column dG_ref: " 
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
				Log.error() << "CharmmEEF1: unknown unit for parameter column dG_free: " 
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
				Log.error() << "CharmmEEF1: unknown unit for parameter column dH_ref: " 
										<< options["unit_dH_ref"] << ". Assuming kJ/mol as default unit." << endl;
			}
		}

		double factor_Cp_ref = 1.0;
		if (options.has("unit_Cp_ref"))
		{
			if (options["unit_Cp_ref"] == "kcal/(molK)")
			{
				factor_Cp_ref = Constants::JOULE_PER_CAL;
			}
			else if (options["unit_Cp_ref"] == "cal/(molK)")
			{
				factor_Cp_ref = Constants::JOULE_PER_CAL * 0.001;					
			}
			else if (options["unit_Cp_ref"] == "J/(molK)")
			{
				factor_Cp_ref = 0.001;
			}
			else if (options["unit_Cp_ref"] != "kJ/(molK)")
			{
				Log.error() << "CharmmEEF1: unknown unit for parameter column Cp_ref: " 
										<< options["unit_Cp_ref"] << ". Assuming kJ/(molK) as default unit." << endl;
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
				Log.error() << "CharmmEEF1: unknown unit for parameter column sig_w: " 
										<< options["unit_sig_w"] << ". Assuming Angstrom as default unit." << endl;
			}
		}

		double factor_R_min = 1.0;
		if (options.has("unit_R_min"))
		{
			if (options["unit_R_min"] == "pm")
			{
				factor_R_min = 0.1;
			}
			else if (options["unit_R_min"] != "A"
							 && options["unit_R_min"] != "Angstrom")
			{
				Log.error() << "CharmmEEF1: unknown unit for parameter column R_min: " 
										<< options["unit_R_min"] << ". Assuming Angstrom as default unit." << endl;
			}
		}

		double factor_V = 1.0;
		if (options.has("unit_V"))
		{
			if (options["unit_V"] != "A^3"
							 && options["unit_V"] != "Angstrom^3")
			{
				Log.error() << "CharmmEEF1: unknown unit for parameter column V: " 
										<< options["unit_V"] << ". Assuming Angstrom^3 as default unit." << endl;
			}
		}
		
		String key;
		for (i = 0; i < getNumberOfKeys(); ++i)
		{	
			// get the key
			key = getKey(i);
			if (atom_types.hasType(key))
			{
				Position idx = atom_types.getType(key);
				// retrieve the values
				V_[idx]				= getValue(i, index_V).toFloat() * factor_V;
				dG_ref_[idx]	= getValue(i, index_dG_ref).toFloat() * factor_dG_ref;
				dG_free_[idx] = getValue(i, index_dG_free).toFloat() * factor_dG_free;
				dH_ref_[idx]	= getValue(i, index_dH_ref).toFloat() * factor_dH_ref;
				Cp_ref_[idx]	= getValue(i, index_Cp_ref).toFloat() * factor_Cp_ref;
				sig_w_[idx]		= getValue(i, index_sig_w).toFloat() * factor_sig_w;
				R_min_[idx]		= getValue(i, index_R_min).toFloat() * factor_R_min;
				is_defined_[idx] = true;
			} 
			else 
			{
				Log.warn() << "unknown atom type in Charmm EEF1 parameters: " << key << "   i = " << i << endl;
			}
		}

		return true;
	}


	bool CharmmEEF1::hasParameters(Atom::Type I) const 
		throw()
	{
		return ((I >= 0) && ((Size)I < number_of_atom_types_) && is_defined_[I]);
	}


	CharmmEEF1::Values CharmmEEF1::getParameters(Atom::Type I) const 
		throw()
	{
		CharmmEEF1::Values parameters;
		assignParameters(parameters, I);
		return parameters;
	}


	bool CharmmEEF1::assignParameters
		(CharmmEEF1::Values& parameters, Atom::Type I) const throw()
	{
		if (hasParameters(I)) 
		{
			parameters.V				= V_[I];
			parameters.dG_ref		= dG_ref_[I];
			parameters.dG_free	= dG_free_[I];
			parameters.sig_w		= sig_w_[I];
			parameters.r_min		= R_min_[I];
			parameters.dH_ref		= dH_ref_[I];
			parameters.Cp_ref		= Cp_ref_[I];
			parameters.Cp_ref		= Cp_ref_[I];

			return true;
		}

		return false;
	}


	const CharmmEEF1& CharmmEEF1::operator = (const CharmmEEF1& charmm_EEF1)
		throw()
	{
		// clear old contents
		clear();

		// assign base class contents
		ParameterSection::operator = (charmm_EEF1);

		// copy attributes
		number_of_atom_types_ = charmm_EEF1.number_of_atom_types_;

		V_ = new float [number_of_atom_types_];
		memcpy(V_, charmm_EEF1.V_, sizeof(float) * number_of_atom_types_);

		dG_ref_ = new float [number_of_atom_types_];
		memcpy(dG_ref_, charmm_EEF1.dG_ref_, sizeof(float) * number_of_atom_types_);

		dG_free_ = new float [number_of_atom_types_];
		memcpy(dG_free_, charmm_EEF1.dG_free_, sizeof(float) * number_of_atom_types_);

		dH_ref_ = new float [number_of_atom_types_];
		memcpy(dH_ref_, charmm_EEF1.dH_ref_, sizeof(float) * number_of_atom_types_);

		Cp_ref_ = new float [number_of_atom_types_];
		memcpy(Cp_ref_, charmm_EEF1.Cp_ref_, sizeof(float) * number_of_atom_types_);

		sig_w_ = new float [number_of_atom_types_];
		memcpy(sig_w_, charmm_EEF1.sig_w_, sizeof(float) * number_of_atom_types_);

		R_min_ = new float [number_of_atom_types_];
		memcpy(R_min_, charmm_EEF1.R_min_, sizeof(float) * number_of_atom_types_);
		
		is_defined_ = new bool [number_of_atom_types_];
		memcpy(is_defined_, charmm_EEF1.is_defined_, sizeof(bool) * number_of_atom_types_);

		return *this;
	}

	
	bool CharmmEEF1::operator == (const CharmmEEF1& charmm_EEF1) const
		throw()
	{
		if (!ParameterSection::operator == (charmm_EEF1))
		{
			return false;
		}
		else 
		{
			if (number_of_atom_types_ != charmm_EEF1.number_of_atom_types_)
			{
				return false;
			}
			else
			{
				for (Size i = 0; i < number_of_atom_types_; ++i)
				{
					if (V_ != charmm_EEF1.V_)
					{
						return false;
					}
					if (dG_ref_ != charmm_EEF1.dG_ref_)
					{
						return false;
					}
					if (dG_free_ != charmm_EEF1.dG_free_)
					{
						return false;
					}
					if (dH_ref_ != charmm_EEF1.dH_ref_)
					{
						return false;
					}
					if (Cp_ref_ != charmm_EEF1.Cp_ref_)
					{
						return false;
					}
					if (sig_w_ != charmm_EEF1.sig_w_)
					{
						return false;
					}
					if (R_min_ != charmm_EEF1.R_min_)
					{
						return false;
					}
				}
			}
		}
		return true;
	}
	 
} // namespace BALL
