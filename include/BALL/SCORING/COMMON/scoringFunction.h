// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scoringFunction.h,v 1.1 2005/11/21 19:27:03 anker Exp $

#ifndef BALL_SCORING_COMMON_SCORINGFUNCTION_H
#define BALL_SCORING_COMMON_SCORINGFUNCTION_H

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SCORING/COMMON/baseFunction.h>
#include <BALL/SCORING/COMMON/scoringComponent.h>

namespace BALL
{

	class ScoringComponent;

	class ScoringFunction
	{

		public:

		struct Option
		{

			/**
			*/
			static const char* VERBOSITY;

			/**
			*/
			static const char* BASE_FUNCTION_TYPE;

		};


		struct Default
		{

			/**
			*/
			static const Size VERBOSITY;

			/**
			*/
			static const Size BASE_FUNCTION_TYPE;

		};

		///
		ScoringFunction()
			throw();

		///
		ScoringFunction(const ScoringFunction& sf)
			throw();

		///
		ScoringFunction(System& system)
			throw();

		///
		ScoringFunction(System& system, const Options& options)
			throw();

		///
		ScoringFunction(System& system, Molecule& receptor, Molecule& ligand,
				const Options& options)
			throw();

		///
		virtual ~ScoringFunction()
			throw();

		///
		virtual void clear()
			throw();

		///
		ScoringFunction& operator = (const ScoringFunction& sf)
			throw();

		///
		bool setup()
			throw();

		/// 
		bool setup(System& system, Molecule& receptor, Molecule& ligand)
			throw();

		/// 
		bool setup(System& system, Molecule& receptor, Molecule& ligand,
				const Options& options)
			throw();

		///
		virtual bool specificSetup()
			throw(Exception::TooManyErrors);

		///
		void setMaximumNumberOfErrors(Size nr)
			throw();

		///
		Size getMaximumNumberOfErrors() const
			throw();

		///
		void setSystem(System& system) 
			throw();

		///
		System* getSystem() const
			throw();

		///
		void setFirstMolecule(Molecule& molecule1) 
			throw();

		///
		Molecule* getFirstMolecule() const
			throw();

		///
		void setSecondMolecule(Molecule& molecule2) 
			throw();

		///
		Molecule* getSecondMolecule() const
			throw();

		/** Set the receptor of the complex (which is just the same as
				molecule1_).
		*/
		void setReceptor(Molecule& receptor) 
			throw();

		/** Return the receptor of the complex (which is just the same as
				molecule1_).
		*/
		Molecule* getReceptor() const
			throw();

		/** Set the ligand of the complex (which is just the same as
				molecule2_).
		*/
		void setLigand(Molecule& ligand) 
			throw();

		/** Return the ligand of the complex (which is just the same as
				molecule2_).
		*/
		Molecule* getLigand() const
			throw();

		///
		void setIntercept(double intercept)
			throw();

		///
		double getIntercept() const
			throw();

		/// Set the instance of BaseFunction for the scoring function.
		void setBaseFunction(ScoringBaseFunction& base_function)
			throw();

		/** Return a pointer to the instance of BaseFunction used by this
				scoring function.
		*/
		ScoringBaseFunction* getBaseFunction() const
			throw();

		///
		void insertComponent(ScoringComponent* component, float coefficient = 1.0f)
			throw();

		/** Remove a component from the component list of this scoring
				function and delete it from memory. If there was no matching
				component, do nothing. Note that the user will get no feedback if
				there was no component removed.
		*/
		void removeComponent(const ScoringComponent* component)
			throw();

		/** Remove a component from the component list of this scoring
				function and delete it from memory. If there was no matching
				component, do nothing. Note that the user will get no feedback if
				there was no component removed. Only the first occurrence of a name
				in the component list will be removed.
		*/
		void removeComponent(const String& name)
			throw();

		/** Set the coefficient of a component. If there was no matching
				component, do nothing. Note that the user will get no feedback if
				there was no coefficient set.
		*/
		void setCoefficient(const ScoringComponent* component, float coefficient)
			throw();

		/** Set the coefficient of a named component. If there was no matching
				component, do nothing. Note that the user will get no feedback if
				there was no coefficient set. Only the first occurrence of a name
				in the component list will be changed.
		*/
		void setCoefficient(const String& name, float coefficient)
			throw();

		/** Get the coefficient of a component. If there was no matching
				component, return false, ow return true. 
		*/
		bool getCoefficient(const ScoringComponent* component,
				float& coefficient) const
			throw();

		/** Get the coefficient of a component. If there was no matching
				component, return false, ow return true. 
		*/
		bool getCoefficient(const String& name, float& coefficient) const
			throw();

		/// Return a pointer to a component in the components vector.
		ScoringComponent* getComponent(const String& name) const
			throw();

		/// Return a pointer to a component in the components vector.
		ScoringComponent* getComponent(const Size index) const
			throw();

		///
		double calculateScore()
			throw();

		///
		const HashSet<const Atom*>& getUnassignedAtoms() const
			throw();

		///
		HashSet<const Atom*>& getUnassignedAtoms()
			throw();

		///
		Options options;


		protected:

		//_ A system containing the molecules for scoring
		System* system_;

		/*_ The first molecule of the complex, being the receptor in
				receptor/ligand complexes
			*/
		Molecule* molecule1_;

		/*_ The second molecule of the complex, being the ligand in
				receptor/ligand complexes
			*/
		Molecule* molecule2_;

		//_ The name of the scoring function
		String name_;

		//_ The overall score of the scoring function
		double score_;

		//_ The intercept necessary for calculating the score
		double intercept_;

		/*_ The base funcion for scoring simple terms
		*/
		ScoringBaseFunction* base_function_;

		/*_ A list of components comprising the scoring function, along with
				the coefficients for each term necessary for calculating the final
				score.
		*/
		std::vector< std::pair<ScoringComponent*, float> > components_;

		//_ Atoms, for which the setup of the force field fails
		HashSet<const Atom*> unassigned_atoms_;

		//_ Max number of unassigned atoms
		Size max_number_of_errors_;

		//_ Actual number of countde errors
		Size number_of_errors_;


	};

} // namespace BALL

#endif // BALL_SCORING_COMMON_SCORINGFUNCTION_H
