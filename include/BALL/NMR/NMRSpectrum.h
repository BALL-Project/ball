// $Id: NMRSpectrum.h,v 1.2 2000/07/03 21:08:49 oliver Exp $

#define NMR_SPECTRUM

#include<BALL/KERNEL/system.h>
#include<BALL/DATATYPE/string.h>
#include<BALL/DATATYPE/stringHashMap.h>
#include<BALL/FORMAT/PDBFile.h>

#include<BALL/NMR/names.h>

#ifndef CREATE_SPECTRUM_PROCESSOR
#	include<BALL/NMR/createSpectrumProcessor.h>
#endif

#ifndef SHIFT_MODULE
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_NMR_PEAK
#	include<BALL/NMR/peak.h>
#endif

#include <list>
using std::list;

using namespace std;

///////////////////////////////////////////////////////////////////////////

/* shift Module sind alle von Prozessoren abgeleitet
   CBallNMRSpectrum verwaltet eine Liste mit Prozessoren
   verbesserung : eine von Prozessor abgeleitete gemeinsame Basisklasse 
   		  der shift Module entwerfen und die Liste darauf definieren
   		  stellt sicher das nur shift module in der Liste abgelegt 
   		  werden koennen.

   Shift Module koennen ueber Strings definiert werden.
   neue Module erforden eine neue Zeile in insert_shift_module(CBallString)
   und dementsprechung eine neu compilierung. besser waere es die Neucompilierung
   auf das neue Modulzu beschraenken.
   
*/

///////////////////////////////////////////////////////////////////////////

namespace BALL {

/**@name	NMRSpectrum
*/

//@{

/**	
		realizing the NMRSpectrum Datastructure
		containing a pointer to a system named {\tt system\_} from which the spectrum will be
		calculated.
		the spectrum is realized with a list of peaks called {\tt spectrum\_}.
		a list of shift modules named {\tt processorlist\_} contains the modules which are used
		to do the calculation.
		{\tt names_} points to the names instance, which correlates the shiftmodules with them 
		stringnames.
		the CreateSpectrumProcessor {\tt create\_spectrum\_\ will create {\tt spectrum\_}
*/

class NMRSpectrum
	{
	private:

	list<peak> *spectrum_;
	list<ShiftModule* > processorlist_;
	System *system_;
	ShiftNames *names_;
	CreateSpectrumProcessor *create_spectrum_;
	int dichte_;	

	public:
	
	/**@name	Constructors and Destructors
	*/

	//@{

	/**	Default Constructor
	*/
	NMRSpectrum();
	
	/**	Destructor
	*/
	~NMRSpectrum();
	
	//@}

	/**@name 	class special functions
	*/

	//@{
	
	/**	sets {\tt system\_} to a system
	*/
	void set_system(System* s);

	/**	returns a pointer to the system set to {\tt system\_}
	*/
	System* get_system();

	/**	inserts a shiftmodule to {\tt processorlist\_} using {\tt names\_}
	*/
	void insert_shift_module(String module_name);
	
	/**	starts	every processor in {\tt processorlist\_}
	*/
	void calculate_shifts();
	
	/**	starts the CreateSpectrumProcessor which creates the peaklist {\tt spectrum\_}
		this list will be sortet after peak´s {\tt ppm\_}
	*/
	void create_spectrum();
	
	/** 	returns a pointer to {\tt spectrum\_}
	*/
	list<peak>* get_spectrum();	
	
	
	/**	returns the ppm of the lowest peak of {\tt spectrum\_}
	*/
	float get_spectrum_min();
	
	
	/**	returns the ppm of the highest peak of {\tt spectrum\_}
	*/
	float get_spectrum_max();
	
	/**	sets {\tt spectrum\_} to a peaklist
	*/
	void set_spectrum(list<peak>*);
	
	/**	explicitly sorts the peaklist {\tt spectrum\_}
	*/
	void sort_spectrum();

	/**	writes a file for gnuplot that represents the NMRSpectrum
	*/
	void plot_spectrum(String);
	
	/**	writes a file containing all peak´s {\tt ppm\_} value
	*/
	void plot_peaks(String);	
	
	/**	iterates through every atom of {\tt system\_} writing
		in file its full name and the chemical shift property
	*/
	void write_peaks(String);

	/**	sets the value of {\tt dichte\_} used for the plot_spectrum function
	*/
	void set_dichte(int);
	
	/**	returns the value of {\tt dichte\_}
	*/
	int get_dichte();	
	
	//@}

	/**@name 	friend functions
	*/

	//@{
	
	/** 	writes in file the difference of the two first files
		the float describes the tolerance
	*/
	friend void make_difference(float,String,String,String);

	/**	compares the chemical shifts of the atoms of the passed systems.
		atoms with identical full names are compared. The difference is set
		as occupancy of the atom of the system of the first NMRSpectrum.
		This changed system is written as new PDBFile (first String).
		Another file is written (second String) containing the atoms fullname
		its chemical shift and its difference to the shift of the second system
	*/
	friend void set_difference(NMRSpectrum*,NMRSpectrum*,String,String);

	//@}

	};

//@}
	
} // namespace Ball

