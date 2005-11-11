// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularFileDialog.sip,v 1.2.6.1 2005/11/11 17:21:24 leonhardt Exp $
//

class MolecularFileDialog
  : ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static MolecularFileDialog* getInstance(Position /index/);

  ~MolecularFileDialog() throw();
  virtual void initializeWidget(MainControl&) throw();
  virtual void readFiles();
  virtual System* openFile(const String&) throw();
  virtual System* openFile(const String&, const String&, const String&) throw();
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
};
