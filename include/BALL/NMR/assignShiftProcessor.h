#ifndef BALL_NMR_ASSIGNSHIFTPROCESSOR_H
#define BALL_NMR_ASSIGNSHIFTPROCESSOR_H

#include<BALL/KERNEL/PDBAtom.h>
#include<BALL/DATATYPE/string.h>
#include<BALL/DATATYPE/stringHashMap.h>
#include<BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/NMRStarFile.h>

#ifndef SHIFT_MODULE
#include<BALL/NMR/shiftModule.h>
#endif

namespace BALL 
{
                
	/**@name	AssignShiftProcessor
	*/
	//@{            

	/**	Sets a property called chemical_shift to hydrogens
	*/
	class AssignShiftProcessor
		: public UnaryProcessor<Composite>
	{
		public:
		
		/**@name	Constructors and Destructors
		*/
		//@{

		BALL_CREATE(AssignShiftProcessor)

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
		virtual Processor::Result operator()(Composite&);
		//@}
		
		private:
		
		StringHashMap<float> shift_table_;
		const vector<NMRAtomData*>& atom_data_;
		bool valid_;

		AssignShiftProcessor();
	};

	//@}

} // namespace Ball

#endif // BALL_NMR_ASSIGNSHIFTPROCESSOR_H
