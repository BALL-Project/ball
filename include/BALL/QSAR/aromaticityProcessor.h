// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_AROMATICITYPROCESSOR_H
#define BALL_QSAR_AROMATICITYPROCESSOR_H

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

//#include <queue>

namespace BALL
{
	/**	Processor method to detect aromaticity of molecules. If it is called
			as a processor with the operator (), the RingPerceptionProcessor is 
			called before, to calculate a ring set. It can be called explicitely
			with aromatize with a ringset as parameter.
			\\
	*/
	class AromaticityProcessor
		:	public UnaryProcessor<Molecule>
	{
		public:

		BALL_CREATE(AromaticityProcessor)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/		
		AromaticityProcessor();
	
		/** Copy constructor
		*/
		AromaticityProcessor(const AromaticityProcessor& aro);

		/** Destrcutor
		*/
		~AromaticityProcessor();
		//@}
	
		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		AromaticityProcessor& operator = (const AromaticityProcessor& aro);
		//@}
	
		/** @name Accessors
		*/
		//@{
		/** Calculated the aromaticity of the molcule and sets the properties
				"IsAromatic" for aromatic atoms, and Bond::ORDER__AROMATIC for 
				bonds which are aromatic. 
				@param SSSR ring set as vector<vector<Atom*> >, (vector of rings in vector<Atom*>)
		*/
  	void aromatize(vector<vector<Atom*> >& sssr, Molecule& molecule);
		//@}

		/** @name Processor-related methods
		*/
		//@{
		Processor::Result operator () (Molecule& molecule);
		//@}


		protected:

		/** @name Predicates
		*/
		//@{
		bool isValid(const Molecule& molecule);
		//@}


		private:

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
	};
} // namespace BALL

#endif // BALL_QSAR_AROMATICITYPROCESSOR_H
