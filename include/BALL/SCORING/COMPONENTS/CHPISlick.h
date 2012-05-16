// $Id: CHPI.h,v 1.2.10.1 2007/08/07 16:16:15 bertsch Exp $
// Molecular Mechanics: SLICK force field, CH---pi interaction

#ifndef BALL_SCORING_COMPONENTS_CHPISLICK_H
#define BALL_SCORING_COMPONENTS_CHPISLICK_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/DATATYPE/options.h>

namespace BALL
{
	/** SLICK ringstacking component.
			{\bf Definition:} \URL{BALL/SCORING/COMPONENTS/CHPI.h}
	*/
	class BALL_EXPORT CHPISlick : public ScoringComponent
	{
		public:
			/// A class for storing aromatic rings and information associated with
			/// them. At the moment we only need the ring centres and the atoms.
			class AromaticRing
			{
				public:

					///
					AromaticRing()
						;

					///
					AromaticRing(const AromaticRing& aromatic_ring)
						;

					///
					AromaticRing(const std::vector<Atom*>& atoms)
						;

					///
					const Vector3& getCentre() const
						;

					///
					const Vector3& getNormalVector() const
						;

					///
					void setRing(const std::vector<Atom*>& atoms)
						;

					// accessors for the private data members
					const std::vector<Atom*>& getRing() const
						;

					///
					void dump(std::ostream& s = std::cout) const
						;

				private:

					//_ A hash set containing pointers to all ring atoms:
					std::vector<Atom*> ring_atoms_;

					//_ The centre of the aromatic ring.
					Vector3 centre_;

					//_ The normal vector of the ring plane.
					Vector3 normal_vector_;

					//_
					void computeCentre_()
						;

					//_
					void computeNormalVector_()
						;

			};


			/// A class for storing aliphatic CH groups suitable for building
			/// CH-\pi interactions with aromatic rings.
			class CHGroup
			{
				public:

					///
					CHGroup()
						;

					///
					~CHGroup()
						;

					///
					CHGroup(const CHGroup& CH_groups)
						;

					///
					CHGroup (const Atom* C_atom, const Atom* H_atom)
						;

					///
					void setCAtom(const Atom* C_atom)
						;

					///
					void setHAtom(const Atom* H_atom)
						;

					///
					void setAtoms(const Atom* C_atom, const Atom* H_atom)
						;

					///
					const Atom* getHAtom() const

					{
						return(H_atom_);
					}

					///
					const Atom* getCAtom() const

					{
						return(C_atom_);
					}

					void dump(std::ostream& s = std::cout) const
						;

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
			CHPISlick()
				;

			/**	Constructor.
			 */
			CHPISlick(ScoringFunction& sf)
				;

			/**	Copy constructor
			 */
			CHPISlick(const CHPISlick& chpi)
				;

			/**	Destructor.
			 */
			virtual ~CHPISlick()
				;

			//@}
			/**	@name	Assignment
			 */
			//@{

			/** Assignment.
			 */
			const CHPISlick& operator = (const CHPISlick& chpi)
				;

			/** Clear method.
			 */
			virtual void clear()
				;

			//@}
			/**	@name	Predicates.
			 */
			//@{

			bool operator == (const CHPISlick& chpi) const
				;

			//@}
			/**	@name	Setup Methods
			 */
			//@{

			/**	Setup method.
			 */
			virtual bool setup()
				;

			void update(const vector<std::pair<Atom*, Atom*> >&);

			/**
			 * Calculate CH_PI interaction score
			 */
			virtual double updateScore()
				;

			///
			Options options;

		private:

			//_ A vector containing all possible interactions between aliphatic
			//_ C-H groups in the sugar and aromatic rings in the receptor.
			std::vector< std::pair<const AromaticRing*, const CHGroup*> > possible_interactions_;

			//_ A vector containing all aliphatic C-H groups of the ligand.
			std::vector<CHGroup> all_CH_groups_;

			//_ A vector containing all aromatic rings of the receptor.
			std::vector<AromaticRing> all_aromatic_rings_;

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

	};



} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_CHPISLICK_H
