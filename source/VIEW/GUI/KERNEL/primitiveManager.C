// $Id: primitiveManager.C,v 1.1.2.1 2002/11/23 17:39:22 amoll Exp $

#include <BALL/VIEW/GUI/KERNEL/primitiveManager.h>

using std::endl;
using std::ostream;
using std::istream;

namespace BALL
{
	namespace VIEW
	{
		PrimitiveManager::PrimitiveManager()
			throw()
		{
		}		

		PrimitiveManager::~PrimitiveManager()
			throw()
		{
		}

		bool PrimitiveManager::hasComposite(const Composite* composite) const
			throw()		
		{
			return composite_to_objects_.has((Composite*) composite);
		}
				
		bool PrimitiveManager::hasGeometricObject(const GeometricObject* object) const
			throw()
		{			
			return object_set_.has((GeometricObject*)object);		
		}

		bool PrimitiveManager::insertGeometricObject(GeometricObject* object, Composite* composite)
			throw()		
		{		
			if (hasGeometricObject(object)) return false;
			
			object->setComposite(composite);
			object_set_.insert(object);
			
						
			if(!composite_to_objects_.has(composite))
			{
				// insert composite to hashmap
				composite_to_objects_[composite];

				// parent->childs schachteln
				Composite* parent = composite->getParent();
				
				List_it start_it;
				if (parent == 0 || ! hasComposite(parent))
				{
		   		start_it = objects_list_.insert(objects_list_.end(), object);
				}
				else
				{
					// insert childs geometric objects after parents objects
					start_it = objects_list_.insert(getSecondIterator(parent), object);

					// set new end iterator for parent
					It_pair parent_pair(getFirstIterator(parent), start_it);
					composite_to_objects_[parent] = parent_pair;
				}

				It_pair it_pair(start_it, start_it);			
				composite_to_objects_[composite] = it_pair;

				return true;				
			}


			// ok, composite exists already

			List_it list_it = objects_list_.insert(getSecondIterator(composite), object);			
			composite_to_objects_[composite] = It_pair(getFirstIterator(composite), list_it);
			return true;
		}		

		bool PrimitiveManager::applyToAllComposites(UnaryProcessor<Composite>& processor)
			throw()
		{
			bool ok =	true; 
			Map_it it = composite_to_objects_.begin();
			for(; +it; ++it)
			{
				ok = ok && (it->first)->apply(processor);
			}				

			return ok;			
		}			
				
		PrimitiveManager::List_const_it PrimitiveManager::getFirstIterator(Composite* composite) const
			throw()
		{
			return (List_const_it) composite_to_objects_[composite].first;
		}

		
		PrimitiveManager::List_const_it PrimitiveManager::getSecondIterator(Composite* composite) const
			throw()		
		{
			return composite_to_objects_[composite].second;
		}
					
		PrimitiveManager::List_it PrimitiveManager::getFirstIterator(Composite* composite)
			throw()
		{
			return composite_to_objects_[composite].first;
		}

		
		PrimitiveManager::List_it PrimitiveManager::getSecondIterator(Composite* composite)
			throw()		
		{
			return composite_to_objects_[composite].second;
		}
				
		void PrimitiveManager::dump(std::ostream& s = std::cout, Size depth = 0) const
			throw()
		{
		}

		void PrimitiveManager::read(std::istream& s)
			throw()
		{
		}

		void PrimitiveManager::write(std::ostream& s) const
			throw()
		{
		}

		void PrimitiveManager::destroy()
			throw()
		{
		}

		bool PrimitiveManager::isValid() const
			throw()
		{
			if (!object_set_.isValid() || composite_to_objects_.isValid()) return false;

			List_const_it it = objects_list_.begin();
			for (; it != objects_list_.end(); it++)
			{
				if (*it == 0) return false;
			}

			return true;
		}

		/*
		bool PrimitiveManager::applyToGeometricObjects(UnaryProcessor<GeometricObject>& processor, Composite* composite)
			throw()
		{
			if (!hasComposite(composite))
			{
				return false;
			}

			bool result = true;
			List_it it = getFirstIterator(composite);
			for (; it != getSecondIterator(composite); it++)
			{
				result &=(*it)->apply(processor);
			}

			return result;
		}

		bool PrimitiveManager::applyToAllGeometricObjects(UnaryProcessor<GeometricObject>& processor)
			throw()
		{
			bool result = true;
			List_it it = objects_list_.begin();
			for (; it != objects_list_.end(); it++)
			{
				result &=(*it)->apply(processor);
			}

			return result;
		}
*/
		Size PrimitiveManager::countGeometricObjects(Composite* composite) const
			throw()
		{
			if (!hasComposite(composite))
			{
				return 0;
			}

			Size nr = 1;
			List_const_it it = getFirstIterator(composite);
			for (; it != getSecondIterator(composite); it++)
			{
				nr++;
			}

			return nr;
		}

		Size PrimitiveManager::getNumberOfGeometricObjects() const
			throw()
		{
			return object_set_.size();
		}

//#ifdef BALL_NO_INLINE_FUNCTIONS
//#	include <BALL/VIEW/GUI/KERNEL/primitiveManager.iC>
//#	endif 

	} // namespace VIEW
} // namespace BALL

