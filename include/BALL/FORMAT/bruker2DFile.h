// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_BRUKER2DFILE_H
#define BALL_FORMAT_BRUKER2DFILE_H

#ifndef BALL_FORMAT_JCAMPFILE_H
# include <BALL/FORMAT/JCAMPFile.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
# include <BALL/DATATYPE/regularData2D.h>
#endif

namespace BALL
{
	/**	Bruker 2D spectrum format.
			A class for reading Bruker two-dimensional NMR spectra.
			 \par
			
    	\ingroup  NMRFileFormats
	*/
	class BALL_EXPORT Bruker2DFile
		: public File
	{
	 public:

		/** Default constructor.
		 */
		Bruker2DFile();

		/**	Constructor.
		 *	@param name important: name of the Bruker-*directory*
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		Bruker2DFile(const String& name, OpenMode open_mode = std::ios::in | std::ios::binary);

		/**	Destructor.
		*/
		virtual ~Bruker2DFile();

		/** Read a spectrum. 
				It will be stored in spectrum_;
		*/
		void read();

		/** Read a spectrum from "name". 
				It will be stored in spectrum_;
		*/
		void read(const String &name);

		/// Return a reference to the spectrum.
		const RegularData2D& getData() const { return spectrum_; }
		///
		RegularData2D& getData() { return spectrum_; }

		/**	Returns a list of peaks found in the spectrum.
		*/
		std::list<std::pair<int, int> > getPeakList() const;

		/** Returns the shift corresponding to a position in the bitmap.
		*/
		std::pair<double, double> getShift(Position x, Position y);

		/** Returns the coordinates of a point in the original data next to the given coordinates.
		*/
		std::pair<Position, Position> getPosition(double x, double y);

		/** Set the parameters needed for conversion index<->shift.
		*/
		void setShiftRange(double offsetf1, double offsetf2, double swidthf1, double swidthf2, 
											 double bfreqf1, double bfreqf2, double spointnumf1, double spointnumf2);

		private:
			const Bruker2DFile& operator = (const Bruker2DFile& file);

	  protected:
		/// These classes give access to the parameters used in aqcuiring the spectrum.
		JCAMPFile parsf1_;
		JCAMPFile parsf2_;

		Size minx_;
		Size maxx_;
		Size miny_;
		Size maxy_;

		// To be replaced by regularData2D
		RegularData2D dat_;
		double soffsetf1_;
		double soffsetf2_;
		double swidthf1_;
		double swidthf2_;
		double bfreqf1_;
		double bfreqf2_;
		Size spointnumf1_;
		Size spointnumf2_;
		RegularData2D spectrum_;
	};
}

#endif
