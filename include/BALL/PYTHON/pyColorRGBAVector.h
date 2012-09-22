// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyColorRGBAVector.h,v 1.2 2005/12/23 17:01:56 amoll Exp $
//

#ifndef BALL_PYTHON_PYCOLORRGBAVECTOR_H
#define BALL_PYTHON_PYCOLORRGBAVECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#include <vector>

namespace BALL 
{
	/** Equivalent for a STL::vector of ColorRGBA in Python
			\ingroup PythonExtensions
	*/
	class PyColorRGBAVector
		:	public std::vector<VIEW::ColorRGBA>
	{
		public:

		BALL_CREATE(PyColorRGBAVector)

		/**	@name	Type Definitions
		*/
		//@{

		/**	ColorRGBA type
		*/
		typedef VIEW::ColorRGBA ValueType;

		/**	Pointer type
		*/
		typedef VIEW::ColorRGBA* PointerType;

		/**	Iterator type.
		*/
		typedef std::vector<VIEW::ColorRGBA>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef std::vector<VIEW::ColorRGBA>::const_iterator ConstIterator;

		//@}
		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
		*/
		PyColorRGBAVector();

		/// Copy constructor.
		PyColorRGBAVector(const PyColorRGBAVector& c_vector);

		/// Copy constructor.
		PyColorRGBAVector(const std::vector<VIEW::ColorRGBA>& c_vector);
			
		///	Destructor
		virtual ~PyColorRGBAVector() throw();
		//@}

	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYCOLORRGBAVECTOR_H
