#include <BALL/FORMAT/PDBFile.h>
#include <BALL/VIEW/KERNEL/client.h>

using namespace std;
using namespace BALL;
using namespace BALL::VIEW;

int main(int argc, char** argv)
{
	
	// check for command line arguments
	if (argc != 4)
	{
		cerr << argv[0] << " <host> <port> <PDB file>" << endl;
		cerr << "  Reads a PDB file and sends its contents via the network" << endl;
		cerr << "  to the MOLVIEW server listening on <port> of <host> and" << endl;
		cerr << "  displays the contents of the file there." << endl << endl;
		return 1;
	}
	
	// open a file for input
	PDBFile	pdb_file(argv[3]);
	
	if (pdb_file.bad())	
	{
		cerr << "Cannot open PDB file " << argv[3] << endl;
		return 2;
	}

	// create a system and read the contents of the 
	// PDB file into this system
	System S;
	pdb_file >> S;
	pdb_file.close();

	// create a MOLVIEW client and
	// connect to the given host and port
	Size port = String(argv[1]).toInt();
	Client client(argv[2], port);

	// transmit the system
	client.getScene().insert(S);

	// that's it.
	return 0;
}
