// $Id: standardPredicates.C,v 1.2 2000/05/19 13:19:49 anker Exp $

#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/CONCEPT/selectable.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL 
{

	// Atom name predicate

	bool AtomNamePredicate::operator () (const Atom& atom) const
	{
    return (atom.getName() == argument_);
 	}

	// Atom type predicate

	bool AtomTypePredicate::operator () (const Atom& atom) const
	{
		return (atom.getTypeName() == argument_);
 	}


	// element predicate

	bool ElementPredicate::operator () (const Atom& atom) const
	{
		return (atom.getElement().getSymbol() == argument_);
	}

	// residue predicate

	bool ResiduePredicate::operator () (const Atom& atom) const
	{
		const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			return (res->getName() == argument_);
		}

		return false;
	}
	
	// residue ID predicate

	bool ResidueIDPredicate::operator () (const Atom& atom) const
	{
    const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			return (res->getID() == argument_);
		}
		
		return false;
	}
	
	// protein predicate

	bool ProteinPredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// chain predicate

	bool ChainPredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// secondary structure predicate

	bool SecondaryStructurePredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// solvent predicate

	bool SolventPredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// backbone predicate

	bool BackBonePredicate::operator () (const Atom& atom) const
	{
		if (atom.hasAncestor(RTTI::getDefault<Residue>()))
		{
			String name = RTTI::castTo<Atom>(atom)->getName();
			if ((name == "C") || (name == "N") || (name == "CA") || (name == "O"))
			{
				return true;
			}
		}

		return false;
	}
	
	// nucleotide predicate

	bool NucleotidePredicate::operator () (const Atom& atom) const
	{
		return RTTI::isKindOf<Nucleotide>(atom);
	}

	bool inRingPredicate::operator () (const Atom& atom) const
	{
		// BAUSTELLE
		return false;
	}

	bool doubleBondsPredicate::operator () (const Atom& atom) const
	{
		String s = argument_;
		s.trim();
		int n = ((String) s[1]).toInt();
		int count = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__DOUBLE)
			{
				count++;
			}
		}

		String op = s[0];
		if (op == "<")
		{
			if (count < n)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (op == ">") 
			{
				if (count > n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else
			{
				if (count == n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		Log.error() << "doubleBond::operator (): Illegal operator " << s[0] << endl;
		return false;
	}
	
	bool tripleBondsPredicate::operator () (const Atom& atom) const
	{
		String s = argument_;
		s.trim();
		int n = ((String) s[1]).toInt();
		int count = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__TRIPLE)
			{
				count++;
			}
		}
		String op = s[0];
		if (op == "<")
		{
				if (count < n)
				{
					return true;
				}
				else
				{
					return false;
				}
		}
		else
		{
			if (op == ">") 
			{
				if (count > n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else
			{
				if (count == n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		Log.error() << "tripleBond::operator (): Illegal operator " << s[0] << endl;
		return false;
	}
	
	bool aromaticBondsPredicate::operator () (const Atom& atom) const
	{
		String s = argument_;
		s.trim();
		int n = ((String) s[1]).toInt();
		int count = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__AROMATIC)
			{
				count++;
			}
		}
		String op = s[0];
		if (op == "<")
		{
			if (count < n)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (op == ">") 
			{
				if (count > n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else
			{
				if (count == n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		Log.error() << "aromaticBond::operator (): Illegal operator " << s[0] << endl;
		return false;
	}
	
	bool numberOfBondsPredicate::operator () (const Atom& atom) const
	{
		String s = argument_;
		s.trim();
		int n = ((String) s[1]).toInt();
		int count = atom.countBonds(); 
		String op = s[0];
		if (op == "<")
		{
			if (count < n)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (op == ">") 
			{
				if (count > n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else
			{
				if (count == n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		Log.error() << "numberOfBonds::operator (): Illegal operator " << s[0] << endl;
		return false;
	}
	
	bool connectedToPredicate::operator () (const Atom& atom) const
	{
		//BAUSTELLE
		return false;
	}

	bool sp3HybridizedPredicate::operator () (const Atom& atom) const
	{
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__SINGLE)
			{
				return false;
			}
		}
		return true;
	}

	bool sp2HybridizedPredicate::operator () (const Atom& atom) const
	{
		int dcount = 0;
		int acount = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__DOUBLE)
			{
				dcount++;
			}
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__AROMATIC)
			{
				acount++;
			}
		}
		if ((dcount == 1) || (acount > 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool spHybridizedPredicate::operator () (const Atom& atom) const
	{
		int dcount = 0;
		int tcount = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__DOUBLE)
			{
				dcount++;
			}
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__TRIPLE)
			{
				tcount++;
			}
		}
		if ((dcount == 2) || (tcount == 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

} // namespace BALL
