// $Id: pyChainList.h,v 1.1 2000/06/27 07:44:09 oliver Exp $

#ifndef BALL_PYTHON_PYCHAINLIST_H
#define BALL_PYTHON_PYCHAINLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class Chain;
	class BaseFragment;
	
	/**
	*/
	class PyChainList
		:	public List<Chain*>
	{
		public:

		BALL_CREATE_NODEEP(PyChainList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Chain* type
		*/
		typedef Chain* ValueType;

		/**	Pointer type
		*/
		typedef Chain** PointerType;

		/**	Iterator type.
		*/
		typedef List<Chain*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Chain*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyChainList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyChainList(const PyChainList& new_list);
			
		/**	Construct from a BaseFragment
				This constructor creates an PyChainList object from
				all atoms of a \Ref{BaseFragment} object.
		*/
		PyChainList(const BaseFragment& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyChainList();
		//@}

		/**	@name Assignment
		*/
		//@{
		/**
		*/
		void set(const BaseFragment& fragment, bool selected_only = false);
		//@}
	};

} // namespace BALL

#endif // BALL_PYTHON_PYCHAINLIST_H
