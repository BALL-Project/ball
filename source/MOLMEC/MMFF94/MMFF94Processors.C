// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.C,v 1.1.2.11 2006/03/02 16:38:09 amoll Exp $
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
			H_id_to_type_[fields[0]] = fields[1].toUnsignedInt();
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


void MMFF94AtomTyper::assignTo(System& s)
{
	AtomIterator ait = s.beginAtom();
	for (; +ait; ++ait)
	{
		ait->setProperty("IsAromatic", false);
	}

	vector<vector<Atom*> > rings;

	RingPerceptionProcessor rpp;
	rpp.calculateSSSR(rings, s);

	AromaticityProcessor ap;
	ap.aromatizeSimple(rings);
	
	for (Position r = 0; r < rings.size(); r++)
	{
		const vector<Atom*>& ring = rings[r];

		for (Position a = 0; a < ring.size(); a++)
		{
			Atom& atom = *(Atom*)ring[a];
			atom.setProperty("IsAromatic", true); 
		}
	}

	AtomTyper::assignTo(s);

	// assign heterocyclic 5 ring members:
	// e.g. C5A C5B N5A N5B
	
	// iterate over all rings
	for (Position r = 0; r < rings.size(); r++)
	{
		vector<Atom*>& ring = rings[r];
		if (ring.size() != 5) continue;

		HashSet<Atom*> ring_atoms;

		for (Position p = 0; p < 5; p++)
		{
			ring_atoms.insert(ring[p]);
		}

		HashSet<Atom*> hetero_atoms;

		for (Position p = 0; p < 5; p++)
		{
			if (ring[p]->getElement().getSymbol() != "C" &&
			    ring[p]->getElement().getSymbol() != "N") 
			{
				hetero_atoms.insert(ring[p]);
			}
			else if (ring[p]->getElement().getSymbol() == "N")
			{
				Size nr_val = 0;
				AtomBondIterator bit = ring[p]->beginBond();
				for (; +bit; ++bit)
				{
					Atom& partner = *bit->getPartner(*ring[p]);
					if (ring_atoms.has(&partner))
					{
						nr_val += bit->getOrder();
					}
				}

				if (nr_val < 3)
				{
					hetero_atoms.insert(ring[p]);
				}
			}
		}

		if (hetero_atoms.size() == 0) continue;

		for (Position p = 0; p < 5; p++)
		{
			Atom& atom = *ring[p];

//   			if (atom.getTypeName() == "NPYL") continue;

			String element = atom.getElement().getSymbol();
			if (element != "N" && element != "C") continue;
			if (atom.getType() > 66) continue;

			bool alpha = false;

			AtomBondIterator bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				Atom& partner = *bit->getPartner(atom);
				if (hetero_atoms.has(&partner))
				{
					alpha = true;
					break;
				}
			}

			String type_name;
			Position type;

			if (alpha)
			{
				if (element == "C") { type = 63; type_name = "C5A";}
				else                { type = 65; type_name = "N5A";}
			}
			else
			{
				if (element == "C") { type = 64; type_name = "C5B";}
				else                { type = 66; type_name = "N5B";}
			}

			if (type_name == "N5B")
			{
				bool NPYL = false;
				AtomBondIterator bit = atom.beginBond();
				for (; +bit; ++bit)
				{
					Atom& partner = *bit->getPartner(atom);
					if (!ring_atoms.has(&partner))
					{
						if (partner.getElement().getSymbol() == "C")
						{
							NPYL = true;
							break;
						}
					}
				}

				if (NPYL) continue;
			}

			atom.setTypeName(type_name);
			atom.setType(type);

		} // all atoms in one ring
	} // all rings: done

	////////////////////////////////////////////////////////////////
	// assign Hydrogen atom types according to their binding partner
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
		if (!H_id_to_type_.has(H_type_name)) continue;

		Position H_nr = H_id_to_type_[H_type_name];

		if (ait->getType() != -1 && ait->getType() != 29)
		{
Log.error() << "#~~#   2 " << ait->getType() << " " << H_nr            << " "  << __FILE__ << "  " << __LINE__<< std::endl;
		}
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
