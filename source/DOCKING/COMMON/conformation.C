// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/conformation.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <map>
#include <set>

using namespace std;

namespace BALL
{
	Conformation::Conformation()
	{
		id_ = "";
	}

	Conformation::Conformation(AtomContainer* parent, AtomContainer* conformation_molecule)
	{
		generateHash(conformation_molecule, id_);
		takeConformation(*parent, *conformation_molecule);
	}

	Conformation::Conformation (const Conformation& conf)
	: id_(conf.id_),
	source_(conf.source_),
	number_of_atoms_(conf.number_of_atoms_),
	atom_positions_(conf.atom_positions_),
	properties_(conf.properties_)

	{

	}

	Conformation::~Conformation()
	{
	}

	Size Conformation::getNoOfAtoms() const
	{
		return number_of_atoms_;
	}


	// convert double to String with precision of 2
	String Conformation::doubleToString(const double& value)
	{
		double tmp = value*100;
		double i;
		if (modf(tmp, &i) > 0.5) tmp = ceil(tmp);
		else tmp = floor(tmp);

		tmp /= 100;
		String t(tmp);
		int index = t.find_last_of(".");
		if (index >= 0)
		{
			int size = t.size();
			for (Size i = 0; i < 3 && index < size; i++)
			{
				index++;
			}
			t = t.substr(0, index);
		}

		return t;
	}


	void Conformation::generateHash(const AtomContainer* mol, String& hash)
	{
		String input_string="";

		map<const Atom*, Size> atom_map;
		set<const Bond*> visisted_bonds;
		Size i = 0;
		for (AtomConstIterator it = mol->beginAtom(); +it; it++, i++)
		{
			const Vector3& pos = it->getPosition();
			input_string += it->getElement().getSymbol() +doubleToString(pos[0])+doubleToString(pos[1])+doubleToString(pos[2]);
			atom_map.insert(make_pair(&*it, i));
		}

		for (AtomConstIterator it = mol->beginAtom(); +it; it++)
		{
			for (Atom::BondConstIterator bond_it = it->beginBond(); +bond_it; bond_it++)
			{
				if (visisted_bonds.find(&*bond_it) != visisted_bonds.end())
				{
					continue;
				}
				Size a1 = atom_map[bond_it->getFirstAtom()];
				Size a2 = atom_map[bond_it->getSecondAtom()];
				input_string += String(a1)+String(a2)+String(bond_it->getOrder());
				visisted_bonds.insert(&*bond_it);
			}
		}

		hash = QCryptographicHash::hash(QByteArray(input_string.c_str()), QCryptographicHash::Sha1).toHex().constData();
	}


	void Conformation::generateHash(const AtomContainer* mol, const HashMap < Size, Vector3 > & positions, String& hash)
	{
		String input_string="";

		map<const Atom*, Size> atom_map;
		set<const Bond*> visisted_bonds;
		Size i = 0;
		AtomConstIterator it = mol->beginAtom();
		for (; it != mol->endAtom(); it++, i++)
		{
			HashMap<Size, Vector3>::const_iterator s_it = positions.find(i);
			if (s_it != positions.end())
			{
				input_string += it->getElement().getSymbol()+doubleToString(s_it->second[0])+doubleToString(s_it->second[1])+doubleToString(s_it->second[2]);
			}
			else
			{
				const Vector3& pos = it->getPosition();
				input_string += it->getElement().getSymbol()+doubleToString(pos[0])+doubleToString(pos[1])+doubleToString(pos[2]);
			}
			atom_map.insert(make_pair(&*it, i));
		}

		for (AtomConstIterator it = mol->beginAtom(); +it; it++)
		{
			for (Atom::BondConstIterator bond_it = it->beginBond(); +bond_it; bond_it++)
			{
				if (visisted_bonds.find(&*bond_it) != visisted_bonds.end())
				{
					continue;
				}
				Size a1 = atom_map[bond_it->getFirstAtom()];
				Size a2 = atom_map[bond_it->getSecondAtom()];
				input_string += String(a1)+String(a2)+String(bond_it->getOrder());
				visisted_bonds.insert(&*bond_it);
			}
		}

		hash = QCryptographicHash::hash(QByteArray(input_string.c_str()), QCryptographicHash::Sha1).toHex().constData();
	}


	bool Conformation::operator == (const Conformation& conf)
	{
		if (atom_positions_.size() != conf.atom_positions_.size())
		{
			return false;
		}
		for (Size i = 0; i < atom_positions_.size(); i++)
		{
			if ((atom_positions_[i]-conf.atom_positions_[i]).getLength() > 1e-02)
			{
				return false;
			}
		}
		return true;
	}

	bool Conformation::operator != (const Conformation& conf)
	{
		return !operator == (conf);
	}

	void Conformation::takeConformation(const AtomContainer* parent, AtomContainer* ac)
	{
		takeConformation(*parent, *ac);
	}

	void Conformation::takeConformation(const AtomContainer& parent, const AtomContainer& ac)
	{
		source_ = &ac;

		AtomConstIterator atom_it = ac.beginAtom();
		AtomConstIterator parent_it = parent.beginAtom();
		atom_positions_.clear();

		Size i = 0;
		for (; +atom_it && +parent_it; ++atom_it, ++parent_it, ++i)
		{
			if (atom_it->getPosition().getDistance(parent_it->getPosition()) > 1e-02)
			{
				atom_positions_.insert(make_pair(i, atom_it->getPosition()));
			}
		}
		if (atom_it != ac.endAtom() || parent_it != parent.endAtom())
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Conformation::takeConformation() error", "Number of atoms of given molecule does not match the number atoms of the given parent molecule!");
		}

		number_of_atoms_ = i;
		properties_.clear();
		Size no_prop = ac.countProperties();
		for (Size i = 0; i < no_prop; i++)
		{
			const NamedProperty& prop = ac.getNamedProperty(i);
			addProperty(prop.getName(), prop.toString());
		}
	}

	void Conformation::applyConformation(AtomContainer& ac) const
	{
		AtomIterator atom_it = ac.beginAtom();

		Size i = 0;
		for (; +atom_it; ++atom_it, ++i)
		{
			HashMap<Size, Vector3>::const_iterator it = atom_positions_.find(i);
			if (it != atom_positions_.end())
			{
				atom_it->setPosition(it->second);
			}
		}

		if (i != number_of_atoms_)
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Conformation::applyConformation() error", "Number of atoms of given molecule does not match the number atoms of this Conformation!");
		}

		for (Size i = 0; i < properties_.size(); i++)
		{
			ac.setProperty(properties_[i].first, properties_[i].second);
		}
	}

	const HashMap < Size, Vector3 > * Conformation::getAtomPositions()
	{
		return &atom_positions_;
	}

	void Conformation::setAtomPositions(const AtomContainer* parent, const HashMap < Size, Vector3 > & pos)
	{
		atom_positions_ = pos;
		number_of_atoms_ = parent->countAtoms();
		generateHash(parent, pos, id_);
	}

	String Conformation::getId()
	{
		return id_;
	}

	void Conformation::setID(String& hash)
	{
		id_ = hash;
	}

	bool Conformation::checkID(String& hash)
	{
		return id_ == hash;
	}

	const AtomContainer* Conformation::getParent()
	{
		return source_;
	}

	void Conformation::setParent(AtomContainer* parent)
	{
		source_ = parent;
	}

	void Conformation::addProperty(const String& name, const String& value)
	{
		// Do not store internal IDs as molecule-properties.
		// Also do not store score-properties, because they are saved in Result objects and will be automatically added to the created Molecule by DockingFile::read().
		if (name == "Ligand_UID" || name == "Conformation_input_UID" || name.hasPrefix("score"))
		{
			return;
		}
		String key = name;

		// substitute character within key that would produce xml-reader errors
		key.substituteAll(" ", "_");
		key.substituteAll("/", "_");
		key.substituteAll(":", "");
		properties_.push_back(make_pair(key, value));
	}

	const vector < pair < String, String > > * Conformation::getProperties()
	{
		return &properties_;
	}
}
