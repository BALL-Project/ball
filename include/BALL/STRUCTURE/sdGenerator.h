
#ifndef BALL_STRUCTURE_SDGENERATOR_H
#define BALL_STRUCTURE_SDGENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <string>
#include <vector>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/CONCEPT/property.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/MATHS/matrix44.h>

#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/STRUCTURE/ringClusterer.h>
#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/STRUCTURE/rsConstructor.h>
#include <BALL/STRUCTURE/chainBuilder.h>
#include <BALL/STRUCTURE/moleculeAssembler.h>

namespace BALL
{

	/// ???? TODO: in enum in der Klasse verpacken!
	const Property in_ring = Atom::NUMBER_OF_PROPERTIES + 1;
	const Property pre_core_chain = in_ring + 1;
	const Property core_chain = pre_core_chain + 1;
	const Property on_queue = core_chain + 1;
	const Property assigned = on_queue + 1;
	const Property found = assigned + 1;
	const Property invalid = assigned + 1;
	const Property deposited = invalid + 1;
	const Property firstneighbour = deposited + 1;
	const Property sequenced = firstneighbour + 1;
	const Property EqAS = sequenced + 1;
	const Property FxAS = EqAS + 1;
	const Property ambigous = FxAS + 1;
	const Property amorphous = ambigous + 1;
	const Property edge = amorphous + 1;
	const Property built_in_chain = edge + 1;
	const Property assembled = built_in_chain + 1;
	const Property shifted = assembled + 1;
	const Property in_file = shifted + 1;
	const Property pre_assembled = in_file + 1;
	const Property rotated = pre_assembled +1;
	const Property zig = rotated + 1;
	const Property zag = zig + 1;
	const Property straight = zag + 1;

	/**
		* \brief The main class, provides methods for analysis of the input.
		* 
		*/
	class SDGenerator
	{

	private:

		/**
		* \brief Distinguishes between ring-atoms and core-chain-atoms, removes all H-Atoms from the System
		* @param molecule_sys 
		*/
		void checkAtoms(System& molecule_sys);

		/**
		* \brief Determines the smallest set of smallest Rings of the input System
		* @param molecule_sys 
		* @return the smallest set of smallest Rings 
		*/
		vector <vector<Atom*> > getSSSR(System& molecule_sys);


	public:
		/**
		 * 
		 * \brief Default-Constructor
		 */
		SDGenerator();

		/**
		 * 
		 * \brief Destructor
		 */
		~SDGenerator();

		/**
		* \brief Generates a structure Diagram from the input System
		* @param molecule_sys 
		*/
		void generateSD(System& molecule_sys);

		/**
		* \brief finds an atom's neighbours inside a certain ring
		* \param ring	the ring that shall be searched for the neighbours
		* \param atom	the atom, whos neighbours shall be found
		* @return atom's neighbours inside of ring
		*/
		pair<Atom*, Atom*> getNeighbours(vector<Atom*>& ring, Atom*& atom);

		/**
		* \brief Puts the Atoms in each ring of the input ringsystem into the correct order
		* @param ringsystem 
		* @return the ringsystem with sorted atoms
		*/
		vector<vector<Atom*> > sequenceRings(vector<vector<Atom*> >& ringsystem);

	};

} // namepspace BALL

#endif // BALL_STRUCTURE_SDGENERATOR_H
