// $Id: geometricProperties.C,v 1.1 1999/08/26 08:02:38 oliver Exp $

#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <stdio.h>
#include <math.h>
#include <values.h>

namespace BALL {

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
			lower_.x = atom.getPosition().x;

		if (lower_.y > atom.getPosition().y) 
			lower_.y = atom.getPosition().y;

		if (lower_.z > atom.getPosition().z) 
				lower_.z = atom.getPosition().z;

		if (upper_.x < atom.getPosition().x) 
			upper_.x = atom.getPosition().x;

		if (upper_.y < atom.getPosition().y) 
			upper_.y = atom.getPosition().y;

		if (upper_.z < atom.getPosition().z) 
			upper_.z = atom.getPosition().z;

		return Processor::CONTINUE;
	}


	Vector3& BoundingBoxProcessor::getLower(void)
	{
		return lower_;
	}

	Vector3& BoundingBoxProcessor::getUpper(void)
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
		center_ /= (float)n_;

		return true;
	}

	Processor::Result GeometricCenterProcessor::operator()(Atom& atom)
	{
		center_ += atom.getPosition();
		n_++;

		return Processor::CONTINUE;
	}

	Vector3& GeometricCenterProcessor::getCenter(void)
	{
		return center_;
	}


	// ---------- FragmentDistanceCollector -----------------

	// default constructor
	FragmentDistanceCollector::FragmentDistanceCollector(void) 
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


	bool FragmentDistanceCollector::start(void)
	{
		// clear the array containing the collected fragments
		fragments.clear();

		// if no reference fragment is set, return immediately
		if (reference_composite_ == 0)
			return false;

		// Go for it...
		return true;
	}

	float FragmentDistanceCollector::getDistance(void) const
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

		Size                                i,j;
		Size																size;


		for (composite_it = reference_composite_->beginSubcomposite(); composite_it != reference_composite_->endSubcomposite(); ++composite_it) 
		{
			if (RTTI<Atom>::isKindOf(*composite_it))
			{
				atom_ptr = RTTI<Atom>::castTo(*composite_it);
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
				if ((difference = (*atom_iterator2).getPosition().getSquareDistance(center)) > fragment_radius)
					fragment_radius = difference;        

			fragment_radius = sqrt(fragment_radius);

			for ( j=0 , collect_it = false ; j < number_of_atoms && (collect_it == false) ; j++) 
			{
				atom_position = reference_atoms[j]->getPosition();
				 
				if (atom_position.getDistance(center) <= (distance + fragment_radius)) 
					for (atom_iterator2 = mol_fragment->beginAtom(); +atom_iterator2 && (collect_it == false); ++ atom_iterator2)
						if ((*atom_iterator2).getPosition().getSquareDistance(atom_position) < squared_distance_) 
						{
							fragments.push_back(mol_fragment);
							collect_it = true;
						}
			}
		}

	 all_fragments_.clear();
	 return true;
	}

	Processor::Result FragmentDistanceCollector::operator()(Composite& composite)
	{
		
		if (RTTI<Fragment>::isKindOf(composite))
		{
			Fragment*	mol_fragment = RTTI<Fragment>::castTo(composite);
			all_fragments_.push_back(mol_fragment);
		}

		return Processor::CONTINUE;
	}

	Size FragmentDistanceCollector::getNumberOfFragments(void)
	{
		return fragments.size();
	}

	void FragmentDistanceCollector::setComposite(const Composite& composite)
	{
		reference_composite_ = &composite;
	}

	const Composite* FragmentDistanceCollector::getComposite(void) const
	{
		return reference_composite_;
	}

} // namespace BALL
