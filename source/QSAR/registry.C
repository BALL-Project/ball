#include <BALL/QSAR/registry.h>


using namespace BALL::QSAR;


Registry::Registry()
{
	default_kernel_par1 = 2;
	default_kernel_par2 = -1;
	
	/// add new Model classes here:
	
	RegistryEntry r0(0,1,"Multiple Linear Regression", "MLR", (CreateMethod) &ModelFactory<MLRModel>::create,this);
	registered_models.push_back(r0);
	
	RegistryEntry r00(0,1,"Ridge Regression","RR",(CreateMethod) &ModelFactory<RRModel>::create,this);
	r00.parameterNames.push_back("lambda");
	r00.parameterDefaults.push_back(0.003);
	registered_models.push_back(r00);
	
	RegistryEntry r01(0,1,"Principal Component Regression","PCR",(CreateMethod) &ModelFactory<PCRModel>::create,this);
	r01.parameterNames.push_back("Fraction of variance to be explained");
	r01.parameterDefaults.push_back(0.95);
	registered_models.push_back(r01);
	
	RegistryEntry r1(0,1,"Partial Least Squares","PLS",(CreateMethod) &ModelFactory<PLSModel>::create,this);
	r1.parameterNames.push_back("number of PLS components");
	r1.parameterDefaults.push_back(10);
	r1.optimizableParameters.push_back(0);
	registered_models.push_back(r1);

	RegistryEntry r2(0,1,"Orthogonal Partial Least Squares","OPLS",(CreateMethod) &ModelFactory<OPLSModel>::create,this);
	r2.parameterNames.push_back("number of OPLS components");
	r2.parameterNames.push_back("number of PLS components");
	r2.parameterDefaults.push_back(5);
	r2.parameterDefaults.push_back(10);
	r2.optimizableParameters.push_back(0);
	r2.optimizableParameters.push_back(1);
	registered_models.push_back(r2);
	
	RegistryEntry r3(0,1,"Automated Lazy Learning","ALL",(CreateMethod) &ModelFactory<ALLModel>::create,this);
	r3.parameterNames.push_back("kernel width");
	r3.parameterNames.push_back("lambda");
	r3.parameterDefaults.push_back(4);
	r3.parameterDefaults.push_back(0.003);
	r3.optimizableParameters.push_back(0);
	registered_models.push_back(r3);
	
	RegistryEntry r31(0,1,"k Nearest Neighbor Regression","KNN",(CreateMethod) &ModelFactory<KNNModel>::create,this);
	r31.parameterNames.push_back("k (number of nearest neighbors)");
	r31.parameterNames.push_back("lambda");
	r31.parameterDefaults.push_back(3);
	r31.parameterDefaults.push_back(0.003);
	r31.optimizableParameters.push_back(0);
	registered_models.push_back(r31);
	
	RegistryEntry r4(1,1,"Kernel Partial Least Squares","KPLS",(CreateKernel1) &ModelFactory<KPLSModel>::createKernel1, (CreateKernel2) &ModelFactory<KPLSModel>::createKernel2,this);
	r4.parameterNames.push_back("number of PLS components");
	r4.parameterDefaults.push_back(10);
	r4.optimizableParameters.push_back(0);
	registered_models.push_back(r4);
	
	RegistryEntry r5(1,1,"Kernel Principal Component Regression","KPCR",(CreateKernel1) &ModelFactory<KPCRModel>::createKernel1, (CreateKernel2) &ModelFactory<KPCRModel>::createKernel2,this);
	r5.parameterNames.push_back("Fraction of variance to be explained");
	r5.parameterDefaults.push_back(0.95);
	registered_models.push_back(r5);
	
	RegistryEntry r6(1,1,"Gausssian Process","GP",(CreateKernel1) &ModelFactory<GPModel>::createKernel1, (CreateKernel2) &ModelFactory<GPModel>::createKernel2,this);
	r6.parameterNames.push_back("lambda");
	r6.parameterDefaults.push_back(0.03);
	registered_models.push_back(r6);
	
	RegistryEntry r9(1,1,"Support Vector Regression","SVR",(CreateKernel1) &ModelFactory<LibsvmModel>::createKernel1, NULL,this);
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
	
	RegistryEntry r7(0,0,"Linear Discriminant Analysis","LDA",(CreateMethod) &ModelFactory<LDAModel>::create,this);
	r7.parameterNames.push_back("lambda");
	r7.parameterDefaults.push_back(0.03);
	registered_models.push_back(r7);
	
	RegistryEntry r8(0,0,"Logistical Regression","Logit",(CreateMethod) &ModelFactory<LogitModel>::create,this);
	registered_models.push_back(r8);
	
	RegistryEntry r10(0,0,"Simple Naive Bayes","snB",(CreateMethod) &ModelFactory<SNBModel>::create,this);
	r10.parameterNames.clear();
	r10.parameterDefaults.clear();
	registered_models.push_back(r10);
	
	RegistryEntry r11(0,0,"Naive Bayes","nB",(CreateMethod) &ModelFactory<NBModel>::create,this);
	r11.parameterNames.push_back("discretization steps");
	r11.parameterDefaults.push_back(4);
	registered_models.push_back(r11);
	
	
	classification_statistics.push_back("average accuracy");
	classification_statistics.push_back("weighted accuracy");
	classification_statistics.push_back("overall accuracy");
	classification_statistics.push_back("average MCC");
	classification_statistics.push_back("overall MCC");
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
		String mess = "A model with the name \""+model_name+"\"does not exist!";
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model creation error",mess.c_str());
	}
	return -1;	
}
		

RegistryEntry::RegistryEntry(bool k, bool r, String n, String ab, CreateMethod c0, Registry* reg)
{
	kernel = k;
	regression = r;
	name = n;
	name_abreviation = ab;
	create = c0;
	createKernel1 = NULL;
	createKernel2 = NULL;
	reg->model_map[ab] = reg->registered_models.size();
	registry_ = reg;
}


RegistryEntry::RegistryEntry(bool k, bool r, String n, String ab, CreateKernel1 c1, CreateKernel2 c2, Registry* reg)
{
	kernel = k;
	regression = r;
	name = n;
	name_abreviation = ab;
	create = NULL;
	createKernel1 = c1;
	createKernel2 = c2;
	reg->model_map[ab] = reg->registered_models.size();
	registry_ = reg;
}


const vector<BALL::String>* RegistryEntry::getStatistics()
{
	if(regression) return NULL;
	
	return &registry_->classification_statistics;
}


BALL::String RegistryEntry::getStatName(int s)
{
	vector<String>* statistics = 0;
	String name = ""; 
	if(!regression) statistics = &registry_->classification_statistics;
	
	if(statistics==0 || s<0 || s>=statistics->size()) return name;
		
	return (*statistics)[s];
}


RegistryEntry::~RegistryEntry()
{
}

