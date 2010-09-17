// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_AROMATICITYPROCESSOR_H
#define BALL_QSAR_AROMATICITYPROCESSOR_H

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
	#include <BALL/DATATYPE/options.h>
#endif

namespace BALL
{
	/** Aromaticity Processor 
	  	
		\brief	This processor provides methods for detection and assignment of aromaticity.

	   Processor method to detect aromaticity of AtomContainers. If it is called
		 as a processor with the operator (), the RingPerceptionProcessor is 
		 called before, to calculate a ring set. It can be called explicitely
		 with aromatize with a ringset as parameter.
	*/
	class BALL_EXPORT AromaticityProcessor
		:	public UnaryProcessor<AtomContainer>
	{
		public:

		/** @name Constant Definitions
		*/
		//@{
		/// Option names
		struct BALL_EXPORT Option
		{	
			/**	overwrite bond orders or storing the bond order as a property
			*/
		 	static const char* OVERWRITE_BOND_ORDERS;	
		};

		/// Default values for options
		struct BALL_EXPORT Default
		{
			static const bool OVERWRITE_BOND_ORDERS;
		};
		//@}

		/** @name Constructors and Destructors
		*/
	
		BALL_CREATE(AromaticityProcessor)

		//@{
		/** Default constructor
		*/		
		AromaticityProcessor();
	
		/** Copy constructor
		*/
		AromaticityProcessor(const AromaticityProcessor& aro);

		/** Destrcutor
		*/
		virtual ~AromaticityProcessor();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		AromaticityProcessor& operator = (const AromaticityProcessor& aro);
		//@}

		/** @name Public Attributes
		*/
		//@{
		/// options
		Options options;

		/** reset the options to default values
		*/
		void setDefaultOptions();
		//@}

		/** @name Accessors
		*/
		//@{

		/** Calculated the aromaticity of the molcule and sets the properties
				"IsAromatic" for aromatic atoms, and Bond::ORDER__AROMATIC for 
				bonds which are aromatic. 
				@param SSSR ring set as vector<vector<Atom*> >, (vector of rings in vector<Atom*>) 
				after the processing this variable holds all aromatic systems, which are not necessarily single rings!
		*/
  	void aromatize(const vector<vector<Atom*> >& sssr, AtomContainer& ac);

		/** Method to set aromaticity with the use of simple rules. Each ring from the given SSSR set
		 * 	(which needs just to be a set of rings) is examined for itself. (needed for the implementation
		 * 	of the MMFF94 force field which does not agree with the advanced araomtaticity definition of
		 * 	the default method.). \par
		 * 	It does not set any aromaticity flags, neither at the atoms nor bonds to bond order aromatic!
		 * 	@Param sssr is the set to be examined, only aromatic rings will stay 
		 */
		void aromatizeSimple(vector<vector<Atom*> >& sssr);
		
		//@}
		/** @name Processor-related methods
		*/
		//@{
		
		///
		Processor::Result operator () (AtomContainer& ac);
		
		///
		virtual bool start();
		//@}


		protected:

		/** @name Predicates
		*/
		//@{
		bool isValid_(const AtomContainer& ac);
		//@}


		private:

		/*_ simple criterion if a ring can be aromatic
		*/
		bool simpleCanBeAromatic_(const HashSet<Atom*>& ring);

		/*_ simple criterion if a ring can be aromatic, with weaker condition
				double bonds not need to be alternating inside the ring
		*/
		bool simpleCanBeAromaticWeaker_(const HashSet<Atom*>& ring);

		/*_ Tries to extend an aromatic system. Main method that tries to extend 
				the aromaticity to intersecting rings.
				@param SSSR ring set as vector<HashSet<Atom*> >, (vector of rings in HashSet<Atom*>)
				@param ring as HashSet<Atom*>, the ring to extended
		*/		
		void extendAromaticSystem_(vector<HashSet<Atom*> >& sssr, HashSet<Atom*> ring);
			
		/*_ Predicate that return true if the ring has conjugated double bonds. The criterion 
				is very weak, beacuse the predicate only considers carbon atoms, no hetero atoms
				are counted.
				@param ring as HashSet<Atom*>, to be tested
		*/
		bool hasConjugatedDoubleBonds_(HashSet<Atom*> ring);
			
		/*_ This mthod return the number of pi electrons in the ring (aromatic system)
				@param ring as HashSet<Atom*>, from which the number of pi electrons to count
		*/
		Size countPiElectrons_(HashSet<Atom*>& ring);

		/** Should we set the bond orders to aromatic or merely store the information as a property of the bond?
		 *  This value depends on the value of the option OVERWRITE_BOND_ORDERS 
		 */
		bool overwrite_bond_orders_;
	};
} // namespace BALL

#endif // BALL_QSAR_AROMATICITYPROCESSOR_H
