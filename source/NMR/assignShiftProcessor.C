// $Id: assignShiftProcessor.C,v 1.22 2000/12/17 09:39:40 amoll Exp $

#include <BALL/NMR/assignShiftProcessor.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

namespace BALL
{

	AssignShiftProcessor::AssignShiftProcessor(const AssignShiftProcessor& processor)
	  : UnaryProcessor<Composite>(),
 	   shift_table_(processor.shift_table_),
 	   atom_data_(processor.atom_data_),
 	   valid_(processor.valid_),
 	   molecule_(processor.molecule_),
 	   number_of_fragment_(processor.number_of_fragment_)
	{
	}

	AssignShiftProcessor::AssignShiftProcessor()
 	 : UnaryProcessor<Composite>(),
 	   atom_data_(RTTI::getDefault<std::vector<NMRAtomData*> >())
	{
	}

	bool AssignShiftProcessor::start()
	{
		if (!valid_)
		{
			Log.error() << "AssignShiftProcessor: shift data were not assigned" << endl;
			return false;
		}

		// ----------transforming the names from STAR-FILE-STANDARD to PDB------
		FragmentDB frag_db;
		if (!frag_db.getNamingStandards().has("Star-PDB"))
		{
			 Log.error() << "AssignShiftProcessor::start: "
									 << "no appropriate map found for name conversion" << endl;
			return false;
		}

		StringHashMap<String>* map = frag_db.getNamingStandards()["Star-PDB"];

		// ---------------------read translate table ------------------------
		Path path;
		ifstream tableFile(path.find("NMR/translate.dat").c_str(), ios::in);
		if (!tableFile)
		{
			Log.error() << "AssignShiftProcessor: translate.dat not found:" << endl;   
			return false;
		}

		StringHashMap<String> transform_table;
		String name;

		do
		{
			String entry;
			tableFile >> name;
			tableFile >> entry;
			transform_table[name] = entry;
		}
		while (name != "END");

		tableFile.close();

		// ----------------------read the shiftdata ------------------------
		for (Position atom_pos = 0; atom_pos < atom_data_.size() ; atom_pos++)
		{
			// normalize the atom name to reflect the PDB standard
			String residue_name = atom_data_[atom_pos]->residue_label;
			String atom_name    = atom_data_[atom_pos]->atom_name;
			bool normalized = false;
			if (map != 0)
			{
				normalized = frag_db.normalize_names.matchName(residue_name, atom_name, map);
			}

			const String entry(residue_name + ":" + atom_name);

			if (!normalized && !transform_table.has(entry))
			{
				Log.warn() << "AssignShiftProcessor::start: could not convert atom name " << entry << endl;
			}

			String prefix(atom_data_[atom_pos]->residue_seq_code);
			prefix += residue_name;
			prefix += ":";

			if (!transform_table.has(entry))
			{
				String full_name(prefix);
				full_name += atom_name;
				shift_table_[full_name] = atom_data_[atom_pos]->shift_value;
				//cout << full_name << " " << atom_data_[atom_pos]->shift_value << endl;
				continue;
			}

			// from now on getting the transformed names for atoms
			if (!transform_table[entry].has('/'))
			{
				String full_name(residue_name);
				full_name += transform_table[entry];
				shift_table_[full_name] = atom_data_[atom_pos]->shift_value;
				//cout << full_name << " " << atom_data_[atom_pos]->shift_value << endl;
				continue;
			}
			
			// adding multiple atoms for one entry
			std::vector<String> tokens;
			Size size = transform_table[entry].split(tokens, "/");
			for (Position wordpos = 0; wordpos < size ; wordpos++)
			{
				String full_name(atom_data_[atom_pos]->residue_seq_code);
				full_name += tokens[wordpos];
				shift_table_[full_name] = atom_data_[atom_pos]->shift_value;
				//cout << full_name << " " << atom_data_[atom_pos]->shift_value << endl;
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

		if (!RTTI::isKindOf<Atom>(object))
		{
			return Processor::CONTINUE;
		}

		// --------------------set the shift value--------------------------------------------
		Atom* patom = RTTI::castTo<Atom>(object);
		
		String full_name;
		if (patom->getResidue() != 0)
		{
			full_name = patom->getResidue()->getID();
		}
		else 
		{
			full_name = String(number_of_fragment_);
		}
		full_name += patom->getFragment()->getName();
		full_name += ":";
		full_name += patom->getName();

		if (patom->hasProperty(ShiftModule::PROPERTY__SHIFT))
		{
			Log.error() << "AssignShiftProcessor: atom contains already shift data: " << full_name << endl;   
			patom->clearProperty(ShiftModule::PROPERTY__SHIFT);
		}

		if (shift_table_.has(full_name))
		{
			patom->setProperty(ShiftModule::PROPERTY__SHIFT, shift_table_[full_name]);
			Log.info() << "atom found " << full_name << endl;
		}
		else 
		{
			Log.info() << "AssignShiftProcessor: entry not found: " << full_name << endl;   
		}               

		return Processor::CONTINUE;
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/NMR/assignShiftProcessor.iC>
#	endif

}//namespace BALL
