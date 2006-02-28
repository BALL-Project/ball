// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.C,v 1.1.2.9 2006/02/28 09:43:53 amoll Exp $
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
		if (line == "" || line[0] == '*') continue;
		
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

		HashSet<Atom*> ring_atoms;
		for (Position a = 0; a < ring.size(); a++)
		{
			ring_atoms.insert(ring[a]);
		}

		HashSet<Atom*>::Iterator ait = ring_atoms.begin();
		for (; +ait; ++ait)
		{
			Atom& atom = **ait;
			atom.setProperty("IsAromatic", true); 
			/*
			AtomBondIterator bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				if (ring_atoms.has(bit->getPartner(atom)))
				{
					bit->setOrder(Bond::ORDER__AROMATIC);
				}
			}
			*/
		}
	}

	AtomTyper::assignTo(s);
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
