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

/**	sets a property called chemical_shift to Hydrogens
*/
class AssignShiftProcessor:public UnaryProcessor<Object>
{
	public:
	
	/**@name	Constructors and Destructors
	*/
	//@{
	
	/**	Detailed constructor.
	*/
	AssignShiftProcessor(const vector<NMRAtomData*>& atomData);

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
	virtual Processor::Result operator()(Object&);
	//@}
	
	private:
	
	// private Variablen :
	
	StringHashMap<float> shiftTable_;
	PDBAtom *patom_;
	//String tableFileName_;
	const vector<NMRAtomData*>& atomData_;

	// private Funktionen :	

	AssignShiftProcessor();
};

//@}

} // namespace Ball

#endif // BALL_NMR_ASSIGNSHIFTPROCESSOR_H
