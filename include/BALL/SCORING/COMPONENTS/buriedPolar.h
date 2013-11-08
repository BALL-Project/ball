// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMPONENTS_BURIEDPOLAR_H
#define BALL_SCORING_COMPONENTS_BURIEDPOLAR_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/SCORING/COMPONENTS/fresnoTypes.h>


namespace BALL
{
	/** Fresno buried polar component.
		{\bf Definition:} \URL{BALL/DOCKING/SCORING_COMPONENTS/buriedPolar.h}
	*/
	class BALL_EXPORT BuriedPolar
		:	public ScoringComponent
	{
		public:
			/**	Option names
			*/
			struct Option
			{
				static const char* BP_R1_OFFSET;
				static const char* BP_R2_OFFSET;
				static const char* VERBOSITY;
			};

			/** Default values for SLICK options.
			*/
			struct Default
			{
				static const float BP_R1_OFFSET;
				static const float BP_R2_OFFSET;
				static const Size VERBOSITY;
			};

			/** @name	Constructors and Destructors
			*/
			//@{

			/**	Constructor.
			*/
			BuriedPolar(ScoringFunction& sf);

			/**	Copy constructor
			*/
			BuriedPolar(const BuriedPolar& bp);

			/**	Destructor.
			*/
			virtual ~BuriedPolar();

			//@}
			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
			*/
			const BuriedPolar& operator = (const BuriedPolar& bp);

			/** Clear method.
			*/
			void clear();

			//@}
			/**	@name	Setup Methods
			*/
			//@{

			/**	Setup method.
			*/
			bool setup();

			void setupLigand();

			//@}
			/**	@name	Accessors
			*/
			//@{

			/**	Calculates and returns the component's energy.
			*/
			double updateScore();

			void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);
			//@}

		private:
			AtomPairVector possible_buried_polar_interactions_;

			double factor_;

			/*_ This length will be added to the sum of the van-der-Waals radii for
					obtaining the lower bound of the scoring function.
			*/
			double r1_offset_;

			/*_ The upper bound for the scoring function is obtained by adding a
					constant to the lower bound.
			*/
			double r2_offset_;

			Size verbosity_;

			Size getType(Atom* atom);

			bool isBackboneAtom(const Atom* atom);

			FresnoTypes* receptor_fresno_types_;
			FresnoTypes* ligand_fresno_types_;
	};
} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_BURIEDPOLAR_H
