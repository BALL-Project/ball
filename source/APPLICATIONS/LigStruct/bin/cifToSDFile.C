

#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
//#include <BALL/MATHS/angle.h>
//#include <BALL/MATHS/vector3.h>
//#include <BALL/MATHS/matrix44.h>


#include <vector>

//#include <BALL/STRUCTURE/geometricProperties.h>
//#include <BALL/MATHS/quaternion.h>
#include <BALL/STRUCTURE/RMSDMinimizer.h>


//#include <BALL/STRUCTURE/geometricTransformations.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/filesystem.hpp>

#include <list>
using namespace OpenBabel;
using namespace BALL;
using namespace std;
using namespace boost::filesystem;

// test path:
// "/Users/pbrach/files/projects/Master-2014_2015/0_data/raw_data/cod/cif/1/00/00"
/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Convert CIF to SDF", " ", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "directory containing CIF files", INFILE, true);
	parpars.registerParameter("o", "single output SDF", OUTFILE, true);
	
	parpars.setSupportedFormats("i","");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "Searches recursively the specified input directory and its "
									"sub directories for CIFs";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// C O D E ##################################
 
	
	// 1.) Find all CIFs ----------------------------------------------------
	Log << "Reading molecules..."<<endl;
	
//	path start_dir ( parpars.get("i").c_str() );
//	exists(start_dir);
//	is_regular_file(start_dir);
//	is_directory(start_dir);
//	try
//	{
//		if( exists(start_dir) )    // does p actually exist?
//		{
//			if ( is_regular_file(start_dir) )        // is p a regular file?
//				cout << start_dir << " is a file" << '\n';
			
//			else if ( is_directory(start_dir) )      // is p a directory?
//			{
//				cout << start_dir << " is a directory\n";
				
////				copy(filesystem::directory_iterator(start_dir), filesystem::directory_iterator(),  // directory_iterator::value_type
////						 ostream_iterator<filesystem::directory_entry>(cout, "\n"));  // is directory_entry, which is
//				// converted to a path by the
//				// path stream inserter
//			}
//			else
//				cout << start_dir << " exists, but is neither a regular file nor a directory\n";
//		}
//		else
//			cout << start_dir << " does not exist\n";
//	}
	
//	catch (const filesystem_error& ex)
//	{
//		cout << ex.what() << '\n';
//	}
	
	
	// 2.) Convert all CIFs ----------------------------------------------------
//	ofstream outfile(parpars.get("o"), ios::out);


	Log << "......done!"<<endl;
}

