// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: export_fragment.C,v 1.3 2004/05/27 18:13:08 oliver Exp $
//

#include <fstream>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>

using namespace BALL;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		Log.error() << "I need two filenames: HIN/PDB input, FragmentDB output." 
			<< std::endl;
		return(1);
	}

	String infilename(argv[1]);

	std::vector<String> substrings;
	infilename.split(substrings, ".");

	System system;

	if ((substrings[substrings.size()-1] == "pdb")
		|| (substrings[substrings.size()-1] == "PDB"))
	{
		PDBFile infile(infilename);
		infile >> system;
		infile.close();
	}
	else
	{
		if ((substrings[substrings.size()-1] == "hin")
				|| (substrings[substrings.size()-1] == "HIN"))
		{
			HINFile infile(infilename);
			infile >> system;
			infile.close();
		}
		else
		{
			Log.error() << "I only understand HyperChem .hin and PDB .pdb files."
				<< std::endl;
		}
	}

	String outfilename(argv[2]);

	std::ofstream outfile(outfilename.c_str(), std::ios::out);

	outfile << "<node>Unknown Compound" << std::endl;
	outfile << "\t<node>Names" << std::endl;
	outfile << "\t\t<node>Unknown Name</node>" << std::endl;
	outfile << "\t</node>" << std::endl;
	outfile << "\t<node>Atoms" << std::endl;
	AtomConstIterator atom_it = system.beginAtom();
	for (; +atom_it; ++atom_it)
	{
		outfile << "\t\t<node>" << atom_it->getName() << "<value>"
			<< atom_it->getElement().getSymbol() << " "
			<< atom_it->getPosition().x << " "
			<< atom_it->getPosition().y << " "
			<< atom_it->getPosition().z << "</value></node>" << std::endl;
	}
	outfile << "\t</node>" << std::endl;
	outfile << "\t<node>Bonds" << std::endl;
	Size counter = 1;
	char bond_order_char;
	AtomBondConstIterator bond_it;
	BALL_FOREACH_BOND(system, atom_it, bond_it)
	{
		switch (bond_it->getOrder())
		{
			case Bond::ORDER__SINGLE: bond_order_char = 's'; break;
			case Bond::ORDER__DOUBLE: bond_order_char = 'd'; break;
			case Bond::ORDER__TRIPLE: bond_order_char = 't'; break;
			case Bond::ORDER__QUADRUPLE: bond_order_char = 'q'; break;
			case Bond::ORDER__AROMATIC: bond_order_char = 'a'; break;
		}
		outfile << "\t\t<node>" << counter++ << "<value>"
			<< bond_it->getFirstAtom()->getName() << " "
			<< bond_it->getSecondAtom()->getName() << " "
			<< bond_order_char << "</value></node>" << std::endl;
	}
	outfile << "\t</node>" << std::endl;
	outfile << "</node>" << std::endl;

}
