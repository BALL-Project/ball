// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: KCFFile.h,v 1.2 2005/12/23 17:01:44 amoll Exp $
//

#ifndef BALL_FORMAT_KCFFILE_H
#define BALL_FORMAT_KCFFILE_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#	include <BALL/FORMAT/genericMolFile.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{
	class System;
	class Atom;
	class Molecule;

	/**	KEGG KCF file class.
		This class reads and writes KEGG (Kyoto Encyclopedia of Genes and Genomes) KCF (KEGG Chemical Function) files.\par
			
    	\ingroup  StructureFormats
	*/
	class BALL_EXPORT KCFFile
		: public GenericMolFile
	{
		public:

		BALL_CREATE(KCFFile)


		/**	@name Constants */
		//@{
		static const char* ENTRY_TAG;
		static const char* NODE_TAG;
		static const char* EDGE_TAG;
		static const char* DELIMITER_TAG;
		static const char* CONTINUED_LINE;
	  //@}
		
		/**	@name Type definitions */
		//@{
		typedef HashMap<const Atom*, Position> AtomIndexMap;
		typedef HashMap<Position, Atom*> IndexAtomMap;
		//@}
		
		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		KCFFile() ;

		////
		KCFFile(const String& filename, File::OpenMode open_mode = std::ios::in)
			throw(Exception::FileNotFound);

		////
		KCFFile(const KCFFile& file) throw(Exception::FileNotFound);

		///
		virtual ~KCFFile() ;
		//@}

		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{
		
		///	Write a molecule to the file
		virtual bool write(const Molecule& molecule) throw(File::CannotWrite);

		/**	Write a system to the KCF file.
			Each Molecule in the system is written as distinct molecule in the KCF file.
		*/
		virtual bool write(const System& system) throw(File::CannotWrite);
		
		/**	Read a system from a KCF File.
			  Each molecule in the file is stored as an individual instance of Molecule
		  	within the system. Previous contents of the system are erased.
		*/
		virtual bool read(System& system) throw(Exception::ParseError);

		/**	Read a single molecule from the file
   			If unscuccesful, a NULL pointer is returned.
		*/
		virtual Molecule* read() throw(Exception::ParseError);

		///
		const KCFFile& operator = (const KCFFile& file)
			;

		//@}

		protected:

		bool readENTRY_(Molecule& mol);		
		bool readNODE_(Molecule& mol, IndexAtomMap& index_to_atom);
		bool readEDGE_(IndexAtomMap& index_to_atom);		
		bool readDELIMITER_();
	};
	
} // namespace BALL

#endif // BALL_FORMAT_KCFFILE_H
