#include <BALL/QSAR/registry.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/svmModel.h>
#include <BALL/QSAR/logitModel.h>
#include <BALL/QSAR/snBModel.h>
#include <BALL/QSAR/nBModel.h>

#include <BALL/QSAR/allModel.h>
#include <BALL/QSAR/gpModel.h>
#include <BALL/QSAR/knnModel.h>
#include <BALL/QSAR/kpcrModel.h>
#include <BALL/QSAR/kplsModel.h>
#include <BALL/QSAR/ldaModel.h>
#include <BALL/QSAR/mlrModel.h>
#include <BALL/QSAR/oplsModel.h>
#include <BALL/QSAR/pcrModel.h>
#include <BALL/QSAR/plsModel.h>
#include <BALL/QSAR/rrModel.h>

using namespace std;

namespace BALL
{
	namespace QSAR
	{
		Registry::Registry()
		{
			default_kernel_par1 = 2; // default value for first parameter of non-rbf kernels
			default_kernel_par2 = -1; // default value for second parameter of non-rbf kernels
			default_rbf_par = 0.005; // default value for parameter of rbf kernels
			
			default_k = 5;
			default_no_permutations = 10;
			default_no_boostrap_samples = 50;
			default_valdition_fraction = 0.25;
			default_nested_folds = 4;
			default_correlation_cutoff = 0.95;
			
			default_gridsearch_stepwidth = 0.25;
			default_gridsearch_recursion = 0;
			default_gridsearch_steps = 20;
			default_gridsearch_par1_start = 0.25;
			default_gridsearch_par2_start = -0.25;
			
			/// add new Model classes here:
			RegistryEntry r0(0, 1, "Multiple Linear Regression", "MLR", (CreateMethod) &ModelFactory<MLRModel>::create);
			addEntry(r0, 0);
			
			RegistryEntry r00(0, 1, "Ridge Regression", "RR", (CreateMethod) &ModelFactory<RRModel>::create);
			r00.parameterNames.push_back("lambda");
			r00.parameterDefaults.push_back(0.003);
			addEntry(r00, 1);
			
			RegistryEntry r01(0, 1, "Principal Component Regression", "PCR", (CreateMethod) &ModelFactory<PCRModel>::create);
			r01.parameterNames.push_back("Fraction of variance to be explained");
			r01.parameterDefaults.push_back(0.95);
			r01.latent_variables = 1;
			addEntry(r01, 2);
			
			RegistryEntry r1(0, 1, "Partial Least Squares", "PLS", (CreateMethod) &ModelFactory<PLSModel>::create);
			r1.parameterNames.push_back("number of PLS components");
			r1.parameterDefaults.push_back(10);
			r1.optimizableParameters.insert(0);
			r1.latent_variables = 1;
			addEntry(r1, 3);

			RegistryEntry r2(0, 1, "Orthogonal Partial Least Squares", "OPLS", (CreateMethod) &ModelFactory<OPLSModel>::create);
			r2.parameterNames.push_back("number of OPLS components");
			r2.parameterNames.push_back("number of PLS components");
			r2.parameterDefaults.push_back(5);
			r2.parameterDefaults.push_back(10);
			r2.optimizableParameters.insert(0);
			r2.optimizableParameters.insert(1);
			r2.latent_variables = 1;
			addEntry(r2, 4);

			RegistryEntry r3(0, 1, "Automated Lazy Learning", "ALL", (CreateMethod) &ModelFactory<ALLModel>::create);
			r3.parameterNames.push_back("kernel width");
			r3.parameterNames.push_back("lambda");
			r3.parameterDefaults.push_back(6);
			r3.parameterDefaults.push_back(0.003);
			r3.optimizableParameters.insert(0);
			addEntry(r3, 5);

			RegistryEntry r31(0, 1, "k Nearest Neighbor Regression", "KNN", (CreateMethod) &ModelFactory<KNNModel>::create);
			r31.parameterNames.push_back("k (number of nearest neighbors)");
			r31.parameterNames.push_back("lambda");
			r31.parameterDefaults.push_back(3);
			r31.parameterDefaults.push_back(0.003);
			r31.optimizableParameters.insert(0);
			addEntry(r31, 6);

			RegistryEntry r4(1, 1, "Kernel Partial Least Squares", "KPLS", (CreateKernel1) &ModelFactory<KPLSModel>::createKernel1, (CreateKernel2) &ModelFactory<KPLSModel>::createKernel2);
			r4.parameterNames.push_back("number of PLS components");
			r4.parameterDefaults.push_back(10);
			r4.optimizableParameters.insert(0);
			r4.latent_variables = 1;
			addEntry(r4, 7);

			RegistryEntry r5(1, 1, "Kernel Principal Component Regression", "KPCR", (CreateKernel1) &ModelFactory<KPCRModel>::createKernel1, (CreateKernel2) &ModelFactory<KPCRModel>::createKernel2);
			r5.parameterNames.push_back("Fraction of variance to be explained");
			r5.parameterDefaults.push_back(0.95);
			r5.latent_variables = 1;
			addEntry(r5, 8);

			RegistryEntry r6(1, 1, "Gausssian Process", "GP", (CreateKernel1) &ModelFactory<GPModel>::createKernel1, (CreateKernel2) &ModelFactory<GPModel>::createKernel2);
			r6.parameterNames.push_back("lambda");
			r6.parameterDefaults.push_back(0.03);
			addEntry(r6, 9);

		#ifdef BALL_HAS_LIBSVM
			RegistryEntry r9(1, 1, "Support Vector Regression", "SVR", (CreateKernel1) &ModelFactory<LibsvmModel>::createKernel1, NULL);
			r9.parameterNames.push_back("use nu-SVR (else epsilon-SVR)?");
			r9.parameterNames.push_back("use shrinking heuristic?");
			r9.parameterNames.push_back("nu");
			r9.parameterNames.push_back("epsilon");
			r9.parameterNames.push_back("stopping criteria");
			r9.parameterNames.push_back("constraint violation cost");
			r9.parameterDefaults.push_back(1);
			r9.parameterDefaults.push_back(1);
			r9.parameterDefaults.push_back(0.05);
			r9.parameterDefaults.push_back(0.1);
			r9.parameterDefaults.push_back(1e-3);
			r9.parameterDefaults.push_back(1);
			addEntry(r9, 10);
		#endif
			RegistryEntry r7(0, 0, "Linear Discriminant Analysis", "LDA", (CreateMethod) &ModelFactory<LDAModel>::create);
			r7.parameterNames.push_back("lambda");
			r7.parameterDefaults.push_back(0.03);
			addEntry(r7, 11);
		// 	RegistryEntry r8(0, 0, "Logistical Regression", "Logit", (CreateMethod) &ModelFactory<LogitModel>::create);
		// 	addEntry(r8, 12);
			
			RegistryEntry r10(0, 0, "Simple Naive Bayes", "snB", (CreateMethod) &ModelFactory<SNBModel>::create);
			r10.parameterNames.push_back("min prob difference"); 
			r10.parameterNames.push_back("class-id for undef. activity"); 
			r10.parameterDefaults.push_back(0);
			r10.parameterDefaults.push_back(0);
			addEntry(r10, 13);
			
			RegistryEntry r11(0, 0, "Naive Bayes", "nB", (CreateMethod) &ModelFactory<NBModel>::create);
			r11.parameterNames.push_back("discretization steps");
			r11.parameterNames.push_back("min prob difference"); 
			r11.parameterNames.push_back("class-id for undef. activity"); 
			r11.parameterDefaults.push_back(4);
			r11.parameterDefaults.push_back(0);
			r11.parameterDefaults.push_back(0);
			addEntry(r11, 14);
			
			
			
			classification_statistics[0] = "average sensitivity"; 
			classification_statistics[1] = "weighted sensitivity"; 
			classification_statistics[2] = "overall accuracy"; 
			classification_statistics[3] = "average MCC"; 
			classification_statistics[4] = "overall MCC"; 
			classification_statistics[5] = "True Discovery Rate"; 
			
			regression_statistics[0] = "R2/Q2";
			
			feature_selection_names[0] = "Remove Colinear Features";
			feature_selection_names[1] = "forward selection"; 
			feature_selection_names[2] = "backward selection";
			feature_selection_names[3] = "stepwise selection";
			feature_selection_names[4] = "Remove Low Response Correlation";
			feature_selection_names[5] = "Remove Insignificant Coefficients"; 
			feature_selection_names[6] = "TwinScan";
			
			validation_names[1] = "test fit to training data";
			validation_names[2] = "cross validation";
			validation_names[3] = "bootstrapping";
			validation_names[4] = "response permutation test";
			validation_names[5] = "nested cross validation";
			validation_names[6] = "calculate coefficient stddev";
			validation_names[7] = "test fit to external data";
		}


		Registry::~Registry()
		{
		}

		RegistryEntry* Registry::getEntry(String model_name)
		{
			map<String, int>::iterator name_it = model_map.find(model_name);
			bool ok = 0;
			if (name_it != model_map.end())
			{
				map<int, RegistryEntry>::iterator model_it = registered_models.find(name_it->second);
				if (model_it != registered_models.end())
				{	
					return &model_it->second;
				}
			}
			if (!ok)
			{
				String mess = "A model with the name \""+model_name+"\"does not exist!";
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Model creation error", mess.c_str());
			}	
			return NULL;
		}

		RegistryEntry* Registry::getEntry(int ID)
		{
			map<int, RegistryEntry>::iterator it = registered_models.find(ID);
			if (it != registered_models.end())
			{
				return &it->second;
			}
			else
			{
				String mess = "A model with the ID \""+String(ID)+"\"does not exist!";
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Model creation error", mess.c_str());
			}	
			return NULL;
		}
				
		int Registry::getModelNo(String model_name)
		{
			map<String, int>::iterator it = model_map.find(model_name);
			if (it != model_map.end())
			{
				return it->second;
			}
			else
			{
				String mess = "A model with the name \""+model_name+"\" does not exist!";
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Model-name error", mess.c_str());
			}
			return -1;	
		}


		String Registry::getValidationName(unsigned int no)
		{
			map<unsigned int, String>::iterator it = validation_names.find(no);
			if (it != validation_names.end())
			{
				return it->second;
			}
			else
			{
				String mess = "A validation-type for number \""+String(no)+"\" does not exist!"; 
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Validation-number error", mess.c_str());
			}
			return "";
		}


		String Registry::getClassificationStatisticName(unsigned int no)
		{
			map < unsigned int, String > ::iterator it = classification_statistics.find(no); 
			if (it != classification_statistics.end())
			{
				return it->second;
			}
			else
			{
				String mess = "A classification-statistic for number \""+String(no)+"\" does not exist!"; 
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "classification-statistic error", mess.c_str()); 
			}
			return "";
		}

		String Registry::getRegressionStatisticName(unsigned int no)
		{
			map<unsigned int, String>::iterator it = regression_statistics.find(no);
			if (it != regression_statistics.end())
			{
				return it->second;
			}
			else
			{
				String mess = "A regression-statistic for number \""+String(no)+"\" does not exist!"; 
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "regression-statistic error", mess.c_str());
			}
			return "";
		}

		String Registry::getFeatureSelectionName(unsigned int no)
		{
			map<unsigned int, String>::iterator it = feature_selection_names.find(no);
			if (it != feature_selection_names.end())
			{
				return it->second;
			}
			else
			{
				String mess = "A feature-selection for number \""+String(no)+"\" does not exist!"; 
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "feature-selecion error", mess.c_str());
			}
			return "";
		}

		const map < unsigned int, String > * Registry::getClassificationStatistics()
		{
			return &classification_statistics; 
		}

		const map<unsigned int, String>* Registry::getRegressionStatistics()
		{
			return &regression_statistics;
		}

		void Registry::addEntry(RegistryEntry entry, int uniqueID)
		{
			if (model_map.find(entry.name_abreviation) != model_map.end())
			{
				String mess = "A RegistryEntry for key\""+String(entry.name_abreviation)+"\" already exists!"; 
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Registry error", mess.c_str());
			}
			if (registered_models.find(uniqueID) != registered_models.end())
			{
				String mess = "A RegistryEntry for ID\""+String(uniqueID)+"\" already exists!"; 
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Registry error", mess.c_str());
			}
			entry.registry_ = this;
			model_map[entry.name_abreviation] = uniqueID;
			registered_models.insert(make_pair(uniqueID, entry));	
		}

		RegistryEntryIterator Registry::beginEntry()
		{
			return registered_models.begin();
		}

		RegistryEntryIterator Registry::endEntry()
		{
			return registered_models.end();
		}



		//  -----  RegistryEntry  -------
			

		RegistryEntry::RegistryEntry(bool k, bool r, String n, String ab, CreateMethod c0)
		{
			kernel = k;
			regression = r;
			name = n;
			name_abreviation = ab;
			create = c0;
			createKernel1 = NULL;
			createKernel2 = NULL;
			latent_variables = 0;
			parameterNames.resize(0);
			parameterDefaults.resize(0);
			optimizableParameters.clear();
		}


		RegistryEntry::RegistryEntry(bool k, bool r, String n, String ab, CreateKernel1 c1, CreateKernel2 c2)
		{
			kernel = k;
			regression = r;
			name = n;
			name_abreviation = ab;
			create = NULL;
			createKernel1 = c1;
			createKernel2 = c2;
			latent_variables = 0;
			parameterNames.resize(0);
			parameterDefaults.resize(0);
			optimizableParameters.clear();
		}

		RegistryEntry::RegistryEntry(const RegistryEntry& entry)
		{
			kernel = entry.kernel;
			regression = entry.regression;
			name = entry.name;
			name_abreviation = entry.name_abreviation;
			create = entry.create;
			createKernel1 = entry.createKernel1;
			createKernel2 = entry.createKernel2;
			latent_variables = entry.latent_variables;
			parameterNames = entry.parameterNames;
			parameterDefaults = entry.parameterDefaults;
			optimizableParameters = entry.optimizableParameters;
			registry_ = entry.registry_;
		}


		const map<unsigned int, String>* RegistryEntry::getStatistics()
		{
			if (regression) return &registry_->regression_statistics; 
			return &registry_->classification_statistics; 
		}


		BALL::String RegistryEntry::getStatName(int s)
		{
			if (!regression) return registry_->getClassificationStatisticName(s); 
			return registry_->getRegressionStatisticName(s);
		}


		RegistryEntry::~RegistryEntry()
		{
		}


		Registry* RegistryEntry::getRegistry()
		{
			return registry_;
		}
	}
}
