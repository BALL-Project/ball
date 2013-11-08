#include <BALL/SCORING/FUNCTIONS/rescoring.h>
#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/SCORING/COMPONENTS/aromaticRingStacking.h>
#include <BALL/SCORING/COMPONENTS/polarity.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/featureSelection.h>
#include <BALL/QSAR/statistics.h>
#include <BALL/QSAR/plsModel.h>
#include <BALL/QSAR/kplsModel.h>
#include <BALL/QSAR/automaticModelCreator.h>

using namespace BALL;
using namespace std;

Rescoring::Rescoring(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options, String free_energy_label, ScoringFunction* sf)
{
	name_ = "";
	if (!sf)
	{
		scoring_function_ = new MMScoring(receptor, reference_ligand, options);
	}
	else
	{
		scoring_function_ = sf;
	}

	free_energy_label_ = free_energy_label;
	ff_filename_ = options.get("filename");

	convert_ = (free_energy_label_.hasSubstring("Ki")||free_energy_label_.hasSubstring("IC50"));
	if (convert_)
	{
		Log.level(10)<<"[info:] using Ki or IC50 as input, will convert them to free-energy values."<<endl<<endl;
	}

	use_calibration_ = 0;
	setup_();
}

void Rescoring::setup_()
{
	convert_ = (free_energy_label_.hasSubstring("Ki")||free_energy_label_.hasSubstring("IC50"));
	if (convert_)
	{
		Log.level(10)<<"[info:] using Ki or IC50 as input, will convert them to free-energy values."<<endl<<endl;
	}

	data_.Y_.resize(1);
	data_.descriptor_matrix_.resize(0);
	model_ = 0;
	check_applicability_ = 1;
	stddev_out_.open("outliers.txt");
}


Rescoring::~Rescoring()
{
	//delete scoring_function_;
	delete model_;
}


void Rescoring::addScoreContributions(Molecule* mol)
{
	// add affinity data
	if (mol->hasProperty(free_energy_label_))
	{
		float value = ((String)(mol->getProperty(free_energy_label_).toString())).toFloat();
		if (convert_)
		{
			value *= 1e-09;
			value = 1.987*298.15*log(value)*4.184/1000;
			mol->setProperty("binding free energy", value);
		}

		// In some datasets the IC50 for some molecules is annotated as 0nM, which is of course nonsense (-> pIC50 = -inf). Thus, we will ignore such molecules.
		if (Maths::isInfinite(value) /* || value > 0*/ || value < -90)
		{
			Log.level(20)<<"invalid free-energy value found, ignoring this molecule!!"<<endl;
			return;
		}

		data_.Y_[0].push_back(value);
		String name = mol->getName();
		if (name == "") name = "noname";
		data_.substance_names_.push_back(name);
	}
	else
	{
		Log.level(20)<<"[warning:] no free-energy property found, skipping this molecule!"<<endl;
		return;
	}

	if (data_.descriptor_matrix_.size() == 0)
	{
		vector<double> v;
		generateScoreContributions_(mol, 0, &v);
		Size no_columns = v.size();
		data_.column_names_.resize(no_columns);
		data_.descriptor_matrix_.resize(no_columns);

		for (Size i = 0; i < no_columns; i++)
		{
			data_.column_names_[i] = String(i);
			data_.descriptor_matrix_[i].push_back(v[i]);
		}
	}
	else
	{
		generateScoreContributions_(mol, &data_.descriptor_matrix_, 0);
	}
}


void Rescoring::recalibrate()
{
	if (!use_calibration_) return; // do nothing if no calibration is to be done

	data_.centerData(1);

	using namespace QSAR;

	// create PLS model
	delete model_;
	AutomaticModelCreator amc(&data_);
	amc.setMinQuality(0.20);
	Model* m = amc.generateModel();
	model_ = dynamic_cast<RegressionModel*>(m);
	return;

	PLSModel* pls = new PLSModel(data_);
	//KPLSModel* pls = new KPLSModel(data_, 2, 0.005, -1);
	model_ = pls;
	KernelModel* kernel_model = dynamic_cast<KernelModel*>(model_);
	pls->setNoComponents(1);

	model_->readTrainingData();
	model_->train();
	model_->validation->crossValidation(5);
	double q2 = model_->validation->getQ2();
	model_->validation->testInputData();
	double r2 = model_->validation->getR2();
	Log.level(40)<<"Quality after recalibration using all grid cells/features: "<<endl;
	Log.level(40)<<" R^2 = "<<r2<<endl;
	Log.level(40)<<" Q^2 = "<<q2<<endl;

	// select features
	FeatureSelection fs(*model_);
	double d = 0.95;
	fs.removeHighlyCorrelatedFeatures(d);
	model_->optimizeParameters(5);
	if (kernel_model) kernel_model->kernel->gridSearch(0.25, 20, 0, 5);
	fs.twinScan(5);
	model_->optimizeParameters(5);
	if (kernel_model) kernel_model->kernel->gridSearch(0.25, 20, 0, 5);

	// If very few features are selected, the resulting model is very likely highly unstable, so try to find some additional helpful features with a more expensive feature selection procedure
	Size no_features = model_->getDescriptorIDs()->size();
	if (no_features < 5)
	{
		Log.level(40) << "Only "<<no_features<<" features found by TwinScan ... "<<endl<<flush;
		model_->deleteDescriptorIDs();

		double threshold = -0.03;
		fs.setQualityIncreaseCutoff(threshold);
		fs.twinScan(5);
		model_->optimizeParameters(5);
		if (kernel_model) kernel_model->kernel->gridSearch(0.25, 20, 0, 5);
		Log.level(40) << "   #Features found by TwinScan with cutoff=-0.01 : "<<model_->getDescriptorIDs()->size()<<endl<<flush;
		threshold = 0.01;
		fs.setQualityIncreaseCutoff(threshold);
		fs.forwardSelection(5);
		Log.level(40) << "   #Features found by forward selection : "<<model_->getDescriptorIDs()->size()<<endl<<flush;
		model_->optimizeParameters(5);
		if (kernel_model) kernel_model->kernel->gridSearch(0.25, 20, 0, 5);

		if (model_->getDescriptorIDs()->size() < 2)
		{
			model_->deleteDescriptorIDs();
			multiset<unsigned int>* features = const_cast<multiset<unsigned int>*>(model_->getDescriptorIDs());

			unsigned int no_descriptors = data_.getNoDescriptors();
			for (Size i = 0; i < no_descriptors; i++)
			{
				features->insert(i);
			}

			model_->optimizeParameters(5);
			if (kernel_model) kernel_model->kernel->gridSearch(0.25, 20, 0, 5);
			model_->readTrainingData();
			model_->train();

			// remove feature with coefficient = 0
			const Eigen::MatrixXd* coeffs = model_->getTrainingResult();
			int row = 1;
			for (multiset < unsigned int > ::iterator it = features->begin(); it != features->end(); row++)
			{
				if (fabs((*coeffs)(row, 1)) < 1e-06)
				{
					multiset<unsigned int>::iterator tmp = it;
					tmp++;
					features->erase(it);
					it = tmp;
				}
				else it++;
			}
			Log.level(40) << "   #Features with coeff!=0 :"<<features->size()<<endl<<flush;
		}
	}

	// do cross-validation and display result
	model_->readTrainingData();
	model_->train();

	model_->validation->crossValidation(5);
	q2 = model_->validation->getQ2();
	model_->validation->testInputData();
	r2 = model_->validation->getR2();
	Log.level(40)<<endl<<"Quality after recalibration using only "<<model_->getDescriptorIDs()->size()<<" grid cells/features: "<<endl;
	Log.level(40)<<" R^2 = "<<r2<<endl;
	Log.level(40)<<" Q^2 = "<<q2<<endl;
}


double Rescoring::rescore(Molecule* mol)
{
	if (!scoring_function_)
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Rescoring3D::rescore() Error", "No scoring-function has been set!");
	}
	bool trained_model = 1;
	if (!model_) trained_model = 0;
// 			else
// 			{
// 				trained_model = (model_->getTrainingResult()->getRowCount()==model_->getDescriptorIDs()->size());
// 			}
	if (!trained_model)
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Rescoring3D::rescore() Error", "No trained model available! Use recalibrate first");
	}

	scoring_function_->setLigand(*mol);
	scoring_function_->update();
	double score = scoring_function_->updateScore();
	vector<double> score_contributions(0, 0);
	generateScoreContributions_(mol, 0, &score_contributions);

	if (data_.descriptor_matrix_.size() == 0)
	{
		vector<double> v(1, 0);
		data_.descriptor_matrix_.resize(score_contributions.size(), v);
	}

	double rescore = score;
	bool valid = 1;

	// If scoring-function detected a bad pose, we prevent those ligands from receiving good scores during rescoring.
// 			if(score>=0)
// 			{
// 				if(mol->hasProperty("score"))
// 				{
// 					rescore = ((String)mol->getProperty("score").toString()).toDouble();
// 				}
// 				valid=0;
// 			}

	/// if desired, check if compound is within applicability domain
	if (valid && check_applicability_)
	{
		const Eigen::MatrixXd & descriptor_transformations_ = model_->getDescriptorTransformations();
		const multiset<unsigned int>* descriptor_IDs = model_->getDescriptorIDs();

		Size j = 1;
		double max_deviation_factor = 0;
		for (Size i = 0; i < score_contributions.size(); i++)
		{
			// if feature is not part of current model, ignore it
			if (descriptor_IDs->find(i) == descriptor_IDs->end())
			{
				continue;
			}

			double deviation_factor = abs(score_contributions[i]-descriptor_transformations_(1, j))/descriptor_transformations_(2, j);
			if (deviation_factor > max_deviation_factor)
			{
				max_deviation_factor = deviation_factor;
			}

			// check whether (score_contributions[i]-mean of feature) > 5*stddev of feature
			if (abs(score_contributions[i]-descriptor_transformations_(1, j)) > 5*descriptor_transformations_(2, j))
			{
				if (score_contributions[i] < descriptor_transformations_(1, j))
				{
					score_contributions[i] = descriptor_transformations_(1, j)-5*descriptor_transformations_(2, j);
				}
				else
				{
					score_contributions[i] = descriptor_transformations_(1, j)+5*descriptor_transformations_(2, j);
				}
				//valid = 0;
				//break;
			}

			j++;
		}

		if (!valid)
		{
			Log.level(20)<<"[Warning:] compound outside of applicability domain, no rescoring done!"<<endl;
			rescore = 1e10;
		}
	}

	if (valid) // use the model to predict binding affinity only if it is applicable to the current molecule
	{
		rescore = model_->predict(score_contributions, 1)[0];
	}

	/// If prediction seems to be very unreliable, use docking-score instead.
	if (valid)
	{
		const Eigen::MatrixXd & y_tr = model_->getYTransformations();
		if (fabs(rescore-y_tr(1, 1)) > 10*y_tr(2, 1))
		{
			rescore = score;
			if (mol->hasProperty("score"))
			{
				rescore = ((String)mol->getProperty("score").toString()).toDouble();
			}
			valid = 0;
		}
	}

	mol->setProperty("re-score", rescore);

	// save score and affinity in order to be able to calculate quality later
	if (valid && mol->hasProperty(free_energy_label_))
	{
		float value = ((String)(mol->getProperty(free_energy_label_).toString())).toFloat();
		if (convert_)
		{
			value *= 1e-09;
			value = 1.987*298.15*log(value)*4.184/1000;
			mol->setProperty("binding free energy", value);
		}

		if (Maths::isInfinite(value) || value < -90)
		{
			Log.level(20)<<"invalid free-energy value found, ignoring this molecule for calculation of prediction-quality!!"<<endl;
		}
		else
		{
			predicted_affinities_.push_back(rescore);
			experimental_affinities_.push_back(value);
		}
	}

	Log<<"re-score = "<<rescore<<endl;
	return rescore;
}

void Rescoring::saveModel(String filename)
{
	bool trained_model = 1;
	if (!model_) trained_model = 0;
// 			else
// 			{
// 				Size no_features = model_->getDescriptorIDs()->size();
// 				if(no_features==0) no_features=data_.getNoDescriptors();
// 				trained_model = (model_->getTrainingResult()->getRowCount()==no_features);
// 			}
	if (!trained_model)
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Rescoring::saveModel() Error", "No trained model available! Use recalibrate first");
	}
	model_->saveToFile(filename);
}

void Rescoring::loadModel(String filename)
{
	if (model_) delete model_;
	QSAR::Model* new_model = QSAR::createNewModelFromFile(filename, data_);
	QSAR::RegressionModel* new_regr_model = dynamic_cast<QSAR::RegressionModel*>(new_model);
	if (new_regr_model) model_ = new_regr_model;
	else
	{
		delete new_model;
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Rescoring::loadModel() Error", "Model to be loaded from file is no regression-model and thus cannot be used!");
	}
}

void Rescoring::calculateQuality(double& correlation, double& q2, double& std_error)
{
	double mean_p = QSAR::Statistics::getMean(predicted_affinities_);
	double mean_e = QSAR::Statistics::getMean(experimental_affinities_);
	double covar = QSAR::Statistics::getCovariance(predicted_affinities_, experimental_affinities_, mean_p, mean_e);
	double stddev_e = QSAR::Statistics::getStddev(experimental_affinities_, mean_e);
	double stddev_p = QSAR::Statistics::getStddev(predicted_affinities_, mean_p);

	correlation = covar / (stddev_e*stddev_p);
	double ssE = 0;
	double ssY = 0;

	for (Size i = 0; i < experimental_affinities_.size(); i++)
	{
		ssE += pow(experimental_affinities_[i]-predicted_affinities_[i], 2);
		ssY += pow(experimental_affinities_[i]-mean_e, 2);
		Log<<predicted_affinities_[i]<<"  "<<experimental_affinities_[i]<<endl;
	}

	q2 = 1-(ssE/ssY);
	std_error = sqrt(ssE/experimental_affinities_.size());
}

const String& Rescoring::getName()
{
	return name_;
}
