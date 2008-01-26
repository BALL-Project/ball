// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/MOLMEC/AMBER/GAFFTypeProcessor.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv){
	
 	String fname = "benzene.hin";
 	if (argc > 1)
 		fname = argv[1];
 	HINFile mol(fname, std::ios::in);

	System S;
	mol >> S;

	AssignBondOrderProcessor abp;
	S.apply(abp);

	GAFFTypeProcessor gt;
	gt.options[GAFFTypeProcessor::Option::ATOMTYPE_FILENAME] = "Amber/AMBERTypes.dat";
	S.apply(gt);
}

