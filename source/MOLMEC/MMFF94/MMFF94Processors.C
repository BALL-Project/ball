// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.C,v 1.1.8.2 2007/08/05 18:53:02 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Processors.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/SYSTEM/path.h>

#include <BALL/SYSTEM/timer.h>
//    #define BALL_MMFF94_TEST
#define BALL_DEBUG_TEST

using namespace std;

namespace BALL
{

MMFF94AtomTyper::MMFF94AtomTyper()
{
}

MMFF94AtomTyper::MMFF94AtomTyper(const MMFF94AtomTyper& t)
	: AtomTyper(t)
{
}

bool MMFF94AtomTyper::setupHydrogenTypes(Parameters& p, const String& section)
{
	ParameterSection p_sec;
	p_sec.extractSection(p, section);

	for(Position i = 0; i < p_sec.getNumberOfKeys(); ++i)
	{
		String key = p_sec.getKey(i);

		if (p_sec.getNumberOfVariables() < 1)
		{
			continue;
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : "
									<< section << " Not enough fields in one line "
									<< std::endl;
			return false;
		}

		partner_type_to_htype_[key] = p_sec.getValue(i, 0);
	}

	return true;
}

// read MMFFAROM.PAR to map normal to aromatic types
bool MMFF94AtomTyper::setupAromaticTypes(Parameters& p, const String& section)
{
	aromatic_types_5_map_.clear();
	cation_atoms_.clear();

	vector<String> fields;

	ParameterSection p_sec;
	p_sec.extractSection(p, section);

	for(Position i = 0; i < p_sec.getNumberOfKeys(); ++i)
	{
		try
		{
			String key = p_sec.getKey(i);

			key.split(fields);

			if (fields.size() + p_sec.getNumberOfVariables() < 7)
			{
				continue;
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : "
										<< section << " Not enough fields in one line "
										<< std::endl;
				return false;
			}

			AromaticType type;

			const String old_type = fields[1];
			// special threatment for some of the 5 ring heteroaromatic types later:
			if (old_type.hasSuffix("*")) continue;

			type.new_type = fields[2];
			const Size ring_size = p_sec.getValue(i, 1).toUnsignedInt();
			const String L5 = p_sec.getValue(i, 2).toUnsignedInt();
			type.cation = p_sec.getValue(i, 3).toBool();
			type.anion = p_sec.getValue(i, 4).toBool();

			if (ring_size == 5)
			{
				aromatic_types_5_map_[old_type + "|" + L5] = type;

				if (type.cation) cation_atoms_.insert(old_type);
			}
		}
		catch(...)
		{
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : "
										<< section << " Not enough fields in one line "
										<< std::endl;
				return false;
		}
	}

	return true;
}


bool MMFF94AtomTyper::assignAromaticType_5_(Atom& atom, Position L5, bool anion, bool cation)
{
	String old_type = atom.getTypeName();

	if (anion && old_type == "N5M") return true;

	String key = old_type + "|" + String(L5);

	// try full match
	HashMap<String, AromaticType>::ConstIterator it = aromatic_types_5_map_.find(key);
	if (it != aromatic_types_5_map_.end())
	{
		const AromaticType& type = it->second;
		bool found = true;
		if  (cation && !type.cation) 
		{
			found = false;
		}
		else if (anion && !type.anion)  
		{
			found = false;
		}

		if (found)
		{
			atom.setTypeName(type.new_type);
			atom.setType(id_to_type_[type.new_type]);
			return true;
		}
	}

	const Position element = atom.getElement().getAtomicNumber();
	
	String new_type;

	// carbon
	if (element == 6) 
	{
		if      (L5 == 2) new_type = "C5A";
		else if (L5 == 3) new_type = "C5B";
		else if (L5 == 4) new_type = "C5";
	}
	// nitrogen
	else if (element == 7)
	{
		if      (L5 == 1) new_type = "NPYL";
		if      (L5 == 2) new_type = "N5A";
		else if (L5 == 3) new_type = "N5B";
		else if (L5 == 4) new_type = "N5";

		// single bonded oxide as partner?
		bool oxide = false;

		// nr valence electrons
		Size val = 0;

		AtomBondIterator bit = atom.beginBond();
		for (; +bit; ++bit)
		{
			val += bit->getOrder();
			Atom& partner = *bit->getPartner(atom);
			if (partner.getElement().getSymbol() == "O" &&
					partner.countBonds() == 1)
			{
				oxide = true;
			}
		}

		// if this is a nitrogen with 4 valence electrons,
		// we have special types:
		if (val == 4)
		{
			if (oxide)
			{
				if (new_type.size() == 3)
				{
					new_type += "X"; // N5AX N5BX
				}
				else
				{
					new_type += "OX"; // N5OX
				}
			}
			else
			{
				new_type += "+"; // N5A+ N5B+
			}
		}
	}
	// oxygen
	else if (element == 8)
	{
		if (L5 == 1) new_type = "OFUR";
	}
	// sulfur
	else if (element == 16)
	{
		if (L5 == 1) new_type = "STHI";
	}
	else
	{
		Log.error() << "Problem: No valid MMFF94 Type was assigned for " << atom.getFullName() << " " << atom.getTypeName() << std::endl;
		return false;
	}

	atom.setTypeName(new_type);
	atom.setType(id_to_type_[new_type]);

	return true;
}

bool MMFF94AtomTyper::setupSymbolsToTypes(Parameters& p, const String& section)
{
	vector<String> fields;
	ParameterSection p_sec;
	p_sec.extractSection(p, section);

	for(Position i = 0; i < p_sec.getNumberOfKeys(); ++i)
	{
		String key = p_sec.getKey(i);

		if (p_sec.getNumberOfVariables() < 1)
		{
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : "
									<< section << " Not enough fields in one line "
									<< std::endl;
			return false;
		}

		try
		{
			id_to_type_[key] = p_sec.getValue(i, 0).toUnsignedInt();
		}
		catch(...)
		{
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : "
									<< section << std::endl;
			return false;
		}
	}

	return true;
}


void MMFF94AtomTyper::collectHeteroAtomTypes(const MMFF94AtomTypes& atom_types)
{
	hetero_atom_types_.clear();
	const vector<MMFF94AtomType>& types = atom_types.getAtomTypes();

	for (Position p = 0; p < types.size(); p++)
	{
		const MMFF94AtomType& type = types[p];
		if (type.aspec == 7 &&
				type.crd == 3   &&
				type.val == 3)
		{
			hetero_atom_types_.insert(p);
			continue;
		}
		if ((type.aspec == 8  || type.aspec == 16) &&
				type.crd == 2   &&
				type.val == 2)
		{
			hetero_atom_types_.insert(p);
		}
	}

	// remove type nr 10, because it causes trouble in
	// 5 ring hetero systems with multiple Nitrogen atoms
	// there it would get L1 type along with type nr 39
	hetero_atom_types_.erase(10);
}


void MMFF94AtomTyper::assignTo(System& s)
{
	// delete any previous marks about the aromaticity:
	AtomIterator ait = s.beginAtom();
	for (; +ait; ++ait)
	{
		ait->setProperty("IsAromatic", false);
	}

	// Mark the aromatic atoms:
	for (Position r = 0; r < aromatic_rings_.size(); r++)
	{
		const HashSet<Atom*>& ring = aromatic_rings_[r];

		HashSet<Atom*>::ConstIterator ait = ring.begin();
		for (; +ait; ++ait)
		{
			Atom& atom = **ait;
			atom.setProperty("IsAromatic", true); 
		}
	}

	// assign the atom types of all atoms which are not 
	// member of a heteroaromatic ring per SMARTS rules
	AtomTyper::assignTo(s);

	///////////////////////////////////////////////////
	// assign heterocyclic 5 ring members types:
	// e.g. C5A C5B N5A N5B C5
	//////////////////////////////////////////////////
	
	// iterate over all rings
	for (Position r = 0; r < aromatic_rings_.size(); r++)
	{
		const HashSet<Atom*>& ring_atoms = aromatic_rings_[r];
		HashSet<Atom*>::ConstIterator ait;

		// in aromatic 6 ring systems make CB out of CNN+
		if (ring_atoms.size() == 6)
		{
			ait = ring_atoms.begin();
			for (; +ait; ++ait)
			{
				Atom& atom = **ait;
				if (atom.getTypeName() == "CNN+")
				{
					atom.setTypeName("CB");
					atom.setType(37);

					// prevent NCN+ assignment for nitrogens connected to pyrimidin like ring structures!
					// reset the type of the atom to NC=N
					AtomBondIterator abit = atom.beginBond();
					for (;+abit;++abit)
					{
						Atom* partner = (*abit).getPartner(atom);
						if (partner->getType() == 55)
						{
							partner->setType(40);
							partner->setTypeName("NC=N");
						}
					}
				}
			}
		}

		// we are new only interested in rings of 5 atoms
		if (ring_atoms.size() != 5) continue;

		// collect some informations about the hetero atoms and charged atoms
		Atom* hetero_atom = 0;
		Atom* cation_atom = 0;
		Atom* anion_atom = 0;

		ait = ring_atoms.begin();
		for (; +ait; ++ait)
		{
			Atom& atom = **ait;
			String element = atom.getElement().getSymbol();
			if ((element != "C" && element != "N") ||
					hetero_atom_types_.has(atom.getType()))
			{
				//Other hetero atoms should get precedence over nitrogen
				//Otherwise structures with hetero 5-rings like GESCIQ randomly assign bogus types
				//Note: This is a hack, but seems to work. A proper fix would be to figure out
				//the correct order in which hetero atoms need to be examined.
				if(!hetero_atom || hetero_atom->getElement().getAtomicNumber() == 7) {
					hetero_atom = &atom;
				}
			}

			if (cation_atoms_.has(atom.getTypeName())) cation_atom = &atom;

			if (atom.getTypeName() == "NM" || atom.getTypeName() == "N5M") anion_atom = &atom;
		}

		bool charged = hetero_atom == 0 && (cation_atom || anion_atom);

		Atom* charged_atom = cation_atom;
		if (charged_atom == 0) charged_atom = anion_atom;

		Atom* atom_x = hetero_atom;
		if (atom_x == 0) atom_x = charged_atom;

		// we are only interested in hetero rings and charged rings
		// no hetero atom and no charged atom? Then continue to next ring
		if (atom_x == 0) continue;
		
		// iterate over all atoms of this ring:
		ait = ring_atoms.begin();
		for (; +ait; ++ait)
		{
			Atom& atom = **ait;

			Position L5 = 0;

			if (hetero_atom == &atom)
			{
				L5 = 1;
			}
			else if (charged)
			{
				L5 = 4;
			}
			// is this atom in alpha position (directly connected to a hetero atom)?
			else if (atom.isBoundTo(*atom_x))
			{
				L5 = 2;
			}
			// beta position
			else
			{
				L5 = 3;
			}

			assignAromaticType_5_(atom, L5, anion_atom != 0, cation_atom != 0);
//       Log.error() << atom.getTypeName()            << " "  << __FILE__ << "  " << __LINE__<< std::endl;
		} // all atoms in one ring
	} // all rings: done

	////////////////////////////////////////////////////////////////
	// assign Hydrogen atom types according to their binding partner
	////////////////////////////////////////////////////////////////
	ait = s.beginAtom();
	for (; +ait; ++ait)
	{
		if (ait->getElement().getSymbol() != "H" ||
				ait->countBonds() != 1)
		{
			continue;
		}

		Bond& bond = *ait->getBond(0);
		const Atom& partner = *bond.getPartner(*ait);

		const String key = partner.getTypeName();
		if (!partner_type_to_htype_.has(key)) continue;

		String H_type_name = partner_type_to_htype_[key];
		if (!id_to_type_.has(H_type_name)) continue;

		Position H_nr = id_to_type_[H_type_name];

		if (ait->getType() > (Index) H_nr) continue;

		ait->setTypeName(H_type_name);
		ait->setType(H_nr);
	}
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

MMFF94ChargeProcessor::MMFF94ChargeProcessor()
	:	UnaryProcessor<Atom>(),
		es_parameters_(0)
{
}

MMFF94ChargeProcessor::MMFF94ChargeProcessor(const MMFF94ChargeProcessor& p)
	:	UnaryProcessor<Atom>(p),
		es_parameters_(0)
{
}

void MMFF94ChargeProcessor::setup(const String& section)
{
	clear();

	Path path;
	String filename(path.find("MMFF94/mmff94.ini"));

	Parameters p(filename);

	ParameterSection p_sec;
	p_sec.extractSection(p, section);

	for(Position i = 0; i < p_sec.getNumberOfKeys(); ++i)
	{
		String key = p_sec.getKey(i);

		if (p_sec.getNumberOfVariables() < 1)
		{
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : "
									<< section << " Not enough fields in one line "
									<< std::endl;
			return;
		}

		if (p_sec.getValue(i, 0) == "*")
		{
			rule_types_.insert(key);
			continue;
		}

		types_to_charges_[key] = p_sec.getValue(i, 0).toFloat();
	}
}

void MMFF94ChargeProcessor::clear()
	
{
	types_to_charges_.clear();
	rule_types_.clear();
}

const MMFF94ChargeProcessor& MMFF94ChargeProcessor::operator = (const MMFF94ChargeProcessor&)
	
{
	return *this;
}


Processor::Result MMFF94ChargeProcessor::operator () (Atom& atom)
{
	atoms_.push_back(&atom);
	return Processor::CONTINUE;
}

void MMFF94ChargeProcessor::assignPartialCharges_()
{
	HashMap<String, float>::ConstIterator it;

	vector<vector<Atom*> > N5Ms;
	vector<vector<Atom*> > NIMs;

	N5Ms.resize(aromatic_rings_.size());
	NIMs.resize(aromatic_rings_.size());

	for (Position p = 0; p < atoms_.size(); p++)
	{
		Atom& atom = *atoms_[p];

		// make sure we overwrite all charges
		atom.setCharge(0);

		String type = atom.getTypeName();

		// rule_types_ has all types which need a more 
		// sophisticated rule for assigning the partial charge
		if (!rule_types_.has(type))
		{
			// ok, this is a triavial case:
			// if we dont find this type it has no charge
			// otherwise take it from the parameter file
			it = types_to_charges_.find(type);

			if (it != types_to_charges_.end())
			{
				atom.setCharge(it->second);
			}

			continue;
		}

		if (type == "N5M") 
		{
			for (Position r = 0; r < aromatic_rings_.size(); r++)
			{
				if (aromatic_rings_[r].has(&atom))
				{
					N5Ms[r].push_back(&atom);
					break;
				}
			}
			continue;
		}

		if (type == "NIM+") 
		{
			for (Position r = 0; r < aromatic_rings_.size(); r++)
			{
				if (aromatic_rings_[r].has(&atom))
				{
					NIMs[r].push_back(&atom);
					break;
				}
			}
			continue;
		}

		if (type == "O2S")
		{
			AtomBondIterator bit = atom.beginBond();

			for (; +bit; ++bit)
			{
				Atom* partner = bit->getPartner(atom);
				if (partner->getElement().getSymbol() == "S" &&
						partner->countBonds() == 3)
				{
					Size nr = 0;
					AtomBondIterator b = partner->beginBond();

					for (; +b; ++b)
					{
						nr += (*b).getOrder();
					}


					if (nr == 3) atom.setCharge(-0.5);

					break;
				}
			}

			continue;
		}

		if (type == "NR%")
		{
			AtomBondIterator bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				if (bit->getOrder() == Bond::ORDER__TRIPLE)
				{
					if (bit->getPartner(atom)->getElement().getSymbol() == "N")
					{
						atom.setCharge(1);
					}
				}
			}
		}

		if (type == "SM")
		{
			// the single bonded sulfur shares its charge if its partner also 
			// has an oxygen bonded, which has only one valence electron
			bool share = false;

			Atom& partner = *atom.getBond(0)->getPartner(atom);
			AtomBondIterator bit = partner.beginBond();
			for (; +bit; ++bit)
			{
				Atom& partner2 = *bit->getPartner(partner);
				if (partner2.getElement().getSymbol() == "O" &&
						partner2.countBonds() == 1 &&
						partner2.getBond(0)->getOrder() == Bond::ORDER__SINGLE)
				{
					share = true;
				}
			}

			if (!share) atom.setCharge(-1);
			else        atom.setCharge(-0.5);

			continue;
		}
	}


	// distribute the negative charge of N5M atoms over the aromatic ring
	for (Position r = 0; r < N5Ms.size(); r++)
	{
		if (N5Ms[r].size() == 0) continue;
	
		float charge = -1.0 / (float) N5Ms[r].size();

		for (Position a = 0; a < N5Ms[r].size(); a++)
		{
			N5Ms[r][a]->setCharge(charge);
		}
	}

	// distribute the positiive charge of NIM+ atoms over the aromatic ring and 
	// neighbouring NGD+ atoms
	for (Position r = 0; r < NIMs.size(); r++)
	{
		if (NIMs[r].size() == 0) continue;

		Size nr_partner = 0;

		// count nr of NGD+ atoms bound to this ring
		HashSet<Atom*>::Iterator ait = aromatic_rings_[r].begin();
		for (; +ait; ++ait)
		{
			AtomBondIterator bit = (*ait)->beginBond();
			for (; +bit; ++bit)
			{
				const String& partner_type = bit->getPartner(**ait)->getTypeName();
				if (partner_type == "NGD+" ||
						partner_type == "NCN+") 
				{
					nr_partner++;
				}
			}
		}
	
		float charge = 1.0 / (float) (NIMs[r].size() + nr_partner);

		// charge for the partner N atoms is already set, 
		// now care for the NIM+ atoms:
		for (Position a = 0; a < NIMs[r].size(); a++)
		{
			NIMs[r][a]->setCharge(charge);
		}
	} // NIM+ type atoms
}


bool MMFF94ChargeProcessor::finish()
{
	assignPartialCharges_();
	
	HashMap<Atom*, float> charges;

	// distribute initial charges to neighbouring atoms
	for (Position p = 0; p < atoms_.size(); p++)
	{
		Atom& atom = *atoms_[p];
//   	  Log.info() << "Intial Charge "  << atom.getName() << " " << atom.getTypeName() << "  " << atom.getCharge() << std::endl;

//   #ifdef BALL_MMFF94_TEST
		atom.setProperty("InitialCharge", atom.getCharge());
//   #endif

 		const Size nr_bonds = atom.countBonds();

		// only care for bonded atoms:
		// others cant distribute charge
		if (nr_bonds == 0) continue;

		float phi = es_parameters_->getPhi(atom.getType());

		if (phi == MMFF94_INVALID_VALUE) 
		{
			unassigned_atoms_.insert(&atom);
			continue;
		}

		if (phi == 0) continue;

		float charge = atom.getCharge();
		float c = charge * phi;
		atom.setCharge((1 - (float)nr_bonds * phi) * atom.getCharge());

//      Log.info() << "Intial Charge1 "  << atom.getName() << " " << atom.getTypeName() << "  " << atom.getCharge() << std::endl;

		// iterate over the bonds and distribute the charge c to each partner atom
		HashMap<Atom*, float>::Iterator hit;
		AtomBondIterator bit = atom.beginBond();
		for (; +bit; ++bit)
		{
			Atom* atom2 = (Atom*)bit->getPartner(atom);
			hit = charges.find(atom2);
			if (hit != charges.end())
			{
				hit->second += c;
			}
			else
			{
				charges[atom2] = c;
			}
		}
//      Log.info() << "Intial Charge2 "  << atom.getName() << " " << atom.getTypeName() << "  " << atom.getCharge() << std::endl;
	}

	// now add up all charges
	HashMap<Atom*, float>::Iterator cit = charges.begin();
	for (; cit != charges.end(); ++cit)
	{
		Atom& atom = *cit->first;
		atom.setCharge(atom.getCharge() + cit->second);
//      Log.info() << "Intial Charge3 "  << atom.getName() << " " << atom.getTypeName() << "  " << atom.getCharge() << std::endl;
	}


	////////////////////////////////////////////////////////
	// bond partial charges
	for (Position p = 0; p < atoms_.size(); p++)
	{
		Atom& atom = *atoms_[p];
	
		double charge = atom.getCharge();
		AtomBondIterator bit = atom.beginBond();
		Index at1 = atom.getType();
		for (; +bit; ++bit)
		{
			const Atom* const atom2 = bit->getPartner(atom);
			Index at2 = atom2->getType();
			Position bt = bit->hasProperty("MMFF94SBMB");
			double pcharge;
			
			if (at1 < at2) pcharge = - es_parameters_->getPartialCharge(at1, at2, bt);
			else           pcharge =   es_parameters_->getPartialCharge(at2, at1, bt);

			if (fabs(pcharge) == MMFF94_INVALID_VALUE) 
			{
				unassigned_atoms_.insert(&atom);
				unassigned_atoms_.insert((Atom*)atom2);
				continue;
			}

			charge += pcharge;
		}

		atom.setCharge(charge);
//      Log.info() << "Intial Charge4 "  << atom.getName() << " " << atom.getTypeName() << "  " << atom.getCharge() << std::endl;
	} // all atoms
	
	return Processor::CONTINUE;
}

bool MMFF94ChargeProcessor::start()
{
	atoms_.clear();
	unassigned_atoms_.clear();
	return es_parameters_ != 0;
}

} // namespace BALL
