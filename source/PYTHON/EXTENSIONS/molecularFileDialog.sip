// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularFileDialog.sip,v 1.1 2004/02/12 13:02:22 amoll Exp $
//

class MolecularFileDialog
  : Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  BALL_EMBEDDABLE(MolecularFileDialog,);

// methods implemented by the BALL_EMBEDDED macro
static Size countInstances();
static MolecularFileDialog* getInstance(Position /index/);

  bool readHINFile(String, String) throw();
  bool readMOL2File(String, String) throw();
  bool readMOLFile(String, String) throw();
  bool readPDBFile(String, String) throw();
  bool writeFile();
  bool writeHINFile(String, const System&) throw();
  bool writeMOL2File(String, const System&) throw();
  bool writeMOLFile(String, const System&) throw();
  bool writePDBFile(String, const System&) throw();
  void readFile();
//  void checkMenuEntries() throw();
//  void fetchPreferences(INIFile &inifile) throw();
  void finalizeWidget(MainControl&) throw();
  void initializeWidget(MainControl&) throw();
  void openFile(const String&) throw();
  void openFile(const String&, const String&, const String&) throw();
//  void writePreferences(INIFile &inifile) throw();
  ~MolecularFileDialog() throw();
};
