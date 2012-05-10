/* PropertyModifier.C
*
* Copyright (C) 2011 Marcel Schumann
*
* This program free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include "version.h"

using namespace BALL;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("PropertyModifier", "modify molecule property tags", VERSION, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input file", INFILE, true);
	parpars.registerParameter("o", "output file", OUTFILE, true);
	parpars.registerParameter("mode", "'delete', 'add' or 'rename' properties", STRING, true);
	parpars.registerParameter("name", "property name ('all' will erase all properties if in delete-mode)", STRING, true);
	parpars.registerParameter("value", "property value (in case of prop. adding only)", STRING);
	parpars.registerParameter("new_name", "new property name (in case of renaming only)", STRING);
	parpars.registerFlag("rm", "remove input file when finished");
	String man = "With this tools you can add, rename or delete molecule property tags.\nThese tags can for example contain information about scores, binding-free-energy, IDs or names for the resp. molecule.\nThe output of this tool is a molecule file in which the desired property tags have been added/renamed/deleted (as chosen).";
	parpars.setToolManual(man);
	list<String> slist;
	slist.push_back("add");
	slist.push_back("rename");
	slist.push_back("delete");
	parpars.setParameterRestrictions("mode",slist);
	parpars.setSupportedFormats("i","mol2,sdf,drf");
	parpars.setSupportedFormats("o","mol2,sdf,drf");
	parpars.setOutputFormatSource("o","i");
	parpars.parse(argc, argv);

	const String& nf = CommandlineParser::NOT_FOUND;
	String mode = parpars.get("mode");
	bool delete_mode = (mode == "delete");
	bool add_mode = (mode == "add");
	bool rename_mode = (mode == "rename");

	int n = delete_mode+add_mode+rename_mode;
	if (n != 1)
	{
		cerr<<"[Error:] Please set parameter 'mode' to either 'add' or 'delete' or 'rename'!"<<endl;
		return 1;
	}

	String prop_name = parpars.get("name");
	String new_prop_name = parpars.get("new_name");
	if (new_prop_name == nf && rename_mode)
	{
		Log.error()<<"[Error:] 'new_prop_name' needs to be specified in rename-mode!"<<endl;
		return 1;
	}
	String value = parpars.get("value");

	GenericMolFile* input = MolFileFactory::open(parpars.get("i"), ios::in);
	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, input);

	list<String> l = parpars.getList("name");
	list<String> v;
	if (add_mode) v = parpars.getList("value");
	else if (rename_mode) v = parpars.getList("new_name");
	map<String, String> names;
	list<String>::iterator it2 = v.begin();
	for (list < String > ::iterator it = l.begin(); it != l.end(); it++)
	{
		if (it2 != v.end())
		{
			names.insert(make_pair(*it, *it2));
			it2++;
		}
		else names.insert(make_pair(*it, ""));
	}

	for (Molecule* mol = input->read(); mol; delete mol, mol = input->read())
	{
		if (delete_mode)
		{
			if (prop_name != "all")
			{
				for (map < String, String > ::iterator it = names.begin(); it != names.end(); it++)
				{
					mol->clearProperty(it->first);
				}
			}
			else mol->PropertyManager::clear();
		}
		else if (rename_mode)
		{
			for (NamedPropertyIterator it = mol->beginNamedProperty(); it != mol->endNamedProperty(); it++)
			{
				map<String, String>::iterator map_it = names.find(it->getName());
				if (map_it != names.end())
				{
					*it = NamedProperty(map_it->second, it->toString());
				}
			}
		}
		else if (add_mode)
		{
			for (map < String, String > ::iterator it = names.begin(); it != names.end(); it++)
			{
				mol->setProperty(it->first, it->second);
			}
		}

		*output << *mol;
	}

	input->close();
	output->close();
	delete input;
	delete output;
	if (parpars.has("rm"))
	{
		File::remove(parpars.get("i"));
	}
}
