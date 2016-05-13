// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/MATHS/matrix44.h>

#include <cstdio>
#include <cmath>
#include <deque>
#include <set>

namespace BALL 
{

	bool BoundingBoxProcessor::start()
		
	{
		lower_.set(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		upper_.set(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
		return true;
	}

	bool BoundingBoxProcessor::finish()
		
	{
		if ((lower_.x == std::numeric_limits<float>::max()) && (lower_.y == std::numeric_limits<float>::max()) && (lower_.z == std::numeric_limits<float>::max())
				&& (upper_.x == -std::numeric_limits<float>::max()) && (upper_.y == -std::numeric_limits<float>::max()) && (upper_.z == -std::numeric_limits<float>::max()))
		{
			lower_.set(0, 0, 0);
			upper_.set(0, 0, 0);

			return false;
		}
		return true;
	}


	Processor::Result BoundingBoxProcessor::operator()(const Vector3& v)
		
	{

		if (lower_.x > v.x) 
		{
			lower_.x = v.x;
		}

		if (lower_.y > v.y) 
		{
			lower_.y = v.y;
		}

		if (lower_.z > v.z) 
		{
			lower_.z = v.z;
		}

		if (upper_.x < v.x) 
		{
			upper_.x = v.x;
		}

		if (upper_.y < v.y) 
		{
			upper_.y = v.y;
		}

		if (upper_.z < v.z) 
		{
			upper_.z = v.z;
		}

		return Processor::CONTINUE;
	}

	SimpleBox3 BoundingBoxProcessor::getBox() const
		
	{
		return SimpleBox3(lower_, upper_);
	}

	const Vector3& BoundingBoxProcessor::getLower() const
		
	{
		return lower_;
	}

	const Vector3& BoundingBoxProcessor::getUpper() const
		
	{
		return upper_;
	}


	//  GeometricCenterProcessor
	bool GeometricCenterProcessor::start()
		
	{
		center_.set(0, 0, 0);
		n_ = 0;

		return true;
	}

	bool GeometricCenterProcessor::finish()
		
	{
		if (n_ != 0)
		{
			center_ /= (float)n_;
		}

		return true;
	}

	Processor::Result GeometricCenterProcessor::operator()(const Vector3& v)
		
	{
		center_ += v;
		n_++;

		return Processor::CONTINUE;
	}

	Vector3& GeometricCenterProcessor::getCenter()
		
	{
		return center_;
	}


	// ---------- FragmentDistanceCollector -----------------

	// default constructor
	FragmentDistanceCollector::FragmentDistanceCollector() 
		
		:	reference_composite_(0)
	{
	}

	FragmentDistanceCollector::FragmentDistanceCollector(const Composite& composite) 
		
		:	reference_composite_(&composite),
			squared_distance_(0)
	{
	}

	FragmentDistanceCollector::FragmentDistanceCollector(const Composite& composite,float distance) 
		
		:	reference_composite_(&composite),
			squared_distance_(distance * distance)
	{
	}

	bool FragmentDistanceCollector::start()
		
	{
		// clear the array containing the collected fragments
		fragments.clear();

		// if no reference fragment is set, return immediately
		return reference_composite_ != 0;
	}

	float FragmentDistanceCollector::getDistance() const
		
	{
		return sqrt(squared_distance_);
	}

	void FragmentDistanceCollector::setDistance(float distance)
		
	{
		squared_distance_ = distance * distance;
	}

	bool FragmentDistanceCollector::finish()
		
	{
		bool                              collect_it = false;
		AtomConstIterator                 atom_iterator2;
		const Fragment*                   mol_fragment;
		Composite::CompositeConstIterator composite_it;
		vector<const Atom*>               reference_atoms;
		const Atom*                       atom_ptr;
		GeometricCenterProcessor          center_processor;
		Vector3                           center;
		float                             fragment_radius;
		float                             difference;

		Size                              i, j;
		Size                              size;


		for (composite_it = reference_composite_->beginComposite(); 
				 composite_it != reference_composite_->endComposite(); ++composite_it) 
		{
            if (RTTI::isKindOf<Atom>(&*composite_it))
			{
				atom_ptr = RTTI::castTo<Atom>(*composite_it);
				reference_atoms.push_back(atom_ptr);	
			}
		}

		Size    number_of_atoms = 0;
		Vector3 atom_position;
		float   distance = sqrt(squared_distance_);

		number_of_atoms = (Size)reference_atoms.size();
		size = (Size)all_fragments_.size();

		for (i = 0; i < size; i++) 
		{
			mol_fragment = all_fragments_[i]; 
			mol_fragment->apply(center_processor);
			center = center_processor.getCenter();
			fragment_radius = 0;

			for (atom_iterator2 = mol_fragment->beginAtom(); +atom_iterator2; ++ atom_iterator2)
			{
				if ((difference = (*atom_iterator2).getPosition().getSquareDistance(center)) > fragment_radius)
				{
					fragment_radius = difference;        
				}
			}
			fragment_radius = sqrt(fragment_radius);

			for (j = 0, collect_it = false; j < number_of_atoms && !collect_it; j++)
			{
				atom_position = reference_atoms[j]->getPosition();
				 
				if (atom_position.getDistance(center) <= (distance + fragment_radius)) 
				{
					for (atom_iterator2 = mol_fragment->beginAtom(); 
							 +atom_iterator2 && (!collect_it); ++ atom_iterator2)
					{
						if ((*atom_iterator2).getPosition().getSquareDistance(atom_position) < squared_distance_) 
						{
							fragments.push_back(mol_fragment);
							collect_it = true;
						}
					}
				}
			}
		}

	 all_fragments_.clear();
	 return true;
	}

	Processor::Result FragmentDistanceCollector::operator()(const Composite& composite)
		
	{
		const Fragment* mol_fragment = dynamic_cast<const Fragment*>(&composite);
		if (mol_fragment)
		{
			all_fragments_.push_back(mol_fragment);
		}

		return Processor::CONTINUE;
	}

	Size FragmentDistanceCollector::getNumberOfFragments()
		
	{
		return (Size)fragments.size();
	}

	void FragmentDistanceCollector::setComposite(const Composite& composite)
		
	{
		reference_composite_ = &composite;
	}

	const Composite* FragmentDistanceCollector::getComposite() const
		
	{
		return reference_composite_;
	}

  // Calculate the torsion angle between four atoms
  Angle calculateTorsionAngle(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4)
		throw(Exception::IllegalPosition)
	{
		Vector3 a12(a2.getPosition() - a1.getPosition());
Vector3 a23(a3.getPosition() - a2.getPosition());
		Vector3 a34(a4.getPosition() - a3.getPosition());

		Vector3 n12(a12 % a23);
		Vector3 n34(a23 % a34);

		if (n12 == Vector3::getZero() ||
				n34 == Vector3::getZero())
		{
			throw(Exception::IllegalPosition(__FILE__, __LINE__, 0, 0, 0));
		}

		n12.normalize();
		n34.normalize();

		Vector3 cross_n12_n34(n12 % n34);
		float direction = cross_n12_n34 * a23;
		float scalar_product = n12 * n34;

		if (scalar_product > 1.0)
		{
			scalar_product = 1.0;
		}
		if (scalar_product < -1.0)
		{
			scalar_product = -1.0;
		}
		Angle a(acos(scalar_product));

		if (direction < 0.0)
		{
			a = -1.0 * (float)a;
		}

		return a;
	}

	bool setTorsionAngle(const Atom& a1, const Atom& a2, Atom& a3, const Atom& a4, Angle angle)
	{
		//We first need to determine the part of the molecule which needs to be
		//rotated. this will be done by a simple BFS.
		std::deque<Atom*> bfs_queue;

		//This set containes the atoms which should be rotated
		std::set<Atom*> component;

		//The starting point needs to be handled explicitly, as
		//we may not consider a2
		component.insert(&a3);

		for(unsigned int i = 0; i < a3.countBonds(); ++i)
		{
			if(a3.getPartnerAtom(i) != &a2)
			{
				component.insert(a3.getPartnerAtom(i));
				bfs_queue.push_back(a3.getPartnerAtom(i));
			}
		}

		//Perform the BFS
		while(!bfs_queue.empty())
		{
			Atom* atom = bfs_queue.front();
			bfs_queue.pop_front();

			for(unsigned int i = 0; i < atom->countBonds(); ++i)
			{
				//If a2 is in the same connected component as a3, we cannot
				//set the torision angle
				if(atom->getPartnerAtom(i) == &a2)
				{
					return false;
				}

				if(component.find(atom->getPartnerAtom(i)) == component.end())
				{
					component.insert(atom->getPartnerAtom(i));
					bfs_queue.push_back(atom->getPartnerAtom(i));
				}
			}
		}

		//Now compute the current torsion angle and compute the residual rotation
		angle -= calculateTorsionAngle(a1, a2, a3, a4);

		//Setup the rotation. We first need to make a3 the origin
		//of the selected component, then apply the rotation and
		//subsequently translate a3 to its original position.
		Matrix4x4 rotation;
		rotation.rotate(angle, a3.getPosition() - a2.getPosition());
		Matrix4x4 trans;
		trans.setTranslation(-a3.getPosition());
		rotation *= trans;
		trans.setTranslation(a3.getPosition());
		rotation = trans * rotation;


		for(std::set<Atom*>::iterator it = component.begin(); it != component.end(); ++it)
		{
			(*it)->setPosition(rotation * (*it)->getPosition());
		}

		return true;
	}

  // Calculate the bond angle between three atoms
  Angle calculateBondAngle(const Atom& a1, const Atom& a2, const Atom& a3)
		throw(Exception::IllegalPosition)
	{
		// two atoms can't have the same position
		if (a1.getPosition() == a2.getPosition() ||
				a3.getPosition() == a2.getPosition())
		{
			throw(Exception::IllegalPosition(__FILE__, __LINE__, a2.getPosition().x,
a2.getPosition().y, a2.getPosition().z));
		}

		Vector3 a12(a1.getPosition() - a2.getPosition());
		Vector3 a23(a3.getPosition() - a2.getPosition());

		a12.normalize();
		a23.normalize();

		float scalar_product = a12 * a23;
		if (scalar_product > 1.0)
		{
			scalar_product = 1.0;
		}
		if (scalar_product < -1.0)
		{
			scalar_product = -1.0;
		}
		Angle a(acos(scalar_product));

		return a;
	}

} // namespace BALL
