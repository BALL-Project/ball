// $Id: anisotropyShiftProcessor.C,v 1.4 2000/09/20 13:42:13 amoll Exp $

#include<BALL/NMR/anisotropyShiftProcessor.h>

#include<BALL/KERNEL/atom.h>
#include<BALL/KERNEL/PTE.h>
#include<BALL/DATATYPE/string.h>

using namespace std;

namespace BALL
{

	AnisotropyShiftProcessor::AnisotropyShiftProcessor()
		throw()
	{
	}

	AnisotropyShiftProcessor::~AnisotropyShiftProcessor()
		throw()
	{
	}

	bool AnisotropyShiftProcessor::finish()
		throw()
	{
		if (proton_list_.size() == 0)
		{
			return true;
		}

		const float dX1   = -13.0;
		const float dX2   =  -4.0;
		const float dXN1  = -11.0;
		const float dXN2  =  -5.0;
		const float ndX1  = -11.0;
		const float ndX2  =   1.4;
		const float ndXN1 =  -7.0;
		const float ndXN2 =   1.0;

	  list<const PDBAtom*>::iterator proton_iter;
	  list<const Bond*   >::iterator eff_iter;

		// Iteriere über alle Protonen in proton_list_
		for (proton_iter = proton_list_.begin(); proton_iter != proton_list_.end(); ++proton_iter)
		{
			float gs = 0;

			for (eff_iter = eff_list_.begin(); eff_iter != eff_list_.end(); ++eff_iter)
			{
				// Für jedes Proton iteriere über alle Effektorbindungen in eff_list_
				const PDBAtom* patom = *proton_iter;
				const Bond* bond = *eff_iter;
				const Atom* c_atom = (bond->getFirstAtom());
				const Atom* o_atom = (bond->getSecondAtom());
				const Atom* x_atom = 0;
				if ((*proton_iter)->getResidue() != c_atom->getFragment())
				{
					String name = c_atom->getName();
					if (name == "C")
					{
						name = "CA";
					}
					else 
					{
						if (name == "CG")
						{
							name = "CB";
						}
						else 
						{
							if (name == "CD")
							{
								name = "CG";
							}
						}
					}
					for (Position pos = 0; pos < patom->countBonds(); pos++)
					{
						const Bond& hbond = *c_atom->getBond(pos);
						if (hbond.getBoundAtom(*c_atom)->getName() == name)
						{
							x_atom = hbond.getBoundAtom(*c_atom);
							break;
						}
					}
					// was passiert wenn x_atom nicht gesetzt ???

					const Vector3& c_pos = c_atom->getPosition();
					const Vector3& o_pos = o_atom->getPosition();
					const Vector3& x_pos = x_atom->getPosition();

					// baue rechtwinkliges Koordinatensystem auf :
					Vector3 vz = o_pos - c_pos;
					vz /= vz.getLength();
					Vector3 vy = vz % (x_pos - c_pos);
					vy /= vy.getLength();
					Vector3 vx = vz % vy;
					vx /= vx.getLength();
					const Vector3 cen = c_pos + (vz * 1.1);
					const Vector3 v1 = patom->getPosition() - cen;
					const Vector3 v2 = v1 % vy;
					const Vector3 v3 = v2 % vx;

					const float& distance = v1.getLength();
					const float stheta = v2.getLength() / (v1.getLength() * vy.getLength());
					const float sgamma = v3.getLength() / (v2.getLength() * vx.getLength());
					float calc1, calc2;
					if ((*proton_iter)->getName() == "H")
					{
						calc1 = dXN1 * ((3.0 * stheta * stheta) - 2.0);
						calc2 = dXN2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
					}
					else
					{
						calc1 = dX1 * ((3.0 * stheta * stheta) - 2.0);
						calc2 = dX2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
					}
					gs += (calc1 + calc2) / (3.0 * distance * distance * distance);
				}
			}

			for (eff_iter = eff_list_2_.begin(); eff_iter != eff_list_2_.end(); ++eff_iter)
			{
				const PDBAtom* patom = *proton_iter;
				const Bond* bond = *eff_iter;
				const Atom* c_atom = bond->getFirstAtom();
				const Atom* n_atom = bond->getSecondAtom();
				const Atom* o_atom = 0;

				if ((*proton_iter)->getName() == "H" ||
						(*proton_iter)->getResidue() == n_atom->getFragment())
				{
					continue;
				}

				for (Position pos = 0; pos < patom->countBonds(); pos++)
				{
					const Bond& hbond = *c_atom->getBond(pos);
					if (hbond.getBoundAtom(*c_atom)->getName() == "O")
					{
						o_atom = hbond.getBoundAtom(*c_atom);
					}
				}
				const Vector3& c_pos = c_atom->getPosition();
				const Vector3& o_pos = o_atom->getPosition();
				const Vector3& n_pos = n_atom->getPosition();

				// baue rechtwinkliges Koordinatensystem auf
				Vector3 vz = n_pos - c_pos;
				const float vz_scalar = vz.getLength();
				vz.normalize();
				Vector3 vy = vz % (o_pos - c_pos);
				vy.normalize();
				Vector3 vx = vz % vy;
				vx.normalize();
				const Vector3 cen = c_pos + (vz * (0.85 * vz_scalar));
				const Vector3 v1 = patom->getPosition() - cen;
				const Vector3 v2 = v1 % vy;
				const Vector3 v3 = v2 % vx;

				const float& distance = v1.getLength();
				const float stheta = v2.getLength() / (v1.getLength() * vy.getLength());
				const float sgamma = v3.getLength() / (v2.getLength() * vx.getLength());
				float calc1, calc2;
				if ((*proton_iter)->getName() == "H")
				{
					calc1 = ndXN1 * ((3.0 * stheta * stheta) - 2.0);
					calc2 = ndXN2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
				}
				else
				{
					calc1 = ndX1 * ((3.0 * stheta * stheta) - 2.0);
					calc2 = ndX2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
				}

				gs += (calc1 + calc2) / (3.0 * distance * distance * distance);
			}
			float shift = (*proton_iter)->getProperty("chemical_shift").getFloat();
			shift += gs;
			(const_cast<PDBAtom*>(*proton_iter))->setProperty("chemical_shift", shift);
		}
		return true;
	}

	Processor::Result AnisotropyShiftProcessor::operator()(Composite& composite)
		throw()
	{
		// hier werden alle Effektorbindungen gesammelt( C=O ) und in eff_list_ gespeichert.
		// hier werden alle Wasserstoffe in proton_list_ gespeichert.
		if (!RTTI::isKindOf<PDBAtom>(composite))
		{
			return Processor::CONTINUE;
		}

		const PDBAtom* patom = RTTI::castTo<PDBAtom>(composite);

		if (patom->getElement() == PTE[Element::H])
		{
			proton_list_.push_back(patom);
			return Processor::CONTINUE;
		}

		if (patom->getElement() != PTE[Element::C])
		{
			return Processor::CONTINUE;
		}

		// suche im Backbone
		if (patom->getName() == "C" && patom->isBound())
		{
			bool foundN = false;
			bool foundO = false;
			Position bondN;

			// laufe alle Bindungen des Atoms durch und suche nach Sauerstoff-Doppelbindung
			for (Position pos = 0; pos < patom->countBonds(); pos++)
			{
				const Bond* bond = patom->getBond(pos);
				if ((bond->getBoundAtom(*patom)->getName()) == "N")
				{
					foundN = true;
					bondN  = pos;
				}
				if ((bond->getBoundAtom(*patom)->getName()) == "O")
				{
					foundO = true;
					eff_list_.push_back(bond);
				}
			}
			if (foundN && foundO)
			{
				eff_list_2_.push_back(patom->getBond(bondN));
			}
			return Processor::CONTINUE;
		}

		// suche in der Seitenkette nach ASP ASN GLU GLN
		const String& residue_name = patom->getResidue()->getName();

		if ((residue_name == "ASP" || residue_name == "ASN") &&
				 patom->getName() == "CG" && patom->isBound()	)
		{
			// laufe alle Bindungen des Atoms durch und suche nach Sauerstoff-Doppelbindung
			for (Position pos = 0; pos < patom->countBonds(); pos++)
			{
				const Bond* bond = patom->getBond(pos);
				if (bond->getBoundAtom(*patom)->getElement() == PTE[Element::O] &&
						bond->getBoundAtom(*patom)->getName() == "OD1")
				{
					eff_list_.push_back(bond);
				}
			}
			return Processor::CONTINUE;
		}

		if ((residue_name == "GLU" || residue_name == "GLN") &&
				 patom->getName() == "CD" && patom->isBound()	)
		{
			// laufe alle Bindungen des Atoms durch und suche nach Sauerstoff-Doppelbindung
			for (Position pos = 0; pos < patom->countBonds(); pos++)
			{
				const Bond* bond = patom->getBond(pos);
				if (bond->getBoundAtom(*patom)->getElement() == PTE[Element::O] &&
						bond->getBoundAtom(*patom)->getName() == "OE1"  )
				{
					eff_list_.push_back(bond);
				}
			}
		}
		return Processor::CONTINUE;
	}

}	// namespace Ball
