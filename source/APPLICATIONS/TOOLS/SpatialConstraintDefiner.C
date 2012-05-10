/* SpatialConstraintDefiner.C
*
* Copyright (C) 2011 Marcel Schumann
*
* This program free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/DOCKING/COMMON/constraints.h>
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include <BALL/DATATYPE/options.h>
#include "version.h"

using namespace BALL;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("SpatialConstraintDefiner", "define spatial constraint", VERSION, String(__DATE__), "Docking");
	parpars.registerParameter("ini", "input configuration file", INFILE);
	parpars.registerParameter("i", "input molecule file", INFILE, true);
	parpars.registerParameter("o", "output configuration file", OUTFILE, true);
	parpars.registerParameter("type", "'fraction' or 'number' of compound atoms", STRING, true);
	parpars.registerParameter("n", "desired number/fraction of atoms in spatial area", DOUBLE, true);
	parpars.registerParameter("p", "penalty value", DOUBLE, true, "1e11");
	String man = "This tool allows to define spatial constraints for docking or scoring.\n\nFor convenience, we use a molecule file as input and generate a boundary box around the contained compound. This molecule can therefore for example contain the reference ligand (obtained from a co-crystal structure), or a docked compound, or just a set of dummy atoms used to manually define the boundaries of the desired spatial constraint.\nFurthermore, you need to specify how many atoms of the compound to be docked (or scored) should be located inside the spatial area. You can either specify a number of atoms or a fraction of molecule atoms for this.\n\nOutput of this tool is a ini-file that contains the desired spatial constraint.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("ini","ini");
	parpars.setSupportedFormats("o","ini");
	list<String> slist;
	slist.push_back("fraction");
	slist.push_back("number");
	parpars.setParameterRestrictions("type",slist);
	parpars.parse(argc, argv);

	Log.setMinLevel(cout, 10);

	GenericMolFile* ligand_file = MolFileFactory::open(parpars.get("i"));
	Molecule* molecule = ligand_file->read();
	ligand_file->close();
	delete ligand_file;

	Options options;
	list<Constraint*> constraints;
	if (parpars.has("ini"))
	{
		DockingAlgorithm::readOptionFile(parpars.get("ini"), options, constraints, molecule);
	}

	bool atom_fraction = false;
	if (parpars.get("type") == "fraction") atom_fraction = true;

	ReferenceArea* rf = new ReferenceArea(molecule, atom_fraction, parpars.get("n").toDouble(), parpars.get("p").toDouble());
	String name = "SpatialConstraintDefiner on ";
	name += parpars.get("i");
	rf->setName(name);
	constraints.push_back(rf);

	/// Write output and clean everthing up
	DockingAlgorithm::writeOptionFile(parpars.get("o"), options, constraints);
	for (list<Constraint*>::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		delete *it;
	}
	delete molecule;
	return 0;
}
