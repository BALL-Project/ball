// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularStructure.sip,v 1.1 2004/04/16 13:44:25 amoll Exp $
//

class  MolecularStructure
  : ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/molecularStructure.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static MolecularStructure* getInstance(Position /index/);

  //AmberConfigurationDialog& getAmberConfigurationDialog() throw();
  AmberFF& getAmberFF() throw();
  //CharmmConfigurationDialog& getCharmmConfigurationDialog() throw();
  CharmmFF& getCharmmFF() throw();
  ForceField& getForceField() throw();
  bool checkResidue();
  void centerCamera(Composite*);
  void addHydrogens();
  void amberMDSimulation();
  void buildBonds();
  void buildPeptide();
  void calculateForceFieldEnergy();
  void calculateHBonds();
  void calculateRMSD();
  void calculateSecondaryStructure();
  void checkMenu(MainControl&) throw();
  void chooseAmberFF();
  void chooseCharmmFF();
  void createGridFromDistance();
  //void fetchPreferences(INIFile&) throw();
  //void writePreferences(INIFile&) throw();
  void mapProteins();
  //void onNotify(Message*) throw();
  void runMinimization();
  void setupForceField();
  void showAmberForceFieldOptions();
  void showCharmmForceFieldOptions();
  ~MolecularStructure() throw();
};
