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
				SortedList<int> optimizableParameters;
				//bool optimizable;
				String name; // name of the Model
				String name_abreviation;
				CreateMethod create;
				CreateKernel1 createKernel1;
				CreateKernel2 createKernel2;
				vector<String> parameterNames;
				vector<double> parameterDefaults;
				
			private:
				Registry* registry_;
		};
		
		
		
		class Registry
		{
			public:
				Registry();

				~Registry();
				
				vector<RegistryEntry> registered_models;
				
				double default_kernel_par1;
				
				double default_kernel_par2;
				
				/** default value for k-fold cross validation */
				int default_k;
				
				/** the default number of permutations to be done during response permutation testing */
				int default_no_permutations;
				
				/** the default number of boostrap samples */
				int default_no_boostrap_samples;
				
				/** returns the RegistryEntry for a given model name */
				RegistryEntry* getRegistryEntry(String model_name);
				
				/** return the ID of a specified model */
				int getModelNo(String model_name);
				
				/** enable fast finding of a RegistryEntry for a given model name */
				map<String,int> model_map;
				
				vector<String> classification_statistics;
		};
		

	}
}

#endif // QSAR_REGISTRY
