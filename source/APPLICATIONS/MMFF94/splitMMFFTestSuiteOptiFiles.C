// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: splitMMFFTestSuiteOptiFiles.C,v 1.1.2.2 2005/03/22 16:32:53 amoll Exp $
//
// A small program for spliting the Optimol log file from the MMFF94 test suite
// into smaller files, which are better to handle for parsing 

#include <BALL/common.h>

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/file.h>


using namespace std;
using namespace BALL;

void getAtomData(LineBasedFile& file, vector<String>& data)
{
	while (file.readLine())
	{
		if (file.getLine().hasSubstring("OPTIMOL-LIST")) 
		{
			file.skipLines(1);
			return;
		}

		vector<String> fields;
		Size size = file.getLine().split(fields);
		Position pos = 0; 

		while (pos < size)
		{
			data.push_back(fields[pos + 2]);
			pos += 3;
		}
	}
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		Log.error() << "Usage: splitMMFFTestSuiteOptiFiles <location of MMFF94_opti.log> <folder to write files>" << std::endl;
		return 1;
	}

	vector<String> atoms_file_contents;
	String file_name;
	String dir = argv[2];
	vector<String> file_names;

	LineBasedFile infile(argv[1]);
	while (infile.readLine())
	{
		if (infile.getLine().hasSubstring("Structure Name:") ||
				infile.getLine().hasSubstring("New Structure Name"))
		{
			file_name = infile.getLine().after(": ");
			while (!infile.getLine().hasSubstring("ATOM NAME"))
			{
				infile.readLine();
			}

			vector<String> atoms, names, types, symbols, charges, fcharges;

			while (infile.readLine())
			{
				if (infile.getLine().hasSubstring("OPTIMOL-LIST")) break;
				vector<String> fields;
				Size size = infile.getLine().split(fields);
				Position pos = 0; 

				while (pos < size)
				{
					atoms.push_back(fields[pos]);
					names.push_back(fields[pos + 1]);
					types.push_back(fields[pos + 2]);
					pos += 3;
				}
			}

			infile.skipLines(1);
	
			getAtomData(infile, symbols);
			getAtomData(infile, charges);
			getAtomData(infile, fcharges);
			

			File outfile(dir + FileSystem::PATH_SEPARATOR + file_name + ".atoms", std::ios::out);
			Position pos = 0;
			for (; pos < atoms.size(); pos++)
			{
				static char buf[800];
				sprintf(buf, "%5s%5s%5s%7s%7s%7s", 
						atoms[pos].c_str(),
						names[pos].c_str(),
						types[pos].c_str(),
						symbols[pos].c_str(),
						charges[pos].c_str(),
						fcharges[pos].c_str()
						);
				outfile << buf;
				outfile << std::endl;
			}

			outfile.close();
		}

		if (infile.getLine().hasSubstring("Total ENERGY (Kcal)"))
		{
			vector<String> fields;

			File outfile(dir + FileSystem::PATH_SEPARATOR + file_name + ".results", std::ios::out);
			for (Position pos = 0; pos < 16; pos ++)
			{
				Size nr_fields = infile.getLine().split(fields);
				if (nr_fields < 2 ||
						infile.getLine().hasPrefix(" Bond Torsion"))
				{
					infile.readLine();
					continue;
				}

				outfile << fields[nr_fields - 1] << "              #  ";
				for (Position i = 0; i < nr_fields - 1; i++)
				{
					outfile << fields[i] << " ";
				}
				outfile << std::endl;

				infile.readLine();
			}

			String rms = infile.getLine().after("=");
			rms = rms.before("Kcal");
			rms.trim();

			outfile << rms << "             # rms" << std::endl;
			outfile.close();
		}
	}

	return 0;
}
