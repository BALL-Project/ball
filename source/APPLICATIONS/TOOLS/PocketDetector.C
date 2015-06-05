// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/bindingPocketProcessor.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/DOCKING/COMMON/constraints.h>
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include "version.h"
#include <iostream>

using namespace std;
using namespace BALL;
//using namespace BALL::Docking;

void createReferenceArea(const vector<vector<std::pair<Vector3, double> > >& layers, const Vector3& ref_center, ReferenceArea*& ref_area, String filename, Molecule* ligand)
{
	Molecule mol;

	multimap<double, Vector3> dist_to_position;

	for (Size i = 0; i < layers.size(); i++)
	{
		for (Size j = 0; j < layers[i].size(); j++)
		{
			double dist = layers[i][j].first.getDistance(ref_center);
			if (dist < 12)
			{
				dist_to_position.insert(make_pair(dist, layers[i][j].first));
			}
		}
	}

	if (dist_to_position.empty())
	{
		Log.error() << "[Error:] No surface points in the neighborhood of the reference ligand found. Please check the location of your reference ligand!" << endl;
		exit(1);
	}

	Vector3 origin = ref_center - Vector3(10, 10, 10);
	HashGrid3<Vector3> grid(origin, 20, 20, 20, 2);

	// add pocket-point that is nearest to the geometrical center of the reference ligand to the grid
	multimap<double, Vector3>::iterator it = dist_to_position.begin();
	grid.insert(it->second, it->second);
	it++;

	for (; it != dist_to_position.end(); it++)
	{
		HashGridBox3<Vector3>* current_box = grid.getBox(it->second);
		Position x, y, z;
		grid.getIndices(*current_box, x, y, z);
		if (x > 1) x--;
		if (y > 1) y--;
		if (z > 1) z--;

		// search pocket-points near the current one
		bool found_nearby_point = 0;
		for (Size i = x; i < x+3 && i < 20; i++)
		{
			for (Size j = y; j < y+3 && j < 20; j++)
			{
				for (Size k = z; k < z+3 && k < 20; k++)
				{
					const HashGridBox3<Vector3>* box = grid.getBox(i, j, k);

					for (HashGridBox3 < Vector3 > ::ConstDataIterator b_it = box->beginData();
						+b_it; b_it++)
					{
						if (it->second.getDistance(*b_it) < 1.5)
						{
							found_nearby_point = true;
							break;
						}
					}
				}
			}
		}

		if (found_nearby_point)
		{
			current_box->insert(it->second);
			if (filename != "")
			{
				Atom* atom = new Atom;
				atom->setElement(PTE_::getElement(Element::CARBON));
				atom->setPosition(it->second);
				mol.insert(*atom);
			}
		}
	}

	if (filename != "")
	{
		GenericMolFile* output = MolFileFactory::open(filename, ios::out, "mol2");
		*output << mol;
		delete output;
	}

	// find min and max
	double min_x = 9999; double max_x = -9999;
	double min_y = 9999; double max_y = -9999;
	double min_z = 9999; double max_z = -9999;
	for (HashGrid3 < Vector3 > ::BoxIterator it = grid.beginBox(); it != grid.endBox(); it++)
	{
		for (HashGridBox3<Vector3>::ConstDataIterator b_it = it->beginData();
		+b_it; b_it++)
		{
			const Vector3& p = *b_it;
			if (p[0] < min_x) min_x = p[0];
			if (p[0] > max_x) max_x = p[0];
			if (p[1] < min_y) min_y = p[1];
			if (p[1] > max_y) max_y = p[1];
			if (p[2] < min_z) min_z = p[2];
			if (p[2] > max_z) max_z = p[2];
		}
	}

	// check size of detected pocket
	double volume = (max_x-min_x)*(max_y-min_y)*(max_z-min_z);
	if (volume < 100)
	{
		Log.level(20)<<"Detected pocket is very small ( < 100A^3) and therefore rejected."<<endl;
		ref_area = 0;
		return;
	}

	Vector3 p1(min_x, min_y, min_z);
	Vector3 p2(max_x, min_y, min_z);
	Vector3 p3(max_x, max_y, min_z);
	Vector3 p4(max_x, max_y, max_z);
	ref_area = new ReferenceArea(p1, p2, p3, p4, true, 0.5, 1e11);

	ref_area->calculateScore(ligand);
	double fraction = ref_area->getContainedAtoms();
	Log.level(10)<<"Fraction of atoms of reference ligand located in detected binding pocket: "<<fraction<<endl;

	ref_area->setNumberDesiredAtoms(fraction);
	ref_area->setName("Pocket found by PocketDetector");
}


int main (int argc, char **argv)
{
	CommandlineParser parpars("PocketDetector", "detect binding pocket", VERSION, String(__DATE__), "Docking");
	parpars.registerMandatoryInputFile("rec", "receptor pdb-file");
	parpars.registerMandatoryInputFile("rl", "reference ligand");
	parpars.registerMandatoryOutputFile("o", "output file");
	parpars.registerOptionalInputFile(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME, "input ini file");
	parpars.registerOptionalOutputFile("mol_out", "output file for pseudo-atoms describing pocket (for visualization purposes)");

	String man = "This tool tries to detect the binding pocket in which the reference ligand is located.\nTherefore, probe atoms are placed above the protein surface at positions of relative deep burial. The cluster of probe atoms around the geometric center of the reference ligand is used for the description of the binding pocket.\n\nAs input we need:\n\
    * a file containing a protonated protein in pdb-format. Furthermore, it should contain only relevant (i.e. strongly bound) water molecules as detected by WaterFinder.\n\
    * a file containing a reference ligand.\n\
      This reference ligand should be located in the binding pocket.\n\
      Supported formats are mol2, sdf or drf (DockResultFile, xml-based).\n\nOutput of this tool is a docking configuration file that contains the description of the detected binding pocket. This file should in following pipeline steps be specified for docking and rescoring.";

	parpars.setToolManual(man);
	parpars.setSupportedFormats("rec","pdb");
	parpars.setSupportedFormats("rl",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME,"ini");
	parpars.setSupportedFormats("o","ini");
	parpars.setSupportedFormats("mol_out",MolFileFactory::getSupportedFormats());
	parpars.parse(argc, argv);

	PDBFile input(parpars.get("rec"));
	Protein p;
	input >> p;

	GenericMolFile* input2 = MolFileFactory::open(parpars.get("rl"));
	Molecule* ligand = input2->read();
	delete input2;
	Vector3 lig_center(0, 0, 0);
	Size no_atoms = 0;
	for (AtomConstIterator it = ligand->beginAtom(); +it; ++it, no_atoms++)
	{
		lig_center += it->getPosition();
	}
	lig_center /= no_atoms;

	BindingPocketProcessor bpp;
	p.apply(bpp);

	Molecule mol;

	const vector<vector<std::pair<Vector3, double> > >& layers = bpp.getLayers();

/*
	cout<<"number of layers = "<<layers.size()<<endl;
	for (Size i = 0; i < layers.size(); i++)
	{
		for (Size j = 0; j < layers[i].size(); j++)
		{
			double dist = layers[i][j].first.getDistance(lig_center);
			if (dist < 12)
			{
				Atom* atom = new Atom;
				atom->setElement(PTE_::getElement(i));
				atom->setPosition(layers[i][j].first);
				mol.insert(*atom);
			}
		}
	}

	GenericMolFile* output = MolFileFactory::open("probeatoms.mol2", ios::out);
	*output << mol;
	delete output;
*/


	Options option;
	list<Constraint*> constraints;
	if (parpars.has(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME))
	{
		DockingAlgorithm::readOptionFile(parpars.get(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME), option, constraints, ligand);
	}
	String outmol_filename = "";
	if (parpars.has("mol_out"))
	{
		outmol_filename = parpars.get("mol_out");
	}
	ReferenceArea* ref_area = NULL;
	createReferenceArea(layers, lig_center, ref_area, outmol_filename, ligand);
	if (ref_area) constraints.push_back(ref_area);
	DockingAlgorithm::writeOptionFile(parpars.get("o"), option, constraints);
	if (ref_area) delete ref_area;

	return 0;
}
