// $Id: NMRSpectrum.h,v 1.5 2000/09/22 13:18:14 amoll Exp $

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

	/**	Realizing the NMRSpectrum Datastructure
			containing a pointer to a system named {\tt system\_} from which the spectrum will be
			calculated.
			the spectrum is realized with a list of peaks called {\tt spectrum\_}.
			A list of shift modules named {\tt processorlist\_} contains the modules which are used
			to do the calculation.
			{\tt names_} points to the names instance, which correlates the shiftmodules with their 
			stringnames.
			The CreateSpectrumProcessor {\tt create\_spectrum\_\ will create {\tt spectrum\_}
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
		
		/**	Sets {\tt system\_} to a system
		*/
		void setSystem(System* s);

		/**	Returns a pointer to the system set to {\tt system\_}.
		*/
		const System* getSystem() const;

		/**	Inserts a shiftmodule to {\tt processorlist\_} using {\tt names\_}.
		*/
		void insertShiftModule(const String& module_name);
		
		/**	Starts every processor in {\tt processorlist\_}
		*/
		void calculateShifts();
		
		/**	Starts the CreateSpectrumProcessor which creates the peaklist {\tt spectrum\_}.
				This list will be sortet after peak´s {\tt ppm\_}.
		*/
		void createSpectrum();
		
		/** Returns a pointer to {\tt spectrum\_}.
		*/
		const list<Peak1D>& getPeakList() const;	
		
		/**	Returns the ppm of the lowest peak of {\tt spectrum\_}.
		*/
		float getSpectrumMin() const;
		
		/**	Returns the ppm of the highest peak of {\tt spectrum\_}.
		*/
		float getSpectrumMax() const;
		
		/**	Sets {\tt spectrum\_} to a peaklist.
		*/
		void setPeakList(const list<Peak1D>& spectrum);
		
		/**	Explicitly sorts the peaklist {\tt spectrum\_}.
		*/
		void sortSpectrum();

		/**	Writes a file for gnuplot that represents the NMRSpectrum.
		*/
		void plotSpectrum(const String& filename) const;
		
		/**	Writes a file containing all peak´s {\tt ppm\_} value.
		*/
		void plotPeaks(const String& filename) const;	
		
		/**	Iterates through every atom of {\tt system\_} writing
				in a file its full name and the chemical shift property.
		*/
		void writePeaks(const String& filename) const;

		/**	Sets the value of {\tt density\_} used for the plot_spectrum function.
		*/
		void setDensity(Size density);
		
		/**	Returns the value of {\tt density\_}.
		*/
		Size getDensity() const;	
		
		//@}

		/**@name 	friend functions
		*/

		//@{
		
		/** Writes in a file the difference of the two first files.
				The float describes the tolerance.
		*/
		friend void makeDifference(const float&, const String&, const String&, const String&);

		/**	Compares the chemical shifts of the atoms of the passed systems.
				Atoms with identical full names are compared. The difference is set
				as occupancy of the atom of the system of the first NMRSpectrum.
				This changed system is written as new PDBFile (first String).
				Another file is written (second String) containing the atoms fullname
				its chemical shift and its difference to the shift of the second system
		*/
		friend void setDifference(NMRSpectrum*, NMRSpectrum*, String, String);

		//@}

		protected:

		list<Peak1D>						spectrum_;
		System*									system_;
		ShiftModel							shift_model_;
		CreateSpectrumProcessor create_spectrum_;
		Size										density_;
		bool										is_sorted_;
	};

//@}
	
} // namespace BALL

#endif // BALL_NMR_NMRSPECTRUM_H
