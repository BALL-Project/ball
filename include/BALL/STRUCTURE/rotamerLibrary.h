// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_ROTAMERLIBRARY_H
#define BALL_STRUCTURE_ROTAMERLIBRARY_H

#ifndef BALL_STRUCTURE_FRAGMENT_DB_H
#	include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_STRUCTURE_RESIDUEROTAMERSET_H
 #include <BALL/STRUCTURE/residueRotamerSet.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
	#include <BALL/DATATYPE/hasMap.h>
#endif

//#include <map>

namespace BALL
{
	class FragmentDB;
	class String;

	/** @brief Rotamer Library Class.
		
		\ingroup StructureRotamers
		A rotamer library contains a list of possible rotamers
		for each of the 18 amino acid side chains (GLY and ALA do not have rotamers). 
	*/
	class BALL_EXPORT RotamerLibrary
	{
		public:

		BALL_CREATE(RotamerLibrary)

    /** @name String constants
     */
    //@{

    /// Rotamer library used when calling the default constructor
    static const String DEFAULT_LIBRARY;

    /// Fragment DB used when calling the default constructor
    static const String DEFAULT_FRAGMENT_DB;
    //@}

		/**	@name	Constructors and Destructors
		*/
		//@{
		/**	Default constructor.
        Uses rotamer library <tt>RotamerLibrary::DEFAULT_LIBRARY</tt> and 
        fragment db <tt>RotamerLibrary::DEFAULT_FRAGMENT_DB</tt>.
		*/
		RotamerLibrary() ;

		///	Detailed constructor, provided for convenience
		RotamerLibrary(const String& filename, const FragmentDB& fragment_db) ;

		/// Detailed constructor
		RotamerLibrary(const FragmentDB& fragment_db) ;

		///	Copy constructor
		RotamerLibrary(const RotamerLibrary& library) ;

		///	Destructor
		virtual ~RotamerLibrary();
		//@}

		/**	@name Assignment
		*/
		//@{
		///	Assignment operator
		RotamerLibrary& operator = (const RotamerLibrary& rhs);
		//@}
			

		/**	@name Accessors
		*/
		//@{
		/// Return the residue set of the residue name.
		ResidueRotamerSet* getRotamerSet(const String& name);

		/// Return the residue set of the given name and torsions.
		ResidueRotamerSet* getRotamerSet(const String& name, float phi, float psi);

		/// Return the residue set of the given residue.
		ResidueRotamerSet* getRotamerSet(const Residue& residue);

		/// Return total number of rotamers.
		Size getNumberOfRotamers() const;

		/// Return the number of rotamers of the residue with the name.
		Size getNumberOfRotamers(const String& name) const;

		/// returns the number of rotamer sets
		Size getNumberOfRotamerSets() const;

		/// adds rotamer with specific phi/psi backbone torsion
		void addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions, Index phi, Index psi);

		/// adds a rotamer with name
		void addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions);

		/// returns true if the rotamers are backbone dependent
		bool isBackboneDependent() const;

		/// sets whether the rotamers are backbone dependent or not
		void setBackboneDependent(bool dependent);

		/// returns true if the library contains rotamers with this name
		bool hasRotamers(const String& name) const;

		/// method to test the validity of this library, typically called after the building (see SCWRLLibraryFile)
		bool validate();

		/// method to sort the Rotamers of each ResidueRotamerSet descendingly according to the probability
		void sort();

    /** Clear rotamer library.
        Removes all rotamers and sets library to backbone independent.
     */
    void clear();
		//@}

		protected:

		/** @name Protected Attributes
		*/
		//@{
		/// Contains the available variants (phi, psi, name, rotamer set) in case of bb dep
		HashMap<Index, HashMap<Index, HashMap<String, ResidueRotamerSet> > > bb_dep_sets_;
	
		/// Contains the available variants (name, rotamer set) in case of bb indep
		HashMap<String, ResidueRotamerSet> bb_indep_sets_;

		/// 
		Index getNearestBackboneTorsion_(double angle) const;

		/// discretization step width of the torsion angles
		Size step_width_;

		/// fragment db used in this class
		FragmentDB fragment_db_;

		/// flag which is true when the library has backbone dependent rotamers
		bool backbone_dependent_;
		//@}
	};

} // namespace BALL

#endif // BALL_STRUCTURE_ROTAMERLIBRARY_H
