// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: splitMMFFTestSuiteOptiFiles.C,v 1.1.8.1 2007/03/25 21:32:23 oliver Exp $
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
#include <BALL/DATATYPE/hashSet.h>

//   #define DEBUG
#ifdef DEBUG
#define LINE_PRINT Log.error() << infile.getLine() << "         " << __LINE__ << std::endl;
#else
#define LINE_PRINT
#endif

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

	Size nr_rings = 0;
	Size nr_aromatic_rings = 0;

	vector<String> fields;

	while (infile.readLine())
	{
		LINE_PRINT

		if (infile.getLine().hasSubstring("Structure Name:") ||
				infile.getLine().hasSubstring("New Structure Name"))
		{
			file_name = infile.getLine().after(": ");
			Log.error() <<  file_name << std::endl;
			nr_aromatic_rings = 0; 
			nr_rings = 0;
		}

		else if (infile.getLine().hasSuffix("SUBRINGS"))
		{
			HashSet<String> lines;
			
			while (!infile.getLine().hasSubstring("OPTIMOL-LIST"))
			{
				if (lines.has(infile.getLine()))
				{
					while (infile.getLine() != "" && 
							   !infile.getLine().hasSubstring("OPTIMOL")) 
					{
						infile.readLine();
					}
				}

				else if (infile.getLine().hasSuffix("SUBRINGS"))
				{
					infile.getLine().split(fields);
					nr_rings += fields[3].toUnsignedShort();
					lines.insert(infile.getLine());
				}
				else if (infile.getLine().hasSuffix("AROMATIC"))
				{
					nr_aromatic_rings++;
				}

				infile.readLine();
			}
		}

		else if (infile.getLine().hasSuffix("****"))
		{
			File rings_file(dir + FileSystem::PATH_SEPARATOR + file_name + ".rings", std::ios::out);
			rings_file << nr_rings << std::endl;
			rings_file << nr_aromatic_rings << std::endl;
			rings_file.close();
		}

		else if (infile.getLine().hasSubstring(" ATOM NAME  TYPE"))
		{
			vector<String> atoms, names, types, symbols, charges, fcharges;

			while (infile.readLine())
			{
				if (infile.getLine().hasSubstring("OPTIMOL")) break;
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

		else if (infile.getLine().hasSubstring("Total ENERGY (Kcal)"))
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

		/// stretch 
		else if (infile.getLine() == "   I          J            I    J   CLASS   LENGTH   LENGTH    DIFF.    ENERGY   CONSTANT")
		{
			File outfile(dir + FileSystem::PATH_SEPARATOR + file_name + ".stretch", std::ios::out);

			infile.readLine();
			while (infile.readLine() && infile.getLine() != "")
			{
				vector<String> fields;
				infile.getLine().split(fields);

				outfile << fields[0] << " " << fields[2] << " " 
								<< fields[6] << " " << fields[8] << " " 
								<< fields[11]  << std::endl;
			}
		}
		
		/// bend
		else if (infile.getLine() == "  I       J       K     I    J    K  CLASS    ANGLE       ANGLE      DIFF.      ENERGY   CONSTANT")
		{
			File outfile(dir + FileSystem::PATH_SEPARATOR + file_name + ".bend", std::ios::out);

			infile.readLine();
			while (infile.readLine() && infile.getLine() != "")
			{
				vector<String> fields;
				infile.getLine().split(fields);

				// I J K type angle delta energy ka
				outfile << fields[0] << " " << fields[1] << " " 
								<< fields[3] << " " << fields[7] << " " 
								<< fields[9] << " " << fields[10] << " " 
								<< fields[11] << " " << fields[12]  << std::endl;
			}
		}
		

		/// stretch bend
		else if (infile.getLine() == "  I       J       K     I    J    K  CLASS    ANGLE       ANGLE      R(I,J)    ENERGY       I-J")
		{
			File outfile(dir + FileSystem::PATH_SEPARATOR + file_name + ".stretchbend", std::ios::out);

			infile.readLine();
			while (infile.readLine() && infile.getLine() != "")
			{
				vector<String> fields;
				infile.getLine().split(fields);

				// I J K type energy f_ij
				outfile << fields[0] << " " << fields[1] << " " 
								<< fields[3] << " " << fields[7] << " " 
								<< fields[11] << " " << fields[12] << " " << std::endl;
			}
		}
		/// torsions
		else if (infile.getLine() == "  I       J          K       L        I   J   K   L   CLASS    ANGLE     ENERGY     V1      V2      V3")
		{
			File outfile(dir + FileSystem::PATH_SEPARATOR + file_name + ".torsions", std::ios::out);

			infile.readLine();
			while (infile.readLine() && infile.getLine() != "")
			{
				vector<String> fields;
				infile.getLine().split(fields);

				// I J K L type angle energy v1 v2 v3
				outfile << fields[0] << " " << fields[1] << " " 
								<< fields[3] << " " << fields[5] << " " 
								<< fields[10] << " " << fields[11] << " "
								<< fields[12] << " " << fields[13] << " "
								<< fields[14] << " " << fields[15]
								<< std::endl;
			}
		}
		/// planes
		else if (infile.getLine() == "  I -- J -- K ... L            I  J  K  L         ANGLE       ENERGY    CONSTANT")
		{
			File outfile(dir + FileSystem::PATH_SEPARATOR + file_name + ".planes", std::ios::out);

			infile.readLine();
			while (infile.readLine() && infile.getLine() != "")
			{
				vector<String> fields;
				infile.getLine().split(fields);

				// I J K L angle energy constant
				outfile << fields[0] << " " << fields[1] << " " 
								<< fields[2] << " " << fields[3] << " " 
								<< fields[9] << " " << fields[10] << " "
								<< fields[11]
								<< std::endl;
			}
		}
		/// non bonded
		else if (infile.getLine() == "     ATOM  PAIR           R       VDW      EREP    EATTR      EQ     R*    EPS")
		{
			File outfile(dir + FileSystem::PATH_SEPARATOR + file_name + ".nonbonded", std::ios::out);

			infile.readLine();
			while (infile.readLine() && infile.getLine() != "")
			{
				vector<String> fields;
				infile.getLine().split(fields);

				if (fields.size() < 11) break;

     		// ATOMPAIR           R       VDW      EREP    EATTR      EQ     R*    EPS
				outfile << fields[0] << " " << fields[2] << " " 
								<< fields[4] << " " << fields[5] << " " 
								<< fields[6] << " " << fields[7] << " "
								<< fields[8] << " " << fields[9] << " "
								<< fields[10] << std::endl;
			}
		}

	}

	return 0;
}
