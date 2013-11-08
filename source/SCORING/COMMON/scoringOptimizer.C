// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMMON/scoringOptimizer.h>
#include <BALL/DOCKING/COMMON/structurePreparer.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/SCORING/FUNCTIONS/PLPScoring.h>
#include <BALL/SCORING/FUNCTIONS/PBScoring.h>
#include <BALL/KERNEL/PTE.h>

using namespace BALL;
using namespace std;


ScoringOptimizer::ScoringOptimizer(Options& options, bool train)
{
	scoring_function_name_ = options.get("scoring_type");
	options_ = options;
	train_ = train;
}


void ScoringOptimizer::addComplex(String name, String receptor_file, String ligand_file, double binding_free_energy)
{
	Complex c;
	c.name = name;
	c.receptor_file = receptor_file;
	c.ligand_file = ligand_file;
	c.binding_free_energy = binding_free_energy;
	complexes_.push_back(c);
}


void ScoringOptimizer::evaluate(double* correlation, double* R2, double* RMSE, Size* no_valid_complexes)
{
	StructurePreparer sp;
	if (scoring_function_name_.hasSubstring("PLP"))
	{
		sp.setScoringType("PLP");
	}

	result_.predictions.clear();
	result_.target_names.clear();
	result_.expected_affinities.clear();
	result_.score_contributions.clear();
	result_.score_contribution_names.clear();

	Size skipped_complexes = 0;
	Size atom_overlaps = 0;
	Size ignored_outliers = 0;
	Size i = 0;
	double sum_squared_errors = 0;

	for (list < Complex > ::iterator it = complexes_.begin(); it != complexes_.end(); it++, i++)
	{
		cout<<"==== evaluating complex "<<i<<", "<<it->name<<" ========="<<endl;

		ScoringFunction* scoring_function = NULL;

		try
		{
			System ligand;
			Molecule* mol = 0;
			System receptor;
			PDBFile p(it->receptor_file);
			p >> receptor;
			String par_file = options_.get("filename");
			sp.prepare(&receptor, par_file);
			SDFile* sdf = 0;

			if (it->ligand_file.hasSubstring(".mol2"))
			{
				MOL2File m(it->ligand_file);
				m >> ligand;
			}
			else if (it->ligand_file.hasSubstring(".mol"))
			{
				MOLFile m(it->ligand_file);
				m >> ligand;
			}
			else if (it->ligand_file.hasSubstring(".sdf"))
			{
				sdf = new SDFile(it->ligand_file);
				mol = sdf->read();
				ligand.insert(*mol);
			}

			scoring_function = createScoringFunction(receptor, ligand);
			bool finished = 0;
			double threshold = 1000;
			if (!train_) threshold = 0;

			for (Size m = 0; !finished; m++)
			{
				sp.prepare(&ligand, par_file);

				if (train_) scoring_function->unsetTrainingParameters();

				scoring_function->setLigand(ligand); ///

				double score;

				scoring_function->update();
				score = scoring_function->updateScore();
				scoring_function->printResult(true);

// 				int heavy_atoms=0;
// 				for(AtomIterator ait=ligand.beginAtom();+ait;++ait)
// 				{
// 					if(ait->getElement().getSymbol()!="H")
// 					{
// 						heavy_atoms++;
// 					}
// 				}
// 				score = (1./(pow(heavy_atoms,1./3)))*score;


				if (score >= 0.99*1e10)
				{
					cout<<"Atom-overlaps found!! Ignoring this complex!"<<endl;
					atom_overlaps++;
					if (!sdf) break;
					else
					{
						//delete mol;
						mol = sdf->read();
						if (mol == 0) break;
						ligand.clear();
						ligand.insert(*mol);
						continue;
					}
				}

				if (score >= threshold)
				{
					cout<<"Score>="<<threshold<<", considering this complex as outlier and ignoring it!"<<endl;
					ignored_outliers++;
					if (!sdf) break;
					else
					{
						//delete mol;
						mol = sdf->read();
						if (mol == 0) break;
						ligand.clear();
						ligand.insert(*mol);
						continue;
					}
				}

				double binding_free_energy = it->binding_free_energy;
				if (sdf) binding_free_energy = String(mol->getProperty("binding free energy").getString()).toDouble();
				String name = it->name;
				if (sdf) name += ", "+String(m);

				result_.predictions.push_back(score);
				result_.expected_affinities.push_back(binding_free_energy);
				result_.target_names.push_back(name);
				sum_squared_errors += pow(score-binding_free_energy, 2);

				vector<double> v;
				result_.score_contributions.push_back(v);
				scoring_function->getScoreContributions(result_.score_contributions[result_.score_contributions.size()-1], result_.score_contribution_names);

				if (!sdf) finished = true;
				else
				{
					//delete mol;
					mol = sdf->read();
					if (mol == 0) break;
					ligand.clear();
					ligand.insert(*mol);
				}
			}
			delete sdf;
		}
		catch(BALL::Exception::GeneralException e)
		{
			cout<<e.getName()<<" : "<<e.getMessage()<<endl;
			cout<<"Error for this complex, skipping it!"<<endl;
			delete scoring_function;
			skipped_complexes++;
			continue;
		}

		delete scoring_function;
	}

	double mean_expected = getMean(result_.expected_affinities);
	double mean_predicted = getMean(result_.predictions);

	double covar = getCovariance(result_.expected_affinities, result_.predictions, mean_expected, mean_predicted);
	double stddev_expected = getStddev(result_.expected_affinities, mean_expected);
	double stddev_predicted = getStddev(result_.predictions, mean_predicted);

	double cor = covar/(stddev_expected*stddev_predicted);

	cout<<endl<<"number of skipped complexes="<<skipped_complexes<<endl<<endl;
	cout<<"number of ignored outliers="<<ignored_outliers<<endl;
	cout<<endl<<"number of complexes with atom overlaps="<<atom_overlaps<<endl<<endl;

	cout << "Correlation between score and experimentally determined affinity = "<<cor<<endl<<endl;

	double r2 = 1 - (sum_squared_errors/getSumOfSquares(result_.expected_affinities, mean_expected));
	cout<<"sum_squared_errors="<<sum_squared_errors<<endl;
	cout<<"sum_squared_response_meandev="<<getSumOfSquares(result_.expected_affinities, mean_expected)<<endl;

	*no_valid_complexes = result_.predictions.size();
	cout<<"N = "<<result_.predictions.size()<<endl;
	cout<<"R2 = "<<r2<<endl;
	double rmse = sqrt(sum_squared_errors/result_.predictions.size());
	cout<<"RMSE = "<<rmse<<endl;

	if (correlation != 0) *correlation = cor;
	if (R2 != 0) *R2 = r2;
	if (RMSE != 0) *RMSE = rmse;
}


const ScoringOptimizer::Result* ScoringOptimizer::getResult()
{
	return &result_;
}


void ScoringOptimizer::printMatrix(ostream& out)
{
	out<<"target name\t";
	for (Size i = 0; i < result_.score_contribution_names.size(); i++)
	{
		out<<result_.score_contribution_names[i]<<"\t";
	}
	out<<"binding free energy";
	out<<endl;

	list<String>::iterator it1 = result_.target_names.begin();
	list<double>::iterator it2 = result_.expected_affinities.begin();
	for (Size i = 0; i < result_.score_contributions.size(); i++, it1++, it2++) // for each evaluated complex
	{
		out<<*it1<<"\t"; //name
		for (Size j = 0; j < result_.score_contributions[i].size(); j++)
		{
			out<<result_.score_contributions[i][j]<<"\t";
		}

		out<<*it2<<endl; // exp. affinity
	}
	out<<endl;
}



ScoringFunction* ScoringOptimizer::createScoringFunction(System& receptor, System& ligand)
{
	ScoringFunction* scoring_function;

	if (scoring_function_name_ == "MM")
	{
		scoring_function = new MMScoring(receptor, ligand, options_);
	}
	else if (scoring_function_name_ == "PLP")
	{
		scoring_function = new PLPScoring(receptor, ligand, options_);
	}
	else if (scoring_function_name_ == "PB")
	{
		scoring_function = new PBScoring(receptor, ligand, options_);
	}
	else
	{
		String mess="ScoringFunction type \'"+scoring_function_name_+"\' unsupported!";
		throw Exception::GeneralException(__FILE__, __LINE__, "IMeedyDock error", mess);
	}

	return scoring_function;
}


double BALL::getMean(const list<double>& v)
{
	double sum = 0;
	for (list < double > ::const_iterator it = v.begin(); it != v.end(); it++)
	{
		sum += *it;
	}
	return sum/v.size();
}


double BALL::getCovariance(const list<double>& v1, const list<double>& v2, double mean1, double mean2)
{
	if (v1.size() != v2.size())
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "getCovariance() error!", "Error within covariance calculation: lists have unequal length!");
	}

	if (mean1 == -1) mean1 = getMean(v1);
	if (mean2 == -1) mean2 = getMean(v2);
	double sum_of_squares = 0;

	list<double>::const_iterator it1 = v1.begin();
	list<double>::const_iterator it2 = v2.begin();
	for (; it1 != v1.end() && it2 != v2.end(); it1++, it2++)
	{
		sum_of_squares += (*it1-mean1)*(*it2-mean2);
	}
	return sum_of_squares/(v1.size()-1);
}


double BALL::getSumOfSquares(const list<double>& m, double mean)
{
	if (mean == -1) mean = getMean(m);
	double sum_of_squares = 0;

	for (list < double > ::const_iterator it = m.begin(); it != m.end(); it++)
	{
		sum_of_squares += pow(*it-mean, 2);
	}
	return sum_of_squares;
}


double BALL::getStddev(const list<double>& m, double mean)
{
	if (mean == -1) mean = getMean(m);
	double sum_of_squares = 0;

	for (list < double > ::const_iterator it = m.begin(); it != m.end(); it++)
	{
		sum_of_squares += pow(*it-mean, 2);
	}

	double var = sum_of_squares/(m.size()-1);
	return sqrt(var);
}
