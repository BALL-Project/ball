// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_PARTIALCHARGEPROCESSOR_H
#define BALL_QSAR_PARTIALCHARGEPROCESSOR_H

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/hashSet.h>

#include <queue>
#include <vector>
#include <map>

namespace BALL
{
	/**	Generic .
			{\bf Definition:}\URL{BALL/include/BALL/QSAR/ringFinder.h}
			\\
	*/
	class PartialChargeProcessor
    : public UnaryProcessor<Molecule>
	{
		public:

		BALL_CREATE(PartialChargeProcessor)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		PartialChargeProcessor();
	
		/** Copy contructor
		*/
		PartialChargeProcessor(const PartialChargeProcessor& pc);

		/** Destructor
		*/
		~PartialChargeProcessor();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		PartialChargeProcessor& operator = (const PartialChargeProcessor& pc);
		//@}
		
		/** @name Accessors
		*/
		//@{
		/** Methods to calculate the PEOE charges of Gasteiger. This method
				can be called explicitely, not as processor.
		*/
  	void calculatePEOE(Molecule& molecule);
		//@}

		/** @name Processor-realted methods
		*/
		//@{		
    virtual Processor::Result operator () (Molecule& molecule);
		//@}

		private:

    /*_ @name Accessors
    */

		std::pair<std::vector<float>, std::vector<float> > readIonizationEnergies_();

		
		//@{
		/*_ Helper function to get the ionization energy for an element
		*/
		float getIonizationEnergy_(Element::AtomicNumber atomic_number, Size charge);


		std::vector<float> readElectronAffinities_();

		/*_ Helper funtion to get the electron affinity of an element
		*/
		float getElectronAffinity_(Element::AtomicNumber atomic_number, Size charge);

		/*_ Helper function to get the PEOE a,b and c factor
		*/
		void getabcFactors_(Atom* atom, float& a, float& b, float& c);
		//@}		

		HashSet<Element::AtomicNumber> warned_elements_;
	};
} // namespace BALL

#endif // BALL_QSAR_RINGFINDER_H
