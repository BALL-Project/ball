#include <BALL/QSAR/configIO.h>
#include <BALL/QSAR/exception.h>
#include <BALL/QSAR/registry.h>
 
using namespace std;

namespace BALL
{
	namespace QSAR
	{

		void ConfigIO::putbackLine(std::istream* input, String& line)
		{
			input->unget();
			for (unsigned int i = 0; i < line.size(); i++)
			{
				input->unget();
			}
		}
			
		/// ---------------- Reading of Input-sections (sd-/csv-files) -----------

		InputConfiguration::InputConfiguration()
		{
			sd_file="";
			read_sd_descriptors = 0;
			center_data = 0;
			center_y = 0;
			output="";
			validation_fraction = 0;
			separate_activity_file = 0;
			within_section = 0;
			csv_file.resize(0);
			csv_no_response.resize(0);
			csv_desc_labels.resize(0);
			csv_compound_labels.resize(0);
			csv_separator.resize(0);
			nonnumeric_class_names = 0;
			done = 0;
		}

		InputConfiguration ConfigIO::readInputConfiguration(istream* input)
		{	
			InputConfiguration conf;	
			
			for (int j = 0; input != NULL && !input->eof(); j++) // read ONE InputReader section
			{
				String line;
				getline(*input, line);
				
				if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
				{
					j--;
					continue;
				}
					
				line.trim();
					
				if (j == 0)
				{
					if (!line.hasPrefix("[InputReader]"))
					{
						throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The current section is no Input-section!");
					}
					else continue;
				}
				else if (line.hasPrefix("["))
				{
					putbackLine(input, line);
					break;
				}	
				
				if (line.hasPrefix("done"))
				{
					conf.done = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("sd_file"))
				{
					conf.sd_file = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("output"))
				{
					conf.output = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("separate_activity_file"))
				{
					conf.separate_activity_file = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("activity_IDs"))
				{
					String act = ((String)line.after("=")).trimLeft();
					unsigned int no = act.countFields(" ");
					for (unsigned int i = 0; i < no; i++)
					{
						conf.activities.insert(act.getField(i).toInt());
					}
				}
				else if (line.hasPrefix("center_data"))
				{
					conf.center_data = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("center_response"))
				{
					conf.center_y = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("external_val_fraction"))
				{
					conf.validation_fraction = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("read_sd_descriptors"))
				{
					conf.read_sd_descriptors = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("csv_file"))
				{
					conf.csv_file.push_back(((String)line.after("=")).trimLeft());
				}
				else if (line.hasPrefix("csv_separator"))
				{
					conf.csv_separator.push_back(((String)((String)line.after("=")).after("\"")).before("\"")); 
				}
				else if (line.hasPrefix("csv_desc_labels"))
				{
					conf.csv_desc_labels.push_back(((String)line.after("=")).trimLeft().toBool());
				}
				else if (line.hasPrefix("csv_no_response"))
				{
					conf.csv_no_response.push_back(((String)line.after("=")).trimLeft().toInt());
				}
				else if (line.hasPrefix("csv_compound_labels"))
				{
					conf.csv_compound_labels.push_back(((String)line.after("=")).trimLeft().toBool());
				}
				else if (line.hasPrefix("nonnumeric_class_names"))
				{
					conf.nonnumeric_class_names = ((String)line.after("=")).trimLeft().toBool();
				}
				else
				{
					String m = "Configuration option \""+line+"\" unknown!!";
					throw Exception::ConfigurationReadingError(__FILE__, __LINE__, m.c_str());
				}
			}
			
			unsigned int no = conf.csv_file.size();
			if (no != conf.csv_separator.size() || no != conf.csv_desc_labels.size() || no != conf.csv_compound_labels.size() || no != conf.csv_no_response.size())
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Some options for CSV-files to be appended to other input data are missing!"); 
			}
			
			if (conf.sd_file == "" && (conf.csv_file.size() == 0 || conf.csv_file[0] == ""))
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "SD-file or CSV-file must be specified within config-file!"); 
			}
			if (conf.output == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Output file must be specified within config-file!"); 
			}
				
			return conf;
		}



		/// ------- Reading sections for random partitioning of input data --------

		InputPartitioningConfiguration::InputPartitioningConfiguration()
		{
			input_file="";
			validation_fraction = 0;
			no_folds = 0;
			ID = -1;
			done = 0;
			even_split = 0;
		}

		InputPartitioningConfiguration ConfigIO::readInputPartitioningConfiguration(istream* input)
		{
			InputPartitioningConfiguration conf;
			
			for (int j = 0; !input->eof(); j++) // read ONE Validator section
			{
				String line;
				getline(*input, line);
						
				if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
				{
					j--;
					continue;
				}
					
				line.trim();
					
				if (j == 0)
				{
					if (!line.hasPrefix("[InputPartitioner]"))
					{
						throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The given section is no InputPartitioning-section!");
					}
					else continue;
				}
				else if (line.hasPrefix("["))
				{
					putbackLine(input, line);
					break;
				}	
				
				if (line.hasPrefix("done"))
				{
					conf.done = ((String)line.after("=")).trimLeft().toBool();
				}		
				else if (line.hasPrefix("input_file"))
				{
					conf.input_file = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("val_fraction"))
				{
					conf.validation_fraction = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("no_folds"))
				{
					conf.no_folds = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("ID"))
				{
					conf.ID = ((String)line.after("=")).trimLeft().toInt();
				}
				else if(line.hasPrefix("even_split"))
				{
					conf.even_split = ((String)line.after("=")).trimLeft().toInt();
				}
				else
				{
					String m = "Configuration command \""+line+"\" unknown!!";
					throw Exception::ConfigurationReadingError(__FILE__, __LINE__, m.c_str());
				}	
			}
			
			if (conf.input_file == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Input-file must be specified within config-file!"); 
			}
			if (conf.validation_fraction == 0)
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The fraction of the input data to be set aside for validation must be specified within config-file!"); 
			}
			if (conf.no_folds == 0)
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The number of folds must be specified within config-file!"); 
			}
			if (conf.ID == -1)
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The ID of this input partitioning must be specified within config-file!"); 
			}
			
			return conf;	
		}	
				
				
				
		/// ---------------- Reading of Model-sections ------------------
				
		ModelConfiguration::ModelConfiguration()
		{
			done = 0;
			data_file="";
			descriptor_source_model = "";
			output="";
			model_no = -1; 
			kernel_type = 0;
			kernel_par1 = 0;
			kernel_par2 = 0;
			model_parameters.resize(0);
			k_fold = 0;
			optimize_model_parameters = 0;
			grid_search_stepwidth = 0;
			grid_search_steps = 0;
			grid_search_recursions = 0;
			no_training = 0;
		}


		ModelConfiguration ConfigIO::readModelConfiguration(istream* input)
		{
			ModelConfiguration conf;
			
			
			for (int j = 0; !input->eof(); j++) // read ONE ModelCreator section
			{			
				String line;
				getline(*input, line);
				
				if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
				{
					j--;
					continue;
				}
					
				line.trim();
					
				if (j == 0)
				{
					if (!line.hasPrefix("[ModelCreator]"))
					{
						throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The given secton is no Model-section!!");
					}
					else continue;
				}
				else if (line.hasPrefix("["))
				{
					putbackLine(input, line);
					break;
				}
						
				if (line.hasPrefix("done"))
				{
					conf.done = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("data_file"))
				{
					conf.data_file = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("output"))
				{
					conf.output = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("descriptor_source_model"))
				{
					conf.descriptor_source_model = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("model_parameters"))
				{
					line = ((String)line.after("=")).trimLeft();
					unsigned int no = line.countFields(" ");
					for (unsigned int i = 0; i < no; i++)
					{
						conf.model_parameters.push_back(line.getField(i).toDouble());
					}
				}
				else if (line.hasPrefix("model_no"))
				{
					conf.model_no = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("grid_search_steps"))
				{
					conf.grid_search_steps = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("grid_search_recursions"))
				{
					conf.grid_search_recursions = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("grid_search_stepwidth"))
				{
					conf.grid_search_stepwidth = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("k_fold"))
				{
					conf.k_fold = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("optimize_model_parameters"))
				{
					conf.optimize_model_parameters = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("kernel_type"))
				{
					conf.kernel_type = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("kernel_par1"))
				{
					conf.kernel_par1 = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("kernel_par2"))
				{
					conf.kernel_par2 = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("no_training"))
				{
					conf.no_training = ((String)line.after("=")).trimLeft().toBool();
				}
				else
				{
					String m = "Configuration option \""+line+"\" unknown!!";
					throw Exception::ConfigurationReadingError(__FILE__, __LINE__, m.c_str());
				}
			}
				
			if (conf.data_file == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "QSARData file must be specified within config-file!"); 
			}
			if (conf.output == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Output file must be specified within config-file!"); 
			}
			if (conf.model_no == -1)
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Model type must be specified within config-file!"); 
			}
			
			return conf;
		}




		/// ---------------- Reading of Feature-Selection-sections ------------------


		FeatureSelectionConfiguration::FeatureSelectionConfiguration()
		{

			model="";
			data_file="";
			feat_type = 0;
			output="";
			k_fold = -1;
			statistic_name="average accuracy";
			statistic = 0;
			remove_correlated = 0;
			cor_threshold = -10;
			opt_model_after_fs = 0;
			opt_kernel_after_fs = 0;
			quality_increase_cutoff = -1;
			opt = 0;
			done = 0;
			grid_search_steps = 0;
			grid_search_recursions = 0;
			grid_search_stepwidth = 0;
			opt_k_fold = 0;
			selection_name = "";
		}


		FeatureSelectionConfiguration ConfigIO::readFeatureSelectionConfiguration(istream* input)
		{
			FeatureSelectionConfiguration conf;
			Registry reg;
			
			for (int j = 0; !input->eof(); j++) // read ONE FeatureSelector section
			{		
				String line;
				getline(*input, line);
						
				if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
				{
					j--;
					continue;
				}
					
				line = line.trim();
					
				if (j == 0)
				{
					if (!line.hasPrefix("[FeatureSelector]"))
					{
						throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The given secton is no FeatureSelection-section!!");
					}
					else continue;
				}
				else if (line.hasPrefix("["))
				{
					putbackLine(input, line);
					break;
				}	
					
				if (line.hasPrefix("done"))
				{
					conf.done = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("model_file"))
				{
					conf.model = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("data_file"))
				{
					conf.data_file = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("feature_selection_type"))
				{
					conf.feat_type = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("k_fold"))
				{
					conf.k_fold = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("quality_increase_cutoff"))
				{
					conf.quality_increase_cutoff = ((String)line.after("=")).trimLeft().toDouble();				
				}
				else if (line.hasPrefix("output"))
				{
					conf.output = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("remove_correlated_features"))
				{
					conf.remove_correlated = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("cor_threshold"))
				{
					conf.cor_threshold = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("optimize_parameters"))
				{
					conf.opt = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("classification_statistic") || line.hasPrefix("regression_statistic"))
				{
					String s = ((String)line.after("=")).trimLeft();
					if (s == "R2/Q2 version2") s="R2/Q2"; 
					conf.statistic_name = s;
					
					const map<unsigned int, String>* stats;
					if (line.hasPrefix("classification_statistic"))
					{
						stats = reg.getClassificationStatistics(); 
					}
					else stats = reg.getRegressionStatistics();
					
					string choices = ""; 
					bool ok = 0;
					for (map < unsigned int, String > ::const_iterator it = stats->begin(); it != stats->end(); it++)
					{
						if (s == it->second)
						{
							ok = 1;
							conf.statistic = it->first;
							break;
						}
						else
						{
							choices += it->second+", ";
						}
					}
					if (!ok)
					{
						String m = "qualitiy statistic \'"+s+"\' unknown!\n";
						m += "possible choices are: "+choices;
						
						throw Exception::ConfigurationReadingError(__FILE__, __LINE__, m.c_str());
					}				
				}
				else if (line.hasPrefix("opt_model_par_after_fs"))
				{
					conf.opt_model_after_fs = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("opt_kernel_par_after_fs"))
				{
					conf.opt_kernel_after_fs = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("grid_search_steps"))
				{
					conf.grid_search_steps = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("grid_search_recursions"))
				{
					conf.grid_search_recursions = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("grid_search_stepwidth"))
				{
					conf.grid_search_stepwidth = ((String)line.after("=")).trimLeft().toDouble();
				}
				else if (line.hasPrefix("opt_k_fold"))
				{
					conf.opt_k_fold = ((String)line.after("=")).trimLeft().toInt();
				}
				else
				{
					String m = "Configuration command \""+line+"\" unknown!!";
					throw Exception::ConfigurationReadingError(__FILE__, __LINE__, m.c_str());
				}
			}
						
			if (conf.data_file == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "QSARData file must be specified within config-file!"); 
			}
			if (conf.model == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Model file must be specified within config-file!"); 
			}
			if (conf.output == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Output file must be specified within config-file!"); 
			}
			if (conf.k_fold == -1 && !conf.remove_correlated && conf.feat_type != 0 && conf.feat_type != 4)
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "k-fold must be specified within config-file!"); 
			}
			if (conf.remove_correlated && conf.cor_threshold == -10)
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "\"cor_threshold\" must be specified when removing of highly correlated features is desired!"); 
			}
			if (conf.opt_model_after_fs && conf.opt_k_fold < 2)
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "\"opt_k_fold\" must be specified when model parameters should be optimized after feature selection!"); 
			}
			if (conf.opt_kernel_after_fs && (conf.opt_k_fold < 2 || conf.grid_search_steps == 0 || conf.grid_search_stepwidth == 0))
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "\"opt_k_fold\", \"grid_search_steps\" and \"grid_search_stepwidth\" must be specified when kernel parameters should be optimized after feature selection!"); 		
			}
			
			if (conf.remove_correlated) conf.selection_name="Remove Colinear Features"; 
			else if (conf.feat_type >= 0)
			{
				conf.selection_name = reg.getFeatureSelectionName(conf.feat_type);
			}
			else
			{
				String mess = "feature-selection type \"";
				mess += String(conf.feat_type) + "\" is unknown!";
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, mess.c_str());
			}
			
			
			return conf;
		}




		/// ---------------- Reading of Validation-sections ------------------


		ValidationConfiguration::ValidationConfiguration()
		{
			model="";
			data="";
			validation_data="";
			val_type = -1;
			output="";
			k_folds = 0;
			bootstrap_samples = 0;
			no_of_permutation_tests = 0;
			statistic = 0;
			statistic_name="average accuracy";
			for_visualization_only = 0; 
			external_predictions.resize(0);
			done = 0;
		}


		ValidationConfiguration ConfigIO::readValidationConfiguration(istream* input)
		{
			ValidationConfiguration conf;
			Registry reg;
			
			for (int j = 0; !input->eof(); j++) // read ONE Validator section
			{
				String line;
				getline(*input, line);
						
				if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
				{
					j--;
					continue;
				}
					
				line.trim();
					
				if (j == 0)
				{
					if (!line.hasPrefix("[Validator]"))
					{
						throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The given secton is no Validation-section!!");
					}
					else continue;
				}
				else if (line.hasPrefix("["))
				{
					putbackLine(input, line);
					break;
				}			
					
				if (line.hasPrefix("done"))
				{
					conf.done = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("model_file"))
				{
					conf.model = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("validation_type"))
				{
					conf.val_type = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("data_file"))
				{
					conf.data = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("validation_data_file"))
				{
					conf.validation_data = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("k_fold"))
				{
					conf.k_folds = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("bootstrap_samples"))
				{
					conf.bootstrap_samples = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("no_of_permutation_tests"))
				{
					conf.no_of_permutation_tests = ((String)line.after("=")).trimLeft().toInt();
				}
				else if (line.hasPrefix("output"))
				{
					conf.output = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("classification_statistic") || line.hasPrefix("regression_statistic"))
				{
					String s = ((String)line.after("=")).trimLeft();
					if (s == "R2/Q2 version2") s="R2/Q2"; 
					conf.statistic_name = s;
					
					const map<unsigned int, String>* stats;
					if (line.hasPrefix("classification_statistic"))
					{
						stats = reg.getClassificationStatistics(); 
					}
					else stats = reg.getRegressionStatistics();
					
					string choices = ""; 
					bool ok = 0;
					for (map < unsigned int, String > ::const_iterator it = stats->begin(); it != stats->end(); it++)
					{
						if (s == it->second)
						{
							ok = 1;
							conf.statistic = it->first;
							break;
						}
						else
						{
							choices += it->second+", ";
						}
					}
					if (!ok)
					{
						String m = "qualitiy statistic \'"+s+"\' unknown!\n";
						m += "possible choices are: "+choices;
						
						throw Exception::ConfigurationReadingError(__FILE__, __LINE__, m.c_str());
					}				
				}
				else if (line.hasPrefix("external_validation_predictions"))
				{
					conf.for_visualization_only = 1; 
					
					String file_names = ((String)line.after("=")).trimLeft();
					int no = file_names.countFields();
					for (int i = 0; i < no; i++)
					{
						conf.external_predictions.push_back(file_names.getField(i));
					}
				}
				else
				{
					String m = "Configuration command \""+line+"\" unknown!!";
					throw Exception::ConfigurationReadingError(__FILE__, __LINE__, m.c_str());
				}
			}
				
			if (conf.data == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "QSARData file must be specified within config-file!"); 
			}
			if (conf.model == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Model file must be specified within config-file!"); 
			}
		// 	if (conf.val_type == -1)
		// 	{
		// 		throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Type of Validation to be done must be specified within config-file!"); 
		// 	}
			if (conf.output == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "Output file must be specified within config-file!"); 
			}
			if ((conf.val_type == 3 || conf.val_type == 4 || conf.val_type == 6)  && conf.bootstrap_samples <= 0)
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The number of bootstrap samples must be specified within config-file!"); 
			}
			if (conf.k_folds == 0 && conf.no_of_permutation_tests > 0)
			{
				if (!conf.for_visualization_only)
				{
					throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "'k_folds' must be specified within config-file!"); 
				}
			}
			
			return conf;
		}



		/// ---------------- Reading of Prediction-sections ------------------


		PredictionConfiguration::PredictionConfiguration()
		{
			model="";
			data="";
			output="";
			print_expected = 1;
			statistic = 0;
			done = 0;
		}

		PredictionConfiguration ConfigIO::readPredictionConfiguration(istream* input)
		{
			PredictionConfiguration conf;
			Registry reg;
			
			for (int j = 0; !input->eof(); j++) // read ONE Predictor section
			{
				String line;
				getline(*input, line);
					
				if(line=="" || line.hasPrefix("#") || line.hasPrefix("//") || line.hasPrefix("%"))
				{
					j--;
					continue;
				}
					
				line.trim();
					
				if (j == 0)
				{
					if (!line.hasPrefix("[Predictor]"))
					{
						throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "The given secton is no Prediction-section!!");
					}
					else continue;
				}
				else if (line.hasPrefix("["))
				{
					putbackLine(input, line);
					break;
				}	
					
				if (line.hasPrefix("done"))
				{
					conf.done = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("model_file"))
				{
					conf.model = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("output"))
				{
					conf.output = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("data_file")) // compounds to be predicted
				{
					conf.data = ((String)line.after("=")).trimLeft();
				}
				else if (line.hasPrefix("print_expected")) // compounds to be predicted
				{
					conf.print_expected = ((String)line.after("=")).trimLeft().toBool();
				}
				else if (line.hasPrefix("classification_statistic") || line.hasPrefix("regression_statistic"))
				{
					String s = ((String)line.after("=")).trimLeft();
					if (s == "R2/Q2 version2") s="R2/Q2"; 
					conf.statistic_name = s;
					
					const map<unsigned int, String>* stats;
					if (line.hasPrefix("classification_statistic"))
					{
						stats = reg.getClassificationStatistics(); 
					}
					else stats = reg.getRegressionStatistics();
					
					string choices = ""; 
					bool ok = 0;
					for (map < unsigned int, String > ::const_iterator it = stats->begin(); it != stats->end(); it++)
					{
						if (s == it->second)
						{
							ok = 1;
							conf.statistic = it->first;
							break;
						}
						else
						{
							choices += it->second+", ";
						}
					}
					if (!ok)
					{
						String m = "qualitiy statistic \'"+s+"\' unknown!\n";
						m += "possible choices are: "+choices;
						
						throw Exception::ConfigurationReadingError(__FILE__, __LINE__, m.c_str());
					}				
				}
				else
				{
					String m = "Configuration command \""+line+"\" unknown!!";
					throw Exception::ConfigurationReadingError(__FILE__, __LINE__, m.c_str());
				}
			}
				
			if (conf.model == "" || conf.data == "" || conf.output == "")
			{
				throw Exception::ConfigurationReadingError(__FILE__, __LINE__, "'model-file', 'output' and 'data-file' must be specified within config-file!"); 
			}
			
			return conf;	
		}
	}
}
