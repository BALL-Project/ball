// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_SURFACEDESCRIPTORS_H
#define BALL_QSAR_SURFACEDESCRIPTORS_H

#ifndef BALL_QSAR_SURFACEBASE_H
# include <BALL/QSAR/surfaceBase.h>
#endif

namespace BALL
{
	// 15 surface descriptors 

	/** This descriptor calculates the sum of atomic SAS
			van der Waals surface areas of the molecule, 
			where the atoms	have a positive partial charge.			
	*/
	class BALL_EXPORT PositiveVdWSurface
		: public SurfaceBase
	{
		public:

		BALL_CREATE(PositiveVdWSurface);
		
		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		PositiveVdWSurface();

		/** Copy constrcutor
		*/
		PositiveVdWSurface(const PositiveVdWSurface& ps);
		
		/** Destructor
		*/
		virtual ~PositiveVdWSurface();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		PositiveVdWSurface& operator = (const PositiveVdWSurface& ps);
		//@}		
	};

	/** This descriptor calculates the sum of atomic SAS
			van der Waals surface areas of the molecule, 
			where the atoms	have a negative partial charge.
	*/
	class BALL_EXPORT NegativeVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(NegativeVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NegativeVdWSurface();

		/** Copy constructor
		*/
		NegativeVdWSurface(const NegativeVdWSurface& ns);

		/** Destructor
		*/
		virtual ~NegativeVdWSurface();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment Operator
		*/
		NegativeVdWSurface& operator = (const NegativeVdWSurface& ns);
		//@}
	};

	/** This descriptor calculates the sum of atomic SAS
			van der Waals surface areas of the molecule,
			where the partial charge of the atom is 
			positive and the atom is a polar atom.
	*/
	class BALL_EXPORT PositivePolarVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(PositivePolarVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		PositivePolarVdWSurface();

		/** Copy constructor
		*/
		PositivePolarVdWSurface(const PositivePolarVdWSurface& pps);

		/** Desctructor
		*/
		virtual ~PositivePolarVdWSurface();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		PositivePolarVdWSurface& operator = (const PositivePolarVdWSurface& pps);
		//@}
	};

	/** This descriptor calculates the sum of atomic SAS
			van der Waals surface areas of the molecule,
			where the partial charge of the atom is 
			negative and the atom is a polar atom.
	*/
	class BALL_EXPORT NegativePolarVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(NegativePolarVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		NegativePolarVdWSurface();

		/** Copy constructor
		*/
		NegativePolarVdWSurface(const NegativePolarVdWSurface& nps);
		
		/** Destructor
		*/
		virtual ~NegativePolarVdWSurface();
		//@}

		/** @name Assigment
		*/
		//@{
		/** Assignment operator
		*/
		NegativePolarVdWSurface& operator = (const NegativePolarVdWSurface& nps);
		//@}
	};

	/** This descriptor calculates the sum of atomic SAS
			van der Waals surface areas of the molecule,
			of the hydrophobic atoms.
	*/
	class BALL_EXPORT HydrophobicVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(HydrophobicVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		HydrophobicVdWSurface();

		/** Copy constructor
		*/
		HydrophobicVdWSurface(const HydrophobicVdWSurface& hs);

		/** Destructor
		*/
		virtual ~HydrophobicVdWSurface();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		HydrophobicVdWSurface& operator = (const HydrophobicVdWSurface& hs);
		//@}
	};

	/** This descriptor calculates the sum of atomic SAS
			van der Waals surface areas of the molecule,
			of the polar atoms.
	*/
	class BALL_EXPORT PolarVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(PolarVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default Constructor
		*/
		PolarVdWSurface();

		/** Copy constructor
		*/
		PolarVdWSurface(const PolarVdWSurface& ps);
		
		/** Destructor
		*/
		virtual ~PolarVdWSurface();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		PolarVdWSurface& operator = (const PolarVdWSurface& ps);
		//@}
	};

	/** This descriptor returns the fraction of the atomic
			SAS van der Waals surface, where the partial 
			charge is positive. It is calculated by deviding
			the positive van der Waals area by the molecules
			van der Waals surface area.
	*/
	class BALL_EXPORT RelPositiveVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(RelPositiveVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		RelPositiveVdWSurface();
		
		/** Copy constructor
		*/
		RelPositiveVdWSurface(const RelPositiveVdWSurface& rps);
		
		/** Destructor
		*/
		virtual ~RelPositiveVdWSurface();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assginment operator
		*/
		RelPositiveVdWSurface& operator = (const RelPositiveVdWSurface& rps);
		//@}
		
		/** @name Accessors
		*/
		//@{
		double compute(AtomContainer& ac);
		//@}
	};

	/** This descriptor returns the fraction of the atomic
			SAS van der Waals surface, where the partial
			charge is negative. It is calculated by deviding
			the negative van der Waals area by the molecules
			van der Waals surface area.
	*/
	class BALL_EXPORT RelNegativeVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(RelNegativeVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default Constructor
		*/		
		RelNegativeVdWSurface();

		/** Copy Constructor
		*/
		RelNegativeVdWSurface(const RelNegativeVdWSurface& rns);
		
		/** Destructor
		*/
		virtual ~RelNegativeVdWSurface();
		//q}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		RelNegativeVdWSurface& operator = (const RelNegativeVdWSurface& rns);
		//@}

		/** @name Accessors
		*/
		//@{
		double compute(AtomContainer& ac);
		//@}
	};

	/** This descriptor returns the fraction of the atomic
			SAS van der Waals surface, where the partial 
			charge is positive and the atoms are polar.
			It is calculated by deviding the positive polar
			van der Waals surface by the van der Waals surface
			area of the molecule
	*/
	class BALL_EXPORT RelPositivePolarVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(RelPositivePolarVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		RelPositivePolarVdWSurface();

		/** Copy constructor
		*/
		RelPositivePolarVdWSurface(const RelPositivePolarVdWSurface& rpps);

		/** Destructor
		*/
		virtual ~RelPositivePolarVdWSurface();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		RelPositivePolarVdWSurface& operator = (const RelPositivePolarVdWSurface& rpps);
		//@}

		/** @name Accessors
		*/
		//@{
		double compute(AtomContainer& ac);
		//@}
	};

	/** This descriptor returns the fraction of the atomic
			SAS van der Waals surface, where the partial
			charge is negative and the atoms are polar.
			It is calculated by deviding the negative polar
			van der Waals surface by the van der Waals surface
			areas of the molecule.
	*/
	class BALL_EXPORT RelNegativePolarVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(RelNegativePolarVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/		
		RelNegativePolarVdWSurface();

		/** Copy constructor
		*/
		RelNegativePolarVdWSurface(const RelNegativePolarVdWSurface& rnps);

		/** Destructor
		*/
		virtual ~RelNegativePolarVdWSurface();
		//@}
		
		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		RelNegativePolarVdWSurface& operator = (const RelNegativePolarVdWSurface& rnps);
		//@}

		/** @name Accessors
		*/
		//@{
		double compute(AtomContainer& ac);
		//@}
	};

	/** This descriptor returns the fraction of the atomic
			SAS van der Waals surface, where the atoms
			are hydrophobic. It is calculated by deviding
			the hydrophobic van der Waals surface by the 
			van der Waals surface of the molecule.
	*/
	class BALL_EXPORT RelHydrophobicVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(RelHydrophobicVdWSurface)

		/** Constructors and Desctructors
		*/
		//@{
		/** Default constructor
		*/
		RelHydrophobicVdWSurface();

		/** Copy Constructor
		*/
		RelHydrophobicVdWSurface(const RelHydrophobicVdWSurface& rhs);

		/** Destructor
		*/
		virtual ~RelHydrophobicVdWSurface();
		//@}

		/** @name Assignment 
		*/
		//@{
		/** Assignment operator
		*/
		RelHydrophobicVdWSurface& operator = (const RelHydrophobicVdWSurface& rhs);
		//@}
		
		/** @name Accessors
		*/
		//@{
		double compute(AtomContainer& ac);
		//@}
	};

	/** This descriptor returns the fraction of the atomic
			SAS van der Waals surface, where the atoms 
			are polar. It is calculated by deviding the 
			polar van der Waals surface by the van der Waals
			surface of the molecule.
	*/
	class BALL_EXPORT RelPolarVdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(RelPolarVdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default Constructor
		*/
		RelPolarVdWSurface();

		/** Copy constructor
		*/
		RelPolarVdWSurface(const RelPolarVdWSurface& rps);

		/** Destructor
		*/
		virtual ~RelPolarVdWSurface();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		RelPolarVdWSurface& operator = (const RelPolarVdWSurface& rps);
		//@}

		/** @name Accessors
		*/
		//@{
		double compute(AtomContainer& ac);
		//@}
	};

	/** This descriptor calculates the van
			der Waals volume of the molecule
	*/
	class BALL_EXPORT VdWVolume
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(VdWVolume)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/		
		VdWVolume();

		/** Copy constructor
		*/
		VdWVolume(const VdWVolume& vol);

		/** Destructor
		*/
		virtual ~VdWVolume();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		VdWVolume& operator = (const VdWVolume& vol);
		//@}
	};

	/** This descriptor calculates the density of
			the molecule, by deviding the molecular 
			weight by the van der Waals surface area.
	*/
	class BALL_EXPORT Density
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(Density)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/		
		Density();

		/** Copy constructor
		*/
		Density(const Density& rho);

		/** Destructor
		*/
		virtual ~Density();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignmenr operator
		*/
		Density& operator = (const Density& rho);
		//@}
	};

	/** This descriptor calculates the van 
			der Waals surface area of the molecule.
	*/
	class BALL_EXPORT VdWSurface
		:	public SurfaceBase
	{
		public:

		BALL_CREATE(VdWSurface)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/		
		VdWSurface();

		/** Copy constructor
		*/
		VdWSurface(const VdWSurface& s);
		
		/** Destructor
		*/
		virtual ~VdWSurface();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		VdWSurface& operator = (const VdWSurface& s);
		//@}
	};

} // namespace BALL

#endif // BALL_QSAR_SURFACEDESCRIPTORS_H

