// -*- Mode: C++; tab-width: 2: -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_SIMPLEBASE_H
#define BALL_QSAR_SIMPLEBASE_H

#ifndef BALL_KERNEL_MOLECULE_H
#include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_QSAR_DESCRIPTOR_H
#include <BALL/QSAR/descriptor.h>
#endif
#include <vector>

namespace BALL
{
	/** QSAR molecular simple descriptors base class, this
			class implements some methods to calculate simple
			descriptors.
			\\
	*/
	class SimpleBase
		:	public Descriptor
	{
		public:
	
		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		SimpleBase();

		/** Copy constructor
		*/
		SimpleBase(const SimpleBase& sb);

		/** Named constructor
		*/
		SimpleBase(const String& name);

		/** Named unit constructor
		*/
		SimpleBase(const String& name, const String& unit);

		/** Destructor
		*/
		virtual ~SimpleBase();
		//@}
		
		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		virtual SimpleBase& operator = (const SimpleBase& sb);
		//@}


		protected:

		/** @name Predicates
		*/
		//@{
		/*_ Returns true if the data is calculated already, and the data is still correct,
				otherwise false is returned
				@param molecule to examine
		*/
		bool isValid(Molecule& molecule);
		//@}

		/** @name Accessors
		*/
		//@{
		/*_ Performs the calculation of some of the simple descriptors.
		*/
		void calculate(Molecule& molecule);
		//@}
		
		
		private:
		
		/** @name Accessors
		*/
		
		//@{
		/*_ This method calculates the pmi of the x, y and z axis, return the pmi 
				@param referenced double which holds the pmi x component after calculation
				@param referenced double which holds the pmi y component after calculation
				@param referenced double which holds the pmi z component after calculation
				@param molecule from which the pmi is calculated
		*/
		double calcPrincipalMomentOfInertia_(double& pmi_x, double& pmi_y, double& pmi_z, Molecule& molecule);
		
		/*_ Helper function that reads the atomic polarizabilities 
				from a file from the data section of BALL. It is called from
				getAtomicPolarizability_ and reads into a static variable, hence
				it is read one time per instance.
		*/
		std::vector<float> readAtomicPolarizabilities_();

		/*_ Method which returns the atomic polarizability of the element given as parameter
				@param the atomic number of the element
		*/
		float getAtomicPolarizability_(int atomic_number);
		//@}

		/** @name Predicates
		*/
		//@{
		/*_ Predicate that returns true if the atom is a carbon and in an
				exclusively aromatic or aliphatic surrounding, means is only
				bound to carbon an hydrogen!
		*/
		bool isHydrophobic_(const Atom* atom) const;
		//@}
	};
} // namespace BALL

#endif
