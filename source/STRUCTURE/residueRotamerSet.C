// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/residueRotamerSet.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/bond.h>

#include <algorithm>

using namespace std;

namespace BALL
{
	Rotamer::Rotamer()
		: P(0.0),
			chi1(0.0),
			chi2(0.0),
			chi3(0.0),
			chi4(0.0)
	{
	}

	Rotamer::Rotamer(const Rotamer& rotamer)
		:	P(rotamer.P),
			chi1(rotamer.chi1),
			chi2(rotamer.chi2),
			chi3(rotamer.chi3),
			chi4(rotamer.chi4)
	{
	}

	Rotamer::Rotamer(float new_P, float new_chi1, float new_chi2, float new_chi3, float new_chi4)
		: P(new_P),
			chi1(new_chi1),
			chi2(new_chi2),
			chi3(new_chi3),
			chi4(new_chi4)
	{
	}

	// default constructor for ResidueRotamerSet
	ResidueRotamerSet::ResidueRotamerSet()
		: valid_(false),
			name_(""),
			rotamers_(),
			movable_atoms_chi1_(),
			movable_atoms_chi2_(),
			movable_atoms_chi3_(),
			movable_atoms_chi4_(),
			number_of_torsions_(0),
			has_torsion_phi_(false),
			has_torsion_psi_(false),
			phi_(0),
			psi_(0)
	{
	}
        
	// constructor  for ResidueRotamerSet
	ResidueRotamerSet::ResidueRotamerSet(const Residue& residue, Size number_of_torsions)
		:	valid_(false),
			name_(""),
			rotamers_(),
			movable_atoms_chi1_(),
			movable_atoms_chi2_(),
			movable_atoms_chi3_(),
			movable_atoms_chi4_(),
			number_of_torsions_(number_of_torsions),
			has_torsion_phi_(false),
			has_torsion_psi_(false),
			phi_(0),
			psi_(0)

	{
		setTemplateResidue(residue, number_of_torsions);
	}

	bool ResidueRotamerSet::setTemplateResidue(const Residue& residue, Size number_of_torsions)
	{
		valid_ = false;

		// clear old stuff
		original_coordinates_.clear();
		movable_atoms_chi1_.clear();
		movable_atoms_chi2_.clear();
		movable_atoms_chi3_.clear();
		movable_atoms_chi4_.clear();

		// Test if there are some atoms
		if ((residue.countAtoms()) == 0)
		{
			Log.error()  << " ResidueRotamerSet: Residue does not contain any atoms " << std::endl;
			Log.error()  <<  "ResidueRotamerSet: Maybe the residue is not contained in the fragment database..." << std::endl;  

			valid_ = false;
			return false;
		}

		name_ = residue.getName();
		number_of_torsions_ = number_of_torsions;		

		// build hash map and store the original atom coordinates
		for (AtomConstIterator atom_it = residue.beginAtom(); +atom_it; ++atom_it)
		{
			original_coordinates_[atom_it->getName()] = atom_it->getPosition();
		}

		// Create a hash set to store the atoms that mey not be included in the torsion.
		// These are e.g. the backbone atoms or already assigned torsions (problem case: PRO!)
		HashSet<String> assigned_atoms;

		// identify anchor_atoms: those are the atoms used to "anchor" the first torsion to.
		const Atom* atom_ca = residue.getAtom("CA");
		const Atom* atom_c = residue.getAtom("C");
		const Atom* atom_n = residue.getAtom("N");
		if (atom_ca != 0 && atom_c != 0 && atom_n != 0) 
		{
			assigned_atoms.insert("CA");
			assigned_atoms.insert("C");
			assigned_atoms.insert("N");
		} 
		else 
		{
			Log.error() << " ResidueRotamerSet: An anchor atom is missing. " << endl;
			valid_ = false;
			return false;
		}

		// Identify atoms involved in the torsion for chi1 - chi4.
		// and store them in movable_atoms_chi1-4 (first four entries each).

		const Atom* atom_cb = residue.getAtom("CB"); 
		// Determine the atoms for chi1 and the set of movable atoms
		if (atom_cb != 0 && (number_of_torsions_ > 0)) // CA and N already tested above
		{
			movable_atoms_chi1_.push_back("N");
			movable_atoms_chi1_.push_back("CA");
			movable_atoms_chi1_.push_back("CB");

			if (residue.getAtom("CG") != 0) movable_atoms_chi1_.push_back("CG");
			if (residue.getAtom("CG1") != 0) movable_atoms_chi1_.push_back("CG1");
			if (residue.getAtom("OG") != 0) movable_atoms_chi1_.push_back("OG");
			if (residue.getAtom("OG1") != 0) movable_atoms_chi1_.push_back("OG1");
			if (residue.getAtom("NG") != 0) movable_atoms_chi1_.push_back("NG");
			if (residue.getAtom("NG1") != 0) movable_atoms_chi1_.push_back("NG1");
			if (residue.getAtom("SG") != 0) movable_atoms_chi1_.push_back("SG");
			if (residue.getAtom("SG1") != 0) movable_atoms_chi1_.push_back("SG1");

			if (movable_atoms_chi1_.size() > 3)
			{
				addMovable_(movable_atoms_chi1_, *residue.getAtom(movable_atoms_chi1_[2]), assigned_atoms);
				addMovable_(movable_atoms_chi1_, *residue.getAtom(movable_atoms_chi1_[3]), assigned_atoms);
			}
			else
			{
				// If we couldn't find the atoms, just remember this and forget about the torsion.
				number_of_torsions_ = 0;
			}
		} 
		else
		{
			number_of_torsions_ = 0;
		}

		// Determine the atoms for chi2 and the set of movable atoms
		if (movable_atoms_chi1_.size() >= 4 && (number_of_torsions_ > 1))
		{
			movable_atoms_chi2_.push_back(movable_atoms_chi1_[1]);
			movable_atoms_chi2_.push_back(movable_atoms_chi1_[2]);
			movable_atoms_chi2_.push_back(movable_atoms_chi1_[3]);

			if (residue.getAtom("CD") != 0) movable_atoms_chi2_.push_back("CD");
			if (residue.getAtom("CD1") != 0) movable_atoms_chi2_.push_back("CD1");
			if (residue.getAtom("OD") != 0) movable_atoms_chi2_.push_back("OD");
			if (residue.getAtom("OD1") != 0) movable_atoms_chi2_.push_back("OD1");
			if (residue.getAtom("ND") != 0) movable_atoms_chi2_.push_back("ND");
			if (residue.getAtom("ND1") != 0) movable_atoms_chi2_.push_back("ND1");
			if (residue.getAtom("SD") != 0) movable_atoms_chi2_.push_back("SD");
			if (residue.getAtom("SD1") != 0) movable_atoms_chi2_.push_back("SD1");

			if (movable_atoms_chi2_.size() > 3)
			{
				addMovable_(movable_atoms_chi2_, *residue.getAtom(movable_atoms_chi2_[2]), assigned_atoms);
				addMovable_(movable_atoms_chi2_, *residue.getAtom(movable_atoms_chi2_[3]), assigned_atoms);
			}
			else
			{
				// If we couldn't find the atoms, just remember this and forget about the torsion.
				number_of_torsions_ = 1;
			}
		}
		// Determine the atoms for chi3 and the set of movable atoms
		if (movable_atoms_chi2_.size() >= 4 && (number_of_torsions_ > 2))
		{
			movable_atoms_chi3_.push_back(movable_atoms_chi2_[1]);
			movable_atoms_chi3_.push_back(movable_atoms_chi2_[2]);
			movable_atoms_chi3_.push_back(movable_atoms_chi2_[3]);

			if (residue.getAtom("CE") != 0) movable_atoms_chi3_.push_back("CE");
			if (residue.getAtom("CE1") != 0) movable_atoms_chi3_.push_back("CE1");
			if (residue.getAtom("OE") != 0) movable_atoms_chi3_.push_back("OE");
			if (residue.getAtom("OE1") != 0) movable_atoms_chi3_.push_back("OE1");
			if (residue.getAtom("NE") != 0) movable_atoms_chi3_.push_back("NE");
			if (residue.getAtom("NE1") != 0) movable_atoms_chi3_.push_back("NE1");
			if (residue.getAtom("SE") != 0) movable_atoms_chi3_.push_back("SE");
			if (residue.getAtom("SE1") != 0) movable_atoms_chi3_.push_back("SE1");

			if (movable_atoms_chi3_.size() > 3)
			{
				addMovable_(movable_atoms_chi3_, *residue.getAtom(movable_atoms_chi3_[2]), assigned_atoms);
				addMovable_(movable_atoms_chi3_, *residue.getAtom(movable_atoms_chi3_[3]), assigned_atoms);
			}
			else
			{
				// If we couldn't find the atoms, just remember this and forget about the torsion.
				number_of_torsions_ = 2;
			}
		}

		// Determine the atoms for chi4 and the set of movable atoms
		if (movable_atoms_chi3_.size() >= 4 && (number_of_torsions > 3))
		{
			movable_atoms_chi4_.push_back(movable_atoms_chi3_[1]);
			movable_atoms_chi4_.push_back(movable_atoms_chi3_[2]);
			movable_atoms_chi4_.push_back(movable_atoms_chi3_[3]);

			if (residue.getAtom("CZ") != 0) movable_atoms_chi4_.push_back("CZ");
			if (residue.getAtom("OZ") != 0) movable_atoms_chi4_.push_back("OZ");
			if (residue.getAtom("NZ") != 0) movable_atoms_chi4_.push_back("NZ");
			if (residue.getAtom("SZ") != 0) movable_atoms_chi4_.push_back("SZ");

			if (movable_atoms_chi4_.size() > 3)
			{
				addMovable_(movable_atoms_chi4_, *residue.getAtom(movable_atoms_chi4_[2]), assigned_atoms);
				addMovable_(movable_atoms_chi4_, *residue.getAtom(movable_atoms_chi4_[3]), assigned_atoms);
			}
			else
			{
				// If we couldn't find the atoms, just remember this and forget about the torsion.
				number_of_torsions_ = 3;
			}
		}
		
		valid_ = true;
		return true;
	}


	// Copy Constructor
	ResidueRotamerSet::ResidueRotamerSet(const ResidueRotamerSet& rhs)
		: valid_(rhs.valid_),
			name_(rhs.name_),
			rotamers_(rhs.rotamers_),
			movable_atoms_chi1_(rhs.movable_atoms_chi1_),
			movable_atoms_chi2_(rhs.movable_atoms_chi2_),
			movable_atoms_chi3_(rhs.movable_atoms_chi3_),
			movable_atoms_chi4_(rhs.movable_atoms_chi4_),
			number_of_torsions_(rhs.number_of_torsions_),
			original_coordinates_(rhs.original_coordinates_),
			has_torsion_phi_(rhs.has_torsion_phi_),
			has_torsion_psi_(rhs.has_torsion_psi_),
			phi_(rhs.phi_),
			psi_(rhs.psi_)
	{
	}

	ResidueRotamerSet::~ResidueRotamerSet()
	{
	}

	const ResidueRotamerSet& ResidueRotamerSet::operator = (const ResidueRotamerSet& rhs)
	{
		if (this != &rhs)
		{
			valid_ = rhs.valid_;
			name_ = rhs.name_;
			rotamers_ = rhs.rotamers_;
			movable_atoms_chi1_ = rhs.movable_atoms_chi1_;
			movable_atoms_chi2_ = rhs.movable_atoms_chi2_;
			movable_atoms_chi3_ = rhs.movable_atoms_chi3_;
			movable_atoms_chi4_ = rhs.movable_atoms_chi4_;
			number_of_torsions_ = rhs.number_of_torsions_;
			original_coordinates_ = rhs.original_coordinates_;
			has_torsion_phi_ = rhs.has_torsion_phi_;
			has_torsion_psi_ = rhs.has_torsion_psi_;
			phi_ = rhs.phi_;
			psi_ = rhs.psi_;
		}

		return (*this);
	}

	// return the number of rotamers
	Size ResidueRotamerSet::getNumberOfRotamers() const
	{
		return (Size)rotamers_.size();
	}

	// return the number of torsions
	Size ResidueRotamerSet::getNumberOfTorsions() const
	{
		return number_of_torsions_;
	}

	const Rotamer& ResidueRotamerSet::operator [] (Position index) const throw(Exception::IndexOverflow)
	{
  	if (index >= rotamers_.size())
    {
      throw Exception::IndexOverflow(__FILE__, __LINE__, index, rotamers_.size() - 1);
    }
    return rotamers_[index];
  }

	// set the number of torsions
	void ResidueRotamerSet::setNumberOfTorsions(Size number_of_torsions)
		throw(Exception::IndexOverflow)
	{
		if (number_of_torsions > 4)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, number_of_torsions, 4);
		}
		number_of_torsions_ = number_of_torsions;
	}

	// adds a rotamer to a ResidueRotamerSet 
	void ResidueRotamerSet::addRotamer(const Rotamer& rotamer)
  {
		rotamers_.push_back(rotamer);
	}

	void ResidueRotamerSet::deleteRotamer(Iterator loc)
	{
		rotamers_.erase(loc);
	}

	void ResidueRotamerSet::deleteRotamers(Iterator begin, Iterator end)
	{
		rotamers_.erase(begin, end);
	}

	/** This method modifies the atom position of the template side chain
			and then returns a copy of that.
	*/
	/*
	void ResidueRotamerSet::buildRotamer(Residue& residue, const Rotamer& rotamer)
	{*/
		// restore original atom coordinates (see setRotamer)
		/*
		AtomIterator atom_it = residue.beginAtom();
		for (Size index = 0; +atom_it && (index < original_coordinates_.size()); ++atom_it, ++index)
		{
			atom_it->setPosition(original_coordinates_[index]);
		}*/
/*
		for (AtomIterator atom_it = residue.beginAtom(); +atom_it; ++atom_it)
		{
			if (original_coordinates_.has(atom_it->getName()))
			{
				atom_it->setPosition(original_coordinates_[atom_it->getName()]);
			}
			else
			{
				Log.error() << "ResidueRotamerSet: Template does not have atom named '" << atom_it->getName() << "'" << endl;
			}
		}

		// Transform the residue template side_chain_ such that the torsion angles of the template
		// are set to the values stored in rotamer
		setTorsionAngle_(residue, movable_atoms_chi1_, rotamer.chi1);
		setTorsionAngle_(residue, movable_atoms_chi2_, rotamer.chi2);
		setTorsionAngle_(residue, movable_atoms_chi3_, rotamer.chi3);
		setTorsionAngle_(residue, movable_atoms_chi4_, rotamer.chi4);

		//Residue* r = new Residue(side_chain_); 
		//return r; 
	}
*/
	// Get the name of the residue rotamer set
	const String& ResidueRotamerSet::getName() const 
	{
		return name_;
	}

	// Set the name of the residue rotamer set
	void ResidueRotamerSet::setName(const String& name)
	{
		name_ = name;
	}

	// Test if the residue rotamer set is valid
	bool ResidueRotamerSet::isValid() const 
	{
		return valid_;
	}

	// Function for filling the vectors of movable atoms
	void ResidueRotamerSet::addMovable_(vector<String>& movable, const Atom& a, const HashSet<String>& assigned_atoms)
	{
		Atom::BondConstIterator bond_it = a.beginBond();
		
		for (; bond_it != a.endBond(); ++bond_it)
		{
			Atom* b = bond_it->getPartner(a);
			String pattern = b->getName();

			if (find(movable.begin(), movable.end(), pattern) == movable.end()
					&& !assigned_atoms.has(pattern))
			{
				movable.push_back(pattern);
				addMovable_(movable, *b, assigned_atoms);
			}
		}
	}

	// Determine a transformation that sets the torsion of the 4 atoms to the given angle
	void ResidueRotamerSet::setTorsionAngle_(Residue& residue, const vector<String>& movable, float torsion)
	{
		Angle angle(torsion, false);

		// Test if there is a real torsion
		if (movable.size() < 4)
		{
			return ;
		}

		// Initiate Transformation Processor
		TransformationProcessor proc;

		// Compute transformation that moves the torsion atoms into normal position
		Vector3 a1(residue.getAtom(movable[0])->getPosition());
		Vector3 a2(residue.getAtom(movable[1])->getPosition());
		Vector3 a3(residue.getAtom(movable[2])->getPosition());
		Vector3 a4(residue.getAtom(movable[3])->getPosition());
		Vector3	v1(0.0,0.0,0.0);
		Vector3 v2(1.0,0.0,0.0);
		Vector3 v3(0.0,1.0,0.0);

		Matrix4x4 M = StructureMapper::matchPoints(a2, a3, a1, v1, v2, v3);

		// Apply this transformation to all atoms of the residue
		proc.setTransformation(M);
		residue.apply(proc);

		// Compute the rotation matrix (rotation around the x-axes)
		// that sets the torsion angle

		Matrix4x4 R;
		R.m11 = 1.0;	
		
		Vector3 proj_a4(M * a4);
		proj_a4.x = 0.0;
		
		float length_proj_a4 = proj_a4.getLength();

		if (length_proj_a4 != 0.0)
		{
			// Normalize the projection of a4
			proj_a4 /= length_proj_a4;
			
			// Compute cos and sin of the torsion angle 
			float cos_angle = cos(angle);
			float sin_angle = sin(angle);

			// Calculate rotation matrix
			if (proj_a4.y != 0.0)
			{
				R.m23 = (cos_angle * proj_a4.z / proj_a4.y - sin_angle) / (proj_a4.y + proj_a4.z * proj_a4.z / proj_a4.y); 
				R.m22 = (cos_angle - R.m23 * proj_a4.z) / proj_a4.y;	
			}
			else 
			{
				R.m23 = cos_angle / proj_a4.z;
				R.m22 = sin_angle / proj_a4.z;
			}

			R.m32 = -R.m23;
			R.m33 =  R.m22;

			// Rotate all atoms with index larger equal 3
			for (Size i = 3; i < movable.size(); i++)
			{
				// Check if atom exists
				if (residue.getAtom(movable[i]))
				{
					residue.getAtom(movable[i])->setPosition(R * residue.getAtom(movable[i])->getPosition());
				}
				else
				{
					Log.warn() << "ResidueRotamerSet: Rotamer could not be set for atom " << movable[i]
					           << " in residue " << residue.getFullName() << " " << __FILE__ << " "
					           << __LINE__ << std::endl;
				}
			}
		}
	}

	// Transform the side chain such that the torsion angles are identical to the angles of the rotamer
	bool ResidueRotamerSet::setRotamer(Residue& residue, const Rotamer& rotamer) 
	{
		// restore the original side chain atom coordinates (necessary to reproduce rotamers
		// with the desired precision).
		// The application of many successive transformations on the same residue
		// is possible, but leads to slightly different coordinates for
		// the same rotamer at different times.
		
		Residue side_chain(residue);
		for (AtomIterator atom_it = side_chain.beginAtom(); +atom_it; ++atom_it)
		{
			if (original_coordinates_.has(atom_it->getName()))
			{
				atom_it->setPosition(original_coordinates_[atom_it->getName()]);
			}
			else
			{
				Log.error() << "ResidueRotamerSet: Template does not have atom named '" << atom_it->getName() << "'" << endl;
				return false;
			}
		}

		// Transform the residue template side_chain such that the torsion angles of the template
		// are set to the values stored in rotamer
		if (number_of_torsions_ > 0)
		{
			setTorsionAngle_(side_chain, movable_atoms_chi1_, rotamer.chi1);
		}
		if (number_of_torsions_ > 1)
		{
			setTorsionAngle_(side_chain, movable_atoms_chi2_, rotamer.chi2);
		}
		if (number_of_torsions_ > 2)
		{
			setTorsionAngle_(side_chain, movable_atoms_chi3_, rotamer.chi3);
		}
		if (number_of_torsions_ > 3)
		{
			setTorsionAngle_(side_chain, movable_atoms_chi4_, rotamer.chi4);
		}

		// Initiate vectors
		Vector3 a1, a2, a3, z1, z2, z3;
		
		// Initiate counter
		Size counter(0);

		// Search for the 3 backbone atoms that are needed for calculating the transformation
		for (AtomConstIterator atom_it = residue.beginAtom(); atom_it != residue.endAtom(); ++atom_it)
		{
			if ((*atom_it).getName() == "CB")
			{
				z1 = (*atom_it).getPosition();
				counter++;
			}
			if ((*atom_it).getName() == "CA") 
			{
				z2 = (*atom_it).getPosition();
				counter++;
			}
			if ((*atom_it).getName() == "N") 
			{
				z3 = (*atom_it).getPosition();
				counter++;
			}
		}

		if (counter < 3) 
		{
			return false;
		}

		
		// Check if the residue template side_chain_ contains the 3 backbone atoms for the matching
		if (side_chain.getAtom("CB") != 0 && side_chain.getAtom("CA") != 0 && side_chain.getAtom("N") != 0)
		{
			a1 = side_chain.getAtom("CB")->getPosition();
			a2 = side_chain.getAtom("CA")->getPosition();
			a3 = side_chain.getAtom("N")->getPosition();
		}
		else 
		{
			Log.error() << " template of " << residue.getName() 
									<< " does not contain all backbone atoms " << endl;
			return false;
		}

		// Initiate transformation processor
		TransformationProcessor proc;

		// Calculate the transformation
		Matrix4x4 M = StructureMapper::matchPoints(a1, a2, a3, z1, z2, z3);

		// Apply the transformation to side_chain
		proc.setTransformation(M);
		side_chain.apply(proc);

		// Change the coordinates of residue
		for (AtomIterator atom_it = residue.beginAtom(); +atom_it; ++atom_it)
		{
			if (side_chain.getAtom(atom_it->getName()) != 0)
			{
				for (Size i = 3; i < movable_atoms_chi1_.size(); ++i)
				{
					if (movable_atoms_chi1_[i] == atom_it->getName())
					{
						atom_it->setPosition(side_chain.getAtom(atom_it->getName())->getPosition());
						break;
					}
				}
			}
			else 
			{
				Log.error() << " missing template atom " << atom_it->getName() << endl;
				return false;
			}
		}

		return true;
	}


	Rotamer ResidueRotamerSet::getRotamer(const Residue& residue)  const
	{
		Rotamer r;
		r.P = 1.0;

		const Atom* a1 = 0;
		const Atom* a2 = 0;
		const Atom* a3 = 0;
		const Atom* a4 = 0;

		AtomConstIterator atom_it;

		if (number_of_torsions_ > 0)
		{
			Size count(0);
			for (atom_it = residue.beginAtom(); +atom_it; ++atom_it)
			{
				String name(atom_it->getName());
				if (name == movable_atoms_chi1_[0])
				{
					a1 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi1_[1])
				{
					a2 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi1_[2])
				{
					a3 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi1_[3])
				{
					a4 = &*atom_it;
					count++;
				}
			}
			
			if (count == 4)
			{
				r.chi1 = calculateTorsionAngle(*a1, *a2, *a3, *a4).toDegree();
			}
		}

	
		if (number_of_torsions_ > 1)
		{
			Size count(0);
			for (atom_it = residue.beginAtom(); +atom_it; ++atom_it)
			{
				String name(atom_it->getName());
				if (name == movable_atoms_chi2_[0])
				{
					a1 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi2_[1])
				{
					a2 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi2_[2])
				{
					a3 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi2_[3])
				{
					a4 = &*atom_it;
					count++;
				}
			}
			
			if (count == 4)
			{
				r.chi2 = calculateTorsionAngle(*a1, *a2, *a3, *a4).toDegree();
			}
		}

	
		if (number_of_torsions_ > 2)
		{
			Size count(0);
			for (atom_it = residue.beginAtom(); +atom_it; ++atom_it)
			{
				String name(atom_it->getName());
				if (name == movable_atoms_chi3_[0])
				{
					a1 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi3_[1])
				{
					a2 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi3_[2])
				{
					a3 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi3_[3])
				{
					a4 = &*atom_it;
					count++;
				}
			}
			
			if (count == 4)
			{
				r.chi3 = calculateTorsionAngle(*a1, *a2, *a3, *a4).toDegree();
			}
		}

	
		if (number_of_torsions_ > 3)
		{
			Size count(0);
			for (atom_it = residue.beginAtom(); +atom_it; ++atom_it)
			{
				String name(atom_it->getName());
				if (name == movable_atoms_chi4_[0])
				{
					a1 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi4_[1])
				{
					a2 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi4_[2])
				{
					a3 = &*atom_it;
					count++;
				}
				if (name == movable_atoms_chi4_[3])
				{
					a4 = &*atom_it;
					count++;
				}
			}
			
			if (count == 4)
			{
				r.chi4 = calculateTorsionAngle(*a1, *a2, *a3, *a4).toDegree();
			}
		}

		return r;
	}


	const Rotamer& ResidueRotamerSet::getRotamer(Position index) const
	{
		return rotamers_[index % getNumberOfRotamers()];
	}

	bool ResidueRotamerSet::hasTorsionPhi() const
	{
		return has_torsion_phi_;
	}

	Angle ResidueRotamerSet::getTorsionPhi() const
	{
		return phi_;
	}

	void ResidueRotamerSet::setTorsionPhi(const Angle& angle)
	{
		phi_ = angle;
		has_torsion_phi_ = true;
	}

	bool ResidueRotamerSet::hasTorsionPsi() const
	{
		return has_torsion_psi_;
	}

	Angle ResidueRotamerSet::getTorsionPsi() const
	{
		return psi_;
	}

	void ResidueRotamerSet::setTorsionPsi(const Angle& angle)
	{
		psi_ = angle;
		has_torsion_psi_ = true;
	}

	void ResidueRotamerSet::sort()
	{
		std::sort(rotamers_.begin(), rotamers_.end(), RotamerProbabilityGreaterThan_());
	}
 
} // namespace BALL

