#ifndef QSAR_REGISTRY
#define QSAR_REGISTRY

#ifndef MODEL_FACTORY_H
#include <BALL/QSAR/modelFactory.h>
#endif

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/pcrModel.h>
#include <BALL/QSAR/mlrModel.h>
#include <BALL/QSAR/plsModel.h>
#include <BALL/QSAR/oplsModel.h>
#include <BALL/QSAR/libsvmModel.h>
#include <BALL/QSAR/kplsModel.h>
#include <BALL/QSAR/allModel.h>
#include <BALL/QSAR/svrModel.h>
#include <BALL/QSAR/gpModel.h>
#include <BALL/QSAR/kpcrModel.h>
#include <BALL/QSAR/ldaModel.h>
#include <BALL/QSAR/svmModel.h>
#include <BALL/QSAR/logitModel.h>
#include <BALL/QSAR/libsvmModel.h>
#include <BALL/QSAR/knnModel.h>
#include <BALL/QSAR/rrModel.h>
#include <BALL/QSAR/snBModel.h>
#include <BALL/QSAR/nBModel.h>

namespace BALL
{
	namespace QSAR
	{
		
		typedef Model* (*CreateMethod) (const QSARData& q);
		typedef Model* (*CreateKernel1) (const QSARData& q, int k, double p1, double p2);
		typedef Model* (*CreateKernel2) (const QSARData& q, String s1, String s2);
		
		class Registry;
		
		class RegistryEntry
		{
			public:
			
				RegistryEntry(bool k, bool r, String n, String ab, CreateMethod c0, Registry* reg);
				RegistryEntry(bool k, bool r, String n, String ab, CreateKernel1 c1, CreateKernel2 c2, Registry* reg);
				~RegistryEntry();
				
				const vector<String>* getStatistics();
				String getStatName(int s);

				bool kernel;
				bool regression; // regression or classification?!
				bool latent_variables; // is this a latent variable model?!
				SortedList<int> optimizableParameters;
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
		};
		
		
		
		class Registry
		{
			public:
				Registry();

				~Registry();
				
				vector<RegistryEntry> registered_models;
				
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
				
				/** returns the RegistryEntry for a given model name */
				RegistryEntry* getRegistryEntry(String model_name);
				
				/** return the ID of a specified model */
				int getModelNo(String model_name);
				
				const vector<String>* getClassificationStatistics();
				const vector<String>* getRegressionStatistics();
				const vector<String>* getFeatureSelectionNames();
				
				
			private:
				/** enable fast finding of a RegistryEntry for a given model name */
				map<String,int> model_map;
				
				vector<String> classification_statistics;
				vector<String> feature_selection_names;
				
				friend class RegistryEntry;
		};
		

	}
}

#endif // QSAR_REGISTRY
