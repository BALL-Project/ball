// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef BALL_QSAR_SIMPLEDESCRIPTORS_H
#define BALL_QSAR_SIMPLEDESCRIPTORS_H

#ifndef BALL_QSAR_SIMPLEBASE_H
#	include <BALL/QSAR/simpleBase.h>
#endif

namespace BALL
{
	// 40 simple descriptors

	/** This descriptor calculates the molecular weight
	*/
	class MolecularWeight
		:	public SimpleBase
	{
		public:

		BALL_CREATE(MolecularWeight)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/		
		MolecularWeight();

		/** Copy constructor
		*/
		MolecularWeight(const MolecularWeight& mw);

		/** Destructor
		*/
		~MolecularWeight();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		MolecularWeight& operator = (const MolecularWeight& mw);
		//@}
	};

	/** This descriptor counts the number of bonds of the 
			molecule.
	*/
	class NumberOfBonds
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default Constructor
		*/		
		NumberOfBonds();

		/** Copy constructor
		*/
		NumberOfBonds(const NumberOfBonds& nb);

		/** Destructor
		*/
		~NumberOfBonds();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfBonds& operator = (const NumberOfBonds& nb);
		//@}
	};

	/** This descriptor counts the number of single bonds of the
			molecule. 
	*/
	class NumberOfSingleBonds
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfSingleBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfSingleBonds();

		/** Copy constructor
		*/
		NumberOfSingleBonds(const NumberOfSingleBonds& nsb);

		/** Destructor
		*/
		~NumberOfSingleBonds();

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfSingleBonds& operator = (const NumberOfSingleBonds& nsb);
		//@}
	};

	/** This descriptor counts the number of double bonds of 
			the molecule
	*/
	class NumberOfDoubleBonds
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfDoubleBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfDoubleBonds();

		/** Copy constructor
		*/
		NumberOfDoubleBonds(const NumberOfDoubleBonds& ndb);

		/** Destructor
		*/
		~NumberOfDoubleBonds();
		//@}

		/** @name Assigenment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfDoubleBonds& operator = (const NumberOfDoubleBonds& ndb);
		//@}
	};

	/** This descriptor counts the number of triple bonds of
			the molecule
	*/
	class NumberOfTripleBonds
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfTripleBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfTripleBonds();

		/** Copy constructor
		*/
		NumberOfTripleBonds(const NumberOfTripleBonds& ntb);

		/** Destructor
		*/
		~NumberOfTripleBonds();
		//@}

		/** @name Assignment 
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfTripleBonds& operator = (const NumberOfTripleBonds& ntb);
		//@}		
	};

	/** This descriptor counts the number of aromatic bonds.
	*/
	class NumberOfAromaticBonds
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfAromaticBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfAromaticBonds();

		/** Copy constructor
		*/
		NumberOfAromaticBonds(const NumberOfAromaticBonds& nab);

		/** Destructor
		*/
		~NumberOfAromaticBonds();
		//@}

		/** @name Assigment 
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfAromaticBonds& operator = (const NumberOfAromaticBonds& nab);
		//@}
	};

	/** This descriptor counts the number of aromatic atoms
	*/
	class NumberOfAtoms
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfAtoms)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfAtoms();

		/** Copy constructor
		*/
		NumberOfAtoms(const NumberOfAtoms& na);

		/** Destructor
		*/
		~NumberOfAtoms();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfAtoms& operator = (const NumberOfAtoms& na);
		//@}
	};

	/** This descriptor counts the number of boron
	*/
	class NumberOfBoron
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfBoron)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfBoron();
		
		/** Copy constructor
		*/
		NumberOfBoron(const NumberOfBoron& nb);

		/** Destructor
		*/
		~NumberOfBoron();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfBoron& operator = (const NumberOfBoron& nb);
		//@}
	};

	/** This descriptor counts the number of carbon
	*/
	class NumberOfCarbon
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfCarbon)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfCarbon();

		/** Copy constructor
		*/
		NumberOfCarbon(const NumberOfCarbon& nc);

		/** Destructor
		*/
		~NumberOfCarbon();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfCarbon& operator = (const NumberOfCarbon& nc);
		//@}
	};

	/** This descriptor counts the number of nitrogen
	*/
	class NumberOfNitrogen
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfNitrogen)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfNitrogen();

		/** Copy constructor
		*/
		NumberOfNitrogen(const NumberOfNitrogen& nn);

		/** Destructor
		*/
		~NumberOfNitrogen();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment Operator
		*/
		NumberOfNitrogen& operator = (const NumberOfNitrogen& nn);
		//@}
	};

	/** This descriptor counts the number of oxygen
	*/
	class NumberOfOxygen
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfOxygen)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default construcotr
		*/
		NumberOfOxygen();

		/** Copy constructor
		*/
		NumberOfOxygen(const NumberOfOxygen& no);

		/** Destructor
		*/
		~NumberOfOxygen();

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfOxygen& operator = (const NumberOfOxygen& no);
		//@}
	};

	/** This descriptor counts the number of flourine
	*/
	class NumberOfFlourine
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfFlourine)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfFlourine();

		/** Copy contructor
		*/
		NumberOfFlourine(const NumberOfFlourine& nf);

		/** Destructor
		*/
		~NumberOfFlourine();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfFlourine& operator = (const NumberOfFlourine& nf);
		//@}
	};

	/** This descriptor counts the number of phosphorus
	*/
	class NumberOfPhosphorus
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfPhosphorus)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfPhosphorus();

		/** Copy constructor
		*/
		NumberOfPhosphorus(const NumberOfPhosphorus& np);
		
		/** Destructor
		*/
		~NumberOfPhosphorus();
		//@}
	
		/** @name Assignment
		*/
		//7
		/** Assignment operator
		*/
		NumberOfPhosphorus& operator = (const NumberOfPhosphorus& np);
		//@}
	};

	/** This descriptor counts the number of sulfur.
	*/
	class NumberOfSulfur
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfSulfur)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfSulfur();

		/** Copy constructor
		*/
		NumberOfSulfur(const NumberOfSulfur& ns);

		/** Destructor
		*/
		~NumberOfSulfur();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfSulfur& operator = (const NumberOfSulfur& ns);
		//@}
	};

	/** This descriptor counts the number of chlorine.
	*/
	class NumberOfChlorine
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfChlorine)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfChlorine();

		/** Copy Constructor
		*/
		NumberOfChlorine(const NumberOfChlorine& nc);

		/** Destructor
		*/
		~NumberOfChlorine();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfChlorine& operator = (const NumberOfChlorine& nc);
		//@}
	};

	/** This descriptor counts the number of bromine.
	*/
	class NumberOfBromine
		: public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfBromine)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default construcor
		*/
		NumberOfBromine();

		/** Copy constructor
		*/
		NumberOfBromine(const NumberOfBromine& nb);

		/** Destructor
		*/
		~NumberOfBromine();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfBromine& operator = (const NumberOfBromine& nb);
		//@}
	};

	/** This descriptor counts the number of iodine.
	*/
	class NumberOfIodine
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfIodine)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfIodine();

		/** Copy constructor
		*/
		NumberOfIodine(const NumberOfIodine& ni);

		/** Destructor
		*/
		~NumberOfIodine();
		//@}

		/** @name Assigenment 
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfIodine& operator = (const NumberOfIodine& ni);
		//@}
	};

	/** This descriptor counts the number of hydrogen
	*/
	class NumberOfHydrogen
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfHydrogen)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfHydrogen();

		/** Copy constructor
		*/
		NumberOfHydrogen(const NumberOfHydrogen& nh);

		/** Destructor
		*/
		~NumberOfHydrogen();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfHydrogen& operator = (const NumberOfHydrogen& nh);
		//@}
	};

	/** This descriptor adds up the formal charges.
	*/
	class FormalCharge
		:	public SimpleBase
	{
		public:

		BALL_CREATE(FormalCharge)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		FormalCharge();

		/** Copy constructor
		*/
		FormalCharge(const FormalCharge& fc);

		/** Destructor
		*/
		~FormalCharge();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		FormalCharge& operator = (const FormalCharge& fc);
		//@}		
	};

	/** This descriptor counts the heavy atoms, heavy atoms
			are atoms with an atomic number strictly greater than
			1 (all but hydrgen).
	*/
	class NumberOfHeavyAtoms
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfHeavyAtoms)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfHeavyAtoms();

		/** Copy constructor
		*/
		NumberOfHeavyAtoms(const NumberOfHeavyAtoms& nha);

		/** Destructor
		*/
		~NumberOfHeavyAtoms();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfHeavyAtoms& operator = (const NumberOfHeavyAtoms& nha);
		//@}
	};

	/** Atom information content (mean). This is the entropy of the 
			element distribution in the molecule (including implicit hydrogen). 
			Let $n_i$ be the number of occurences of atomic number $i$ in the 
			molecule. Let $p_i = \frac{n_i}{\sum n_i}$. The value of this 
			descriptor is the negative of the sum over all $i$ of 
			$p_i \cdot \log p_i$.
	*/	
	class MeanAtomInformationContent
		:	public SimpleBase
	{
		public:

		BALL_CREATE(MeanAtomInformationContent)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		MeanAtomInformationContent();

		/** Copy constructor
		*/
		MeanAtomInformationContent(const MeanAtomInformationContent& maic);
		
		/** Destructor
		*/
		~MeanAtomInformationContent();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		MeanAtomInformationContent& operator = (const MeanAtomInformationContent& maic);
		//@}
	};

	/** This descriptor is $n$ times MeanAtomInformationContent.
	*/
	class AtomInformationContent
		:	public SimpleBase
	{
		public:

		BALL_CREATE(AtomInformationContent)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default Constructor
		*/
		AtomInformationContent();
		
		/** Copy constructor
		*/
		AtomInformationContent(const AtomInformationContent& aic);

		/** Destructor
		*/
		~AtomInformationContent();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		AtomInformationContent& operator = (const AtomInformationContent& aic);
		//@}
		
		/** @name Accessors
		*/
		//@{
		double compute(Molecule& molecule);
		//@}
	};

	/** This descriptor counts the number of single bonds which are
			rotatable. A single bond is rotatable if it is a heavy bond,
			not in a ring and bounds not a lone atom like halogens.
	*/
	class NumberOfRotatableSingleBonds
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfRotatableSingleBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfRotatableSingleBonds();

		/** Copy constructor
		*/
		NumberOfRotatableSingleBonds(const NumberOfRotatableSingleBonds& nrsb);

		/** Destructor
		*/
		~NumberOfRotatableSingleBonds();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfRotatableSingleBonds& operator = (const NumberOfRotatableSingleBonds& nrsb);
		//@}
	};

	/** This descriptor counts the fraction of the rotatble single 
			bonds. This is the number of rotatble single bonds devided 
			by the number of single bonds.
	*/
	class RelNumberOfRotatableSingleBonds
		:	public SimpleBase
	{
		public:

		BALL_CREATE(RelNumberOfRotatableSingleBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		RelNumberOfRotatableSingleBonds();

		/** Copy constructor
		*/
		RelNumberOfRotatableSingleBonds(const RelNumberOfRotatableSingleBonds& rnrsb);

		/** Destructor
		*/
		~RelNumberOfRotatableSingleBonds();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		RelNumberOfRotatableSingleBonds& operator = (const RelNumberOfRotatableSingleBonds& rnrsb);
		//@}

		/** Accessors
		*/
		//@{
		double compute(Molecule& molecule);
		//@}
	};

	/** This descriptor counts the number of aromatic atoms, a atom 
			is aromatic if it participates a aromatic bond.
	*/
	class NumberOfAromaticAtoms
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfAromaticAtoms)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfAromaticAtoms();

		/** Copy constructor
		*/
		NumberOfAromaticAtoms(const NumberOfAromaticAtoms& naa);

		/** Destructor
		*/
		~NumberOfAromaticAtoms();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment Operator
		*/
		NumberOfAromaticAtoms& operator = (const NumberOfAromaticAtoms& naa);
		//@}
	};

	/** This descriptor counts the number of heavy bonds, a bond is
			heavy if both participating atoms are heavy.
	*/
	class NumberOfHeavyBonds
		:	public SimpleBase
	{
		public:	

		BALL_CREATE(NumberOfHeavyBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default Constructors
		*/
		NumberOfHeavyBonds();

		/** Copy constructors
		*/
		NumberOfHeavyBonds(const NumberOfHeavyBonds& nhb);

		/** Destructor
		*/
		~NumberOfHeavyBonds();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfHeavyBonds& operator = (const NumberOfHeavyBonds& nhb);
		//@}
	};
	
	/** This descriptor counts the number of rotatable bonds, a bond 
			ia rotatble if it is heavy, not in a ring and not bounds 
			lone atoms like halogens.
	*/
	class NumberOfRotatableBonds
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfRotatableBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfRotatableBonds();

		/** Copy constructor
		*/
		NumberOfRotatableBonds(const NumberOfRotatableBonds& nrb);
		
		/** Destructors
		*/
		~NumberOfRotatableBonds();

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfRotatableBonds& operator = (const NumberOfRotatableBonds& nrb);
		//@}
	};

	/** This descriptor returns the fraction of rotatble bonds. It is
			calculated by deviding the number of rotatable bonds by the 
			number of bonds
	*/
	class RelNumberOfRotatableBonds
		:	public SimpleBase
	{
		public:

		BALL_CREATE(RelNumberOfRotatableBonds)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		RelNumberOfRotatableBonds();

		/** Copy constructor
		*/
		RelNumberOfRotatableBonds(const RelNumberOfRotatableBonds& rnrb);

		/** Destructor
		*/
		~RelNumberOfRotatableBonds();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		RelNumberOfRotatableBonds& operator = (const RelNumberOfRotatableBonds& rnrb);
		//@}

		/** @name Accessors
		*/
		//@{
		double compute(Molecule& molecule);
		//@}
	};

	/** The vertex adjacency is calculated using the formula
			\( 1 + \log_2 m \), where $m$ is the number of heavy 
			bonds.
	*/
	class VertexAdjacency
		:	public SimpleBase
	{
		public:

		BALL_CREATE(VertexAdjacency)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		VertexAdjacency();

		/** Copy constructor
		*/
		VertexAdjacency(const VertexAdjacency& va);

		/** Destructor
		*/
		~VertexAdjacency();
		//@}
		
		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		VertexAdjacency& operator = (const VertexAdjacency& va);
		//@}

		/** @name Accessors
		*/
		//@{
		double compute(Molecule& molecule);
		//@}
	};

	/** The vertex adjacency equality si calculateed using the
			formula: \(-(1-f) \log_2(1-f) - f \log_2 f  \), where
			$f$ is defined as:
			\( f = \frac{(n^2 -2m)}{n^2}\), where $n$ is the number
			of heavy atoms and $m$ is the number of heavy bonds. If
			$f$ is not in $(0,1)$ then $0$ is returned.
	*/
	class VertexAdjacencyEquality
		:	public SimpleBase
	{
		public:

		BALL_CREATE(VertexAdjacencyEquality)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		VertexAdjacencyEquality();

		/** Copy constructor
		*/
		VertexAdjacencyEquality(const VertexAdjacencyEquality& vae);

		/** Destructor
		*/
		~VertexAdjacencyEquality();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		VertexAdjacencyEquality& operator = (const VertexAdjacencyEquality& vae);
		//@}

		/** @name Accessors
		*/
		//@{
		double compute(Molecule& molecule);
		//@}
	};

	/** This descriptor returns the number of hydrogen bonds acceptor atoms.
			If other set of atoms should be considered as hydrogen bond acceptors
			then this can be set via setExpression with an Expression string as 
			parameter.
	*/
	class NumberOfHydrogenBondAcceptors
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfHydrogenBondAcceptors)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfHydrogenBondAcceptors();

		/** Copy constructor
		*/
		NumberOfHydrogenBondAcceptors(const NumberOfHydrogenBondAcceptors& nhba);

		/** Expression constructor
		*/
		NumberOfHydrogenBondAcceptors(const String& expression);

		/** Destructor
		*/
		~NumberOfHydrogenBondAcceptors();
		//@}

		/** Accessors
		*/
		//@{
		/*_ Getter which returns the Expression string
		*/
		const String& getExpression() const;
		
		/*_ Setter which sets the Expression string
		*/
		void setExpression(const String& expression);

		/*_ This method return the number, which is calculated
				using the Expression string.
		*/
		double compute(Molecule& molecule);
		//@}


		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfHydrogenBondAcceptors& operator = (const NumberOfHydrogenBondAcceptors& nhba);
		//@}


		private:
		
		/*_ String which holds the expression.
		*/
		String expression_;
	};

	/** This descriptor returns the number of the hydrogen bond donors of the
			molecule. If another set of atoms want to be used, it can be set via
			\texttt{setExpression}, with an Expression string as parameter.
	*/
	class NumberOfHydrogenBondDonors
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfHydrogenBondDonors)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NumberOfHydrogenBondDonors();

		/** Copy constructor
		*/
		NumberOfHydrogenBondDonors(const NumberOfHydrogenBondDonors& nhbd);

		/** Expression constructor
		*/
		NumberOfHydrogenBondDonors(const String& expression);

		/** Destructor
		*/
		~NumberOfHydrogenBondDonors();
		//@}

		/** @name Accessors
		*/
		//@{
		/*_ Setter which sets the Expression string
		*/
		void setExpression(const String& expression);

		/*_ Getter which returns the Expression string
		*/
		const String& getExpression() const;

		/*_ Methods which returns the number calcualted from 
				the Expression string.
		*/
		double compute(Molecule& molecule);
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfHydrogenBondDonors& operator = (const NumberOfHydrogenBondDonors& nhbd);
		//@}


		private:

		/*_ String which holds the expression.
		*/
		String expression_;
	};
	
	/** This descriptor return the number of hydrophobic atoms. If another set of
			hydrophobic atoms should be considered, another expression can be set via
			setExpression, with an Expression string as parameter.
	*/
	class NumberOfHydrophobicAtoms
		:	public SimpleBase
	{
		public:

		BALL_CREATE(NumberOfHydrophobicAtoms)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructur
		*/
		NumberOfHydrophobicAtoms();

		/** Copy constructor
		*/
		NumberOfHydrophobicAtoms(const NumberOfHydrophobicAtoms& nha);

		/** Expression constructor
		*/
		NumberOfHydrophobicAtoms(const String& expression);

		/** Destructor
		*/
		~NumberOfHydrophobicAtoms();
		//@}

		/** @name Acessors
		*/
		//@{
		/*_ Getter which returns the Expression string
		*/
		const String& getExpression() const;

		/*_ Setter which set the Expression string
		*/
		void setExpression(const String& expression);

		/*_ Method which computes the number from the 
				Expression string 
		*/
		double compute(Molecule& molecule);
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		NumberOfHydrophobicAtoms& operator = (const NumberOfHydrophobicAtoms& nha);
		//@}


		private:
		
		/*_ String which holds the expression
		*/
		String expression_;
	};

	/** This descriptor is calculated by adding up the
			differences in atomic polarizabilities of the both
			participating atoms through all bonds.
	*/
	class BondPolarizabilities
		:	public SimpleBase
	{
		public:

		BALL_CREATE(BondPolarizabilities)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		BondPolarizabilities();

		/** Copy constructor
		*/
		BondPolarizabilities(const BondPolarizabilities& bp);

		/** Destructor
		*/
		~BondPolarizabilities();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		BondPolarizabilities& operator = (const BondPolarizabilities& bp);
		//@}
	};

	/** This descriptor adds up the atomic polarizabilities
			for all atoms.
	*/
	class AtomicPolarizabilities
		:	public SimpleBase
	{
		public:

		BALL_CREATE(AtomicPolarizabilities)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		AtomicPolarizabilities();

		/** Copy constructors
		*/
		AtomicPolarizabilities(const AtomicPolarizabilities& ap);

		/** Destructor
		*/
		~AtomicPolarizabilities();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		AtomicPolarizabilities& operator = (const AtomicPolarizabilities& ap);
		//@}		
	};

	/** This descriptor returns the number of rings in the SSSR
	*/
	class SizeOfSSSR
		:	public SimpleBase
	{
		public:

		BALL_CREATE(SizeOfSSSR)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		SizeOfSSSR();

		/** Copy constructor
		*/
		SizeOfSSSR(const SizeOfSSSR& sizeofsssr);

		/** Destructor
		*/
		~SizeOfSSSR();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment Operator
		*/
		SizeOfSSSR& operator = (const SizeOfSSSR& sssr);
		//@}		
	};

	/** This descriptor returns the principle moment of inertia
			calculated from the filed coordinates!
	*/
	class PrincipalMomentOfInertia
		:	public SimpleBase
	{
		public:

		BALL_CREATE(PrincipalMomentOfInertia)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		PrincipalMomentOfInertia();

		/** Copy constructor
		*/
		PrincipalMomentOfInertia(const PrincipalMomentOfInertia& pmi);

		/** Destrcutor
		*/
		~PrincipalMomentOfInertia();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		PrincipalMomentOfInertia& operator = (const PrincipalMomentOfInertia& pmi);
		//@}
	};

	/** This descripor adds up the x-axis component of the principal 
			moment of inertia over all atoms, calculated from the filed
			coordinates!
	*/
	class PrincipalMomentOfInertiaX
		:	public SimpleBase
	{
		public:

		BALL_CREATE(PrincipalMomentOfInertiaX)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		PrincipalMomentOfInertiaX();

		/** Copy constructor
		*/
		PrincipalMomentOfInertiaX(const PrincipalMomentOfInertiaX& pmix);

		/** Destructor
		*/
		~PrincipalMomentOfInertiaX();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		PrincipalMomentOfInertiaX& operator = (const PrincipalMomentOfInertiaX& pmix);
		//@}
	};

	/** This descriptor adds up the y-axis component of the principal 
			moment of inertia over all atoms, calculated from the filed coordinates!
	*/
	class PrincipalMomentOfInertiaY
		:	public SimpleBase
	{
		public:

		BALL_CREATE(PrincipalMomentOfInertiaY)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		PrincipalMomentOfInertiaY();

		/** Copy constructor
		*/
		PrincipalMomentOfInertiaY(const PrincipalMomentOfInertiaY& pmiy);

		/** Destructor
		*/
		~PrincipalMomentOfInertiaY();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		PrincipalMomentOfInertiaY& operator = (const PrincipalMomentOfInertiaY& pmiy);
		//@}
	};

	/** This descriptor adds up the z-axis component of the principal
			moment of interia over all atoms, calculated from the filed
			coordinates!
	*/
	class PrincipalMomentOfInertiaZ
		:	public SimpleBase
	{
		public:

		BALL_CREATE(PrincipalMomentOfInertiaZ)

		/** @name Constructor and Destrcutors
		*/
		//@{
		/** Default constructor
		*/
		PrincipalMomentOfInertiaZ();

		/** Copy constructor
		*/
		PrincipalMomentOfInertiaZ(const PrincipalMomentOfInertiaZ& pmiz);

		/** Destructor
		*/
		~PrincipalMomentOfInertiaZ();
		//@}
		
		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		PrincipalMomentOfInertiaZ& operator = (const PrincipalMomentOfInertiaZ& pmiz);
		//@}
	};

} // namespace BALL

#endif // BALL_QSAR_SIMPLEDESCRIPTORS_H

