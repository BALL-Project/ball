// $Id: assignShiftProcessor.C,v 1.8 2000/09/18 14:39:05 oliver Exp $

#include<BALL/NMR/assignShiftProcessor.h>

#ifndef BALL_KERNEL_PDBATOM_H
#	include<BALL/KERNEL/PDBAtom.h>
#endif

#ifndef BALL_KERNEL_PTE_H
# include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
# include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_COMMON_PATH_H
# include <BALL/SYSTEM/path.h>
#endif

using namespace std;

namespace BALL
{

	bool AssignShiftProcessor::start()
	{
		// BAUSTELLE: this could be done in a more general manner
		FragmentDB frag_db;
		StringHashMap<String>* map = 0;
		if (frag_db.getNamingStandards().has("Amber"))
		{
			map = frag_db.getNamingStandards()["Amber"];
		}
		
		
		if (!valid_)
		{
			Log.error() << "AssignShiftProcessor: shift data were not assigned" << endl;
			return false;
		}

		// ---------------------read translate table ------------------------
		Path path;
		ifstream tableFile(path.find("NMR/translate.dat").c_str(),ios::in);

		if (!tableFile)
		{
			Log.error() << "AssignShiftProcessor: translate.dat not found:" << endl;   
			return false;
		}
		StringHashMap<String> transformTable;

		String name;
		do
		{
			String entry;
			tableFile >> name;
			tableFile >> entry;
			transformTable[name] = entry;
cout << name << " " << entry << endl;
		}
		while (name != "END");
		tableFile.close();
cout << endl << endl;

		// ----------------------read the shiftdata ------------------------
		for (Position atompos = 0; atompos < atom_data_.size() ; atompos++)
		{
			String prefix(atom_data_[atompos]->residueSeqCode);
			prefix += atom_data_[atompos]->residueLabel;
			prefix += ":";

			// non H-atoms need no name transforming
			if (atom_data_[atompos]->atomType != 'H')
			{
				String atomName(prefix);
				atomName += atom_data_[atompos]->atomName;
				shift_table_[atomName] = atom_data_[atompos]->shiftValue;
cout << atomName << " " << atom_data_[atompos]->shiftValue << endl;
				continue;
			}


			// normalize the atom name to reflect the PDB standard
			String residue_name = atom_data_[atompos]->residueLabel;
			String atom_name = atom_data_[atompos]->atomName;
			if (map != 0)
			{
				frag_db.normalize_names.matchName(residue_name, atom_name, map);
			}
			String entry(residue_name + ":" + atom_name);
			
			// from now on getting the transformed name for H-atoms
			if (!transformTable.has(entry))
			{
				Log.error() << "AssignShiftProcessor: unknown atom from shift data "
					<< entry << endl;
				continue;
			}

			if (!transformTable[entry].has('/'))
			{
				String atomName(atom_data_[atompos]->residueSeqCode);
				atomName += transformTable[entry];
				shift_table_[atomName] = atom_data_[atompos]->shiftValue;
cout << atomName << " " << atom_data_[atompos]->shiftValue << endl;
				continue;
			}

			String arr[5];
			Size size = transformTable[entry].split(arr, 5, "/");
			for (Position wordpos = 0; wordpos < size ; wordpos++ )
			{
				String atomName(prefix);
				atomName += arr[wordpos];	
				shift_table_[atomName] = atom_data_[atompos]->shiftValue;
cout << atomName << " " << atom_data_[atompos]->shiftValue << endl;
			}
		}

		return true;
	}

	Processor::Result AssignShiftProcessor::operator()(Composite&  object)
	{
		//-----------------switching by the type of given object--------------------------
		if (RTTI::isKindOf<Molecule>(object))
		{
			if (molecule_ == 0)
			{
				molecule_ = RTTI::castTo<Molecule>(object);
				number_of_fragment_ = 0;
			}
			return Processor::CONTINUE;

			if (RTTI::castTo<Molecule>(object) != molecule_)
			{
					Log.error() << "AssignShiftProcessor: atom not from same molecule as before" << endl;
					return Processor::BREAK;
			}
		}

		if (RTTI::isKindOf<Fragment>(object))
		{
			number_of_fragment_++;
			return Processor::CONTINUE;
		}

		if (!RTTI::isKindOf<PDBAtom>(object))
		{
			return Processor::CONTINUE;
		}

		// --------------------set the shift value--------------------------------------------
		PDBAtom* patom_;
		patom_= RTTI::castTo<PDBAtom>(object);
		
		String atomName(number_of_fragment_);
		atomName += patom_->getFragment()->getName();
		atomName += ":";
		atomName += patom_->getName();

		patom_->clearProperty("chemical_shift");
		if (shift_table_.has(atomName))
		{
			patom_->setProperty("chemical_shift", shift_table_[atomName]);
cout << "atom found " << atomName << endl;
		}
else 
{
	Log.error() << "AssignShiftProcessor: entry not found: " << atomName << endl;   
}               

		return Processor::CONTINUE;
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/NMR/assignShiftProcessor.iC>
#	endif

}//namespace BALL
