// $Id: fresnoEnergyProcessor.h,v 1.1 2002/01/28 10:43:29 anker Exp $

#ifndef BALL_ENERGY_FRESNOENERGYPROCESSOR_H
#define BALL_ENERGY_FRESNOENERGYPROCESSOR_H

#ifndef BALL_ENERGY_ENERGYPROCESSOR_H
#	include <BALL/ENERGY/energyProcessor.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif


namespace BALL
{

	/** Processor for calculating the binding free energy according to the
			Fresno model.
			This processor implements th scoring function developed by Rogman et al.
			(J. Med. Chem. 42:4650-4658, 1999). \\
			{\bf Definition:} \URL{BALL/ENERGY/fresnoEnergyProcessor}
	*/

	class FresnoEnergyProcessor
		:	public EnergyProcessor
	{

		public:

		/** Symbolic names for option keys.
				This struct contains a symbolic name for each recognized key in
				FresnoEnergyProcessor::options.
		*/
		struct Option
		{
			/** The verbosity level.
					Use integer values with this option.
					@see Default::VERBOSITY
					@param verbosity integer
			 */
			static const char* VERBOSITY;
		};

		/** Default values for energy calculations.
		 */
		struct Default
		{
			/** Default verbosity level.
					@see Option::VERBOSITY
			 */
			static const Size VERBOSITY;
		};

		/** @name Constructors and destructors 
		*/
		//@{

		/** Default constructor 
		*/
		FresnoEnergyProcessor()
			throw();

		/** Copy constructor 
				@param proc the processor to copy
		*/
		FresnoEnergyProcessor(const FresnoEnergyProcessor& proc)
			throw();

		/** Destructor 
		*/
		virtual ~FresnoEnergyProcessor()
			throw();

		//@}
		/** @name Accessors 
		*/
		//@{

		/**
		*/
		void setCoefficients(const ::vector<double>& coefficients)
			throw();

		/**
		*/
		const ::vector<double>& getCoefficients() const
			throw();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const FresnoEnergyProcessor& operator =
			(const FresnoEnergyProcessor& proc) throw();

		/** Clear function 
		*/
		virtual void clear() throw();
		
		//@}
		/** @name Processor functions 
		*/
		//@{

		/** 
		*/
		virtual bool finish() 
			throw();

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const FresnoEnergyProcessor& proc) 
			const throw();

		//@}
		/** @name Options 
		*/
		//@{

		/** Options
		*/
		Options options;

		//@}


		protected:

		/*_ Calculate the underlying function of some contributions
		*/
		double calculateHelperFunction_(double x, double l, double u) const
			throw();


		private:

		/*_ Calculate HB
		*/
		double calculateHB_() const
			throw();

		/*_ Calculate LIPO
		*/
		double calculateLIPO_() const
			throw();

		/*_ Calculate ROT
		*/
		double calculateROT_() const
			throw();

		/*_ Calculate BP
		*/
		double calculateBP_() const
			throw();

		/*_ Calculate DESOLV
		*/
		double calculateDESOLV_() const
			throw();

		/*_ The coefficients of the scoring function
		*/
		::vector<double> coefficients_;

	};
}

#endif // BALL_ENERGY_FRESNOENERGYPROCESSOR_H
