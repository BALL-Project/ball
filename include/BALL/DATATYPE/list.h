// $Id: list.h,v 1.4 2000/07/07 04:20:42 amoll Exp $

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

		/**	Constant iterator type.
		*/
		typedef typename list<Value>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		List()
			: list<Value>()
		{
		}

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		List(const List& new_list, bool /* deep = true */)
			: list<Value>(new_list)
		{
		}
			
		/** Clear the list.
				Remove all contents from the list.
		*/
		void destroy()
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
    void set(const List& list, bool /* deep */ = true)
		{
			clear();

			const_iterator it = list.begin();
			for ( ; it != list.end(); ++it)
			{
				push_back(const_cast<Value&>(*it));
			}
		}

		/** Assign a list from another.
		*/
		List& operator = (const List& list)
		{
			set(list);
			return *this;
		}
			
		/// Assigns the content of a list to another
    void get(List& list, bool deep = true) const
		{
			list.set(*this, deep);
		}

		/// Swaps the contents of two lists
    void swap(List& list)
		{
		//	swap(*this, list);
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
			return size();
		}
		
		//@}

		/**	@name	Predicates */
		//@{

		/** Return true if the list is empty.
				This method return {\bf true} if the list does not contain any entries.
		*/
		bool isEmpty() const
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
		void host(Visitor<List<Value> >& visitor)
		{
			//BAUSTELLE
		}
  			
		//@}

		/**	@name	Internal Iterators */
		//@{

		/** Processor application method.
				Applies the processor to each entry of the list.
				@param processor the processor to be applied
		*/
		bool apply(UnaryProcessor<List<Value> >& processor)
		{
			// BAUSTELLE
		}
		//@}

		

	};

} // namespace BALL

#endif // BALL_DATATYPE_LIST_H
