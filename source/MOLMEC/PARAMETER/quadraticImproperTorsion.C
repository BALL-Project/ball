// $Id: quadraticImproperTorsion.C,v 1.1 2000/02/06 19:59:22 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/quadraticImproperTorsion.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	FFPSQuadraticImproperTorsion::FFPSQuadraticImproperTorsion()
		:	FFParameterSection(),
			torsions_()
	{
	}

	FFPSQuadraticImproperTorsion::~FFPSQuadraticImproperTorsion()
	{
		destroy();
	}

	void FFPSQuadraticImproperTorsion::destroy() 
	{
		FFParameterSection::destroy();
	}

	bool FFPSQuadraticImproperTorsion::extractSection(ForceFieldParameters& parameters, const String& section_name)
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// extract the section information
		if (!FFParameterSection::extractSection(parameters, section_name))
		{
			Log.level(LogStream::ERROR) << "Could not find section " 
				<< section_name << " in parameter file!" << endl;
			return false;
		}
		
		// check whether all variables we need are defined, terminate otherwise
		if (!hasVariable("phase") || !hasVariable("k"))
		{
			Log.level(LogStream::ERROR) << "QuadraticImproperTorsion section (" << section_name 
				<< ") needs columns phase and k!" << endl;
			return false;
		}

		// build a two dimensional array of the atom types
		// loop variable
		const FFPSAtomTypes&	atom_types = parameters.getAtomTypes();
		number_of_atom_types_ = atom_types.getNumberOfTypes();

		// clear all old torsions
		//	- torsions_ is a vector containing Values objects
		//  - torsion_hash_map_ hashes the product of the atom types (I + number_of_atom_types_ * J +...)
		//		to a the index in torsions_
		torsions_.clear();
		torsion_hash_map_.clear();
		
		// determine the units of the phase and potential wall
		// (if given in options)
		float factor_phase = 1.0;
		if (options.has("unit_phase"))
		{
			if (options["unit_phase"] == "rad")
			{	
				factor_phase = 180.0 / Constants::PI;
			}
		}

		float factor_k = 1.0;
		if (options.has("unit_k"))
		{
			if (options["unit_k"] == "kcal/mol")
			{
				factor_k = Constants::JOULE_PER_CAL;
			}
		}

		Atom::Type		type_I;
		Atom::Type		type_J;
		Atom::Type		type_K;
		Atom::Type		type_L;
		String				key;
		String				fields[5];

		StringHashMap<Index>::Iterator it;
		for (it = section_entries_.begin(); it != section_entries_.end(); ++it)
		{
			key = it->first;
			if (key.split(fields, 4) == 4)
			{
				// determine all atom types
				type_I = atom_types.getType(fields[0]);
				type_J = atom_types.getType(fields[1]);
				type_K = atom_types.getType(fields[2]);
				type_L = atom_types.getType(fields[3]);

				// create a new torsion and store 
				// it in the vector of torsions
				Size array_idx = torsions_.size();	
				Values v;
				v.phase = getValue(key, "phase").toFloat() * factor_phase;
				v.k = getValue(key, "k").toFloat() * factor_k;
				torsions_.push_back(v);
						
				// insert the array index and the atom type key into the
				// hash map

				// calculate a unique number for each possible combination of
				// atom types and use it to hash the torsion parameters	
				Size index = type_I + type_J * number_of_atom_types_ 
									 + type_K * number_of_atom_types_ * number_of_atom_types_ 
									 + type_L * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
				torsion_hash_map_.insert(pair<Size, Size>(index, array_idx));
			} else {
				Log.level(LogStream::ERROR) << "Could not interpret key " << key << endl;
			}
		}

		return true;
	}


	bool FFPSQuadraticImproperTorsion::hasParameters
		(Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const 
	{
		if ((I < 0) || ((Size)I >= number_of_atom_types_))
		{
			return false;
		}

		if ((J < 0) || ((Size)J >= number_of_atom_types_))
		{
			return false;
		}

		if ((K < 0) || ((Size)K >= number_of_atom_types_))
		{
			return false;
		}

		if ((L < 0) || ((Size)L >= number_of_atom_types_))
		{
			return false;
		}

		// calculate the key for this combination of atom types
		Size index = I + number_of_atom_types_ * J 
							 + K * number_of_atom_types_ * number_of_atom_types_
							 + L * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;

		// and look it up in the hash table
		bool result = torsion_hash_map_.has(index);

		// now, check for wildcards at the inner positions
		if (!result)
		{
			index = I + number_of_atom_types_ * Atom::ANY_TYPE 
							 + Atom::ANY_TYPE * number_of_atom_types_ * number_of_atom_types_
							 + L * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		return result;
	}

	FFPSQuadraticImproperTorsion::Values FFPSQuadraticImproperTorsion::getParameters
		(Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const 
	{
		FFPSQuadraticImproperTorsion::Values parameters;
		assignParameters(parameters, I, J, K, L);
		return parameters;
	}


	bool FFPSQuadraticImproperTorsion::assignParameters
		(FFPSQuadraticImproperTorsion::Values& parameters,
		 Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const 
	{
		// calculate the key for this combination of atom types
		Size index = I + number_of_atom_types_ * J 
							 + K * number_of_atom_types_ * number_of_atom_types_
							 + L * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;

		// and look it up in the hash table
		bool result = torsion_hash_map_.has(index);

		// now, check for wildcards at the inner positions
		if (!result)
		{
			index = I + number_of_atom_types_ * Atom::ANY_TYPE 
							 + Atom::ANY_TYPE * number_of_atom_types_ * number_of_atom_types_
							 + L * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		// and look it up in the hash table
		if (result)
		{
			parameters = torsions_[torsion_hash_map_[index]];
		}

		return result;
	}
	 
} // namespace BALL
