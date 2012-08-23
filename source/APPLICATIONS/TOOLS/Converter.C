/* Converter.C
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
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/FORMAT/commandlineParser.h>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("Converter", "interconvert molecular file-formats", VERSION, String(__DATE__), "Convert, combine and store");
	parpars.registerParameter("i", "input file", INFILE, true);
	parpars.registerParameter("o", "output file", OUTFILE, true);
	parpars.registerParameter("f", "output format", STRING);
	parpars.registerFlag("rm", "remove input file when finished");
	String man = String("This tool can be used to convert between different molecular file-formats.\nSupported formats are ") + MolFileFactory::getSupportedFormats() + String(".");
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("o",MolFileFactory::getSupportedFormats());

	String formats = MolFileFactory::getSupportedFormats();
	vector<String> v;
	formats.split(v,",");
	list<String> format_list;
	for (Size i=0; i<v.size(); i++)
	{
		format_list.push_back(v[i]);
	}
	parpars.setParameterRestrictions("f",format_list);

	parpars.parse(argc, argv);

	String default_format = "mol2";
	if (parpars.has("f")) default_format = parpars.get("f");
	GenericMolFile* input = MolFileFactory::open(parpars.get("i"), ios::in);
	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, default_format);
	DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
	if (drf_output)
	{
		drf_output->setToolInfo(parpars.getStartCommand(), parpars.getStartTime());
	}

	Molecule* mol;
	int no_written = 0;
	int no_ignored = 0;
	while ((mol = input->read()))
	{
		bool b = output->write(*mol);
		if (b) no_written++;
		else no_ignored++;
		delete mol;
		if (no_written%50 == 0)
		{
			Log.level(5) << "\r" << no_written << "molecules";
			Log.flush();
		}
	}

	Log.level(20) << "\r";
	if (no_ignored > 0) Log.level(20) << "ignored " << no_ignored << " identical molecules!" << endl;
	Log.level(20) << "wrote " << no_written << " molecules." << endl;

	input->close();
	output->close();

	delete input;
	delete output;

	if (parpars.has("rm"))
	{
		File::remove(parpars.get("i"));
	}
}
