// $Id: slickEnergy.h,v 1.4 2006/05/21 18:15:28 anker Exp $

#ifndef BALL_SCORING_FUNCTIONS_SLICKENERGY_H
#define BALL_SCORING_FUNCTIONS_SLICKENERGY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SCORING_FUNCTION_H
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
	class BALL_EXPORT SLICKEnergy : public ScoringFunction
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
			SLICKEnergy();

			/** Construct a SLICKEnergy with a system and pointers to protein and
					ligand
			*/
			SLICKEnergy(Molecule& protein, Molecule& ligand);

			/** Construct a SLICKEnergy with a system and options.
			*/
			SLICKEnergy(Molecule& protein, Molecule& ligand,
					Options& options);

			/**	Copy constructor
			*/
			SLICKEnergy(const SLICKEnergy& slick);

			/** Destructor
			*/
			virtual ~SLICKEnergy();

			//@}
			/**	@name Assignment
			*/
			//@{

			/**	Assignment operator.
			*/
			const SLICKEnergy& operator = (const SLICKEnergy& slick);

			/** Clear method.
			*/
			virtual void clear();

			//@}
			/**	@name	Setup Methods
			*/
			//@{

			/**
			* 	Setup of scoring function
			*/
			bool setup();

			//@}
			/**	@name Accessors specific to the SLICKEnergy scoring function
			*/
			//@{

			/**
			*/
			double getCHPIScore() const;

			/**
			*/
			double getHydrogenBondScore() const;

			/**
			*/
			double getVDWScore() const;

			/**
			*/
			double getPolarSolvationScore() const;

			/**
			*/
			double getNonpolarSolvationScore() const;
			//@}

		private:
			/**
			* Register the scoring components used by SLICKEnergy.
			*/
			void registerComponents_();
	};

} // namespace BALL

#endif // BALL_SCORING_FUNCTIONS_SLICKENERGY_H
