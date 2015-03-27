// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

/* CrystalGenerator.C
*  2012 Stefan Nickels 
*/

// ----------------------------------------------------
// $Maintainer: Stefan Nickels $
// $Authors:    Stefan Nickels $
// ----------------------------------------------------

#include <BALL/XRAY/crystalGenerator.h>
#include <BALL/XRAY/crystalInfo.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/SYSTEM/path.h>

#include "version.h"

using namespace std;
using namespace BALL;

int main(int argc, char* argv[])
{
	// instantiate CommandlineParser object supplying
	// - tool name
	// - short description
	// - version string
	// - build date
	// - category
	CommandlineParser parpars("CrystalGenerator", "creates crystals", "VERSION", String(__DATE__), "Structure Creation");
	
	parpars.registerMandatoryInputFile("i", "input pdb file");
	parpars.registerMandatoryOutputFile("o", "output pdb file");

	// the space group
	parpars.registerMandatoryStringParameter("sg", "space group symbol in Herman-Mauguin notation");
  
	CrystalGenerator generator;
	
	// Fill the space group symbol list
	generator.getSpaceGroupFilename();
	Path path;
	String filename = generator.getSpaceGroupFilename();
	String filepath = path.find(filename);
	if (filepath == "")
	{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
	}

	Parameters* param = new Parameters(filepath);
	if (!param->isValid()) return false;

	ParameterSection* pms = new ParameterSection();
	pms->extractSection(*param, "SpacegroupList");	
	if (!pms->isValid()) return false;
	
	delete param;
			
	list<String> space_groups;
	for (Position i = 0; i < pms->getNumberOfKeys(); i++)
	{
		space_groups.push_back(pms->getValue(i,0).c_str());
	}
	
	delete pms;
					
	parpars.setParameterRestrictions("sg", space_groups);
	
	// the cell axes
	parpars.registerOptionalDoubleParameter("axis_a", "cell axis a");
	parpars.registerOptionalDoubleParameter("axis_b", "cell axis b");
	parpars.registerOptionalDoubleParameter("axis_c", "cell axis c");
  
	// the cell angles
	parpars.registerOptionalDoubleParameter("angle_alpha", "cell angle alpha");
	parpars.setParameterRestrictions("angle_alpha", 0.0, 359.0);
	parpars.registerOptionalDoubleParameter("angle_beta", "cell angle beta");
	parpars.setParameterRestrictions("angle_beta", 0.0, 359.0);
	parpars.registerOptionalDoubleParameter("angle_gamma", "cell angle gamma");
	parpars.setParameterRestrictions("angle_gamma", 0.0, 359.0);
	
	// the unit cell indices to create
	parpars.registerMandatoryIntegerParameter("from_uc_a", "from unit cell index a");
	parpars.setParameterRestrictions("from_uc_a", 0, 9);
	parpars.registerMandatoryIntegerParameter("from_uc_b", "from unit cell index b");
	parpars.setParameterRestrictions("from_uc_b", 0, 9);
	parpars.registerMandatoryIntegerParameter("from_uc_c", "from unit cell index c");
	parpars.setParameterRestrictions("from_uc_c", 0, 9);
	parpars.registerMandatoryIntegerParameter("to_uc_a", "to unit cell index a");
	parpars.setParameterRestrictions("from_uc_a", 0, 9);
	parpars.registerMandatoryIntegerParameter("to_uc_b", "to unit cell index b");
	parpars.setParameterRestrictions("from_uc_b", 0, 9);
	parpars.registerMandatoryIntegerParameter("to_uc_c", "to unit cell index c");
	parpars.setParameterRestrictions("from_uc_c", 0, 9);
  
	String man = "TODO: Manual";
	parpars.setToolManual(man);
	
	parpars.setSupportedFormats("i","pdb");
	parpars.setSupportedFormats("o","pdb");

	parpars.parse(argc, argv);
   
	PDBFile file;
	file.open(parpars.get("i"));
	System system;
	file >> system;

	generator.setSystem(&system);

	boost::shared_ptr<CrystalInfo> ci_ptr(new CrystalInfo());
	ci_ptr->setSpaceGroup(parpars.get("sg"));
	ci_ptr->setCellEdgeLengthA(parpars.get("axis_a").toDouble());
	ci_ptr->setCellEdgeLengthB(parpars.get("axis_b").toDouble());
	ci_ptr->setCellEdgeLengthC(parpars.get("axis_c").toDouble());
	ci_ptr->setCellAngleAlpha(Angle(parpars.get("angle_alpha").toDouble(), false));
	ci_ptr->setCellAngleBeta(Angle(parpars.get("angle_beta").toDouble(), false));
	
	ci_ptr->setCellAngleGamma(Angle(parpars.get("angle_gamma").toDouble(), false));
  
	generator.setCrystalInfo(ci_ptr);
	
	System* output = 0;
	std::list<System*> crystal = generator.generatePacking(parpars.get("from_uc_a").toInt(), parpars.get("to_uc_a").toInt(),
																												 parpars.get("from_uc_b").toInt(), parpars.get("to_uc_b").toInt(),
																												 parpars.get("from_uc_c").toInt(), parpars.get("to_uc_c").toInt());
	output = new System();
	output->setName(system.getName());
	std::list<System*>::iterator it_c = crystal.begin();	

	for(;it_c != crystal.end(); it_c++)
	{
		output->spliceAfter(**it_c);
	}

	PDBFile outfile(parpars.get("o"), ios::out); 
	outfile << *output;
	outfile.close();

	Log << "seems to be done." << endl;

}

