#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/featureSelection.h>
#include <iostream>

using namespace BALL::QSAR;
using namespace std;


void optimizeParameters(Model* model)
{
	try
	{
		model->optimizeParameters(5,20);
		KernelModel* km = dynamic_cast<KernelModel*>(model);
		if(km) km->kernel->gridSearch(0.25,20,0,5);
	}
	catch(Exception::SingularMatrixError e)
	{
		// do nothing; we just cannot optimize the model before feature selection
	}
}


void selectFeatures(Model* model)
{
	FeatureSelection fs(*model);
	double threshold = 0.95;
	fs.removeHighlyCorrelatedFeatures(threshold);
	optimizeParameters(model);
	fs.twinScan(5);
	optimizeParameters(model);
	fs.forwardSelection(5);
	optimizeParameters(model);
}


int main(int argc, char* argv[])
{
	if(argc!=4)
	{
		cout<<"Please specify: <data file> <output model file> <use random test-sets>"<<endl;
		return 0;
	}

	typedef unsigned int Size;

	Registry registry;
	QSARData input_data;
	input_data.readFromFile(argv[1]);
	bool use_regression = !input_data.checkforDiscreteY();
	bool use_random_testsets = atoi(argv[3]);
	Size no_folds = 3;
	if(use_random_testsets) no_folds = 10;

	Size best_model_id = 0;
	Size best_kernel_id = 0;
	double best_nested_quality = 0;

	/// Do 4-fold nested cross validation, including feature selection steps, for each model type and, if applicable, each kernel function

	cout<<"model-name  kernel  #features  nested Q^2  stddev"<<endl;
	cout<<"---------------------------------------------------"<<endl;

	cout<<std::setiosflags(std::ios::fixed)<<std::left;

	for(Size model_id=1; model_id<14; model_id++)
	{
		RegistryEntry* reg_entry;
		try
		{
			reg_entry = registry.getEntry(model_id);
		}
		catch(BALL::Exception::GeneralException e)
		{
			// a model with the current id does not exist
			continue;
		}

		if(use_regression!=reg_entry->regression) continue;

		Size no_kernel_types = 1;
		if(reg_entry->kernel) no_kernel_types=3;

		for(Size kernel_id=1; kernel_id<=no_kernel_types; kernel_id++)
		{
			double nested_q2 = 0;
			int no_features = 0;
			vector<double> q2_values;

			for(Size fold_id=0; fold_id<no_folds; fold_id++)
			{
				vector<QSARData*> sets;
				if(use_random_testsets)
				{
					// randomly select 25% of compounds for external validation set
					sets = input_data.generateExternalSet(0.25);
				}
				else
				{
					sets = input_data.evenSplit(no_folds,fold_id);
				}
				if(input_data.isDataCentered())
				{
					bool center_y = input_data.isResponseCentered();
					sets[0]->centerData(center_y); // train-partition
					sets[1]->centerData(center_y); // test-partition
				}

				Model* model;
				if(!reg_entry->kernel) model=(*reg_entry->create)(*sets[0]);
				else model=(*reg_entry->createKernel1)(*sets[0],kernel_id,1, -1);
				model->setParameters(reg_entry->parameterDefaults);
				optimizeParameters(model);

				// select relevant features using training partition
				selectFeatures(model);
				no_features += model->getDescriptorIDs()->size();

				// train model using only the training partition
				model->readTrainingData();
				model->train();

				// test fit to external validation data
				model->data = sets[1];
				model->model_val->testInputData(true);
				nested_q2 += model->model_val->getFitRes();
				q2_values.push_back(model->model_val->getFitRes());

				delete sets[0];
				delete sets[1];
				delete model;
			}

			nested_q2 /= no_folds;
			no_features /= no_folds;
			double stddev = Statistics::getStddev(q2_values);

			cout<<setw(10)<<reg_entry->name_abreviation<<"  ";
			if(reg_entry->kernel)
			{
				if(kernel_id==1) cout<<setw(6)<<"polyn.";
				else if(kernel_id==2) cout<<setw(6)<<"rbf";
				else if(kernel_id==3) cout<<setw(6)<<"sigm.";
			}
			else cout<<setw(6)<<"none";
			cout<<"  ";
			cout<<setw(9)<<no_features<<"  "<<setw(10)<<nested_q2<<"  "<<setw(6)<<stddev<<endl<<flush;

			double quality = nested_q2-stddev; // make sure to prefer models w/ low stddev
			if(quality>best_nested_quality)
			{
				best_nested_quality = quality;
				best_model_id = model_id;
				best_kernel_id = kernel_id;
			}
		}
	}

	if(best_nested_quality<0.45)
	{
		cerr<<"Sorry, no model with satisfactory prediction quality found!"<<endl;
		return 1;
	}

	/// Create best model using ENTIRE data set and save it to the desired filename
	RegistryEntry* reg_entry = registry.getEntry(best_model_id);
	Model* model;
	if(!reg_entry->kernel) model=(*reg_entry->create)(input_data);
	else model=(*reg_entry->createKernel1)(input_data,best_kernel_id,1, -1);
	model->setParameters(reg_entry->parameterDefaults);
	optimizeParameters(model);
	selectFeatures(model);
	model->readTrainingData();
	model->train();

	model->saveToFile(argv[2]);
	cout<<"A model with satisfactory prediction quality has been sucessfully created and saved to file."<<endl;
}

