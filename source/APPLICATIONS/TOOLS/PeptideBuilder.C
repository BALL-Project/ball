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
	parpars.registerMandatoryInputFile("i", "input torsion-file");
	parpars.registerMandatoryOutputFile("o", "peptide output pdb-file");

	String man = "This tool creates a peptide by a given torsion file. The amino acids shall be given in three letter code, the phi, psi, and omega angles shall be given in degree.\n\nExample:\n\n# aa   phi    psi   omega\n\nA    -180    140\n\nC    -180    180\n\nG    -90    -140\n\nP    -65    -40       0   # cis\n\nT    -120    -90\n\nP    -78     146     180  # trans";

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
			fields.clear();
			aa_torsion.split(fields);

			// split and check
			// either expect a line
			//    G
			//    A    -180    140
			// or
			//    A    -180    140   180 # some comment
			if (   (fields.size() != 1)
					&& (   (fields.size()<3)
					    || ((fields.size()>3) && (fields[3] != "#") && (fields[4] != "#")) ))
			{
				Log.error() << "Error in while reading file " << parpars.get("i") << " in line " << aa_torsion << endl;
			}

			String aa = fields[0];

			// assume a helix if no angles given
			float phi = 300;
			float psi = 310;
			float omega = 0.;

			if (fields.size()>1)
			{
				phi = fields[1].toFloat();
				psi = fields[2].toFloat();
			}

			if ((fields.size()>4) && (fields[3] != "#"))
				omega = fields[3].toFloat();

			char test_aa = aa.toChar();

			if (Peptides::IsOneLetterCode(test_aa))
			{
				aa = Peptides::ThreeLetterCode(test_aa);
			}

			// check 
			// we need three letter code, angles should be given in degree (phi than psi)
			if (!Peptides::IsThreeLetterCode(aa))
			{
				Log.error() << "Error while reading line " << aa_torsion << " : amino acid shall be given in three letter code." << endl;
			}

			// append the peptide 
			pb->addAminoAcid(aa, Angle(phi, false), Angle(psi, false), Angle(omega, false));
		}
	}

  // now build the peptide 
  Protein* prot = pb->construct();

  cout << " a peptide "  << Peptides::GetSequence(*prot) << endl;
  cout << " was build";

/*
	for (ResidueIterator res_it= prot->beginResidue(); +res_it; ++res_it)
	{
		cout << res_it->getName() << " " << res_it->getTorsionPhi().toDegree() << " " << res_it->getTorsionPsi().toDegree() << " " << res_it->getTorsionOmega().toDegree()  << endl;
	}
*/

	// give sensible names	
	String pep_name = parpars.get("peptide_name");
  pb->setChainName(pep_name);
  pb->setProteinName(pep_name);

  // normalize the names
  prot->apply(fdb.normalize_names);

  // build the missing bonds
  prot->apply(fdb.build_bonds);

  // add missing hydrogens
  prot->apply(fdb.add_hydrogens);

	// store	
	PDBFile out_file(parpars.get("o"), std::ios::out);
	out_file << *prot;
	out_file.close();

	cout << " and stored as " << parpars.get("o") << endl;
	return 0;
}

