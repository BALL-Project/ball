// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: buildBondsProcessor.h,v 1.3 2005/02/20 21:36:31 bertsch Exp $
//

/** TODO
	* - parameter section for:
	*		o hash grid size parameter
	*		o mass defect parameter
	*		o #atoms-threshold for switch hash grid/ simple version 
	*			(or skip simple version?)
	*	- how to deal with existing bonds? option for complete rebuild?
	* - method to reestimate bond orders of rings (due to bad aromatic
	*		bond order detection, bc it is hard to distinguish between 
	*		double and aromatic bonds)
	*	-	complete the test methods
	*/


#ifndef BALL_STRUCTURE_BUILDBONDSPROCESSOR_H
#define BALL_STRUCTURE_BUILDBONDSPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
	#include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
	#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_KERNEL_PTE_H
	#include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
	#include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_KERNEL_BOND_H
	#include <BALL/KERNEL/bond.h>
#endif

#include <vector>
#include <utility>

#define BALL_STRUCTURE_BUILDBONDSPROCESSOR_DEFAULT_FILE "bond_lengths/bond_lengths.db"

namespace BALL 
{

	/**	Bond creation processor
	*/
	class BuildBondsProcessor 
		: public UnaryProcessor<AtomContainer> 
	{

		public:

			/** @name	Constructors and Destructors
			*/
			//@{
			///	default constructor
			BuildBondsProcessor();
		
			/// copy construcor
			BuildBondsProcessor(const BuildBondsProcessor& bbp);
		
			/// constructor with parameter filename
			BuildBondsProcessor(const String& file_name) throw(Exception::FileNotFound);
			
			/// destructor
			virtual ~BuildBondsProcessor();
			//@}

			/**	@name	Processor-related methods 
			*/
			//@{
			/// processor method which is called after the operator () call
			virtual bool finish();

			/// processor method which is called before the operator () call
			virtual bool start();

			/// operator () for the processor
			virtual Processor::Result operator () (AtomContainer& ac);
			//@}

			/**	@name	Accessors
			*/
			//@{
			/// Return the number of bonds built during the last application.
			Size getNumberOfBondsBuilt();

			/// sets the parameters file
			void setBondLengths(const String& file_name) throw(Exception::FileNotFound);
			//@}

			/** @name Assignment
			*/
			//@{
			/// assignment operator
			BuildBondsProcessor& operator = (const BuildBondsProcessor& bbp);
			//@}

		protected:
		
			/// builds bonds, based on atom distances read from parameter file using a 3D hash grid
			Size buildBondsHashGrid3_(AtomContainer& ac);
		
			/// builds bonds, based on atom distances read from parameter file; simply iterates over atom pairs
			Size buildBondsSimple_(AtomContainer& ac);

			/// after the bonds are built, the orders are estimated
			void estimateBondOrders_(AtomContainer& ac);
			
			/// method to read the paramter file
			void readBondLengthsFromFile_(const String& file_name = "") throw(Exception::FileNotFound);
			
			/// number of bonds, which are created during the processor call
			Size num_bonds_;
		
			/// structure where bond order distances are stored in
			HashMap<Size, HashMap<Size, HashMap<Bond::BondOrder, float> > > bond_lengths_;

			/// structure were the bond maxima stored in (used in buildBonds_)
			HashMap<Size, HashMap<Size, float> > max_bond_lengths_;

			/// structure were the bond minima stored in (used in buildBonds_)
			HashMap<Size, HashMap<Size, float> > min_bond_lengths_;

			/*_ returns the best fitting bond order of a bond between atoms of
					element e1 and element e2 with a distance of length
			*/
			Bond::BondOrder getNearestBondOrder_(float length, Size e1, Size e2);

			/*_ Returns true if the atom with atomic number an1 and atom with
					atomic number an2 can share a bond. If, the parameter length
					holds the maximal length of such a bond.
			*/
			bool getMaxBondLength_(float& length, Size an1, Size an2);

			/*_ Returns true if the atom with atomic number an1 and atom with
					atomic number an2 can share a bond. If, the parameter length
					holds the minimal length of such a bond.
			*/
			bool getMinBondLength_(float& length, Size an1, Size an2);

			/// parameter which holds the longest possible bond
			float max_length_;
			
	};

} // namespace BALL 


#endif // BALL_STRUCTURE_BUILDBONDSPROCESSOR_H
