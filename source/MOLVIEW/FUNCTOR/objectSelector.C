// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: objectSelector.C,v 1.5 2002/12/12 10:57:43 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/objectSelector.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>

using std::ostream;

namespace BALL
{
	namespace MOLVIEW
	{

		ObjectSelector::ObjectSelector()
			throw()
		{
		}

		ObjectSelector::~ObjectSelector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<ObjectSelector>() << std::endl;
			#endif 
		}

	Processor::Result ObjectDeselector::operator() (Composite &composite)
		throw()
	{
		if (RTTI::isKindOf<AtomContainer> (composite))
		{
			AtomIterator ai;
			AtomBondIterator bi;		
			BALL_FOREACH_BOND((*(AtomContainer*) &composite), ai, bi)
			{
				if (!bi->isSelected())
				for (Size i=0; i < bi->getDegree();i++)
				{
					bi->getChild(i)->deselect();
				}
			}

			BALL_FOREACH_ATOM((*(AtomContainer*) &composite), ai)
			{
				for (Size i=0; i < ai->getDegree();i++)
				{
					ai->getChild(i)->deselect();
				}
			}
		}

		if (RTTI::isKindOf<Atom> (composite))
		{
			for (Size i=0; i < composite.getDegree();i++)
			{
				composite.getChild(i)->deselect();
			}
		}
		return Processor::CONTINUE;
	}

	Processor::Result ObjectSelector::operator() (Composite &composite)
		throw()
	{
		if (RTTI::isKindOf<AtomContainer> (composite))
		{
			AtomIterator ai;
			AtomBondIterator bi;		
			BALL_FOREACH_BOND((*(AtomContainer*) &composite), ai, bi)
			{
				if (bi->isSelected())
				for (Size i=0; i < bi->getDegree();i++)
				{
					bi->getChild(i)->select();
				}
			}

			BALL_FOREACH_ATOM((*(AtomContainer*) &composite), ai)
			{
				for (Size i=0; i < ai->getDegree();i++)
				{
					ai->getChild(i)->select();
				}
			}
		}

		if (RTTI::isKindOf<Atom> (composite))
		{
			for (Size i=0; i < composite.getDegree();i++)
			{
				composite.getChild(i)->select();
			}
		}
		return Processor::CONTINUE;
	}

	} // namespace MOLVIEW
} // namespace BALL
