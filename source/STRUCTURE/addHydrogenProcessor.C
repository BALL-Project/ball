// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: addHydrogenProcessor.C,v 1.1.2.3 2006/10/24 12:47:25 amoll Exp $
//

#include <BALL/STRUCTURE/addHydrogenProcessor.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/matrix44.h>

namespace BALL
{

	AddHydrogenProcessor::AddHydrogenProcessor()
	{
	}

	AddHydrogenProcessor::~AddHydrogenProcessor()
	{
	}

  Processor::Result AddHydrogenProcessor::operator() (Composite &composite)
	{
		Atom* atom = dynamic_cast<Atom*>(&composite);
		if (atom == 0) return Processor::CONTINUE;

		// number of electrons that have to be delivered through bonds:
		Index con = getConnectivity(*atom);
		
		//
		Size sum_bond_orders = countBondOrders(*atom);
		
		//
		Index h_to_add = con - sum_bond_orders;
		if (h_to_add <= 0) return Processor::CONTINUE;

		float bond_length = 1.2;
		Vector3 atom_position = atom->getPosition();
		Size nr_bonds = atom->countBonds();

		vector<Atom*> partners = getPartners_(*atom);
		if (partners.size() < atom->countBonds())
		{
			Log.error() << "Could not find partner in AddHydrogenProcessor: "
									<< atom->getFullName() << std::endl;
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
				return Processor::CONTINUE;
			}

			// h_to_add == 1
			Vector3 bv = partners[0]->getPosition() - atom_position;
			Vector3 axis = getNormal_(bv);

			Matrix4x4 m;
			m.setRotation(Angle(106, false), axis);
			bv = m * bv;
			if (!normalize_(bv)) bv = Vector3(0, 0, 1);
			bv *= bond_length;
			addHydrogen_(*atom, atom_position - bv);
			return Processor::CONTINUE;
		}

		if (isRingAtom_(*atom))
		{
			// Nitrogen in Ring
			if (con == 3)
			{
			}
			
			// Carbon in Ring
			if (con == 4)
			{
			}
		}

		if (hasMultipleBond_(*atom))
		{
			Vector3 bv = partners[0]->getPosition() - atom_position;
			Matrix4x4 m;

			// e.g. (C[-H][-H]=O)
			if (con == 4 && h_to_add == 2)
			{
				Vector3 bv = partners[0]->getPosition() - atom_position;
				if (!normalize_(bv)) bv = Vector3(-1,0,0);

				Vector3 axis = getNormal_(bv);
				m.setRotation(Angle(120, false), axis);
				bv = m * bv;
				bv *= bond_length;

				addHydrogen_(*atom, atom_position + bv);
				// add second bond
				operator() (*atom);
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
				// add second and third bond
				operator() (*atom);
				return Processor::CONTINUE;
			}

			if (h_to_add == 2)
			{
				// add second bond
				Vector3 bv = partners[0]->getPosition() - atom_position;
				if (Maths::isZero(bv.getLength()))
				{
					addHydrogen_(*atom, atom_position - Vector3(0,0,1));
					return Processor::CONTINUE;
				}

				Vector3 axis = getNormal_(bv);
				Matrix4x4 m;
				m.setRotation(Angle(106, false), axis);
				bv = m * bv;
				if (!normalize_(bv)) bv = Vector3(0, 1, 0);
				bv *= bond_length;
				addHydrogen_(*atom, atom_position + bv);
				// add third bond
				operator() (*atom);
				return Processor::CONTINUE;
			}

			// h_to_add == 1
			Vector3 p1 = partners[0]->getPosition();
			Vector3 p2 = partners[1]->getPosition();
			// connection line between the two partner atoms:
			Vector3 d = p2 - p1;
			if (Maths::isZero(d.getLength()))
			{
				addHydrogen_(*atom, atom_position - Vector3(0,1,0));
				return Processor::CONTINUE;
			}

			// Point between two partner aoms:
			Vector3 p = p1 + d / 2.;
			Vector3 d2 = p - atom_position;
			Matrix4x4 m;
			m.setRotation(Angle(106, false), d);
			Vector3 v = m * d2;
			if (!normalize_(v)) v = Vector3(0, 0, 1);
			v *= bond_length;
			addHydrogen_(*atom, atom_position + v);
		}


		return Processor::CONTINUE;
	}

	bool AddHydrogenProcessor::normalize_(Vector3& v)
	{
		float l = v.getLength();
		if (Maths::isZero(l)) return false;
		v /= l;
		return true;
	}


	Vector3 AddHydrogenProcessor::getNormal_(const Vector3& v)
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

	bool AddHydrogenProcessor::isRingAtom_(Atom& atom)
	{
		return (ring_atoms_.has(&atom));
	}

	bool AddHydrogenProcessor::hasMultipleBond_(Atom& atom)
	{
		AtomBondIterator bit = atom.beginBond();
		for (; +bit; ++bit)
		{
			if (bit->getOrder() != Bond::ORDER__SINGLE)
			{
				return true;
			}
		}
	
		return false;
	}


	vector<Atom*> AddHydrogenProcessor::getPartners_(Atom& atom)
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
		hydrogen->setName(String("H") + atom.getName());
		Composite* parent = atom.getParent();
		if (parent != 0)
		{
			parent->appendChild(*hydrogen);
		}
		atom.createBond(*hydrogen)->setOrder(Bond::ORDER__SINGLE);
	}


	Size AddHydrogenProcessor::getConnectivity(Atom& atom)
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
		}
		else 
		{
			electrons =  18 - group;
		}

		electrons -= atom.getFormalCharge();
		if (electrons < 0) 
		{
			Log.error() << "Could not calculate number of electrons for " 
									<< atom.getFullName() << std::endl;
			return 0;
		}

		return (Size) (electrons);
	}

	Size AddHydrogenProcessor::countBondOrders(Atom& atom)
	{
		float nr = 0;
		AtomBondIterator bit = atom.beginBond();
		for (; +bit; ++bit)
		{
			if (bit->getOrder() == Bond::ORDER__AROMATIC)
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


} //Namespace BALL

