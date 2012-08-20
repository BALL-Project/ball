// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/commandlineParser.h>

#include <BALL/MATHS/angle.h>

#include <BALL/STRUCTURE/peptideBuilder.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/peptides.h>

#include <vector>
#include <iostream>
#include "version.h"

using namespace std;
using namespace BALL;

void usage (const char *progname);

int main (int argc, char **argv)
{
	CommandlineParser parpars("PeptideBuilder", "build a peptide  ", VERSION, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input torsion-file", INFILE, true);
	parpars.registerParameter("o", "peptide output pdb-file", OUTFILE, true);
	parpars.registerParameter("peptide_name", "name of the peptide", STRING, true);

	String man = "This tool creates a peptide by a given torsion file. The amino acids shall be given in three letter code, the phi and psi angles shall be given in degree.";

	parpars.setToolManual(man);
	parpars.setSupportedFormats("i", "txt");
	parpars.setSupportedFormats("o", "pdb");
	parpars.parse(argc, argv);

	// initialize the peptide builder
	BALL::Peptides::PeptideBuilder* pb = new BALL::Peptides::PeptideBuilder;

	// "link" the fragment db for adding missing information
  FragmentDB fdb("");
  pb->setFragmentDB(&fdb);

	// read the torsion file (format AA, PHI, PSI)
	// e.g. ALA 109 65
	LineBasedFile torsion_file(parpars.get("i"), std::ios::in);
	vector<String> fields;
	while (torsion_file.LineBasedFile::readLine())
	{
		// get the line
		String aa_torsion = torsion_file.getLine();

		if ((aa_torsion.countFields(" ")>0) && (aa_torsion(0,1) != "#"))
		{
			// split and check
			if (aa_torsion.split(fields) != 3)
			{
				Log.error() << "Error in while reading file " << parpars.get("i") << " in line " << aa_torsion << endl;
			}

			String aa = fields[0];
			float phi = fields[1].toFloat();
			float psi = fields[2].toFloat();
			char  test_aa = aa.toChar();

			if (Peptides::IsOneLetterCode(test_aa))
			{
				aa = Peptides::ThreeLetterCode(test_aa);
			}

			// check 
			// we need three letter code, angles should be given in degree (phi than psi)
			if (not Peptides::IsThreeLetterCode(aa))
			{
				Log.error() << "Error while reading line " << aa_torsion << " : amino acid shall be given in three letter code." << endl;
			}

			// append the peptide 
			pb->addAminoAcid(aa, Angle(phi, false), Angle(psi, false));
		}
	}

  // now build the peptide 
  Protein* prot = pb->construct();

  cout << "a peptide was build: " <<  Peptides::GetSequence(*prot) << endl;

	// give sensible names	
	String pep_name = parpars.get("peptide_name");
  pb->setChainName(pep_name);
  pb->setProteinName(pep_name);

	// store	
	PDBFile out_file(parpars.get("o"), std::ios::out);
	out_file << *prot;
	out_file.close();

	return 0;
}

