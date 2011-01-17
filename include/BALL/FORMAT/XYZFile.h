// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_XYZFILE_H
#define BALL_FORMAT_XYZFILE_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#	include <BALL/FORMAT/genericMolFile.h>
#endif

namespace BALL 
{
	class System;
	class Molecule;
	class AtomContainer;

	/**	XYZ file class.
			This class enables BALL to read and write XMol XYZ files.
			The XYZ format is a very simple molecular file format. In general it contains
			only the atom type (i.e., the element) and the cartesian coordinates
			of the structure. 
			Newer versions of this file format also contain bonds (without bond order) and
			atom names.\par
			The first line of each XYZ file contains a single integer number: the number
			of atoms in the file. The second line is just a comment line. When reading a 
			XYZ file, BALL stores this comment as the name attribute of the system read.
			Similarly, on writing the system, it's name is written to this comment line.
			All remaining lines contain the element symbol and the three coordinates
			in free format.	 \par
			
    	\ingroup  StructureFormats
	*/
	class BALL_EXPORT XYZFile
		: public GenericMolFile
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		XYZFile();

		/** Detailed constructor.
		 *	Create a XYZ file and open it with mode <tt>open_mode</tt> (reading is default)
		 *	@param filename the filename
		 *	@param open_mode the openmode - default is  \link File::IN File::IN \endlink 
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		XYZFile(const String& filename, File::OpenMode open_mode = std::ios::in);

		/** Destructor
		 */
		virtual ~XYZFile();
		
		//@}
		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{
		
		/** Write an AtomContainer to the XYZ file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		virtual bool write(const AtomContainer& ac);

		/**	Write a system to the XYZ file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		virtual bool write(const System& system);
		
		/**	Write a molecule to the XYZ file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		virtual bool write(const Molecule& mol);

		/**	Read a system from the XYZ file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		virtual bool read(System&	system);

		/** Read a molecule from the XYZ file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		virtual Molecule* read();

		///@deprecated Instead of this method use XYZFile::getComment()
		BALL_DEPRECATED const String& getComent() const { return comment_; }
		
		///
		const String& getComment() const { return comment_; }

		private:
			const XYZFile& operator = (const XYZFile& file);

		String comment_;
		
		//@}
	};
} // namespace BALL

#endif // BALL_FORMAT_XYZFILE_H
