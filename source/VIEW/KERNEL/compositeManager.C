// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: compositeManager.C,v 1.6 2004/10/20 14:41:05 amoll Exp $
//

#include <BALL/VIEW/KERNEL/compositeManager.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atomContainer.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		CompositeManager::CompositeManager()
			throw()
		{
		}

		CompositeManager::CompositeManager(const CompositeManager& /*cm*/)
			throw()
		{
			// ???? copy ctor not implemented!
		}

		CompositeManager::~CompositeManager()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<CompositeManager>() << std::endl;
			#endif 

			clear();
		}

		void CompositeManager::clear()
			throw()
		{
			CompositeIterator it = begin();
			for(; it != end(); it++)
			{
				(*it)->destroy();
			}

			composite_set_.clear();
		}

	  bool CompositeManager::insert(Composite& composite)
			throw()
    {
			if (has(composite) || !composite.isRoot())
			{
				return false;
			}

			String name = ((AtomContainer*) &composite)->getName();

			if (name_set_.has(name))
			{
				for (Position p = 1; p < 2000; p++)
				{
					if (!name_set_.has(name + String(p)))
					{
						name = name + String(p);
						break;
					}
				}

				((AtomContainer*) &composite)->setName(name);
			}

			composite_set_.insert(&composite);
			name_set_.insert(name);
			return true;
		}

		bool CompositeManager::has(const Composite& composite) const
			throw()
		{
			if (composite_set_.has((Composite*) &composite)) return true;

			CompositeConstIterator it = begin();
			for(; it != end(); it++)
			{
				if ((*it)->isAncestorOf(composite)) return true;
			}

			return false;
		}

		bool CompositeManager::remove(Composite& composite) 
			throw()
		{
			if (!has(composite)) return false;

			if (composite_set_.has(&composite))
			{
				composite_set_.erase(&composite);
				name_set_.erase(((AtomContainer*) &composite)->getName());
			}

			composite.destroy();
			return true;
		}
				
		void CompositeManager::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "number of composites: " << composite_set_.size() << std::endl;
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
