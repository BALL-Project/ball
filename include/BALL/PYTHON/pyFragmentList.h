// $Id: pyFragmentList.h,v 1.1 2000/06/27 07:44:10 oliver Exp $

#ifndef BALL_PYTHON_PYFRAGMENTLIST_H
#define BALL_PYTHON_PYFRAGMENTLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{
	class Fragment;
	class BaseFragment;
	
	/**
	*/
	class PyFragmentList
		:	public List<Fragment*>
	{
		public:

		BALL_CREATE_NODEEP(PyFragmentList)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Fragment* type
		*/
		typedef Fragment* ValueType;

		/**	Pointer type
		*/
		typedef Fragment** PointerType;

		/**	Iterator type.
		*/
		typedef List<Fragment*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef List<Fragment*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty list.
		*/
		PyFragmentList();

		/** Copy constructor.
				Create a copy of an existing list.
				@param	map the list to be copied
				@param	deep ignored
		*/
		PyFragmentList(const PyFragmentList& new_list);
			
		/**	Construct from a BaseFragment
				This constructor creates an PyFragmentList object from
				all atoms of a \Ref{BaseFragment} object.
		*/
		PyFragmentList(const BaseFragment& fragment, bool selected_only = false);

		/**	Destructor
		*/
		virtual ~PyFragmentList();
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

#endif // BALL_PYTHON_PYFRAGMENTLIST_H
