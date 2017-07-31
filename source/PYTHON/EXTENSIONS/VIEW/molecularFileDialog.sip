class MolecularFileDialog
	: ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
%End
	public:

	static Size countInstances();
	static MolecularFileDialog* getInstance(Position);

	~MolecularFileDialog() throw();
	virtual void initializeWidget(MainControl&) throw();
	virtual void readFiles();
	virtual System* openMolecularFile(const String&) throw();
	virtual System* openMolecularFile(const String&, const String&,
		const String&) throw();
	virtual bool writeFile();
	System* readPDBFile(String, String) throw();
	System* readHINFile(String, String) throw();
	System* readMOL2File(String, String) throw();
	System* readMOLFile(String, String) throw();
	System* readSDFile(String, String) throw();
	bool writePDBFile(String, const System&) throw();
	bool writeHINFile(String, const System&) throw();
	bool writeMOLFile(String, const System&) throw();
	bool writeMOL2File(String, const System&) throw();
	bool writeSDFile(String, const System&) throw();

	private:
	MolecularFileDialog(const MolecularFileDialog&);
};
