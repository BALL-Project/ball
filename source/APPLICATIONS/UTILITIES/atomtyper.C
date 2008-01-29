// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOL2File.h>
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

	Options options;
	options[GAFFTypeProcessor::Option::ATOMTYPE_FILENAME] = "Amber/GAFFTypes.dat";
	if (argc > 2)
		options[GAFFTypeProcessor::Option::ATOMTYPE_FILENAME] = String("Amber/")+argv[2];

	GAFFTypeProcessor gt(options);
	S.apply(gt);
	MOL2File m("rockaz.mol2", std::ios::out);
	m << S;
	m.close();
}

