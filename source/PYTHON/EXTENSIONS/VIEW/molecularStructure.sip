class  MolecularStructure
	: ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/molecularStructure.h>
%End
	public:

	static Size countInstances();
	static MolecularStructure* getInstance(Position);

	AmberFF& getAmberFF() throw();
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
	void mapProteins();
	void runMinimization(bool);
	void setupForceField();
	void showAmberForceFieldOptions();
	void showCharmmForceFieldOptions();
	void calculateFDPB(bool = true);
	~MolecularStructure() throw();

	private:
	MolecularStructure(const MolecularStructure&);
};
