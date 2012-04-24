// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/NMR/anisotropyShiftProcessor.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/DATATYPE/string.h>

using namespace std;

namespace BALL
{

	const char* AnisotropyShiftProcessor::PROPERTY__ANISOTROPY_SHIFT = "AnisotropyShift";

	AnisotropyShiftProcessor::AnisotropyShiftProcessor()
		:	ShiftModule(),
			proton_list_(),
			eff_list_(),
			eff_list_2_(),
			ignore_other_chain_(false)
	{
	}

	AnisotropyShiftProcessor::AnisotropyShiftProcessor(const AnisotropyShiftProcessor& processor)
		:	ShiftModule(processor),
			proton_list_(processor.proton_list_),
			eff_list_(processor.eff_list_),
			eff_list_2_(processor.eff_list_2_),
			ignore_other_chain_(processor.ignore_other_chain_)
	{
	}

	AnisotropyShiftProcessor::~AnisotropyShiftProcessor()
		
	{
	}

	void AnisotropyShiftProcessor::init()
	{
		valid_ = false;

		if (parameters_ == 0)
		{
			return;
		}

		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "Anisotropy");

		if (parameter_section.options.has("ignore_other_chain"))
		{
			ignore_other_chain_ = parameter_section.options.getBool("ignore_other_chain");
		}

		valid_ = true;
	}

	bool AnisotropyShiftProcessor::finish()
	{
		// Abort if the parameters were not initialized correctly.
		if (!isValid())
		{
			return false;
		}

		// Abort if there's nothing to do (no protons).
		if (proton_list_.empty())
		{
			return true;
		}

		// Some constants.
		const float dX1   = -13.0;
		const float dX2   =  -4.0;
		const float dXN1  = -11.0;
		const float dXN2  =  -5.0;
		const float ndX1  = -11.0;
		const float ndX2  =   1.4;
		const float ndXN1 =  -7.0;
		const float ndXN2 =   1.0;

	  
		// Iterate over all protons affected.
		std::list<const Atom*>::const_iterator proton_it(proton_list_.begin());
		for (; proton_it != proton_list_.end(); ++proton_it)
		{
			// Total shift is zero initially
			float total_shift = 0.0;

			// Iterate over all effector bonds (all anisotropic bonds).
			std::list<const Bond*>::const_iterator eff_iter(eff_list_.begin());
			for (; eff_iter != eff_list_.end(); ++eff_iter)
			{
				// Iterate over all bonds of each effector atom.
				const Bond* bond = *eff_iter;
				const Atom* c_atom = bond->getFirstAtom();
				const Atom* o_atom = bond->getSecondAtom();
				if (c_atom->getElement() != PTE[Element::C])
				{
					o_atom = bond->getFirstAtom();
					c_atom = bond->getSecondAtom();
				}
				
				// Make surethe proton and the effector are from different residues.
				const Atom* x_atom = 0;
				if ((*proton_it)->getFragment() != c_atom->getFragment())
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
					for (Position pos = 0; pos < c_atom->countBonds(); pos++)
					{
						const Bond& hbond = *c_atom->getBond(pos);
						if (hbond.getBoundAtom(*c_atom)->getName() == name)
						{
							x_atom = hbond.getBoundAtom(*c_atom);
							break;
						}
					}
					for (Position pos = 0; (x_atom == 0) && (pos < o_atom->countBonds()); pos++)
					{
						const Bond& hbond = *o_atom->getBond(pos);
						if (hbond.getBoundAtom(*o_atom)->getName() == name)
						{
							x_atom = hbond.getBoundAtom(*o_atom);
							break;
						}
					}
					// ??? What happens if x_atom is not set.

					if ((c_atom == 0) || (o_atom == 0) || (x_atom == 0))	
					{
						Log.error() << "Could not set all atoms in ASP! c_atom = " << c_atom << "  o_atom = " << o_atom << "  x_atom = " << x_atom << endl;
						Log.error() << "c_atom->getName() = "<< c_atom->getFullName() << "    o_atom->getName() = " << o_atom->getFullName() << endl;
						continue;
					}
					else 
					{
						const Vector3& c_pos = c_atom->getPosition();
						const Vector3& o_pos = o_atom->getPosition();
						const Vector3& x_pos = x_atom->getPosition();

						// Construct an orthogonal coordinate system.
						Vector3 vz(o_pos - c_pos);
						vz.normalize();
						Vector3 vy(vz % (x_pos - c_pos));
						vy.normalize();
						Vector3 vx (vz % vy);
						vx.normalize();
						const Vector3 cen(c_pos + (vz * 1.1));
						const Vector3 v1((*proton_it)->getPosition() - cen);
						const Vector3 v2(v1 % vy);
						const Vector3 v3(v2 % vx);

						const float distance = v1.getLength();
						const float stheta = v2.getLength() / (v1.getLength() * vy.getLength());
						const float sgamma = v3.getLength() / (v2.getLength() * vx.getLength());
						float calc1;
						float calc2;
						if ((*proton_it)->getName() == "H")
						{
							calc1 = dXN1 * ((3.0 * stheta * stheta) - 2.0);
							calc2 = dXN2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
						}
						else
						{
							calc1 = dX1 * ((3.0 * stheta * stheta) - 2.0);
							calc2 = dX2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
						}

						float shift = (calc1 + calc2) / (3.0 * distance * distance * distance);
						// ?????
						// check whether effector and nucleus are in the same chain
						if ((ignore_other_chain_) && ((*proton_it)->getResidue() != 0) && (c_atom->getResidue() != 0))
						{
							if ((*proton_it)->getResidue()->getChain() == c_atom->getResidue()->getChain())
							{
								shift = 0.0;
							}
						}
						total_shift += shift;	
					}
				}
			}

			for (eff_iter = eff_list_2_.begin(); eff_iter != eff_list_2_.end(); ++eff_iter)
			{
				const Bond* bond = *eff_iter;
				const Atom* c_atom = bond->getFirstAtom();
				const Atom* n_atom = bond->getSecondAtom();
				
				if (c_atom->getElement() != PTE[Element::C])
				{
					n_atom = bond->getFirstAtom();
					c_atom = bond->getSecondAtom();
				}
				const Atom* o_atom = 0;

				// Skip the H atom of this residue.
				if ((*proton_it)->getName() == "H" && (*proton_it)->getFragment() == n_atom->getFragment())
				{
					continue;
				}

				for (Position pos = 0; pos < c_atom->countBonds(); pos++)
				{
					const Bond& hbond = *c_atom->getBond(pos);
					if (hbond.getBoundAtom(*c_atom)->getName() == "O")
					{
						o_atom = hbond.getBoundAtom(*c_atom);
						break;
					}
				}
				
				if (o_atom != 0)
				{
					Vector3 c_pos = c_atom->getPosition();
					Vector3 o_pos = o_atom->getPosition();
					Vector3 n_pos = n_atom->getPosition();

					// baue rechtwinkliges Koordinatensystem auf
					Vector3 vz = n_pos - c_pos;
					const float vz_scalar = vz.getLength();
					vz.normalize();
					Vector3 vy = vz % (o_pos - c_pos);
					vy.normalize();
					Vector3 vx = vz % vy;
					vx.normalize();
					const Vector3 cen = c_pos + (vz * (0.85 * vz_scalar));
					const Vector3 v1 = (*proton_it)->getPosition() - cen;
					const Vector3 v2 = v1 % vy;
					const Vector3 v3 = v2 % vx;

					const float distance = v1.getLength();
					const float stheta = v2.getLength() / (v1.getLength() * vy.getLength());
					const float sgamma = v3.getLength() / (v2.getLength() * vx.getLength());
					float calc1, calc2;
					if ((*proton_it)->getName() == "H")
					{
						calc1 = ndXN1 * ((3.0 * stheta * stheta) - 2.0);
						calc2 = ndXN2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
					}
					else
					{
						calc1 = ndX1 * ((3.0 * stheta * stheta) - 2.0);
						calc2 = ndX2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
					}

					// ?????
					float shift = (calc1 + calc2) / (3.0 * distance * distance * distance);	
					// check whether effector and nucleus are in the same chain
					if (ignore_other_chain_ && ((*proton_it)->getResidue() != 0) && (c_atom->getResidue() != 0))
					{
						if ((*proton_it)->getResidue()->getChain() == c_atom->getResidue()->getChain())
						{
							shift = 0.0;
						}
					}
					total_shift += shift;
				}
				else 
				{
					Log.error() << "O atom not found for " << c_atom->getFullName() << "-" << n_atom->getFullName() << endl;
				}
			}
			float shift = (*proton_it)->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
			shift -= total_shift;
			(const_cast<Atom*>(*proton_it))->setProperty(ShiftModule::PROPERTY__SHIFT, shift);
			(const_cast<Atom*>(*proton_it))->setProperty(PROPERTY__ANISOTROPY_SHIFT, -total_shift);
		}

		return true;
	}

	Processor::Result AnisotropyShiftProcessor::operator () (Composite& composite)
	{
		// Collect all effector bonds(C=O) and store them in eff_list_
		// All protons are collected in proton_list_.
		if (!RTTI::isKindOf<Atom>(composite))
		{
			return Processor::CONTINUE;
		}

		const Atom* patom = RTTI::castTo<Atom>(composite);

		if (patom->getElement() == PTE[Element::H])
		{
			proton_list_.push_back(patom);
			return Processor::CONTINUE;
		}

		if (patom->getElement() != PTE[Element::C])
		{
			return Processor::CONTINUE;
		}

		// Figure out whether we found a backbon C atom in a carbonyl group.
		if (patom->getName() == "C" && patom->isBound())
		{
			bool foundN = false;
			bool foundO = false;
			Position bondN = 0;

			// Iterate over all bonds and search for a double bond to an oxygen.
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

		// Search for side-chain effectors in ASP ASN GLU GLN.
		const String& residue_name = patom->getFragment()->getName();

		// Search for ASP and ASN, look for ASP/ASN:CG=OD1
		if ((residue_name == "ASP" || residue_name == "ASN") &&
				 patom->getName() == "CG" && patom->isBound()	)
		{
			// Walk over all bonds and search for a double bond to an oxygen.
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

		// Search for GLU/GLN:CD=OE1.
		if ((residue_name == "GLU" || residue_name == "GLN") &&
				 patom->getName() == "CD" && patom->isBound()	)
		{
			// Walk over all bonds and search for a double bond to an oxygen.
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

}	// namespace BALL
