// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: splitMMFFTestSuiteMolFiles.C,v 1.1.8.1 2007/03/25 21:32:22 oliver Exp $
//
// A small program for adding hydrogens to a PDB file (which usually comes
// without hydrogen information) and minimizing all hydrogens by means of a
// conjugate gradient minimizer.

#include <BALL/common.h>

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/file.h>


using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		Log.error() << "Usage: splitMMFFTestSuiteMolFiles <location of MMFF94_dative.mol2> <folder to write mol2 files>" << std::endl;
		return 1;
	}

	vector<String> file_contents;
	String file_name;
	String dir = argv[2];
	bool get_filename = false;
	vector<String> file_names;

	LineBasedFile infile(argv[1]);
	while (infile.readLine())
	{
		if (infile.getLine().hasSuffix("MOLECULE"))
		{
			if (!file_contents.empty() && file_name != "")
			{
				File outfile(dir + FileSystem::PATH_SEPARATOR + file_name + ".mol2", std::ios::out);
				vector<String>::iterator vit = file_contents.begin();
				while (vit != file_contents.end())
				{
					outfile << *vit << std::endl;
					vit++;
				}

				outfile.close();
			}
			file_contents.clear();
			get_filename = true;
			file_name = "";
			file_contents.push_back(infile.getLine());
			continue;
		}

		if (get_filename)
		{
			file_name = infile.getLine();
			file_names.push_back(file_name);
			Log.info() << "Found mol2 file for " << file_name << std::endl;
			get_filename = false;
		}

		file_contents.push_back(infile.getLine());
	}

	File outfile(dir + FileSystem::PATH_SEPARATOR + "filenames.txt", std::ios::out);
	vector<String>::iterator vit = file_names.begin();
	while (vit != file_names.end())
	{
		outfile << *vit << std::endl;
		vit++;
	}

	Log.info() << "Extracted " << file_names.size() << " files" << std::endl;

	outfile.close();

	return 0;
}
