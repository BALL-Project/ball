// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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

#ifdef BALL_COMPILER_GXX || BALL_COMPILER_LLVM
#warning "This header file is deprecated and should not be used in new code! As a replacement for BALL::List the use of std::list is strongly suggested."
#endif

namespace BALL 
{
	/** Extended list object.
		This object is an improved version of the STL list class
		
    \ingroup  DatatypeMiscellaneous
	*/
 	template <typename Value>
	class List
		:	public std::list<Value>
	{
		public:

		/**	@name	Type Definitions
		*/
		//@{

		/**	Iterator type.
		*/
		typedef typename std::list<Value>::iterator Iterator;
		// for STL compatibility
		typedef typename std::list<Value>::iterator iterator;

		/**	Constant iterator type.
		*/
		typedef typename std::list<Value>::const_iterator ConstIterator;
		// for STL compatibility
		typedef typename std::list<Value>::const_iterator const_iterator;

		//@}
		/**	@name	Constructors and Destructors */
		//@{

		BALL_CREATE_DEEP(List)

		/**	Default constructor.
				Create an empty list.
		*/
		List()
			: std::list<Value>()
		{
		}

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		List(const List& new_list, bool /* deep = true */)
			: std::list<Value>(new_list)
		{
		}
			
		/** Clear the list
		*/
 		void destroy()
		{
			std::list<Value>::clear();
		}
	
		/** Destructor
		*/
		virtual ~List()
		{
			std::list<Value>::clear();
		}

		//@}
		/**	@name	Assignment */
		//@{
			
		/** Assign a list from another.
				@param	list	the map to be copied
				@param	deep ignored
		*/
    void set(const List& list, bool /* deep */ = true)
		{
			std::list<Value>::clear();

			ConstIterator it = list.begin();
			for ( ; it != list.end(); ++it)
			{
				std::list<Value>::push_back(const_cast<Value&>(*it));
			}
		}

		/** Assign a list from another.
		*/
		const List& operator = (const List& list)
		{
			set(list);
			return *this;
		}
			
		/// Assign the content of a list to another
    void get(List& list, bool deep = true) const 
		{
			list.set(*this, deep);
		}

		/// Swaps the contents of two lists
    void swap(List& list)
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
		*/
		Size getSize() const
		{
			return (Size)std::list<Value>::size();
		}

		/** Remove an item from the list. The first item that matches <tt>item</tt> will be removed.
				@param 	item the item to be removed
				@return bool <b>true</b> if the item was removed
		 */
		bool remove(const Value& item)
		{
			Iterator it = std::list<Value>::begin();
			for (; it != std::list<Value>::end(); ++it)
			{
				if (*it == item)
				{
					std::list<Value>::erase(it);
					return true;
				}
			}
			return false;
		}
		
		//@}
		/**	@name	Predicates */
		//@{

		/** Return true if the list is empty.
				This method return <b>true</b> if the list does not contain any entries.
		*/
		bool isEmpty() const
		{
			return (std::list<Value>::size() == 0);
		}

		//@}
		/**	@name	DatatypeMiscellaneous */
		//@{

		/**	Visitor host method.
				Lists may be visited.
				@param	visitor	the visitor
		*/
		virtual void host(Visitor<List<Value> >& visitor);

		//@}
		/**	@name	Internal Iterators */
		//@{

		/** Processor application method.
				Applies the processor to each entry of the list.
				@param processor the processor to be applied
		*/
		bool apply(UnaryProcessor<Value>& processor)
		{
			if (!processor.start()) return false;

			for (Iterator it = std::list<Value>::begin(); it != std::list<Value>::end(); ++it)
			{
				Processor::Result result = processor(*it);
				if (result <= Processor::BREAK)
				{
					return (result == Processor::BREAK);
				}
			}

			return processor.finish();
		}

		//@}

		/** Equality operator.
				Test if two instances have the same size and same items at the same positions.
		*/
		bool operator == (const List<Value>& list) const
		{
			if (std::list<Value>::size() != list.size())
			{
				return false;
			}

			typename List<Value>::ConstIterator this_it = std::list<Value>::begin();
			typename List<Value>::ConstIterator list_it = list.begin();

			for (; this_it != std::list<Value>::end(); ++this_it)
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
		bool operator != (const List<Value>& list) const
		{
			return !(*this == list);
		}

	};

	template <typename Value>
	void List<Value>::host(Visitor<List<Value> >& visitor)
	{
		visitor.visit(*this);
	}
} // namespace BALL

#endif // BALL_DATATYPE_LIST_H
