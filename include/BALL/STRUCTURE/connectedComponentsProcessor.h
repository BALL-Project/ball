#ifndef BALL_STRUCTURE_CONNECTEDCOMPONENTSPROCESSOR_H
#define BALL_STRUCTURE_CONNECTEDCOMPONENTSPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
# include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#include <BALL/KERNEL/molecule.h>
#endif

#ifndef BOOST_PENDING_DISJOINT_SETS_HPP
#include <boost/pending/disjoint_sets.hpp>
#endif
namespace BALL
{
	/** Connected Component Processor
	    \ingroup StructureMiscellaneous
	*/

	/** \brief Computation of the connected components of the molecular graph.
	 * 
	 * The processor can be applied to all instances of \link AtomContainer 
	 * AtomContainer \endlink to group the contained atoms to their connected 
	 * component. This is done according to the <b>intra bonds</b> assoziated 
	 * with each atom. Thus this enables to identify sub-groups of <b>covalently 
	 * connected atoms</b> in the atom container. This is especially useful when 
	 * a molecule entry actually contains more than one independent molecule 
	 * (like ions plus a drug molecule).
	 */
	class BALL_EXPORT ConnectedComponentsProcessor
		: public UnaryProcessor<AtomContainer>
	{
		public:
		/** @name Type definitions
		 */
		//@{
		typedef std::vector<Atom*>      Component;
		typedef std::vector<Molecule>   MolVec;
		typedef	std::vector<Component>  ComponentVector;
		//@}

		BALL_CREATE(ConnectedComponentsProcessor);


		/** @name	Constructors and Destructors
		 */
		//@{

		/// Default Constructor
		ConnectedComponentsProcessor();

		/// Destructor
		virtual ~ConnectedComponentsProcessor();
		//@}

		/**	@name	Processor-related methods 
		*/
		//@{

		/// Processor method which is called before the operator()-call.
		virtual bool start();

		/** Clears the data structures.
		*/
		void clear();

		/** Operator () for the processor 
		 */
		virtual Processor::Result operator ()(AtomContainer& ac);

		/// Processor method which is called after the operator()-call.
		virtual bool finish();

		//@}

		/** @name Accessors and convenience methods.
		*/
		//@{

		/// Returns the number of connected components found.
		Size getNumberOfConnectedComponents();
		
		/** Returns a copy of the protected member "components_" in 'comp', if
		 *  the processor was applied to any atom container before. comp then
		 *  contains all connected molecules/components of that molecule.
		 * 
		 * The contained Component entries are vectors of atom pointers. Any 
		 * previously contained element in comp will be discarded.
		 * 
		 * The result will again contain all atoms of the molecule the processor
		 * was applied to before.
		 * 
		 * @param comp - will contain all found components
		 *
		*/
		void getComponents(ComponentVector &comp);
		
		/** Convenience method. Inserts the atoms of the largest molecule into
		 * a input of type \link Molecule Molecule \endlink. <b>Note:</b> thus that 
		 * input normally needs to be empty
		 * (or at least free of other atom entries) otherwise the atoms are simply 
		 * added to the pre-existing set. Properties of the original molecule
		 * are NOT transfered to the output molecule.
		 * 
		 * @param result - atoms that belong to the larges contained molecule will 
		 *								 be inserted into result
		*/
		void getLargestComponent(Molecule &result);
		
		/** Convenience method. Inserts elements of type \link Molecule Molecule 
		 * \endlink that have at least a minimum  number of atoms into the input 
		 * vector. Properties of the original molecule are NOT transfered to the 
		 * output molecules.
		 * 
		 * @param result - \link Molecule Molecule \endlink's that have the minimum 
		 *								 number of atoms will be inserted here
		 * @param min - minimal amount of atoms that are needed for a molecule to 
		 *							be selected as result
		*/
		void getMinAtomsComponents(MolVec &result, size_t min);
		
		/** Convenience method. Inserts all identified \link Molecule Molecule 
		 * \endlink's into the input vector. Properties of the original molecule
		 * are NOT transfered to the output molecules.
		 * 
		 * @param result - All real \link Molecule Molecule \endlink's will be 
		 *								 inserted here
		*/
		void getAllComponents(MolVec& results);
		//@}

		protected:
			typedef boost::disjoint_sets < int*, int*,
							boost::find_with_full_path_compression > DisjointSet;
			ComponentVector components_;
	};
}

#endif // BALL_STRUCTURE_CONNECTEDCOMPONENTSPROCESSOR_H
