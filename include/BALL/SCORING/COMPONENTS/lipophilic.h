// $Id: lipophilic.h,v 1.2 2006/02/21 16:19:32 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#ifndef BALL_SCORING_COMPONENTS_LIPOPHILIC_H
#define BALL_SCORING_COMPONENTS_LIPOPHILIC_H

#include <BALL/SCORING/COMMON/scoringComponent.h>

namespace BALL
{

	/** Fresno lipophilic component.
			{\bf Definition:} \URL{BALL/SCORING/COMMON/scoringComponent.h}
	*/
	class Lipophilic
		:	public ScoringComponent
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
			static const Size VERBOSITY;

		};


		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		Lipophilic()
			throw();

		/**	Constructor.
		*/
		Lipophilic(ScoringFunction& sf)
			throw();

		/**	Copy constructor
		*/
		Lipophilic(const Lipophilic& li)
			throw();

		/**	Destructor.
		*/
		virtual ~Lipophilic()
			throw();

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
		virtual bool setup()
			throw();

		//@}
		/**	@name	Accessors	
		*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual double calculateScore()
			throw();

		//@}

		private:

		/*_
		*/
		std::vector< std::pair<const Atom*, const Atom*> > possible_lipophilic_interactions_;

		/*_ This length will be added to the sum of the van-der-Waals radii for
				obtaining the lower bound of the scoring function.
		*/
		double r1_offset_;

		/*_ The upper bound for the scoring function is obtained by adding a
				constant to the lower bound.
		*/
		double r2_offset_;

	};

} // namespace BALL

#endif // BALL_SCORING_COMPONENTS_LIPOPHILIC_H
