// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("SimilarityAnalyzer", "analyze similarity between molecule files", "0.9", String(__DATE__), "Analysis");
	parpars.registerMandatoryInputFile("i1", "input molecule file 1");
	parpars.registerMandatoryInputFile("i2", "input molecule file 2");
	parpars.registerOptionalOutputFile("o", "output eps-/png- or pdf-file");
	parpars.registerOptionalStringParameter("title", "plot title");
	parpars.registerFlag("quiet", "by quiet, i.e. do not print status");
	String man = "This tool evaluates the similarity between molecules in two input files and creates a distribution plot to visualize the result.\n\nTherefore, for each molecule a pathway-based, hashed binary fingerprint is generated and compared to the fingerprint of other molecules by use of the Tanimoto similarity measure.\nThe output graphic will created by use of gnuplot, so make sure to have it installed and in your PATH environment variable.\n\nThe resulting plot (in form of an eps-, png- or pdf-file; as chosen) shows the distribution of similarity values obtained by comparing each molecule in input file 1 against each molecule in input file 2.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i1",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("i2",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("o","png,pdf,eps");
	parpars.parse(argc, argv);

	GenericMolFile* input1 = MolFileFactory::open(parpars.get("i1"));
	GenericMolFile* input2 = MolFileFactory::open(parpars.get("i2"));

	MolecularSimilarity molsim("fragments/functionalGroups.smarts");

	list<vector<bool> > fingerprints_input1;
	list<vector<bool> > fingerprints_input2;

	bool quiet = (parpars.get("quiet")!=CommandlineParser::NOT_FOUND);

	int mols = 0;
	if (!quiet)
	{
		Log.level(10) << "Will now read input files ... ";
		Log.flush();
	}
	for (Size i = 0; i < 2; i++)
	{
		GenericMolFile* input;
		list<vector<bool> >* fingerprints;
		if (i == 0)
		{
			input = input1;
			fingerprints = &fingerprints_input1;
		}
		else if (i == 1)
		{
			input = input2;
			fingerprints = &fingerprints_input2;
		}

		mols = 0;
		for (Molecule* mol = input->read(); mol; delete mol, mol = input->read())
		{
			mols++;
			fingerprints->resize(mols);
			molsim.generatePathFingerprint(*mol, fingerprints->back());
		}

		if (mols==0)
		{
			Log.error() << endl << "[Error:] " << "input-file " << (i+1) << " contains no molecules (or the file has the wrong extension) !" << endl << endl;
			exit(1);
		}
	}
	if (!quiet) Log.level(10) << "\rreading input done." << endl << flush;

	double simsum = 0;
	double min_sim = 999999;
	double max_sim = -999999;
	int no_identical = 1;
	int stepsize = 2;
	vector<int> counts((100+stepsize)/stepsize, 0);
	int size = counts.size();
	multiset<double> sortedlist;

	Size total_num_pairs = fingerprints_input1.size()*fingerprints_input2.size();
	if (parpars.get("i1") == parpars.get("i2")) total_num_pairs = (total_num_pairs/2)+1;
	Size no = 0;
	for (list<vector<bool> >::iterator it1 = fingerprints_input1.begin(); it1 != fingerprints_input1.end(); it1++)
	{
		bool found_identical = 0;
		int a = 0;
		int max = mols;
		if (parpars.get("i1") == parpars.get("i2")) max = (max/2)+1;
		for (list<vector<bool> >::iterator it2 = fingerprints_input2.begin(); it2 != fingerprints_input2.end() && a < max; it2++, a++)
		{
			if (!quiet && no%10000 == 0)
			{
				int p = (int)((((float)no)/total_num_pairs)*100);
				Log.level(20)<<"\r"<<p<<"%"<<flush;
			}
			double sim = molsim.calculateSimilarity(*it1, *it2);
			sortedlist.insert(sim);
			if (sim < min_sim) min_sim = sim;
			if (sim > max_sim) max_sim = sim;
			if (sim > 0.99 && !found_identical)
			{
				no_identical++;
				found_identical = true;
			}
			int cell = (int)((sim*100)/stepsize);
			if (cell == size) cell = size-1;
			counts[cell]++;
			simsum += sim;
			no++;
		}
	}

	Log.level(15)<<"\rmin="<<min_sim<<", max="<<max_sim<<endl;
	Log.level(15)<<"number of compounds in input1 that have at lease one very similar molecule in input2 (sim>0.99) = "<<no_identical<<endl;
	Log.level(15)<<"average similarity = "<<simsum/no<<endl<<endl;

	multiset<double>:: iterator it = sortedlist.begin();
	for (Size a = 0; a < no/2; a++)
	{
		it++;
	}
	Log.level(15)<<"median of similarity = "<<*it<<endl;

	String filename = parpars.get("o");
	String f1 = "plot_data.txt";
	bool non_interactive = (filename != CommandlineParser::NOT_FOUND);
	if (non_interactive) File::createTemporaryFilename(f1, ".txt");
	ofstream out(f1.c_str());
	float num_pairs = fingerprints_input1.size()*fingerprints_input2.size();
	if (parpars.get("i1") == parpars.get("i2")) num_pairs /= 2;
	for (Size i = 0; i < counts.size(); i++)
	{
		out<<(i*stepsize)<<"\t"<<(counts[i]/num_pairs)<<endl;
	}
	out.close();
	String f2 = "plot.txt";
	if (non_interactive) File::createTemporaryFilename(f2, ".txt");
	out.open(f2.c_str());
	if (non_interactive)
	{
		if (filename.hasSuffix(".pdf"))
		{
			out<<"set term pdf enhanced"<<endl;
		}
		else if (filename.hasSuffix(".eps"))
		{
			out<<"set term postscript eps color enhanced"<<endl;
		}
		else
		{
			out<<"set term png enhanced size 1024, 650"<<endl;
		}

		out<<"set output \""<<filename<<"\""<<endl;
	}
	out<<"unset key"<<endl;
	String title = parpars.get("title");
	if (title != CommandlineParser::NOT_FOUND)
	{
		out<<"set title \""<<title<<"\""<<endl;
	}
	out<<"set xlabel \"% similarity\""<<endl;
	out<<"set ylabel \"fraction of compound-pairs\""<<endl;
	out<<"set xrange [-"<<(stepsize*0.75)<<":"<<(100+(stepsize*0.75))<<"]"<<endl;
	out<<"set style fill solid"<<endl;
	out << "plot \"" << f1 << "\" w boxes linestyle 3"<<endl;
	out.close();

	String c = "gnuplot ";
	if (!non_interactive) c += "-persist ";
	c += String(" < ") + f2;
	system(c.c_str());

	if (non_interactive)
	{
		File::remove(f1);
		File::remove(f2);
	}
	delete input1;
	delete input2;
}
