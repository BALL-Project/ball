// $Id: analyticalSES.C,v 1.9 2000/06/27 11:08:44 oliver Exp $
// $Id: analyticalSES.C,v 1.9 2000/06/27 11:08:44 oliver Exp $

#include <BALL/STRUCTURE/analyticalSES.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/baseFragment.h>

namespace BALL
{

  extern int connolly_ (int number_of_atoms, double *coordinates, double *radius,
												double *volume, double *area, double probe, double exclude, double* atom_areas);

	float calculateSESArea	
		(const BaseFragment& fragment, float probe_radius)
	{
		// extract all atoms: iterate over all composites and
		// check whether they are Atoms
		vector<Atom*>	atoms;
		AtomIterator	it = fragment.beginAtom();
		for (; +it; ++it)
		{
			if (it->getRadius() > 0.0)
			{
				atoms.push_back(&*it);
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
		double* atom_areas = new double[atoms.size()];
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


		// int connolly_ (int number_of_atoms, double *coordinates, double *radius,
		//								double *volume, double *area, double probe, double exclude, double* atom_areas);
		// call connolly_
		connolly_(number_of_atoms, coordinates, radii, 
							&volume, &area, probe_radius, 0.0, atom_areas);
							
		// free the input fields
		delete [] coordinates;
		delete [] radii;
		delete [] atom_areas;

		return area;
	}

  float calculateSESAtomAreas
		(const BaseFragment& fragment, HashMap<Atom*,float>& atom_areas, float probe_radius)
	{
		// extract all atoms: iterate over all composites and
		// check whether they are Atoms
		vector<Atom*>	atoms;
		AtomIterator	it = fragment.beginAtom();
		for (; +it; ++it)
		{
			if (it->getRadius() > 0.0)
			{
				atoms.push_back(&*it);
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
		double* tmp_atom_areas = new double[atoms.size()];
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


		// call connolly_
		connolly_(number_of_atoms, coordinates, radii, 
							&volume, &area, probe_radius, 0.0, tmp_atom_areas);
							

		// extract the atom areas and store them in the hash map
		atom_areas.clear();
		for (Position i = 0; i < atoms.size(); ++i)
		{
			atom_areas.insert(pair<Atom*, float>(atoms[i], tmp_atom_areas[i]));
		}

		// free the input fields
		delete [] coordinates;
		delete [] radii;
		delete [] tmp_atom_areas;

		return area;
	}

	float calculateSESVolume	
		(const BaseFragment& fragment, float probe_radius)
	{
		// extract all atoms: iterate over all composites and
		// check whether they are Atoms
		vector<Atom*>	atoms;
		AtomIterator	it = fragment.beginAtom();
		for (; +it; ++it)
		{
			if (it->getRadius() > 0.0)
			{
				atoms.push_back(&*it);
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
		double* tmp_atom_areas = new double[atoms.size()];
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

		// call connolly_
		connolly_(number_of_atoms, coordinates, radii, 
							&volume, &area, probe_radius, 0.0, tmp_atom_areas);
							
		// free the input fields
		delete [] coordinates;
		delete [] radii;
		delete [] tmp_atom_areas;

		return volume;
	}

} // namespace BALL
