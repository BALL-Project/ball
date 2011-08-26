// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/MOPACOutputFile.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>

namespace BALL
{
	MOPACOutputFile::MOPACOutputFile()
		: GenericMolFile()
	{
	}

	MOPACOutputFile::MOPACOutputFile(const String& name, File::OpenMode open_mode)
		: GenericMolFile()
	{
		open(name, open_mode);
	}

	MOPACOutputFile::~MOPACOutputFile()
	{
	}

	void MOPACOutputFile::clear()
	{
		atoms_.clear();
		type_counts_.clear();
	}

	bool MOPACOutputFile::read(System& system)
	{
		Molecule* mol = read();

		if (mol)
		{
			system.insert(*mol);

			if (system.getName() == "")
				system.setName(mol->getName());
		}
		else
		{
			return false;
		}

		return true;
	}

	Molecule* MOPACOutputFile::read()
	{
		clear();

		Molecule* result = new Molecule;

		// read until we find the cartesian coordinates
		bool found_coords = false;

		while (!found_coords && readLine())
		{
			getLine().toUpper();

			found_coords = getLine().hasSubstring("CARTESIAN COORDINATES");
		}

		if (!found_coords)
		{
			Log.error() << "Could not find cartesian coordinates in MOPAC output file" << std::endl;

			delete(result);

			return NULL;
		}

		// try to find the first coordinate line
		RegularExpression coord_reg_exp("\\s*[0-9]+\\s+[A-Z]+\\s+-?[0-9.]+\\s+-?[0-9.]+\\s+-?[0-9.]+");

		bool first_coord_found = false;

		while (!first_coord_found && readLine())
		{
			first_coord_found =coord_reg_exp.match(getLine());
		}

		if (!first_coord_found)
		{
			Log.error() << "Could not find cartesian coordinates in MOPAC output file" << std::endl;

			delete(result);

			return NULL;
		}

		addAtom_(getLine(), result);

		while (readLine() && coord_reg_exp.match(getLine()))
			addAtom_(getLine(), result);

		// TODO: handle bonds
		// ok, now try to find the charges

		bool found_charges = false;
		while (!found_charges && readLine())
		{
			found_charges = getLine().hasSubstring("NET ATOMIC CHARGES AND DIPOLE CONTRIBUTIONS");
		}

		if (found_charges)
		{
			bool first_charge_found = false;

			RegularExpression charge_reg_exp("\\s*[0-9]+\\s+[A-Z]+\\s+-?[0-9.]+\\s+[0-9.]+");

			while (!first_charge_found && readLine())
			{
				first_charge_found = charge_reg_exp.match(getLine());
			}

			if (first_charge_found)
			{
				std::vector<String> split;
				getLine().split(split);

				atoms_[split[0].toInt()-1]->setCharge(split[2].toFloat());

				while (readLine() && charge_reg_exp.match(getLine()))
				{
					getLine().split(split);
					atoms_[split[0].toInt()-1]->setCharge(split[2].toFloat());
				}
			}
		}

		return result;
	}

	void MOPACOutputFile::addAtom_(const String& line, Molecule* mol)
	{
		std::vector<String> split;
		line.split(split);

		if (type_counts_.find(split[1]) == type_counts_.end())
			type_counts_[split[1]] = 1;
		else
			type_counts_[split[1]]++;

		atoms_.push_back(new Atom(PTE[split[1]], split[1]+String(type_counts_[split[1]])));

		Vector3 position(split[2].toFloat(), split[3].toFloat(), split[4].toFloat());
		atoms_.back()->setPosition(position);

		mol->insert(*(atoms_.back()));
	}
}
