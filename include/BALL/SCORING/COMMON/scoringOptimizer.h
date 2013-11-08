// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMMON_SCORINGOPTIMIZER_H
#define BALL_SCORING_COMMON_SCORINGOPTIMIZER_H

#include <BALL/SCORING/COMMON/scoringFunction.h>


namespace BALL
{
	/** Class for evaluating and optimizing ScoringFunctions on a given set of receptor-ligand complexes whose binding free energy is known */
	class BALL_EXPORT ScoringOptimizer
	{
		public:
			ScoringOptimizer(Options& options, bool train);

			void addComplex(String name, String receptor_file, String ligand_file, double binding_free_energy);

			void evaluate(double* correlation = 0, double* R2 = 0, double* RMSE = 0, Size* no_valid_complexes = 0);

			void printMatrix(std::ostream& out = std::cout);

			struct Result
			{
				std::list<double> predictions;
				std::list<double> expected_affinities;
				std::list<String> target_names;
				std::vector<std::vector<double> > score_contributions;
				std::vector<String> score_contribution_names;
			};

			const Result* getResult();

		private:
			struct Complex
			{
				String name;
				String receptor_file;
				String ligand_file;
				double binding_free_energy;
			};

			list<Complex> complexes_;

			Options options_;
			String scoring_function_name_;

			Result result_;

			/** if set to true, the coefficients of all ScoringComponents will be set to 1 and all transformations will be disabled. This is necessary if the output is to be used for optimization by regression */
			bool train_;

			ScoringFunction* createScoringFunction(System& receptor, System& ligand);
	};

	static double getMean(const list<double>& v);

	static double getCovariance(const list<double>& v1, const list<double>& v2, double mean1, double mean2);

	static double getStddev(const list<double>& m, double mean);

	static double getSumOfSquares(const list<double>& m, double mean);
}


#endif // BALL_SCORING_COMMON_SCORINGOPTIMIZER_H
