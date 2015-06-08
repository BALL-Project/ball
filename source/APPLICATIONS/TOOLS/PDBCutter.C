// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_PDBFILE_H
# include <BALL/FORMAT/PDBFile.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
# include <BALL/STRUCTURE/fragmentDB.h>
#endif

#include <BALL/STRUCTURE/buildBondsProcessor.h>
#include <BALL/FORMAT/commandlineParser.h>
#include "version.h"
#include <iostream>

using namespace std;
using namespace BALL;

void usage (const char *progname);

int main (int argc, char **argv)
{
	CommandlineParser parpars("PDBCutter", "separate ligand and receptor  ", VERSION, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "input pdb-file");
	parpars.registerMandatoryOutputFile("rec", "receptor output pdb-file");
	parpars.registerMandatoryOutputFile("lig", "ligand output pdb-file");
	parpars.registerMandatoryStringParameter("lig_chain", "chain-name of ligand");
	parpars.registerMandatoryStringParameter("lig_name", "ligand name");
	parpars.registerOptionalStringListParameter("rm_ch", "protein chains that are to be deleted");
	parpars.registerOptionalStringListParameter("rm_res", "pdb-residues that are to be deleted (e.g. water or ions)");
	String man = "This tool splits a given pdb-file into two files containing receptor and reference ligand, respectively.\n\nThe name of the reference ligand (exactly as it appears in the pdb-file) and the name of its chain need to be specified by '-lig_name' and '-lig_chain'.\nOptionally, chains (e.g. in case of multimers) or pdb-residues (e.g. water or ions) that you don't need can be deleted from the receptor. In this case, specify their names with '-rm_ch' or '-rm_res'.\n\nOutput of this tool is one pdb-file containing the receptor-structure, i.e. the protein w/o reference ligand and w/o undesired chains/residues (if any were specified), and one pdb-file containing the reference ligand.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i","pdb");
	parpars.setSupportedFormats("rec","pdb");
	parpars.setSupportedFormats("lig","pdb");
	parpars.parse(argc, argv);

	PDBFile f0;
	f0.open(parpars.get("i"));
	System sys0;
	f0 >> sys0;
	Protein protein;
	protein.insert(sys0);

	Protein ligand;
	String lig_chain_name = parpars.get("lig_chain");
	String lig_name = parpars.get("lig_name");
	set<String> delete_chains;
	set<String> delete_residues;
	list<String> plist = parpars.getList("rm_ch");
	for (list < String > ::iterator it = plist.begin(); it != plist.end(); it++)
	{
		delete_chains.insert(*it);
	}
	plist = parpars.getList("rm_res");
	for (list < String > ::iterator it = plist.begin(); it != plist.end(); it++)
	{
		delete_residues.insert(*it);
	}
	bool del_res = (delete_residues.size()>0);

	for (ChainIterator it_ch = protein.beginChain(); it_ch != protein.endChain(); ++it_ch)
	{
		if (it_ch->getName() == lig_chain_name)
		{
			for (ResidueIterator it_res = it_ch->beginResidue(); it_res != it_ch->endResidue(); ++ it_res)
			{
				if (it_res->getName() == lig_name)
				{
					Residue* new_res = new Residue(*it_res);
					ligand.append(*new_res);
					it_res->select();
				}
				else if (delete_residues.find(it_res->getName()) != delete_residues.end())
				{
					it_res->select();
				}
			}
		}
		else if (del_res)
		{
			for (ResidueIterator it_res = it_ch->beginResidue(); it_res != it_ch->endResidue(); ++ it_res)
			{
				if (delete_residues.find(it_res->getName()) != delete_residues.end())
				{
					it_res->select();
				}
			}
		}

		if (delete_chains.find(it_ch->getName()) != delete_chains.end())
		{
			it_ch->select();
		}
	}

	protein.removeSelected();

	PDBFile f2(parpars.get("lig"), std::ios::out);
	f2 << ligand;
	f2.close();

	PDBFile f3(parpars.get("rec"), std::ios::out);
	f3 << protein;
	f3.close();

	return 0;
}
