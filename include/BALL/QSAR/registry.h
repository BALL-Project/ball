#ifndef BALL_QSAR_REGISTRY_H
#define BALL_QSAR_REGISTRY_H

#ifndef MODEL_FACTORY_H
#include <BALL/QSAR/modelFactory.h>
#endif

#include <map>

#ifdef BALL_HAS_LIBSVM
# include <BALL/QSAR/libsvmModel.h>
#endif

namespace BALL
{
	namespace QSAR
	{
		class QSARData;

		typedef Model* (*CreateMethod) (const QSARData& q);
		typedef Model* (*CreateKernel1) (const QSARData& q, int k, double p1, double p2);
		typedef Model* (*CreateKernel2) (const QSARData& q, String s1, String s2);
		
		class Registry;
		
		class BALL_EXPORT RegistryEntry
		{
			public:
			
				/** Constructor for a RegistryEntry for a linear model.
				@param ID *unique* identifier for the model
				@param n the name of the model 
				@param ab *unique* abbreviation of the model-name */
				RegistryEntry(bool k, bool r, String n, String ab, CreateMethod c0);
				
				/** Constructor for a RegistryEntry for a non-linear model.
				@param ID *unique* identifier for the model 
				@param n the name of the model 
				@param ab *unique* abbreviation of the model-name */
				RegistryEntry(bool k, bool r, String n, String ab, CreateKernel1 c1, CreateKernel2 c2);
				
				RegistryEntry(const RegistryEntry& entry);
				
				~RegistryEntry();
				
				const std::map<unsigned int,String>* getStatistics();
				String getStatName(int s);

				bool kernel;
				bool regression; // regression or classification?!
				bool latent_variables; // is this a latent variable model?!
				std::multiset<int> optimizableParameters;
				//bool optimizable;
				String name; // name of the Model
				String name_abreviation;
				CreateMethod create;
				CreateKernel1 createKernel1;
				CreateKernel2 createKernel2;
				vector<String> parameterNames;
				vector<double> parameterDefaults;
				Registry* getRegistry();
				
			private:
				Registry* registry_;
				
				friend class Registry;
		};
		
		
		typedef std::map<int,RegistryEntry>::iterator RegistryEntryIterator;
		
		class BALL_EXPORT Registry
		{
			public:
				Registry();

				~Registry();
				
				/** default value for first parameter of non-rbf kernels */
				double default_kernel_par1;
				
				/** default value for second parameter of non-rbf kernels */
				double default_kernel_par2;
				
				/** default value for parameter of rbf kernels */
				double default_rbf_par;
				
				/** default value for k-fold cross validation */
				int default_k;
				
				/** the default number of permutations to be done during response permutation testing */
				int default_no_permutations;
				
				/** the default number of boostrap samples */
				int default_no_boostrap_samples;
				
				/** the default value for the fraction of the input data that should be set aside in case of external/nested validation */ 
				double default_valdition_fraction;
				
				/** the default number of nested validation folds */
				int default_nested_folds;
				
				/** the default value for the absolute value of the correlation coefficient for removing of nearly colinear features */
				double default_correlation_cutoff;
				
				double default_gridsearch_stepwidth;
				int default_gridsearch_recursion;
				int default_gridsearch_steps;
				double default_gridsearch_par1_start;
				double default_gridsearch_par2_start;
				
				void addEntry(RegistryEntry entry, int uniqueID);
				
				/** returns the RegistryEntry for a given model name */
				RegistryEntry* getEntry(String model_name);
				
				/** returns the RegistryEntry for a given model ID */
				RegistryEntry* getEntry(int ID);
				
				/** return the ID of a specified model */
				int getModelNo(String model_name);
				
				String getClassificationStatisticName(unsigned int no);
				String getRegressionStatisticName(unsigned int no);
				String getFeatureSelectionName(unsigned int no);
				String getValidationName(unsigned int no);
				const std::map<unsigned int,String>* getClassificationStatistics();
				const std::map<unsigned int,String>* getRegressionStatistics();
				
				/** returns an iterator to the first model in model_map */
				RegistryEntryIterator beginEntry();
				
				/** returns an iterator past the last model of model_map */
				RegistryEntryIterator endEntry();
				
				
			private:
				std::map<int,RegistryEntry> registered_models;
				
				/** enable fast finding of a RegistryEntry for a given model name */
				std::map<String,int> model_map;
				
				std::map<unsigned int,String> classification_statistics;
				std::map<unsigned int,String> regression_statistics;
				std::map<unsigned int,String> feature_selection_names;
				std::map<unsigned int,String> validation_names;
				
				friend class RegistryEntry;
		};
		

	}
}

#endif // BALL_QSAR_REGISTRY_H
