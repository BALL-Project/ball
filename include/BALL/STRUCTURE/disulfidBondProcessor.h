#ifndef BALL_STRUCTURE_DISULFIDBONDPROCESSOR_H
#define BALL_STRUCTURE_DISULFIDBONDPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
# include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

namespace BALL
{
	/** Disulfid Bond Processor
	    \ingroup StructureMiscellaneous
	*/

	/** Computation of disulfid bonds of the atom container.
	 */
	class BALL_EXPORT DisulfidBondProcessor
		: public UnaryProcessor<AtomContainer>
	{
		public:
		/** @name Type definitions
		 */
		//@{
		typedef std::pair<Atom*, Atom*>    DisulfidBond;
		typedef	std::vector<DisulfidBond>  DisulfidBondVector;
		//@}

		BALL_CREATE(DisulfidBondProcessor);


		/** @name	Constructors and Destructors
		 */
		//@{

		/// Default Constructor
		DisulfidBondProcessor();

		/// Destructor
		virtual ~DisulfidBondProcessor();
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

		/// Return the number of created disulfid bonds.
		Size getNumberOfCreatedDisulfidBonds();

		/// Return the vector of computed disulfid bonds.
		DisulfidBondVector& getCreatedDisulfidBonds();

		/// Return the vector of created disulfid bonds, const variant.
		const DisulfidBondVector& getCreatedDisulfidBonds() const;

		//@}

		/// Connect two sulfurs by disulid bond
		bool connect(Atom* atom1, Atom* atom2);

		/// Connect two residues by disulid bond
		bool connect(Residue* residue1, Residue* residue2);

		/// Connect two AtomContainers by disulid bond
		bool connect(Composite* composite1, Composite* composite2);

		protected:
			DisulfidBondVector  bonds_;
	};
}

#endif // BALL_STRUCTURE_DISULFIDBONDPROCESSOR_H
