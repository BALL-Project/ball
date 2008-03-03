// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
#ifndef MODEL
#define MODEL

#include <vector>
#include <newmat.h>

#ifndef VALIDATION
#include <BALL/QSAR/validation.h>
#endif

#ifndef QSARH
#include <BALL/QSAR/QSARData.h>
#endif

#ifndef SORTEDLIST
#include <BALL/QSAR/sortedList.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif

#include <BALL/MATHS/parsedFunction.h>

namespace BALL 
{
	namespace QSAR
	{
		class Model
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
				virtual RowVector predict(const vector<double>& substance, bool transform) =0; 
				
				/** removes all entries from descriptor_IDs */
				void deleteDescriptorIDs();
			
				/** Starts training the model. */
				virtual void train() =0;
				
				
				/** optimizes parameters (!=number of features) of the model, e.g. no of latente variables in case of PLS model or kernel width in case of automated lazy learning model. \n
				The number of selected features (=descriptors) is NOT changed by this method. Use class FeatureSelection in order to do this.\n
				@return 1 if parameters were optimized using cross-validation. The best Q2 value is assumed to be saved in ModelValidation.Q2 \n
				0 if the model has no parameters to be optimized, so that no cross-validation was done.*/
				virtual bool optimizeParameters(int /*k*/, int /*no_steps*/=30){return 0;};
				
				virtual double calculateStdErr()
				{return -1.0;};
				
				/** sets the model parameters according to the given values. */
				virtual void setParameters(vector<double>& /*v*/){};
				
				virtual vector<double> getParameters() const;

				/** returns a const pointer to the descriptor IDs of this model */
				SortedList<unsigned int>* getDescriptorIDs();
				
				void setDataSource(const QSARData* q);
				
				/** save Model to a file */
				virtual void saveToFile(string filename) = 0;
				
				/** reconstruct a saved Model from a file */
				virtual void readFromFile(string filename) = 0;
				//@}
				
				
				/** @name Attributes
				 */
				//@{
				/** pointer to the input data class for this model */
				const QSARData* data;
				
				/** returns a const pointer to the descriptor matrix of this model */
				const Matrix* getDescriptorMatrix();
				
				/** returns a const pointer to the names of the substances of this model */
				const vector<string>* getSubstanceNames();
				
				/** returns a const pointer to the names of the descriptors of this model */
				const vector<string>* getDescriptorNames();
				
				/** returns a const pointer to the activity values of this model */
				const Matrix* getY();
				
				/** manually specify a set of descriptors */
				void setDescriptorIDs(const SortedList<unsigned int>& sl);
				
				/** returns the type of the current model, e.g. "MLR", "PLS", ... */
				const string* getType();
				
				/** a ModelValidation object, that is used to validate this model and that will contain the results of the validations */
				Validation* model_val;
				//@}
				
				
			protected:
			
				/** @name Accessors
				 */
				//@{
				/** returns a RowVector containing only the values for these descriptors, that have been selected for this model \n
				@param substance a vector of *all* descriptor values for the substance to be predicted */
				RowVector getSubstanceVector(const vector<double>& substance, bool transform);
				
				RowVector getSubstanceVector(const RowVector& substance, bool transform);
				
				/** transforms a prediction (obtained by Model.train()) according to the inverse of the transformation(s) of the activity values of the training data */
				void backTransformPrediction(RowVector& pred);
				
				/** adds offset lambda to the diagonal of the given matrix */
				void addLambda(Matrix& matrix, double& lambda);
				
				/** reads selected descriptors, their names and the information about their transformations (mean and stddev of each descriptor). This function is used after feature selection to read information about the selected features */
				void readDescriptorInformation();
				//@}	
				
				
				/** @name Attributes
				 */
				//@{
				/** newmat-matrix containing the values of each descriptor for each substance */ 
				Matrix descriptor_matrix_;
					
				/** names of all substances */
				vector<string> substance_names_;
	
				/** names of all descriptors */
				vector<string> descriptor_names_;
				
				/** 2xm dimensional matrix (m=no of descriptors) containing mean and stddev of each selected descriptor. \n
				The content of this matrix is updated only by Model.readTrainingData() */
				Matrix descriptor_transformations_;
				
				/** 2xc dimensional matrix (c=no of activities) containing mean and stddev of each activity.\n
				The content of this matrix is updated only by Model.readTrainingData() */
				Matrix y_transformations_;
				
				/** newmat-Matrix containing the experimentally determined results (active/non-active) for each substance. \n
				Each column contains the values for one activity. */
				Matrix Y_;

				/** The type of model, e.g. "MLR", "GP", ... */
				String type_;
				
				/** list containing the IDs of the selected descriptors (=features); with IDs >= 0 \n
				If this list is empty, it is assumed that no feature selection was done, i.e. that all descriptors are to be considered for cross-validation and prediction of activity. \n
				If it is not empty, only the descriptors in this list are used for cross-validation and prediction of activity. */
				SortedList<unsigned int> descriptor_IDs_;			
				//@}				
				
				friend class Validation;
				friend class RegressionValidation;
				friend class ClassificationValidation;
				friend class PCRModel;
				friend class KPCRModel;
				friend class FeatureSelection;
		};
	}
}

#endif // MODEL
