// $Id: geometricProperties.C,v 1.9 2000/10/30 00:19:59 amoll Exp $

#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <stdio.h>
#include <math.h>
#include <values.h>

namespace BALL 
{

	bool BoundingBoxProcessor::start()
	{
		lower_.set(FLT_MAX, FLT_MAX, FLT_MAX);
		upper_.set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		return true;
	}

	bool BoundingBoxProcessor::finish()
	{
		if ((lower_.x == FLT_MAX) && (lower_.y == FLT_MAX) && (lower_.z == FLT_MAX)
				&& (upper_.x == -FLT_MAX) && (upper_.y == -FLT_MAX) && (upper_.z == -FLT_MAX))
		{
			lower_.set(0, 0, 0);
			upper_.set(0, 0, 0);

			return false;
		}
		return true;
	}


	Processor::Result BoundingBoxProcessor::operator()(Atom& atom)
	{

		if (lower_.x > atom.getPosition().x) 
		{
			lower_.x = atom.getPosition().x;
		}

		if (lower_.y > atom.getPosition().y) 
		{
			lower_.y = atom.getPosition().y;
		}

		if (lower_.z > atom.getPosition().z) 
		{
			lower_.z = atom.getPosition().z;
		}

		if (upper_.x < atom.getPosition().x) 
		{
			upper_.x = atom.getPosition().x;
		}

		if (upper_.y < atom.getPosition().y) 
		{
			upper_.y = atom.getPosition().y;
		}

		if (upper_.z < atom.getPosition().z) 
		{
			upper_.z = atom.getPosition().z;
		}

		return Processor::CONTINUE;
	}

	Box3 BoundingBoxProcessor::getBox() const
	{
		return Box3(lower_, upper_);
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

	Processor::Result GeometricCenterProcessor::operator()(Atom& atom)
	{
		center_ += atom.getPosition();
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
		if (reference_composite_ == 0)
		{
			return false;
		}

		// Go for it...
		return true;
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
		bool                                collect_it = false;
		AtomIterator                        atom_iterator2;
		Fragment*														mol_fragment;
		Composite::SubcompositeIterator     composite_it;
		vector<Atom*>												reference_atoms;
		Atom*                               atom_ptr;
		GeometricCenterProcessor            center_processor;
		Vector3                             center;
		float                               fragment_radius;
		float                               difference;

		Size                                i, j;
		Size																size;


		for (composite_it = reference_composite_->beginSubcomposite(); 
				 composite_it != reference_composite_->endSubcomposite(); ++composite_it) 
		{
			if (RTTI::isKindOf<Atom>(*composite_it))
			{
				atom_ptr = RTTI::castTo<Atom>(*composite_it);
				reference_atoms.push_back(atom_ptr);	
			}
		}

		Size    number_of_atoms = 0;
		Vector3 atom_position;
		float   distance = sqrt(squared_distance_);

		number_of_atoms = reference_atoms.size();
		size = all_fragments_.size();

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

			for ( j=0 , collect_it = false ; j < number_of_atoms && (collect_it == false) ; j++) 
			{
				atom_position = reference_atoms[j]->getPosition();
				 
				if (atom_position.getDistance(center) <= (distance + fragment_radius)) 
				{
					for (atom_iterator2 = mol_fragment->beginAtom(); 
							 +atom_iterator2 && (collect_it == false); ++ atom_iterator2)
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

	Processor::Result FragmentDistanceCollector::operator()(Composite& composite)
	{
		
		if (RTTI::isKindOf<Fragment>(composite))
		{
			Fragment*	mol_fragment = RTTI::castTo<Fragment>(composite);
			all_fragments_.push_back(mol_fragment);
		}

		return Processor::CONTINUE;
	}

	Size FragmentDistanceCollector::getNumberOfFragments()
	{
		return fragments.size();
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
	{
		Vector3 a12(a2.getPosition() - a1.getPosition());
		Vector3 a23(a3.getPosition() - a2.getPosition());
		Vector3 a34(a4.getPosition() - a3.getPosition());

		Vector3 n12(a12 % a23);
		Vector3 n34(a23 % a34);
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

  // Calculate the bond angle between three atoms
  Angle calculateBondAngle(const Atom& a1, const Atom& a2, const Atom& a3)
	{
		Vector3 a12(a2.getPosition() - a1.getPosition());
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
