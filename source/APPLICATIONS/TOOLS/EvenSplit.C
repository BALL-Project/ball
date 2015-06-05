// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("EvenSplit", "generate splits w/ equal property range", VERSION, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "input file");
	parpars.registerMandatoryOutputFile("o1", "output file 1");
	parpars.registerMandatoryOutputFile("o2", "output file 2");

	parpars.registerOptionalStringParameter("prop", "property name", "binding_free_energy");
	parpars.registerOptionalIntegerParameter("n", "max. number of compounds to use from input file");
	parpars.registerOptionalIntegerParameter("k", "extract each k'th compound to 2nd output file", 2);
	parpars.registerOptionalIntegerParameter("offset", "offset; extract each (i+offset)%k == 0 to 2nd output file");
	String man = "This tool splits a molecule file into two subsets in such a way that each of them convers an equal range of a property. The property with respect to which this is to be done should be specified with '-prop'.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i","mol2,sdf,drf");
	parpars.setSupportedFormats("o1","mol2,sdf,drf");
	parpars.setSupportedFormats("o2","mol2,sdf,drf");

	parpars.parse(argc, argv);

	GenericMolFile* input = MolFileFactory::open(parpars.get("i"), ios::in);
	GenericMolFile* output1 = MolFileFactory::open(parpars.get("o1"), ios::out, input);
	GenericMolFile* output2 = MolFileFactory::open(parpars.get("o2"), ios::out, input);

	String s = parpars.get("offset");
	int offset = 0;
	if (s != CommandlineParser::NOT_FOUND) offset = s.toInt();

	bool use_max = 0;
	int max = 0;
	s = parpars.get("n");
	if (s != CommandlineParser::NOT_FOUND)
	{
		max = s.toInt();
		if (max > 0) use_max = 1;
	}

	int k = 2;
	s = parpars.get("k");
	if (s != CommandlineParser::NOT_FOUND) k = s.toInt();
	String property = parpars.get("prop");

	multimap<double, Molecule*> prop_to_id;
	Size missing_prop = 0;
	//Size no_written = 0;

	for (Molecule* mol = input->read(); mol; mol = input->read())
	{
		if (mol->hasProperty(property))
		{
			String p = mol->getProperty(property).toString();
			prop_to_id.insert(make_pair(p.toDouble(), mol));
		}
		else missing_prop++;
	}

	int i = 1;
	Size no_o1 = 0;
	Size no_o2 = 0;
	for (multimap < double, Molecule* > ::iterator it = prop_to_id.begin();
		it!=prop_to_id.end(); it++, i++)
	{
		if (use_max && i > max)
		{
			delete it->second;
			continue;
		}

		if ((i+offset)%k == 0)
		{
			*output2 << *it->second;
			no_o2++;
		}
		else
		{
			*output1 << *it->second;
			no_o1++;
		}
		delete it->second;
	}


	if (missing_prop > 0)
	{
		Log<<"[Warning:]"<<missing_prop<<" molecules in the input file did not contain the desired property and were ignored."<<endl;
	}

	Log<<"Wrote "<<no_o1<<" molecules to file '"<<parpars.get("o1")<<"' and "<<no_o2<<" molecules to file '"<<parpars.get("o2")<<"'."<<endl;

	input->close();
	output1->close();
	output2->close();
	delete input;
	delete output1;
	delete output2;
}
