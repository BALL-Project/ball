// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringPerceptionProcessor.h,v 1.5 2004/11/03 20:33:25 oliver Exp $
//

#ifndef BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
#define BALL_QSAR_RINGPERCEPTIONPROCESSOR_H

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#include <queue>

namespace BALL
{
	/**	Processor, which marks all atoms and bonds in a ring structure with the
	    Composite Property "InRing".
			calculateSSSR() can also compute the number of rings found.
			
			The processor is an implementation of Figueras algorithm, described in:
			J. Figueras, J. Chem. Inf. Comput. Sci., 1996, 36(5), 986-991
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
				@param SSSR, vector of rings, where the rings are stored in vector<Atom*>
				@param AtomContiner, from which AtomContainer the rings are to be percepted
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

		/*_ A bond, which when deleted leads to the smallest ring
				is deleted. 
				@param ring_set atoms to test
				@param ac the atom container the algorithm works on
		*/
		void checkEdges_(HashSet<Atom*>& ring_set, AtomContainer& ac);
		//@}
	};
} // namespace BALL

#endif // BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
