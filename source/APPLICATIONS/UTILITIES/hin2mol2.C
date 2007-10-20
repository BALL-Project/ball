// $Id: hin2mol2.C,v 1.2.12.1 2007/03/25 21:32:30 oliver Exp $
//
// A very simple utility for converting HyperChem files into MOL2 files

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/FORMAT/HINFile.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		cerr << "Need one filename! Aborting." << endl;
		return(1);
	}
	
	String infile_name(argv[1]);
	String outfile_name(infile_name);
	outfile_name.reverse();
	outfile_name = outfile_name.after(".");
	outfile_name.reverse();
	outfile_name = outfile_name + ".mol2";

	HINFile hin_file(infile_name);
	System system;
	hin_file >> system;
	hin_file.close();
	cout << "System contains " << system.countAtoms() << " atoms." << endl;

	MOL2File mol2_file(outfile_name, ios::out);
	mol2_file << system;
	mol2_file.close();
	cout << "Wrote MOL2 file " << outfile_name << endl;

	return(0);
}

