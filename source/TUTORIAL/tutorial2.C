// $Id: tutorial2.C,v 1.2 2000/03/09 22:27:19 oliver Exp $
// tutorial example 2
// ------------------
// read BPTI from a PDB file, print its sequence
// (use of PDBFile, ResidueIterator)

// needed for cout
#include <iostream>

// the BALL kernel classes
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/system.h>

// reading and writing of PDB files
#include <BALL/FORMAT/PDBFile.h>

// we use the BALL namespace  and the std namespace (for cout and endl)
using namespace BALL;
using namespace std;

int main()
{
	// create a PDBFile object
	PDBFile infile("bpti.pdb");
	
	// create a system
	System S;

	// read the contents of bpti.pdb into the system 
	infile >> S;

	// close the file
	infile.close();


	ResidueIterator res_it;
	for (res_it = S.beginResidue(); 
			 res_it != S.endResidue();
			 ++res_it)
	{
		cout << res_it->getName() << endl;
	}
}
