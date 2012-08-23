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
	CommandlineParser parpars("X-Ray CrystalGenerator", "creates crystals", "bla", String(__DATE__), "Structure Creation");
	
  parpars.registerParameter("i",  "input pdb file",    INFILE,  true);
	parpars.registerParameter("o",  "output pdb file",   OUTFILE, true);

  // the space group
	parpars.registerParameter("sg", "space group symbol in Herman-Mauguin notation", STRING, true);
  
  // the cell axes
  parpars.registerParameter("axis_a", "cell axis a", DOUBLE, false);
	//parpars.setParameterRestrictions("axis_a", 0, 100);
	parpars.registerParameter("axis_b", "cell axis b", DOUBLE, false);
	//parpars.setParameterRestrictions("axis_b", 0, 100);
	parpars.registerParameter("axis_c", "cell axis c", DOUBLE, false);
	//parpars.setParameterRestrictions("axis_c", 0, 100);
  
  // the cell angles
	parpars.registerParameter("angle_alpha", "cell angle alpha", DOUBLE, false);
	parpars.setParameterRestrictions("angle_alpha", 0., 359.);
	parpars.registerParameter("angle_beta", "cell angle beta", DOUBLE, false);
	parpars.setParameterRestrictions("angle_beta", 0., 359.);
	parpars.registerParameter("angle_gamma", "cell angle gamma", DOUBLE, false);
	parpars.setParameterRestrictions("angle_gamma", 0., 359.);
	
  // the unit cell indices to create
  parpars.registerParameter("from_uc_a", "from unit cell index a", INT, true);
	parpars.setParameterRestrictions("from_uc_a", 0, 9);
	parpars.registerParameter("from_uc_b", "from unit cell index b", INT, true);
	parpars.setParameterRestrictions("from_uc_b", 0, 9);
	parpars.registerParameter("from_uc_c", "from unit cell index c", INT, true);
	parpars.setParameterRestrictions("from_uc_c", 0, 9);
	parpars.registerParameter("to_uc_a", "to unit cell index a", INT, true);
	parpars.setParameterRestrictions("from_uc_a", 0, 9);
	parpars.registerParameter("to_uc_b", "to unit cell index b", INT, true);
	parpars.setParameterRestrictions("from_uc_b", 0, 9);
	parpars.registerParameter("to_uc_c", "to unit cell index c", INT, true);
	parpars.setParameterRestrictions("from_uc_c", 0, 9);
  
  String man = "Blubber";
	parpars.setToolManual(man);
	
  parpars.setSupportedFormats("i","pdb");

	parpars.parse(argc, argv);
   
  PDBFile file;
  file.open(parpars.get("i"));
  System system;
  file >> system;

  CrystalGenerator generator;
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

