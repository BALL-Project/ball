// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.C,v 1.7 2004/10/22 20:44:14 amoll Exp $

#include <BALL/VIEW/KERNEL/primitiveManager.h>

namespace BALL
{
	namespace VIEW
	{

PrimitiveManager::PrimitiveManager()
	throw()
	:Object()
{
}

PrimitiveManager::~PrimitiveManager()
	throw()
{
	clear();
}

PrimitiveManager::PrimitiveManager(const PrimitiveManager& pm)
	throw()
	: Object(pm)
{
	*this = pm;
}

void PrimitiveManager::clear()
	throw()
{
	// call clear for all stored representations to clear also their geometric objects
	RepresentationsIterator it = begin();
	for (; it != end(); it++)
	{
		(*it)->clear();
	}
	representations_.clear();
}


void PrimitiveManager::insert(Representation& representation)
	throw()
{
	if (has(representation)) return;
	representations_.push_back(&representation);
}


bool PrimitiveManager::has(const Representation& representation) const
	throw()
{
	RepresentationsConstIterator it = begin();
	for (; it != end(); it++)
	{
		if (*it == &representation) return true;
	}
	return false;
}


void PrimitiveManager::remove(Representation& representation)
	throw()
{
	bool found = false;
	RepresentationsIterator it = begin();
	for( ; it != end(); it++)
	{
		if (*it == &representation)
		{
			found = true;
			break;
		}
	}

	if (!found) return;

	representations_.erase(it);
	delete &representation;
}

void PrimitiveManager::dump(std::ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);

	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	BALL_DUMP_DEPTH(s, depth);

	s << "number of representations: " << representations_.size() << std::endl;

	RepresentationsConstIterator it = begin();
	for (; it != end(); it++)
	{
		(*it)->dump(s, depth +1);
		s << std::endl;
	}

	BALL_DUMP_STREAM_SUFFIX(s);     
}

Representation* PrimitiveManager::createRepresentation()
	throw()
{
	Representation* rp = new Representation;
	insert(*rp);
	return rp;
}

const PrimitiveManager& PrimitiveManager::operator = (const PrimitiveManager& pm)
	throw()
{
	RepresentationsConstIterator it = pm.begin();

	for (; it != pm.end(); it++)
	{
		Representation* rp = new Representation(**it);
		representations_.push_back(rp);
	}

	return *this;
}

bool PrimitiveManager::operator == (const PrimitiveManager& pm) const
	throw()
{
	if (pm.getNumberOfRepresentations() != getNumberOfRepresentations()) return false;

	RepresentationsConstIterator it1 = begin();
	RepresentationsConstIterator it2 = pm.begin();
	for (; it1 != end() && it2 != pm.end(); it1++)
	{
		if (**it1 != **it2) return false;
		it2++;
	}

	return true;
}


List<Representation*> PrimitiveManager::removedComposite(const Composite& composite)
	throw()
{
	List<Representation*> removed_representations;
	RepresentationsIterator rep_it = begin();
	for (; rep_it != end(); rep_it++)
	{
		Representation::CompositesConstIterator composite_it = (*rep_it)->begin();
		for(; composite_it != (*rep_it)->end(); composite_it++)
		{
			if (&composite == *composite_it ||
					composite.isAncestorOf(**composite_it))
			{
				removed_representations.push_back(*rep_it);
				break;
			}
		}
	}

	rep_it = removed_representations.begin();
	for (; rep_it != removed_representations.end(); rep_it++)
	{
		remove(**rep_it);
	}
	return removed_representations;
}

List<Representation*> PrimitiveManager::getRepresentationsOf(const Composite& composite)
	throw()
{
	List<Representation*> changed_representations;
	RepresentationsIterator rep_it = begin();
	for (; rep_it != end(); rep_it++)
	{
		Representation::CompositesConstIterator composite_it = (*rep_it)->begin();
		for(; composite_it != (*rep_it)->end(); composite_it++)
		{
			if (&composite == *composite_it ||
					composite.isRelatedWith(**composite_it)) 
			{
				changed_representations.push_back(*rep_it);
				break;
			}
		}
	}

	return changed_representations;
}

} } // namespaces
