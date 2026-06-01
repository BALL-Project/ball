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
	// Axis/value limits (FEATPLOT-01, #524). These are numeric, but we register them
	// as optional *string* parameters with an empty default on purpose: a double
	// parameter would stringify its 0.0 default into the parameter map (see
	// CommandlineParser::checkAndRegisterParameter), making 0.0 indistinguishable
	// from "unset". An empty string default keeps the parameter out of the map until
	// the user supplies it, so the NOT_FOUND sentinel reliably means "not given" —
	// and 0.0 remains a legal, honored limit. We parse the value with toDouble().
	parpars.registerOptionalStringParameter("p1_min", "lower limit for the property 1 axis");
	parpars.registerOptionalStringParameter("p1_max", "upper limit for the property 1 axis");
	parpars.registerOptionalStringParameter("p2_min", "lower limit for the property 2 axis");
	parpars.registerOptionalStringParameter("p2_max", "upper limit for the property 2 axis");
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

	// User-supplied axis/value limits (FEATPLOT-01, #524). Detect presence via the
	// NOT_FOUND sentinel rather than the 0.0 default, because 0.0 is a legal limit.
	String p1_min_str = parpars.get("p1_min");
	String p1_max_str = parpars.get("p1_max");
	String p2_min_str = parpars.get("p2_min");
	String p2_max_str = parpars.get("p2_max");
	bool has_p1_min = (p1_min_str!=nf);
	bool has_p1_max = (p1_max_str!=nf);
	bool has_p2_min = (p2_min_str!=nf);
	bool has_p2_max = (p2_max_str!=nf);
	double p1_min_val = has_p1_min ? p1_min_str.toDouble() : 0.0;
	double p1_max_val = has_p1_max ? p1_max_str.toDouble() : 0.0;
	double p2_min_val = has_p2_min ? p2_min_str.toDouble() : 0.0;
	double p2_max_val = has_p2_max ? p2_max_str.toDouble() : 0.0;

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
			double prop1_value = ((String)mol->getProperty(propname1).toString()).toDouble();

			// Drop points whose prop1 falls outside the requested limits (FEATPLOT-01).
			if ((has_p1_min && prop1_value < p1_min_val) || (has_p1_max && prop1_value > p1_max_val))
			{
				continue;
			}

			if (!use_prop2)
			{
				properties.insert(make_pair(prop1_value, 0));
			}
			else
			{
				if (mol->hasProperty(propname2))
				{
					double prop2_value = ((String)mol->getProperty(propname2).toString()).toDouble();

					// In scatter mode the prop2 limits clamp the y axis as well.
					if ((has_p2_min && prop2_value < p2_min_val) || (has_p2_max && prop2_value > p2_max_val))
					{
						continue;
					}

					properties.insert(make_pair(prop1_value, prop2_value));
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

		// When the user requested explicit p1 limits, span the 100 histogram buckets
		// over exactly that domain instead of the scanned data extremes (FEATPLOT-01).
		if (has_p1_min) min_p1 = p1_min_val;
		if (has_p1_max) max_p1 = p1_max_val;

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

	// Emit explicit axis ranges only when the user supplied a limit. The values are
	// written as numeric doubles formatted by the stream (never as raw user strings),
	// so no shell/gnuplot injection is possible (threat T-999.74-01). An unset side
	// uses gnuplot's '*' autorange token. Nothing is emitted when no limit is set,
	// keeping default output byte-identical (FEATPLOT-01, #524).
	if (has_p1_min || has_p1_max)
	{
		plot<<"set xrange [";
		if (has_p1_min) plot<<p1_min_val; else plot<<"*";
		plot<<":";
		if (has_p1_max) plot<<p1_max_val; else plot<<"*";
		plot<<"]"<<endl;
	}

	if (!use_prop2)
	{
		plot<<"set ylabel \"no. of molecules\""<<endl;
		plot<<"set style fill solid"<<endl;
		plot << "plot \"" << filename1 << "\" w boxes linestyle 3"<<endl;
	}
	else
	{
		// In scatter mode the prop2 limits clamp the y axis.
		if (has_p2_min || has_p2_max)
		{
			plot<<"set yrange [";
			if (has_p2_min) plot<<p2_min_val; else plot<<"*";
			plot<<":";
			if (has_p2_max) plot<<p2_max_val; else plot<<"*";
			plot<<"]"<<endl;
		}
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
