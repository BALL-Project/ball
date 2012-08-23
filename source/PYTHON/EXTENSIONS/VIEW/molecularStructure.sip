// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
	static MolecularStructure* getInstance(Position);

  //AmberConfigurationDialog& getAmberConfigurationDialog() throw();
  AmberFF& getAmberFF() throw();
  //CharmmConfigurationDialog& getCharmmConfigurationDialog() throw();
  CharmmFF& getCharmmFF() throw();
  ForceField& getForceField() throw();
  bool checkResidue();
  void centerCamera(Composite*);
  void addHydrogens();
  void MDSimulation(bool);
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
  void runMinimization(bool);
  void setupForceField();
  void showAmberForceFieldOptions();
  void showCharmmForceFieldOptions();
	void calculateFDPB(bool = true);
  ~MolecularStructure() throw();

	private:
		MolecularStructure(const MolecularStructure&);
};
