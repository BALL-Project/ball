// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/molecule.h>
//#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include <BALL/DOCKING/COMMON/flexibleMolecule.h>
#include <BALL/FORMAT/commandlineParser.h>
#include "version.h"

using namespace BALL;
using namespace std;

double calculateMedian(HashMap<String, double>& scores)
{
	multiset<double> list;
	for (HashMap < String, double > ::iterator it = scores.begin(); it != scores.end(); it++)
	{
		if (it->second < 100)
		{
			list.insert(it->second);
		}
	}
	int size = list.size();
	int i = 0;
	multiset<double>::iterator it = list.begin();
	for (; i < size/2 && it != list.end(); it++, i++)
	{
	}
	return *it;
}

int main(int argc, char* argv[])
{
	CommandlineParser parpars("AntitargetRescorer", "rescore w/ anti-target dock-results", VERSION, String(__DATE__), "Rescoring");
	parpars.registerMandatoryInputFile("t", "input file w/ target dock-results");
	parpars.registerMandatoryInputFile("at", "input file w/ anti-target dock-results");
	parpars.registerMandatoryOutputFile("o", "output file");
	String manual = "This tool rescores docking output poses.\nAntitargetRescoring can be used to try to enhance target specificity. Therefore, dock your compounds into your target of interest and into a (very) different protein and supply the docking results here. All compounds that received a very good antitarget-score will thus be penalized, i.e. they will have a much worse score within the output file.\n\nAs input we need:\n\
    * a file containing the compounds that are to be rescored. Supported formats are mol2, sdf or drf (DockResultFile, xml-based). Those compound should have been docket into the specified protein (i.e. the target).\n\
    * a file containing the same compounds docked into the antitarget.\n\nOutput of this tool is a file in the same format as the input ligand file containing all compounds with scores obtained by rescoring in form of a property 'antitarget_rescore'.";
	parpars.setToolManual(manual);
	parpars.setSupportedFormats("t","mol2,sdf,drf");
	parpars.setSupportedFormats("at","mol2,sdf,drf");
	parpars.setSupportedFormats("o","mol2,sdf,drf");
	parpars.parse(argc, argv);

	GenericMolFile* target = MolFileFactory::open(parpars.get("t"));
	GenericMolFile* antitarget = MolFileFactory::open(parpars.get("at"));

	String label_t = "score";
	String label_at = "score";

	/// Fetch anti-target scores and compute their median
	HashMap<String, double> antitarget_scores;
	int at_missing_scores = 0;
	for (Molecule* mol = antitarget->read(); mol; delete mol, mol = antitarget->read())
	{
		if (mol->hasProperty(label_at))
		{
			double antitarget_score = ((String)mol->getProperty(label_at).toString()).toDouble();
			String topology_hash;
			FlexibleMolecule::generateTopologyHash(mol, topology_hash, 1);
			antitarget_scores.insert(make_pair(topology_hash, antitarget_score));
		}
		else at_missing_scores++;
	}
	double median_at = calculateMedian(antitarget_scores);

	/// Fetch target scores and compute their median
	HashMap<String, double> target_scores;
	for (Molecule* mol = target->read(); mol; delete mol, mol = target->read())
	{
		if (mol->hasProperty(label_at))
		{
			double target_score = ((String)mol->getProperty(label_at).toString()).toDouble();
			String topology_hash;
			FlexibleMolecule::generateTopologyHash(mol, topology_hash, 1);
			target_scores.insert(make_pair(topology_hash, target_score));
		}
	}
	double median_t = calculateMedian(target_scores);

	/// Compute re-scores and write rescored molecules to output file
	double offset = median_t-median_at;
	int t_missing_scores = 0;
	delete target;
	target = MolFileFactory::open(parpars.get("t"));
	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, target);
	for (Molecule* mol = target->read(); mol; delete mol, mol = target->read())
	{
		String topology_hash;
		FlexibleMolecule::generateTopologyHash(mol, topology_hash, 1);

		if (mol->hasProperty(label_t))
		{
			double target_score = ((String)mol->getProperty(label_at).toString()).toDouble();

			HashMap<String, double>::iterator it = antitarget_scores.find(topology_hash);
			if (it != antitarget_scores.end())
			{
				double antitarget_score = it->second+offset;

				double p;
				if (target_score < 0) p = target_score*0.95;
				else p = target_score*1.05;
				if (antitarget_score < p)
				{
					double rescore = p+fabs(p-antitarget_score)*100;
					mol->setProperty("antitarget_rescore", rescore);
				}
				else
				{
					mol->setProperty("antitarget_rescore", target_score);
				}
			}
			else
			{
				mol->setProperty("antitarget_rescore", target_score);
			}
		}
		else t_missing_scores++;

		*output << *mol;
	}

	if (at_missing_scores > 0)
	{
		Log.level(20)<<"[Warning:] "<<at_missing_scores<<" molecules in antitarget input file did not contain score property."<<endl;
	}
	if (t_missing_scores > 0)
	{
		Log.level(20)<<"[Warning:] "<<t_missing_scores<<" molecules in target input file did not contain score property."<<endl;
	}

	target->close();
	antitarget->close();
	output->close();
	delete target;
	delete antitarget;
	delete output;
}
