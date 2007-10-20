// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyDatasetVector.h,v 1.1.4.1 2007/03/25 21:25:20 oliver Exp $
//

#ifndef BALL_PYTHON_PYDATASETVECTOR_H
#define BALL_PYTHON_PYDATASETVECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_DATASET_H
#	include <BALL/VIEW/DATATYPE/dataset.h>
#endif

namespace BALL 
{
	using namespace BALL::VIEW;

	/** Equivalent for a STL::Vector of Dataset Pointers in Python
			\ingroup PythonExtensions
	*/
	class PyDatasetVector
		:	public vector<Dataset*>
	{
		public:

		BALL_CREATE(PyDatasetVector)

		/**	@name	Type Definitions
		*/
		//@{

		/**	Dataset* type
		*/
		typedef Dataset* ValueType;

		/**	Pointer type
		*/
		typedef Dataset** PointerType;

		/**	Iterator type.
		*/
		typedef vector<Dataset*>::iterator Iterator;

		/**	Constant iterator type.
		*/
		typedef vector<Dataset*>::const_iterator ConstIterator;

		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor.
				Create an empty vetor.
		*/
		PyDatasetVector();

		/// Copy constructor.
		PyDatasetVector(const PyDatasetVector& dvector);

		/// Copy constructor.
		PyDatasetVector(const vector<Dataset*>& dvector);
		
		/// Constructor from a const Dataset pointer list
		PyDatasetVector& operator = (const vector<Dataset*>& dvector)	throw();
				
		///	Destructor
		virtual ~PyDatasetVector() throw();
		//@}
	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYDATASETVECTOR_H
