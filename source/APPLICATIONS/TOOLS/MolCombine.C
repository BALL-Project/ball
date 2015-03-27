// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/UCK.h>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("MolCombine", "combine molecular files", VERSION, String(__DATE__), "Convert, combine and store");
	parpars.registerMandatoryInputFile("i1", "input molecule file A");
	parpars.registerMandatoryInputFile("i2", "input molecule file B");
	parpars.registerMandatoryStringParameter("mode","'intersection', 'union' or 'b_not_a'");
	parpars.registerFlag("ignH", "ignore hydrogens, i.e. match molecules to any protonation state.", true);
	parpars.registerMandatoryOutputFile("o", "output file");
	parpars.registerFlag("replace_prop", "replace properties from file 1 w/ those from file 2");
	parpars.registerFlag("rm", "remove input files when finished");
	String man = "This tool generates the intersection or union of two given chemical files. Property-tags of molecules that appear in both input files are automatically merged.\n\nIf you want to match molecules regardless of their protonation state, use option '-ignH'.\n\nOutput of this tool is a file containing the union resp. intersection of all molecules of input A and B.";
	parpars.setToolManual(man);
	list<String> slist;
	slist.push_back("intersection");
	slist.push_back("union");
	slist.push_back("b_not_a");
	parpars.setParameterRestrictions("mode",slist);
	parpars.setSupportedFormats("i1","mol2,sdf,drf");
	parpars.setSupportedFormats("i2","mol2,sdf,drf");
	parpars.setSupportedFormats("o","mol2,sdf,drf");
	parpars.parse(argc, argv);

	String mode = parpars.get("mode");
	bool use_union = (mode == "union");
	bool use_intersection = (mode == "intersection");
	bool use_b_not_a = (mode == "b_not_a");

	int n = use_union+use_intersection+use_b_not_a;
	if (n != 1)
	{
		cerr<<"[Error:] Please set parameter 'mode' to either 'union' or 'intersection' or 'b_not_a'!"<<endl;
		return 1;
	}

	GenericMolFile* input = MolFileFactory::open(parpars.get("i1"), ios::in);
	HashMap<String, Molecule*> input1;
	list<Molecule*> selected_molecules;
	const String& nf = CommandlineParser::NOT_FOUND;
	bool ignoreH = (parpars.get("ignH")!=nf);
	bool replace_prop = (parpars.get("replace_prop")!=nf);

	Molecule* mol;
	int no_written = 0;
	int no_ignored = 0;
	while ((mol = input->read()))
	{
		String hash;
		UCK uck(*mol, ignoreH);
		hash = uck.getUCK();

		input1.insert(make_pair(hash, mol));
		if (use_union) selected_molecules.push_back(mol);
		if (use_b_not_a) delete mol;
	}

	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, input);
	delete input;
	input = MolFileFactory::open(parpars.get("i2"), ios::in);
	set<Molecule*> deleted_mols;

	while ((mol = input->read()))
	{
		String hash;
		UCK uck(*mol, ignoreH);
		hash = uck.getUCK();

		HashMap<String, Molecule*>::iterator it = input1.find(hash);
		if (it == input1.end()) // mol is found in file2 but not in file1
		{
			if (use_intersection)
			{
				deleted_mols.insert(mol);
				delete mol;
				continue;
			}
			else if (use_union || use_b_not_a)
			{
				*output << *mol;
				delete mol;
				continue;
			}
		}
		else if (use_b_not_a)
		{
			delete mol;
			continue;
		}

		Molecule* mol1 = it->second;
		Molecule* mol2 = mol;

		for (NamedPropertyIterator p_it = mol2->beginNamedProperty(); p_it != mol2->endNamedProperty(); p_it++)
		{
			if (replace_prop || !mol1->hasProperty(p_it->getName()))
			{
				mol1->setProperty(p_it->getName(), p_it->toString());
			}
		}
		delete mol2;
		if (!use_union) selected_molecules.push_back(mol1);
	}

	for (list < Molecule* > ::iterator it = selected_molecules.begin(); it != selected_molecules.end(); it++)
	{
		*output << **it;
		no_written++;

		if (use_union || use_b_not_a)
		{
			deleted_mols.insert(*it);
			delete *it;
		}
	}

	if (use_intersection)
	{
		for (HashMap < String, Molecule* > ::iterator it = input1.begin(); it != input1.end(); it++)
		{
			if (deleted_mols.find(it->second) == deleted_mols.end()) delete it->second;
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
		File::remove(parpars.get("i1"));
		File::remove(parpars.get("i2"));
	}
}
