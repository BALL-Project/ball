// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/analyticalSES.h>
// v2.2 (PR-removal phase 6): formerly visible via the removed persistenceManager.h include chain
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomContainer.h>

namespace BALL
{
	extern int connolly_ (int number_of_atoms, double *coordinates, double *radius,
	                      double *volume, double *area, double probe, double exclude, double* atom_areas);

	float calculateSESArea(const AtomContainer& fragment, float probe_radius)
	{
		// extract all atoms: iterate over all composites and
		// check whether they are Atoms
		vector<const Atom*>	atoms;
		AtomConstIterator	it = fragment.beginAtom();
		for (; +it; ++it)
		{
			if (it->getRadius() > 0.0)
			{
				atoms.push_back(&*it);
			}
		}
		
		// if no atoms are found, return zero
		if (atoms.empty())
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
		//                double *volume, double *area, double probe, double exclude, double* atom_areas);
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
		(const AtomContainer& fragment, HashMap<const Atom*,float>& atom_areas, float probe_radius)
	{
		// extract all atoms: iterate over all composites and
		// check whether they are Atoms
		vector<const Atom*>	atoms;
		AtomConstIterator	it = fragment.beginAtom();
		for (; +it; ++it)
		{
			if (it->getRadius() > 0.0)
			{
				atoms.push_back(&*it);
			}
		}
		
		// if no atoms are found, return zero
		if (atoms.empty())
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
			// Explicit double -> float narrowing. The connolly_ Fortran routine returns
			// double-precision atom areas; the HashMap<const Atom*, float> API contract
			// stores float (saves 4 bytes/atom; SES rounding error is dominated by
			// surface tessellation anyway). Cast documents the intent + silences C4244.
			atom_areas.insert(std::pair<const Atom*, float>(atoms[i], static_cast<float>(tmp_atom_areas[i])));
		}

		// free the input fields
		delete [] coordinates;
		delete [] radii;
		delete [] tmp_atom_areas;

		return area;
	}

	// v2.2 H3d.B (D-H3.8): StableId-keyed overload. Replicates the v0
	// surface computation; only the output-map key type changes.
	float calculateSESAtomAreas
		(const AtomContainer& fragment,
		 HashMap<MoleculeStore::StableId, float>& atom_areas_sid,
		 float probe_radius)
	{
		vector<const Atom*> atoms;
		MoleculeStore* store = nullptr;
		AtomConstIterator it = fragment.beginAtom();
		for (; +it; ++it)
		{
			if (it->getRadius() <= 0.0) continue;
			MoleculeStore* s = const_cast<Atom&>(*it).getStore();
			if (s == nullptr) continue;
			if (store == nullptr) store = s;
			else if (s != store) continue;
			atoms.push_back(&*it);
		}
		if (atoms.empty()) { atom_areas_sid.clear(); return 0; }

		double* coordinates = new double[atoms.size() * 3];
		double* radii = new double[atoms.size()];
		double* tmp_atom_areas = new double[atoms.size()];
		for (Size i = 0; i < atoms.size(); i++)
		{
			float tmp[3];
			atoms[i]->getPosition().get(tmp);
			coordinates[i * 3]     = tmp[0];
			coordinates[i * 3 + 1] = tmp[1];
			coordinates[i * 3 + 2] = tmp[2];
			radii[i] = atoms[i]->getRadius();
		}

		double area, volume;
		int number_of_atoms = (int)atoms.size();
		connolly_(number_of_atoms, coordinates, radii,
		          &volume, &area, probe_radius, 0.0, tmp_atom_areas);

		atom_areas_sid.clear();
		for (Position i = 0; i < atoms.size(); ++i)
		{
			const MoleculeStore::StableId sid =
				store->stable_id(atoms[i]->getStoreIndex());
			atom_areas_sid.insert(std::pair<MoleculeStore::StableId, float>(
				sid, static_cast<float>(tmp_atom_areas[i])));
		}

		delete [] coordinates;
		delete [] radii;
		delete [] tmp_atom_areas;
		return area;
	}

	float calculateSESVolume
		(const AtomContainer& fragment, float probe_radius)
	{
		// extract all atoms: iterate over all composites and
		// check whether they are Atoms
		vector<const Atom*>	atoms;
		AtomConstIterator	it = fragment.beginAtom();
		for (; +it; ++it)
		{
			if (it->getRadius() > 0.0)
			{
				atoms.push_back(&*it);
			}
		}
		
		// if no atoms are found, return zero
		if (atoms.empty())
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
