// $Id: assignShiftProcessor.C,v 1.16 2000/09/26 13:43:10 amoll Exp $

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

		// ----------transforming the names from STAR-FILE-STANDARD to PDB------
		FragmentDB frag_db;
		StringHashMap<String>* map = 0;
		if (frag_db.getNamingStandards().has("Star-PDB"))
		{
			map = frag_db.getNamingStandards()["Star-PDB"];
		}
		else 
		{
			 Log.warn() << "AssignShiftProcessor::start: "
									<< "no appropriate map found for name conversion" << endl;
		}

		// ---------------------read translate table ------------------------
		Path path;
		ifstream tableFile(path.find("NMR/translate.dat").c_str(), ios::in);
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
		}
		while (name != "END");

		tableFile.close();

		// ----------------------read the shiftdata ------------------------
		for (Position atompos = 0; atompos < atom_data_.size() ; atompos++)
		{
			// normalize the atom name to reflect the PDB standard
			String residue_name = atom_data_[atompos]->residueLabel;
			String atom_name    = atom_data_[atompos]->atomName;
			bool normalized;
			if (map != 0)
			{
				normalized = frag_db.normalize_names.matchName(residue_name, atom_name, map);
			}

			const String entry(residue_name + ":" + atom_name);

			if (!normalized && !transformTable.has(entry))
			{
				Log.warn() << "AssignShiftProcessor::start: could not convert atom name " 
									 << residue_name << ":" << atom_name << endl;
			}

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
			for (Position wordpos = 0; wordpos < size ; wordpos++)
			{
				String fullName(atom_data_[atompos]->residueSeqCode);
				fullName += tokens[wordpos];
				shift_table_[fullName] = atom_data_[atompos]->shiftValue;
cout << fullName << " " << atom_data_[atompos]->shiftValue << endl;
			}
		}

		molecule_ = 0;
		return true;
	}

	Processor::Result AssignShiftProcessor::operator()(Composite& object)
	{
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

		if (patom_->hasProperty(ShiftModule::PROPERTY__SHIFT))
		{
			Log.error() << "AssignShiftProcessor: atom contains already shift data: " << fullName << endl;   
			patom_->clearProperty(ShiftModule::PROPERTY__SHIFT);
		}
		if (shift_table_.has(fullName))
		{
			patom_->setProperty(ShiftModule::PROPERTY__SHIFT, shift_table_[fullName]);
cout << "atom found " << fullName << endl;
		}
		else 
		{
			Log.info() << "AssignShiftProcessor: entry not found: " << fullName << endl;   
		}               

		return Processor::CONTINUE;
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/NMR/assignShiftProcessor.iC>
#	endif

}//namespace BALL
