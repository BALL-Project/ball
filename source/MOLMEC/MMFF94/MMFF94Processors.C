// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.C,v 1.1.2.17 2006/03/07 00:00:05 amoll Exp $
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

//    #define BALL_DEBUG_MMFF
#define BALL_DEBUG_TEST

using namespace std;

namespace BALL 
{

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
			continue;
			/*
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
									<< filename << " Not enough fields in one line " 
									<< line << std::endl;
			return false;
			*/
		}

//   if (rules_.size() > 0 && fields[2] == rules_[rules_.size() - 1]) continue;

		lines.push_back(vector<String>());

		const Position pos = lines.size() - 1;
		vector<String>& vs = lines[pos];
		vs.resize(number_expected_fields_);

		for (Position p = 0; p < number_expected_fields_; p++)
		{
			fields[p].trim(" ");
			vs[p] = fields[p];
		}

		if (fields[3] == "") continue;

		names_.push_back(fields[1]);
		types_.push_back(fields[2].toInt());
		rules_.push_back(fields[3]);
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
	AtomIterator ait = mol.beginAtom();
	for(; +ait; ++ait)
	{
		ait->setType(-1);
	}

	SmartsMatcher sm;

	for (Position rule = 0; rule < types_.size(); rule++)
	{
		try
		{
			vector<HashSet<const Atom*> > result = sm.match(mol, rules_[rule]);
			if (result.size() == 0) continue;
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
			}
		}
		catch(Exception::ParseError e)
		{
			Log.error() << e << std::endl;
		}
	}
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

MMFF94AtomTyper::MMFF94AtomTyper()
{
//   	number_expected_fields_ = 5;
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

Log.error() << "#~~#   6 "   << key          << " "  << __FILE__ << "  " << __LINE__<< std::endl;
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
}


void MMFF94AtomTyper::assignTo(System& s)
{
	// delete any previous marks about the aromaticity:
	AtomIterator ait = s.beginAtom();
	for (; +ait; ++ait)
	{
		ait->setProperty("IsAromatic", false);
	}

	// collect all aromatic rings:
	vector<vector<Atom*> > rings;
	RingPerceptionProcessor rpp;
	rpp.calculateSSSR(rings, s);
	AromaticityProcessor ap;
	ap.aromatizeSimple(rings);
	
	// Mark the aromatic atoms:
	for (Position r = 0; r < rings.size(); r++)
	{
		const vector<Atom*>& ring = rings[r];

		for (Position a = 0; a < ring.size(); a++)
		{
			Atom& atom = *(Atom*)ring[a];
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
	for (Position r = 0; r < rings.size(); r++)
	{
		vector<Atom*>& ring = rings[r];

		if (ring.size() == 6)
		{
			for (Position p = 0; p < 6; p++)
			{
				if (ring[p]->getTypeName() == "CNN+")
				{
					ring[p]->setTypeName("CB");
					ring[p]->setType(37);
				}
			}
		}

		// we are only interested in rings of 5 atoms
		if (ring.size() != 5) continue;

		// put all atoms of the ring in a hashset
		HashSet<Atom*> ring_atoms;
		for (Position p = 0; p < 5; p++)
		{
			ring_atoms.insert(ring[p]);
		}

		// collect some informations about the hetero atoms and charged atoms
		Atom* hetero_atom = 0;
		Atom* cation_atom = 0;
		Atom* anion_atom = 0;
Log.error() << "#~~#   5 an "  << anion_atom << "  c " << cation_atom           << " "  << __FILE__ << "  " << __LINE__<< std::endl;

		for (Position p = 0; p < 5; p++)
		{
			Atom& atom = *ring[p];
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
		if (atom_x == 0)
		{
			continue;
		}
		
		// iterate over all atoms of this ring:
		for (Position p = 0; p < 5; p++)
		{
			Atom& atom = *ring[p];

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

  Log.error() << "#~~#   3 "  << atom.getName() << " " << atom.getTypeName() << "  " << L5           << "   ";
			assignAromaticType_5_(atom, L5, anion_atom != 0, cation_atom != 0);
  Log.error() << atom.getTypeName()            << " "  << __FILE__ << "  " << __LINE__<< std::endl;
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

void MMFF94ChargeProcessor::clear()
	throw()
{
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

bool MMFF94ChargeProcessor::finish()
{
	// ???????? partial charge
	
	HashMap<Atom*, float> charges;

	for (Position p = 0; p < atoms_.size(); p++)
	{
		Atom& atom = *atoms_[p];

		// ?????????
		float charge = atom.getVelocity().x;
		atom.setCharge(charge);
		//////////////////

		if (charge >= 0.0 || atom.countBonds() == 0) 
		{
			continue;
		}

		float phi = es_parameters_->getPhi(atom.getType());

		if (phi == MMFF94_INVALID_VALUE) 
		{
			unassigned_atoms_.insert(&atom);
			continue;
		}

		float c = charge * 0.5;
		float d = charge - c;
		atom.setCharge(d);

		c /= (float) atom.countBonds();

		AtomBondIterator bit = atom.beginBond();
		for (; +bit; ++bit)
		{
			Atom* atom2 = (Atom*)bit->getPartner(atom);
			if (!charges.has(atom2))
			{
				charges[atom2] = c;
			}
			else
			{
				charges[atom2] += c;
			}
		}
	}

	HashMap<Atom*, float>::Iterator cit = charges.begin();
	for (; +cit; ++cit)
	{
		Atom& atom = *cit->first;
		atom.setCharge(atom.getCharge() + cit->second);
	}


	for (Position p = 0; p < atoms_.size(); p++)
	{
		Atom& atom = *atoms_[p];
	
		double charge = atom.getCharge();
//   	Log.error() << "#~~#   7 "   << atom.getName() << " " << charge         ;
		AtomBondIterator bit = atom.beginBond();
		Index at1 = atom.getType();
		for (; +bit; ++bit)
		{
			const Atom* const atom2 = bit->getPartner(atom);
			Index at2 = atom2->getType();
			Position bt = bit->hasProperty("MMFF94SBMB"); // ????
			double pcharge;
			
			if (at1 < at2) pcharge = - es_parameters_->getPartialCharge(at1, at2, bt);
			else           pcharge =   es_parameters_->getPartialCharge(at2, at1, bt);

			if (fabs(pcharge) == MMFF94_INVALID_VALUE) 
			{
				unassigned_atoms_.insert(&atom);
				unassigned_atoms_.insert((Atom*)atom2);
				continue;
			}
//   	Log.error() << " " << atom2->getName() << " " << pcharge;
			charge += pcharge;
		}

		atom.setCharge(charge);
//   		Log.error() << "      :  "    << charge         << " "  << __FILE__ << "  " << __LINE__<< std::endl;
	}
	

	return Processor::CONTINUE;
}

bool MMFF94ChargeProcessor::start()
{
	atoms_.clear();
	unassigned_atoms_.clear();
	return es_parameters_ != 0;
}

} // namespace BALL
