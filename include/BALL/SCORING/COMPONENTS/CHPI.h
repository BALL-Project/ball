/* CHPI.h
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

#ifndef BALL_SCORING_COMPONENTS_CHPI_H
#define BALL_SCORING_COMPONENTS_CHPI_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>

namespace BALL
{
	/** SLICK ringstacking component.
		{\bf Definition:} \URL{BALL/SCORING/COMPONENTS/CHPI.h}
	*/
	class BALL_EXPORT CHPI : public ScoringComponent
	{
		public:
			/// A class for storing aromatic rings and information associated with
			/// them. At the moment we only need the ring centres and the atoms.
			class AromaticRing
			{
				public:
					///
					AromaticRing()
						throw();

					///
					AromaticRing(const AromaticRing& aromatic_ring)
						throw();

					///
					AromaticRing(const std::vector<Atom*>& atoms)
						throw();

					///
					const Vector3& getCentre() const
						throw();

					///
					const Vector3& getNormalVector() const
						throw();

					///
					void setRing(const std::vector<Atom*>& atoms)
						throw();

					// accessors for the private data members
					const std::vector<Atom*>& getRing() const
						throw();

					///
					void dump(std::ostream& s = std::cout) const
						throw();

					/** Recalculates ring-center and normal-vector. Use this function if the parent-molecule has been moved or changed */
					void update();

				private:
					//_ A hash set containing pointers to all ring atoms:
					std::vector<Atom*> ring_atoms_;

					//_ The centre of the aromatic ring.
					Vector3 centre_;

					//_ The normal vector of the ring plane.
					Vector3 normal_vector_;

					//_
					void computeCentre_()
						throw();

					//_
					void computeNormalVector_()
						throw();
			};

			/// A class for storing aliphatic CH groups suitable for building
			/// CH-\pi interactions with aromatic rings.
			class CHGroup
			{
				public:
					///
					CHGroup()
						throw();

					///
					~CHGroup()
						throw();

					///
					CHGroup(const CHGroup& CH_groups)
						throw();

					///
					CHGroup (const Atom* C_atom, const Atom* H_atom)
						throw();

					///
					void setCAtom(const Atom* C_atom)
						throw();

					///
					void setHAtom(const Atom* H_atom)
						throw();

					///
					void setAtoms(const Atom* C_atom, const Atom* H_atom)
						throw();

					///
					const Atom* getHAtom() const
						throw()
					{
						return(H_atom_);
					}

					///
					const Atom* getCAtom() const
						throw()
					{
						return(C_atom_);
					}

					void dump(std::ostream& s = std::cout) const
						throw();

				private :
					//_
					const Atom* H_atom_;

					//_
					const Atom* C_atom_;
			};

			struct Option
			{
				///
				static const String VERBOSITY;

				///
				static const String CX_DISTANCE_UPPER;

				///
				static const String CHX_ANGLE_LOWER;

				///
				static const String HX_PROJECTED_DISTANCE_LOWER;

				///
				static const String HX_PROJECTED_DISTANCE_UPPER;

				///
				static const String DISTANCE_TOLERANCE;

				///
				static const String ANGLE_TOLERANCE;

				///
				static const String LIMIT;

				///
				static const String CREATE_INTERACTION_FILE;
			};

			struct Default
			{
				///
				static const Size VERBOSITY;

				///
				static const float CX_DISTANCE_UPPER;

				///
				static const float CHX_ANGLE_LOWER;

				///
				static const float HX_PROJECTED_DISTANCE_LOWER;

				///
				static const float HX_PROJECTED_DISTANCE_UPPER;

				///
				static const float DISTANCE_TOLERANCE;

				///
				static const float ANGLE_TOLERANCE;

				///
				static const float LIMIT;

				///
				static const bool CREATE_INTERACTION_FILE;
			};

			/** @name	Constructors and Destructors
			*/
			//@{

			/**	Default constructor.
			*/
			CHPI()
				throw();

			/**	Constructor.
			*/
			CHPI(ScoringFunction& sf)
				throw();

			/**	Copy constructor
			*/
			CHPI(const CHPI& chpi)
				throw();

			/**	Destructor.
			*/
			virtual ~CHPI()
				throw();

			//@}
			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
			*/
			const CHPI& operator = (const CHPI& chpi)
				throw();

			/** Clear method.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Predicates.
			*/
			//@{

			bool operator == (const CHPI& chpi) const
				throw();

			//@}
			/**	@name	Setup Methods
			*/
			//@{

			/**	Setup method.
			*/
			virtual bool setup()
				throw();

			void setupLigand();

			virtual double updateScore()
				throw();

			void update(const vector<std::pair<Atom*, Atom*> >& /*atom_pairs*/);

			///
			Options options;

		private:
			/** A TimeStamp that is used to check during each call of update whether the receptor has been changed.\n
			If this is the case, the ring-centers and normal-vector will be recomputed. */
			TimeStamp update_time_stamp_;

			//_ A vector containing all possible interactions between aliphatic C-H groups and aromatic rings
			vector<pair<const AromaticRing*, const CHGroup*> > possible_interactions_;

			// A vector containing all aliphatic C-H groups of the ligand.
			vector<CHGroup*> ligand_CH_groups_;

			// A vector containing all aromatic rings of the receptor.
			vector<AromaticRing*> receptor_aromatic_rings_;

			// A vector containing all aliphatic C-H groups of the ligand.
			vector<CHGroup*> receptor_CH_groups_;

			// A vector containing all aromatic rings of the receptor.
			vector<AromaticRing*> ligand_aromatic_rings_;

			//_
			float CX_distance_upper_;

			//_
			float CHX_angle_lower_;

			//_
			float HX_projected_distance_lower_;

			//_
			float HX_projected_distance_upper_;

			//_ The tolerance area for creating scores instead of simply counted
			//_ interactions.
			float distance_tolerance_;
			float angle_tolerance_;

			//_ A limit for ignoring interactions that have only very small
			//_ scores
			float limit_;

			//_ Verbosity of the code
			Size verbosity_;

			//_ Flag for writing HIN file containing pseudomolecules for the CHPI
			//_ interactions found in the system
			bool write_interactions_file_;

			RingPerceptionProcessor rp_;
			AromaticityProcessor ap_;

			float distance_cutoff_;
	};
} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_CHPI_H
