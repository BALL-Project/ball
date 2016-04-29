// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser par("PDBDownload", "retrieve pdb-file from pdb.org", VERSION, String(__DATE__), "Get Data");
	par.registerMandatoryStringParameter("id", "PDB ID for desired structure");
	par.registerMandatoryOutputFile("o", "output file");
	par.registerOptionalStringParameter("p", "proxy");
	String man("Download a pdb-file from the pdb data bank (http://www.pdb.org/) using the specified ID of the desired protein structure.");
	par.setToolManual(man);
	par.setSupportedFormats("o","pdb");
	par.parse(argc, argv);

	String prefix("http://files.rcsb.org/download/");
	String id = par.get("id");
	String url = prefix;
	url += id;
	url += ".pdb ";

	String command = "";
	#ifndef BALL_OS_DARWIN
		if (par.has("p"))
		{
			command += String("export http_proxy=") + par.get("p") + String(" > /dev/null 2> /dev/null; ");
			command += String("setenv http_proxy ") + par.get("p") + String(" > /dev/null 2> /dev/null; ");
		}
		command += String("wget ");
		command += url + String(" -O ") + par.get("o") + String(" > /dev/null 2> /dev/null; ");
	#else
		command = String("curl ");
		command += String("-o ") + par.get("o");
		if (par.has("p")) command += String(" --proxy1.0 ") + par.get("p");
		command += String(" ") + url + String(" > /dev/null 2> /dev/null; ");
	#endif

	int status = system(command.c_str());
	if (status > 0)
	{
		cerr << "[Error:] Could not establish network connection. Perhaps you need a proxy (or the specified proxy is incorrect). " << endl << endl;
		return 1;
	}

	// Check if we really got a pdb-file or just a file containing an error message
	LineBasedFile file(par.get("o"));
	bool header_found = false;
	bool error_message_found = false;
	for (Size i=0; i < 10 && file.readLine(); i++)
	{
		if (file.getLine().hasSubstring("HEADER")) header_found = true;
		if (file.getLine().hasSubstring("the requested file is not available")) error_message_found = true;
	}

	if (error_message_found)
	{
		cerr << "[Error:] A pdb-structure with the given pdb-ID does not exist!" << endl << endl;
		return 1;
	}
	if (!header_found)
	{
		cerr << "[Error:] The specified URL does not contain a valid pdb-file (no 'HEADER' found)!" << endl << endl;
		File::remove(par.get("o"));
		return 1;
	}

	Log.level(20) << "PDB-file " << par.get("id") << " sucessfully downloaded to " << par.get("o") << endl << endl;
	return 0;
}
