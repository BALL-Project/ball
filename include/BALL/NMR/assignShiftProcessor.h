// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: assignShiftProcessor.h,v 1.22 2004/05/07 13:07:36 amoll Exp $
//

#ifndef BALL_NMR_ASSIGNSHIFTPROCESSOR_H
#define BALL_NMR_ASSIGNSHIFTPROCESSOR_H

#ifndef BALL_NMR_SHIFT_MODULE_H
#	include	<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_FORMAT_NMRSTARFILE_H
#	include <BALL/FORMAT/NMRStarFile.h>
#endif

#include <vector>

namespace BALL 
{             
	class FragmentDB;

	/**	Set a property called chemical_shift.
	 		You have to set the FragmentDB after you create an instance of this processor.
		\ingroup Spectra		
	*/
	class AssignShiftProcessor
		: public UnaryProcessor<Composite>
	{
		public:
		
		/** @name	Constructors and Destructors
		*/
		//@{

		BALL_CREATE(AssignShiftProcessor)

		/**	Default constructor
		*/
		AssignShiftProcessor();

		/**	Detailed constructor.
		*/
		AssignShiftProcessor(const vector<NMRAtomData>& atom_data);

		/**	Copy constructor.
		*/
		AssignShiftProcessor(const AssignShiftProcessor& processor);

		/**	Destructor
		*/
		virtual ~AssignShiftProcessor();
		
		//@}
		/** @name	Processor specific functions.
		*/
		//@{
		
		/**	Start method.
		*/
		virtual bool start();
		
		/**	Application method
		*/
		virtual Processor::Result operator () (Composite& composite);

		//@}
		/**	@name	Accessors
		*/
		//@{

		/// 
		void setFragmentDB(const FragmentDB* db) 
			throw();

		///
		const FragmentDB* getFragmentDB()
			throw();

		/**	Return the state of the object
		*/
		bool isValid() const
			throw();

		//@}
		
		protected:
		
		StringHashMap<float>							shift_table_;
		const std::vector<NMRAtomData>&		atom_data_;
		const Molecule*										molecule_;
		Position													number_of_fragment_;
		FragmentDB* 											fragment_db_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/NMR/assignShiftProcessor.iC>
#	endif
  
} // namespace BALL

#endif // BALL_NMR_ASSIGNSHIFTPROCESSOR_H
