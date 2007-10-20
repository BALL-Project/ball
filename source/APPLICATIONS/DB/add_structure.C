

#include <BALL/FORMAT/DBInterface.h>
#include <QtGui/QApplication>

using namespace BALL;

int main(int argc, char** argv)
{
	if (argc < 5 || argc > 6)
	{
		std::cerr << argv[0] << "<MOL file> <name> <source_id> <method_id> [<id>]"
		<< "\n"
		<< "where <MOL file>   is the MDL MOL file to be read (containing topology and conformation),\n"
		<< "      <name>       is the name of the compound or empty (\"\"),\n"
		<< "      <source_id>  is a string describing the source of the compound, e.g. KEGG43:C0001,\n"
    << "      <method_id>  is the database ID of the conformation generation method, (1 is default/default)\n"
    << "      <id>         is the database ID under which the topology should be stored (primary key)." << std::endl;
		return 1;
	}

	QApplication app(argc, argv, false);
	std::cout << "Connecting to database..." << std::endl;
	

	DBInterface db;
	db.connect();
	if (!db.good())
	{
		std::cerr << "Could not open database!" << std::endl;
		std::cerr << "Check the settings in ~/.ballrc." << std::endl;
		return 1;
	}

	MOLFile mf(argv[1]);
	System sys;
	mf >> sys;
	mf.close();
	std::cout << "Structure contains " << sys.countAtoms() << " atoms and " << sys.countBonds() << " bonds." << std::endl;

	// Make sure the specified conformation generation method exists
	DBInterface::ID method_id = String(argv[4]).toInt();
	DBInterface::ConformationMethod method = db.getConformationMethod(method_id);
	if (method.first == "" && method.second == "")
	{
		std::cerr << "unknown conformation generation method: #" << method_id << std::endl;
		return 3;
	}

	DBInterface::ID id = 0;
	if (argc == 4)
	{	
		id = db.newTopology(sys, String(argv[2]), String(argv[3]));		
	}
	else
	{
		id = db.newTopology(sys, String(argv[2]), String(argv[3]), String(argv[5]).toInt());
	}

	std::cout << "Stored topology." << std::endl;

	// Create a new conformation for the 
	db.storeConformation(id, method_id, sys);
	
	std::cout << "Stored structure from " << argv[2] << " as #" << id << std::endl;
	return 0;
}
