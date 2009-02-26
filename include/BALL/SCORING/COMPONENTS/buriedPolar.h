// $Id: buriedPolar.h,v 1.3 2006/05/21 17:38:39 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#ifndef BALL_SCORING_COMPONENTS_FRESNOBURIEDPOLAR_H
#define BALL_SCORING_COMPONENTS_FRESNOBURIEDPOLAR_H

#include <BALL/SCORING/COMMON/scoringComponent.h>

namespace BALL
{

	/** Fresno buried polar component.
			{\bf Definition:} \URL{BALL/SCORING/COMPONENTS/buriedPolar.h}
	*/
	class BuriedPolar
		:	public ScoringComponent
	{

		public:

		/**	Option names
		*/
		struct Option
		{

			/**
			*/
			static const char* BP_R1_OFFSET;

			/**
			*/
			static const char* BP_R2_OFFSET;

			/**
			*/
			static const char* CREATE_INTERACTIONS_FILE;

			/**
			*/
			static const char* VERBOSITY;

		};

		/** Default values for SLICK options.
		*/
		struct Default
		{

			/**
			*/
			static const float BP_R1_OFFSET;

			/**
			*/
			static const float BP_R2_OFFSET;

			/**
			*/
			static const bool CREATE_INTERACTIONS_FILE;

			/**
			*/
			static const Size VERBOSITY;

		};



		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		BuriedPolar()
			;

		/**	Constructor.
		*/
		BuriedPolar(ScoringFunction& sf)
			;

		/**	Copy constructor
		*/
		BuriedPolar(const BuriedPolar& bp)
			;

		/**	Destructor.
		*/
		virtual ~BuriedPolar()
			;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const BuriedPolar& operator = (const BuriedPolar& bp)
			;

		/** Clear method.
		*/
		virtual void clear()
			;

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const BuriedPolar& bp) const
			;

		//@}
		/**	@name	Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup()
			;

		//@}
		/**	@name	Accessors	
		*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual double calculateScore()
			;

		//@}

		private:

		/*_
		*/
		std::vector< std::pair<const Atom*, const Atom*> > possible_buried_polar_interactions_;

		/*_
		*/
		double factor_;

		/*_ This length will be added to the sum of the van-der-Waals radii for
				obtaining the lower bound of the scoring function.
		*/
		double r1_offset_;

		/*_ The upper bound for the scoring function is obtained by adding a
				constant to the lower bound.
		*/
		double r2_offset_;

		// The following two variables are only necessary if the buried polar
		// component is meant to assign its own radii. They are unused at the
		// moment.

		///
		Molecule* bp_receptor_;

		///
		Molecule* bp_ligand_;

		///
		bool write_interactions_file_;

		///
		Size verbosity_;

	};

} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_FRESNOBURIEDPOLAR_H
