#include <BALL/FORMAT/molFileFactory.h>

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/antechamberFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/XYZFile.h>

#include <BALL/DATATYPE/string.h>

namespace BALL
{

	GenericMolFile* MolFileFactory::open(const String& name, File::OpenMode open_mode)
	{
		String tmp = name.right(5);
		tmp.toLower(0, 5);
		
		if (tmp.hasSuffix(".ac")) {
			return new AntechamberFile(name, open_mode);
		} else if(tmp.hasSuffix(".pdb") || tmp.hasSuffix(".ent") || tmp.hasSuffix(".brk")) {
			return new PDBFile(name, open_mode);
		} else if(tmp.hasSuffix(".hin")) {
			return new HINFile(name, open_mode);
		} else if(tmp.hasSuffix(".mol")) {
			return new MOLFile(name, open_mode);
		} else if(tmp.hasSuffix(".sdf")) {
			return new SDFile(name, open_mode);
		} else if(tmp.hasSuffix(".mol2")) {
			return new MOL2File(name, open_mode);
		} else if(tmp.hasSuffix(".xyz")) {
			return new XYZFile(name, open_mode);
		} else {
			return NULL;
		}
	}

}

