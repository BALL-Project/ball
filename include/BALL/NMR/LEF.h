// $Id: LEF.h,v 1.2 2000/07/03 21:08:49 oliver Exp $

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
		
/**@name	LEF
*/
//@{		

/**	Shift assignment processor implementing LEF.
*/



class LEFShift:public ShiftModule
	{
	public:
	
	/**@name	Constructors and Destructors
	*/
	//@{

	/**	Default constructor.
	*/
	LEFShift();
	
	/**	Destructor
	*/
	virtual ~LEFShift();
	
	//@}

	
	/**@name	Processor specific functions.
	*/
	//@{
	
	/**	Start method.
		nothing important is done yet.
	*/
	virtual bool start();
	
	/**	Finish method.
		Here shift calculation is done.
		There are four constant floats which contain the charges of atoms, named
		{\tt QC}, {\tt QN}, {\tt QO} and {\tt QH}.
		It iterates over every Hydrogen in {\tt proton\_list\_}.
		Its position is stored in {\tt proton} and the position of its bounded Atom
		is stored in {\tt bindung}. Their difference vector is calculated and stored in
		{\tt prot\_bin}.
		Next an iteration over every atom in {\tt atom\_list\_} is started.
		If the atom is in another residue then the proton is, the atom´s name is checked,
		and according to that {\tt ladung} is assigned whith the charge of the atom.
		The position of the atom is stored in {\tt atom}. Next the difference vector between
		{\tt proton} and {\tt atom} is calculated and stored in {\tt prot\_atom}.
		Then the scalarproduct of the two difference vectors {\tt prot\_bin} and
		{\tt prot\_atom} is calculated and stored in {\tt sc}.
		After that the Lengths of the vectors are stored in {\tt b\_prot\_bin} and {\tt b\_prot\_atom}.
		Next the cosinus of the angle between these vectors is calculated and stored in 
		{\tt theta}. Then {\tt Efact} is assigned the quotient of {\tt ladung} and the product of both
		vector´s lenghts. {\tt theta} * {\tt Efact} is added to float {\tt Ez}.
		Then the next iteration step starts.
		If {\tt atom\_list\_} has finished {\tt Ez} * {\tt sigmaE} ,which is another constant ,
		is substracted from the actual proton´s chemical shift.
		Then iteration goes on whith next proton of {\tt proton\_list\_}
		
		
	*/
	virtual bool finish();

	/**	Application method
		PDBAtoms are assigned to two different lists, named {\tt proton\_list\_}
		and {\tt atom\_list\_}. In {\tt proton\_list\_} every Hydrogen is stored except
		the ones called "H". That special Hydrogen is stored as well as PDBAtoms named
		"C", "N" and "O" in {\tt atom\_list\_}.
		
	*/
	virtual Processor::Result operator()(Object&);
	//@}
	

	private:
	
	// private Variablen :

	FDPB *pb_;
	list<PDBAtom*> proton_list_;	
	list<PDBAtom*> atom_list_;
	PDBAtom* patom_;
	System* system_;
	float shift_;
	// private Funktionen :
	
	
	
};

} // namespace Ball


//@}
