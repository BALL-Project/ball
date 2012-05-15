#ifndef BALL_STRUCTURE_CONNECTEDCOMPONENTSPROCESSOR_H
#define BALL_STRUCTURE_CONNECTEDCOMPONENTSPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
# include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

namespace BALL
{
	/** Connected Component Processor
	    \ingroup StructureMiscellaneous
	*/

	/** Computation of the connected components of the molecular graph.
	 */
	class BALL_EXPORT ConnectedComponentsProcessor
		: public UnaryProcessor<AtomContainer>
	{
		public:
		/** @name Type definitions
		 */
		//@{
		typedef std::vector<Atom*>      Component;
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

		/** @name Accessors.
		*/
		//@{

		/// Return the number of connected components found.
		Size getNumberOfConnectedComponents();

		/// Return the vector of connected components.
		ComponentVector& getComponents();

		/// Return the vector of connected components, const variant.
		const ComponentVector& getComponents() const;

		/** Split a System into its connected components.
				
				This function inserts one \link Molecule Molecule \endlink per connected component
				into to_split. The Atoms will be re-parented and ripped out of their previous parents. 
				<b>Note</b> that this destroys any further hierarchical organization (e.g. Chains, Fragments, 
				...) that was previously present.

				This function will apply the connected component computation itself, so it is not
				necessary to call apply() previously.

				@param to_split the System to split
				@return Size - the number of connected components
		*/
		Size splitIntoMolecules(System& to_split);

		//@}

		protected:
			ComponentVector components_;
	};
}

#endif // BALL_STRUCTURE_CONNECTEDCOMPONENTSPROCESSOR_H
