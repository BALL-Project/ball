// $Id: analyticalSES.C,v 1.5 2000/03/26 12:36:24 oliver Exp $
// $Id: analyticalSES.C,v 1.5 2000/03/26 12:36:24 oliver Exp $

#include <BALL/STRUCTURE/analyticalSES.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/DATATYPE/hashSet.h>

namespace BALL
{

	extern int connolly_ (int number_of_atoms, double* coordinates, double* radius,
												double* volume, double* area, double probe, double exclude);

	float calculateSESArea	
		(const Composite& composite, float probe_radius)
	{
		// extract all atoms: iterate over all composites and
		// check whether they are Atoms
		vector<Atom*>	atoms;
		Composite::SubcompositeIterator	it = composite.beginSubcomposite();
		for (; it != composite.endSubcomposite(); ++it)
		{
			if (RTTI::isKindOf<Atom>(*it))
			{
				atoms.push_back(RTTI::castTo<Atom>(*it));
			}
		}
		
		// if no atoms are found, return zero
		if (atoms.size() == 0)
		{
			return 0;
		}
				
		// create the field required by nsc and fill it with the atom coordinates
		double* coordinates = new double[atoms.size() * 3];
		double* radii = new double[atoms.size()];
		for (Size i = 0; i < atoms.size(); i++)
		{
			float tmp[3];
			atoms[i]->getPosition().get(tmp);
			coordinates[i * 3]			= tmp[0];
			coordinates[i * 3 + 1]	= tmp[1];
			coordinates[i * 3 + 2]	= tmp[2];
			radii[i] = atoms[i]->getRadius();
		}

		double area;
		double volume;
		int number_of_atoms = (int)atoms.size();
		double exclude_radius = 0.0;


		// call connolly_
		connolly_(number_of_atoms, coordinates, radii, &volume, 
							&area, probe_radius, exclude_radius);
							
		// free the input fields
		delete [] coordinates;
		delete [] radii;

		return area;
	}

} // namespace BALL
