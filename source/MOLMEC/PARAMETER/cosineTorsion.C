// $Id: cosineTorsion.C,v 1.6 2001/05/17 01:30:55 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/cosineTorsion.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	CosineTorsion::CosineTorsion()
		:	ParameterSection(),
			torsions_()
	{
	}

	CosineTorsion::~CosineTorsion() throw()
	{
		clear();
	}

	void CosineTorsion::clear() throw()
	{
		ParameterSection::clear();
	}

	bool CosineTorsion::extractSection(Parameters& parameters, const String& section_name)
	{
		return ParameterSection::extractSection(parameters, section_name);
	}

	bool CosineTorsion::extractSection(ForceFieldParameters& parameters, const String& section_name)
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// extract the section information
		if (!ParameterSection::extractSection(parameters, section_name))
		{
			Log.error() << "CosineTorison::extractSection: Could not find section " 
				<< section_name << " in parameter file!" << endl;
			return false;
		}
		
		// check whether all variables we need are defined, terminate otherwise
		if (!hasVariable("div") || !hasVariable("V")
				|| !hasVariable("phi0") || !hasVariable("f"))
		{
			Log.error() << "CosineTorsion::extractSection: CosineTorsion section (" << section_name 
				<< ") needs columns div, V, phi0, and f!" << endl;
			return false;
		}

		// build a two dimensional array of the atom types
		// loop variable
		Size	i;
		const AtomTypes&	atom_types = parameters.getAtomTypes();
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

		float factor_V = 1.0;
		if (options.has("unit_V"))
		{
			if (options["unit_V"] == "kcal/mol")
			{
				factor_V = Constants::JOULE_PER_CAL;
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
			if (key.split(fields, 5) == 5)
			{
				// the first of line for each torsion has to contain N as last part of the key.	
				// this line only contains the number of torsion terms in "div"
				if (fields[4] == "N")
				{
					// determine all atom types
					type_I = atom_types.getType(fields[0]);
					type_J = atom_types.getType(fields[1]);
					type_K = atom_types.getType(fields[2]);
					type_L = atom_types.getType(fields[3]);

					// retrieve the number of torsion terms
					Size n = getValue(key, "div").toUnsignedInt();
					if ((n < 1) || (n > 3))
					{
						Log.error() << "CosineTorsion::extractSection: wrong number of torsion of torsion terms for "
							<< key << ": " << n << endl;
					} else {

						// create a new torsion and store 
						// it in the vector of torsions
						Size array_idx = (Size)torsions_.size();	
						torsions_.push_back(Values(n));
						
						// try to find the torsion terms
						for (i = 0; i < n; i++)
						{
							// calculate the correct key: "I J K L <number>"
							String term_key(key, 0, (Size)key.size() - 1);
							term_key += (String)(i + 1);
							
							// lookup the corresponding entry
							torsions_[array_idx].values[i].n			= getValue(term_key, "div").toFloat();
							torsions_[array_idx].values[i].phase	= getValue(term_key, "phi0").toFloat() * factor_phase;
							torsions_[array_idx].values[i].f			= getValue(term_key, "f").toFloat();
							torsions_[array_idx].values[i].V			= getValue(term_key, "V").toFloat() * factor_V;
						}
						
						// insert the array index and the atom type key into the
						// hash map

						// calculate a unique number for each possible combination of
						// atom types and use it to hash the torsion parameters	
						Size index = type_I + type_J * number_of_atom_types_ 
											 + type_K * number_of_atom_types_ * number_of_atom_types_ 
											 + type_L * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
						torsion_hash_map_.insert(pair<Size, Size>(index, array_idx));
					}
				}
			} else {
				Log.error() << "CosineTorsion::extractSection: could not interpret key " << key << endl;
			}
		}

		return true;
	}


	bool CosineTorsion::hasParameters
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

		// check for the reverse order of atoms, too
		if (!result)
		{
			index = L + number_of_atom_types_ * K 
							 + J * number_of_atom_types_ * number_of_atom_types_
							 + I * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		// now, check for wildcards at the outer positions
		if (!result)
		{
			index = Atom::ANY_TYPE + number_of_atom_types_ * J 
							 + K * number_of_atom_types_ * number_of_atom_types_
							 + Atom::ANY_TYPE * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		// ... and wildcards and reverse order for the inner atoms
		if (!result)
		{
			index = Atom::ANY_TYPE + number_of_atom_types_ * K 
							 + J * number_of_atom_types_ * number_of_atom_types_
							 + Atom::ANY_TYPE * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		// finally, check for wildcards at the first two positions
		// (for improper torsions)
		if (!result)
		{
			index = Atom::ANY_TYPE + number_of_atom_types_ * Atom::ANY_TYPE 
							 + K * number_of_atom_types_ * number_of_atom_types_
							 + L * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		return result;
	}

	CosineTorsion::Values CosineTorsion::getParameters
		(Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const 
	{
		CosineTorsion::Values parameters;
		assignParameters(parameters, I, J, K, L);
		return parameters;
	}


	bool CosineTorsion::assignParameters
		(CosineTorsion::Values& parameters,
		 Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const 
	{
		// calculate the key for this combination of atom types
		Size index = I + number_of_atom_types_ * J 
							 + K * number_of_atom_types_ * number_of_atom_types_
							 + L * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;

		// and look it up in the hash table
		bool result = torsion_hash_map_.has(index);

		// check for the reverse order of atoms, too
		if (!result)
		{
			index = L + number_of_atom_types_ * K 
						+ J * number_of_atom_types_ * number_of_atom_types_
						+ I * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		// now, check for wildcards at the outer positions
		if (!result)
		{
			index = Atom::ANY_TYPE + number_of_atom_types_ * J 
							 + K * number_of_atom_types_ * number_of_atom_types_
							 + Atom::ANY_TYPE * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		// ... and wildcards and reverse order for the inner atoms
		if (!result)
		{
			index = Atom::ANY_TYPE + number_of_atom_types_ * K 
							 + J * number_of_atom_types_ * number_of_atom_types_
							 + Atom::ANY_TYPE * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		// finally, check for wildcards at the first two positions
		// (for improper torsions)
		if (!result)
		{
			index = Atom::ANY_TYPE + number_of_atom_types_ * Atom::ANY_TYPE 
							 + K * number_of_atom_types_ * number_of_atom_types_
							 + L * number_of_atom_types_ * number_of_atom_types_ * number_of_atom_types_;
			result = torsion_hash_map_.has(index);
		}

		// and look it up in the hash table
		if (result)
		{
			parameters.set(torsions_[torsion_hash_map_[index]]);
		}

		return result;
	}
	 
} // namespace BALL
