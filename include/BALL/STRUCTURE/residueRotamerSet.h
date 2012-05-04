// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_RESIDUEROTAMERSET_H
#define BALL_STRUCTURE_RESIDUEROTAMERSET_H

#ifndef BALL_MATHS_ANGLE_H
	#include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
	#include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{
	/// forward declarations
	class Residue;
	template <typename Key> class HashSet;
	template <typename Key> class StringHashMap;
	class Atom;

	/** Rotamer class.
		\ingroup StructureRotamers

		This class is used to describe <em>rotamers</em>, i.e. rotational conformers,
		of amino acid side chains. These rotamers correspond to different low-energy conformations.
		Rotamers can also be used to store and retrieve specific conformations of a side chain.
		In principle, a rotamer object is just a list containing the side chain torsion angles (up to four)
		and the probability of that rotamer (useful only in the context of rotamer libraries).
	*/
	class BALL_EXPORT Rotamer
	{
		public:

		/**	@name	Constructors
		*/
		//@{
		/// Default constructor
		Rotamer();

		/// Copy constructor
		Rotamer(const Rotamer& rotamer);

		//// Detailed constructor
		Rotamer(float new_P, float new_chi1, float new_chi2 = 0.0, 
						float new_chi3 = 0.0, float new_chi4 = 0.0);
		//@}


		/**	@name	Attributes.
		*/
		//@{
		/// Probability.
		float P;
		
		/// Angle \f$\chi_1\f$ in degrees
		float chi1;

		///	Angle \f$\chi_2\f$ in degrees
		float chi2;
		
		/// Angle \f$\chi_3\f$ in degrees
		float chi3;
		
		/// Angle \f$\chi_4\f$ in degrees
		float chi4;
		//@}
	};


	/** Residue Rotamer Set.
		\ingroup StructureRotamers		
		This class stores all potential rotamers for a given side chain type.
		By linking a residue rotamer set to a specific side chain, one can iteratively
		try all contained rotamers for this side chain. The rotamers for a side chain	
		are typically stored in a \link RotamerLibrary rotamer library \endlink.

		The basic functionality is to store the rotamers and to modify residues 
		according to this rotamers.
	*/
	class BALL_EXPORT ResidueRotamerSet
	{
		public:

		/**	@name	Type definitions
		*/
		//@{
		///
		typedef std::vector<Rotamer>::const_iterator	ConstIterator;
		typedef std::vector<Rotamer>::const_iterator	const_iterator;
		///
		typedef std::vector<Rotamer>::iterator				Iterator;
		typedef std::vector<Rotamer>::iterator				iterator;
		//@}


		/**	@name Constructors and Destructors
		*/
		//@{
		BALL_CREATE(ResidueRotamerSet)

		/**	Default constructor.
		*/
		ResidueRotamerSet();

		/** Copy constructor
		*/
		ResidueRotamerSet(const ResidueRotamerSet& rotamer_set);

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
		Iterator begin()
		{
			return rotamers_.begin();
		}

		/**
		*/
		Iterator end()
		{
			return rotamers_.end();
		}
		
		/**
		*/
		ConstIterator begin() const
		{
			return rotamers_.begin();
		}

		/**
		*/
		ConstIterator end() const
		{
			return rotamers_.end();
		}
		//@}

		/**	@name	Assignment
		*/
		//@{
		///	The assignment operator
		const ResidueRotamerSet& operator = (const ResidueRotamerSet& residue_rotamer_set);
		//@}

		/** @name Accessors
		*/
		//@{
		/// Get the name of the ResidueRotamerSet.
		const String& getName() const; 

		/// Find out if the class instance is valid.
		bool isValid() const; 

		/// Return the number of rotamers from this residue rotamer set.
		Size getNumberOfRotamers() const;

		/**	Return the number of valid torsions in the side chain.
				The number returned is between zero and four.
		*/
		Size getNumberOfTorsions() const;

		/**	Set the number of valid torsions for this side chain.
				\exception Exception::IndexOverflow if the number of torsions is above four.
		*/
		void setNumberOfTorsions(Size number_of_torsions) throw(Exception::IndexOverflow);

		///	Random access operator for single rotamers.
		const Rotamer& operator [] (Position index)	const throw(Exception::IndexOverflow);

		/// 
		bool hasTorsionPhi() const;

		///
		Angle getTorsionPhi() const;

		///
		void setTorsionPhi(const Angle& phi);

		///
		bool hasTorsionPsi() const;

		///
		Angle getTorsionPsi() const;

		///
		void setTorsionPsi(const Angle& psi);
		//@}

		/**	@name	Rotamer Assignment
		*/
		//@{
		/// Set the template residue, name, base coordinates ...
		bool setTemplateResidue(const Residue& residue, Size number_of_torsions);

		/**	Assign a specific rotamer.
				Set the side-chain torsions of <tt>residue</tt> to the values specified by <tt>rotamer</tt>.
		*/
		bool setRotamer(Residue& residue, const Rotamer& rotamer);

		/**	Calculate the torsion angle of a residue.
				Retrieve the side-chain torsion angles from the residue's current conformation.
		*/
		Rotamer getRotamer(const Residue& residue) const;

		/**	Return a reference to a given rotamer by number.
				The index should be in the range [0,  \link Rotamer::getNumberOfRotamers getNumberOfRotamers \endlink ()[,
				however the index is computed modulo the maximum number, so it
				really doesn't matter.
		*/
		const Rotamer& getRotamer(Position index) const;

		/// Assign a new name.
		void setName(const String& name); 

		///	Add a rotamer to the current set.
		void addRotamer(const Rotamer& rotamer);

    /// Delete the Rotamer at the given iterator location.
		void deleteRotamer(Iterator loc);
		
		/// Delete the range of rotamers.
		void deleteRotamers(Iterator begin, Iterator end);

		/// Sort the rotamers descendingly according to their probability.
		void sort();
		//@}
	
		

		protected:

		
		struct RotamerProbabilityGreaterThan_
			: std::binary_function<Rotamer, Rotamer, bool>
		{
			bool operator () (const Rotamer& r1, const Rotamer& r2)
			{
				return r1.P > r2.P;
			}
		};

		/** Determine all movable atoms.
				This method walks along the bonds starting at <tt>a</tt> and adds them
				to the <tt>movable</tt> vector. Atoms whose names are contained in <tt>assigned_atoms</tt>
				are ignored.
		*/
		void addMovable_(std::vector<String>& movable, const Atom& a, const HashSet<String>& assigned_atoms); 

		/// Set the torsion angles
		void setTorsionAngle_(Residue& residue, const std::vector<String>& movable, float torsion); 

		/// Indicates whether the instance is valid  
		bool valid_; 

		/// Name of the residue (variant name)
		String name_;
		
		/// Array containing all rotamers
		std::vector<Rotamer> rotamers_;

		/// Names of the movable atoms (names) for each of the torsions
		std::vector<String> movable_atoms_chi1_;
		std::vector<String> movable_atoms_chi2_;
		std::vector<String> movable_atoms_chi3_;
		std::vector<String> movable_atoms_chi4_;

		///	Number of valid torsions in the side chain
		Size number_of_torsions_;

		/// Original coordinates of the template residue
		StringHashMap<Vector3> original_coordinates_;

		/// true if this residue rotamer set is backbone dependent
		bool has_torsion_phi_;

		/// true if this residue rotamer set is backbone dependent
		bool has_torsion_psi_;

		/// The torsion phi
		Angle phi_;

		/// The torsion psi
		Angle psi_;
	};

} // namespace BALL

#endif // BALL_STRUCTURE_RESIDUEROTAMERSET_H
