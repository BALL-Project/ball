// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/MATHS/common.h>
#include <BALL/FORMAT/commandlineParser.h>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("BindingDBCleaner", "fix bindingdb.org downloads", VERSION, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "input file");
	parpars.registerMandatoryStringParameter("type", "type of contained activity values: 'Ki' or 'IC50'");
	parpars.registerMandatoryOutputFile("o", "output file");
	parpars.registerMandatoryStringParameter("target", "binding-DB target name");
	String manual = "This tool cleans up the sd-properties contained in sd-files downloaded from bindingdb.org.\n\nFor all compounds in the input file, the affinity value for the specified target is searched and retained but all other properties are removed. Furthermore, the IC50 or Ki value of each compound is converted to a binding-free-energy value in units of [kJ/mol] that is added as a property-tag named 'binding_free_energy'.\n\nAll compounds in the input file for which no IC50 resp. Ki value for the specified target can found, are ignored and not written to the output file.";
	parpars.setToolManual(manual);
	list<String> slist;
	slist.push_back("IC50");
	slist.push_back("Ki");
	parpars.setParameterRestrictions("type", slist);
	parpars.setSupportedFormats("i","mol2,sdf,drf");
	parpars.setSupportedFormats("o","mol2,sdf,drf");
	parpars.parse(argc, argv);

	GenericMolFile* input = MolFileFactory::open(parpars.get("i"));
	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, "mol2.gz");
	String target_name = parpars.get("target");
	target_name.trim();

	String type = parpars.get("type");
	bool use_IC50 = (type == "IC50");
	bool use_Ki = (type == "Ki");
	String response_property = "Enzymologic: IC50 nM";
	if (use_Ki) response_property = "Enzymologic: Ki nM";

	if (!use_IC50 && !use_Ki)
	{
		Log.error() << "[Error:] Please set parameter 'type' to either 'IC50' or 'Ki'!" << endl;
		return 1;
	}

	Size no_mols_found = 0;
	Size no_acitivities_found = 0;
	Size total = 1;
	Size no_invalid = 0;
	for (Molecule* mol = input->read(); mol; mol = input->read(), total++)
	{
        bool found = false;
        int prop_id = 0;
		int target_no = 1;

		for (NamedPropertyIterator it = mol->beginNamedProperty();
			it!=mol->endNamedProperty(); it++)
		{
			String prop = "TARGET Biomolecule "+String(target_no);
			if (it->getName() == prop)
			{
				String resp = response_property+" "+String(target_no);

				if (it->toString().trim() == target_name
					&& mol->hasProperty(resp))
				{
					String resp_value = String(mol->getProperty(resp).toString()).trim();

					if (resp_value == "n/a")
					{
						target_no++;
						continue;
					}

					for (NamedPropertyIterator it2 = mol->beginNamedProperty(); it2 != mol->endNamedProperty(); it2++)
					{
						it2->clear();
					}

					if (resp_value.hasPrefix(">"))
					{
						resp_value = resp_value.after(">");
					}
					if (resp_value.hasPrefix("<"))
					{
						resp_value = resp_value.after("<");
					}

					mol->setProperty("Target", target_name);
					mol->setProperty(response_property, resp_value);
					double value = resp_value.toFloat();
					if (use_IC50)
					{
						String name = "Enzymologic: pIC50 nM";
                        if (prop_id > 0)
                        {
                            name += " "+String(prop_id);
                        }
						mol->setProperty(name, -log10(value));
					}
					else if (use_Ki)
					{
						String name = "Enzymologic: pKi nM";
                        if (prop_id > 0)
                        {
                            name += " " + String(prop_id);
                        }
						mol->setProperty(name, -log10(value));
					}

					String name = "binding_free_energy";
                    if (prop_id > 0)
                    {
                        name += " " + String(prop_id);
                    }
					float free_energy = 1.987*298.15*log(1e-09*value)*4.184/1000;
					mol->setProperty(name, free_energy);

					// remove compounds with completely senseless binding-affinity data
					if (free_energy > -125 && free_energy < 0 && !BALL::Maths::isNan(free_energy) && BALL::Maths::isFinite(free_energy))
					{
                        found = true;
                        prop_id++;
						no_acitivities_found++;
					}
					break;
				}
				target_no++;
			}
		}

		if (found)
		{
			*output << *mol;
			no_mols_found++;
		}
		delete mol;
	}

	if (no_invalid > 0)
	{
		Log << "[Warning:] Ignored "<<no_invalid<<" compounds due to invalid free-energy (<-125 or >0) !" <<endl;
	}

	Log.level(20) << "\rFound " << no_acitivities_found << " activities measurements for " << no_mols_found << " molecules and saved them to " << parpars.get("o") << endl;

	delete input;
	delete output;
}
