// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.C,v 1.1.4.17 2006/10/03 19:44:34 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Processors.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/KERNEL/forEach.h>

#include <BALL/SYSTEM/timer.h>
//    #define BALL_MMFF94_TEST
#define BALL_DEBUG_TEST

using namespace std;

namespace BALL 
{


StringHashMap<float> AtomTyper::rule_times = StringHashMap<float>();

AtomTyper::AtomTyper()
	: number_expected_fields_(4)
{
}

AtomTyper::AtomTyper(const AtomTyper& t)
{
	names_ = t.names_;
	rules_ = t.rules_;
	types_ = t.types_;
}
		
bool AtomTyper::setup(const String& filename)
{
	names_.clear();
	rules_.clear();
	types_.clear();
	element_to_rules_.clear();

	LineBasedFile infile(filename);

	vector<vector<String> > lines;
	vector<String> fields;
	
	while (infile.readLine())
	{
		const String line = infile.getLine();

		// comments and empty lines
		if (line.size() < 2 || line[0] == '*') continue;
		
		if (line.split(fields, "|") < number_expected_fields_)
		{
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
									<< filename << " Not enough fields in one line " 
									<< line << std::endl;
			
			continue;
		}

		lines.push_back(vector<String>());

		const Position pos = lines.size() - 1;
		vector<String>& vs = lines[pos];
		vs.resize(number_expected_fields_);

		for (Position p = 0; p < number_expected_fields_; p++)
		{
			fields[p].trim();
			vs[p] = fields[p];
		}

		if (fields[3] == "") continue;

		names_.push_back(fields[1]);
		types_.push_back(fields[2].toInt());
		rules_.push_back(fields[3]);

		// the element for this rule:
		String element = fields[0];
		if (element == "") element = "X";
		if (!element_to_rules_.has(element)) 
		{
			element_to_rules_[element] = vector<Position>();
		}
		element_to_rules_[element].push_back(rules_.size() - 1);
	}

	if (!specificSetup_()) 
	{
		Log.error() << "Error while parsing line in File " << filename << std::endl;
		return false;
	}

	return true;
}

void AtomTyper::assignTo(System& s)
{
	MoleculeIterator mit = s.beginMolecule();
	for (;+mit; ++mit)
	{
		assignTo(*mit);
	}
}

void AtomTyper::assignTo(Molecule& mol)
{
	atoms_.clear();

	AtomIterator ait = mol.beginAtom();
	for(; +ait; ++ait)
	{
		ait->setType(-1);
		ait->setTypeName(BALL_ATOM_DEFAULT_TYPE_NAME);
		atoms_.insert(&*ait);
	}

	SmartsMatcher sm;

	/////////////////////////////////////////////////////////////
	// Give the SmartsMatcher the smallest set of smallest rings:
	vector<vector<Atom*> > rings_vector;
	for (Position p = 0; p < rings_.size(); p++)
	{
		rings_vector.push_back(vector<Atom*>());
		HashSet<Atom*>::Iterator it = rings_[p].begin();
		for (; +it; ++it)
		{
			rings_vector[p].push_back(*it);
		}
	}
 	sm.setSSSR(rings_vector);

	/////////////////////////////////////////////////////////////
	// Next we iterate over all rule sets:
	HashSet<const Atom*> aromatic_atoms_;
	HashSet<Atom*>::Iterator atoms_it;
	HashMap<String, vector<Position> >::Iterator eit = element_to_rules_.begin();
	for (; +eit; ++eit)
	{
		if (atoms_.size() == 0) break;

		// the element of this rule set:
		String element = eit->first;
		// hetero elements, like ions can share one rule set:
		bool any_atom = element == "X";
		
		// collect all atoms, which have the same element, as the current rule set:
		aromatic_atoms_.clear();
		atoms_it = atoms_.begin();
		for (; +atoms_it; ++atoms_it)
		{
			const String& this_element = (**atoms_it).getElement().getSymbol();
			if (!any_atom) 
			{
				// match only the atoms with the correct element for this rule
				if (this_element == element)
				{
					aromatic_atoms_.insert(*atoms_it);
				}
			}
			else
			{
				// match all atoms with elements, which share a rule set with other elements
				if (!element_to_rules_.has(this_element))
				{
					aromatic_atoms_.insert(*atoms_it);
				}
			}
		}

		// if no atoms to be matched: continue to next rule set
		if (aromatic_atoms_.size() == 0) continue;

		const vector<Position>& rule_numbers = eit->second;
		for (Index nr = (Index)rule_numbers.size() - 1; nr >= 0; nr--)
		{
			/// Match the given rule from the vector to a given set of atoms
			Position rule = rule_numbers[nr];

			try
			{
				vector<HashSet<const Atom*> > result;
//      Log.error() << names_[rule]         << " "  << __FILE__ << "  " << __LINE__<< std::endl;
   #ifdef BALL_MMFF94_TEST
				if (!rule_times.has(names_[rule])) rule_times[names_[rule]] = 0;
   #endif
				Timer t;
				t.start();
				sm.match(result, mol, rules_[rule], aromatic_atoms_);
				if (result.size() == 0) continue;

				// iterate over all matched atoms, and set values accordingly:
				for (Position pos = 0; pos < result.size(); pos++)
				{
					HashSet<const Atom*>& set = result[pos];
					if (set.size() != 1) 
					{
						Log.error() << "Problem with smarts expr " << rules_[rule]  << " in " << __FILE__ << " " << __LINE__ << std::endl;
						continue;
					}

					Atom& atom = *(Atom*)*set.begin();
					atom.setType(types_[rule]);
					atom.setTypeName(names_[rule]);
					assignSpecificValues_(atom);
					// erase the atom from the sets of atoms, that are still to be matched
					aromatic_atoms_.erase(&atom);
					atoms_.erase(&atom);
				}
   #ifdef BALL_MMFF94_TEST
				t.stop();
				rule_times[names_[rule]] += t.getClockTime();
   #endif
			}
			catch(Exception::ParseError e)
			{
				Log.error() << e << std::endl;
			}
		} // rules for one element
	} // all element rule sets
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

MMFF94AtomTyper::MMFF94AtomTyper()
{
}

MMFF94AtomTyper::MMFF94AtomTyper(const MMFF94AtomTyper& t)
	: AtomTyper(t)
{
}

bool MMFF94AtomTyper::setupHydrogenTypes(const String& filename)
{
	LineBasedFile infile(filename);

	vector<String> fields;
	
	while (infile.readLine())
	{
		const String line = infile.getLine();

		// comments and empty lines
		if (line.size() < 2 || line[0] == '*') continue;
		
		if (line.split(fields) < 2)
		{
			continue;
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
									<< filename << " Not enough fields in one line " 
									<< line << std::endl;
			return false;
		}

		partner_type_to_htype_[fields[0]] = fields[1];
	}

	return true;
}

// read MMFFAROM.PAR to map normal to aromatic types
bool MMFF94AtomTyper::setupAromaticTypes(const String& filename)
{
	aromatic_types_5_map_.clear();
	cation_atoms_.clear();

	LineBasedFile infile(filename);
	vector<String> fields;
	
	while (infile.readLine())
	{
		String line;
		try
		{
			line = infile.getLine();

			// comments and empty lines
			if (line.size() < 2 || line[0] == '*') continue;
			
			if (line.split(fields) < 7)
			{
				continue;
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
										<< filename << " Not enough fields in one line " 
										<< line << std::endl;
				return false;
			}

			AromaticType type;
			
			const String old_type = fields[0];
			// special threatment for some of the 5 ring heteroaromatic types later:
			if (old_type.hasSuffix("*")) continue;
			
			type.new_type = fields[1];
			const Size ring_size = fields[3].toUnsignedInt();
			const String L5 = fields[4].toUnsignedInt();
			type.cation = fields[5].toBool();
			type.anion = fields[6].toBool();

			if (ring_size == 5)
			{
				aromatic_types_5_map_[old_type + "|" + L5] = type;

				if (type.cation) cation_atoms_.insert(old_type);
			}
		}
		catch(...)
		{
				Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
										<< filename << " Not enough fields in one line " 
										<< line << std::endl;
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
	if (+it)
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

bool MMFF94AtomTyper::setupSymbolsToTypes(const String& filename)
{
	LineBasedFile infile(filename);
	vector<String> fields;
	
	while (infile.readLine())
	{
		const String line = infile.getLine();

		// comments and empty lines
		if (line.size() < 2 || line[0] == '*') continue;
		
		if (line.split(fields) < 2)
		{
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
									<< filename << " Not enough fields in one line " 
									<< line << std::endl;
			return false;
		}

		try
		{
			id_to_type_[fields[0]] = fields[1].toUnsignedInt();
		}
		catch(...)
		{
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
									<< filename << " " << line << std::endl;
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
				hetero_atom = &atom;
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

void MMFF94ChargeProcessor::setup(const String& filename)
{
	clear();
	LineBasedFile infile(filename);
	vector<String> fields;
	
	while (infile.readLine())
	{
		const String line = infile.getLine();

		// comments and empty lines
		if (line.size() < 2 || line[0] == '*') continue;
		
		if (line.split(fields) < 2)
		{
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
									<< filename << " Not enough fields in one line " 
									<< line << std::endl;
			return;
		}

		if (fields[1] == "*")
		{
			rule_types_.insert(fields[0]);
			continue;
		}

		types_to_charges_[fields[0]] = fields[1].toFloat();
	}
}
		
void MMFF94ChargeProcessor::clear()
	throw()
{
	types_to_charges_.clear();
	rule_types_.clear();
}

const MMFF94ChargeProcessor& MMFF94ChargeProcessor::operator = (const MMFF94ChargeProcessor&)
	throw()
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

			if (+it)
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
			/// ????
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
			if (+hit)
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
	for (; +cit; ++cit)
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

//////////////////////////////////////////////////////////////////////
// Kekuliser
//////////////////////////////////////////////////////////////////////

bool Kekuliser::AtomInfo::operator < (const Kekuliser::AtomInfo& info) const
{
	return (*atom) < *info.atom;
}

Kekuliser::Kekuliser()
{
}

bool Kekuliser::setup(Molecule& mol)
{
	// collect aromatic bonds and atoms to speed up SMARTS matching:
	AtomIterator ait = mol.beginAtom();
	for (; +ait; ++ait)
	{
		aromatic_atoms_.insert(&*ait);
	}


	SmartsMatcher sm;
	// dont recalculate the smallest set of smallest rings:
	// no ring information needed for smarts matcher:
	vector<vector<Atom*> > rings_vector;
 	sm.setSSSR(rings_vector);

	Position nr_ca = 0;

	vector<HashSet<const Atom*> > result;
	HashSet<const Atom*>::Iterator sit;

	//////////////////////////////////////////////////////////////
	// fix carboxlic acid:
	sm.match(result, mol, "[#8;D1]#6[#8;D1]", aromatic_atoms_);

	for (Position pos = 0; pos < result.size(); pos++)
	{
		HashSet<const Atom*>& set = result[pos];
		vector<Atom*> oxygen;
		Atom* carbon = 0;

		for (sit = set.begin(); +sit; ++sit)
		{
			if ((*sit)->getElement().getSymbol() == "C")
			{
				carbon = (Atom*)*sit;
			}
			else
			{
				oxygen.push_back((Atom*)*sit);
			}
		}

		oxygen[0]->getBond(*carbon)->setOrder(Bond::ORDER__SINGLE);
		oxygen[1]->getBond(*carbon)->setOrder(Bond::ORDER__DOUBLE);
		aromatic_atoms_.erase(oxygen[0]);
		aromatic_atoms_.erase(oxygen[1]);
		aromatic_atoms_.erase(carbon);
		nr_ca++;
	}
	
	//////////////////////////////////////////////////////////////
	// fix amidene and guanidine
	// sm.match(result, mol, "[#7;D1]#6([#7;D1])*");
	Size nr_am_gu = 0;
	result.clear();
	sm.match(result, mol, "[#7;D1]#6([#7;D1])", aromatic_atoms_);
	if (result.size() != 0)
	{
		for (Position pos = 0; pos < result.size(); pos++)
		{
			HashSet<const Atom*>& set = result[pos];
			vector<Atom*> nitrogen;
			Atom* carbon = 0;

			for (sit = set.begin(); +sit; ++sit)
			{
				if ((*sit)->getElement().getSymbol() == "C")
				{
					carbon = (Atom*)*sit;
				}
				else
				{
					nitrogen.push_back((Atom*)*sit);
				}
			}

			nitrogen[0]->getBond(*carbon)->setOrder(Bond::ORDER__SINGLE);
			nitrogen[1]->getBond(*carbon)->setOrder(Bond::ORDER__DOUBLE);
			aromatic_atoms_.erase(carbon);
			aromatic_atoms_.erase(nitrogen[0]);
			aromatic_atoms_.erase(nitrogen[1]);
			nr_am_gu++;
		}
	}
	
	//////////////////////////////////////////////////////////////
	// fix phosphonic acid
	result.clear();
	Size nr_phos = 0;
//   	sm.match(result, mol, "[p]([oD1])([oD1])([oD1])[#6,#8]");
	sm.match(result, mol, "[P]([#8;D1])([#8;D1])([#8;D1])", aromatic_atoms_);

	for (Position pos = 0; pos < result.size(); pos++)
	{
		HashSet<const Atom*>& set = result[pos];
		vector<Atom*> oxygen;
		Atom* phosphor = 0;

		for (sit = set.begin(); +sit; ++sit)
		{
			if ((*sit)->getElement().getSymbol() == "P")
			{
				phosphor = (Atom*)*sit;
			}
			else
			{
				oxygen.push_back((Atom*)*sit);
			}
		}
		
		oxygen[0]->getBond(*phosphor)->setOrder(Bond::ORDER__DOUBLE);
		oxygen[1]->getBond(*phosphor)->setOrder(Bond::ORDER__SINGLE);
		oxygen[2]->getBond(*phosphor)->setOrder(Bond::ORDER__SINGLE);

		aromatic_atoms_.erase(phosphor);
		aromatic_atoms_.erase(oxygen[0]);
		aromatic_atoms_.erase(oxygen[1]);
		aromatic_atoms_.erase(oxygen[2]);
		nr_phos++;
	}
	
	//////////////////////////////////////////////////////////////
	// fix aromatic rings
	bool ok = fixAromaticRings_();

	// recollect the remaining aromatic bonds:	
	unassigned_bonds_.clear();
	AtomBondIterator bit;
	BALL_FOREACH_BOND(mol, ait, bit)
	{
		if (bit->getOrder() == Bond::ORDER__AROMATIC)
		{
			unassigned_bonds_.push_back(&*bit);
		}
	}

#ifdef BALL_MMFF94_TEST
	Log.error() << "Kekulized bounds: " <<
	            << "CA   "  << nr_ca << " " 
							<< "NH2  "  << nr_am_gu << " " 
							<< "PHOS "  << nr_phos  << std::endl;
#endif
	return ok;
}


bool Kekuliser::fixAromaticRings_()
{
	if (aromatic_rings_.size() == 0) return true;

	bool ok = true;

	getMaximumValence_();
	calculateAromaticSystems_();

	AtomInfo temp_ai;
	temp_ai.current_charge = 0;
	temp_ai.curr_double = 0;

	// iterate over all aromatic systems:
	vector<HashSet<Atom*> >::iterator rit = aromatic_systems_.begin();
	for (; rit != aromatic_systems_.end(); rit++)
	{
		// abort for strange rings:
		if ((*rit).size() < 3)
		{
			Log.error() << "Kekulizer: Could not assign ring with " << (*rit).size()<< " atoms. " << std::endl;

			if ((*rit).size())
			{
				Log.error() << (**(*rit).begin()).getFullName() << std::endl;
			}
			ok = false;
			continue;
		}

		atom_infos_.clear();

		bool abort_this_ring = false;

		// for one aromatic system: collect all needed informations for the individual atoms:
		HashSet<Atom*>::Iterator hit = (*rit).begin();
		for (; +hit; ++hit)
		{
			Atom& atom = *(Atom*)*hit;

			// calculate the current valence:
			Index curr_valence = 0;
			AtomBondIterator bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				if (bit->getOrder() < 2 ||
						bit->getOrder() > 4)
				{
					curr_valence++;
				}
				else
				{
					curr_valence += (Index) bit->getOrder();
				}
			}

			// calculate the number of bonds that need order of two:
			Index max_double = max_valence_[&atom] - curr_valence;
			if (max_double < 0)
			{
				Log.error() << "Kekulizer: Could not calculate max number of double bonds for " << atom.getFullName() << std::endl;
				Log.error() << "Max: "  << max_valence_[&atom] << "  Curr:  " << curr_valence << std::endl;
				abort_this_ring = true;
				ok = false;
				break;
			}

			// calculate the maximum number of double bonds if atom is to be charged:
			Index max_double_charged = max_double;
			String esym = atom.getElement().getSymbol();
			if (esym == "N" ||
					esym == "S" ||
					esym == "O")
			{
				bool hydrogen_bound = false;
				AtomBondIterator abit = atom.beginBond();
				for (; +abit; ++abit)
				{
					if (abit->getPartner(atom)->getElement().getAtomicNumber() == 1)
					{
						hydrogen_bound = true;
						break;
					}
				}

				if (!hydrogen_bound)
				{
					if (esym == "N" && curr_valence == 3 ||
							esym == "S" && curr_valence == 2 ||
							esym == "O" && curr_valence == 2)
					{
						max_double_charged ++;
					}
				}
			}

			Index min_double_charged = max_double;

			if (esym == "N") min_double_charged = curr_valence;

			atom_infos_.push_back(temp_ai);
			AtomInfo& info = atom_infos_[atom_infos_.size() - 1];
			info.atom = &atom;
			info.max_double_charged = max_double_charged;
			info.min_double_charged = min_double_charged;
			info.min_double 				= max_double;
			info.max_double 				= max_double;

			// collect aromatic bonds for this atom:
			bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				// add aromatic bonds only once:
				if (bit->getOrder() == Bond::ORDER__AROMATIC &&
						*bit->getPartner(atom) > atom) 
				{
					// set bond order initialy to single
					bit->setOrder(Bond::ORDER__SINGLE);
					info.abonds.push_back(&*bit);
				}
			}

		} // all aromatic atoms of this ring

		if (abort_this_ring) continue;

		std::sort(atom_infos_.begin(), atom_infos_.end());

		// put the ids of the atoms partner into a vector per AtomInfo
		HashMap<Atom*, Position> atom_to_id;
		for (Position p = 0; p < atom_infos_.size(); p++)
		{
			atom_to_id[atom_infos_[p].atom] = p;
		}

		for (Position p = 0; p < atom_infos_.size(); p++)
		{
			AtomInfo& ai = atom_infos_[p];
			for (Position b = 0; b < ai.abonds.size(); b++)
			{
				ai.partner_id.push_back(atom_to_id[ai.abonds[b]->getPartner(*ai.atom)]);
			}
		}

		try_charge_ = false;
		if (!fixAromaticSystem_(0)) 
		{
			try_charge_ = true;
			if (!fixAromaticSystem_(0))
			{
				ok = false;
				// we were not successfull, so reset the bonds to aromatic:
				for (Position i = 0; i < atom_infos_.size(); i++)
				{
					AtomInfo& ai = atom_infos_[i];
					for (Position b = 0; b < ai.abonds.size(); b++)
					{
						ai.abonds[b]->setOrder(Bond::ORDER__AROMATIC);
					}
				}
			}
		}
	} // all aromatic systems


	return ok;
}

bool Kekuliser::fixAromaticSystem_(Position it)
{
	// no more bonds and no more atoms?
	if (it >= atom_infos_.size() - 1)
	{
		return idealValenceAchieved_();
	}

	AtomInfo& ai = atom_infos_[it];
	
	// no aromatic bonds left?
	if (ai.abonds.size() == 0)
	{
		return fixAromaticSystem_(it + 1);
	}

	Index max_double = ai.max_double;
	if (try_charge_) max_double = ai.max_double_charged;

	// at full valence?
	if (ai.curr_double == max_double)
	{
		if (fixAromaticSystem_(it + 1))
		{
			return true;
		}
	}
	else // not at full valence!
	{
		// protonierung?
		if (buildConjugatedSystem_(it)) return true;
	}

	for (Position b = 0; b < ai.abonds.size(); b++)
	{
		ai.abonds[b]->setOrder(Bond::ORDER__SINGLE);
	}

	return false;
}


bool Kekuliser::buildConjugatedSystem_(Position it)
{
	AtomInfo& ai = atom_infos_[it];

	for (Position b = 0; b < ai.abonds.size(); b++)
	{
		// get the bond and partner atom:
		Bond* bond = ai.abonds[b];
		Position p = ai.partner_id[b];
		AtomInfo& pi = atom_infos_[p];

		if (pi.curr_double >= pi.max_double)
		{
			continue;
		}

		bond->setOrder(Bond::ORDER__DOUBLE);
		ai.curr_double++;
		pi.curr_double++;

		if (fixAromaticSystem_(it + 1)) return true;

		bond->setOrder(Bond::ORDER__SINGLE);
		ai.curr_double--;
		pi.curr_double--;
	}

	return false;
}


void Kekuliser::calculateAromaticSystems_()
{
	aromatic_systems_.clear();
	aromatic_atoms_.clear();
	// collect all aromatic ring atoms:
	vector<HashSet<Atom*> >::iterator rit = aromatic_rings_.begin();
	for (; rit != aromatic_rings_.end(); ++rit)
	{
		// all atoms in current ring:
		HashSet<Atom*>::Iterator hit = (*rit).begin();
		for (; +hit; ++hit)
		{
			// all bonds of this atom:
			AtomBondIterator bit = (**hit).beginBond();
			for (; +bit; ++bit)
			{
				if (bit->getOrder() == Bond::ORDER__AROMATIC)
				{
					aromatic_atoms_.insert(*hit);
					break;
				}
			}
		}
	}

	// iterate over all aromatic ring atoms:
	while (aromatic_atoms_.size() > 0)
	{
		// first ring aromatic atom that is still left:
		Atom* atom = (Atom*)*aromatic_atoms_.begin();

		current_aromatic_system_.clear();

		collectSystems_(*atom);

		aromatic_systems_.push_back(current_aromatic_system_);
	}
}

void Kekuliser::collectSystems_(Atom& atom)
{
	current_aromatic_system_.insert(&atom);
	aromatic_atoms_.erase(&atom);

	// all bonds of this atom:
	AtomBondIterator abit = atom.beginBond();
	for (; +abit; ++abit)
	{
		// if aromatic bond:
		if (abit->getOrder() != Bond::ORDER__AROMATIC) continue;

		Atom* partner = abit->getPartner(atom);
		
		// if not seen partner before:
		if (!aromatic_atoms_.has(partner)) continue;

		collectSystems_(*partner);
	}
}

void Kekuliser::getMaximumValence_()
{
	max_valence_.clear();

	vector<HashSet<Atom*> >::iterator rit = aromatic_rings_.begin();
	for (; rit != aromatic_rings_.end(); rit++)
	{
		HashSet<Atom*>::Iterator hit = (*rit).begin();
		for (; +hit; ++hit)
		{
			Atom& atom = **hit;
			Index max_valence = 0;
			if (max_valence_.has(&atom)) continue;

//   			Index formal_charge = (Index)atom.getFormalCharge();

			Position atomic_number = atom.getElement().getAtomicNumber();

			switch (atomic_number)
			{
				case 6:
					max_valence = 4;// - (int) fabs(formal_charge);
					break;

				case 8:
				case 16:
				case 34:
				case 52:
					max_valence = 2;// + formal_charge;
					break;

				 case 7:
				 case 15:
				 case 33:
					 max_valence = 3;// + formal_charge;
					 break;
			}

			// Nitrogen and sulfur:
			if (atomic_number == 7 || 
					atomic_number == 16)
			{
				AtomBondIterator abit = atom.beginBond();
				for (; +abit; ++abit)
				{
					if (abit->getOrder() != Bond::ORDER__DOUBLE) continue;

					if (abit->getPartner(atom)->getElement().getAtomicNumber() == 8)
					{
						max_valence += 2;
					}
				}
			}

			max_valence_[&atom] = max_valence;
		}
	}
}


bool Kekuliser::idealValenceAchieved_()
{
	for (Position p = 0; p < atom_infos_.size(); p++)
	{
		AtomInfo& ai = atom_infos_[p];
		if (!try_charge_)
		{
			if (ai.curr_double < ai.min_double &&
					ai.curr_double > ai.max_double)
			{
				return false;
			}
		}

		if (ai.curr_double < ai.min_double_charged &&
				ai.curr_double > ai.max_double_charged)
		{
			return false;
		}
	}

	return true;
}


void Kekuliser::clear()
{
	aromatic_systems_.clear();
	aromatic_rings_.clear();
	unassigned_bonds_.clear();
	aromatic_atoms_.clear();
	max_valence_.clear();
	current_aromatic_system_.clear();
	atom_infos_.clear();
	try_charge_ = false;
}

} // namespace BALL
