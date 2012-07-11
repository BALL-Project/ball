// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/addHydrogenProcessor.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>

//    #define DEBUG

#ifdef DEBUG
#define DEBUG_LINE Log.error() << "AddHydrogen: " << __LINE__ << std::endl;
#else
#define DEBUG_LINE
#endif

namespace BALL
{

	AddHydrogenProcessor::AddHydrogenProcessor()
		: atom_nr_(0), last_atom_(0), nr_hydrogens_(0)
	{
	}

	AddHydrogenProcessor::~AddHydrogenProcessor()
	{
	}

	Processor::Result AddHydrogenProcessor::operator() (Composite &composite)
	{
		if(composite.isResidue()) {
			return placePeptideBondH_(static_cast<Residue*>(&composite));
		}

		if(!composite.isAtom()) {
			return Processor::CONTINUE;
		}

		Atom* atom = static_cast<Atom*>(&composite);

		if (last_atom_ != atom)
		{
			atom_nr_ = 1;
		}
		else
		{
			atom_nr_ ++;
		}

		last_atom_ = atom;

		// prevent adding Hydrogens, e.g. to aromatic Carboxy group
		if (atom->countBonds() == 1 && atom->getBond(0)->isAromatic())
		{
			return Processor::CONTINUE;
		}

		// number of electrons that have to be delivered through bonds:
		Index con = getConnectivity(*atom);
		
		//
		Size sum_bond_orders = countBondOrders(*atom);
		
		//
		Index h_to_add = con - sum_bond_orders;
		
		if (h_to_add <= 0) return Processor::CONTINUE;

		float bond_length = getBondLength_(atom->getElement().getAtomicNumber());
		Vector3 atom_position = atom->getPosition();
		Size nr_bonds = atom->countBonds();
		Matrix4x4 m;

		vector<Atom*> partners = getPartners_(*atom);
		if (partners.size() < atom->countBonds())
		{
			Log.error() << "Could not find partner in AddHydrogenProcessor: "
									<< atom->getFullName(Atom::ADD_RESIDUE_ID) << std::endl;
			return Processor::CONTINUE;
		}

		// one bond and one Hydrogen missing: (e.g. H-F)
		if (con == 1)
		{
			Vector3 p = atom_position - Vector3(bond_length, 0, 0);
			addHydrogen_(*atom, p);
			return Processor::CONTINUE;
		}

		// linear compounds
		if (h_to_add == 1 && 
				nr_bonds == 1	&& 
				sum_bond_orders > 2)
		{
			Vector3 diff = partners[0]->getPosition() - atom_position;
			if (!normalize_(diff)) diff = Vector3(0, 1, 0);
			diff *= bond_length;
			addHydrogen_(*atom, atom_position - diff);
			DEBUG_LINE
			return Processor::CONTINUE;
		}

		// two partner atoms and a planar 106 degree angle: (e.g. H-O-H)
		if (con == 2)
		{
			if (h_to_add == 2)
			{
				// add first bond
				Vector3 p = atom_position - Vector3(bond_length, 0, 0);
				addHydrogen_(*atom, p);
				// add second bond
				operator() (*atom);
				DEBUG_LINE
				return Processor::CONTINUE;
			}

			// h_to_add == 1
			Vector3 bv = partners[0]->getPosition() - atom_position;
			Vector3 axis = getNormal_(bv);

			m.setRotation(Angle(106, false), axis);
			bv = m * bv;
			if (!normalize_(bv)) bv = Vector3(0, 0, 1);
			bv *= bond_length;
			addHydrogen_(*atom, atom_position - bv);
			DEBUG_LINE
			return Processor::CONTINUE;
		}

		// Ring atoms:
		if (isRingAtom_(*atom))
		{
			Vector3 v1 = partners[0]->getPosition() - atom_position;
			Vector3 v2 = partners[1]->getPosition() - atom_position;
			if (Maths::isZero(v1.getLength())) v1 = Vector3(1,0,0);
			if (Maths::isZero(v2.getLength())) v2 = Vector3(0,1,0);
			v1.normalize();
			v2.normalize();

			Vector3 v3 = -(v1 + v2);
			if (Maths::isZero(v3.getLength())) v3 = Vector3(0,0,1);
			v3.normalize();
			v3 *= bond_length;

			// e.g. Nitrogen in Ring
			if (con == 3 || (con == 4 && atom->getFormalCharge() == 1))
			{
				if (h_to_add == 1)
				{
					addHydrogen_(*atom, atom_position + v3);
					DEBUG_LINE
					return Processor::CONTINUE;
				}
			}
			
			// Carbon in Ring
			if (con == 4)
			{
				Vector3 vx = v2 - v1;

				if (h_to_add == 2)
				{
					m.setRotation(Angle(60, false), vx);
					addHydrogen_(*atom, atom_position + m * v3);
					m.setRotation(Angle(-60, false), vx);
					addHydrogen_(*atom, atom_position + m * v3);
					DEBUG_LINE
					return Processor::CONTINUE;
				}

				if (h_to_add == 1)
				{
					if (atom->countBonds() == 3)
					{
						// maybe an other Hydrogen was already added?
						AtomBondIterator abit = atom->beginBond();
						for (; +abit; ++abit)
						{
							Atom* partner = abit->getPartner(*atom);
							if (partner->getElement().getAtomicNumber() != 1) continue;
							m.setRotation(Angle(60, false), vx);
							partner->setPosition(atom_position + m * v3);
							m.setRotation(Angle(-60, false), vx);
							addHydrogen_(*atom, atom_position + m * v3);
							DEBUG_LINE
							return Processor::CONTINUE;
						}
					}

					// planar and 1 atom to add:
					if (atom->countBonds() == 2)
					{
						addHydrogen_(*atom, atom_position + v3);
						DEBUG_LINE
					}
					else
					{
						v3 = -(v1 % v2);
						if (Maths::isZero(v3.getLength())) v3 = Vector3(0,0,1);
						v3.normalize();
						v3 *= bond_length;

						addHydrogen_(*atom, atom_position + v3);
						DEBUG_LINE
					}

					return Processor::CONTINUE;

					// not planar and one hydrogen to add
					return Processor::CONTINUE;
				}
			}
		}

		if (hasMultipleBond_(*atom))
		{
			Vector3 bv = partners[0]->getPosition() - atom_position;

			// e.g. (C[-H][-H]=O) or (H-N=O)
			if ((con == 4 && h_to_add == 2) ||
					(con == 3 && h_to_add == 1))
			{
				Vector3 bv = partners[0]->getPosition() - atom_position;
				if (!normalize_(bv)) bv = Vector3(-1,0,0);

				Vector3 axis = getNormal_(bv);
				m.setRotation(Angle(120, false), axis);
				bv = m * bv;
				bv *= bond_length;

				addHydrogen_(*atom, atom_position + bv);
				DEBUG_LINE
				// add second bond ?
				if (h_to_add == 2) operator() (*atom);
				return Processor::CONTINUE;
			}
			
			// e.g. (C[-H][-H]=O)
			if (con == 4 && h_to_add == 1)
			{
				Vector3 p1 = partners[0]->getPosition() - atom_position;
				Vector3 p2 = partners[1]->getPosition() - atom_position;
				if (!normalize_(p1)) p1 = Vector3(0,1,0);
				if (!normalize_(p2)) p2 = Vector3(0,0,1);

				Vector3 v = p1 + p2;
				if (!normalize_(v)) v = Vector3(1,0,0);
				v *= bond_length;

				addHydrogen_(*atom, atom_position - v);
				DEBUG_LINE
				return Processor::CONTINUE;
			}
		}
		
		// three partner atoms and a 106 degree angle: (NH3)
		if (con == 3)
		{
			if (h_to_add == 3)
			{
				// add first bond
				Vector3 p = atom_position - Vector3(bond_length, 0, 0);
				addHydrogen_(*atom, p);
				DEBUG_LINE

				return operator() (*atom);
			}

			if (h_to_add == 2)
			{
				// add second bond
				Vector3 bv = partners[0]->getPosition() - atom_position;
				if (!normalize_(bv)) bv = Vector3(0, 1, 0);

				Vector3 axis = getNormal_(bv);
				m.setRotation(Angle(112.754181, false), axis);
				axis = m * bv;

				m.setRotation(Angle(120.0, false), axis);
				Vector3 new_pos = m * bv;
				addHydrogen_(*atom, atom_position + new_pos * bond_length);
				addHydrogen_(*atom, atom_position + m * new_pos * bond_length );

				DEBUG_LINE
				// add third bond
				return Processor::CONTINUE;
			}

			if (h_to_add == 1)
			{
				//TODO: This can be improved further. However the approximation
				//      should provide a good placement.
				Vector3 p1 = partners[0]->getPosition();
				Vector3 p2 = partners[1]->getPosition();
				// connection line between the two partner atoms:
				Vector3 d = p2 - p1;
				if (Maths::isZero(d.getLength()))
				{
					addHydrogen_(*atom, atom_position - Vector3(0,1,0));
					DEBUG_LINE
					return Processor::CONTINUE;
				}

				// Point between two partner aoms:
				Vector3 p = p1 + d / 2.;
				Vector3 d2 = p - atom_position;
				m.setRotation(Angle(117.3, false), d);
				Vector3 v = m * d2;
				if (!normalize_(v)) v = Vector3(0, 0, 1);
				v *= bond_length;
				addHydrogen_(*atom, atom_position + v);
				DEBUG_LINE
			}
		}

		// Carbon without double bonds and not in ring: 
		// tetrahedral: e.g. CH4
		if (con == 4)
		{
			if (h_to_add == 4)
			{
				// add first hydrogen randomly
				addHydrogen_(*atom,atom_position + Vector3(bond_length, 0, 0));
				DEBUG_LINE

				// continue with the next case:
				return operator() (*atom);
			}

			Vector3 v = partners[0]->getPosition() - atom_position;
			if (!normalize_(v)) v = Vector3(0,1,0);

			if (h_to_add == 3)
			{
				// Rotate the partner atom around the target atom
				// in order to obtain the first hydrogen
				Vector3 axis = getNormal_(v);
				m.setRotation(Angle(109.471221, false), axis);
				Vector3 new_pos = m * v * bond_length;
				addHydrogen_(*atom, atom_position + new_pos);

				// Create two copies of the first hydrogen by rotating
				// for 120 degrees.
				m.setRotation(Angle(120, false), v);
				new_pos = m * new_pos;
				addHydrogen_(*atom, atom_position + new_pos);
				new_pos = m * new_pos;
				addHydrogen_(*atom, atom_position + new_pos);

				DEBUG_LINE
				// add 2 other bonds
				return Processor::CONTINUE;
			}

			Vector3 v2 = partners[1]->getPosition() - atom_position;
			if (!normalize_(v2)) v2 = Vector3(0,0,1);

			if (h_to_add == 2)
			{
				// Create a normal to the plane defined by the atom and its two partners
				Vector3 v12 = partners[1]->getPosition() - partners[0]->getPosition();
				if (!normalize_(v12)) v12 = Vector3(0, 1, 0);

				Vector3 norm = v % v2;

				// The new hydrogen atoms are obtained by rotating the normal around the
				// connection between the two partner atoms
				m.setRotation(Angle(-(180 - 109.471221)/2.0, false), v12);
				Vector3 new_pos = m * norm * bond_length;
				addHydrogen_(*atom, atom_position + new_pos);

				m.setRotation(Angle(-109.471221, false), v12);
				new_pos = m * new_pos * bond_length;
				addHydrogen_(*atom, atom_position + new_pos);
				DEBUG_LINE

				return Processor::CONTINUE;
			}

			if (h_to_add == 1)
			{
				Vector3 v3 = partners[2]->getPosition() - atom_position;
				if (!normalize_(v3)) v3 = Vector3(1,0,0);

				Vector3 v4;

				v4 = v + v2 + v3;
				DEBUG_LINE

				if (!normalize_(v4)) v4 = Vector3(1,0,0);

				v4 *= bond_length;
				addHydrogen_(*atom, atom_position - v4);
				return Processor::CONTINUE;
			}
		} // end carbon

		return Processor::CONTINUE;
	}


	bool AddHydrogenProcessor::normalize_(Vector3& v) const
	{
		float l = v.getLength();
		if (Maths::isZero(l)) 
		{
			Log.error() << "Warning in AddHydrogens: found 2 atoms with same positions!" << std::endl;
			return false;
		}
		v /= l;
		return true;
	}


	Vector3 AddHydrogenProcessor::getNormal_(const Vector3& v) const
	{
		Vector3 n = v % Vector3(1,0,0);
		if (!normalize_(n))
		{ 
			n = v % Vector3(0,1,0);
			if (!normalize_(n))
			{
				n = v % Vector3(0,0,1);
			}
		}

		return n;
	}


	bool AddHydrogenProcessor::isRingAtom_(const Atom& atom) const
	{
		return (ring_atoms_.has(&atom));
	}


	bool AddHydrogenProcessor::hasMultipleBond_(const Atom& atom) const
	{
		AtomBondConstIterator bit = atom.beginBond();
		for (; +bit; ++bit)
		{
			if (bit->getOrder() != Bond::ORDER__SINGLE)
			{
				return true;
			}
		}
	
		return false;
	}


	vector<Atom*> AddHydrogenProcessor::getPartners_(Atom& atom) const
	{
		vector<Atom*> partners;
		AtomBondIterator bit = atom.beginBond();
		for (; +bit; ++bit)
		{
			Atom* partner = bit->getPartner(atom);
			if (partner != 0) 
			{
				partners.push_back(partner);
			}
		}

		return partners;
	}


	void AddHydrogenProcessor::addHydrogen_(Atom& atom, Vector3 position)
	{
		Atom* hydrogen = new Atom();
		hydrogen->setElement(PTE[1]);
		hydrogen->setPosition(position);
		String name("H");
		if (atom_nr_ != 1) name += String(atom_nr_);
		name += atom.getName();
		hydrogen->setName(name);
		Composite* parent = atom.getParent();
		if (parent != 0)
		{
			parent->appendChild(*hydrogen);
		}
		atom.createBond(*hydrogen)->setOrder(Bond::ORDER__SINGLE);
		nr_hydrogens_++;
	}


	Size AddHydrogenProcessor::getConnectivity(const Atom& atom) const
	{
		const Element& element = atom.getElement();
		Size group = element.getGroup();
		if (group < 1 || (group > 2 && group < 13))
		{
			return 0;
		}

		Index electrons = 0;
		if (group < 3) 
		{
			electrons = group;
			electrons -= atom.getFormalCharge();
		}
		else 
		{
			electrons =  18 - group;
			electrons += atom.getFormalCharge();
		}

		if (electrons < 0) 
		{
			Log.error() << "Could not calculate number of electrons for "
			            << atom.getFullName(Atom::ADD_RESIDUE_ID) << std::endl;
			return 0;
		}

		return (Size) (electrons);
	}

	Processor::Result AddHydrogenProcessor::placePeptideBondH_(Residue* res)
	{
		const float BOND_LENGTH_N_H = 1.020f;

		if(res->isAminoAcid() && !res->isNTerminal())
		{
			const Residue* prev = res->getPrevious(RTTI::getDefault<Residue>());

			if(!prev)
			{
				return Processor::CONTINUE;
			}

			Atom* natom = res->getAtom("N");
			const Atom* oatom = prev->getAtom("O");
			const Atom* catom = prev->getAtom("C");

			if(!natom || !oatom || !catom || natom->countBonds() >= 3)
			{
				return Processor::CONTINUE;
			}

			// Place the hydrogen according using the planarity of the peptide bond
			const Vector3 OC(oatom->getPosition() - catom->getPosition());
			const float length = OC.getLength();

			if (Maths::isZero(length))
			{
				return Processor::CONTINUE;
			}

			// Create the new atom
			Atom* hatom = new Atom();
			hatom->setName("H");
			hatom->setElement(PTE[1]);
			hatom->setPosition(natom->getPosition() - (OC * BOND_LENGTH_N_H) / length);
			natom->createBond(*hatom)->setOrder(Bond::ORDER__SINGLE);

			res->appendChild(*hatom);

			++nr_hydrogens_;
		}

		return Processor::CONTINUE;
	}

	Size AddHydrogenProcessor::countBondOrders(const Atom& atom) const
	{
		float nr = 0;
		AtomBondConstIterator bit = atom.beginBond();
		for (; +bit; ++bit)
		{
			if (bit->isAromatic())
			{
				nr += 1.5;
				continue;
			}

			if (bit->getOrder() >= Bond::ORDER__SINGLE &&
					bit->getOrder() <= Bond::ORDER__QUADRUPLE)
			{
				nr += bit->getOrder();
			}
		}

		return (Size)(nr);
	}


	void AddHydrogenProcessor::setRings(const vector<vector<Atom*> >& rings)
	{
		ring_atoms_.clear();

		for (Position i = 0; i < rings.size(); i++)
		{
			for (Position j = 0; j < rings[i].size(); j++)
			{
				ring_atoms_.insert(rings[i][j]);
			}
		}
	}

	// Calculate the reference bond length value using a modified Schomaker-Stevenson rule
	// (taken from MMFF94 force field)
	float AddHydrogenProcessor::getBondLength_(Position element) const
	{
		// currently only supports atoms up to Xenon
		if (element > 53 || element == 0) return 1;

		double re = MMFF94StretchParameters::radii[element - 1];
		// if no radius available for the element:
		if (re == 0) return 1;
		double rh = MMFF94StretchParameters::radii[0];

    //  c and n are constants defined in R.Blom and A. Haaland,
    //  J. Molec. Struc, 1985, 128, 21-27.
		// calculate proportionality constant c
		double c = 0.05;

		// POWER
		const double n = 1.4;

		const double diff_e = fabs((double)(MMFF94StretchParameters::electronegatives[0] - 
																				MMFF94StretchParameters::electronegatives[element - 1]));

		// FORMULA 
		return (float)(re + rh - c * pow(diff_e, n));
	}

	bool AddHydrogenProcessor::start()
	{
		nr_hydrogens_ = 0;
		return true;
	}

} //Namespace BALL

