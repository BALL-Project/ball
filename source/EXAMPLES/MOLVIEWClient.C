#include <BALL/FORMAT/PDBFile.h>
#include <BALL/VIEW/KERNEL/client.h>

using namespace std;
using namespace BALL;
using namespace BALL::VIEW;

int main(int argc, char** argv)
{
	
	// check for command line arguments
	if (argc != 3)
	{
		cerr << argv[0] << " <host>[:<port>] <PDB file>" << endl;
		cerr << "  Reads a PDB file and sends its contents via the network" << endl;
		cerr << "  to the MOLVIEW server listening on <port> of <host> and" << endl;
		cerr << "  displays the contents of the file there." << endl;
		cerr << "  If no port is given, the default port 20000 is assumed." << endl << endl;
		return 1;
	}
	
	// open a file for input
	PDBFile	pdb_file(argv[2]);
	
	if (pdb_file.bad())	
	{
		cerr << "Cannot open PDB file " << argv[2] << endl;
		return 2;
	}

	// create a system and read the contents of the 
	// PDB file into this system
	System S;
	pdb_file >> S;
	pdb_file.close();

	// extract the hostname and port from the command line
	String host_port(argv[1]);
	String host;
	Size port;
	if (host_port.has(':'))
	{
		port = String(host_port.after(":")).toUnsignedInt();
		host = host_port.before(":");
	} 
	else 
	{
		host = host_port;
		port = VIEW_DEFAULT_PORT;
	}
	
	// create a MOLVIEW client and
	// connect to the given host and port
	cout << "connecting to port " << port << " of " << host << endl;
	Client client(host, port);


	// transmit the system
	cout << "sending " << S.countAtoms() << " atoms to " << host << endl;
	client.insert(S);

	// that's it.
	cout << "done." << endl;
	return 0;
}
