// $Id: NMRSpectrum.h,v 1.4 2000/09/19 13:34:57 oliver Exp $

#ifndef BALL_NMR_NMRSPECTRUM_H
#define BALL_NMR_NMRSPECTRUM_H

#ifndef BALL_NMR_CREATESPECTRUMPROCESSOR_H
#	include<BALL/NMR/createSpectrumProcessor.h>
#endif

#ifndef BALL_NMR_SHIFTMODEL_H
#	include<BALL/NMR/shiftModel.h>
#endif

#ifndef BALL_NMR_PEAK_H
#	include<BALL/NMR/peak.h>
#endif

#include <list>


/* shift Module sind alle von Prozessoren abgeleitet
   NMRSpectrum verwaltet eine Liste mit Prozessoren
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

namespace BALL 
{

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
		void setSystem(System* s);

		/**	returns a pointer to the system set to {\tt system\_}
		*/
		const System* getSystem() const;

		/**	inserts a shiftmodule to {\tt processorlist\_} using {\tt names\_}
		*/
		void insertShiftModule(const String& module_name);
		
		/**	starts	every processor in {\tt processorlist\_}
		*/
		void calculateShifts();
		
		/**	starts the CreateSpectrumProcessor which creates the peaklist {\tt spectrum\_}
			this list will be sortet after peak´s {\tt ppm\_}
		*/
		void createSpectrum();
		
		/** 	returns a pointer to {\tt spectrum\_}
		*/
		const list<Peak1D>& getPeakList() const;	
		
		
		/**	returns the ppm of the lowest peak of {\tt spectrum\_}
		*/
		float getSpectrumMin() const;
		
		
		/**	returns the ppm of the highest peak of {\tt spectrum\_}
		*/
		float getSpectrumMax() const;
		
		/**	sets {\tt spectrum\_} to a peaklist
		*/
		void setPeakList(const list<Peak1D>& spectrum);
		
		/**	explicitly sorts the peaklist {\tt spectrum\_}
		*/
		void sortSpectrum();

		/**	writes a file for gnuplot that represents the NMRSpectrum
		*/
		void plotSpectrum(const String& filename) const;
		
		/**	writes a file containing all peak´s {\tt ppm\_} value
		*/
		void plotPeaks(const String& filename) const;	
		
		/**	iterates through every atom of {\tt system\_} writing
			in file its full name and the chemical shift property
		*/
		void writePeaks(const String& filename) const;

		/**	sets the value of {\tt dichte\_} used for the plot_spectrum function
		*/
		void setDensity(Size density);
		
		/**	returns the value of {\tt dichte\_}
		*/
		Size getDensity() const;	
		
		//@}

		/**@name 	friend functions
		*/

		//@{
		
		/** 	writes in file the difference of the two first files
			the float describes the tolerance
		*/
		friend void makeDifference(float,String,String,String);

		/**	compares the chemical shifts of the atoms of the passed systems.
			atoms with identical full names are compared. The difference is set
			as occupancy of the atom of the system of the first NMRSpectrum.
			This changed system is written as new PDBFile (first String).
			Another file is written (second String) containing the atoms fullname
			its chemical shift and its difference to the shift of the second system
		*/
		friend void setDifference(NMRSpectrum*,NMRSpectrum*,String,String);

		//@}

		protected:

		list<Peak1D> spectrum_;
		System* system_;
		ShiftModel	shift_model_;
		CreateSpectrumProcessor create_spectrum_;
		Size density_;
	};

//@}
	
} // namespace BALL

#endif // BALL_NMR_NMRSPECTRUM_H
