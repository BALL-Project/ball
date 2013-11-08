// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMMON_SCORINGCOMPONENT_H
#define BALL_SCORING_COMMON_SCORINGCOMPONENT_H

#include <BALL/DATATYPE/string.h>
#include <BALL/SCORING/COMMON/baseFunction.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>

#include <BALL/KERNEL/atom.h>


namespace BALL
{
	class ScoringFunction;

	class BALL_EXPORT ScoringComponent
	{
		public:

			ScoringComponent();

			ScoringComponent(const ScoringComponent& sc);

			ScoringComponent(ScoringFunction& sf);

			/** select the base function (necessary only if the specific ScoringComponent uses it). Possible choices are 'fermi' and 'linear'. */
			void selectBaseFunction(String function);

			virtual ~ScoringComponent();

			virtual void clear();

			virtual bool setup();

			/** Function that needs to be called once for every new ligand.\n
			The default implementation of this base class does nothing and should be overloaded by derived classes if necessary. */
			virtual void setupLigand();

			ScoringFunction* getScoringFunction() const;

			void setScoringFunction(ScoringFunction& sf);

			String getName() const;

			void setName(const String& name);

			const String& getTypeName();

			//virtual double getScore() const;

			double getRawScore() const;

			double getScaledScore() const;

			/** Update this ScoringComponent using the given atom-pairs. \n
			This function should be overloaded by all ScoringComponents */
			virtual void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);

			/** Calculate the score for this component (for all interactions that have been set by the last call of update()) and return the score.\n
			The coefficient assigned by the user to this component should be taken into account for the calculation, so that the weighted score is returned. */
			virtual double updateScore();

			virtual void setLigandIntraMolecular(bool b);

			/** Is this ScoringComponent to be used to calculate an intra-molecular score for the ligand ? */
			bool isLigandIntraMolecular();

			/** Can the scores of this ScoringFunction be precomputed for a grid? */
			bool isGridable();

			/** Does this component calculate the score based on atom-atom pairs? */
			bool isAtomPairwise();

			void setCoefficient(const double& coeff);

			const double& getCoefficient();

			void setNormalizationParameters(double stddev, double mean);

			void getNormalizationParameters(double& stddev, double& mean);

			bool isEnabled();

			void enable();

			void disable();

		protected:
			/**
			 * Is this ScoringComponent to be used to calculate an intra-molecular score for the ligand?
			 */
			bool ligand_intra_molecular_;

			/**
			 * Can the scores of this ScoringFunction be precomputed for a grid?
			 */
			bool gridable_;

			/**
			 * Does this component calculate the score based on atom-atom pairs?
			 */
			bool atom_pairwise_;

			ScoringFunction* scoring_function_;

			ScoringBaseFunction* base_function_;

			double score_;

			double coefficient_;

			double stddev_;
			double mean_;

			/**
			 * Multiplies the score_ with coefficient_ and, if necessary, does transformation and backtransformation.
			 */
			double scaleScore(double score) const;

			/**
			 * Determines whether this ScoringComponent is enabled and should be used when ScoringFunction::update()/updateScore() is called.
			 */
			bool enabled_;

			/**
			 * An abreviation for the type of this ScoringComponent, i.e.:
			 * 'ES', 'vdW', 'fragSolv', 'nRot', 'HB'.\n
			 * This type-name cannot be changed dynmically and is used to differentiate between the differnt types of ScoringComponents as is necessary
			 * e.g. for PharmacophoreConstraints.
			 */
			String type_name_;

		private:
			/**
			 * A name for this individual ScoringComponent. It can be set by use of setName().
			 */
			String name_;
	};
}

#endif // BALL_SCORING_COMMON_SCORINGCOMPONENT_H
