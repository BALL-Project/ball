/* Ligand3DGenerator.C
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
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <openbabel/mol.h>
#include <openbabel/builder.h>
#include <openbabel/forcefield.h>
#include "version.h"

using namespace BALL;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("Ligand3DGenerator", "generate 3D coordinates for small molecules", VERSION, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input file", INFILE, true);
	parpars.registerParameter("o", "output file", OUTFILE, true);
	parpars.registerParameter("ph", "pH-value for pH-dep. protonation", DOUBLE, false, "7.0");
	parpars.registerFlag("rm", "remove input file when finished");
	String man = String("This tool generates 3D conformations for small molecules and adds hydrogens to them.\nAs input we only a chemical file containing valid topologies. Any 2D or 3D conformations in the input file are ignored, but the existence of the correct bond-orders is necessary.\n\nNote however, that the main purpose of this tool is to generate feasible start-conformations for docking or other optimization procedures. Therefore, the generated 3D coordinates for each fragment should be all right, but in extreme cases (i.e. very large and/or complex molecules) different fragments might still overlap with each other.\n\nSupported formats are ") + MolFileFactory::getSupportedFormats() + String(".");
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("o",MolFileFactory::getSupportedFormats());
	parpars.setOutputFormatSource("o","i");
	parpars.parse(argc, argv);

	GenericMolFile* input = MolFileFactory::open(parpars.get("i"), ios::in);
	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, input);
	DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
	if (drf_output)
	{
		drf_output->setToolInfo(parpars.getStartCommand(), parpars.getStartTime());
	}

	Molecule* mol;
	int no_written = 0;
	int no_ignored = 0;
	double pH = 7.4;
	if (parpars.get("ph") != CommandlineParser::NOT_FOUND)
	{
		pH = parpars.get("ph").toDouble();
	}
	OpenBabel::OBForceField* pFF = OpenBabel::OBForceField::FindForceField("MMFF94");
	if (!pFF)
	{
		cerr << "[Error:] Openbabel MMFF force-field could not be found.\nPlease make sure to link this executable to plugin_forcefields.so, otherwise the openbabel forcefield-plugins cannot be used!" << endl;
		exit(1);
	}

	for (Size i=1; (mol = input->read()); i++)
	{
		OpenBabel::OBMol* obmol = MolecularSimilarity::createOBMol(*mol);

		// Note: Openbabel::OpGen3D::Do() is NOT used directly here, because it re-assigns hydrogens for a static ph of 7.4, i.e. the pH can not be specified. Furthermore, it does also not remove hydrogens before using OBBuilder, resulting in hydrogens sometimes having more than one bond!

		// Remove all hydrogens before using OBBuilder, otherwise hydrogens end up having more than one bond! (Sigh!)
		bool switched_ff = false;
		bool ob_error = false;
		try
		{
			obmol->DeleteHydrogens();

			// Create 3D coordinates for individual fragments using template library
			OpenBabel::OBBuilder builder;
			builder.Build(*obmol);
			obmol->SetDimension(3);

			// Add hydrogens for the specified ph-value
			obmol->AddHydrogens(false, true, pH);

			bool setup_ok = pFF->Setup(*obmol);
			if (!setup_ok)
			{
// 				pFF = OpenBabel::OBForceField::FindForceField("UFF");
// 				switched_ff = true;
// 				if (pFF) setup_ok = pFF->Setup(*obmol);
				if (!setup_ok)
				{
					Log.level(20) << "[Warning:] Openbabel force-field setup failed for molecule " << i << ". Will only use template-based approach without force-field optimization for this molecule!" << endl;
				}
			}

			if (setup_ok)
			{
				// Optimize bond-lengths and torsions
				pFF->SteepestDescent(70, 1.0e-4); // 250, 1e-04

				// Rotate fragments around rotatable bonds
				pFF->WeightedRotorSearch(10, 5); // 200, 25

				// Optimize bond-lengths and torsions again
				pFF->ConjugateGradients(70, 1.0e-4); // 250, 1e-06

				pFF->UpdateCoordinates(*obmol);
			}
		}
		catch (...)
		{
			Log.level(20) << "Error while trying to generate 3D coordinates for molecule "<<i<<" ";
			if (mol->getName() != "") Log.level(20) << "'" << mol->getName() << "'";
			Log.level(20) << ". Will write unmodified input coordinates to output-file." << endl;
			delete obmol;
			ob_error = true;

			bool b = output->write(*mol);
			if (b) no_written++;
			else no_ignored++;
		}

		if (!ob_error)
		{
			// Fetch final 3D coordinates
			Molecule* new_mol = MolecularSimilarity::createMolecule(*obmol);
			for (NamedPropertyIterator it=mol->beginNamedProperty();
				it!=mol->endNamedProperty(); it++)
			{
				new_mol->setProperty(*it);
			}
			new_mol->setName(mol->getName());
			delete obmol;
			delete mol;

			bool b = output->write(*new_mol);
			if (b) no_written++;
			else no_ignored++;
			delete new_mol;
		}

		if (switched_ff)
		{
			pFF = OpenBabel::OBForceField::FindForceField("MMFF94");
		}
		if (no_written%5 == 0)
		{
			Log.level(20)<<"\r"<<no_written<<" molecules";
			Log.flush();
		}
	}

	Log.level(20)<<"\r";
	if (no_ignored > 0) Log.level(20)<<"ignored "<<no_ignored<<" identical molecules!"<<endl;
	Log.level(20)<<"wrote "<<no_written<<" molecules."<<endl;

	input->close();
	output->close();

	delete input;
	delete output;

	if (parpars.has("rm"))
	{
		File::remove(parpars.get("i"));
	}
}
