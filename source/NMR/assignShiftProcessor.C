// $Id: assignShiftProcessor.C,v 1.12 2000/09/25 13:30:55 oliver Exp $

#include<BALL/NMR/assignShiftProcessor.h>
#include<BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

namespace BALL
{

	bool AssignShiftProcessor::start()
	{
		if (!valid_)
		{
			Log.error() << "AssignShiftProcessor: shift data were not assigned" << endl;
			return false;
		}

		// ---------------------read translate table ------------------------
		Path path;
		ifstream tableFile(path.find("NMR/translate.dat").c_str(), ios::in);
		if (!tableFile)
		{
			Log.error() << "AssignShiftProcessor: translate.dat not found:" << endl;   
			return false;
		}

		// BAUSTELLE: this could be done in a more general manner
		FragmentDB frag_db;
		StringHashMap<String>* map = 0;
		if (frag_db.getNamingStandards().has("Amber-PDB"))
		{
			map = frag_db.getNamingStandards()["Amber-PDB"];
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
			// normalize the atom name to reflect the PDB standard
			String residue_name = atom_data_[atompos]->residueLabel;
			String atom_name    = atom_data_[atompos]->atomName;
			if (map != 0)
			{
				if (!frag_db.normalize_names.matchName(residue_name, atom_name, map))
				{
					Log.warn() << "AssignShiftProcessor::start: could not convert atom name " 
										 << residue_name << ":" << atom_name << endl;
				}
			}
			else 
			{
				Log.warn() << "AssignShiftProcessor::start:  no appropriate map found for name conversion" << endl;
			}
			const String entry(residue_name + ":" + atom_name);

			String prefix(atom_data_[atompos]->residueSeqCode);
			prefix += residue_name;
			prefix += ":";

			// non H-atoms need no name transforming
			if (atom_data_[atompos]->atomType != 'H' || !transformTable.has(entry))
			{
				String fullName(prefix);
				fullName += atom_name;
				shift_table_[fullName] = atom_data_[atompos]->shiftValue;
cout << fullName << " " << atom_data_[atompos]->shiftValue << endl;
				continue;
			}

			// from now on getting the transformed name for H-atoms
			if (!transformTable[entry].has('/'))
			{
				String fullName(residue_name);
				fullName += transformTable[entry];
				shift_table_[fullName] = atom_data_[atompos]->shiftValue;
cout << fullName << " " << atom_data_[atompos]->shiftValue << endl;
				continue;
			}

			std::vector<String> tokens;
			Size size = transformTable[entry].split(tokens, "/");
			for (Position wordpos = 0; wordpos < size ; wordpos++ )
			{
				shift_table_[tokens[wordpos]] = atom_data_[atompos]->shiftValue;
cout << tokens[wordpos] << " " << atom_data_[atompos]->shiftValue << endl;
			}
		}

		molecule_ = 0;
		return true;
	}

	Processor::Result AssignShiftProcessor::operator()(Composite& object)
	{
cout << "--" << endl;
		//-----------------switching by the type of given object--------------------------
		if (RTTI::isKindOf<Molecule>(object))
		{
			if (molecule_ == 0)
			{
				molecule_ = RTTI::castTo<Molecule>(object);
				number_of_fragment_ = 0;
cout << "molekuel gesetzt" << endl;
				return Processor::CONTINUE;
			}
			else
			{
				Log.error() << "AssignShiftProcessor: a second molecule was given" << endl;
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
cout << "kein PDBAtom" << endl;
			return Processor::CONTINUE;
		}

		// --------------------set the shift value--------------------------------------------
		PDBAtom* patom_;
		patom_= RTTI::castTo<PDBAtom>(object);
		
		String fullName(number_of_fragment_);
		fullName += patom_->getFragment()->getName();
		fullName += ":";
		fullName += patom_->getName();

		patom_->clearProperty(ShiftModule::PROPERTY__SHIFT);
		if (shift_table_.has(fullName))
		{
			patom_->setProperty(ShiftModule::PROPERTY__SHIFT, shift_table_[fullName]);
cout << "atom found " << fullName << endl;
		}
else 
{
	Log.error() << "AssignShiftProcessor: entry not found: " << fullName << endl;   
}               

		return Processor::CONTINUE;
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/NMR/assignShiftProcessor.iC>
#	endif

}//namespace BALL
