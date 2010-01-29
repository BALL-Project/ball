/* sortedList.h
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef SORTEDLIST
#define SORTEDLIST

#include <list>
#include <iterator>
#include <iostream>
#include <BALL/DATATYPE/string.h>

typedef unsigned int uint;


namespace BALL
{
	namespace QSAR
	{
		/** class for a list that is been kept sorted ascendingly at all times */
		template<typename X> class SortedList : public std::list<X>
		{
			public:	
				/** @name Type definitions
				 */
				//@{
				typedef typename std::list<X>::iterator Iterator;
				
				
				
				/** @name Constructors and Destructors
				 */
				//@{
				SortedList()
					: std::list<X>()
				{
					out_It = std::list<X>::begin();
					search_it = std::list<X>::begin();
					last_insertion=std::list<X>::end();
				};
				//@}
				
				
				
				/** @name Accessors
				 */
				//@{
				/** inserts id into the list at the correct position, i.e. it keeps the list sorted ascendingly. */
				Iterator insert(X id)
				{					
// 					Iterator tmp=search_it;
// 					tmp--;
// 					if((search_it==std::list<X>::end() || *search_it>=id) && (search_it==std::list<X>::begin() || *tmp<id))
// 					{
// 						std::list<X>::insert(search_it,id);
// 						last_insertion=--search_it;
// 					}
// 					else
// 					{
						Iterator it=std::list<X>::begin();
						while(*it<id && it!=std::list<X>::end())
						{
							it++;
						}
						std::list<X>::insert(it,id);
						last_insertion=--it;
					//}
					return last_insertion;
				};
				
				
				/** returns an iterator to the position found by the last call of SortedList.contains() */
				Iterator getFoundPosition()
				{
					return search_it;
				}
				
				
				/** inserts a value at the given position, after checking that this is the correct location */
				Iterator insert(Iterator it, X id)
				{
					Iterator tmp=it;
					tmp--;
					if((it==std::list<X>::end() || *it>=id) && (it==std::list<X>::begin() || *tmp<id))
					{
						std::list<X>::insert(it,id);
						last_insertion=--it;
					}
					else
					{
						insert(id);
					}
					return last_insertion;
				};
					
				
				/**  inserts a value at the end of the list, after checking that this is the correct location */
 				void push_back(X id)
 				{
					if(std::list<X>::back()<id)
					{
						std::list<X>::push_back(id);
					}
					else
					{
 						insert(id);
					}
 				};
				
				
				/** resets out_It to set first element of the list and return a reference to this element. */
				const X& front() 
				{
					out_It = std::list<X>::begin();
					return std::list<X>::front();
				};
					
				
				/** returns a reference to the next element of the list \n
				Run front() after filling the list and before trying to output it by using next() for the first time ! */
				const X& next() 
				{
					//if(out_It!=std::list<X>::end())
					//{
						const X& a = *out_It;
						out_It++;
						return a;
					//}
				};
				
				
				/** erase a given value from the list */
				void erase(X id)
				{
					Iterator it=std::list<X>::begin();
					while(*it<id && it!=std::list<X>::end())
					{
						it++;
					}
					if(*it==id) 
					{
						erase(it);
					}
				};
				
				
				/** erases the element at the given position and sets the given iterator to the next element of the list */
				void erase(Iterator& it)
				{
					if(it==std::list<X>::end()) return;
					bool reset_out_it=(out_It==it);
					bool reset_last_insertion=(last_insertion==it);
					bool reset_search_it=(search_it==it);
					
					Iterator tmp=it;
					tmp++;
					std::list<X>::erase(it);
					it=tmp;
					
					if(reset_out_it) out_It=it;
					if(reset_last_insertion) last_insertion=std::list<X>::end();
					if(reset_search_it) search_it=it;
				};
										
				
				/** removes the newest element, i.e. the last inserted element of this list.*/
				void deleteLastInsertion()
				{
					// last insertion has already been erased!
					if(last_insertion==std::list<X>::end()) return;
					
					if(!std::list<X>::empty())
					{
						erase(last_insertion);
					}
				};
		
				
				/** output the content of the list as a String */
				String toStr()
				{
					front();
					String out=""; 
					while(hasNext())
					{
						out+=String(next());
						out+="  ";
					}
					return out;
				}
						
						
				/** returns the last inserted value of this list. Use empty() to check whether this SortedList is empty before using this method */
				X getLastInsertedValue()
				{ 
					return *last_insertion;
				};
				//@}
				
				
				/** @name Predicates
				 */
				//@{
				bool hasNext() 
				{
					if(out_It==std::list<X>::end())
					{
						return 0;
					}
					else
					{
						return 1;
					}
				};
				
				
				/** checks whether id is contained in the list */
				bool contains(X id) 
				{
					if(std::list<X>::empty())
					{ 
						search_it=std::list<X>::begin();
						return false;
					}
					Iterator it=std::list<X>::begin();
					while(*it<id && it!=std::list<X>::end())
					{
						it++;
					}
					search_it=it;
					
					if(*it==id) {return true; }
					else { return false; }	
				};

				void clear()
				{
					std::list<X>::clear();
					last_insertion=std::list<X>::begin();
					out_It=std::list<X>::begin();
					search_it=std::list<X>::begin();
				};
				
				uint size()
				{
					return std::list<X>::size();
				};
				//@}
				
				
			private:
				/** @name Attributes
				 */
				//@{
				/** iterator pointing to the position of the last insertion. \n 
				This is used to be able to quickly remove the newest element from the list. */
				Iterator last_insertion;
				
				/** iterator used for output of the list. Each call of next() moves this iterator forward; front() resets it to the front of the list */
				Iterator out_It;
				
				/** iterator to the position found by the last call of SortedList.contains() */
				Iterator search_it;
				//@}
		};
		
	}
}

#endif // SORTEDLIST
