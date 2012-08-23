/* scoringOptimizer.h
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
