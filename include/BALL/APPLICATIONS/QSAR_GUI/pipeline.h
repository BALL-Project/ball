#ifndef PIPELINE_H 
#define PIPELINE_H 

#include <list>
#include <map>
#include <iterator>

 
namespace BALL
{
	namespace VIEW
	{
		template<typename X> class Pipeline
		{
			public:	
							
				typedef typename std::list<X>::iterator iterator;
				
				iterator begin()
				{
					return list_.begin();
				};
				
				iterator end()
				{
					return list_.end();
				};
				
				void insert(X object)
				{
					list_.push_back(object);
					iterator it = list_.end();
					it--; // iterator to the last cell of the list
					map_.insert(make_pair(object,it));
				};
				
				iterator erase(X object)
				{
					map_iterator map_it = map_.find(object);
					if(map_it!=map_.end())
					{
						iterator list_it = map_it->second;
						map_.erase(map_it);
						return list_.erase(list_it);  // returns iterator to next field
					}
					return list_.end();
				};
				
				bool contains(X object)
				{
					if(map_.find(object)==map_.end())
					{
						return false;
					}
					return true;
				};
				
				
				iterator find(X object)
				{
					map_iterator map_it = map_.find(object);
					if(map_it==map_.end())
					{
						return list_.end();
					}
					return map_it->second;	
				};
				
				uint size()
				{
					return list_.size();
				};
				
				bool empty()
				{
					if(list_.size()==0) return true;
					return false;
				};
				
				void clear()
				{
					map_.clear();
					list_.clear();
				};
		
			
			private:
				/** all objects in the exact order that they were inserted into this Pipeline */
				std::list<X> list_;
				
				/** map holding all inserted object in order to enable fast searching */
				std::map<X,iterator> map_;
						
				typedef typename std::map<X,iterator>::iterator map_iterator;
		};
	}
}


#endif // PIPELINE_H 
