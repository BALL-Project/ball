// $Id: NMRSpectrum.h,v 1.11 2001/03/02 22:19:33 amoll Exp $

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

namespace BALL 
{

	/**	Realizing the NMRSpectrum Datastructure
			containing a pointer to a system named {\tt system_} from which the spectrum will be
			calculated.	The spectrum is realized with a list of peaks called {\tt spectrum_}. \\
			{\bf Definition}\URL{BALL/NMR/NMRSpectrum.h}
	*/
	class NMRSpectrum
	{
		public:
		
		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default Constructor
		*/
		NMRSpectrum();
		
		/**	Destructor
		*/
		~NMRSpectrum();
		
		//@}
		/** @name 	class special functions
		*/
		//@{
		
		/**	Sets {\tt system_} to a system
		*/
		void setSystem(System* s);

		/**	Returns a pointer to the system.
		*/
		const System* getSystem() const;
		
		/**	Starts every processor in {\tt processorlist_}
		*/
		void calculateShifts();
		
		/**	Starts the CreateSpectrumProcessor which creates the peaklist {\tt spectrum_}.
				This list will be sortet after peak´s {\tt ppm_}.
		*/
		void createSpectrum();
		
		/** Returns a pointer to {\tt spectrum_}.
		*/
		const list<Peak1D>& getPeakList() const;	
		
		/**	Sets {\tt spectrum_} to a peaklist.
		*/
		void setPeakList(const list<Peak1D>& spectrum);
		
		/**	Returns the ppm of the lowest peak of {\tt spectrum_}.
		*/
		float getSpectrumMin() const;
		
		/**	Returns the ppm of the highest peak of {\tt spectrum_}.
		*/
		float getSpectrumMax() const;
		
		/**	Explicitly sorts the peaklist {\tt spectrum_}.
		*/
		void sortSpectrum();

		/**	Writes a file for gnuplot that represents the NMRSpectrum.
		*/
		void plotSpectrum(const String& filename) const;
		
		/**	Writes a file containing all peak´s {\tt ppm_} value.
		*/
		void plotPeaks(const String& filename) const;	
		
		/**	Iterates through every atom of {\tt system_} writing
				in a file its full name and the chemical shift property.
		*/
		void writePeaks(const String& filename) const;

		/**	Sets the value of {\tt density_} used for the plot_spectrum function.
		*/
		void setDensity(Size density);
		
		/**	Returns the value of {\tt density_}.
		*/
		Size getDensity() const;	

		/**	Set the shift model.
		*/
		void setShiftModel(const ShiftModel& model);
			
		/**	Return the current shift model.
		*/
		const ShiftModel& getShiftModel() const;

		CreateSpectrumProcessor& getCreateSpectrumProcessor() 
			throw();
			
		//@}
		/** @name 	friend functions
		*/
		//@{
		
		/** Writes in a file the difference of the two first files.
				The float describes the tolerance.
		*/
		friend void makeDifference(const float&, const String&, const String&, const String&);

		//@}

		protected:

		list<Peak1D>						spectrum_;
		System*									system_;
		ShiftModel							shift_model_;
		CreateSpectrumProcessor create_spectrum_;
		Size										density_;
		bool										is_sorted_;
	};

} // namespace BALL

#endif // BALL_NMR_NMRSPECTRUM_H
