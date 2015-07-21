// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringPerceptionProcessor.h,v 1.17.4.2 2007/04/03 13:29:45 bertsch Exp $
//

#ifndef BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
#define BALL_QSAR_RINGPERCEPTIONPROCESSOR_H

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_STRUCTURE_SIMPLEMOLECULARGRAPH_H
#include <BALL/STRUCTURE/simpleMolecularGraph.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif


#include <stack>
#include <vector>

namespace BALL
{
/**	Processor, which marks all atoms and bonds in a ring structure with the
			Composite Property "InRing".
			calculateSSSR() can also compute the number of rings found.
			
			The processor is an implementation of the Balducci Pearlman algorithm
			described in:
			Renzo Balducci, Robert S. Pearlman, J. Chem. Inf. Comput. Sci., 34:822-831, 1994
	*/
class BALL_EXPORT RingPerceptionProcessor
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
	virtual ~RingPerceptionProcessor();

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

	/** Getter which returns all the 3 - 6 membered rings, calculateSSSR with
			 *  the Balducci-Pearlman Algorithm (defalt) is needed prior this call.
			 */
	const vector<vector<Atom*> >& getAllSmallRings() const;

	/** @name Processor-related methods
			*/
	//@{
	Processor::Result operator () (AtomContainer& ac);
	//@}

protected:

	// Balducci and Pearlman algorithm
	struct PathMessage_;

	/*_ The tnode structure described in the paper
			*/
	struct TNode_
	{
		/// method to process the messages in the recieve buffer
		void recieve();

		/// method to process the messages in the send buffer
		void send();

		bool haveZeroIntersection(BitVector& beep1, BitVector& beep2);

		bool haveSingleIntersection(BitVector& beep1, BitVector& beep2);

		bool nodeIsNew(BitVector& beep, NodeItem<Index, Index>* node);

		/// the recieve buffer, where messages are stored in
		std::vector<PathMessage_> recieve_buffer;

		/// the send buffer, where messages are stored in
		std::vector<PathMessage_> send_buffer;
	};

	/*_ The pathMsg structure described in the paper
			*/
	struct PathMessage_
	{
		void push(EdgeItem<Index, Index>* bond, TNode_* node);
		
		// path of the message
		BitVector beep;

		/// pointer to the first node this message was sent from
		TNode_* nfirst;

		// pointer to the last node of the messages' path
		TNode_* nlast;

		/// pointer to the first edge of the message path
		EdgeItem<Index, Index>* efirst;
	};

	/// mapping for internal TNode structure and the nodes of the molecular graph
	static HashMap<TNode_*, NodeItem<Index, Index>* > tnode_to_atom_;
	static HashMap<NodeItem<Index, Index>* , TNode_*> atom_to_tnode_;

	/// mapping for the path representation as bitvectors
	static HashMap<EdgeItem<Index, Index>*, Size> bond_to_index_;
	static HashMap<Size, EdgeItem<Index, Index>*> index_to_bond_;

	/// the SSSR detected by the algorithm
	static std::vector<BitVector> rings_;

	/// the matrix for the independency tests
	static std::vector<BitVector> matrix_;

	/// the rings of the ith phase, which are to be forwarded to the ring selector
	static std::vector<BitVector> forwarded_rings_;

	/// rings (beer) which have already been tested
	static std::vector<BitVector> tested_beers_;

	/// contains all 3 to 6 membered rings after the procedure of the Balducci-Pearlman algorithm
	static std::vector<std::vector<Atom*> > all_small_rings_;

	/// contains all 3 to 6 membered rings as beers
	static std::vector<BitVector> all_small_beers_;

	/*_ function that gets a binary edge-encoded ring as a BitVector
					and adds it to the ringset if its linearly independend
			*/
	static void BalducciPearlmanRingSelector_(BitVector bit_vector);
	
	/*_ Implementation of the Balducci/Pearlman algorithm
			*/
	Size BalducciPearlmanAlgorithm_(std::vector<std::vector<Atom*> >& sssr, SimpleMolecularGraph& graph);
};


/**
 * RingProcessor Exception
 *
 * It is possible that due to invalid input or the failure of the
 * algorithim, we can not assign the SSSR correctly. In such a case
 * we throw a RingProcessorException
 */
namespace Exception
{
class BALL_EXPORT RingProcessorException: public GeneralException
{
public:
	RingProcessorException(const char* file, int line)
		: GeneralException(file, line, "RingProcessorException", "")
	{
		message_ = "\nCould not find a valid set of smallest rings for an input molecule.\n\n";
		message_+= "Either the input molecule was not valid (e.g.: not a single connected component)\n";
		message_+= "or the algorithm failed on this special topology.";

		globalHandler.setMessage(message_);
	}
};
} // namespace Exception

} // namespace BALL

#endif // BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
