// $Id: solventDescriptor.h,v 1.1 2000/08/31 16:47:22 anker Exp $

// BAUSTELLE: Pfad?
#ifndef BALL__SOLVENTDESCRIPTOR_H
#define BALL__SOLVENTDESCRIPTOR_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#include <BALL/KERNEL/atom.h>
#endif

namespace BALL
{

	/** */
	typedef struct 
	{
		/** Atom type needed for assignment of Lennard-Jones Parameters */
		Atom::Type type;

		/** Element symbol of the atom */
		String element_symbol;

		/** Radius of the atom */
		float radius;

		/** number of atoms of this kind within the solvent molecule */
		int number_of_atoms;
	} SolventAtomDescriptor;


	/** Solvent Description.
	The calculation of van-der-Waals energies requires information about the
	solvent which is stored in this structure.
	@see 
	*/
	class SolventDescriptor
	{

		public:

		// BALL_CREATE(SolventDescriptor)

		/** @name Constructors and destructor */
		//@{

		/** Default constructor */
		SolventDescriptor();

		/** Copy constructor */
		SolventDescriptor(const SolventDescriptor& solvent);

		/** Detailed constructor */
		SolventDescriptor(const String& name, float number_density,
				const std::vector<SolventAtomDescriptor>& atom_list);

		/** Destructor */
		virtual ~SolventDescriptor();

		/** Clear function */
		void clear();

		/** Destroy function */
		void destroy();

		//@}


		/** @name Assignment */
		//@{

		/** */
		void set(const SolventDescriptor& descriptor);

		/** */
		const SolventDescriptor& operator = (const SolventDescriptor& descriptor);

		//@}


		/** @name Accessors */
		//@{

		/** Set the name */
		void setName(const String& name);

		/** Get the name */
		String getName() const;

		/** Set the number density */
		void setNumberDensity(float number_density);
		
		/** Get the number density of this solvent (in $\A^{-3}$) */
		float getNumberDensity() const;

		/** Set the list of solvent atom descriptors */
		void setSolventAtomDescriptorList(const std::vector<SolventAtomDescriptor>& solvent_atoms);

		/** Get the list of atom descriptors */
		std::vector<SolventAtomDescriptor> getSolventAtomDescriptorList() const;

		/** Get the number of different atom types within a solvent molecule */
		Size getNumberOfAtomTypes() const;

		/** Get atom decriptions by index */
		SolventAtomDescriptor getAtomDescriptor(Position index) const;

		//@}


		protected:

		String name_;
		float number_density_;
		std::vector<SolventAtomDescriptor> solvent_atoms_;

	};
}
	


#endif BALL__SOLVENTDESCRIPTOR_H
