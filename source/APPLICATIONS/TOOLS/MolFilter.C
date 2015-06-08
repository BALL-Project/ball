// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/logP.h>

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("MolFilter", "filter molecule files            ", "0.9", String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "input molecule-file");
	parpars.registerOptionalDoubleParameter("min_logP", "minimal logP value", 0.0);
	parpars.registerOptionalDoubleParameter("max_logP", "maximal logP value", 20.0);
	parpars.registerOptionalDoubleParameter("min_MW", "minimal molecular weight", 0.0);
	parpars.registerOptionalDoubleParameter("max_MW", "maximal molecular weight", 1e10);
	parpars.registerOptionalInputFile("q", "query molecules for similarity searching");
	parpars.registerOptionalDoubleParameter("min_sim", "minimal average similarity", 0.0);
	parpars.registerOptionalDoubleParameter("max_sim", "maximal similarity", 1.0);
	parpars.registerOptionalStringParameter("smarts", "SMARTS pattern");
	parpars.registerOptionalInputFile("smarts_file", "SMARTS pattern");

	parpars.registerOptionalOutputFile("o", "output molecule file");
	parpars.registerFlag("quiet", "by quiet, i.e. do not show progress information");
	parpars.registerFlag("rm", "remove input file when finished");
	parpars.setSupportedFormats("i","mol2, sdf, drf");
	parpars.setSupportedFormats("o","mol2, sdf, drf");
	parpars.setSupportedFormats("q","txt");
	parpars.setSupportedFormats("smarts_file","txt");
	String manual = "MolFilter can filter molecules from a molecule input file according to SMARTS expressions, logP, molecular weight, or similarity to query molecule(s).\n\nOutput of this tool is a molecule file that contains all compounds that fulfilled the specified search criteria.";
	parpars.setToolManual(manual);
	parpars.parse(argc, argv);

	bool quiet = parpars.has("quiet");

	bool use_logP = false;
	double min_logP = 0;
	double max_logP = 20;
	if (parpars.has("min_logP"))
	{
		use_logP = true;
		min_logP = parpars.get("min_logP").toDouble();
	}
	if (parpars.has("max_logP"))
	{
		use_logP = true;
		max_logP = parpars.get("max_logP").toDouble();
	}


	bool use_MW = false;
	double max_MW = 1e10;
	double min_MW = 0;
	if (parpars.has("min_MW"))
	{
		use_MW = true;
		min_MW = parpars.get("min_MW").toDouble();
	}
	if (parpars.has("max_MW"))
	{
		use_MW = true;
		max_MW = parpars.get("max_MW").toDouble();
	}


	bool use_smarts = (parpars.has("smarts") || parpars.has("smarts_file"));
	list<String> smarts;
	if (parpars.has("smarts"))
	{
		String separator = String::CHARACTER_CLASS__WHITESPACE;
		separator += ", ";
		vector<String> toks;
		String par_smarts = parpars.get("smarts");
		par_smarts.splitQuoted(toks, separator.c_str());
		for (Size i = 0; i < toks.size(); i++)
		{
			toks[i].trim();
			if (toks[i] != "")
			{
				smarts.push_back(toks[i]);
			}
		}
	}
	if (parpars.has("smarts_file"))
	{
		std::ifstream in(parpars.get("smarts_file").c_str());
		while (in)
		{
			String tmp;
			in >> tmp;
			tmp.trim();
			if (tmp != "")
			{
				smarts.push_back(tmp);
			}
		}
	}


	Size n = parpars.has("q") + parpars.has("min_sim") + parpars.has("max_sim");
	if (n > 0 && n != 3)
	{
		cerr << "[Error:] If similarity filtering is desired, parameters '-q', '-min_sim' and '-max_sim' must be specified!" << endl;
		exit(1);
	}
	bool similarity_filtering = (n==3);
	double min_sim = 0;
	double max_sim = 1;
	MolecularSimilarity molsim("fragments/functionalGroups.smarts");
	list<vector<bool> > fingerprints_input;
	list<vector<bool> > fingerprints_query;

	if (similarity_filtering)
	{
		min_sim = parpars.get("min_sim").toDouble();
		max_sim = parpars.get("max_sim").toDouble();
		GenericMolFile* input = MolFileFactory::open(parpars.get("i"));
		GenericMolFile* query = MolFileFactory::open(parpars.get("q"));
		int mols = 0;
		for (Molecule* mol = input->read(); mol; delete mol, mol = input->read())
		{
			mols++;
			fingerprints_input.resize(mols);
			molsim.generatePathFingerprint(*mol, fingerprints_input.back());
		}
		mols = 0;
		for (Molecule* mol = query->read(); mol; delete mol, mol = query->read())
		{
			mols++;
			fingerprints_query.resize(mols);
			molsim.generatePathFingerprint(*mol, fingerprints_query.back());
		}
		delete input;
		delete query;
		if (!quiet) Log.level(20)<<"reading input done."<<endl<<flush;
	}

	double simsum = 0;
	int no_identical = 1;
	int no_found = 0;
	int no = 0;

	GenericMolFile* input = MolFileFactory::open(parpars.get("i"));
	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, input);

	list<vector<bool> > ::iterator it1 = fingerprints_input.begin();
	for (Molecule* mol = input->read(); mol; delete mol, mol = input->read())
	{
		bool mol_is_selected = true;

		if (use_MW)
		{
			double MW = 0;
			for (AtomConstIterator a_it = mol->beginAtom(); +a_it; a_it++)
			{
				MW += a_it->getElement().getAtomicWeight();
			}
			if (MW > max_MW || MW < min_MW)
			{
				mol_is_selected = false;
			}
		}

		if (use_logP)
		{
		#ifdef BALL_HAS_OPENBABEL
			LogP calculator;
			String cansmile;
			molsim.generateCanSmile(*mol, cansmile);
			double logP = calculator.calculate(cansmile);
			if (logP > max_logP || logP < min_logP)
			{
				mol_is_selected = false;
			}
		#else
			Log.error() << "[Error:] Filtering by use of logP is not supported because this tool has been built without support for OpenBabel." << endl;
			exit(1);
		#endif
		}

		if (similarity_filtering)
		{
			double max_detected_sim = -999999;
			double average_sim = 0;
			bool found_identical = 0;
			int a = 0;
			int max = fingerprints_query.size();
			if (parpars.get("i") == parpars.get("q")) max = (max/2)+1;

			for (list<vector<bool> >::iterator it2 = fingerprints_query.begin(); it2 != fingerprints_query.end() && a < max; it2++, a++)
			{
				if (!quiet && no%10000 == 0)
				{
					Log.level(20)<<"\r"<<no;
					Log.flush();
				}
				double sim = molsim.calculateSimilarity(*it1, *it2);
				average_sim += sim;
				if (sim > max_detected_sim) max_detected_sim = sim;
				if (sim > 0.99 && !found_identical)
				{
					no_identical++;
					found_identical = true;
				}
				simsum += sim;
				no++;
			}

			average_sim /= a;
			if (average_sim < min_sim || max_detected_sim > max_sim)
			{
				mol_is_selected = false;
				no_found++;
			}

			it1++;
		}

		if (use_smarts)
		{
		#ifdef BALL_HAS_OPENBABEL
			String cansmile;
			Size req_matches = 1;
			molsim.generateCanSmile(*mol,cansmile);

			for (list<String>::iterator s_it = smarts.begin(); s_it != smarts.end(); s_it++)
			{
				Size matches = 0;
				molsim.matchSmarts(cansmile, *s_it, matches, req_matches);
				if (matches < 1)
				{
					mol_is_selected = false;
				}
				else
				{
					mol_is_selected = true;
					break;
				}
			}
		#else
			Log.error() << "[Error:] Filtering by use of SMARTS expressions is not supported because this tool has been built without support for OpenBabel or OpenEye." << endl;
			exit(1);
		#endif
		}

		if (mol_is_selected)
		{
			*output << *mol;
			no_found++;
		}
	}

	Log<<endl<<"Found "<<no_found<<" molecule and wrote them to output file."<<endl;

	delete input;
	delete output;

	if (parpars.has("rm"))
	{
		File::remove(parpars.get("i"));
	}
}
