// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
#define BALL_QSAR_RINGPERCEPTIONPROCESSOR_H

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#include <queue>

namespace BALL
{
	/**	Generic .
			\\
	*/
	class RingPerceptionProcessor
		:	public UnaryProcessor<AtomContainer>
	{
		public:

		BALL_CREATE(RingPerceptionProcessor)

		/** @name Constrcutors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		RingPerceptionProcessor();

		/** Copy constructor
		*/
		RingPerceptionProcessor(const RingPerceptionProcessor& rp);

		/** Destructor
		*/
		~RingPerceptionProcessor();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		RingPerceptionProcessor& operator = (const RingPerceptionProcessor& rp);
		//@}

		/** @name Accessors
		*/
		//@{
		/** Method to get a smallest set of smallest rings (SSSR) from a molecule.
				æparam SSSR, vector of rings, where the rings are stored in vector<Atom*>
				@param Molecule, the molecule from which the rings to be percepted
		*/
		Size calculateSSSR(vector<vector<Atom*> >& sssr, AtomContainer& ac);
		//@}
		/** @name Processor-realted methods
		*/
		//@{
		Processor::Result operator () (AtomContainer& ac);
		//@}
		
		private:

		/*_ @name Accessors
		*/
		//@{
		/*_ Method that return the smallest ring which atom n participates
				@param atom from which the search is started
				@param ring set in which the found ring is stored (if any)
		*/
		Size getRing_(Atom* n, HashSet<Atom*>& ring_set);

		/*_ Method that deletes a atom from a molecular graph which has only
				nodes of degree three (or higher). That ring which leads to the smallest
				ring when deleted is deleted.
				@param ring set
				@param moelcule 
		*/
		void checkEdges_(HashSet<Atom*>& ring_set, AtomContainer& ac);
		//@}
	};
} // namespace BALL

#endif // BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
