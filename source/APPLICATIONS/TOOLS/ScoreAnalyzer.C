// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/QSAR/statistics.h>
#include "version.h"
#include <set>
#include <iostream>

using namespace BALL;
using namespace std;


bool getProperty(Molecule* molecule, String property_name, string& property)
{
	if (!molecule) return false;
	if (!molecule->hasProperty(property_name)) return false;

	property = molecule->getProperty(property_name).toString();
	return true;
}


void calculateQuality(multiset<pair<double, double> >& list, const double& score_threshold, const double& binding_free_energy_threshold, Size& TP, Size& FP, Size& TN, Size& FN, double& sen, double& spec, double& acc, double& frac, double& TDR, double& TRR)
{
	TP = 0;
	FP = 0;
	TN = 0;
	FN = 0;
	frac = 0;
	TDR = 0;  // True Discovery Rate
	TRR = 0; // True Rejection Rate

	for (multiset < pair < double, double > > ::iterator it = list.begin(); it != list.end(); it++)
	{
		const double& score = it->first;
		const double& binding_free_energy = it->second;

		if (score < score_threshold) frac++;

		if (binding_free_energy < binding_free_energy_threshold) // 'active' compounds
		{
			if (score < score_threshold) TP++;
			else FN++;
		}
		else // 'inactive' compounds
		{
			if (score < score_threshold) FP++;
			else TN++;
		}
	}

	sen = ((double)TP)/(TP+FN);
	spec = ((double)TN)/(TN+FP);
	acc = (((double)TP)+TN)/(TP+TN+FP+FN);
	frac = frac/list.size();
	TDR = ((double)TP)/(TP+FP);
	TRR = ((double)TN)/(TN+FN);
}


// area under ROC curve = average relative rank
void calculateAUC(multiset<pair<double, double> >& list, const double& binding_free_energy_threshold, double& AUC, double& AUC_harmonic)
{
	double average_relative_rank = 0;
	double average_relative_harmonic_rank = 0;
	Size no_actives = 0;
	double harmonic_sum = 0;
	Size total_no_decoys = 0;

	for (multiset < pair < double, double > > ::iterator it = list.begin(); it != list.end(); it++)
	{
		const double& binding_free_energy = it->second;
		if (binding_free_energy >= binding_free_energy_threshold) // 'inactive' compounds
		{
			total_no_decoys++;
		}
	}

	Size no_decoys = 0;
	for (multiset < pair < double, double > > ::iterator it = list.begin(); it != list.end(); it++)
	{
		//const double& score = it->first;
		const double& binding_free_energy = it->second;
		if (binding_free_energy < binding_free_energy_threshold) // 'active' compounds
		{
			// ((double)no_decoys)/n = fraction of decoys scored better than current binder
			double relative_rank = 1-(((double)no_decoys)/total_no_decoys);
			average_relative_rank += relative_rank;
			no_actives++;

			double weight = sqrt(1./no_actives);
			harmonic_sum += weight;
			average_relative_harmonic_rank += weight * relative_rank;
		}
		else
		{
			no_decoys++;
		}
	}

	AUC_harmonic = average_relative_harmonic_rank/harmonic_sum;
	AUC = average_relative_rank /= no_actives;
}

BALL::String valueToString(double value)
{
	BALL::String t(value);
	int index = (int)t.find_last_of(".");
	int size = (int)t.size();
	if (index >= 0)
	{
		if (index+1 <= size) index++;
		if (index+1 <= size) index++;
		if (index+1 <= size) index++;
		t = t.substr(0, index);
	}
	return t;
}

int main(int argc, char* argv[])
{
	CommandlineParser parpars("ScoreAnalyzer", "generate ROC or enrichment plots", VERSION, String(__DATE__), "Analysis");
	parpars.registerMandatoryStringParameter("mode", "'roc', 'top50', 'scatter' or 'enrichment'");
	parpars.registerOptionalStringParameter("title", "plot title");
	parpars.registerOptionalOutputFile("o", "output eps-/png-file");
	parpars.registerMandatoryInputFile("i", "input file");
	parpars.registerMandatoryStringParameter("s", "score label");
	parpars.registerMandatoryStringParameter("e", "binding-free-energy/class label name");
	parpars.registerFlag("b", "binary experimental activity data");
	parpars.registerOptionalStringParameter("t", "only in case of non-binary act. data: binding-free-energy threshold; compound with values *below* this threshold will be defined as binder");
	String man = "This tool can be used generate plots that allow to evaluate the quality of docking or (re-)scoring.\n\nThe type of plot to be generated must be chosen by either '-roc', '-top50', '-scatter' or '-enrichment'. The name of the property-tag that contains the scores to be evaluated (e.g. obtained by docking) is to be specified by '-s'; the name of the property-tag containing experimental data (e.g. binding-free-energy measurements or binder/non-binder info) by use '-e'. If the experimental reference data is not binary, then a threshold below which compound will be considered binders must be given with '-t'.\nThe output graphic will created by use of gnuplot, so make sure to have it installed and in your PATH environment variable.\n\nThe output of this tool is a plot in form of an eps or png-file (as chosen).";
	parpars.setToolManual(man);
	list<String> slist;
	slist.push_back("roc");
	slist.push_back("top50");
	slist.push_back("scatter");
	slist.push_back("enrichment");
	parpars.setParameterRestrictions("mode",slist);
	parpars.setSupportedFormats("i","mol2,sdf,drf");
	parpars.setSupportedFormats("o","png,eps");
	parpars.parse(argc, argv);

	const String& nf = CommandlineParser::NOT_FOUND;

	string label = parpars.get("title");
	if (label == nf) label="";

	int plot_type = 0;
	int no = 0;
	String mode = parpars.get("mode");
	if (mode == "roc")
	{
		plot_type = 1;
		no++;
	}
	if (mode == "top50")
	{
		plot_type = 2;
		no++;
	}
	if (mode == "scatter")
	{
		plot_type = 3;
		no++;
	}
	if (mode == "enrichment")
	{
		plot_type = 4;
		no++;
	}

	if (no != 1)
	{
		cerr<<"[Error:] Desired plotting-mode is unknown. Please set parameter 'mode' to 'roc', 'top50', 'scatter' or 'enrichment'."<<endl;
		return 1;
	}

	bool use_dataset_fraction = 0;
	if (plot_type == 4)
	{
		plot_type = 0;
		use_dataset_fraction = 1;
	}
	String x_label = "";
	String y_label = "";

	vector<double> AUCs;

	// for scatter-plot only
	vector<double> correlations;
	vector<double> Q2s;
	vector<double> std_errors;
	vector<double> rank_correlations;
	vector<int> number_compounds;
	vector<double> enrichments_005;

	String filename = "";
	int file_id = 1;
	const list<String>& filenames = parpars.getList("i");
	const list<String>& score_labels = parpars.getList("s");
	const list<String>& exp_labels = parpars.getList("e");
	bool binary_labels = parpars.has("b");
	list<String>::const_iterator s_it = score_labels.begin();
	list<String>::const_iterator e_it = exp_labels.begin();

	for (list < String > ::const_iterator it = filenames.begin(); it != filenames.end(); file_id++, it++) // if desired, read more than one input file
	{
		filename = *it;

		String score_label="score";
		if (s_it != score_labels.end())
		{
			score_label = *s_it;
			s_it++;
		}
		if (x_label != score_label)
		{
			if (x_label != "") x_label += ", ";
			x_label += score_label;
		}

		String exp_name = "Class";
		if (e_it != exp_labels.end())
		{
			exp_name = *e_it;
			e_it++;
		}
		if (y_label != exp_name)
		{
			if (y_label != "") y_label += ", ";
			y_label += exp_name;
		}

		multiset<pair<double, double> > list; // with pairs of score--binding-free-energy
		vector<double> observed; // for scatter-plot only
		vector<double> expected; // for scatter-plot only
		if (plot_type == 3)
		{
			observed.reserve(1000);
			expected.reserve(1000);
		}

		GenericMolFile* input = MolFileFactory::open(filename);
		if (!input)
		{
			Log.error()<<"Error: extension of input-file '"<<filename<<"' not supported!"<<endl;
			return 1;
		}

		bool found = 1;
		int i = 0;

		map<double, pair<double, double> > enrichments;

		for (Molecule* mol = input->read(); mol && found; mol = input->read(), i++)
		{
			//cout<<"\r"<<i<<" molecules"<<flush;
			String property;
			found = getProperty(mol, exp_name, property);
			if (!found) break;

			if (property.hasPrefix('>'))
			{
				property = property.after('>');
			}
			double activity = atof(property.c_str());

			found = getProperty(mol, score_label, property);
			if (!found) break;

			double score = atof(property.c_str());

			if (binary_labels) activity *= -1; // make sure that calculateAUC works correctly: binders should have smaller "binding_free_energy" value than non-binders

			list.insert(make_pair(score, activity));

			if (plot_type == 3)
			{
				observed.push_back(score);
				expected.push_back(activity);
				if (i%1000 == 1) // prevent frequent resizing
				{
					Size new_size = ((i/1000)+1)*1000;
					observed.reserve(new_size);
					expected.reserve(new_size);
				}
			}

			delete mol;
		}
		if (i == 0)
		{
			Log.error()<<"Error: found no properties !!"<<endl;
			exit(1);
		}
		Log.level(20)<<"\rread "<<i<<" molecules"<<endl;

		double binding_free_energy_threshold = -0.2;

		ofstream out;
		String n = "obs_exp_"+String(file_id)+".txt";
		out.open(n.c_str());

		for (multiset < pair < double, double > > ::iterator it = list.begin(); it != list.end(); it++)
		{
			out<<it->first<<"\t"<<it->second<<endl;
		}
		out.close();

		if (!binary_labels)
		{
			String s = parpars.get(String("t"));
			if (s == nf)
			{
				if (plot_type != 3) // scatter plot needs no threshold
				{
					cerr<<"[Error:] You need to specify a binding_free_energy threshold for data file that contains non-binary activity measurements."<<endl;
					exit(1);
				}
			}
			else binding_free_energy_threshold = s.toDouble();
		}

		//if (!binary_labels) cout<<endl<<"---- THRESHOLD = "<<binding_free_energy_threshold<<" ------"<<endl;
		//if (plot_type == 0) cout<<"score_threshold  sensitivity  specificity  accuracy   fraction < score-threshold  TDR  TRR"<<endl;
		double score_threshold = -80;
		double step = 1;
		double score_max_threshold = 10;
		double frac = 0;
		double TDR = 0; // True Disvoery Rate
		double TRR = 0; // True Rejection Rate
		double sensitivity;
		double specificity;
		Size TP, FP, TN, FN;
		double accuracy;

		/// Define lower and upper score boundaries flexibly, based on current data set.
		score_threshold = list.begin()->first;
		multiset<pair<double, double> >::iterator back = list.end();
		back--;
		score_max_threshold = back->first;
		double tmp_max = score_max_threshold;
		if (tmp_max > 1000) tmp_max = 1000;
		step = (tmp_max-score_threshold)/(list.size());
		if (step < 0.01) step = 0.01;
		if (score_max_threshold > 1e11 && plot_type == 1) score_max_threshold = 1e15;
		calculateQuality(list, 1e100, binding_free_energy_threshold, TP, FP, TN, FN, sensitivity, specificity, accuracy, frac, TDR, TRR);

		Log.level(15)<<endl<<"min. score = "<<score_threshold<<endl;
		Log.level(15)<<"max. score = "<<score_max_threshold<<endl;
		Log.level(15)<<"plot resolution = "<<step<<" [score units]"<<endl<<endl;

		int no_binders = TP+FN;
		int total = TP+FP+TN+FN;
		double binder_fraction = (((double)no_binders)/total)*100;
		Log.level(15)<<"number of binders = "<<no_binders<<endl;
		Log.level(15)<<"number of non-binders = "<<(FP+TN)<<endl;
		Log.level(15)<<"total number of molecules = "<<total<<endl;
		Log.level(15)<<"fraction of binders = "<<binder_fraction<<"%"<<endl<<endl;

		double total_inhib_concentration = TDR;

		n = "plot_data_"+String(file_id)+".txt";
		out.open(n.c_str());
		bool wrote_all = 0;

		if (plot_type < 2)
		{
			int no_steps;
			if (score_max_threshold > 1e03)
			{
				no_steps = (int)((1e03-score_threshold)/step);
				no_steps += (int)((score_max_threshold-1e03)/1e07);
			}
			else no_steps = (int)((score_max_threshold-score_threshold)/step);

			bool first = 1;
			for (int i = 0; score_threshold < score_max_threshold; score_threshold += step, i++)
			{
// 				if(plot_type==1)
// 				{
// 					int p = (int)((((double)i)/no_steps)*100);
// 					//cout<<"\r"<<p<<"%"<<flush;
// 				}
				calculateQuality(list, score_threshold, binding_free_energy_threshold, TP, FP, TN, FN, sensitivity, specificity, accuracy, frac, TDR, TRR);
				String f = String(frac*100);
				f += "%";

				if (sensitivity > 0 && !wrote_all)
				{
					double enrichment_factor = TDR/total_inhib_concentration;
					if (plot_type == 0)
					{
						if (!use_dataset_fraction)
						{
							out<<sensitivity<<"\t"<<enrichment_factor<<endl;
						}
						else
						{
							double dataset_fraction = (TP+FP)/((double)(TN+FN+TP+FP));
							out<<dataset_fraction<<"\t"<<enrichment_factor<<endl;

							double diff = fabs(0.005-dataset_fraction);
							enrichments.insert(make_pair(diff, make_pair(dataset_fraction, enrichment_factor)));
							if (enrichments.size() > 2) enrichments.erase(--enrichments.end());
						}
						if (sensitivity >= 0.99 || 1-specificity >= 0.99 /* || enrichment_factor <= 1.1*/)
						{
							wrote_all = true;
						}
					}
					else
					{
						if (first)
						{
							out<<0<<"\t"<<0<<endl;
							out<<(1-specificity)<<"\t"<<0<<endl;
							first = false;
						}
						out<<(1-specificity)<<"\t"<<sensitivity<<endl;
					}
				}

				if (wrote_all) break;
				if (score_threshold > 1000) step = 1e07;
				if (score_threshold > 1e11) step = 1e11;
				if (score_threshold > 1e13) step = 1e13;
			}
			Log.level(15)<<endl;
			calculateQuality(list, 1e100, binding_free_energy_threshold, TP, FP, TN, FN, sensitivity, specificity, accuracy, frac, TDR, TRR);
			if (plot_type == 0) Log.level(15)<<endl<<"[for comparison:] inhibitor-concentration of entire data multiset: "<<total_inhib_concentration<<endl;

			if (plot_type == 1)
			{
				double AUC;
				double AUC_harmonic;
				calculateAUC(list, binding_free_energy_threshold, AUC, AUC_harmonic);
				AUCs.push_back(AUC);
				Log.level(20)<<"AUC = "<<AUC<<endl;
			}
		}
		else if (plot_type == 2) // plot_type == 2
		{
			int no_actives = 0;
			int total_no = 0;
			for (multiset < pair < double, double > > ::iterator it = list.begin(); it != list.end(); it++)
			{
				total_no++;
				if (it->second < binding_free_energy_threshold) no_actives++;
				double expected_actives = total_inhib_concentration*total_no;
				out<<no_actives<<"\t"<<expected_actives<<endl;
			}
		}
		else  // plot_type == 3
		{
			double mean_p = QSAR::Statistics::getMean(observed);
			double mean_e = QSAR::Statistics::getMean(expected);
			double covar = QSAR::Statistics::getCovariance(observed, expected, mean_p, mean_e);
			double stddev_e = QSAR::Statistics::getStddev(expected, mean_e);
			double stddev_p = QSAR::Statistics::getStddev(observed, mean_p);

			double correlation = covar / (stddev_e*stddev_p);
			double ssE = 0;
			double ssY = 0;

			for (Size i = 0; i < expected.size(); i++)
			{
				ssE += pow(expected[i]-observed[i], 2);
				ssY += pow(expected[i]-mean_e, 2);
			}

			double q2 = 1-(ssE/ssY);
			double std_error = sqrt(ssE/expected.size());
			double rank_cor = QSAR::Statistics::calculateRankCorrelation(observed, expected);

			Q2s.push_back(q2);
			std_errors.push_back(std_error);
			rank_correlations.push_back(rank_cor);
			correlations.push_back(correlation);
			number_compounds.push_back(expected.size());
		}
		out.close();

		if (plot_type == 0 && use_dataset_fraction)
		{
			map<double, pair<double, double> >::iterator it = enrichments.begin();
			double& fraction1 = it->second.first;
			double& ef1 = it->second.second;
			it++;
			double& fraction2 = it->second.first;
			double& ef2 = it->second.second;
			double enrichment_factor = ef1+((fraction1-0.005)/(fraction1-fraction2))*(ef2-ef1);
			enrichments_005.push_back(enrichment_factor);
		}
	}// end of processing all input files

	ofstream gp("plot.txt");
	string output_filename = parpars.get("o");
	if (output_filename != nf)
	{
		int size = output_filename.size();
		if (size > 4 && string(output_filename.substr(size-4)) == ".eps")
		{
			gp<<"set term postscript eps color enhanced"<<endl;
			gp<<"set output \""<<output_filename<<"\""<<endl;
			Log.level(2)<<"using eps"<<endl;
		}
		else
		{
			gp<<"set term png enhanced size 1024, 650"<<endl;
			gp<<"set output \""<<output_filename<<"\""<<endl;
		}
	}

	if (label != "")
	{
		gp<<"set title \""<<label<<"\""<<endl;
	}
	if (plot_type == 0)
	{
		if (!use_dataset_fraction) gp<<"set xlabel \"sensitivity\""<<endl;
		else gp<<"set xlabel \"fraction of data set\""<<endl;
		gp<<"set ylabel \"enrichment factor\""<<endl;
		gp<<"unset key"<<endl;
		if (use_dataset_fraction)
		{
			gp<<"set xrange [0:0.1]"<<endl;
			for (int i = 1; i < file_id; i++)
			{
				gp<<"set label \"EF_{0.005}="<<enrichments_005[i-1]<<"\" at graph 0.74, graph "<<(0.09-(i-1)*0.03)<<endl;
			}
		}
		gp<<"plot ";
		for (int i = 1; i < file_id; i++)
		{
			gp<<"\"plot_data_"<<(i)<<".txt\" with lines linetype "<<(i)<<" linewidth 3";
			if (i < file_id-1) gp<<", ";
		}
		gp<<endl;
	}
	else if (plot_type == 1)
	{
		gp<<"set xlabel \"1-specificity = fraction of selected decoys\""<<endl;
		gp<<"set ylabel \"sensitivity = fraction of selected inhibitors\""<<endl;

		for (int i = 1; i < file_id; i++)
		{
			gp<<"set label \"AUC="<<AUCs[i-1]<<"\" at graph 0.74, graph "<<(0.09-(i-1)*0.03)<<endl;
		}

		gp<<"unset key"<<endl;
		gp<<"plot ";
		for (int i = 1; i < file_id; i++)
		{
			gp<<"\"plot_data_"<<(i)<<".txt\" with lines linetype "<<(i+2)<<" linewidth 3, x w l linetype 0";
			if (i < file_id-1) gp<<", ";
		}
		gp<<endl;
	}
	else if (plot_type == 2)
	{
		gp<<"set xlabel \"TP+FP = no of selected compounds\""<<endl;
		gp<<"set ylabel \"TP = no of selected inhibitors\""<<endl;
		gp<<"set xrange [0:50]"<<endl;
		gp<<"unset key"<<endl;
		gp<<"plot ";
		for (int i = 1; i < file_id; i++)
		{
			gp<<"\"plot_data_"<<(i)<<".txt\" using 1 with lines linetype "<<(i)<<" linewidth 3, \"plot_data_"<<(i)<<".txt\" using 2 w l linetype 0";
			if (i < file_id-1) gp<<", ";
		}
		gp<<endl;
	}
	else if (plot_type == 3)
	{
		gp<<"set xlabel \""<<x_label<<"\""<<endl;
		gp<<"set ylabel \""<<y_label<<"\""<<endl;
		gp<<"unset key"<<endl;

		for (Size i = 0; i < correlations.size(); i++)
		{
			gp<<"set label \"n="<<number_compounds[i]<<"\" at graph 0.82, graph "<<(0.2-i*0.04)<<endl;
			String value = valueToString(correlations[i]);
			gp<<"set label \"cor="<<value<<"\" at graph 0.82, graph "<<(0.2-(i+1)*0.04)<<endl;
			value = valueToString(std_errors[i]);
			gp<<"set label \"std-error="<<value<<"\" at graph 0.82, graph "<<(0.2-(i+2)*0.04)<<endl;
			value = valueToString(Q2s[i]);
			gp<<"set label \"Q^2="<<value<<"\" at graph 0.82, graph "<<(0.2-(i+3)*0.04)<<endl;
			value = valueToString(rank_correlations[i]);
			gp<<"set label \"rank-cor="<<value<<"\" at graph 0.82, graph "<<(0.2-(i+4)*0.04)<<endl;
		}
		gp<<endl;

		gp<<"plot \"obs_exp_1.txt\" linestyle 25, x linestyle 0, x+"<<std_errors[0]<<" linestyle 0, x-"<<std_errors[0]<<" linestyle 0"<<endl;
		if (file_id > 2) gp<<", ";
		for (int i = 2; i < file_id; i++)
		{
			gp<<"\"obs_exp_"<<String(i)<<".txt";
			if (i < file_id-1) gp<<", ";
		}
		gp<<endl;
	}

	gp.close();
	system("gnuplot -persist plot.txt");
}
