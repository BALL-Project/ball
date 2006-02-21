// $Id: slickEnergy.h,v 1.2 2006/02/21 16:21:28 anker Exp $

#ifndef BALL_SCORING_FUNCTIONS_SLICKENERGY_H
#define BALL_SCORING_FUNCTIONS_SLICKENERGY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/SCORING/COMMON/scoringFunction.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

namespace BALL 
{
	/** SLICKEnergy scoring function
			{\bf Definition:} \URL{BALL/SCORING/FUNCTIONS/slickEnergy.h}
	*/
	class SLICKEnergy
		: public ScoringFunction
	{

		public:

		/**	@name Constant Definitions
		*/
		//@{

		/**	Option names
		*/
		struct Option
		{
			/**	The additive constant (@see Default::CONST)
			*/
			static const char* CONST;

			/**
			*/
			static const char* CHPI;

			/**
			*/
			static const char* HB;

			/**
			*/
			static const char* VDW;

			/**
			*/
			static const char* NONPOLAR;

			/**
			*/
			static const char* POLAR;

		};


		/** Default values for SLICKEnergy options.
		*/
		struct Default
		{
			/**	The additive constant (@see Default::CONST)
			*/
			static const float CONST;

			/**
			*/
			static const float CHPI;

			/**
			*/
			static const float HB;

			/**
			*/
			static const float VDW;

			/**
			*/
			static const float NONPOLAR;

			/**
			*/
			static const float POLAR;

		};



		//@}
		/** @name	Constructors and Destructors
		*/
		//@{

		BALL_CREATE(SLICKEnergy)

		/**	Default constructor.
		*/
		SLICKEnergy()
			throw();

		/** Construct a SLICKEnergy with a system and pointers to protein and
				ligand
		*/
		SLICKEnergy(System& system, Molecule& protein, Molecule& ligand)
			throw();

		/** Construct a SLICKEnergy with a system and options.
		*/
		SLICKEnergy(System& system, Molecule& protein, Molecule& ligand,
				const Options& options)
			throw();

		/**	Copy constructor
		*/
		SLICKEnergy(const SLICKEnergy& slick)
			throw();

		/** Destructor
		*/
		virtual ~SLICKEnergy()
			throw();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator.
		*/
		const SLICKEnergy& operator = (const SLICKEnergy& slick)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Setup Methods
		*/
		//@{

		/**	Force field specific setup
		*/
		virtual bool specificSetup()
			throw();

		//@}
		/**	@name Accessors specific to the SLICKEnergy scoring function
		*/
		//@{

		/**
		*/
		double getCHPIScore() const
			throw();

		/**
		*/
		double getHydrogenBondScore() const
			throw();

		/**
		*/
		double getVDWScore() const
			throw();

		/**
		*/
		double getPolarSolvationScore() const
			throw();

		/**
		*/
		double getNonpolarSolvationScore() const
			throw();
		//@}


		private:

		/*_
		*/
		void registerComponents_()
			throw();

	};

} // namespace BALL

#endif // BALL_SCORING_FUNCTIONS_SLICKENERGY_H
