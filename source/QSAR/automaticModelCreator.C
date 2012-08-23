#include <BALL/QSAR/automaticModelCreator.h>
#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/featureSelection.h>

using namespace BALL::QSAR;
using namespace std;


AutomaticModelCreator::AutomaticModelCreator(const QSARData* data)
{
	data_ = data;
	min_quality_ = 0.45;
}


void AutomaticModelCreator::setData(const QSARData* data)
{
	data_ = data;
}

void AutomaticModelCreator::setMinQuality(double min_quality)
{
	min_quality_ = min_quality;
}


void AutomaticModelCreator::optimizeParameters(Model* model)
{
	try
	{
		model->optimizeParameters(5, 20);
		KernelModel* km = dynamic_cast<KernelModel*>(model);
		if (km) km->kernel->gridSearch(0.25, 20, 0, 5); 
	}
	catch(Exception::SingularMatrixError e)
	{
		// do nothing; we just cannot optimize the model before feature selection
	}
}

void AutomaticModelCreator::selectFeatures(Model* model)
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


Model* AutomaticModelCreator::generateModel()
{
	Registry registry;
	bool use_regression = !data_->checkforDiscreteY(); 
	bool use_random_testsets = 0;
	Size no_folds = 3;
	if (use_random_testsets) no_folds = 10; 

	Size best_model_id = 0;
	Size best_kernel_id = 0;
	double best_nested_quality = 0;

	/// Do 4-fold nested cross validation, including feature selection steps, for each model type and, if applicable, each kernel function

	Log.level(10)<<"model-name  kernel  #features  nested Q^2  stddev"<<std::endl;
	Log.level(10)<<"---------------------------------------------------"<<std::endl;

	Log.level(10)<<std::setiosflags(std::ios::fixed)<<std::left;

	for (Size model_id = 1; model_id < 14; model_id++)
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

		if (use_regression != reg_entry->regression) continue; 

		Size no_kernel_types = 1;
		if (reg_entry->kernel) no_kernel_types = 3; 

		for (Size kernel_id = 1; kernel_id <= no_kernel_types; kernel_id++)
		{
			double nested_q2 = 0;
			int no_features = 0;
			vector<double> q2_values;

			for (Size fold_id = 0; fold_id < no_folds; fold_id++)
			{
				vector<QSARData*> sets;
				if (use_random_testsets)
				{
					// randomly select 25% of compounds for external validation set
					sets = data_->generateExternalSet(0.25);
				}
				else
				{
					sets = data_->evenSplit(no_folds, fold_id);
				}
				if (data_->isDataCentered())
				{
					bool center_y = data_->isResponseCentered();
					sets[0]->centerData(center_y); // train-partition
					sets[1]->centerData(center_y); // test-partition
				}

				Model* model;
				if (!reg_entry->kernel) model = (*reg_entry->create)(*sets[0]); 
				else model = (*reg_entry->createKernel1)(*sets[0], kernel_id, 1, -1);
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

			Log.level(10)<<std::setw(10)<<reg_entry->name_abreviation<<"  ";
			if (reg_entry->kernel)
			{
				if (kernel_id == 1) Log.level(10)<<setw(6)<<"polyn."; 
				else if (kernel_id == 2) Log.level(10)<<setw(6)<<"rbf"; 
				else if (kernel_id == 3) Log.level(10)<<setw(6)<<"sigm."; 
			}
			else Log.level(10)<<setw(6)<<"none";
			Log.level(10)<<"  ";
			Log.level(10)<<setw(9)<<no_features<<"  "<<setw(10)<<nested_q2<<"  "<<setw(6)<<stddev<<endl<<flush;

			double quality = nested_q2-stddev; // make sure to prefer models w/ low stddev
			if (quality > best_nested_quality)
			{
				best_nested_quality = quality;
				best_model_id = model_id;
				best_kernel_id = kernel_id;
			}
		}
	}

	if (best_nested_quality < min_quality_)
	{
		Log.level(10)<<"Sorry, no model with satisfactory prediction quality found!"<<endl;
		return 0;
	}

	/// Create best model using ENTIRE data set and return it
	RegistryEntry* reg_entry = registry.getEntry(best_model_id);
	Model* model;
	if (!reg_entry->kernel) model = (*reg_entry->create)(*data_); 
	else model = (*reg_entry->createKernel1)(*data_, best_kernel_id, 1, -1);
	model->setParameters(reg_entry->parameterDefaults);
	optimizeParameters(model);
	selectFeatures(model);
	model->readTrainingData();
	model->train();

	return model;
}

