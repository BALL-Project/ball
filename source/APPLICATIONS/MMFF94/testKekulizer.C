// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: testKekulizer.C,v 1.1.4.1 2007/03/25 21:32:23 oliver Exp $
//
// test program for the MMFF94 implementation

#include <BALL/common.h>

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94OutOfPlaneBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Processors.h>
#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <cmath>

using namespace std;
using namespace BALL;

String dir;

Size wrong_types = 0;
File* out = 0;

System* readTestFile(String filename)
{
	MOL2File mol2_file(filename);
	System* system = new System;
	mol2_file >> *system;

	filename = filename.getSubstring(0, filename.size() - 4);
	filename += "atoms";

	vector<String> atoms, names, symbols, fields;
	vector<short> types;

	HashMap<String, Position> name_to_pos;
	Position pos = 0;
	
	LineBasedFile infile(filename);
	while (infile.readLine())
	{
		if (infile.getLine().split(fields) != 6)
		{
			Log.error() << "Error in " << filename << " Not 6 fields in one line " << infile.getLine() << std::endl;
			return 0;
		}

		atoms.push_back(fields[0]);
		types.push_back(fields[2].toUnsignedShort());
		symbols.push_back(fields[3]);
		name_to_pos[fields[0]] = pos;
		pos ++;
	}

	infile.close();

	AtomIterator ait;
	BALL_FOREACH_ATOM(*system, ait)
	{
		if (!name_to_pos.has(ait->getName()))
		{
			Log.error() << "We have no data for the atom " << ait->getName() << " in file " << filename << std::endl;
			continue;
		}

		Position pos = name_to_pos[ait->getName()];
//   ait->setType(types[pos]); // <---------------------------------
		ait->setProperty("Type", types[pos]);
		ait->setProperty("TypeName", symbols[pos]);
	}

	return system;
}


vector<double> getResults(String filename)
{
	filename += ".results";

	vector<double> results;
	LineBasedFile infile(filename);
	while (infile.readLine())
	{
		vector<String> fields;
		infile.getLine().split(fields);
		results.push_back(fields[0].toFloat() * Constants::JOULE_PER_CAL);
	}

	return results;
}


bool testType(System& system, String filename)
{
	bool ok = true;
	AtomIterator ait = system.beginAtom();
	for (; +ait; ++ait)
	{
		Atom& a = *ait;
		String org_symbol = a.getProperty("TypeName").getString();
		String our_symbol = a.getTypeName();

		Index org_type = a.getProperty("Type").getInt();
 		if (org_type == a.getType()) continue;

		String outs(org_symbol + " <-> " + our_symbol);

		wrong_types++;

		ok = false;
		Log.error() << "Type! " << filename << " " << a.getName() << "  was " << String(a.getProperty("Type").getInt()) 
								<< " " << outs << " " << String(a.getType()) << " "  << std::endl;

		(*out) << "Type! " << filename << " " << a.getName() << "  was " << String(a.getProperty("Type").getInt()) 
								<< " " << outs << " " << String(a.getType()) << " "  << std::endl;
	}

	return ok;
}



int runtests(const vector<String>& filenames)
{
	MMFF94 mmff, mmff2;

	vector<String> not_ok;
	Size ok = 0;
	for (Position pos = 0; pos < filenames.size(); pos++)
	{
//      		Log.info() << "> " << filenames[pos] << std::endl;
		bool result = true;
		String full_file_name(dir +FileSystem::PATH_SEPARATOR + filenames[pos] + ".mol2");
		System* system = readTestFile(full_file_name);
		if (system == 0)
		{
			Log.error() << "Could not read mol2 file " << full_file_name << std::endl;
			return -1;
		}

		Size db = 0;
		AtomBondIterator abit;
		AtomIterator ai;
		BALL_FOREACH_BOND(*system, ai, abit)
		{
			if (abit->getOrder() == Bond::ORDER__DOUBLE)
			{
				db ++;
			}
		}

		if (!mmff2.setup(*system))
		{
			Log.error() << "Setup failed for " << full_file_name << std::endl;
			return -1;
		}

	
		Size nrab = 0;
		vector<HashSet<Atom*> > arings = mmff2.getAromaticRings();
		for (Position p = 0; p < arings.size(); p++)
		{
			HashSet<Atom*>::Iterator hit = arings[p].begin();
			for (; +hit; ++hit)
			{
				abit = (**hit).beginBond();
				for (; +abit; ++abit)
				{
					Atom* a = *hit;
					Atom* b = abit->getPartner(*a);
					if (b < a && arings[p].has(b))
					{
						abit->setOrder(Bond::ORDER__AROMATIC);
						nrab++;
					}
				}
			}
		}
		
		/*
   		HINFile outh("asd.hin", std::ios::out);
   		outh << *system;
  		outh.close();
			*/


//   		Log.error() << "Abonds: " << nrab << std::endl;
		mmff.setup(*system);

		double dbn = 0;
		BALL_FOREACH_BOND(*system, ai, abit)
		{
			if (abit->getOrder() == Bond::ORDER__DOUBLE)
			{
				dbn ++;
			}
		}

		if (dbn  != db)
		{
			Log.error() << "Kekulizer: " << dbn << " != " << db << " double bonds!" << std::endl;
			(*out) << "Kekulizer: " << dbn << " != " << db << " double bonds!" << std::endl;
		}
		
		

 		result &= testType(*system, filenames[pos]);

 		if (result) ok++;
		else
		{
			HINFile hin(filenames[pos] + ".hin", std::ios::out);
			hin << *system;
			hin.close();
		}
		delete system;
	}

	Log.info() << "Tested " << filenames.size() << " files, " << ok << " files ok" << std::endl;

	return 0;
}

vector<String> getTestFiles()
{
	vector<String> results;
	LineBasedFile infile(dir + FileSystem::PATH_SEPARATOR + "filenames.txt");
	while (infile.readLine())
	{
		results.push_back(infile.getLine());
	}

	results.pop_back();

	return results;
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		Log.error() << "Usage: readMMFF94TestFile <dir with extracted test files> <all|system name>" << std::endl;
		return 1;
	}

	out = new File("results.txt", std::ios::out);
	dir = argv[1];

	vector<String> files;
	if (String(argv[2]) == "all")
	{
   files = getTestFiles();
	}
	else
	{
   files.push_back(argv[2]);
	}

	int result = runtests(files);
	StringHashMap<float>::Iterator hit = AtomTyper::rule_times.begin();
	for (; hit != AtomTyper::rule_times.end(); ++hit)
	{
		Log.error() << hit->first << " " << hit->second << std::endl;
	}

	out->close();
	delete out;

	return result;
}
