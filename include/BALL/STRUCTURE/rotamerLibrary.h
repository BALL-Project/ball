// $Id: rotamerLibrary.h,v 1.8 1999/09/17 11:16:37 oliver Exp $

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

	/**
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

		/**	Detailled constructor
		*/
		Rotamer(float new_P, float new_chi1, float new_chi2, float new_chi3, float new_chi4);
		//@}

		/**	@name	Attributes
		*/
		//@{

		/**	Probability
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


	/**
	*/
	class ResidueRotamerSet
	{
		public:

		/**	@name	Type definitions
		*/
		//@{
		typedef vector<Rotamer>::const_iterator	ConstIterator;
		typedef vector<Rotamer>::iterator				Iterator;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{
		BALL_CREATE(ResidueRotamerSet)

		/**	Default constructor
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
			
		Iterator begin();

		Iterator end();
		
		ConstIterator begin() const;

		ConstIterator end() const;
		//@}
		
		/**	@name	Assignment Operator
		*/
		//@{

		/**	The assignment operator
		*/
		ResidueRotamerSet&	operator = (const ResidueRotamerSet& residue_rotamer_set);

		//@}

		/** @name Accessors
		*/
		//{
		/** get the name of the ResidueRotamerSet 
		*/
		const String & getName() const; 

		/**
		*/
		Residue& getResidue() ; 

		/** find out if the class instance is valid
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

		//}

		/**	@name	Rotamer Assignment
		*/
		//@{

		/**	Assign a specific rotamer.
		*/
		bool setRotamer(Residue& residue, const Rotamer& rotamer) ;

		/**	Calculate the torsion angle of a residue
		*/
		Rotamer getRotamer(const Residue& residue) const;

		/**  assign a new name
		*/
		void setName(String &name); 

		/**	
		*/
		void addRotamer(const Rotamer& rotamer);

		/**	Build a copy of a specified rotamer
		*/
		Residue* buildRotamer(const Rotamer& rotamer) ;
		//@}
		
		protected:
		/*_ @name Protected attributes 
		*/
		//@{

		/*_ determines all movable atoms 
		*/
		void addMoveable_(vector<String>& moveable, Atom& a); 

		/*_ set the torsion angles
		*/
		void setTorsionAngle_(const vector<String>& moveable, Angle angle); 


		/*_ indicates whether the instance is valid 
		*/ 
		bool valid_; 


		/*_ name of the variant (NOT the residue name, but the variant name
		    obtained from the template DB)
      		*/ 
		String								name_;
		
		/*_ contains the residue (side chain and backbone)
		*/
		Residue								side_chain_;

		/*_ hash map containing all atom names of the variant
		    and pointers to the corresponding atoms in residue_
		*/
		StringHashMap<Atom*>	atom_name_map_;

		/*_ the three anchor atoms (backbone atoms)
		    used to determine the primary transformation
		*/
		Atom*									anchor_atoms_[3];

		/*_ array containing all rotamers
		*/
		vector<Rotamer>				rotamers_;

		/*_ names of the movable atoms (names) for each of the
		    torsions
		*/
		vector<String>				moveable_atoms_chi1_;
		vector<String>				moveable_atoms_chi2_;
		vector<String>				moveable_atoms_chi3_;
		vector<String>				moveable_atoms_chi4_;

		/*_	number of valid torsions of the side chain
		*/
		Size number_of_torsions_;

		//@}
	};

	/**
	*/
	class RotamerLibrary
	{
		public:

		BALL_CREATE(RotamerLibrary)

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

		bool readSQWRLLibraryFile(const String& filename, const FragmentDB& fragment_db);

		ResidueRotamerSet* getRotamerSet(const String& name);

		Size getNumberOfVariants() const;

		Size getNumberOfRotamers() const;

		protected:

		/*_ @name Protected Attributes
		*/
		//@{

		/*_ Contains the available variants 
		*/ 
		vector<ResidueRotamerSet>	variants_;

		/*_ Indicates whether the instance is valid 
		*/ 
		bool valid_;
		//@}
	};

#ifndef BALL_NO_INLINE_FUNCTIONS
#	include <BALL/STRUCTURE/rotamerLibrary.iC>
#endif

} // namespace BALL

#endif // BALL_STRUCTURE_ROTAMERLIBRARY_H
