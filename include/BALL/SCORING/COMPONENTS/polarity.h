// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMPONENTS_POLARITY_H
#define BALL_SCORING_COMPONENTS_POLARITY_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/SCORING/COMPONENTS/fresnoTypes.h>


namespace BALL
{
	/** Fresno buried polar component.
			{\bf Definition:} \URL{BALL/DOCKING/SCORING_COMPONENTS/buriedPolar.h}
	*/
	class BALL_EXPORT Polarity
		:	public ScoringComponent
	{
		public:
			static const double POL_ES_THRESHOLD;
			static const double LIP_ES_THRESHOLD;

			/** @name	Constructors and Destructors
			*/
			//@{

			/**	Constructor.
			*/
			Polarity(ScoringFunction& sf);

			/**	Copy constructor
			*/
			Polarity(const Polarity& bp);

			/**	Destructor.
			*/
			virtual ~Polarity();

			//@}
			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
			*/
			const Polarity& operator = (const Polarity& bp);

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

			Size getType_(const Atom* atom);

			bool isPolar_(const Atom* atom);

			bool isLipophilic_(const Atom* atom);

			bool isBackboneAtom_(const Atom* atom);

			FresnoTypes* receptor_fresno_types_;
			FresnoTypes* ligand_fresno_types_;
	};
} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_POLARITY_H
