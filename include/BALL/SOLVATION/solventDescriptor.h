// $Id: solventDescriptor.h,v 1.9 2001/07/16 23:58:34 amoll Exp $

#ifndef BALL_SOLVATION_SOLVENTDESCRIPTOR_H
#define BALL_SOLVATION_SOLVENTDESCRIPTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL
{

	/** @name Solvent Classes
	*/
	//@{

	/** This struct contains the information for one atom type of the solvent 
	*/
	typedef struct 
	{
		/** Atom type needed for assignment of Lennard-Jones Parameters 
		*/
		Atom::Type type;

		/** Element symbol of the atom 
		*/
		String element_symbol;

		/** Radius of the atom in units of Angstrom 
		*/
		float radius;

		/** Number of atoms of this kind within the solvent molecule 
		*/
		int number_of_atoms;

	} SolventAtomDescriptor;


	/** Solvent Description.
			The calculation of van-der-Waals energies requires information about the
			solvent which is stored in this structure.	\\
			{\bf Definition:} \URL{BALL/SOLVATION/solventDescriptor.h}
	*/
	class SolventDescriptor
	{

		public:

		BALL_CREATE(SolventDescriptor)

		/** @name Constructors and destructor 
		*/
		//@{

		/** Default constructor 
		*/
		SolventDescriptor() throw();

		/** Copy constructor 
		*/
		SolventDescriptor(const SolventDescriptor& solvent) throw();

		/** Detailed constructor 
		*/
		SolventDescriptor(const String& name, float number_density,
			const std::vector<SolventAtomDescriptor>& atom_list) throw();

		/** Destructor 
		*/
		virtual ~SolventDescriptor() throw();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const SolventDescriptor& operator = (const SolventDescriptor& descriptor) 
			throw();

		/** Clear function 
		*/
		void clear() throw();

		//@}
		/** @name Accessors 
		*/
		//@{

		/** Set the name 
		*/
		void setName(const String& name) throw();

		/** Get the name 
		*/
		const String& getName() const throw();

		/** Set the number density 
		*/
		void setNumberDensity(float number_density) throw();
		
		/** Get the number density of this solvent (in $ A^{-3}$) 
		*/
		float getNumberDensity() const throw();

		/** Set the list of solvent atom descriptors 
		*/
		void setSolventAtomDescriptorList(const
			std::vector<SolventAtomDescriptor>& solvent_atoms) throw();

		/** Get the list of atom descriptors 
		*/
		const std::vector<SolventAtomDescriptor>& getSolventAtomDescriptorList() const
			throw();
		
		/** Get the list of atom descriptors 
		*/
		std::vector<SolventAtomDescriptor>& getSolventAtomDescriptorList()
			throw();

		/** Get the number of different atom types within a solvent molecule 
		*/
		Size getNumberOfAtomTypes() const throw();

		/** Get atom decriptions by index 
		*/
		const SolventAtomDescriptor& getAtomDescriptor(Position index) const 
			throw(Exception::IndexOverflow);
		
		/** Get atom decriptions by index 
		*/
		SolventAtomDescriptor& getAtomDescriptor(Position index) 
			throw(Exception::IndexOverflow);

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Validity 
		*/
		bool isValid() const throw();

		/** Equality operator 
		*/
		bool operator == (const SolventDescriptor& descriptor) const throw();

		//@}

		protected:

		/*_ The name of this solvent 
		*/
		String name_;

		/*_ The number density of this solvent, i. e. [missing] 
		*/
		float number_density_;

		/*_ This vector contains all atom types occuring in this solute 
		*/
		std::vector<SolventAtomDescriptor> solvent_atoms_;

		/*_ The valid flag 
		*/
		bool valid_;

	};
}
	
//@}

#endif // BALL_SOLVATION_SOLVENTDESCRIPTOR_H
