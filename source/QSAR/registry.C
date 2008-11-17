#include <BALL/QSAR/registry.h>


using namespace BALL::QSAR;
using namespace BALL;

Registry::Registry()
{
	default_kernel_par1 = 2; // default value for first parameter of non-rbf kernels
	default_kernel_par2 = -1; // default value for second parameter of non-rbf kernels
	default_rbf_par=0.005; // default value for parameter of rbf kernels
	
	default_k = 5;
	default_no_permutations = 10;
	default_no_boostrap_samples = 10;
	default_valdition_fraction = 0.2;
	default_nested_folds = 4;
	default_correlation_cutoff = 0.97;
	
	default_gridsearch_stepwidth = 0.25;
	default_gridsearch_recursion = 0;
	default_gridsearch_steps = 20;
	default_gridsearch_par1_start = 0.25;
	default_gridsearch_par2_start = -0.25;
	
	/// add new Model classes here:
	
	RegistryEntry r0(0,1,"Multiple Linear Regression", "MLR", (CreateMethod) &ModelFactory<MLRModel>::create,this,0);
	registered_models.push_back(r0);
	
	RegistryEntry r00(0,1,"Ridge Regression","RR",(CreateMethod) &ModelFactory<RRModel>::create,this,1);
	r00.parameterNames.push_back("lambda");
	r00.parameterDefaults.push_back(0.003);
	registered_models.push_back(r00);
	
	RegistryEntry r01(0,1,"Principal Component Regression","PCR",(CreateMethod) &ModelFactory<PCRModel>::create,this,2);
	r01.parameterNames.push_back("Fraction of variance to be explained");
	r01.parameterDefaults.push_back(0.95);
	r01.latent_variables=1;
	registered_models.push_back(r01);
	
	RegistryEntry r1(0,1,"Partial Least Squares","PLS",(CreateMethod) &ModelFactory<PLSModel>::create,this,3);
	r1.parameterNames.push_back("number of PLS components");
	r1.parameterDefaults.push_back(10);
	r1.optimizableParameters.push_back(0);
	r1.latent_variables=1;
	registered_models.push_back(r1);

	RegistryEntry r2(0,1,"Orthogonal Partial Least Squares","OPLS",(CreateMethod) &ModelFactory<OPLSModel>::create,this,4);
	r2.parameterNames.push_back("number of OPLS components");
	r2.parameterNames.push_back("number of PLS components");
	r2.parameterDefaults.push_back(5);
	r2.parameterDefaults.push_back(10);
	r2.optimizableParameters.push_back(0);
	r2.optimizableParameters.push_back(1);
	r2.latent_variables=1;
	registered_models.push_back(r2);
	
	RegistryEntry r3(0,1,"Automated Lazy Learning","ALL",(CreateMethod) &ModelFactory<ALLModel>::create,this,5);
	r3.parameterNames.push_back("kernel width");
	r3.parameterNames.push_back("lambda");
	r3.parameterDefaults.push_back(6);
	r3.parameterDefaults.push_back(0.003);
	r3.optimizableParameters.push_back(0);
	registered_models.push_back(r3);
	
	RegistryEntry r31(0,1,"k Nearest Neighbor Regression","KNN",(CreateMethod) &ModelFactory<KNNModel>::create,this,6);
	r31.parameterNames.push_back("k (number of nearest neighbors)");
	r31.parameterNames.push_back("lambda");
	r31.parameterDefaults.push_back(3);
	r31.parameterDefaults.push_back(0.003);
	r31.optimizableParameters.push_back(0);
	registered_models.push_back(r31);
	
	RegistryEntry r4(1,1,"Kernel Partial Least Squares","KPLS",(CreateKernel1) &ModelFactory<KPLSModel>::createKernel1, (CreateKernel2) &ModelFactory<KPLSModel>::createKernel2,this,7);
	r4.parameterNames.push_back("number of PLS components");
	r4.parameterDefaults.push_back(10);
	r4.optimizableParameters.push_back(0);
	r4.latent_variables=1;
	registered_models.push_back(r4);
	
	RegistryEntry r5(1,1,"Kernel Principal Component Regression","KPCR",(CreateKernel1) &ModelFactory<KPCRModel>::createKernel1, (CreateKernel2) &ModelFactory<KPCRModel>::createKernel2,this,8);
	r5.parameterNames.push_back("Fraction of variance to be explained");
	r5.parameterDefaults.push_back(0.95);
	r5.latent_variables=1;
	registered_models.push_back(r5);
	
	RegistryEntry r6(1,1,"Gausssian Process","GP",(CreateKernel1) &ModelFactory<GPModel>::createKernel1, (CreateKernel2) &ModelFactory<GPModel>::createKernel2,this,9);
	r6.parameterNames.push_back("lambda");
	r6.parameterDefaults.push_back(0.03);
	registered_models.push_back(r6);
	
	RegistryEntry r9(1,1,"Support Vector Regression","SVR",(CreateKernel1) &ModelFactory<LibsvmModel>::createKernel1, NULL,this,10);
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
	registered_models.push_back(r9);
	
	RegistryEntry r7(0,0,"Linear Discriminant Analysis","LDA",(CreateMethod) &ModelFactory<LDAModel>::create,this,11);
	r7.parameterNames.push_back("lambda");
	r7.parameterDefaults.push_back(0.03);
	registered_models.push_back(r7);
	
// 	RegistryEntry r8(0,0,"Logistical Regression","Logit",(CreateMethod) &ModelFactory<LogitModel>::create,this,12);
// 	registered_models.push_back(r8);
	
	RegistryEntry r10(0,0,"Simple Naive Bayes","snB",(CreateMethod) &ModelFactory<SNBModel>::create,this,13);
	r10.parameterNames.clear();
	r10.parameterDefaults.clear();
	registered_models.push_back(r10);
	
	RegistryEntry r11(0,0,"Naive Bayes","nB",(CreateMethod) &ModelFactory<NBModel>::create,this,14);
	r11.parameterNames.push_back("discretization steps");
	r11.parameterDefaults.push_back(4);
	registered_models.push_back(r11);
	
	classification_statistics[0] = "average sensitivity";
	classification_statistics[1] = "weighted sensitivity";
	classification_statistics[2] = "overall accuracy";
	classification_statistics[3] = "average MCC";
	classification_statistics[4] = "overall MCC";
	
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

RegistryEntry* Registry::getRegistryEntry(String model_name)
{
	map<String,int>::iterator it = model_map.find(model_name);
	if(it!=model_map.end())
	{
		return &registered_models[it->second];
	}
	else
	{
		String mess = "A model with the name \""+model_name+"\"does not exist!";
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model creation error",mess.c_str());
	}	
	return NULL;
}
		
int Registry::getModelNo(String model_name)
{
	map<String,int>::iterator it = model_map.find(model_name);
	if(it!=model_map.end())
	{
		return it->second;
	}
	else
	{
		String mess = "A model with the name \""+model_name+"\" does not exist!";
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model-name error",mess.c_str());
	}
	return -1;	
}


String Registry::getValidationName(uint no)
{
	map<uint,String>::iterator it = validation_names.find(no);
	if(it!=validation_names.end())
	{
		return it->second;
	}
	else
	{
		String mess = "A validation-type for number \""+String(no)+"\" does not exist!";
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Validation-number error",mess.c_str());
	}
	return "";
}


String Registry::getClassificationStatisticName(uint no)
{
	map<uint,String>::iterator it = classification_statistics.find(no);
	if(it!=classification_statistics.end())
	{
		return it->second;
	}
	else
	{
		String mess = "A classification-statistic for number \""+String(no)+"\" does not exist!";
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"classification-statistic error",mess.c_str());
	}
	return "";
}

String Registry::getFeatureSelectionName(uint no)
{
	map<uint,String>::iterator it = feature_selection_names.find(no);
	if(it!=feature_selection_names.end())
	{
		return it->second;
	}
	else
	{
		String mess = "A feature-selection for number \""+String(no)+"\" does not exist!";
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"feature-selecion error",mess.c_str());
	}
	return "";
}

const map<uint,String>* Registry::getClassificationStatistics()
{
	return &classification_statistics;
}
	
	

RegistryEntry::RegistryEntry(bool k, bool r, String n, String ab, CreateMethod c0, Registry* reg, uint ID)
{
	kernel = k;
	regression = r;
	name = n;
	name_abreviation = ab;
	create = c0;
	createKernel1 = NULL;
	createKernel2 = NULL;
	if(reg->model_map.find(ab)!=reg->model_map.end())
	{
		String mess = "A RegistryEntry for key\""+String(ab)+"\" already exists!";
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Registry error",mess.c_str());
	}
	reg->model_map[ab] = ID;
	registry_ = reg;
	latent_variables = 0;
}


RegistryEntry::RegistryEntry(bool k, bool r, String n, String ab, CreateKernel1 c1, CreateKernel2 c2, Registry* reg, uint ID)
{
	kernel = k;
	regression = r;
	name = n;
	name_abreviation = ab;
	create = NULL;
	createKernel1 = c1;
	createKernel2 = c2;
	if(reg->model_map.find(ab)!=reg->model_map.end())
	{
		String mess = "A RegistryEntry for key\""+String(ab)+"\" already exists!";
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Registry error",mess.c_str());
	}
	reg->model_map[ab] = ID;
	registry_ = reg;
	latent_variables = 0;
}


const map<uint,String>* RegistryEntry::getStatistics()
{
	if(regression) return NULL;
	
	return &registry_->classification_statistics;
}


BALL::String RegistryEntry::getStatName(int s)
{
	if(!regression) return registry_->getClassificationStatisticName(s);
	
	return "";
}


RegistryEntry::~RegistryEntry()
{
}


Registry* RegistryEntry::getRegistry()
{
	return registry_;
}

