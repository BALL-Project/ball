// $Id: slick.h,v 1.1 2005/11/21 19:27:06 anker Exp $

#ifndef BALL_SCORING_FUNCTIONS_SLICK_H
#define BALL_SCORING_FUNCTIONS_SLICK_H

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
	/** SLICK scoring function
			{\bf Definition:} \URL{BALL/SCORING/FUNCTIONS/slick.h}
	*/
	class SLICK
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
			static const char* VDW;

			/**
			*/
			static const char* CHPI;

			/**
			*/
			static const char* NONPOLAR;

			/**
			*/
			static const char* POLAR;

		};


		/** Default values for SLICK options.
		*/
		struct Default
		{
			/**	The additive constant (@see Default::CONST)
			*/
			static const float CONST;

			/**
			*/
			static const float VDW;

			/**
			*/
			static const float CHPI;

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

		BALL_CREATE(SLICK)

		/**	Default constructor.
		*/
		SLICK()
			throw();

		/** Construct a SLICK with a system and pointers to protein and
				ligand
		*/
		SLICK(System& system, Molecule& protein, Molecule& ligand)
			throw();

		/** Construct a SLICK with a system and options.
		*/
		SLICK(System& system, Molecule& protein, Molecule& ligand,
				const Options& options)
			throw();

		/**	Copy constructor
		*/
		SLICK(const SLICK& slick)
			throw();

		/** Destructor
		*/
		virtual ~SLICK()
			throw();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator.
		*/
		const SLICK& operator = (const SLICK& slick)
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
		/**	@name Accessors specific to the SLICK scoring function
		*/
		//@{

		/**
		*/
		double getVDWScore() const
			throw();

		/**
		*/
		double getCHPIScore() const
			throw();

		/**
		*/
		double getPolarSolvationScore() const
			throw();

		/**
		*/
		double getNonpolarSolvationScore() const
			throw();


		private:

		/*_
		*/
		void registerComponents_()
			throw();

	};

} // namespace BALL

#endif // BALL_SCORING_FUNCTIONS_SLICK_H
