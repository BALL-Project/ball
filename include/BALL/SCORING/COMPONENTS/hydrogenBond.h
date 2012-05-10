/* hydrogenBond.h
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

#ifndef BALL_SCORING_COMPONENTS_HYDROGENBOND_H
#define BALL_SCORING_COMPONENTS_HYDROGENBOND_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/SCORING/COMPONENTS/fresnoTypes.h>

namespace BALL
{
	/** Fresno Hydrogen Bond component.
	*/
	class BALL_EXPORT HydrogenBond : public ScoringComponent
	{
		public:
			struct Option
			{
				/**
				*/
				static const char* HB_IDEAL_LENGTH;

				/**
				*/
				static const char* HB_IDEAL_ANGLE;

				/**
				*/
				static const char* HB_DIST_LOWER;

				/**
				*/
				static const char* HB_DIST_UPPER;

				/**
				*/
				static const char* HB_ANG_LOWER;

				/**
				*/
				static const char* HB_ANG_UPPER;

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
				static const float HB_IDEAL_LENGTH;

				/**
				*/
				static const float HB_IDEAL_ANGLE;

				/**
				*/
				static const float HB_DIST_LOWER;

				/**
				*/
				static const float HB_DIST_UPPER;

				/**
				*/
				static const float HB_ANG_LOWER;

				/**
				*/
				static const float HB_ANG_UPPER;

				/**
				*/
				static const Size VERBOSITY;
			};

			enum Mode
			{
				LIGAND_HYDROGENS,
				RECEPTOR_HYDROGENS,
				ALL_HYDROGENS
			};

			//BALL_CREATE(HydrogenBond)

			/** @name	Constructors and Destructors
			*/
			//@{

			/**	Default constructor.
			*/
			HydrogenBond(Mode mode)
				throw();

			/**	Constructor.
			*/
			HydrogenBond(ScoringFunction& sf, Mode mode)
				throw();

			/**	Constructor.
			*/
			HydrogenBond(ScoringComponent& sc, Mode mode)
				throw();

			/**	Copy constructor
			*/
			HydrogenBond(const HydrogenBond& fhb)
				throw();

			/**	Destructor.
			*/
			virtual ~HydrogenBond()
				throw();

			//@}
			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
			*/
			const HydrogenBond& operator = (const HydrogenBond& fhb)
				throw();

			/** Clear method.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Predicates. */
			//@{

			bool operator == (const HydrogenBond& fhb) const
				throw();

			//@}
			/**	@name	Setup Methods
			*/
			//@{

			/**	Setup method.
			*/
			virtual bool setup();

			void setupLigand();

			/** If charge-evaluation is enabled, hydrogen bonds with large charge difference between hydrogen bond acceptor and hydrogen will receive a better score than those with a small difference.\n
			By default, this is disabled. */
			void enableChargeEvaluation(bool b);
			//@}

			/**	@name	Accessors
			*/
			//@{

			virtual double updateScore();

			/** Update possible_hydrogen_bonds_ from the given AtomPairVector.\n
			If intermolecular H-bonds are to be evaluated, the _first_ atom of each pair must be a ligand atom and the second one a receptor atom. This is automatically done this way by ScoringFunction::createNonbondedPairVector(). */
			void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);
			//@}

		private:
			/*_ This vector contains all possible hydrogen bonds. It is created
			* during each call of update(). Pairs are always of the form (hydrogen,
			* acceptor). The donor can be found easily by following the only bond
			* of the hydrogen.
			*/
			std::vector< std::pair<const Atom*, const Atom*> > possible_hydrogen_bonds_;

			double factor_;

			double h_bond_distance_lower_;

			double h_bond_distance_upper_;

			double h_bond_angle_lower_;

			double h_bond_angle_upper_;

			/*_ The ideal hbond length.
			*/
			double ideal_hbond_length_;

			/*_ The ideal hbond angle.
			*/
			double ideal_hbond_angle_;

			Size verbosity_;

			Size getType(Atom* atom);

			FresnoTypes* receptor_fresno_types_;
			FresnoTypes* ligand_fresno_types_;

			Mode mode_;

			bool charge_evaluation_enabled_;
	};
}

#endif // BALL_SCORING_COMPONENTS_HYDROGENBOND_H
