// $Id: assignShiftProcessor.h,v 1.3 2000/09/16 07:43:16 oliver Exp $

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
                
	/**	Set a property called chemical_shift.
	*/
	class AssignShiftProcessor
		: public UnaryProcessor<Composite>
	{
		public:
		
		/**@name	Constructors and Destructors
		*/
		//@{

		BALL_CREATE(AssignShiftProcessor)

		/**	Default constructor
		*/
		AssignShiftProcessor();

		/**	Detailed constructor.
		*/
		AssignShiftProcessor(const vector<NMRAtomData*>& atomData);

		/**	Copy constructor.
		*/
		AssignShiftProcessor(const AssignShiftProcessor& processor);

		/**	Destructor
		*/
		virtual ~AssignShiftProcessor();
		
		//@}

		
		/**@name	Processor specific functions.
		*/
		//@{
		
		/**	Start method.
		*/
		virtual bool start();
		
		/**	Finish method.  
		*/
		virtual bool finish();

		/**	Application method
		*/
		virtual Processor::Result operator () (Composite& composite);
		//@}

		/**	@name	Accessors
		*/
		//@{
		/**	Return the state of the object
		*/
		bool isValid() const;
		//@}
		
		protected:
		
		StringHashMap<float>							shift_table_;
		const std::vector<NMRAtomData*>&	atom_data_;
		bool															valid_;
	};

	//@}

} // namespace BALL

#endif // BALL_NMR_ASSIGNSHIFTPROCESSOR_H
