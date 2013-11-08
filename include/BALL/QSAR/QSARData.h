// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef QSARH
#define QSARH

#include <iostream>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <math.h>
#include <sstream>
#include <fstream>
#include <limits>
#include <fstream>
#include <BALL/QSAR/simpleDescriptors.h>
#include <BALL/QSAR/connectivityDescriptors.h>
#include <BALL/QSAR/partialChargeDescriptors.h>
#include <BALL/QSAR/surfaceDescriptors.h>
#include <BALL/COMMON/exception.h>
#include <cstring>

#ifndef STATISTICS
#include <BALL/QSAR/statistics.h>
#endif

#ifndef QSAR_EXCEPTION
#include <BALL/QSAR/exception.h>
#endif

#include <BALL/CONCEPT/timeStamp.h>

// #ifndef MODEL
// #include "Model.h"
// #endif

namespace BALL
{
	class MolecularSimilarity;

	namespace QSAR
	{
		typedef vector<double> Column;
		typedef vector<Column> VMatrix;
		
		/** QSAR */
		class BALL_EXPORT QSARData 
		{
			public:
				
				QSARData();
				
				~QSARData();
				
				/** @name Predicates */
				
				/** tells whether the features have been centered */
				bool isDataCentered() const;
				
				/** tells whether the response variables have been centered */
				bool isResponseCentered() const;
					
				/** @name Accessors
				 */
				//@{
				/** reads the names of the properties from the first molecule in the given sd-file */
				vector<String>* readPropertyNames(String sd_file);

				/** Fetches input from one sd-file containing all structures and from one file containing the activities of all structures sorted in ascending order. \n 
				The latter file is assumed to have the same name as the first one, with only the extension changed to ".txt"
				@param file the sd-file containing the input */
				void readSDFile(const char* file);
				
				/** Fetches input from one sd-file containing all structures. The activity value for each molecule is taken from its property in the sd-file. \n
				@param a contains the numbers of the properties that are activity-values
				@param file the sd-file containing the input 
				@param useExDesc if set to 1, descriptors read from the sd-file will be used in addition to those calculated by BALL internally 
				@param append if set to 1, the substances read from the sd-file will be appended as new lines to the current descriptor_matrix */
				void readSDFile(const char* file, std::multiset<int>& act, bool useExDesc=1, bool append=0, bool translate_class_labels=0);

				void readSDFile(const char* file, std::set<String>& activity_names, bool useExDesc=1, bool append=0, bool translate_class_labels=0, bool calc_phychem_properties=1, bool calc_topological_properties=1);
					
				/** show descriptor_matrix on stdout */
				void displayMatrix();
				
				/** centers each descriptor to mean of 0 and stddev of 1 
				@param center_Y if ==1, activity values are also centered. Obviously this should NOT be used for classification experiments! */ 
				void centerData(bool center_Y=0);
	
				/** scales each descriptor to stddev of 1 */ 
				void scaleAllDescriptors();
				
				/** returns the number of substances */
				unsigned int getNoSubstances() const;
				
				/** returns the number of descriptors */
				unsigned int getNoDescriptors() const;
				
				/** Read input from a csv file. \n
				This file should contain all descriptor values in the first columns and the activity values in the last no_y columns.\n
				@param no_y the number of activities, i.e. the number of columns containing activity values
				@param xlabels if ==1, names of descriptors are read from the first line of the table
				@param ylabel if ==1, names of substances are read from the first column of the table 
				@param sep the character used to seperate the cells of the table
				@param appendDescriptors if set to 1, descriptors will be read from the file and appended as new columns to the current descriptor_matrix */
				void readCSVFile(const char* file, int no_y, bool xlabels, bool ylabels, const char* sep=",", bool appendDescriptors=0, bool translate_class_labels=0);
	
				/** for testing purposes only: change Y-matrix according to the given equations */
				void manipulateY(std::vector<String> v);
	
				/** for testing purposes only: change Y-matrix according to the given equation
				@param v string containing the equation, e.g."x1+x3*5+x10^2" */
				void manipulateY(String v);
				
				/** Discretize the response values. If the response variable(s) of this data object have been normalized, the given thresolds will be automatically normalized accordingly.
				@param thresolds d thresholds for d+1 classes, that are to be created */
				void discretizeY(std::vector<double> thresholds);
				
				void transformX(std::vector<String> v);
				
				/** partitions the input data into p QSARData object of (approx.) equal size. */
				std::vector<QSARData*> partitionInputData(int p);
				
				/** saves the current QSARData object to a text file */
				void saveToFile(string filename) const;
				
				/** reconstructs a QSARData object from a text file */
				void readFromFile(string filename);
				
				/** generates a training and an external validation set from the current QSARData object 
				@param fraction the fraction of this current coumpounds that should be used as external validation set (by random drawing) */
				std::vector<QSARData*> generateExternalSet(double fraction) const;

				/** Split this data set into a training set and a test set.
				In contrast to generateExternalSet(), compounds for the test set are *not* randomly selected. Instead, this data set is first sorted according to response values (in order to ensure equal response value ranges) and then split regularly into training and test set.
				@param no_test_splits the total number of splits you want to create by successive calls of this function
				@param current_test_split_id the split to be produced, with 0<=current_test_split_id<no_test_splits */
				std::vector<QSARData*> evenSplit(int no_test_splits, int current_test_split_id, int response_id=0) const;
				
				/** returns a pointer to a new vector containing the UNcentered descriptor values for the s'th substance of the current data set */
				std::vector<double>* getSubstance(int s) const;
				
				/** returns a pointer to a new vector containing the UNcentered response values for the s'th substance of the current data set */
				std::vector<double>* getActivity(int s) const;
				
				/** returns the number of response variables */
				unsigned int getNoResponseVariables() const;
				
				const std::vector<string>* getSubstanceNames() const;
				
				/** checks whether the response variables contain only discrete values. This can be used to check whether the current input data set is suitable for a ClassificationModel */
				bool checkforDiscreteY() const;
				
				
				/** checks whether the response variables of a specified file contain only discrete values. */
				bool checkforDiscreteY(const char* file, std::multiset<int>& activity_IDs) const;
				
				/** allows to set the data-folder neccessary for computation of descriptors without using BALL_DATA_PATH enviroment variable, which is useful for standalone applications */
				void setDataFolder(const char* folder);
				
				/** removes compounds whose absolute correlation coefficient to another compound is larger than cor_threshold 
				@param feature_cor_threshold Only features that do not have a correlation larger than this value to another feature are used to calculate the similarity of compounds (=instances). */
				void removeHighlyCorrelatedCompounds(double& compound_cor_threshold, double& feature_cor_threshold);
				
				/** Find all descriptors of the current data set that have a correlation of at least 'similarity' to the specified feature 
				@param descriptor_ID the ID of the descriptor for which similar features should be searched
				@param similarity the desired minimal correlation 
				@param similar_descriptor_IDs list to which the IDs of the found descriptors will be saved as pairs of descriptor ID and descriptor name */

				void getSimilarDescriptors(int descriptor_ID, double correlation, std::list<std::pair<uint,String> >& similar_descriptor_IDs) const;
				//@}
				
				
			protected:
				
				/** @name Accessors
				 */
				//@{

				/**
				Calculates descriptors for one molecule and saves them into one new line of descriptor_matrix
				*/
				void calculateBALLDescriptors(Molecule& m);

				/** Calculates topological descriptors based on functional groups counts done by SMARTS matching */
				void calculateTopologicalDescriptors(Molecule& mol, MolecularSimilarity& molsim, const std::map<String,int>& descriptor_map);

				/** writes the names of all external descriptors into column_names */
				void setDescriptorNames(const Molecule& m, std::multiset<int>& activity_IDs, bool useExDesc=1, bool resize=1);
				
				/** removes columns of invalid descriptor from descriptor_matrix 
				@param invalidDescriptors list containing the IDs of the columns to be deleted */
				void removeInvalidDescriptors(std::multiset<int>& invalidDescriptors);
				
				void removeInvalidSubstances(std::multiset<int>& inv);
				
				/** reconstructs a vector based matrix from a file */
				void readMatrix(VMatrix& mat, std::ifstream& in, char seperator, unsigned int lines, unsigned int col);
				
				/** checks whether the given list of activity IDs contains any values <0 or values that are larger than the number of properties in the current input file.\n
				If such values are found, an Exception of type InvalidActivityID is thrown. */
				void checkActivityIDs(std::multiset<int>& act, int no_properties);
				
				/** appends compound no <s> taken from the given source to the data of this object.
				@param backtransformation if set to true, all features of the compound are back-transformed after adding them to this object. */
				void insertSubstance(const QSARData* source, int s, bool backtransformation=0);
				
				/** prints a vector-based matrix to a file */
				void printMatrix(const VMatrix& mat, std::ostream& out) const;
				//@}
				
				/** @name Attributes
				 */
				//@{
				/** matrix containing the values of each descriptor for each substance */ 
				VMatrix descriptor_matrix_;
				
				/** matrix containing the experimentally determined results (active/non-active) for each substance. Different activities are saved column-wise. */
				VMatrix Y_;
				
				/** 2xm dimensional matrix (m=no of descriptors) containing mean and stddev of each transformed descriptor */
				VMatrix descriptor_transformations_;
				
				/** 2xc dimensional matrix (c=no of activities) containing mean and stddev of each transformed activity */
				VMatrix y_transformations_;
			
				/** names of all descriptors */
				vector<string> column_names_;
	
				/** names of all substances */
				vector<string> substance_names_;
	
				/** contains the numbers of external descriptors for which invalid values (e.g. strings instead numerical values) were encountered in some molecules */
				std::multiset<int> invalidDescriptors_;
				
				std::multiset<int> invalidSubstances_;
				
				String data_folder_;
				
				/** in case of classification data sets with non-numeric class labels, this member maps the names of the individual classes to their assigned id. */ 
				std::map<String,int> class_names_;
				//@}

				
				
				friend class ClassificationValidation;
				friend class RegressionValidation;
				friend class Validation;
				friend class Model;
				friend class FitModel;
				friend class FeatureSelection;
	
		};

	}
}

#endif // QSARH
