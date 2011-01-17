// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_BRUKER1DFILE_H
#define BALL_FORMAT_BRUKER1DFILE_H

#ifndef BALL_FORMAT_JCAMPFILE_H
# include <BALL/FORMAT/JCAMPFile.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
# include <BALL/DATATYPE/regularData1D.h>
#endif

namespace BALL
{
	/**	Bruker 1D spectrum format.
			A class for handling Bruker one-dimensional NMR spectra.
    	\ingroup  NMRFileFormats
	*/
	class BALL_EXPORT Bruker1DFile 
		: public File
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Constructor
		*/
    Bruker1DFile();

		/**	Constructor
		 *	@param name important: name of the Bruker-*directory*
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		Bruker1DFile(const String& name, OpenMode open_mode = std::ios::in | std::ios::binary);

		/// Destructor
		virtual ~Bruker1DFile();

		//@}
		/**	@name Accessors
		*/
		//@{

		/**	Read a spectrum from <tt>name</tt>. 
				It will be stored in spectrum_
		*/
		void read(const String& name);

		///
		void read();

	  /** Return a pointer to the spectrum.
		 */
		const RegularData1D& getData() const { return spectrum_; }

		///
		const JCAMPFile::EntryMap& getParameters() const { return pars_.getEntries();}

		///
		const JCAMPFile::HeaderMap& getHeader() const { return pars_.getHeader();}

		//@}

		private:
			const Bruker1DFile& operator = (const Bruker1DFile& file) ;

		protected:

		Size min_;
		Size max_;

		/// The parameters from the procs file
		JCAMPFile pars_;

		RegularData1D spectrum_;
	};
}

#endif // BALL_FORMAT_BRUKER1DFILE_H
