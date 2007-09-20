// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: assignShiftProcessor.h,v 1.24 2005/12/23 17:01:55 amoll Exp $
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

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#include <BALL/STRUCTURE/fragmentDB.h>
#endif

#include <vector>

namespace BALL 
{
	class FragmentDB;

	/**	Set a property called chemical_shift.
	 		You have to set the FragmentDB after you create an instance of this processor.
		\ingroup Spectra		
	*/
	class BALL_EXPORT AssignShiftProcessor
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
		AssignShiftProcessor(const vector<NMRStarFile::NMRAtomData>& atom_data);

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
		 *  It is assumed, that the residue sequence code of the NMRAtom-dataset
		 *  is equal to the residue sequence code of the applied AtomContainer.
		*/
		virtual Processor::Result operator () (Composite& composite);

		//@}
		/**	@name	Accessors
		*/
		//@{

		/// 
		void setFragmentDB(const FragmentDB& db);

		///
		const FragmentDB& getFragmentDB();

		/**	Return the state of the object
		*/
		bool isValid() const;

		//@}
		
		protected:
		
		StringHashMap<float> shift_table_;
		const std::vector<NMRStarFile::NMRAtomData>&		atom_data_;
		const Molecule*	molecule_;
		Position number_of_fragment_;
		FragmentDB fragment_db_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/NMR/assignShiftProcessor.iC>
#	endif
  
} // namespace BALL

#endif // BALL_NMR_ASSIGNSHIFTPROCESSOR_H
