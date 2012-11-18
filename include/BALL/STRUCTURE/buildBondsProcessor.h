// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_BUILDBONDSPROCESSOR_H
#define BALL_STRUCTURE_BUILDBONDSPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
	#include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
	#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
	#include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_KERNEL_BOND_H
	#include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
	#include <BALL/DATATYPE/options.h>
#endif

namespace BALL 
{

	/**	Bond creation processor
			\ingroup StructureMiscellaneous
	*/
	class BALL_EXPORT BuildBondsProcessor 
		: public UnaryProcessor<AtomContainer> 
	{

		public:

			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{
				/** Name to the file where the bonds lengths, max and
				 *  min bond lengths are stored in.
				 */
				static const char* BONDLENGTHS_FILENAME;
				
				/** If true, the existing bonds are deleted before 
				 *  bonds detection begins. If the atoms are in 
				 *  non-bond distance no bonds will be build! Note
				 *  that the processor cannot rebuild inter-atomcontainer
				 *  bonds, that will cause problem using it with proteins.
				 */
				static const char* DELETE_EXISTING_BONDS;

				/** If this option is set to true, the molecule
				 *  will be reprocessed. In this step the processor
				 *  tries to correct the somtimes wrong orders of 
				 *  aromatic rings.
				 */
				static const char* REESTIMATE_BONDORDERS_RINGS;

				/** If this option is set to true an additional 
				 * 	cleaning step is performed. Overpredicted bonds
				 * 	like at multiple bonds at hydrogen or halogens
				 * 	are deleted, only the shortest bond will stay.
				 */
				static const char* DELETE_OVERESTIMATED_BONDS;
			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				/// default file name for the bond lengths
				static const char* BONDLENGTHS_FILENAME;
				
				/// this option is off by default
				static const bool DELETE_EXISTING_BONDS;

				/// this option is off by default
				static const bool REESTIMATE_BONDORDERS_RINGS;

				/// this option is off by default
				static const bool DELETE_OVERESTIMATED_BONDS;
			};
			//@}
		

			/** @name	Constructors and Destructors
			*/
			//@{

			BALL_CREATE(BuildBondsProcessor);
			
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

			/// Return the bond length Hashmap 
			HashMap<Size, HashMap<Size, HashMap<int, float> > > getBondMap() { return bond_lengths_;};

			//@}

			/** @name Assignment
			*/
			//@{
			/// assignment operator
			BuildBondsProcessor& operator = (const BuildBondsProcessor& bbp);
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
			
		protected:
		
			/// builds bonds, based on atom distances read from parameter file using a 3D hash grid
			Size buildBondsHashGrid3_(AtomContainer& ac);
		
			/// after the bonds are built, the orders are estimated
			void estimateBondOrders_(AtomContainer& ac);

			/// reestimate the bond orders of rings, as aromatic rings are often detected wrong
			void reestimateBondOrdersRings_(AtomContainer& ac);

			/// deletes bonds, like from multiple bonded hydrogens or halogens
			void deleteOverestimatedBonds_(AtomContainer& ac);
			
			/// method to read the paramter file
			void readBondLengthsFromFile_(const String& file_name = "") throw(Exception::FileNotFound);
			
			/// number of bonds, which are created during the processor call
			Size num_bonds_;
		
			/// structure where bond order distances are stored in
			HashMap<Size, HashMap<Size, HashMap<int, float> > > bond_lengths_;

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
