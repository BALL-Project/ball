
#include <BALL/FORMAT/DBInterface.h>
#include <QtGui/QApplication>

using namespace BALL;

DBInterface db;

bool getStructure(DBInterface::ID id, const String& method, const String& parameters, System& system)
{
	std::cout << "Retrieving topology for ID " << id << std::endl;
	db.getTopology(id, system);

	std::cout << "Retrieving method id for " << method << "/" << parameters << std::endl;
	DBInterface::ID method_id = db.getConformationMethod(method, parameters);
	
	std::cout << "Retrieving conformation list for method " << method_id << std::endl;
	DBInterface::IDVector conf_ids = db.getConformationList(id, method_id);
	if (conf_ids.empty())
	{
		std::cerr << "no conformation stored in database for this structure!" << std::endl;
		return false;
	}
	std::cout << "Retrieved " << conf_ids.size() << " conformations" << std::endl;
	std::cout << "Loading conformation #" << conf_ids[0] << std::endl;
	db.loadConformation(conf_ids[0], system);

	return true;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << argv[0] << " <id>" << std::endl;
		return 1;
	}
	QApplication app(argc, argv, false);
	std::cout << "Connecting to database..." << std::endl;
	db.connect();
	if (!db.good())
	{
		std::cerr << "Could not open database!" << std::endl;
		return 1;
	}

	// Get the structure with the default conformation
	System s;
	DBInterface::ID id = String(argv[1]).toInt();
	if (getStructure(id, "default", "default", s))
	{	
		// Write the file
		String filename(30, "DB%08d.mol", id);
		std::cout << "Writing MOL file as " << filename << std::endl;
		MOLFile out(filename, File::MODE_OUT);
		out << s;
		out.close();
	}

	return 0;
}
