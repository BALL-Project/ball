/* lipophilic.h
*
* Copyright (C) 2011 Marcel Schumann
*
* This program free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMPONENTS_LIPOPHILIC_H
#define BALL_SCORING_COMPONENTS_LIPOPHILIC_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/SCORING/COMPONENTS/fresnoTypes.h>


namespace BALL
{
	/** Fresno lipophilic component */
	class BALL_EXPORT Lipophilic  :  public ScoringComponent
	{
		public:
			/**	Option names
			*/
			struct Option
			{
				/**
				*/
				static const char* LIPO_R1_OFFSET;

				/**
				*/
				static const char* LIPO_R2_OFFSET;

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
				static const float LIPO_R1_OFFSET;

				/**
				*/
				static const float LIPO_R2_OFFSET;

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

			/**	Constructor.
			*/
			Lipophilic(ScoringFunction& sf);

			/**	Copy constructor
			*/
			Lipophilic(const Lipophilic& li);

			/**	Destructor.
			*/
			virtual ~Lipophilic();

			//@}
			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
			*/
			const Lipophilic& operator = (const Lipophilic& li)
				throw();

			/** Clear method.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Predicates.
			*/
			//@{

			bool operator == (const Lipophilic& li) const
				throw();

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

			void update(const AtomPairVector& pair_vector);

			/**	Calculates and returns the component's energy.
			*/
			double updateScore();

			//@}

		private:
			/*_
			*/
			AtomPairVector possible_lipophilic_interactions_;

			/*_ This length will be added to the sum of the van-der-Waals radii for
					obtaining the lower bound of the scoring function.
			*/
			double r1_offset_;

			/*_ The upper bound for the scoring function is obtained by adding a
					constant to the lower bound.
			*/
			double r2_offset_;

			///
			bool write_interactions_file_;

			///
			Size verbosity_;

			bool isTypeKnown(Atom* atom);
			Size getType(Atom* atom);

			FresnoTypes* receptor_fresno_types_;
			FresnoTypes* ligand_fresno_types_;
	};
} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_LIPOPHILIC_H
