// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
		{
		}

		CompositeManager::CompositeManager(const CompositeManager& /*cm*/)
		{
			// ???? copy ctor not implemented!
		}

		CompositeManager::~CompositeManager()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<CompositeManager>() << std::endl;
			#endif 

			clear();
		}

		void CompositeManager::clear()
		{
			CompositeIterator it = begin();
			for(; it != end(); it++)
			{
				(*it)->destroy();
			}

			composite_set_.clear();
		}

	  bool CompositeManager::insert(Composite& composite)
    {
			if (!composite.isRoot() || hasRoot(&composite))
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

		bool CompositeManager::hasRoot(const Composite* composite) const
		{
			return composite_set_.has((Composite*) &composite);
		}


		bool CompositeManager::has(const Composite* composite) const
		{
			if (hasRoot(composite)) return true;

			CompositeConstIterator cit = begin();
			for(; cit != end(); cit++)
			{
				Composite::CompositeConstIterator sub_it = composite->beginComposite();
				for (; +sub_it; ++sub_it)
				{
					if (composite == &*sub_it) return true;
				}
			}

			return false;
		}

		void CompositeManager::remove(Composite& composite, bool to_delete) 
		{
			HashSet<Composite*>::Iterator it = composite_set_.find(&composite);
			
			if (+it)
			{
				composite_set_.erase(*it);
				name_set_.erase(((AtomContainer*) &composite)->getName());
			}

			Composite* parent = composite.getParent();
			if (parent != 0)
			{
				parent->removeChild(composite);
			}

			if (to_delete) delete &composite;
		}
				
		void CompositeManager::dump(ostream& s, Size depth) const
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
