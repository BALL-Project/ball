#include <BALL/FORMAT/molFileFactory.h>

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/MOL2File.h>
//#include <BALL/FORMAT/XYZFile.h>

#include <BALL/DATATYPE/string.h>

namespace BALL
{

GenericMolFile* MolFileFactory::open(const String& name)
{
    String tmp = name.right(5);
    tmp.toLower(0, 5);

    if(tmp.hasSuffix(".pdb")) {
        return new PDBFile(name);
    } else if(tmp.hasSuffix(".hin")) {
        return new HINFile(name);
    } else if(tmp.hasSuffix(".mol")) {
        return new MOLFile(name);
    } else if(tmp.hasSuffix(".mol2")) {
        return new MOL2File(name);
//    } else if(tmp.hasSuffix(".xyz")) {
//        return new XYZFile(name);
    } else {
        return NULL;
    }
}

}

