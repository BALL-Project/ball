// $Id: hydrogenBond.h,v 1.3 2006/05/17 12:49:15 anker Exp $
// Molecular Mechanics: Fresno force field, hydrogen bond component

#ifndef BALL_SCORING_COMPONENTS_HYDROGENBOND_H
#define BALL_SCORING_COMPONENTS_HYDROGENBOND_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/DATATYPE/options.h>

namespace BALL
{

	/** Fresno Hydrogen Bond component.
			{\bf Definition:} \URL{BALL/SCORING/COMPONENTS/hydrogenBond.h}
	*/
	class HydrogenBond
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




		BALL_CREATE(HydrogenBond)

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		HydrogenBond()
			;

		/**	Constructor.
		*/
		HydrogenBond(ScoringFunction& sf)
			;

		/**	Constructor.
		*/
		HydrogenBond(ScoringComponent& sc)
			;

		/**	Copy constructor
		*/
		HydrogenBond(const HydrogenBond& fhb)
			;

		/**	Destructor.
		*/
		virtual ~HydrogenBond()
			;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const HydrogenBond& operator = (const HydrogenBond& fhb)
			;

		/** Clear method.
		*/
		virtual void clear()
			;

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const HydrogenBond& fhb) const
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

		/**	
		*/
		virtual double calculateScore()
			;

		const HashMap<const Atom*, Size>& getFresnoTypes()
			;
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
		HashMap<const Atom*, Size> fresno_types;


	};

} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_HYDROGENBOND_H
