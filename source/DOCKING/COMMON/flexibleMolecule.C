// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/DOCKING/COMMON/flexibleMolecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>


namespace BALL
{
	FlexibleMolecule::FlexibleMolecule(Molecule* mol)
	{
		molecule_ownership_ = true;
		molecule = mol;
		Conformation* conf = new Conformation(molecule, molecule);
		addConformation(conf);
		id = conf->getId();
	}

	FlexibleMolecule::FlexibleMolecule(const FlexibleMolecule& flexmol)
	{
		molecule_ownership_ = true;
		molecule = new Molecule(*flexmol.molecule);
		id = flexmol.id;
		for (Size i = 0; i < flexmol.conformations.size(); i++)
		{
			Conformation* conf_copy = new Conformation(*flexmol.conformations[i]);
			addConformation(conf_copy);
		}
	}

	FlexibleMolecule::~FlexibleMolecule()
	{
		if (molecule_ownership_) delete molecule;
		clearConformations();
	}

	void FlexibleMolecule::removeMoleculeOwnership()
	{
		molecule_ownership_ = false;
	}

	void FlexibleMolecule::clearConformations()
	{
		for (Position i = 0; i < conformations.size(); i++)
		{
			delete conformations[i];
		}
		conformations.clear();
		conf_ids_.clear();
	}

	void FlexibleMolecule::addConformation(Conformation* conf)
	{
		// We don't want to insert any conformation twice, so check
		// whether 'conf' has already been added to this FlexibleMolecule
		if (conf_ids_.find(conf->getId()) != conf_ids_.end())
		{
			return;
		}

		if (checkCompatibility(conf))
		{
			conformations.push_back(conf);
			conf_ids_[conf->getId()] = conf;
		}
		else
		{
			throw(Exception::GeneralException(__FILE__, __LINE__, "FlexibleMolecule::addConformation", "conformation not compatible with template molecule"));
		}
	}


	void FlexibleMolecule::calculatePseudoSmile(const Atom* atom, String& smile, HashSet<const Atom*>& visited_atoms, bool ignore_hydrogens)
	{
		if (!atom || visited_atoms.has(atom)) return;
		if (ignore_hydrogens && atom->getElement().getSymbol() == "H")
		{
			return;
		}

		smile += atom->getElement().getSymbol();
		visited_atoms.insert(atom);

		for (Atom::BondConstIterator it = atom->beginBond(); +it; it++)
		{
			const Atom* partner = it->getPartner(*atom);
			if (ignore_hydrogens && partner->getElement().getSymbol() == "H")
			{
				continue;
			}

			if (visited_atoms.has(partner))
			{
				continue;
			}

			int order = it->getOrder();
			String bond_symbol;
			if (order == 2) bond_symbol="=";
			else if(order==3) bond_symbol="#";
			else if (order == 5) bond_symbol="~"; // aromatic bond

			if (it != atom->endBond())
			{
				smile += "("+bond_symbol;
				calculatePseudoSmile(partner, smile, visited_atoms, ignore_hydrogens);
				smile += ")";
			}
			else
			{
				smile += bond_symbol;
				calculatePseudoSmile(partner, smile, visited_atoms, ignore_hydrogens);
			}
		}
	}


	void FlexibleMolecule::generateTopologyHash(const AtomContainer* mol, String& hashkey, bool ignore_hydrogens)
	{
		HashSet<const Atom*> visited_atoms;

		/* USELESS CODE
		 * bool more_than_one_atom = 0;
		 * AtomConstIterator it = mol->beginAtom();
		 * if (it != mol->endAtom())
		 * {
		 *	it++;
		 *	if (it != mol->endAtom()) more_than_one_atom = 1;
	}
	*/

		calculatePseudoSmile(&*mol->beginAtom(), hashkey, visited_atoms, ignore_hydrogens);
		hashkey = QCryptographicHash::hash(QByteArray(hashkey.c_str()), QCryptographicHash::Sha1).toHex().constData();
	}


	bool FlexibleMolecule::checkCompatibility(Conformation* conf)
	{
		// currently only the only compatibility check is whether
		// the number of atoms matches with the number of position
		// vectors in the Conformation object
		return (conf->getNoOfAtoms() == molecule->countAtoms());
	}

	Size FlexibleMolecule::getNumberOfConformations()
	{
		return conformations.size();
	}

	const vector < Conformation* > & FlexibleMolecule::getConformations() const
	{
		return conformations;
	}

	String FlexibleMolecule::getConformationId(Position i)
	{
		if (i < conformations.size())
			return conformations[i]->getId();
		else
		{
			throw(Exception::IndexOverflow(__FILE__, __LINE__, i, conformations.size()));
		}
	}

	Conformation*  FlexibleMolecule::getConformation(Position i)
	{
		if (i < conformations.size())
			return conformations[i];
		else
			//throw(Exception::IndexOverflow(__FILE__, __LINE__, i, conformations.size()));
		return 0;
	}


	Molecule* FlexibleMolecule::getConformer(Position i)
	{
		if (i < conformations.size())
		{
			Molecule* ret = new Molecule(*molecule);
			conformations[i]->applyConformation(*ret);
			return ret;
		}
		else
			//throw(Exception::IndexOverflow(__FILE__, __LINE__, i, conformations.size()));
		return 0;
	}

	Molecule* FlexibleMolecule::getConformerById(String id)
	{
		if (conf_ids_.has(id))
		{
			Molecule* ret = new Molecule(*molecule);
			conf_ids_[id]->applyConformation(*ret);
			return ret;
		}
		else
			return 0;
	}

	Conformation* FlexibleMolecule::getConformationById(String id)
	{
		if (conf_ids_.has(id))
		{
			return conf_ids_[id];
		}
		else
			return 0;
	}

	bool FlexibleMolecule::hasConformation(String id)
	{
		return conf_ids_.has(id);
	}

	Molecule* FlexibleMolecule::getParent()
	{
		return molecule;
	}

	String FlexibleMolecule::getId() const
	{
		return id;
	}

	void FlexibleMolecule::setId(String _id)
	{
		id = _id;
	}
}
