// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_HINFILE_H
#define BALL_FORMAT_HINFILE_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#	include <BALL/FORMAT/genericMolFile.h>
#endif

#ifndef BALL_MATHS_SIMPLEBOX3_H
# include <BALL/MATHS/simpleBox3.h>
#endif

namespace BALL 
{
	/**
	 * HyperChem file class.
	 * This class enables BALL to read and write HyperChem HIN files.  \par
	 *
	 * Note: HIN defines a Molecule as a connected component in the molecule graph
	 *       If you read e.g. a protein from a PDBFile, such that no bonds are set
	 *       each atom will be placed into its own molecule. To prevent this from
	 *       happening use the \link FragmentDB FragmentDB to build missing bonds.
	 *
	 * \ingroup  StructureFormats
	 */
	class BALL_EXPORT HINFile
		: public GenericMolFile
	{
		public:

		/** @name Constructors and Destructors
		*/
		//@{
		
		/** Default constructor
		 */
		HINFile();

		/** Detailed constructor
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		HINFile(const String& filename, File::OpenMode open_mode = std::ios::in);

		/**	Destructor
		 */
		virtual ~HINFile();

		//@}
		/**	@name Assignment.
		*/
		//@{
		
		/** Assignment operator.
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		const HINFile& operator = (const HINFile& rhs);

		//@}
		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{
		
		/**	Write a molecule to a HIN file.
		 *	Note that this changes the properties of atoms in the system.
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		virtual bool write(const Molecule& molecule);

		/**	Write a system to a HIN file.
		 *	Note that this changes the properties of atoms in the system.
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		virtual bool write(const System& system);
		
		/**	Read a molecule from the HIN file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		virtual Molecule* read();

		/** @copydoc GenericMolFile(System& system)
		 */
		virtual bool read(System& system);

		//@}
		/**	@name	Accessors
		*/
		//@{
				
		/**	Check for a periodic boundary in the file.
		*/
		bool hasPeriodicBoundary() const;

		/**	Return the periodic boundary of the file.
				An empty box is returned if no periodic boundary is defined.
				@return	the boundary box
		*/
		SimpleBox3 getPeriodicBoundary() const;

		/**	Return the temperature stored in the file.
				HIN files may contain a <tt>sys</tt> entry containing
				the temperature of the last simulation step. If it is set,
				it is returned. Otherwise 0 is returned.
				@return	the final simulation temperature
		*/
		float	getTemperature() const;

		//@}

		protected:
		
		SimpleBox3		box_;
		
		///	Initialize temperature and box dimensions prior to reading a system.
		virtual void initRead_();

		float		temperature_;
	
		void writeAtom_(const Atom& atom, Size number, Size atom_offset);
	};
} // namespace BALL

#endif // BALL_FORMAT_HINFILE_H
