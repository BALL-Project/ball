#ifndef BALL_KERNEL_SPLITMOLECULE_H
#define BALL_KERNEL_SPLITMOLECULE_H

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_FRAGMENTITERATOR_H
#	include <BALL/KERNEL/fragmentIterator.h>
#endif


#	include <vector>

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif



namespace BALL 
{
	//class System;

	/**	SplitMolecule class.
			Used to represent molecules that are split into fragments. \par
			
   		\ingroup KernelContainers 
	*/
	class BALL_EXPORT SplitMolecule: public Molecule
	{
		public:
		
		BALL_CREATE_DEEP(SplitMolecule)
		
		/**	Default constructor. 
		*/
		SplitMolecule();
	
		/** Copy constructor. 
		*/
		SplitMolecule(const Molecule& molecule, bool deep = true);
		
		/** Destructor. 
		*/
		virtual ~SplitMolecule();
		
		/**	Assignment operator.
				@param molecule the Molecule to assign from
		**/
		SplitMolecule& operator = (const SplitMolecule& molecule);
		
		protected:
		// pointer to each internal fragment of the molecule
		// (NO 3D COORDS)
		std::vector< std::vector<Atom*> > fragments;
		
		// structural templates 
		// (WITH 3d COORDS)
		std::vector< std::vector<Atom*> > templates;
		
	};
}

#endif // BALL_KERNEL_SPLITMOLECULE_H
