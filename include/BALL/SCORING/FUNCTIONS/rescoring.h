// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_FUNCTIONS_RESCORING_H
#define BALL_SCORING_FUNCTIONS_RESCORING_H

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/regressionModel.h>


namespace BALL
{
		class BALL_EXPORT Rescoring
		{
			public:

				Rescoring(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options, String free_energy_label, ScoringFunction* sf);

				virtual ~Rescoring();

				/** add score and experimentally determined binding affinity for one molecule to training data set */
				void addScoreContributions(Molecule* mol);

				/** train a model based on the data entered by use of addScoreContributions() for each molecule */
				void recalibrate();

				/** Rescores the given molecule. Depending on the derived class, this method may or may not use a QSAR-Model.
				If you reimplement this method in a derived class, make sure to make it add the predicted and the experimentally determined affinity value to predicted_affinities_ resp. experimental_affinities_. */
				virtual double rescore(Molecule* mol);

				/** If a QSAR-model was created, it can be saved to file by use of this method */
				void saveModel(String filename);

				/** If the derived class uses a QSAR-model, it can be loaded from a file by use of this method */
				void loadModel(String filename);

				/** Calculated quality statistics for the rescoring that was done by all previous uses of rescore(). */
				void calculateQuality(double& correlation, double& q2, double& std_error);

				/** Return the name of the rescoring approach. */
				const String& getName();


			protected:

				class RescoreQSARData
					: public QSAR::QSARData
				{
					public:

						friend class Rescoring;
						friend class Rescoring3D;
				};

				/** If the Rescoring-approach uses training and a QSAR-model, this function should be implemented in the derived class and should generate score-contributions for the given molecule and add them to 'matrix' or, if matrix == NULL, to 'v'.
				The default function of this base-class does nothing. */
				virtual void generateScoreContributions_(Molecule* /*mol*/, vector<vector<double> >* /*matrix*/, vector<double>* /*v*/) {};

				void setup_();

				ScoringFunction* scoring_function_;

				RescoreQSARData data_;
				QSAR::RegressionModel* model_;
				String free_energy_label_;
				String ff_filename_;
				bool convert_;

				/** determines whether the Rescoring approach uses training/calibration */
				bool use_calibration_;

				/** vector containing one predicted affinity value for each molecule that was rescored by use of rescore(). */
				vector<double> predicted_affinities_;

				/** vector containing one experimentally determined affinity value for each molecule that was rescored by use of rescore(). */
				vector<double> experimental_affinities_;

				/** Determines whether an applicability domain check should be done before rescoring each ligand. If the ligand is outside of the applicability domain, the score obtained by use of the scoring function will be returned without any model-based rescoring. */
				bool check_applicability_;

				/** the name of the rescoring approach */
				String name_;

				std::ofstream stddev_out_;
		};

}

#endif //BALL_SCORING_FUNCTIONS_RESCORING_H
