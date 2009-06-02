// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_PARTIALCHARGEPROCESSOR_H
#define BALL_QSAR_PARTIALCHARGEPROCESSOR_H

#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/hashSet.h>

#include <vector>
#include <utility>

namespace BALL
{
	class AtomContainer;
	class Atom;

	/**	Generic .
			\\
	*/
	class BALL_EXPORT PartialChargeProcessor
    : public UnaryProcessor<AtomContainer>
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
		virtual ~PartialChargeProcessor();
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
  		void calculatePEOE(AtomContainer& ac);
	
		/** allows to set the data-folder without using BALL_DATA_PATH enviroment variable, which is useful for standalone applications */
		void setDataFolder(const char* folder);
		//@}

		/** @name Processor-realted methods
		*/
		//@{		
    virtual Processor::Result operator () (AtomContainer& ac);
		//@}

		private:

    /*_ @name Accessors
    */

		std::pair<std::vector<float>, std::vector<float> > readIonizationEnergies_();

		
		//@{
		/*_ Helper function to get the ionization energy for an element
		*/
		float getIonizationEnergy_(Element::AtomicNumber atomic_number, Size charge);


		void readElectronAffinities_(std::vector<float>& electron_affinities);

		/*_ Helper funtion to get the electron affinity of an element
		*/
		float getElectronAffinity_(Element::AtomicNumber atomic_number, Size charge);

		/*_ Helper function to get the PEOE a,b and c factor
		*/
		void getabcFactors_(Atom* atom, float& a, float& b, float& c);
		//@}		

		HashSet<Element::AtomicNumber> warned_elements_;
		
		String data_folder_;
	};
} // namespace BALL

#endif // BALL_QSAR_RINGFINDER_H
