// $Id: residueTorsions.C,v 1.3 2000/02/11 18:18:17 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/residueTorsions.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

using namespace std;

namespace BALL 
{

	FFPSResidueTorsions::FFPSResidueTorsions()
		:	FFParameterSection()
	{
	}

	FFPSResidueTorsions::~FFPSResidueTorsions()
	{
		destroy();
	}

	void FFPSResidueTorsions::destroy() 
	{
		FFParameterSection::destroy();
	}

	bool FFPSResidueTorsions::extractSection(ForceFieldParameters& parameters, const String& section_name)
	{
		// check whether the parameters are valid
		if (!parameters.isValid())
		{
			return false;
		}
		
		// extract the basis information
		FFParameterSection::extractSection(parameters, section_name);

		// iterate over all keys and construct the hash map of vectors
		for (Size i = 1; i <= getNumberOfKeys(); i++)
		{
			String key = getKey(i);
			String residue = key.getField(0);
			
			// do we have an vector for this residue?
			if (!torsions_.has(residue))
			{	
				// create the vector
				torsions_.insert(pair<String, vector<ResidueTorsion> >(residue, vector<ResidueTorsion>()));
			} 

			// insert the torsions 
			torsions_[residue].push_back(ResidueTorsion(residue, key.getField(1), key.getField(2), key.getField(3), key.getField(4)));
			all_torsions_.insert(key);
		}

		return true;
	}

	Size FFPSResidueTorsions::getNumberOfResidueTorsions(const String& residue_name) const
	{
		// if we know this residue...
		if (torsions_.has(residue_name))
		{
			// return the size of the corresponding array
			return torsions_[residue_name].size();
		}
		
		return 0;
	}


	bool FFPSResidueTorsions::assignTorsion(const String& name, Position i, ResidueTorsion& torsion) const
	{
		// if we know this residue...
		if (torsions_.has(name))
		{
			// ...verify the index...
			Size max_index = torsions_[name].size();

			if (i < (Position)max_index)
			{
				// ...and assign the torsion structure
				torsion = torsions_[name][i];
				return true;
			}
		}		

		return false;
	}

	bool FFPSResidueTorsions::hasTorsion
		(const String& residue, const String& atom_A, const String& atom_B,
		 const String& atom_C, const String& atom_D) const
	{
		String query = residue + " " + atom_A + " " 
			+ atom_B + " " + atom_C + " " + atom_D;
		
		// is this a known torsion?
		if (all_torsions_.has(query))
		{ 
			return true;
		}
		
		// reverse the order of the atoms and try again
		query = residue + " " + atom_D + " " 
			+ atom_C + " " + atom_B + " " + atom_A;
		
		return all_torsions_.has(query);
	}
 
	 
} // namespace BALL
