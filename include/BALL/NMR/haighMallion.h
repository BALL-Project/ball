// $Id: haighMallion.h,v 1.4 2000/09/08 07:09:36 oliver Exp $

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_CONCEPT_OBJECT_H
#	include <BALL/CONCEPT/object.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_PDBATOM_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#ifndef SHIFT_MODULE
#	include<BALL/NMR/shiftModule.h>
#endif

#include <list>
using std::list;

namespace BALL {
		
/**@name	Haigh Mallion Model
*/
//@{		

/**	Shift assignment processor implementing Haigh Mallion theory.
*/
class HaighMallionShift:public ShiftModule
	{
	public:
	
	/**@name	Constructors and Destructors
	*/
	//@{

	/**	Default constructor.
	*/
	HaighMallionShift();
	
	/**	Destructor
	*/
	virtual ~HaighMallionShift();
	
	//@}

	
	/**@name	Processor specific functions.
	*/
	//@{
	
	/**	Start method.
		nothing important is done yet.
	*/
	virtual bool start();
	
	/**	Finish method.
		here the work is done :
		the funcion iterates over all systems's protons by iterating through _proton_list.
		then for each proton every ring in _aromat_list is accessed.
		the actual ring's ringplane and it's area is calculated. 
		
		Iterating over every bond of the aromatic ring, triangle areas are calculated with:
		the bond's two atoms and the actual proton's projection onto the ringplane.
		It is important to state that these areas are signed areas, depending on wether there is 
		a right handed system between the plane's normal vector and the difference vectors from the projection
		to the two bond's atoms.
		Next the protons distances to the actual two bond's atoms are calculated and stored in a_eins and a_zwei.
		Then ts is calculated : ts = f * (1/a_eins^3 + 1/a_zwei) with f is the actual tringle area.
		For every bond of the ring this partial sum is calculated , added and stored in gs.
		Just some constant factor B and the chemical shift using Haigh Mallions Model is done.
		
		Important note:
		Distances are expressed in terms of the actual ring's diameter.
		Triangle areas are expressed in terms of the actual ring's area.
		
	*/
	virtual bool finish();

	/**	Application method
		works as a collector :
		it stores the systems aromatic rings in a list called _aromat_list and
		each proton in a list called _proton_list
	*/
	virtual Processor::Result operator () (Composite& atom);
	//@}
	

	private:
	
	// private Variablen :

	float n_,radius_;
	Vector3 mittelpunkt_,normal_;
	
	list<PDBAtom*> proton_list_;	
	list<Residue*> aromat_list_;
	String **asrings_;
	Residue* residue_;
	PDBAtom* patom_;
	float shift_;
	
	
	
	
};



//@}

}// namespace Ball

