// $Id: rotamerLibrary.h,v 1.2 1999/08/27 12:10:09 oliver Exp $

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
		
		/**	Angle \TeX{$\Chi_1$}
		*/
		Angle chi1;

		/**	Angle \TeX{$\Chi_2$}
		*/
		Angle chi2;
		
		/**	Angle \TeX{$\Chi_3$}
		*/
		Angle chi3;
		
		/**	Angle \TeX{$\Chi_4$}
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

		/**	Detailled constructor
		*/
		ResidueRotamerSet(const Residue& residue);

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
		

		/**	@name	Rotamer Assignment
		*/
		//@{

		/**	Assign a specific rotamer.
		*/
		bool setRotamer(Residue& residue, const Rotamer& rotamer) const;

		/**	
		*/
		void addRotamer(const Rotamer& rotamer);

		/**	Build a copy of a specified rotamer
		*/
		Residue* buildRotamer(const Rotamer& rotamer) const;
		//@}
		
		protected:

		// name of the variant (NOT the residue name, but the variant name
		// obtained from the template DB)
		String								name_;
		
		// contains the residue (side chain and backbone)
		Residue								side_chain_;

		// hash map containing all atom names of the variant
		// and pointers to the corresponding atoms in residue_
		StringHashMap<Atom*>	atom_name_map_;

		// the three anchor atoms (backbone atoms)
		// used to determine the primary transformation
		Atom*									anchor_atoms_[3];

		// array containing all rotamers
		vector<Rotamer>				rotamers_;

		// names of the movable atoms (names) for each of the
		// torsions
		vector<String>				moveable_atoms_chi1_;
		vector<String>				moveable_atoms_chi2_;
		vector<String>				moveable_atoms_chi3_;
		vector<String>				moveable_atoms_chi4_;
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

		/**	Detailled constructor
		*/
		RotamerLibrary(const String& filename);

		/**	Copy constructor
		*/
		RotamerLibrary(const RotamerLibrary& library, bool deep = true);

		/**	Destructor
		*/
		virtual ~RotamerLibrary();

		//@}

		bool readSQWRLLibraryFile(const String& filename);

		ResidueRotamerSet& getRotamerSet(const Residue& residue);

		ResidueRotamerSet& getRotamerSet(const String& name);

		Size getNumberOfVariants() const;

		Size getNumberOfRotamers() const;

		protected:

		vector<ResidueRotamerSet>	variants_;
		
		bool valid_;
	};

#ifndef BALL_NO_INLINE_FUNCTIONS
#	include <BALL/STRUCTURE/rotamerLibrary.h>
#endif

} // namespace BALL

#endif // BALL_STRUCTURE_ROTAMERLIBRARY_H
