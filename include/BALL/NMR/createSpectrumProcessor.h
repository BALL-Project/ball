#define CREATE_SPECTRUM_PROCESSOR

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

#ifndef BALL_NMR_PEAK
#include<BALL/NMR/peak.h>
#endif

#include <list>
using std::list;

namespace BALL {
/**@name	CreateSpectrumProcessor
*/

//@{

/**	Processor creates {\tt peaklist\_}. Each atom큦 chemical shift will create a new peak.
	thist peak큦 {\tt atomlist\_} will contain the corresponding atom.
*/

class CreateSpectrumProcessor:public UnaryProcessor<Object>
	{
	private:
	PDBAtom* patom_;
	list<peak> peaklist_;
	float *wert_;
	float raster_;
	
	public:

	/**@name	Constructors and Destructors
	*/
	
	//@{

	/**	Default Constructor
	*/	
	CreateSpectrumProcessor();
	
	/**	Destructor
	*/
	virtual ~CreateSpectrumProcessor();
	
	//@}

	/**@name	class special functions
	*/

	//@{

	/**	start-function
		nothing is done here
	*/
	virtual bool start();
	
	/**	finish-function
		noting is done here
	*/
	virtual bool finish();
	
	/**	operator-function
		if object is kind of PDBAtom a new peak is created ,
		peak큦 parameters are set and the atom is added to the peak큦
		{\tt atomlist\_}
	*/
	virtual Processor::Result operator()(Object&);
	

	/**	returns a pointer to {\tt peaklist\_}
	*/
	list<peak>* get_peaklist();
	
	/**	returns {\tt raster\_}, actually unused
	*/
	float get_raster();
	
	/** 	sets {\tt raster\_}, actually unused
	*/
	void set_raster(float);

	//@}
	};

//@}

} //namespace Ball
	 
