// $Id: list.h,v 1.11 2001/06/21 02:10:51 oliver Exp $

#ifndef BALL_DATATYPE_LIST_H
#define BALL_DATATYPE_LIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif


#include <list>

namespace BALL 
{
	
	using std::list;
	
	/** Extended list object.
		This object is an improved version of the STL list class
		{\bf Definition:} \URL{BALL/DATATYPE/list.h}
	 */

 	template <typename Value>
	class List
		:	public list<Value>
	{
		public:

		/**	@name	Type Definitions
		*/
		//@{

		/**	Iterator type.
		*/
		typedef typename list<Value>::iterator Iterator;
		// for STL compatibility
		typedef typename list<Value>::iterator iterator;

		/**	Constant iterator type.
		*/
		typedef typename list<Value>::const_iterator ConstIterator;
		// for STL compatibility
		typedef typename list<Value>::const_iterator const_iterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		BALL_CREATE_DEEP(List)

		/**	Default constructor.
				Create an empty list.
		*/
		List() throw()
			: list<Value>()
		{
		}

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		List(const List& new_list, bool /* deep = true */) throw()
			: list<Value>(new_list)
		{
		}
			
		/** Clear the list.
				Remove all contents from the list.
		*/
 		void destroy()  throw()
		{
			clear();
		}
	
		/** Destructor
		*/
		virtual ~List() throw()
		{
			clear();
		}

		//@}

		/**	@name	Assignment */
		//@{
			
		/** Assign a list from another.
				Create a copy of a list.
				@param	list	the map to be copied
				@param	deep ignored
		*/
    void set(const List& list, bool /* deep */ = true) throw()
		{
			clear();

			ConstIterator it = list.begin();
			for ( ; it != list.end(); ++it)
			{
				push_back(const_cast<Value&>(*it));
			}
		}

		/** Assign a list from another.
		*/
		const List& operator = (const List& list) throw()
		{
			set(list);
			return *this;
		}
			
		/// Assign the content of a list to another
    void get(List& list, bool deep = true) const  throw()
		{
			list.set(*this, deep);
		}

		/// Swaps the contents of two lists
    void swap(List& list) throw()
		{
			List<Value> temp;
			temp.set(*this);
			(*this).set(list);
			list.set(temp);
		}

		//@}

		/**	@name Accessors */
		//@{

		/** Return the size of the list.
			@return Size the size of the list
		*/
		Size getSize() const throw()
		{
			return size();
		}

		/** Remove an item from the list. The first item that matches 
			{\tt item} will be removed.
			@param 	item the item to be removed
			@return bool {\bf true} if the item was removed
		 */
		bool remove(const Value& item) throw()
		{
			Iterator it = begin();
			for (; it != end(); ++it)
			{
				if (*it == item)
				{
					erase(it);
					return true;
				}
			}
			return false;
		}
		
		//@}

		/**	@name	Predicates */
		//@{

		/** Return true if the list is empty.
				This method return {\bf true} if the list does not contain any entries.
		*/
		bool isEmpty() const throw()
		{
			return (size() == 0);
		}
		//@}

		/**	@name	Miscellaneous */
		//@{

		/**	Visitor host method.
				Lists may be visited.
				@param	visitor	the visitor
		*/
		virtual void host(Visitor<List<Value> >& visitor) 
			throw();
		//@}

		/**	@name	Internal Iterators */
		//@{

		/** Processor application method.
				Applies the processor to each entry of the list.
				@param processor the processor to be applied
		*/
		bool apply(UnaryProcessor<Value>& processor) throw()
		{
			if (processor.start() == false)
			{
				return false;
			}

			Processor::Result result;
			Iterator it = begin();
			for (; it != end(); ++it)
			{
				result = processor(*it);
				if (result <= Processor::BREAK)
				{
					return (result == Processor::BREAK);
				}
			}
			if (result == Processor::ABORT)
			{
				return false;
			}

			return processor.finish();
		}
		//@}

		/** Equality operator.
				Test if two instances have the same size and same items at the same positions.
		*/
		bool operator == (const List<Value>& list) const throw()
		{
			if (size() != list.size())
			{
				return false;
			}

			List<Value>::ConstIterator this_it = begin();
			List<Value>::ConstIterator list_it = list.begin();

			for (; this_it != end(); ++this_it)
			{
				if (!(*this_it == *list_it))
				{
					return false;
				}
				++list_it;
			}
			return true;	
		}

		/** Inequality operator.
				Test if two instances differ in at least one element.
		*/
		bool operator != (const List<Value>& list) const throw()
		{
			return !(*this == list);
		}

	};

	template <typename Value>
	void List<Value>::host(Visitor<List<Value> >& visitor) 
		throw()
	{
		visitor.visit(*this);
	}

} // namespace BALL

#endif // BALL_DATATYPE_LIST_H
