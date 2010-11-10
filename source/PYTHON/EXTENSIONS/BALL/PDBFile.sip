// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class PDBFile : GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/PDBFile.h>
  #include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  PDBFile();
	// default open mode: READ
  PDBFile(const String&, OpenMode /openmode/ = std::ios::in) throw(FileNotFound);
	~PDBFile();
	void selectAltLocationIndicator(char ch);
  virtual bool read(Protein& /protein/) throw (ParseError);
  virtual bool read(Molecule& /molecule/) throw (ParseError);
  virtual bool read(System& /system/) throw (ParseError);
  virtual bool write(const Protein& /protein/) throw(File::CannotWrite);
  virtual bool write(const Molecule& /protein/) throw(File::CannotWrite);
  virtual bool write(const System& /system/) throw(File::CannotWrite);

	//Actually all of the following methods are non virtual
	//However we need to declare them as virtual to trick sip into
	//producing correct code
  PDBFile& operator >> (Protein& /protein/) throw (ParseError);
  virtual PDBFile& operator >> (Molecule& /molecule/) throw (ParseError);
  virtual PDBFile& operator >> (System& /system/) throw (ParseError);
  PDBFile& operator << (const Protein& /protein/) throw(File::CannotWrite);
  virtual PDBFile& operator << (const Molecule& /molecule/) throw(File::CannotWrite);
  virtual PDBFile& operator << (const System& /system/) throw(File::CannotWrite);

	void close();
	private:
		PDBFile(const PDBFile&);
};
