// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringPerceptionProcessor.h,v 1.6 2005/03/16 12:26:00 bertsch Exp $
//

#ifndef BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
#define BALL_QSAR_RINGPERCEPTIONPROCESSOR_H

#ifndef BALL_KERNEL_ATOMCONTAINER_H
	#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_DATATYPE_BITVECTOR_H
	#include <BALL/DATATYPE/bitVector.h>
#endif

#ifndef BALL_STRUCTURE_MOLECULARGRAPH_H
	#include <BALL/STRUCTURE/molecularGraph.h>
#endif

#include <queue>
#include <stack>
#include <vector>

namespace BALL
{
	/**	Processor, which marks all atoms and bonds in a ring structure with the
	    Composite Property "InRing".
			calculateSSSR() can also compute the number of rings found.
			
			The processor is an implementation of Figueras algorithm, described in:
			J. Figueras, J. Chem. Inf. Comput. Sci., 1996, 36(5), 986-991

			and the Balducci Pearlman algorithm described in:
			Renzo Balducci, Robert S. Pearlman, J. Chem. Inf. Comput. Sci., 34:822-831, 1994
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
		
		/** @name Processor-related methods
		*/
		//@{
		Processor::Result operator () (AtomContainer& ac);
		//@}
	
		/** Method that finds all biconnected components, the algorithm is freely
				adapted from a standard bcc algorithm. Returns the number of bccs found.
		*/
		Size findAllBCC(std::vector<MolecularGraph*>& bcc, MolecularGraph& graph);
								
	
		protected:

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

		/*_ Method that finds all biconnected components, the algorithm is freely 
				adapted from a standard bcc algorithm.
		*/
		Size findAllBCC_(std::vector<MolecularGraph*>& bcc, MolecularGraph& graph);

		/*_ recursive function that finds bccs
		*/
		void DFSBCC_( std::vector<MolecularGraph*>& bccs, Size dfbi, 
									HashMap<NodeItem<Index, Index>*, Size> DFBIndex, 
									NodeItem<Index, Index>* v);
																					
		HashSet<NodeItem<Index, Index>* > visited_;
		HashSet<EdgeItem<Index, Index>* > visited_bonds_;
		HashMap<NodeItem<Index, Index>* , Size> P_;
		HashMap<NodeItem<Index, Index>*, NodeItem<Index, Index>* > parents_;
		stack<EdgeItem<Index, Index>* > BCC_;


		// Balducci and Pearlman algorithm
		struct PathMessage;
		
		/*_ The tnode structure described in the paper
		*/
		struct TNode
		{
			void recieve();
			
			void send();

			std::vector<PathMessage> recieve_buffer;

			std::vector<PathMessage> send_buffer;
		};

		/*_ The pathMsg structure described in the paper
		*/
		struct PathMessage
		{
			void push(EdgeItem<Index, Index>* bond, TNode* node);
		
			BitVector beep;
			
			TNode * nfirst;
			
			TNode * nlast;

			EdgeItem<Index, Index>* efirst;
		};

		// mapping for internal TNode structure and the nodes of the molecular graph
		static HashMap<TNode*, NodeItem<Index, Index>* > tnode_to_atom;
		static HashMap<NodeItem<Index, Index>* , TNode*> atom_to_tnode;
		// mapping for the path representation as bitvectors
		static HashMap<EdgeItem<Index, Index>*, Size> bond_to_index;
		static HashMap<Size, EdgeItem<Index, Index>*> index_to_bond;
		static std::vector<BitVector> rings;
		static std::vector<BitVector> matrix;

		/*_ function that gets a binary edge-encoded ring as a BitVector
				and adds it to the ringset if its linearly independend
		*/
		static void BalducciPearlmanRingSelector(BitVector);

		/*_ Implementation of the Balducci/Pearlman algorithm 
		*/
		void BalducciPearlmanAlgorithm(MolecularGraph& graph);
		//@}
	};
} // namespace BALL

#endif // BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
