// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: rotamerLibrary.h,v 1.25 2003/04/01 21:18:04 oliver Exp $

#ifndef BALL_STRUCTURE_ROTAMERLIBRARY_H
#define BALL_STRUCTURE_ROTAMERLIBRARY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENT_DB_H
#	include <BALL/STRUCTURE/fragmentDB.h>
#endif

namespace BALL
{
	/** Rotamer class.
		\ingroup StructureRotamers

		This class is used to describe <em>rotamers</em>, i.e. rotational conformers,
		of amino acid side chains. These rotamers correspond to different low-energy conformations.
		Rotamers can alos be used to store and retrieve specific conformations of a side chain.
		In principle, a rotamer is just a list containing the side chain torsion angles (up to four)
		and the probability for that angle (useful only in the context of rotamer libraries).
	*/
	class Rotamer
	{
		public:

		/**	@name	Constructors
		*/
		//@{

		/**	Default constructor
		*/
		Rotamer();

		/**	Copy constructor
		*/
		Rotamer(const Rotamer& rotamer);

		/**	Constructor
		*/
		Rotamer(float new_P, float new_chi1, float new_chi2, float new_chi3, float new_chi4);

		//@}

		/**	@name	Attributes.
		*/
		//@{

		/**	Probability.
		*/
		float P;
		
		/**	Angle \TeX{$\chi_1$}
		*/
		Angle chi1;

		/**	Angle \TeX{$\chi_2$}
		*/
		Angle chi2;
		
		/**	Angle \TeX{$\chi_3$}
		*/
		Angle chi3;
		
		/**	Angle \TeX{$\chi_4$}
		*/
		Angle chi4;

		//@}
	};


	/** Residue Rotamer Set.
		\ingroup StructureRotamers		
		This class stores all potential rotamers for a given side-chain type.
		By linking a residue rotamer set to a specific side chain, one can iteratively
		try all possible rotamers for this side chain. These rotamers for a side chain	
		are typically stored in a \link rotamer library RotamerLibrary \endlink.
	*/
	class ResidueRotamerSet
	{
		public:

		/**	@name	Type definitions
		*/
		//@{

		///
		typedef vector<Rotamer>::const_iterator	ConstIterator;
		///
		typedef vector<Rotamer>::iterator				Iterator;

		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		BALL_CREATE_DEEP(ResidueRotamerSet)

		/**	Default constructor.
		*/
		ResidueRotamerSet();

		/** Copy constructor
		*/
		ResidueRotamerSet(const ResidueRotamerSet& rotamer_set, bool deep = true);

		/**	Detailed constructor
		*/
		ResidueRotamerSet(const Residue& residue, Size number_of_torsions);

		/**	Destructor
		*/
		virtual ~ResidueRotamerSet();
		//@}


		/**	@name	External Iteration
		*/
		//@{

		/**	
		*/
		Iterator begin();

		/**
		*/
		Iterator end();
		
		/**
		*/
		ConstIterator begin() const;

		/**
		*/
		ConstIterator end() const;
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	The assignment operator
		*/
		const ResidueRotamerSet& operator = (const ResidueRotamerSet& residue_rotamer_set);

		//@}

		/** @name Accessors
		*/
		//@{

		/** Get the name of the ResidueRotamerSet 
		*/
		const String& getName() const; 

		/**
		*/
		Residue& getResidue(); 

		/** Find out if the class instance is valid
		*/
		bool isValid() const; 

		/**
		*/
		Size getNumberOfRotamers() const;

		/**	Return the number of valid torsions
		*/
		Size getNumberOfTorsions() const;

		/**	Set the number of valid torsions
		*/
		void setNumberOfTorsions(Size number_of_torsions);

		/**	Random access operator for single rotamers.
		*/
		Rotamer& operator [] (Position index);
		//@}

		/**	@name	Rotamer Assignment
		*/
		//@{

		/**	Assign a specific rotamer.
				Sets the side-chain torsions of <tt>residue</tt> to the values specified by <tt>rotamer</tt>.
		*/
		bool setRotamer(Residue& residue, const Rotamer& rotamer);

		/**	Calculate the torsion angle of a residue.
				Retrieve the side-chain torsion angles from the residue's current conformation.
		*/
		Rotamer getRotamer(const Residue& residue) const;

		/**	Return a reference to a given rotamer by number.
				The index should be in the range [0,  \link getNumberOfRotamers getNumberOfRotamers \endlink ()[,
				however the index is computed modulo the maximum number, so it
				really doesn't matter.
		*/
		const Rotamer& getRotamer(Position index) const;

		/** Assign a new name
		*/
		void setName(const String& name); 

		/**	Add a rotamer to the current set.
		*/
		void addRotamer(const Rotamer& rotamer);

		/**	Build a copy of a specified rotamer.
				This method sets the side-chain torsion angles
				according to the values specified by <tt>rotamer</tt> and returns a copy of the residue.
		*/
		Residue* buildRotamer(const Rotamer& rotamer);

		//@}
		
		protected:

		/** Determines all movable atoms 
		*/
		void addMovable_(vector<String>& movable, Atom& a); 

		/** Set the torsion angles
		*/
		void setTorsionAngle_(const vector<String>& movable, Angle angle); 


		/** Indicates whether the instance is valid 
		*/ 
		bool valid_; 


		/** Name of the variant.
				(NOT the residue name, but the variant name obtained from the template DB)
    */ 
		String								name_;
		
		/** Contains the residue (side chain and backbone)
		*/
		Residue								side_chain_;

		/** Hash map containing all atom names of the variant
		    and pointers to the corresponding atoms in residue_
		*/
		StringHashMap<Atom*>	atom_name_map_;

		/** The three anchor atoms (backbone atoms)
		    used to determine the primary transformation
		*/
		Atom*									anchor_atoms_[3];

		/** Array containing all rotamers
		*/
		vector<Rotamer>				rotamers_;

		/** Names of the movable atoms (names) for each of the torsions
		*/
		vector<String> movable_atoms_chi1_;
		vector<String> movable_atoms_chi2_;
		vector<String> movable_atoms_chi3_;
		vector<String> movable_atoms_chi4_;

		/**	Number of valid torsions of the side chain
		*/
		Size number_of_torsions_;

		/**	Original atom coordinates of side_chain_
		*/
		vector<Vector3>				original_coordinates_;
	};

	/** Rotamer Library Class.
		\ingroup StructureRotamers
		A rotamer library contains a list of all possible torsion angle combinations
		observed for each of the 19 amino acid side chains (GLY does not have any
		useful rotamers). 
	*/
	class RotamerLibrary
	{
		public:

		BALL_CREATE_DEEP(RotamerLibrary)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		RotamerLibrary();

		/**	Detailed constructor
		*/
		RotamerLibrary(const String& filename, const FragmentDB& fragment_db);

		/**	Copy constructor
		*/
		RotamerLibrary(const RotamerLibrary& library, bool deep = true);

		/**	Destructor
		*/
		virtual ~RotamerLibrary();

		//@}

		/**	@name Assignment
		*/
		//@{
			
		/**	Assignment operator
		*/
		const RotamerLibrary& operator = (const RotamerLibrary& rhs);
			
		/**	Read from a file.
				This method reads rotamer libraries from SQWRL files (Dunbrack et al.).
		*/
		bool readSQWRLLibraryFile(const String& filename, const FragmentDB& fragment_db);
		//@}
			

		/**	@name Accessors
		*/
		//@{

		/**
		*/
		ResidueRotamerSet* getRotamerSet(const String& name);

		/**
		*/
		Size getNumberOfVariants() const;

		/**
		*/
		Size getNumberOfRotamers() const;
		//@}

		protected:

		/** @name Protected Attributes
		*/
		//@{

		/** Contains the available variants 
		*/ 
		vector<ResidueRotamerSet>	variants_;

		/** Indicates whether the instance is valid 
		*/ 
		bool valid_;
		//@}
	};

#ifndef BALL_NO_INLINE_FUNCTIONS
#	include <BALL/STRUCTURE/rotamerLibrary.iC>
#endif
  
} // namespace BALL

#endif // BALL_STRUCTURE_ROTAMERLIBRARY_H
