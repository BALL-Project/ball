// $Id: randomCoil.h,v 1.2 2000/07/03 21:08:51 oliver Exp $

#include<BALL/SOLVATION/poissonBoltzmann.h>
#include<BALL/COMMON/constants.h>
#include<BALL/KERNEL/system.h>
#include<BALL/KERNEL/atom.h>
#include<BALL/KERNEL/bond.h>
#include<BALL/KERNEL/residue.h>
#include<BALL/KERNEL/molecule.h>
#include<BALL/KERNEL/protein.h>
#include<BALL/KERNEL/residue.h>
#include<BALL/KERNEL/PDBAtom.h>
#include<BALL/DATATYPE/string.h>
#include<BALL/DATATYPE/stringHashMap.h>
#include<BALL/KERNEL/PTE.h>

#ifndef SHIFT_MODULE
#include<BALL/NMR/shiftModule.h>
#endif

#include <list>
using std::list;


namespace BALL {
		
/**@name	random Coil
*/
//@{		

/**	adding random coil shifts to Hydrogens
*/



class RandomCoilShift:public ShiftModule
	{
	public:
	
	/**@name	Constructors and Destructors
	*/
	//@{

	/**	Default constructor.
	*/
	RandomCoilShift();
	
	/**	Destructor
	*/
	virtual ~RandomCoilShift();
	
	//@}

	
	/**@name	Processor specific functions.
	*/
	//@{
	
	/**	Start method.
		A StringHashMap of floats is built and named {\tt rc\_table\_}
		A file called "random.dat" is opened ,it contains the randomcoilshift date for
		every residue´s atoms. The random coil shift is stored in {\tt rc\_table\_}
		under [residue_name:atom_name].

	*/
	virtual bool start();
	
	/**	Finish method.
		nothing is done here
		
	*/
	virtual bool finish();

	/**	Application method
		if current object is a PDBAtom of kind Hydrogen its random coil shift has to be looked up
		in {\tt rc\_table}.
		Get Hydrogens residue name append ":" and its name, the lookup entry is reconstruced.
		If this entry is found within the table the randomcoil shift is added to the Hydrogens
		chemical shift.
		If this entry is not found the random coil shift is set to 1000 and added as well to the
		Hydrogens chemical shift,to mark that Hydrogen not to have a random coil shift table entry.
 
		
		
	*/
	virtual Processor::Result operator()(Object&);
	//@}
	

	private:
	
	// private Variablen :

	StringHashMap<float> rc_table_;
	list<PDBAtom*> proton_list_;	
	PDBAtom* patom_;
	float shift_;
	char *file_;

	// private Funktionen :
	
	
	
};

} // namespace Ball


//@}
