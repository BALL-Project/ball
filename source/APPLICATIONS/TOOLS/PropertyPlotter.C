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
	CommandlineParser parpars("PropertyPlotter", "plot molecule properties", VERSION, String(__DATE__), "Analysis");
	parpars.registerMandatoryInputFile("i", "input file");
	parpars.registerMandatoryStringParameter("p1", "name of property 1");
	parpars.registerOptionalStringParameter("p2", "name of property 2");
	parpars.registerFlag("quiet", "be quiet, i.e. do not print progress information");
	parpars.registerOptionalOutputFile("o", "output png-/eps-file");
	String man = "PropertyPlotter can be used to generate distribution- or scatter-plots of data contained in molecule property-tags.\n\nIn case you want to create a scatter-plot, specify the name of both property-tags to be used with '-p1' and '-p2'. If you want to generate a distribution plot, just specify '-p1'.\nThe output graphic will created by use of gnuplot, so make sure to have it installed and in your PATH environment variable.\n\nThe output of this tool is a plot in form of an eps or png-file (as chosen).";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i","mol2,sdf,drf");
	parpars.setSupportedFormats("o","png,eps");
	parpars.parse(argc, argv);

	GenericMolFile* input = MolFileFactory::open(parpars.get("i"));
	String filename1 = "properties.txt";
	bool non_interactive = parpars.has("o");
	if (non_interactive) File::createTemporaryFilename(filename1, ".txt");
	ofstream data(filename1.c_str());

	const String& nf = CommandlineParser::NOT_FOUND;
	String propname1 = parpars.get("p1");
	String propname2 = parpars.get("p2");
	bool use_prop2 = (propname2!=nf);
	bool quiet = (parpars.get("quiet")!=nf);

	Size missing_prop1 = 0;
	Size missing_prop2 = 0;

	multimap<double, double> properties; // sort according to prop1

	Size no_mols = 0;
	for (Molecule* mol = input->read(); mol; delete mol, mol = input->read())
	{
		no_mols++;

		if (!quiet && (no_mols%100) == 0)
		{
			Log.level(20)<<"\r"<<no_mols<<" molecule ...";
			Log.flush();
		}

		if (mol->hasProperty(propname1))
		{
			if (!use_prop2)
			{
				properties.insert(make_pair(((String)mol->getProperty(propname1).toString()).toDouble(), 0));
			}
			else
			{
				if (mol->hasProperty(propname2))
				{
					properties.insert(make_pair(((String)mol->getProperty(propname1).toString()).toDouble(), ((String)mol->getProperty(propname2).toString()).toDouble()));
				}
				else
				{
					missing_prop2++;
				}
			}
		}
		else
		{
			missing_prop1++;
			if (!mol->hasProperty(propname2))
			{
				missing_prop2++;
			}
		}
	}

	if (!use_prop2) // plot no. of occurences
	{
		double min_p1 = 1e20;
		double max_p1 = -1e20;
		for (multimap < double, double > ::iterator it = properties.begin(); it != properties.end(); it++)
		{
			if (it->first < min_p1) min_p1 = it->first;
			if (it->first > max_p1) max_p1 = it->first;
		}
		double stepsize_p1 = (max_p1-min_p1+1)/100;

		vector<Size> no_p1(100, 0);
		for (multimap < double, double > ::iterator it = properties.begin(); it != properties.end(); it++)
		{
			Size bucket_id = (it->first-min_p1)/stepsize_p1;
			if (bucket_id == 100) bucket_id = 99;
			no_p1[bucket_id]++;
		}

		Size i = 0;
		for (vector < Size > ::iterator it = no_p1.begin(); it != no_p1.end(); it++, i++)
		{
			data<<min_p1+i*stepsize_p1<<"\t"<<*it<<endl;
		}
	}
	else  // scatter-plot
	{
		for (multimap < double, double > ::iterator it = properties.begin(); it != properties.end(); it++)
		{
			data<<it->first<<"\t"<<it->second<<endl;
		}
	}
	data.close();

	if (!quiet)
	{
		Log.level(20)<<"\r read "<<no_mols<<" molecules."<<endl;
	}

	if (missing_prop1 > 0)
	{
		Log.level(20)<<"[Warning:] "<<missing_prop1<<" molecules did not contain a property named '"<<propname1<<"."<<endl;
	}
	if (missing_prop2 > 0)
	{
		Log.level(20)<<"[Warning:] "<<missing_prop2<<" molecules did not contain a property named '"<<propname2<<"."<<endl;
	}

	input->close();
	delete input;

	String output_filename = parpars.get("o");
	String filename2 = "plot.txt";
	if (non_interactive) File::createTemporaryFilename(filename2, ".txt");
	ofstream plot(filename2.c_str());
	plot << "unset key"<<endl;
	plot<<"set xlabel \""<<propname1<<"\""<<endl;

	if (non_interactive)
	{
		if (output_filename.hasSuffix(".eps"))
		{
			plot<<"set term postscript eps color enhanced"<<endl;
			plot<<"set output \""<<output_filename<<"\""<<endl;
		}
		else if (output_filename.hasSuffix(".pdf"))
		{
			plot<<"set term pdf enhanced"<<endl;
		}
		else
		{
			plot<<"set term png enhanced size 1024, 650"<<endl;
			plot<<"set output \""<<output_filename<<"\""<<endl;
		}
	}

	if (!use_prop2)
	{
		plot<<"set ylabel \"no. of molecules\""<<endl;
		plot<<"set style fill solid"<<endl;
		plot << "plot \"" << filename1 << "\" w boxes linestyle 3"<<endl;
	}
	else
	{
		plot<<"set ylabel \""<<propname2<<"\""<<endl;
		plot << "plot \"" << filename1 << "\" linestyle 25"<<endl;
	}
	plot.close();

	String c = "gnuplot ";
	if (!non_interactive) c += "-persist ";
	c += String(" < ") + filename2;
	system(c.c_str());

	if (non_interactive)
	{
		File::remove(filename1);
		File::remove(filename2);
	}
}
