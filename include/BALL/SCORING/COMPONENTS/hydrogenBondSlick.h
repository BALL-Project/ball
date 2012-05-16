// $Id: HydrogenBondSlick.h,v 1.3 2006/05/17 12:49:15 anker Exp $
// Molecular Mechanics: Fresno force field, hydrogen bond component

#ifndef BALL_SCORING_COMPONENTS_HYDROGENBONDSLICK_H
#define BALL_SCORING_COMPONENTS_HYDROGENBONDSLICK_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/DATATYPE/options.h>

namespace BALL
{

	/** Fresno Hydrogen Bond component.
			{\bf Definition:} \URL{BALL/SCORING/COMPONENTS/hydrogenBondSlick.h}
	*/
	class HydrogenBondSlick
		:	public ScoringComponent
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




		BALL_CREATE(HydrogenBondSlick)

		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		HydrogenBondSlick()
			;

		/**	Constructor.
		*/
		HydrogenBondSlick(ScoringFunction& sf)
			;

		/**	Constructor.
		*/
		HydrogenBondSlick(ScoringComponent& sc)
			;

		/**	Copy constructor
		*/
		HydrogenBondSlick(const HydrogenBondSlick& fhb)
			;

		/**	Destructor.
		*/
		virtual ~HydrogenBondSlick()
			;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const HydrogenBondSlick& operator = (const HydrogenBondSlick& fhb)
			;

		/** Clear method.
		*/
		virtual void clear();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const HydrogenBondSlick& fhb) const
			;

		//@}
		/**	@name	Setup Methods
		*/
		//@{

		/**
		 * Setup method.
		*/
		virtual bool setup();

		//@}
		/**	@name	Accessors
		*/
		//@{

		void update(const vector<std::pair<Atom*, Atom*> >& pair_vector);

		/**
		 * Calculates and returns the component's energy.
		*/
		virtual double updateScore();

		//@}

		private:

		/*_ This vector contains all possible hydrogen bonds. It is created
		 * during the setup process. Pairs are always of the form (hydrogen,
		 * acceptor). The donor can be found easily by following the only bond
		 * of the hydrogen.
		*/
		std::vector< std::pair<const Atom*, const Atom*> > possible_hydrogen_bonds_;

		/*_ A hash map containing all hydrogens and bools indicating whether
		 * this hydrigen was already used for scoring a hydrogen bond. At the
		 * moment we use the first hydrogen bond we find and hope that this
		 * will indeed be the only one contributing to the energy score. This
		 * HashMap will only be used by updateEnergy() but we build it in
		 * setup().
		*/
		HashMap<const Atom*, bool> already_used_;

		/*_
		*/
		double factor_;

		/*_
		*/
		double h_bond_distance_lower_;

		/*_
		*/
		double h_bond_distance_upper_;

		/*_
		*/
		double h_bond_angle_lower_;

		/*_
		*/
		double h_bond_angle_upper_;

		/*_ The ideal hbond length.
		*/
		double ideal_hbond_length_;

		/*_ The ideal hbond angle.
		*/
		double ideal_hbond_angle_;

		/*_
		*/
		HashMap<const Atom*, Size> rec_fresno_types_;

		HashMap<const Atom*, Size> lig_fresno_types_;
	};

} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_HYDROGENBONDSLICK_H
