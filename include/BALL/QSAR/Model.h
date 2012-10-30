// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_QSAR_MODEL_H
#define BALL_QSAR_MODEL_H


#ifndef BALL_QSAR_VALIDATION_H
#include <BALL/QSAR/validation.h>
#endif

#ifndef BALL_QSAR_QSARDATA_H
#include <BALL/QSAR/QSARData.h>
#endif

#ifndef BALL_QSAR_EXCEPTION_H
#include <BALL/QSAR/exception.h>
#endif

#ifndef BALL_MATHS_PARSEDFUNCTION_H
#include <BALL/MATHS/parsedFunction.h>
#endif

#include <vector>
#include <set>

#include <Eigen/Core>

namespace BALL
{
	namespace QSAR
	{
		class BALL_EXPORT Model
		{
			public:
				/** @name Constructors and Destructors
				 */
				//@{
				/** constructur
				@param q QSARData object, from which the data for this model should be taken */
				Model(const QSARData& q);

				virtual ~Model();

				/** copy constructur; creates a model with the same specifications as the given one (same model and kernel parameters). If the given model has been trained, the training result is copied as well. \n
				Note, that the input data that has been read by m to m.descriptor_matrix_ and m.Y_ is NOT copied to new model, since the input data is not part of the specification of a model. If nevertheless, copying of the input data is desired, use function copyData() (afterwards).  */
				virtual void operator=(const Model& m);

				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
				//@}


				/** @name Accessors
				 */
				//@{
				/** copies the data (descriptor matrix, names of substances and descriptors) and the IDs of the selected descriptors from m */
				void copyData(const Model& m);
				
				/** copies the IDs of the selected descriptors from m */
				void copyDescriptorIDs(const Model& m);


				/** copies the data for the relevant descriptors from the bound QSARData object into this model and updates Model.descriptor_transformations and Model.y_transformations . \n
				If no explicit feature selection was done, i.e. if descriptor_IDs is emtpy, all data is fetched. \n
				If feature selection was done, i.e. if descriptor_IDs is not empty, only the columns of the relevant descriptors are fetched. */
				void readTrainingData();
				
				
				/** Predicts the activities of a given substance
				@param substance the substance which activity is to be predicted in form of a vecor containing the values for *all* descriptors (if neccessary, relevant descriptors will be selected automatically) 
				@param transform determines whether the values for each descriptor of the given substance should be transformed before prediction of activity. \n
				If (transform==1): each descriptor value is transformed according to the centering of the respective column of QSARData.descriptor_matrix used to train this model. \n
				If the substance to be predicted is part of the same input data (e.g. same SD-file) as the training data (as is the case during cross validation), transform should therefore be set to 0. 
				@return a RowVector containing one value for each predicted activity*/
				virtual Eigen::VectorXd predict(const vector<double>& substance, bool transform) =0; 
				
				/** removes all entries from descriptor_IDs */
				void deleteDescriptorIDs();
			
				/** Starts training the model. */
				virtual void train() =0;
				
				
				/** optimizes parameters (!=number of features) of the model, e.g. no of latente variables in case of PLS model or kernel width in case of automated lazy learning model. \n
				The number of selected features (=descriptors) is NOT changed by this method. Use class FeatureSelection in order to do this.\n
				@return 1 if parameters were optimized using cross-validation. The best Q2 value is assumed to be saved in ModelValidation.Q2 \n
				0 if the model has no parameters to be optimized, so that no cross-validation was done.*/
				virtual bool optimizeParameters(int /*k*/, int /*no_steps*/){return 0;};
				
				bool optimizeParameters(int k);
				
				virtual double calculateStdErr()
				{return -1.0;};
				
				/** sets the model parameters according to the given values. */
				virtual void setParameters(vector<double>& /*v*/){};
				
				virtual vector<double> getParameters() const;

				/** returns a const pointer to the descriptor IDs of this model */
				std::multiset<unsigned int>* getDescriptorIDs();
				
				void setDataSource(const QSARData* q);
				
				/** save Model to a file */
				virtual void saveToFile(string filename) = 0;
				
				/** reconstruct a saved Model from a file */
				virtual void readFromFile(string filename) = 0;
				
				/** returns a const pointer to the descriptor matrix of this model */
				const Eigen::MatrixXd* getDescriptorMatrix();
				
				/** returns a const pointer to the names of the substances of this model */
				const vector<string>* getSubstanceNames();
				
				/** returns a const pointer to the names of the descriptors of this model */
				const vector<string>* getDescriptorNames();

				/** returns descriptor transformations **/
				const Eigen::MatrixXd getDescriptorTransformations();

				const Eigen::MatrixXd getYTransformations();

				/** returns a const pointer to the activity values of this model */
				const Eigen::MatrixXd* getY();
				
				/** manually specify a set of descriptors */
				void setDescriptorIDs(const std::multiset<unsigned int>& sl);
				
				/** returns the type of the current model, e.g. "MLR", "PLS", ... */
				const string* getType();
				
				/** Fetches the un-normalized value for the specified feature of the desired compound (instance) from the data that this Model currently contains. This method is needed for visualization purposes only. */ 
				void getUnnormalizedFeatureValue(int compound, int feature, double& return_value);
				
				/** Fetches the un-normalized value for the specified response of the desired compound (instance) from the data that this Model currently contains. This method is needed for visualization purposes only. */
				void getUnnormalizedResponseValue(int compound, int response, double& return_value);
				//@}
				
				
				/** @name Attributes
				 */
				//@{
				/** pointer to the input data class for this model */
				const QSARData* data;
				
				/** a ModelValidation object, that is used to validate this model and that will contain the results of the validations */
				Validation* model_val;
				//@}
				
				
			protected:
				
				/** The default number of steps for model parameter optimization. \n
				It can be adjusted by the different types of models. \n
				Standard default value is 30. */
				int default_no_opt_steps_; 
			
				/** @name Accessors
				 */
				//@{
				/** returns a Row-Vector containing only the values for these descriptors, that have been selected for this model \n
				@param substance a vector of *all* descriptor values for the substance to be predicted */
				Eigen::VectorXd getSubstanceVector(const vector<double>& substance, bool transform);
				
				Eigen::VectorXd getSubstanceVector(const Eigen::VectorXd& substance, bool transform);
				
				/** transforms a prediction (obtained by Model.train()) according to the inverse of the transformation(s) of the activity values of the training data */
				void backTransformPrediction(Eigen::VectorXd& pred);
				
				/** adds offset lambda to the diagonal of the given matrix */
				void addLambda(Eigen::MatrixXd& matrix, double& lambda);
				
				/** reads selected descriptors, their names and the information about their transformations (mean and stddev of each descriptor). This function is used after feature selection to read information about the selected features */
				void readDescriptorInformation();
				//@}
				
				
				/** @name Input and Output. The following methods can be used to implement the functions saveToFile() and readFromFile() in final classes derived from this base-class 
				 */
				//@{
				/** reconstructs a Eigen::MatrixXd from a given input stream after resizing the given Eigen::MatrixXd as specified */
				void readMatrix(Eigen::MatrixXd& mat, std::ifstream& in, unsigned int lines, unsigned int col);
				
				void readVector(Eigen::RowVectorXd& vec, std::ifstream& in, unsigned int no_cells, bool column_vector);
				
				void readModelParametersFromFile(std::ifstream& in);
				void saveModelParametersToFile(std::ofstream& out);
				
				
				/** overloaded by class RegressionModel, whose member function can also save coefficients and coefficient-errors */
				virtual void saveDescriptorInformationToFile(std::ofstream& out);
				virtual void readDescriptorInformationFromFile(std::ifstream& in, int no_descriptors, bool transformation);
				
				void readResponseTransformationFromFile(std::ifstream& in, int no_y);
				void saveResponseTransformationToFile(std::ofstream& out);
				//@}	
				
				
				/** @name Attributes
				 */
				//@{
				/** matrix containing the values of each descriptor for each substance */ 
				Eigen::MatrixXd descriptor_matrix_;
					
				/** names of all substances */
				vector<string> substance_names_;
	
				/** names of all descriptors */
				vector<string> descriptor_names_;
				
				/** 2xm dimensional matrix (m=no of descriptors) containing mean and stddev of each selected descriptor. \n
				The content of this matrix is updated only by Model.readTrainingData() */
				Eigen::MatrixXd descriptor_transformations_;
				
				/** 2xc dimensional matrix (c=no of activities) containing mean and stddev of each activity.\n
				The content of this matrix is updated only by Model.readTrainingData() */
				Eigen::MatrixXd y_transformations_;
				
				/** Matrix containing the experimentally determined results (active/non-active) for each substance. \n
				Each column contains the values for one activity. */
				Eigen::MatrixXd Y_;

				/** The type of model, e.g. "MLR", "GP", ... */
				String type_;
				
				/** list containing the IDs of the selected descriptors (=features); with IDs >= 0 \n
				If this list is empty, it is assumed that no feature selection was done, i.e. that all descriptors are to be considered for cross-validation and prediction of activity. \n
				If it is not empty, only the descriptors in this list are used for cross-validation and prediction of activity. */
				std::multiset<unsigned int> descriptor_IDs_;			
				//@}				
				
				friend class Validation;
				friend class RegressionValidation;
				friend class ClassificationValidation;
				friend class PCRModel;
				friend class KPCRModel;
				friend class FeatureSelection;
		};
		
		/** convenience function for creating a new Model object from a given model-file */
		BALL_EXPORT Model* createNewModelFromFile(String model_file, const QSARData& q);
	}
}

#endif // BALL_QSAR_MODEL_H
