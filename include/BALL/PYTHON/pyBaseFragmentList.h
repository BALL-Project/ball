// $Id: pyBaseFragmentList.h,v 1.1 2000/07/04 08:05:33 oliver Exp $

#ifndef BALL_PYTHON_PYBASEFRAGMENTLIST_H
#define BALL_PYTHON_PYBASEFRAGMENTLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class BaseFragment;
	
	/**
	*/
	class PyBaseFragmentList
		:	public List<BaseFragment*>
	{
		public:

		BALL_CREATE_NODEEP(PyBaseFragmentList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	BaseFragment* type
		*/
		typedef BaseFragment* ValueType;

		/**	Pointer type
		*/
		typedef BaseFragment** PointerType;

		/**	Iterator type.
		*/
		typedef List<BaseFragment*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<BaseFragment*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyBaseFragmentList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyBaseFragmentList(const PyBaseFragmentList& new_list);
			
		/**	Construct from a BaseFragment
				This constructor creates an PyBaseFragmentList object from
				all base fragments of a \Ref{BaseFragment} object.
		*/
		PyBaseFragmentList(const BaseFragment& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyBaseFragmentList();
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

#endif // BALL_PYTHON_PYBASEFRAGMENTLIST_H
