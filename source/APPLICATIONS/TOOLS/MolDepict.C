// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <openbabel/obconversion.h>
#include "version.h"

using namespace BALL;
using namespace std;

void fixSvg(String& svg);
void createPDF(vector<String>& pictures, vector<String>& IDs, vector<String>& scores, vector<String>& MW, String pdf_filename);
BALL::String valueToString(double value);


int main(int argc, char* argv[])
{
	CommandlineParser parpars("MolDepict", "create structure diagrams", VERSION, String(__DATE__), "Convert, combine and store");
	parpars.registerMandatoryInputFile("i", "input file");
	parpars.registerMandatoryOutputFile("o", "output pdf-file");
	parpars.registerOptionalIntegerParameter("max", "maximal number of pictures (default=60, 0=unlimited)", 60);
	String man = "This tool create structure diagrams for small molecules.\nSupported input-formats are mol, mol2, sdf, drf.\n\nOutput of this tool is one pdf-file containing the structure diagrams for all molecules in the input-file.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("o","pdf");
	parpars.parse(argc, argv);

	GenericMolFile* input = MolFileFactory::open(parpars.get("i"), ios::in);

	bool has_max = 0;
	Size max = 0;
	if (parpars.get("max") != CommandlineParser::NOT_FOUND)
	{
		has_max = true;
		max = parpars.get("max").toInt();
		if (max==0) has_max = false;
	}
	Molecule* mol;
	int no_written = 0;
	vector<String> filenames;
	vector<String> IDs;
	vector<String> scores;
	vector<String> MWs;
	for (Size no_mols=0; (mol = input->read()); no_mols++)
	{
		if(has_max && no_mols+1 > max)
		{
			delete mol;
			break;
		}

		OpenBabel::OBMol* obmol = MolecularSimilarity::createOBMol(*mol);
		stringstream streamer;
		OpenBabel::OBConversion conv;
		conv.SetOutFormat("sdf");
		conv.Write(obmol,&streamer);

		String tmp1;
		File::createTemporaryFilename(tmp1, ".svg");
		ofstream of(tmp1.c_str());
		conv.SetOutFormat("svg");
		conv.SetInFormat("sdf");
		bool success = 1;
		try
		{
			conv.Convert(&streamer,&of);
			//conv.Write(obmol,&of);
		}
		catch(...) // In a few cases (for very strange molecules?), openbabel's conversion to svg fails ...
		{
			success = 0;
		}

		delete obmol;
		of.close();
		String tmp2;
		File::createTemporaryFilename(tmp2, ".png");

		if (success)
		{
			fixSvg(tmp1);

			String c = "convert -quality 70% ";
			c += tmp1 + " " + tmp2;
			system(c.c_str());
		}
		File::remove(tmp1);

		// store filename and info about molecule (MW, ID, score) ----
		filenames.push_back(tmp2);
		double score = 0;
		bool has_score = (mol->hasProperty("score"));
		if (has_score) score = ((String)mol->getProperty("score").toString()).toDouble();
		double mw=0;
		for(AtomConstIterator it=mol->beginAtom(); +it; it++)
		{
			mw += it->getElement().getAtomicWeight();
		}
		String ID = "";
		bool has_id = 0;
		for(NamedPropertyIterator it=mol->beginNamedProperty(); it!=mol->endNamedProperty(); it++)
		{
			if(String(it->getName()).hasPrefix("ID"))
			{
				String spacer = ": ";
				ID = it->getName() + spacer + it->toString();
				ID.substituteAll('_',' ');
				has_id = 1;
			}
		}
		int MW = (int)mw;
		MWs.push_back(MW);
		if (has_score)
		{
			scores.push_back(valueToString(score));
		}
		else scores.push_back("");
		if (has_id)
		{
			IDs.push_back(ID);
		}
		else IDs.push_back("");
		// -------------

		delete mol;
		no_written++;
		if (no_written%10 == 0)
		{
			Log.level(20)<<"\r"<<no_written<<" molecules";
			Log.flush();
		}
	}

	createPDF(filenames, IDs, scores, MWs, parpars.get("o"));

	Log.level(20)<<"\r";
	Log.level(20)<<"wrote "<<no_written<<" molecules."<<endl;

	input->close();
	delete input;
}



void fixSvg(String& filename)
{
	LineBasedFile f(filename);
	vector<String> lines;
	while (f.readLine())
	{
		lines.push_back(f.getLine());
	}
	f.close();

	bool begin_svg = 0;
	bool b = 0;
	bool end_svg = 0;
	ofstream out(filename.c_str());

	for (Size i=0; i < lines.size(); i++)
	{
		if (!b && lines[i].hasPrefix("<line")) begin_svg=1;
		else if (lines[i].hasPrefix("</svg")) end_svg=1;

		if (end_svg)
		{
			out << "</g>" << endl;
			end_svg = 0;
		}
		else if(begin_svg)
		{
			out << "<g style=\"stroke:#000000;stroke-width:1;font-family:sans-serif\">" << endl;
			b = 1;
			begin_svg = 0;
		}

		out << lines[i] << endl;
	}
}


void createPDF(vector<String>& pictures, vector<String>& IDs, vector<String>& scores, vector<String>& MW, String pdf_filename)
{
	String tmp;
	File::createTemporaryFilename(tmp,".tex");
	ofstream out(tmp.c_str());
	out << "\\documentclass[a4paper, twoside, 11pt]{report}\n\
	\\usepackage{graphicx}\n\
	\\usepackage{graphics}\n\
	\\usepackage{geometry}\n\
	\\usepackage{float}\n\
	\\geometry{a4paper,left=8mm,right=8mm, top=10mm, bottom=8mm}\n\
	\\begin{document}\n\
	\\def\\imagetop#1{\\vtop{\\null\\hbox{#1}}}\n\
	\\begin{table}[H]\n\
	\\centering \n\
	\\begin{tabular}{ccc}"<<endl;

	bool row_has_IDs = false;
	for (Size i=0; i<pictures.size(); i++)
	{
		if (i%3 == 0)  // print header containing molecule-IDs
		{
			row_has_IDs = false;
			if (i>0) out << "\\\\ " << endl;
			for (Size j=i; j<=i+2 && j<pictures.size(); j++)
			{
				out << "\\#" << (j+1);
				out << "\\hspace*{0.3cm} " << MW[j] << "g/mol";
				if (scores[j]!="") out<<"\\hspace*{0.2cm} "<<scores[j]<<" kJ/mol ";
				if (IDs[j]!="") row_has_IDs = true;
				if (j<i+2) out<<"  &  ";
			}
			if (row_has_IDs)
			{
				out << "\\\\ " << endl;
				for (Size j=i; j<=i+2 && j<pictures.size(); j++)
				{
					if (IDs[j]!="") out << IDs[j];
					if (j<i+2) out<<"  &  ";
				}
			}
			out << "\\\\ \\hline " << endl;
		}

		if (ifstream(pictures[i].c_str()))
		{
			String height = "3.8cm";
			if (!row_has_IDs) height="4.2cm";
			out << "\\imagetop {\\includegraphics[width=0.3\\textwidth,height=" << height << ",keepaspectratio=true]{" << pictures[i] << "}}";
		}
		else
		{
			out << "picture could not be created." << endl;
		}

		if(i%3 != 2 && i!=pictures.size()-1) out << " &  ";
		else out << "  \\\\ " << endl;

		if((i+1)%15==0)
		{
			out << "\\end{tabular}\n\
			\\end{table}\n\
			\\newpage \n \
			\\begin{table}[H]\n\
			\\centering\n \
			\\begin{tabular}{ccc}" << endl;
		}
		out << endl;
	}

	out<<"\\end{tabular}"<<endl;
	out<<"\\end{table}"<<endl;
	out<<"\\end{document}"<<endl;
	out<<endl;
	out.close();

	system(("pdflatex "+tmp).c_str());
	String c = "mv ";
	// for some weird reason, we must cast to BALL::String for this to compile...
	// the overload of operator + is not recognized for BALL::Substring in Debug mode
	c += ((BALL::String)tmp.before(".tex"))+".pdf ";
	c += pdf_filename;
	// FIXME: This might not work in Windows! (in windows you need the "move" command, not "mv")
	system(c.c_str());

	for (Size i=0; i<pictures.size(); i++)
	{
		File::remove(pictures[i]);
	}
	File::remove(tmp);
	File::remove(((BALL::String)tmp.before(".tex"))+".log");
	File::remove(((BALL::String)tmp.before(".tex"))+".aux");
}

BALL::String valueToString(double value)
{
	if (value>1e7)
	{
		int b = log10(value);
		int f = value/pow((double)10,b);
		String s = String(f) + String("e") + String(b);
		return s;
	}
	BALL::String t(value);
	int index = t.find_last_of(".");
	if (index!=-1)
	{
		if (index+1<=(int)t.size())
			index++;
		if (index+1<=(int)t.size())
			index++;
		t =t.substr(0,index);
	}
	return t;
}
