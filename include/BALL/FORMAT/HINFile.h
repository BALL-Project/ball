// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HINFile.h,v 1.22 2003/02/25 15:53:03 sturm Exp $

#ifndef BALL_FORMAT_HINFILE_H
#define BALL_FORMAT_HINFILE_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#	include <BALL/FORMAT/genericMolFile.h>
#endif

#ifndef BALL_MATHS_BOX_H
# include <BALL/MATHS/box3.h>
#endif

namespace BALL 
{
    /** \ingroup StructureFormats
     *  @{
     */
	/**	HyperChem file class.
			This class enables BALL to read and write HyperChem HIN files.  \par
			<b>Definition:</b> BALL/FORMAT/HINFile.h
	*/
	class HINFile
		: public GenericMolFile
	{
		public:

		BALL_CREATE(HINFile)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		HINFile()
			throw();

		/** Copy constructor
		*/
		HINFile(const HINFile& file)
			throw(Exception::FileNotFound);

		/** Detailed constructor
		*/
		HINFile(const String& filename, File::OpenMode open_mode = std::ios::in)
			throw(Exception::FileNotFound);

		/**	Destructor
		*/
		virtual ~HINFile()
			throw();
		//@}

		/**	@name Assignment.
		*/
		//@{
		/** Assignment operator.
		*/
		const HINFile& operator = (const HINFile& rhs)
			throw();
		//@}

		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{
		
		/**	Write a system to a HIN file.
				Note that this changes the properties of atoms in the system.
		*/
		virtual void write(const Molecule& molecule);
			
		virtual void write(const System& system);
		
		/**	Read a system from the HIN file
		*/
		virtual Molecule* read()
			throw(Exception::ParseError);

		/**
		*/
		virtual bool read(System& system)
			throw(Exception::ParseError);

		/**	Initialize internals.
				Initialize temperature and box dimensions prior to
				reading a system.
		*/
		virtual void initRead();
		//@}

		/**	@name	Accessors
		*/
		//@{
				
		/**	Check for a periodic boundary in the file.
		*/
		bool hasPeriodicBoundary() const;

		/**	Return the periodic boundary of the file.
				An emptry box is returned if no periodic boundary is defined.
				@return	the boundary box
		*/
		Box3 getPeriodicBoundary() const;

		/**	Return the temperature stored in the file.
				HIN files may contain a <tt>sys</tt> entry containing
				the temperature of the last simulation step. If it is set,
				it is returned. Otherwise 0 is returned.
				@return	the final simulation temperature
		*/
		float	getTemperature() const;

		//@}

		protected:
		
		Box3		box_;
		float		temperature_;
	
		void writeAtom_(const Atom& atom, Size number, Size atom_offset);
	};
  /** @} */
} // namespace BALL

#endif // BALL_FORMAT_HINFILE_H
