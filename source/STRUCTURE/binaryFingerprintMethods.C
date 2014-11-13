// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/binaryFingerprintMethods.h>

#include <BALL/COMMON/limits.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/sysinfo.h>
#include <BALL/SYSTEM/timer.h>

#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;
using namespace BALL;


const String BinaryFingerprintMethods::Option::BLOCKSIZE = "blocksize";
const String BinaryFingerprintMethods::Option::SIM_CUTOFF = "sim_cutoff";
const String BinaryFingerprintMethods::Option::STORE_NN = "store_nns";
const String BinaryFingerprintMethods::Option::PRECISION = "precision";
const String BinaryFingerprintMethods::Option::MAX_CLUSTERS = "max_clusters";
const String BinaryFingerprintMethods::Option::N_THREADS = "n_threads";
const String BinaryFingerprintMethods::Option::VERBOSITY = "verbosity";


const unsigned short BinaryFingerprintMethods::Default::BLOCKSIZE = 850;
const float BinaryFingerprintMethods::Default::SIM_CUTOFF = 0.7;
const float BinaryFingerprintMethods::Default::PRECISION = 0.000001;
const unsigned int BinaryFingerprintMethods::Default::MAX_CLUSTERS = 10000;
const bool BinaryFingerprintMethods::Default::STORE_NN = false;
const unsigned int BinaryFingerprintMethods::Default::N_THREADS = 1;
const int BinaryFingerprintMethods::Default::VERBOSITY = 0;


BinaryFingerprintMethods::BinaryFingerprintMethods()
{
	Options options;
	
	setup(options);
	
	lib_features_ = NULL;
	query_features_ = NULL;
}


BinaryFingerprintMethods::BinaryFingerprintMethods(const Options& options)
{
	setup(options);
	
	lib_features_ = NULL;
	query_features_ = NULL;
}


BinaryFingerprintMethods::BinaryFingerprintMethods(const Options& options, const FingerprintFeatures& lib_features)
{
	setup(options);
	
	if (!setLibraryFeatures(lib_features))
	{
		Log.error() << "-- FAILED: Setup of library features failed - invalid data format." << endl;
		Log.error() << "--" << endl;
	}
	
	query_features_ = NULL;
}


BinaryFingerprintMethods::BinaryFingerprintMethods(const Options& options, const FingerprintFeatures& lib_features, const FingerprintFeatures& query_features)
{
	setup(options);
	
	if (!setLibraryFeatures(lib_features))
	{
		Log.error() << "-- FAILED: Setup of library features failed - invalid data format." << endl;
		Log.error() << "--" << endl;
	}
	
	if (!setQueryFeatures(query_features))
	{
		Log.error() << "-- FAILED: Setup of query features failed - invalid data format." << endl;
		Log.error() << "--" << endl;
	}
}


BinaryFingerprintMethods::BinaryFingerprintMethods(const BinaryFingerprintMethods& bfm)
{
	assign(bfm);
}


BinaryFingerprintMethods::~BinaryFingerprintMethods()
{
	clear();
}


BinaryFingerprintMethods& BinaryFingerprintMethods::operator = (const BinaryFingerprintMethods& bfm)
{
	assign(bfm);
	
	return *this;
}


void BinaryFingerprintMethods::assign(const BinaryFingerprintMethods& bfm)
{
	if (&bfm != this)
	{
		lib_features_ = bfm.lib_features_;
		query_features_ = bfm.query_features_;
		
		options_ = bfm.options_;
		n_threads_ = bfm.n_threads_;
		threads_ = NULL;
		thread_data_ = NULL;
		blocksize_ = bfm.blocksize_;
		cc_matrix_size_ = bfm.cc_matrix_size_;
		cutoff_ = bfm.cutoff_;
		precision_ = bfm.precision_;
		store_nns_ = bfm.store_nns_;
		verbosity_ = bfm.verbosity_;
		max_clusters_ = bfm.max_clusters_;
	}
}


void BinaryFingerprintMethods::setup(const Options& options)
{
	if (options.isSet(Option::BLOCKSIZE))
	{
		options_.setDefaultInteger(Option::BLOCKSIZE, options.getInteger(Option::BLOCKSIZE));
	}
	else
	{
		options_.setDefaultInteger(Option::BLOCKSIZE, Default::BLOCKSIZE);
	}
	
	if (options.isSet(Option::SIM_CUTOFF))
	{
		options_.setDefaultReal(Option::SIM_CUTOFF, options.getReal(Option::SIM_CUTOFF));
	}
	else
	{
		options_.setDefaultReal(Option::SIM_CUTOFF, Default::SIM_CUTOFF);
	}
	
	if (options.isSet(Option::STORE_NN))
	{
		options_.setDefaultBool(Option::STORE_NN, options.getBool(Option::STORE_NN));
	}
	else
	{
		options_.setDefaultBool(Option::STORE_NN, Default::STORE_NN);
	}
	
	if (options.isSet(Option::PRECISION))
	{
		options_.setDefaultReal(Option::PRECISION, options.getReal(Option::PRECISION));
	}
	else
	{
		options_.setDefaultReal(Option::PRECISION, Default::PRECISION);
	}
	
	if (options.isSet(Option::MAX_CLUSTERS))
	{
		options_.setDefaultInteger(Option::MAX_CLUSTERS, options.getInteger(Option::MAX_CLUSTERS));
	}
	else
	{
		options_.setDefaultInteger(Option::MAX_CLUSTERS, Default::MAX_CLUSTERS);
	}
	
	if (options.isSet(Option::N_THREADS))
	{
		options_.setDefaultInteger(Option::N_THREADS, options.getInteger(Option::N_THREADS));
	}
	else
	{
		options_.setDefaultInteger(Option::N_THREADS, Default::N_THREADS);
	}
	
	if (options.isSet(Option::VERBOSITY))
	{
		options_.setDefaultInteger(Option::VERBOSITY, options.getInteger(Option::VERBOSITY));
	}
	else
	{
		options_.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
	}
	
	if (SysInfo::getNumberOfProcessors() != -1)
	{
		if (options_.getInteger("n_threads") > SysInfo::getNumberOfProcessors())
		{
			options_.setInteger("n_threads", SysInfo::getNumberOfProcessors());
		}
	}
	
	threads_ = NULL;
	thread_data_ = NULL;
	
	setBlockSize(options_.getInteger(Option::BLOCKSIZE));
	cutoff_ = options_.getReal(Option::SIM_CUTOFF);
	store_nns_ = options_.getBool(Option::STORE_NN);
	precision_ = options_.getReal(Option::PRECISION);
	max_clusters_ = options_.getInteger(Option::MAX_CLUSTERS);
	n_threads_ = options_.getInteger(Option::N_THREADS);
	verbosity_ = options_.getInteger(Option::VERBOSITY);
}


void BinaryFingerprintMethods::clear()
{
	destroyThreadData();
	
	for (unsigned int i=0; i!=lib_iindices_.size(); ++i)
	{
		destroyInvertedIndex(lib_iindices_[i]);
	}
	lib_iindices_.clear();
	
	for (unsigned int i=0; i!=query_iindices_.size(); ++i)
	{
		destroyInvertedIndex(query_iindices_[i]);
	}
	query_iindices_.clear();
}


const Options& BinaryFingerprintMethods::getOptions() const
{
	return options_;
}



void BinaryFingerprintMethods::setVerbosityLevel(const int verbosity)
{
	verbosity_ = verbosity;
}


bool BinaryFingerprintMethods::parseBinaryFingerprint(const String& fprint, vector<unsigned short>& features, unsigned int fp_type, const char* delim)
{
	features.clear();
	
	switch (fp_type)
	{
		case (1):
		{
			for (unsigned int i=0; i!=fprint.size(); ++i)
			{
				switch (fprint[i])
				{
					case '1':
					{
						features.push_back(i+1);
						break;
					}
					case '0':
					{
						break;
					}
					default:
					{
						// Fingerprint is invalid
						features.clear();
						
						return false;
					}
				}
			}
			
			break;
		}
		case (2):
		{
			vector<String> fp_split;
			fprint.split(fp_split, delim);
			
			try
			{
				for (unsigned int i=0; i!=fp_split.size(); ++i)
				{
					features.push_back(fp_split[i].toUnsignedShort() + 1);
				}
			}
			catch (Exception::InvalidFormat e)
			{
				// Fingerprint is invalid
				features.clear();
				
				return false;
			}
			
			break;
		}
		default:
		{
			return false;
		}
	}
	
	reverse(features.begin(), features.end());
	
	return true;
}


void BinaryFingerprintMethods::createThreadData(const unsigned int blocksize, const unsigned int dataset_size, const unsigned int active_iids_size)
{
	threads_ = new thread[n_threads_];
	thread_data_ = new ThreadData[n_threads_];
	
	for (unsigned int i=0; i!=n_threads_; ++i)
	{
		thread_data_[i].first = 0;
		thread_data_[i].last = 0;
		
		thread_data_[i].nn_sim = 0.0;
		thread_data_[i].blocksize = blocksize + 1;
		thread_data_[i].dataset_size = dataset_size;
		thread_data_[i].active_iids_size = active_iids_size;
		
		thread_data_[i].cc_row = new unsigned short[thread_data_[i].blocksize];
		thread_data_[i].cc_matrix = new unsigned short*[thread_data_[i].blocksize];
		for (unsigned short j=0; j!=thread_data_[i].blocksize; ++j)
		{
			thread_data_[i].cc_matrix[j] = new unsigned short[thread_data_[i].blocksize];
		}
		
		if (dataset_size!=0)
		{
			thread_data_[i].uint_array = new unsigned int[dataset_size];
			thread_data_[i].float_array = new float[dataset_size];
		}
		else
		{
			thread_data_[i].uint_array = NULL;
			thread_data_[i].float_array = NULL;
		}
		
		if (active_iids_size!=0)
		{
			thread_data_[i].dprec_matrix = new double*[active_iids_size];
			for (unsigned int j=0; j!=active_iids_size; ++j)
			{
				thread_data_[i].dprec_matrix[j] = new double[dataset_size];
				
				for (unsigned int k=0; k!=dataset_size; ++k)
				{
					thread_data_[i].dprec_matrix[j][k] = 0.0;
				}
			}
		}
		else
		{
			thread_data_[i].dprec_matrix = NULL;
		}
	}
}


void BinaryFingerprintMethods::destroyThreadData()
{
	if (thread_data_!=NULL)
	{
		for (unsigned int i=0; i!=n_threads_; ++i)
		{
			if (thread_data_[i].cc_matrix != NULL)
			{
				for (unsigned int j=0; j!=thread_data_[i].blocksize; ++j)
				{
					delete [] thread_data_[i].cc_matrix[j];
				}
				
				delete [] thread_data_[i].cc_matrix;
				thread_data_[i].cc_matrix = NULL;
			}
			
			if (thread_data_[i].cc_row!=NULL)
			{
				delete [] thread_data_[i].cc_row;
				thread_data_[i].cc_row = NULL;
			}
			
			if (thread_data_[i].uint_array != NULL)
			{
				delete [] thread_data_[i].uint_array;
				thread_data_[i].uint_array = NULL;
			}
			
			if (thread_data_[i].float_array != NULL)
			{
				delete [] thread_data_[i].float_array;
				thread_data_[i].float_array = NULL;
			}
			
			if (thread_data_[i].dprec_matrix != NULL)
			{
				for (unsigned int j=0; j!=thread_data_[i].active_iids_size; ++j)
				{
					delete [] thread_data_[i].dprec_matrix[j];
				}
				
				delete [] thread_data_[i].dprec_matrix;
				thread_data_[i].dprec_matrix = NULL;
			}
		}
		
		delete [] thread_data_;
		thread_data_ = NULL;
	}
	
	if (threads_ != NULL)
	{
		delete [] threads_;
		threads_ = NULL;
	}
}


bool BinaryFingerprintMethods::setLibraryFeatures(const FingerprintFeatures& lib_features)
{
	unsigned short previous;
	
	for (unsigned int i=0; i!=lib_features.size(); ++i)
	{
		if (lib_features[i].empty())
		{
			lib_features_ = 0;
			return false;
		}
		
		previous = lib_features[i][0];
		for (unsigned int j=1; j!=lib_features[i].size(); ++j)
		{
			if (lib_features[i][j] >= previous)
			{
				lib_features_ = 0;
				return false;
			}
			
			previous = lib_features[i][j];
		}
	}
	
	lib_features_ = &lib_features;
	
	return true;
}


bool BinaryFingerprintMethods::setQueryFeatures(const FingerprintFeatures& query_features)
{
	unsigned short previous;
	
	for (unsigned int i=0; i!=query_features.size(); ++i)
	{
		if (query_features[i].empty())
		{
			query_features_ = 0;
			return false;
		}
		
		previous = query_features[i][0];
		for (unsigned int j=1; j!=query_features[i].size(); ++j)
		{
			if (query_features[i][j] >= previous)
			{
				query_features_ = 0;
				return false;
			}
			
			previous = query_features[i][j];
		}
	}
	
	query_features_ = &query_features;
	
	return true;
}


unsigned int BinaryFingerprintMethods::getTargetLibrarySize() const
{
	if (lib_features_ != NULL)
	{
		return lib_features_->size();
	}
	
	return 0;
}


unsigned int BinaryFingerprintMethods::getQueryLibrarySize() const
{
	if (query_features_ != NULL)
	{
		return query_features_->size();
	}
	
	return 0;
}


BALL_INLINE
void BinaryFingerprintMethods::arrayToUpperTriangluarMatrix(LongSize& row, LongSize& col, const LongSize array_index) const
{
	/*
	 *    UPPER TRIANGULAR MATRIX indexing, i.e. including diagonal => (n(n+1) / 2) elements
	 *    Calculate row and column indices [0, n[ from cell index [0, (n(n+1) / 2)[.
	 * 
	 *     | 0  1  2  3  ... 
	 *   ---------------
	 *   0 | 0  1  3  6  
	 *   1 | -  2  4  7
	 *   2 | -  -  5  8
	 *   3 | -  -  -  9
	 * 
	 */
	
	col = floor(sqrt(2 * array_index + 0.25) - 0.5);
	row = array_index - ((col * col + col) / 2);
}


BALL_INLINE
void BinaryFingerprintMethods::arrayToStrictUpperTriangularMatrix(LongSize& row, LongSize& col, const LongSize array_index) const
{
	/*
	 *    STRICT UPPER TRIANGULAR MATRIX indexing, i.e. without diagonal => (n(n-1) / 2) elements.
	 *    Calculate row and column indices [0, n[ from cell index [0, (n(n-1) / 2)[.
	 * 
	 *     | 0  1  2  3  ... 
	 *   ---------------
	 *   0 | -  0  1  3  
	 *   1 | -  -  2  4
	 *   2 | -  -  -  5
	 *   3 | -  -  -  -
	 * 
	 */
	
	col = floor(sqrt(2*array_index + 0.25) + 0.5);
	row = array_index - ((col*col - col) / 2);
}


BALL_INLINE
LongSize BinaryFingerprintMethods::strictUpperTriangularMatrixToArray(const LongSize row, const LongSize col) const
{
	/*
	 *    STRICT UPPER TRIANGULAR MATRIX indexing, i.e. without diagonal => (n(n-1) / 2) elements.
	 *    Calculate cell index [0, (n(n-1) / 2)[ from row and column indices [0, n[.
	 * 
	 *     | 0  1  2  3  ... 
	 *   ---------------
	 *   0 | -  0  1  3  
	 *   1 | -  -  2  4
	 *   2 | -  -  -  5
	 *   3 | -  -  -  -
	 * 
	 */
	
	return ((col*col - col) / 2 + row);
}


BALL_INLINE
bool BinaryFingerprintMethods::getNextComparisonIndex(LongSize& index)
{
	boost::mutex::scoped_lock lock(out_mutex_);
	
	if (n_comparisons_ == 0)
	{
		return false;
	}
	
	--n_comparisons_;
	
	index = n_comparisons_;
	
	return true;
}


BALL_INLINE
bool BinaryFingerprintMethods::checkSimilaritySwitch(const float a_sim, const float b_sim, const unsigned int a_id, const unsigned int b_id) const
{
	if (fabs(a_sim - b_sim) <= precision_)
	{
		if (a_id < b_id)
		{
			return true;
		}
	}
	else
	{
		if (a_sim > b_sim)
		{
			return true;
		}
	}
	
	return false;
}


void BinaryFingerprintMethods::setBlockSize(const unsigned short blocksize)
{
	if (blocksize > Limits<unsigned short>::max())
	{
		blocksize_ = Limits<unsigned short>::max() - 1;
	}
	else
	{
		blocksize_ = blocksize;
	}
	
	cc_matrix_size_ = sizeof(unsigned short) * (blocksize_ + 1);
}


BinaryFingerprintMethods::InvertedIndex* BinaryFingerprintMethods::createInvertedIndex(const vector<pair<const vector<unsigned short>*, unsigned int> >& members)
{
	InvertedIndex* ii = new InvertedIndex;
	ii->n_molecules = members.size();
	ii->n_features = new unsigned short[ii->n_molecules];
	ii->parent_clusters = new unsigned int[ii->n_molecules];
	
	unsigned int f_count;
// 	unsigned short *features;
	const vector<unsigned short>* features;
	vector<vector<unsigned short> > feature_list(Limits<unsigned short>::max() + 1, vector<unsigned short>());
	for (unsigned int i=0; i!=members.size(); ++i)
	{
		f_count = 0;
		features = members[i].first;
		for (unsigned int j=0; j!=features->size(); ++j)
		{
			feature_list[(*features)[j]].push_back(i+1);
			++f_count;
		}
		
		ii->n_features[i] = f_count;
		ii->parent_clusters[i] = members[i].second;
	}
	
	unsigned short num_features = 0;
	for (unsigned int i=0; i!=feature_list.size(); ++i)
	{
		if (feature_list[i].size())
		{
			++num_features;
			feature_list[i].push_back(0);
		}
	}
	
	ii->feature_skip_list = new FeatureList[num_features + 1];
	ii->feature_skip_list[num_features].feature_id = 0;
	
	size_t block_pos_size;
	FeatureList *f_list = ii->feature_skip_list;
	for (unsigned int i=feature_list.size()-1; i!=0; --i)
	{
		block_pos_size = feature_list[i].size();
		if (block_pos_size)
		{
			f_list->feature_id = i;
			f_list->block_positions = new unsigned short[block_pos_size];
			
			memcpy(f_list->block_positions, &feature_list[i][0], sizeof(unsigned short)*block_pos_size);
			
			++f_list;
		}
	}
	
	return ii;
}


void BinaryFingerprintMethods::destroyInvertedIndex(InvertedIndex* ii)
{
	FeatureList *f_list = ii->feature_skip_list;
	while (f_list->feature_id)
	{
		if (f_list->block_positions!=NULL)
		{
			delete [] f_list->block_positions;
		}
		
		++f_list;
	}
	
	delete [] ii->feature_skip_list;
	delete [] ii->parent_clusters;
	delete [] ii->n_features;
	delete ii;
}


void BinaryFingerprintMethods::destroyInvertedIndices(InvertedIndices& ii_destroy)
{
	for (unsigned int i=0; i!=ii_destroy.size(); ++i)
	{
		destroyInvertedIndex(ii_destroy[i]);
	}
	
	ii_destroy.clear();
}


void BinaryFingerprintMethods::createInvertedIndices(const vector<pair<const vector<unsigned short>*, unsigned int> >& molecules, InvertedIndices& target)
{
	vector<pair<const vector<unsigned short>*, unsigned int> > tmp;
	
	for (unsigned int i=0; i < molecules.size(); ++i)
	{
		tmp.push_back(molecules[i]);
		if ((i+1) % blocksize_ == 0)
		{
			target.push_back(createInvertedIndex(tmp));
			tmp.clear();
		}
	}
	
	if (tmp.size())
	{
		target.push_back(createInvertedIndex(tmp));
		tmp.clear();
	}
}


void BinaryFingerprintMethods::calculateCommonCounts_1_1(const FeatureList* ii1, const FeatureList* ii2, unsigned short& cc)
{
	// ###########################################################################################################################
	// For a general description of this method you can take a look 
	// at the comparable function BinaryFingerprintMethods::calculateCommonCounts_M_N( ... )
	// ###########################################################################################################################
	
	while (ii1->feature_id != ii2->feature_id)
	{
		if (ii1->feature_id > ii2->feature_id)
		{
			++ii1;
		}
		else
		{
			++ii2;
		}
	}
	
	while (ii1->feature_id && ii2->feature_id)
	{
		++cc;
		
		++ii1;
		++ii2;
		
		while (ii1->feature_id != ii2->feature_id)
		{
			if (ii1->feature_id > ii2->feature_id)
			{
				++ii1;
			}
			else
			{
				++ii2;
			}
		}
	}
}


void BinaryFingerprintMethods::calculateCommonCounts_1_N(const FeatureList* ii1, const FeatureList* ii2, unsigned short* cc_row)
{
	// ###########################################################################################################################
	// For a general description of this method you can take a look 
	// at the comparable function BinaryFingerprintMethods::calculateCommonCounts_M_N( ... )
	// ###########################################################################################################################
	
	unsigned short* ii2_position;
	
	while (ii1->feature_id != ii2->feature_id)
	{
		if (ii1->feature_id > ii2->feature_id)
		{
			++ii1;
		}
		else
		{
			++ii2;
		}
	}
	
	while (ii1->feature_id && ii2->feature_id)
	{
		ii2_position = ii2->block_positions;
		
		while (*ii2_position)
		{
			++*(cc_row + *ii2_position);
			++ii2_position;
		}
		
		++ii1;
		++ii2;
		
		while (ii1->feature_id != ii2->feature_id)
		{
			if (ii1->feature_id > ii2->feature_id)
			{
				++ii1;
			}
			else
			{
				++ii2;
			}
		}
	}
}


void BinaryFingerprintMethods::calculateCommonCounts_M_N(const InvertedIndex* ii1, const InvertedIndex* ii2, unsigned short** cc_matrix)
{
	FeatureList *f1 = ii1->feature_skip_list;
	FeatureList *f2 = ii2->feature_skip_list;
	
	unsigned short *ii1_position;
	unsigned short *ii2_position;
	unsigned short *cc_matrix_f1;
	
	// ###########################################################################################################################
	// STEP 1:   Make both feature_skip_lists pointing to FeatureLists with same FeatureID.
	//
	//           InvertedIndices are sorted from highest to lowest FeatureID and terminated with default feature ( FeatureID = 0 )!!!
	//           If feature_skip_lists posess no (more) common feature, b1 and b2 finally point to the default (last) 
	//           feature at the end of every feature_skip_lists.
	// ###########################################################################################################################
	while (f1->feature_id != f2->feature_id)
	{
		if (f1->feature_id > f2->feature_id)
		{
			++f1;
		}
		else
		{
			++f2;
		}
	}
	// #####################################################################################################
	// STEP 1 END   
	// #####################################################################################################
	
	if (ii1 != ii2)
	{
		// ###########################################################################################################################
		// OUTER LOOP :           Repeat until one of the InvertedIndices reached the end and no more common features 
		//                        between InvertedIndex 1 and InvertedIndex 2 exist. A InvertedIndices end is indicated  
		//                        via a terminating feature_skip_list with FeatureID 0.
		// ###########################################################################################################################
		while (f1->feature_id && f2->feature_id)
		{
			// #####################################################################################################
			// INNER LOOP 1:   Iterate over all molecules in InvertedIndex 1 with the current FeatureID
			// #####################################################################################################
			ii1_position = f1->block_positions;
			
			while (*ii1_position)
			{
				cc_matrix_f1 = cc_matrix[*ii1_position];
				
				
				// #####################################################################################################
				// INNER LOOP 1.1.:   Iterate over all molecules in InvertedIndex 2 with the current FeatureID and 
				//                    incerement common counts matrix for every molecule pair.
				//
				//                    Molecules are only represented by their position in the corresponding InvertedIndex
				// #####################################################################################################
				ii2_position = f2->block_positions;
				
				while (*ii2_position)
				{
					++*(cc_matrix_f1 + *ii2_position);
					
					++ii2_position;
				}
				// #####################################################################################################
				// INNER LOOP 1.1. END
				// #####################################################################################################
				
				++ii1_position;
			}
			// #####################################################################################################
			// INNER LOOP 1 END   
			// #####################################################################################################
			
			
			// ###########################################################################################################################
			// INNER LOOP 2:   Make both feature_skip_lists pointing to the NEXT FeatureLists with same FeatureID.
			//
			//                 In principle same as STEP 1
			// ###########################################################################################################################
			++f1;
			++f2;
			
			while (f1->feature_id != f2->feature_id)
			{
				if (f1->feature_id > f2->feature_id)
				{
					++f1;
				}
				else
				{
					++f2;
				}
			}
			// #####################################################################################################
			// INNER LOOP 2 END   
			// #####################################################################################################
			
		}
		// #####################################################################################################
		// OUTER LOOP END   
		// #####################################################################################################
	}
	else
	{
		while (f1->feature_id && f2->feature_id)
		{
			ii1_position = f1->block_positions;
			
			while (*ii1_position)
			{
				cc_matrix_f1 = cc_matrix[*ii1_position];
				
				//ii2_position = b2->block_positions;
				ii2_position = ii1_position + 1;
				
				while (*ii2_position)
				{
					++*(cc_matrix_f1 + *ii2_position);
					
					++ii2_position;
				}
				
				++ii1_position;
			}
			
			++f1;
			++f2;
			
			while (f1->feature_id != f2->feature_id)
			{
				if (f1->feature_id > f2->feature_id)
				{
					++f1;
				}
				else
				{
					++f2;
				}
			}
		}
	}
}


void BinaryFingerprintMethods::pairwiseSimilaritiesNearestNeighbours(const unsigned int ii1_index, const unsigned int ii2_index, ThreadData* t_data)
{
	unsigned short* cc_matrix_row;
	unsigned short** cc_matrix = t_data->cc_matrix;
	
	float* nn_sim = t_data->float_array;
	unsigned int* nn_list = t_data->uint_array;
	
	float c, coeff;
	unsigned int tmp_molecule_id;
	unsigned short tmp_n_features;
	
	InvertedIndex *ii1 = lib_iindices_[ii1_index];
	InvertedIndex *ii2 = lib_iindices_[ii2_index];
	unsigned int ii2_n_molecules = ii2->n_molecules;
	unsigned short* ii1_n_features = ii1->n_features;
	unsigned short* ii2_n_features = ii2->n_features;
	unsigned int ii1_molecules_base = ii1_index * blocksize_;
	unsigned int ii2_molecules_base = ii2_index * blocksize_;
	
	if (ii1!=ii2)
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_molecule_id = ii1_molecules_base + u;
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=0; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
				if (checkSimilaritySwitch(coeff, nn_sim[tmp_molecule_id], ii2_molecules_base + v,  nn_list[tmp_molecule_id]))
				{
					nn_sim[tmp_molecule_id] = coeff;
					nn_list[tmp_molecule_id] = ii2_molecules_base + v;
				}
				if (checkSimilaritySwitch(coeff, nn_sim[ii2_molecules_base + v], tmp_molecule_id, nn_list[ii2_molecules_base + v]))
				{
					nn_sim[ii2_molecules_base + v] = coeff;
					nn_list[ii2_molecules_base + v] = tmp_molecule_id;
				}
			}
		}
	}
	// When comparing identical InvertedIndices ( i==j ) only inter-InvertedIndex pairs of different molecules should be considered
	// Therefore the inner loop variable is everytime set to ( outer-loop-variable + 1 )
	else
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_molecule_id = ii1_molecules_base + u;
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=u+1; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
				if (checkSimilaritySwitch(coeff, nn_sim[tmp_molecule_id], ii2_molecules_base + v,  nn_list[tmp_molecule_id]))
				{
					nn_sim[tmp_molecule_id] = coeff;
					nn_list[tmp_molecule_id] = ii2_molecules_base + v;
				}
				if (checkSimilaritySwitch(coeff, nn_sim[ii2_molecules_base + v], tmp_molecule_id, nn_list[ii2_molecules_base + v]))
				{
					nn_sim[ii2_molecules_base + v] = coeff;
					nn_list[ii2_molecules_base + v] = tmp_molecule_id;
				}
			}
		}
	}
}


void BinaryFingerprintMethods::pairwiseSimilaritiesStoredMatrix(const unsigned int ii1_index, const unsigned int ii2_index, ThreadData* t_data)
{
	unsigned short* cc_matrix_row;
	unsigned short** cc_matrix = t_data->cc_matrix;
	
	float c, coeff;
	unsigned int tmp_molecule_id;
	unsigned short tmp_n_features;
	
	InvertedIndex *ii1 = lib_iindices_[ii1_index];
	InvertedIndex *ii2 = lib_iindices_[ii2_index];
	
	unsigned int ii2_n_molecules = ii2->n_molecules;
	unsigned short* ii1_n_features = ii1->n_features;
	unsigned short* ii2_n_features = ii2->n_features;
	
	unsigned int ii1_molecules_base = ii1_index * blocksize_;
	unsigned int ii2_molecules_base = ii2_index * blocksize_;
	
	if (ii1!=ii2)
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_molecule_id = ii1_molecules_base + u;
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=0; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
				// CONCURRENT WRITE ACCESS ON SIM_MATRIX ARRAY WITHOUT MUTEX LOCK
				// This is valid because every array field is accessed only once and no reading occurs during matrix calculation
				if (ii2_molecules_base + v < tmp_molecule_id)
				{
					sim_matrix_[strictUpperTriangularMatrixToArray(ii2_molecules_base + v, tmp_molecule_id)] = coeff;
				}
				else
				{
					sim_matrix_[strictUpperTriangularMatrixToArray(tmp_molecule_id, ii2_molecules_base + v)] = coeff;
				}
			}
		}
	}
	// When comparing identical InvertedIndices ( i==j ) only inter-InvertedIndex pairs of different molecules should be considered
	// Therefore the inner loop variable is everytime set to ( outer-loop-variable + 1 )
	else
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_molecule_id = ii1_molecules_base + u;
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=u+1; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
				// CONCURRENT WRITE ACCESS ON SIM_MATRIX ARRAY WITHOUT MUTEX LOCK
				// This is valid because every array field is accessed only once and no reading occurs during matrix calculation
				if (ii2_molecules_base + v < tmp_molecule_id)
				{
					sim_matrix_[strictUpperTriangularMatrixToArray(ii2_molecules_base + v, tmp_molecule_id)] = coeff;
				}
				else
				{
					sim_matrix_[strictUpperTriangularMatrixToArray(tmp_molecule_id, ii2_molecules_base + v)] = coeff;
				}
			}
		}
	}
}


void BinaryFingerprintMethods::pairwiseSimilaritiesConnectedComponents(const unsigned int ii1_index, const unsigned int ii2_index, ThreadData* t_data)
{
	unsigned short* cc_matrix_row;
	unsigned short** cc_matrix = t_data->cc_matrix;
	
	float* nn_sim = t_data->float_array;
	unsigned int* nn_list = t_data->uint_array;
	LongSize edge_count = t_data->first;
	
	float c, coeff;
	unsigned int tmp_molecule_id;
	unsigned short tmp_n_features;
	
	InvertedIndex *ii1 = lib_iindices_[ii1_index];
	InvertedIndex *ii2 = lib_iindices_[ii2_index];
	
	unsigned int ii2_n_molecules = ii2->n_molecules;
	unsigned short* ii1_n_features = ii1->n_features;
	unsigned short* ii2_n_features = ii2->n_features;
	
	unsigned int ii1_molecules_base = ii1_index * blocksize_;
	unsigned int ii2_molecules_base = ii2_index * blocksize_;
	
	if (ii1!=ii2)
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_molecule_id = ii1_molecules_base + u;
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=0; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
// 				cerr << tmp_molecule_id << " " << ii2_molecules_base + v << " - " << coeff << endl;
				
				if (coeff >= cutoff_)
				{
					if (store_nns_)
					{
						if (checkSimilaritySwitch(coeff, nn_sim[tmp_molecule_id], ii2_molecules_base + v,  nn_list[tmp_molecule_id]))
						{
							nn_sim[tmp_molecule_id] = coeff;
							nn_list[tmp_molecule_id] = ii2_molecules_base + v;
						}
						if (checkSimilaritySwitch(coeff, nn_sim[ii2_molecules_base + v], tmp_molecule_id, nn_list[ii2_molecules_base + v]))
						{
							nn_sim[ii2_molecules_base + v] = coeff;
							nn_list[ii2_molecules_base + v] = tmp_molecule_id;
						}
					}
					
					++edge_count;
					
					boost::mutex::scoped_lock lock(out_mutex_);
					ds->union_set(tmp_molecule_id, ii2_molecules_base + v);
				}
			}
		}
	}
	// When comparing identical InvertedIndices ( i==j ) only inter-InvertedIndex pairs of different molecules should be considered
	// Therefore the inner loop variable is everytime set to ( outer-loop-variable + 1 )
	else
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_molecule_id = ii1_molecules_base + u;
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=u+1; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
// 				cerr << tmp_molecule_id << " " << ii2_molecules_base + v << " - " << coeff << endl;
				
				if (coeff >= cutoff_)
				{
					if (store_nns_)
					{
						if (checkSimilaritySwitch(coeff, nn_sim[tmp_molecule_id], ii2_molecules_base + v,  nn_list[tmp_molecule_id]))
						{
							nn_sim[tmp_molecule_id] = coeff;
							nn_list[tmp_molecule_id] = ii2_molecules_base + v;
						}
						if (checkSimilaritySwitch(coeff, nn_sim[ii2_molecules_base + v], tmp_molecule_id, nn_list[ii2_molecules_base + v]))
						{
							nn_sim[ii2_molecules_base + v] = coeff;
							nn_list[ii2_molecules_base + v] = tmp_molecule_id;
						}
					}
					
					++edge_count;
					
					boost::mutex::scoped_lock lock(out_mutex_);
					ds->union_set(tmp_molecule_id, ii2_molecules_base + v);
				}
			}
		}
	}
}


void BinaryFingerprintMethods::pairwiseSimilaritiesMedoids(const unsigned int ii1_index, const unsigned int ii2_index, ThreadData* t_data)
{
	unsigned short* cc_matrix_row;
	unsigned short** cc_matrix = t_data->cc_matrix;
	
	float c, coeff;
	unsigned int tmp_molecule_id;
	unsigned short tmp_n_features;
	
	InvertedIndex *ii1 = lib_iindices_[ii1_index];
	InvertedIndex *ii2 = lib_iindices_[ii2_index];
	
	unsigned int ii2_n_molecules = ii2->n_molecules;
	unsigned short* ii1_n_features = ii1->n_features;
	unsigned short* ii2_n_features = ii2->n_features;
	
	unsigned int ii1_molecules_base = ii1_index * blocksize_;
	unsigned int ii2_molecules_base = ii2_index * blocksize_;
	
	if (ii1!=ii2)
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_molecule_id = ii1_molecules_base + u;
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=0; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
				boost::mutex::scoped_lock lock(out_mutex_);
				dprec_sim_matrix_[ii2_molecules_base + v] += coeff;
				dprec_sim_matrix_[tmp_molecule_id] += coeff;
			}
		}
	}
	// When comparing identical InvertedIndices ( i==j ) only inter-InvertedIndex pairs of different molecules should be considered
	// Therefore the inner loop variable is everytime set to ( outer-loop-variable + 1 )
	else
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_molecule_id = ii1_molecules_base + u;
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=u+1; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
				boost::mutex::scoped_lock lock(out_mutex_);
				dprec_sim_matrix_[ii2_molecules_base + v] += coeff;
				dprec_sim_matrix_[tmp_molecule_id] += coeff;
			}
		}
	}
}


void BinaryFingerprintMethods::pairwiseSimilaritiesThread(const unsigned int thread_id)
{
	ThreadData *t_data = &thread_data_[thread_id];
	unsigned short** cc_matrix = t_data->cc_matrix;
	
	LongSize index;
	LongSize row_index, col_index;
	
	unsigned int count = 0;
	while (getNextComparisonIndex(index))
	{
		// Reset common counts matrix to 0
		for (unsigned short m=0; m<=blocksize_; ++m)
		{
			memset(cc_matrix[m], '\0', cc_matrix_size_);
		}
		
		if (thread_id == 0)
		{
			if (count == 100 && verbosity_ > 0)
			{
				double perc = 100.0 * double((n_comparisons_backup_ - index) /  double(n_comparisons_backup_));
				
				cerr << "\r++ Status: " << perc << " %          ";
				count = 0;
			}
			
			++count;
		}
		
		// Calculate indices of next InvertedIndex pair to compare
		arrayToUpperTriangluarMatrix(row_index, col_index, index);
		
		// Calculate common counts
		calculateCommonCounts_M_N(lib_iindices_[row_index], lib_iindices_[col_index], cc_matrix);
		
		// Calculate similarities
		(this->*pairwiseSimilaritiesBase)(row_index, col_index, t_data);
	}
	
	if (thread_id == 0 && verbosity_ > 0)
	{
		cerr << endl;
	}
}


bool BinaryFingerprintMethods::pairwiseSimilarities(const vector<unsigned int>& selection, vector<pair<unsigned int, float> >& nn_data)
{
	if (verbosity_ > 5)
	{
		Log << "++ Setting up data structures" << endl;
	}
	
	LongSize n_molecules = selection.size();
	
	// Create inverted indices for library and query molecules
	//vector<pair<unsigned short*, unsigned int> > tmp;
	vector<pair<const vector<unsigned short>*, unsigned int> > tmp;
	if (lib_features_ != NULL)
	{
		//for (unsigned int i=0; i!=lib_features_->size(); ++i)
		for (unsigned int i=0; i!=n_molecules; ++i)
		{
// 			tmp.push_back(make_pair((*lib_features_)[selection[i]], 0));
			tmp.push_back(make_pair(&(*lib_features_)[selection[i]], 0));
		}
		createInvertedIndices(tmp, lib_iindices_);
	}
	else
	{
		Log.error() << "-- FAILED: no library molecules defined." << endl;
		Log.error() << "--" << endl;
		
		return false;
	}
	
	
	LongSize n_iids = lib_iindices_.size();
	n_comparisons_ = (n_iids * n_iids + n_iids) / 2;
	n_comparisons_backup_ = n_comparisons_;
	
	unsigned int n_threads = n_threads_;
	if (n_comparisons_ < n_threads_)
	{
		n_threads = n_comparisons_;
	}
	
	if (threads_ == NULL)
	{
		if (store_nns_)
		{
			createThreadData(blocksize_, n_molecules, 0);
			
			for (unsigned int i=0; i!=n_threads; ++i)
			{
				for (unsigned int j=0; j!=n_molecules; ++j)
				{
					thread_data_[i].uint_array[j] = n_molecules + 1;
					thread_data_[i].float_array[j] = -1.0;
				}
			}
		}
		else
		{
			createThreadData(blocksize_, 0, 0);
		}
	}
	
	if (verbosity_ > 5)
	{
		Log << "++ Start similarity calculations" << endl;
	}
	
	Timer* timer = new Timer();
	timer->start();
	
	for (unsigned int i=0; i!=n_threads; ++i)
	{
		threads_[i] = thread(boost::bind(&BinaryFingerprintMethods::pairwiseSimilaritiesThread, this, i));
	}
	
	if (verbosity_ > 5)
	{
		Log << "++ Calculations running" << endl;
	}
	
	for (unsigned int i=0; i!=n_threads; ++i)
	{
		threads_[i].join();
	}
	
	timer->stop();
	
	if (verbosity_ > 5)
	{
		LongSize total_comp = ((n_molecules * n_molecules) - n_molecules) / 2;
		LongSize comp_per_sec = (total_comp / timer->getClockTime());
		
		Log << "++ Calculations done ++" << endl;
		Log << "++" << endl;
		Log << "++ Comparisons :  " << total_comp << endl;
		Log << "++ Elapsed time:  " << timer->getClockTime() / 3600 << " hours" << endl;
		Log << "++ Comps / sec:   " << comp_per_sec << endl;
		Log << "++" << endl;
	}
	
	
	delete timer;
	
	if (store_nns_)
	{
		for (unsigned int i=0; i!=n_molecules; ++i)
		{
			for (unsigned int j=1; j!=n_threads; ++j)
			{
				if (thread_data_[j].float_array[i] >= thread_data_[0].float_array[i])
				{
					if (checkSimilaritySwitch(thread_data_[j].float_array[i], thread_data_[0].float_array[i], 
						thread_data_[j].uint_array[i],  thread_data_[0].uint_array[i]))
					{
						thread_data_[0].uint_array[i] = thread_data_[j].uint_array[i];
						thread_data_[0].float_array[i] = thread_data_[j].float_array[i];
					}
				}
			}
		}
		
		nn_data.resize(n_molecules);
		for (unsigned int i=0; i!=n_molecules; ++i)
		{
			if (thread_data_[0].float_array[i] == -1)
			{
				nn_data[i] = make_pair(i, 1.0);
			}
			else
			{
				nn_data[i] = make_pair(thread_data_[0].uint_array[i], thread_data_[0].float_array[i]);
			}
		}
	}
	
	destroyThreadData();
	destroyInvertedIndices(lib_iindices_);
	
	return true;
}


bool BinaryFingerprintMethods::checkInputData(const vector<unsigned int>& selection) const
{
	if (lib_features_ == NULL)
	{
		Log.error() << "-- FAILED: No library features specified for calculation." << endl;
		Log.error() << "--" << endl;
		
		return false;
	}
	
	if (selection.size() == 0)
	{
		Log.error() << "-- FAILED: No molecule indices specified for calculation." << endl;
		Log.error() << "--" << endl;
		
		return false;
	}
	else
	{
		if (selection.size() > lib_features_->size())
		{
			Log.error() << "-- FAILED: More molecules specified as contained in the input library." << endl;
			Log.error() << "--" << endl;
			
			return false;
		}
		else
		{
			unsigned int max_index = 0;
			for (unsigned int i=0; i!=selection.size(); ++i)
			{
				if (selection[i] > max_index)
				{
					max_index = selection[i];
				}
			}
			
			if (max_index > lib_features_->size() - 1)
			{
				Log.error() << "-- FAILED: Highest index in specified molcule indices exeeds input library size." << endl;
				Log.error() << "--" << endl;
				
				return false;
			}
		}
	}
	
	return true;
}


bool BinaryFingerprintMethods::connectedComponents(const vector<unsigned int>& selection, 
						   vector<vector<unsigned int> >& ccs, 
						   vector<vector<pair<unsigned int, float> > >& nn_data, 
						   const float cutoff, 
						   const bool store_nns)
{
	ccs.clear();
	nn_data.clear();
	
	if (checkInputData(selection) == false)
	{
		return false;
	}
	
	cutoff_ = cutoff;
	store_nns_ = store_nns;
	
	// Create similarity graph and disjoint set for connected components calculation
	SimilarityGraph sim_graph(selection.size());
	rank.resize(selection.size());
	parent.resize(selection.size());
	ds = new DisjointSet(&rank[0], &parent[0]);
	initialize_incremental_components(sim_graph, *ds);
	incremental_components(sim_graph, *ds);
	
	// Map to store nearest neighbour information
	vector<pair<unsigned int, float> > nn_data_tmp;
	
	// Set appropriate function pointer for connected components calculation
	pairwiseSimilaritiesBase = &BinaryFingerprintMethods::pairwiseSimilaritiesConnectedComponents;
	
	// Run pairwise similarity calculation
	bool success = pairwiseSimilarities(selection, nn_data_tmp);
	
	if (success)
	{
		
		// STEP 1: Get connected components and member indices
		boost::unordered_map<unsigned int, boost::unordered_map<unsigned int, unsigned int> > ccs_tmp;
		boost::unordered_map<unsigned int, boost::unordered_map<unsigned int, unsigned int> >::iterator ccs_iter;
		BOOST_FOREACH(Vertex current_vertex, vertices(sim_graph))
		{
			if (ccs_tmp.find(ds->find_set(current_vertex)) == ccs_tmp.end())
			{
				ccs_tmp[ds->find_set(current_vertex)] = boost::unordered_map<unsigned int, unsigned int>();
			}
			
// 			ccs_tmp[ds->find_set(current_vertex)][current_vertex] = ccs_tmp[ds->find_set(current_vertex)].size();
			ccs_tmp[ds->find_set(current_vertex)].insert(make_pair(current_vertex, 0));
			ccs_tmp[ds->find_set(current_vertex)][current_vertex] = ccs_tmp[ds->find_set(current_vertex)].size();
		}
		
		
		// STEP 2: Get sizes of connected components
		multimap<unsigned int, unsigned int> cc_sizes;
		multimap<unsigned int, unsigned int>::iterator size_iter;
		for (ccs_iter=ccs_tmp.begin(); ccs_iter!=ccs_tmp.end(); ++ccs_iter)
		{
			cc_sizes.insert(make_pair(ccs_iter->second.size(), ccs_iter->first));
		}
		
		// STEP 3: Write information in return data structures
		boost::unordered_map<unsigned int, unsigned int>::iterator cc_iter;
		for (size_iter=cc_sizes.begin(); size_iter!=cc_sizes.end(); ++size_iter)
		{
			ccs.push_back(vector<unsigned int>(size_iter->first, 0));
			
			if (store_nns_)
			{
				nn_data.push_back(vector<pair<unsigned int, float> >(size_iter->first, make_pair(0, 0.0)));
				for (cc_iter=ccs_tmp[size_iter->second].begin(); cc_iter!=ccs_tmp[size_iter->second].end(); ++cc_iter)
				{
					ccs[ccs.size() - 1][cc_iter->second - 1] = cc_iter->first;
					nn_data[ccs.size() - 1][cc_iter->second - 1] = make_pair(ccs_tmp[size_iter->second][nn_data_tmp[cc_iter->first].first] - 1, nn_data_tmp[cc_iter->first].second);
				}
			}
			else
			{
				for (cc_iter=ccs_tmp[size_iter->second].begin(); cc_iter!=ccs_tmp[size_iter->second].end(); ++cc_iter)
				{
					ccs[ccs.size() - 1][cc_iter->second - 1] = cc_iter->first;
				}
			}
		}
		
		ccs_tmp.clear();
		cc_sizes.clear();
		nn_data_tmp.clear();
	}
	
	sim_graph.clear();
	rank.clear();
	parent.clear();
	
	delete ds;
	
	return success;
}


void BinaryFingerprintMethods::cutoffSearchSimilarities(const unsigned int query_index, const unsigned int lib_index, unsigned short** cc_matrix, File& outfile)
{
	unsigned short* cc_matrix_row;
	
	float c, coeff;
	unsigned int tmp_molecule_id;
	unsigned short tmp_n_features;
	
	InvertedIndex *ii1 = query_iindices_[query_index];
	InvertedIndex *ii2 = lib_iindices_[lib_index];
	
	unsigned int ii2_n_molecules = ii2->n_molecules;
	unsigned short* ii1_n_features = ii1->n_features;
	unsigned short* ii2_n_features = ii2->n_features;
	
	unsigned int ii1_molecules_base = query_index * blocksize_;
	unsigned int ii2_molecules_base = lib_index * blocksize_;
	
	// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
	for (unsigned int u=0; u!=ii1->n_molecules; ++u)
	{
		cc_matrix_row = cc_matrix[u+1];
		tmp_molecule_id = ii1_molecules_base + u;
		tmp_n_features = ii1_n_features[u];
		
		for (unsigned int v=0; v!=ii2_n_molecules; ++v)
		{
			// Shared feature count
			c = cc_matrix_row[v+1];
			
			// Calculate Tanimoto similarity as coeff = c / (a + b -c )
			coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
			
// 			cout << c << " " << tmp_n_features << " " << ii2_n_features[v] << " " << coeff << endl;
			
			if (coeff >= cutoff_)
			{
				outfile << tmp_molecule_id << " " << ii2_molecules_base + v << " " << coeff << endl;
// 				cout << tmp_molecule_id << " " << ii2_molecules_base + v << " " << coeff << endl;
			}
		}
	}
}


void BinaryFingerprintMethods::cutoffSearchThread(const unsigned int thread_id)
{
	ThreadData *t_data = &thread_data_[thread_id];
	unsigned short** cc_matrix = t_data->cc_matrix;
	File outfile(t_data->outfile_name, File::MODE_OUT);
	
	for (unsigned int i=t_data->first; i<=t_data->last; ++i)
	{
		for (unsigned int j=0; j!=query_iindices_.size(); ++j)
		{
			// Reset common counts matrix to 0
			for (unsigned short m=0; m != blocksize_ + 1; ++m)
			{
				memset(cc_matrix[m], '\0', cc_matrix_size_);
			}
			
			// Calculate common counts
			calculateCommonCounts_M_N(query_iindices_[j], lib_iindices_[i], cc_matrix);
			
			// Calculate similarities
			cutoffSearchSimilarities(j, i, cc_matrix, outfile);
		}
	}
	
	outfile.close();
}


bool BinaryFingerprintMethods::cutoffSearch(const float cutoff, const String& outfile_name)
{
	cutoff_ = cutoff;
	
	if (verbosity_ > 5)
	{
		Log << "++ Setting up data structures for input libraries" << endl;
	}
	
	if (File::isAccessible(outfile_name))
	{
		Log.error() << "-- FAILED: Specified output file already exists: " << outfile_name << endl;
		Log.error() << "--" << endl;
		
		return false;
	}
	
	// Create inverted indices for library and query molecules
	vector<pair<const vector<unsigned short>*, unsigned int> > tmp;
	if (lib_features_ != NULL)
	{
		for (unsigned int i=0; i!=lib_features_->size(); ++i)
		{
// 			tmp.push_back(make_pair((*lib_features_)[i], 0));
			tmp.push_back(make_pair(&(*lib_features_)[i], 0));
		}
		createInvertedIndices(tmp, lib_iindices_);
	}
	else
	{
		Log.error() << "-- FAILED: No library molecules defined." << endl;
		Log.error() << "--" << endl;
		
		return false;
	}
	
	if (query_features_ != NULL)
	{
		
		tmp.clear();
		for (unsigned int i=0; i!=query_features_->size(); ++i)
		{
// 			tmp.push_back(make_pair((*query_features_)[i], 0));
			tmp.push_back(make_pair(&(*query_features_)[i], 0));
		}
		createInvertedIndices(tmp, query_iindices_);
	}
	else
	{
		Log.error() << "-- FAILED: No query molecules defined." << endl;
		Log.error() << "--" << endl;
		
		return false;
	}
	
	vector<String> tmp_file_names;
	
	if (threads_ == NULL)
	{
		createThreadData(blocksize_, 0, 0);
		
		if (n_threads_ > 1)
		{
			String tmp_file_name;
			
			for (unsigned int i=0; i!=n_threads_; ++i)
			{
				File::createTemporaryFilename(tmp_file_name);
				tmp_file_names.push_back(tmp_file_name);
				
				thread_data_[i].outfile_name = tmp_file_name;
			}
		}
		else
		{
			thread_data_[0].outfile_name = outfile_name;
		}
	}
	
	if (verbosity_ > 5)
	{
		Log << "++ Start similarity calculations" << endl;
	}
	
	LongSize n_comps = lib_iindices_.size();
	LongSize batch_size = n_comps / n_threads_;
	LongSize to_increment = n_comps - (batch_size * n_threads_);
	
	Timer* timer = new Timer();
	timer->start();
	
	unsigned int current_index = 0;
	for (unsigned int i=0; i!=n_threads_; ++i)
	{
		// Set lib_iindices start index for thread i
		thread_data_[i].first = current_index;

		// Increment index for lib_iindides depending on thread number
		if (i < to_increment)
		{
			current_index += batch_size;
		}
		else
		{
			current_index += batch_size - 1;
		}

		// Set lib_iindices end index for thread i
		thread_data_[i].last = current_index;

		// Increment index for lib_iindides
		++current_index;
		
		threads_[i] = thread(boost::bind(&BinaryFingerprintMethods::cutoffSearchThread, this, i));
	}
	
	if (verbosity_ > 5)
	{
		Log << "++ Calculations running" << endl;
	}
	
	for (unsigned int i=0; i!=n_threads_; ++i)
	{
		threads_[i].join();
	}
	
	timer->stop();
	
	if (verbosity_ > 5)
	{
		LongSize total_comp = query_features_->size() * lib_features_->size();
		LongSize comp_per_sec = (total_comp / timer->getClockTime());
		
		Log << "++ Calculations done" << endl;
		Log << "++" << endl;
		Log << "++ Comparisons :  " << total_comp << endl;
		Log << "++ Elapsed time:  " << timer->getClockTime() << " seconds" << endl;
		Log << "++ Comps / sec:   " << comp_per_sec << endl;
		Log << "++" << endl;
	}
	
	// Combine results into a single output file
	if (n_threads_ > 1)
	{
		string line;
		File result_file(outfile_name, File::MODE_OUT);
		
		for (unsigned int i=0; i!=tmp_file_names.size(); ++i)
		{
			File tmp_result(tmp_file_names[i], File::MODE_IN);
			while (getline(tmp_result, line))
			{
				result_file << line << endl;
			}
			tmp_result.close();
			File::remove(tmp_file_names[i]);
		}
		
		result_file.close();
	}
	
	delete timer;
	
	destroyThreadData();
	destroyInvertedIndices(query_iindices_);
	destroyInvertedIndices(lib_iindices_);
	
	return true;
}


bool BinaryFingerprintMethods::calculateSelectionMedoid(const vector<unsigned int>& selection, unsigned int& medoid_index, vector<float>& avg_sims)
{
	store_nns_ = false;
	avg_sims.clear();
	medoid_index = selection.size() + 1;
	LongSize n_molecules = selection.size();
	
	if (checkInputData(selection) == false)
	{
		return false;
	}
	
	if (n_molecules == 1)
	{
		medoid_index = 0;
		avg_sims.push_back(1.0);
		
		return true;
	}
	
	// Allocate matrix to store similarity sum for every molecule
	dprec_sim_matrix_ = new double[n_molecules];
	memset(dprec_sim_matrix_, 0.0, sizeof(double)*n_molecules);
	
	// Set appropriate function pointer for connected components calculation
	pairwiseSimilaritiesBase = &BinaryFingerprintMethods::pairwiseSimilaritiesMedoids;
	
	// Run pairwise similarity calculation
	vector<pair<unsigned int, float> > nn_data_tmp;
	bool success = pairwiseSimilarities(selection, nn_data_tmp);
	
	if (success)
	{
		float avg_sim_max = dprec_sim_matrix_[0] /= (n_molecules - 1);;
		
		medoid_index = 0;
		unsigned int medoid_id = selection[0];
		
		avg_sims.push_back(dprec_sim_matrix_[0]);
		for (unsigned int i=1; i!=n_molecules; ++i)
		{
			dprec_sim_matrix_[i] /= (n_molecules - 1);
			avg_sims.push_back(dprec_sim_matrix_[i]);
			
			if (checkSimilaritySwitch(dprec_sim_matrix_[i], avg_sim_max, selection[i], medoid_id))
			{
				avg_sim_max = dprec_sim_matrix_[i];
				medoid_index = i;
			}
		}
	}
	
	delete [] dprec_sim_matrix_;
	
	return success;
}


bool BinaryFingerprintMethods::averageLinkageClustering(const vector<unsigned int>& selection, 
							vector<pair<unsigned int, float> >& nn_data,
							map<unsigned int, vector<unsigned int> >& cluster_selection)
{
	cluster_selection.clear();
	LongSize n_molecules = selection.size();
	
	if (n_molecules == 1)
	{
		cluster_selection.insert(make_pair(1, vector<unsigned int>(1, 0)));
		
		return true;
	}
	
	if (checkInputData(selection) == false)
	{
		return false;
	}
	
	// Select initial clustering method to use
	if (n_molecules < max_clusters_)
	{
		clustering_method_ = STORED_MATRIX;
	}
	else
	{
		clustering_method_ = STORED_DATA_PARALLEL;
	}
	
	clustering_method_ = STORED_DATA_PARALLEL;
	
	// Pointer to final root cluster
	Cluster* root = NULL;
	Cluster *cluster;
	n_clusters_ = 0;
	
	vec_actives_.clear();
	leaf_clusters_.clear();
	
	if (clustering_method_ == STORED_DATA_PARALLEL)
	{
		// ----------------------------------------------------------------------
		// Stored data average linkage using RNN parallel algorithm
		// ----------------------------------------------------------------------
		if (verbosity_ >= 10)
		{
			Log << "++ RNN parallel method (stored data)" << endl;
		}
		
		for (unsigned int i=0; i!=n_molecules; ++i)
		{
			// Create new singleton cluster
			cluster = createCluster();
			
			cluster->nn = NULL;
			cluster->is_rnn = false;
// 			cluster->leaf_members.push_back((*lib_features_)[selection[i]]);
			cluster->leaf_members.push_back(&(*lib_features_)[selection[i]]);
			
			vec_actives_.push_back(cluster);
			leaf_clusters_.push_back(cluster);
		}
		
		// Set active InvertedIndices size
		active_iids_size_ = 100;
		
		if (nn_data.empty())
		{
			// Nearest neighbours have not been calculated yet.
			store_nns_ = true;
			
			// Set appropriate function pointer for calculation of pairwise nearest neighbours
			pairwiseSimilaritiesBase = &BinaryFingerprintMethods::pairwiseSimilaritiesNearestNeighbours;
			
			// Run pairwise similarity calculation
			pairwiseSimilarities(selection, nn_data);
		}
		
		for (unsigned int i=0; i!=n_molecules; ++i)
		{
// 			cerr << i << " " << nn_data[i].first << " " << nn_data[i].second << endl;
			vec_actives_[i]->nn = vec_actives_[nn_data[i].first];
			vec_actives_[i]->predecessor_sim = nn_data[i].second;
		}
		
		nn_data.clear();
		
		// Run RNN Parallel average linkage 
		averageLinkageParallel(root);
	}
	else
	{
		// ----------------------------------------------------------------------
		// Stored matrix average linkage using NNChain algorithm
		// ----------------------------------------------------------------------
		if (verbosity_ >= 10)
		{
			Log << "++ NNChain method (stored matrix)" << endl;
		}
		
		// NN Chain from scratch. First calculate pairwise similarity matrix.
		// NN data passed to this method could be used to join nearest neighbours but is not implemented so far. Presumably the latter would anyway be slower.
		
		for (unsigned int i=0; i!=n_molecules; ++i)
		{
			// Create new singleton cluster
			cluster = createCluster();
			
			vec_actives_.push_back(cluster);
			leaf_clusters_.push_back(cluster);
		}
		
		store_nns_ = false;
		
		sim_matrix_ = new float[((n_molecules*n_molecules - n_molecules) / 2) + 1];
		sim_matrix_[((n_molecules*n_molecules - n_molecules) / 2)] = -1.0;
		
		pairwiseSimilaritiesBase = &BinaryFingerprintMethods::pairwiseSimilaritiesStoredMatrix;
		
		pairwiseSimilarities(selection, nn_data);
		
		NNChainCore(root);
	}
	
	clusterSelectionKGS(cluster_selection);
	
	finalizeClustering();
	
	return true;
}


void BinaryFingerprintMethods::similarityUpdateAverageLinkageThread(const unsigned int thread_id, const Cluster* merged_cluster)
{
	ThreadData *t_data = &thread_data_[thread_id];
	
	for (unsigned int i= t_data->first; i!=t_data->last; ++i)
	{
		if (merged_cluster != vec_actives_[i])
		{
			similarityUpdateAverageLinkage(merged_cluster, vec_actives_[i]);
		}
	}
}


void BinaryFingerprintMethods::similarityUpdateAverageLinkage(const Cluster* merge_cluster, const Cluster* current_c)
{
	unsigned int current_index = current_c->c_index;
	
	double lc_scale = (double)merge_cluster->l_child->c_size / merge_cluster->c_size;
	double rc_scale = (double)merge_cluster->r_child->c_size / merge_cluster->c_size;
	LongSize lc_index = merge_cluster->l_child->c_index;
	LongSize rc_index = merge_cluster->r_child->c_index;
	
	if (current_index < rc_index)
	{
		if (current_index < lc_index)
		{
			sim_matrix_[strictUpperTriangularMatrixToArray(current_index, lc_index)] *= lc_scale;
			
			sim_matrix_[strictUpperTriangularMatrixToArray(current_index, lc_index)] += 
			sim_matrix_[strictUpperTriangularMatrixToArray(current_index, rc_index)] * rc_scale;
		}
		else
		{
			sim_matrix_[strictUpperTriangularMatrixToArray(lc_index, current_index)] *=  lc_scale;
			
			sim_matrix_[strictUpperTriangularMatrixToArray(lc_index, current_index)] += 
			sim_matrix_[strictUpperTriangularMatrixToArray(current_index, rc_index)] * rc_scale;
		}
	}
	else
	{
		if (current_index < lc_index)
		{
			sim_matrix_[strictUpperTriangularMatrixToArray(current_index, lc_index)] *= lc_scale;
			
			sim_matrix_[strictUpperTriangularMatrixToArray(current_index, lc_index)] += 
			sim_matrix_[strictUpperTriangularMatrixToArray(rc_index, current_index)] * rc_scale;
		}
		else
		{
			sim_matrix_[strictUpperTriangularMatrixToArray(lc_index, current_index)] *=  lc_scale;
			
			sim_matrix_[strictUpperTriangularMatrixToArray(lc_index, current_index)] += 
			sim_matrix_[strictUpperTriangularMatrixToArray(rc_index, current_index)] * rc_scale;
		}
	}
}


void BinaryFingerprintMethods::clusterSimilaritySum_1_N(const InvertedIndex* ii1, const InvertedIndex* ii2, const unsigned short* cc_row, double& sim_sum)
{
	float c;
	unsigned short* ii2_n_features = ii2->n_features;
	unsigned short tmp_n_features = ii1->n_features[0];
	
	for (unsigned int v=0; v!=ii2->n_molecules; ++v)
	{
		// Shared feature count
		c = cc_row[v+1];
		
		// Calculate Tanimoto similarity as coeff = c / (a + b -c )
		sim_sum += c / ( (tmp_n_features + ii2_n_features[v]) - c );
	}
}


void BinaryFingerprintMethods::clusterSimilaritySum_M_N(const InvertedIndex* ii1, const InvertedIndex* ii2, unsigned short** cc_matrix, double& sim_sum)
{
	float c;
	unsigned short* cc_matrix_row;
	
	unsigned short tmp_n_features;
	unsigned int iids_2_size = ii2->n_molecules;
	
	unsigned short* ii1_n_features = ii1->n_features;
	unsigned short* ii2_n_features = ii2->n_features;
	
	// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
	for (unsigned int u=0; u!=ii1->n_molecules; ++u)
	{
		cc_matrix_row = cc_matrix[u+1];
		tmp_n_features = ii1_n_features[u];
		
		for (unsigned int v=0; v!=iids_2_size; ++v)
		{
			// Shared feature count
			c = cc_matrix_row[v+1];
			
			// Calculate Tanimoto similarity as coeff = c / (a + b -c )
			sim_sum += c / ( (tmp_n_features + ii2_n_features[v]) - c );
		}
	}
}


void BinaryFingerprintMethods::similarityMatrixFromClustersThread(const unsigned int thread_id)
{
	ThreadData *t_data = &thread_data_[thread_id];
	unsigned short* cc_row = t_data->cc_row;
	unsigned short** cc_matrix = t_data->cc_matrix;
	
	Cluster *c1, *c2;
	double tmp_sum, scale;
	unsigned short c;
	unsigned int ii1_n_molecules;
	
	LongSize index;
	LongSize row_index;
	LongSize col_index;
	while(getNextComparisonIndex(index))
	{
		arrayToStrictUpperTriangularMatrix(row_index, col_index, index);
		
		c1 = vec_actives_[row_index];
		c2 = vec_actives_[col_index];
		
		tmp_sum = 0.0;
		
		BOOST_FOREACH(InvertedIndex *ii1, c1->c_members)
		{
			ii1_n_molecules = ii1->n_molecules;
			
			if (ii1_n_molecules == 1)
			{
				// Iterate over all involved InvertedIndices from NNChainTip cluster
				BOOST_FOREACH(InvertedIndex *ii2, c2->c_members)
				{
					if (ii2->n_molecules == 1)
					{
						c = 0;
						calculateCommonCounts_1_1(ii1->feature_skip_list, ii2->feature_skip_list, c);
						tmp_sum += (float)c / ( (ii1->n_features[0] + ii2->n_features[0]) - c );
					}
					else
					{
						memset(cc_row, '\0', sizeof(unsigned short) * (ii2->n_molecules + 1));
						calculateCommonCounts_1_N(ii1->feature_skip_list, ii2->feature_skip_list, cc_row);
						clusterSimilaritySum_1_N(ii1, ii2, cc_row, tmp_sum);
					}
				}
			}
			else
			{
				// Iterate over all involved InvertedIndices from NNChainTip cluster
				BOOST_FOREACH(InvertedIndex* ii2, c2->c_members)
				{
					if (ii2->n_molecules == 1)
					{
						memset(cc_row, '\0', sizeof(unsigned short) * (ii1->n_molecules + 1));
						calculateCommonCounts_1_N(ii2->feature_skip_list, ii1->feature_skip_list, cc_row);
						clusterSimilaritySum_1_N(ii2, ii1, cc_row, tmp_sum);
					}
					else
					{
						for (unsigned int i=1; i<=ii1_n_molecules; ++i)
						{
							memset(cc_matrix[i], '\0', sizeof(unsigned short) * (ii2->n_molecules + 1));
						}
						
						calculateCommonCounts_M_N(ii1, ii2, cc_matrix);
						clusterSimilaritySum_M_N(ii1, ii2, cc_matrix, tmp_sum);
					}
				}
			}
		}
		
		scale = 1.0 / (c1->c_size * c2->c_size);
		
		// CONCURRENT WRITE ACCESS ON SIM_MATRIX ARRAY WITHOUT MUTEX LOCK
		// This is valid because every array field is accessed only once and no reading occurs during matrix calculation
		if (c1->c_index < c2->c_index)
		{
// 			cerr << strictUpperTriangularMatrixToArray(c1->c_index, c2->c_index) << " " << scale * tmp_sum << endl;
			sim_matrix_[strictUpperTriangularMatrixToArray(c1->c_index, c2->c_index)] = scale * tmp_sum;
		}
		else
		{
// 			cerr << strictUpperTriangularMatrixToArray(c2->c_index, c1->c_index) << " " << scale * tmp_sum << endl;
			sim_matrix_[strictUpperTriangularMatrixToArray(c2->c_index, c1->c_index)] = scale * tmp_sum;
		}
	}
}


void BinaryFingerprintMethods::calculateParallelSimilaritiesActives(const InvertedIndex* ii1, const InvertedIndex* ii2, unsigned short** cc_matrix, double** sim_matrix)
{
	float c, coeff;
	unsigned int tmp_p_cluster;
	unsigned short tmp_n_features;
	unsigned short* cc_matrix_row;
	
	unsigned short* ii1_n_features = ii1->n_features;
	unsigned short* ii2_n_features = ii2->n_features;
	unsigned int* ii1_p_clusters = ii1->parent_clusters;
	unsigned int* ii2_p_clusters = ii2->parent_clusters;
	
	unsigned int ii2_n_molecules = ii2->n_molecules;
	
	if (ii1 != ii2)
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_p_cluster = ii1_p_clusters[u];
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=0; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
				sim_matrix[tmp_p_cluster][ii2_p_clusters[v]] += coeff;
			}
		}
	}
	else
	{
		// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
		for (unsigned int u=0; u!=ii1->n_molecules; ++u)
		{
			cc_matrix_row = cc_matrix[u+1];
			tmp_p_cluster = ii1_p_clusters[u];
			tmp_n_features = ii1_n_features[u];
			
			for (unsigned int v=u+1; v!=ii2_n_molecules; ++v)
			{
				// Shared feature count
				c = cc_matrix_row[v+1];
				
				// Calculate Tanimoto similarity as coeff = c / (a + b -c )
				coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
				
				sim_matrix[tmp_p_cluster][ii2_p_clusters[v]] += coeff;
			}
		}
	}
}


void BinaryFingerprintMethods::calculateParallelSimilaritiesActivesThread(const unsigned int thread_id)
{
	ThreadData *t_data = &thread_data_[thread_id];
	float* current_nn_sim_s  = t_data->float_array;
	unsigned int* current_nns = t_data->uint_array;
	double** sim_matrix = t_data->dprec_matrix;
	unsigned short** cc_matrix = t_data->cc_matrix;
	
	LongSize active_1_index;
	LongSize active_2_index;
	unsigned int active_1_base;
	unsigned int active_2_base;
	
	vector<InvertedIndex*> active_1;
	vector<InvertedIndex*> active_2;
	unsigned int active_1_size;
	unsigned int active_2_size;
	
	float sim;
	double* sim_row;
	unsigned int tmp_size;
	
	LongSize index;
	while (getNextComparisonIndex(index))
	{
		arrayToUpperTriangluarMatrix(active_1_index, active_2_index, index);
		
		active_1_base = active_1_index * active_iids_size_;
		active_2_base = active_2_index * active_iids_size_;
		active_1 = active_iids_[active_1_index].first;
		active_2 = active_iids_[active_2_index].first;
		active_1_size = active_iids_[active_1_index].second;
		active_2_size = active_iids_[active_2_index].second;
		
		if (active_1_index != active_2_index)
		{
			for (unsigned int j=0; j!=active_1.size(); ++j)
			{
				for (unsigned int k=0; k!=active_2.size(); ++k)
				{
					// Reset common counts matrix to 0
					for (unsigned short m=0; m!=blocksize_ + 1; ++m)
					{
						memset(cc_matrix[m], '\0', cc_matrix_size_);
					}
					
					// Calculate common counts
					calculateCommonCounts_M_N(active_1[j], active_2[k], cc_matrix);
					
					// Calculate similarities
					calculateParallelSimilaritiesActives(active_1[j], active_2[k], cc_matrix, sim_matrix);
				}
			}
			
			for (unsigned int j=0; j!=active_1_size; ++j)
			{
				sim_row = sim_matrix[j];
				tmp_size = vec_actives_[active_1_base + j]->c_size;
				
				for (unsigned int k=0; k!=active_2_size; ++k)
				{
					sim = sim_row[k] / (tmp_size * vec_actives_[active_2_base + k]->c_size);
					
					if (checkSimilaritySwitch(sim, current_nn_sim_s[active_1_base + j], 
						vec_actives_[active_2_base + k]->c_id, vec_actives_[current_nns[active_1_base + j]]->c_id))
					{
						current_nns[active_1_base + j] = active_2_base + k;
						current_nn_sim_s[active_1_base + j] = sim;
					}
					if (checkSimilaritySwitch(sim, current_nn_sim_s[active_2_base + k], 
						vec_actives_[active_1_base + j]->c_id, vec_actives_[current_nns[active_2_base + k]]->c_id))
					{
						current_nns[active_2_base + k] = active_1_base + j;
						current_nn_sim_s[active_2_base + k] = sim;
					}
				}
			}
		}
		else
		{
			for (unsigned int j=0; j!=active_1.size(); ++j)
			{
				for (unsigned int k=j; k!=active_2.size(); ++k)
				{
					// Reset common counts matrix to 0
					for (unsigned short m=0; m!=blocksize_ + 1; ++m)
					{
						memset(cc_matrix[m], '\0', cc_matrix_size_);
					}
					
					// Calculate common counts
					calculateCommonCounts_M_N(active_1[j], active_2[k], cc_matrix);
					
					// Calculate similarities
					calculateParallelSimilaritiesActives(active_1[j], active_2[k], cc_matrix, sim_matrix);
				}
			}
			
			for (unsigned int j=0; j!=active_1_size; ++j)
			{
				sim_row = sim_matrix[j];
				tmp_size = vec_actives_[active_1_base + j]->c_size;
				
				for (unsigned int k=j+1; k!=active_2_size; ++k)
				{
					sim = sim_row[k] / (tmp_size * vec_actives_[active_2_base + k]->c_size);
					
					if (checkSimilaritySwitch(sim, current_nn_sim_s[active_1_base + j], 
						vec_actives_[active_2_base + k]->c_id, vec_actives_[current_nns[active_1_base + j]]->c_id))
					{
						current_nns[active_1_base + j] = active_2_base + k;
						current_nn_sim_s[active_1_base + j] = sim;
					}
					if (checkSimilaritySwitch(sim, current_nn_sim_s[active_2_base + k], 
						vec_actives_[active_1_base + j]->c_id, vec_actives_[current_nns[active_2_base + k]]->c_id))
					{
						current_nns[active_2_base + k] = active_1_base + j;
						current_nn_sim_s[active_2_base + k] = sim;
					}
				}
			}
		}
		
		for (unsigned int j=0; j!=active_1_size; ++j)
		{
			for (unsigned int k=0; k!=active_2_size; ++k)
			{
				sim_matrix[j][k] = 0.0;
			}
		}
	}
}


void BinaryFingerprintMethods::calculateParallelSimilarities(const InvertedIndex* ii1, const InvertedIndex* ii2, unsigned short** cc_matrix, double** sim_matrix)
{
	float c, coeff;
	unsigned int tmp_p_cluster;
	unsigned short tmp_n_features;
	unsigned short* cc_matrix_row;
	
	unsigned short* ii1_n_features = ii1->n_features;
	unsigned short* ii2_n_features = ii2->n_features;
	unsigned int* ii1_p_clusters = ii1->parent_clusters;
	unsigned int* ii2_p_clusters = ii2->parent_clusters;
	
	unsigned int ii2_n_molecules = ii2->n_molecules;
	
	// Iterate over all inter-InvertedIndex molecule pairs to calculate similarities
	for (unsigned int u=0; u!=ii1->n_molecules; ++u)
	{
		cc_matrix_row = cc_matrix[u+1];
		tmp_p_cluster = ii1_p_clusters[u];
		tmp_n_features = ii1_n_features[u];
		
		for (unsigned int v=0; v!=ii2_n_molecules; ++v)
		{
			// Shared feature count
			c = cc_matrix_row[v+1];
			
			// Calculate Tanimoto similarity as coeff = c / (a + b -c )
			coeff = c / ( (tmp_n_features + ii2_n_features[v]) - c );
			
			sim_matrix[tmp_p_cluster][ii2_p_clusters[v]] += coeff;
		}
	}
}


void BinaryFingerprintMethods::calculateParallelSimilaritiesThread(const unsigned int thread_id)
{
	ThreadData *t_data = &thread_data_[thread_id];
	double** sim_matrix = t_data->dprec_matrix;
	unsigned short** cc_matrix = t_data->cc_matrix;
	
	unsigned int base;
	unsigned int active_size;
	vector<InvertedIndex*> active;
	
	LongSize index;
	while(getNextComparisonIndex(index))
	{
		active = active_iids_[index].first;
		active_size = active_iids_[index].second;
		base = index * active_iids_size_;
		
		for (unsigned int i=0; i!=active.size(); ++i)
		{
			for (unsigned int j=0; j!=lib_iindices_.size(); ++j)
			{
				// Reset common counts matrix to 0
				for (unsigned short m=0; m!=blocksize_ + 1; ++m)
				{
					memset(cc_matrix[m], '\0', cc_matrix_size_);
				}
				
				// Calculate common counts
				calculateCommonCounts_M_N(active[i], lib_iindices_[j], cc_matrix);
				
				// Calculate similarities
				calculateParallelSimilarities(active[i], lib_iindices_[j], cc_matrix, sim_matrix);
			}
		}
		
		double* sim_row;
		float sim, current_sim;
		unsigned int tmp_size, tmp_nn = 0;
		for (unsigned int i=0; i!=active_size; ++i)
		{
			sim_row = sim_matrix[i];
			tmp_size = vec_actives_[base + i]->c_size;
			current_sim = vec_actives_[base + i]->predecessor_sim;
			
			for (unsigned int j=0; j!=vec_inactives_.size(); ++j)
			{
				sim = sim_row[j] / (vec_inactives_[j]->c_size * tmp_size);
				
				if (checkSimilaritySwitch(sim, current_sim, vec_inactives_[j]->c_id, vec_inactives_[tmp_nn]->c_id))
				{
					tmp_nn = j;
					current_sim = sim;
				}
				
				sim_matrix[i][j] = 0.0;
			}
			
			vec_actives_[base + i]->predecessor_sim = current_sim;
			vec_actives_[base + i]->nn = vec_inactives_[tmp_nn];
		}
	}
}


void BinaryFingerprintMethods::averageLinkageParallel(Cluster*& root)
{
	if (threads_==NULL)
	{
		createThreadData(blocksize_, vec_actives_.size(), active_iids_size_);
	}
	
	for (unsigned int i=0; i!=n_threads_; ++i)
	{
		for (unsigned int j=0; j!=active_iids_size_; ++j)
		{
			for (unsigned int k=0; k!=vec_actives_.size(); ++k)
			{
				thread_data_[i].dprec_matrix[j][k] = 0.0;
			}
		}
	}
	
	bool secondary_rnn;
	unsigned int rnn_pairs;
	
	Cluster* cl;
	vector<Cluster*> tmp_actives;
	//vector<pair<unsigned short*, unsigned int> > tmp;
	vector<pair<const vector<unsigned short>*, unsigned int> > tmp;
	
	while (true)
	{
		rnn_pairs = 0;
		for (unsigned int i=0; i!=vec_actives_.size(); ++i)
		{
			if (vec_actives_[i] == vec_actives_[i]->nn->nn)
			{
				// RNN pair found
				if (!vec_actives_[i]->is_rnn)
				{
					// RNN pair not yet visited
					vec_actives_[i]->is_rnn = true;
					vec_actives_[i]->nn->is_rnn = true;
					
					cl = mergeClusters(vec_actives_[i], vec_actives_[i]->nn, vec_actives_[i]->predecessor_sim);
					cl->is_rnn = false;
					cl->nn = NULL;
					cl->predecessor_sim = -1.0;
					
					tmp_actives.push_back(cl);
					
					++rnn_pairs;
				}
			}
		}
		
		secondary_rnn = true;
		while (secondary_rnn)
		{
			secondary_rnn = false;
			
			for (unsigned int i=0; i!=vec_actives_.size(); ++i)
			{
				if (!vec_actives_[i]->is_rnn && !vec_actives_[i]->nn->is_rnn)
				{
					if (fabs(vec_actives_[i]->predecessor_sim - vec_actives_[i]->nn->predecessor_sim) <= precision_)
					{
						// RNN pair found
						vec_actives_[i]->is_rnn = true;
						vec_actives_[i]->nn->is_rnn = true;
						
						cl = mergeClusters(vec_actives_[i], vec_actives_[i]->nn, vec_actives_[i]->predecessor_sim);
						cl->is_rnn = false;
						cl->nn = NULL;
						cl->predecessor_sim = -1.0;
						
						tmp_actives.push_back(cl);
						
						++rnn_pairs;
						
						secondary_rnn = true;
					}
				}
			}
		}
		
		
		for (unsigned int i=0; i!=vec_actives_.size(); ++i)
		{
			if (!vec_actives_[i]->is_rnn)
			{
				if (vec_actives_[i]->nn->is_rnn)
				{
					// No RNN but nearest neighbour is an RNN
					vec_actives_[i]->nn = NULL;
					vec_actives_[i]->predecessor_sim = -1.0;
					tmp_actives.push_back(vec_actives_[i]);
				}
				else
				{
					// Cluster has still a correct nearest neighbour
					vec_inactives_.push_back(vec_actives_[i]);
				}
			}
		}
		vec_actives_ = tmp_actives;
		tmp_actives.clear();
		
		
		if (verbosity_ >= 10)
		{
			Log << "++ " << rnn_pairs << " / " << vec_actives_.size() << " / " << vec_inactives_.size() << " / " << n_clusters_
			<< "   (RNNPairs / Actives / Inactives / Clusters)" << endl; 
		}
		
		// Finished ...
		if (vec_actives_.size() == 1 && vec_inactives_.size() == 0)
		{
			root = vec_actives_[0];
			vec_actives_.clear();
			
			return;
		}
		
		// Check if switch to stored matrix method is possible
		if (vec_actives_.size() + vec_inactives_.size() <= max_clusters_)
		{
			vec_actives_.insert(vec_actives_.end(), vec_inactives_.begin(), vec_inactives_.end());
			vec_inactives_.clear();
			
			break;
		}
		
		
		// ----------------------------------------------------------------------
		// Create InvertedIndices for active and inactive clusters
		// ----------------------------------------------------------------------
		
		// Create feature blocks for inactive clusters,
		// i.e. clusters for which the nearest neighbour is still correct
		for (unsigned int i=0; i!=vec_inactives_.size(); ++i)
		{
			for (unsigned int j=0; j!=vec_inactives_[i]->leaf_members.size(); ++j)
			{
// 				tmp.push_back(make_pair(vec_inactives_[i]->leaf_members[j], i));
				tmp.push_back(make_pair(vec_inactives_[i]->leaf_members[j], i));
			}
		}
		createInvertedIndices(tmp, lib_iindices_);
		
		// Create feature InvertedIndices from active clusters
		tmp.clear();
		unsigned int count = 0;
		for (unsigned int i=0; i!=vec_actives_.size(); ++i)
		{
			for (unsigned int j=0; j!=vec_actives_[i]->leaf_members.size(); ++j)
			{
// 				tmp.push_back(make_pair(vec_actives_[i]->leaf_members[j], count));
				tmp.push_back(make_pair(vec_actives_[i]->leaf_members[j], count));
			}
			
			++count;
			
			if ((i+1) % active_iids_size_ == 0)
			{
				active_iids_.push_back(make_pair(vector<InvertedIndex*>(), count));
				createInvertedIndices(tmp, active_iids_.rbegin()->first);
				tmp.clear();
				
				count = 0;
			}
		}
		
		if (tmp.size()!=0)
		{
			active_iids_.push_back(make_pair(vector<InvertedIndex*>(), count));
			createInvertedIndices(tmp, active_iids_.rbegin()->first);
			tmp.clear();
		}
		
		// ----------------------------------------------------------------------
		// SIMILARITY CALCULATIONS - active vs inactive clusters
		// ----------------------------------------------------------------------
		
		if (verbosity_ >= 10)
		{
			Log << "++ actives - inactives ... ";
		}
		
		if (vec_inactives_.size()!=0)
		{
			n_comparisons_ = active_iids_.size();
			
			for (unsigned int i=0; i!=n_threads_; ++i)
			{
				threads_[i] = thread(boost::bind(&BinaryFingerprintMethods::calculateParallelSimilaritiesThread, this, i));
			}
			
			for (unsigned int i=0; i!=n_threads_; ++i)
			{
				threads_[i].join();
			}
			
			// Clean up
			destroyInvertedIndices(lib_iindices_);
		}
		
		if (verbosity_ >= 10)
		{
			Log << "done" << endl;
		}
		
		// ----------------------------------------------------------------------
		// SIMILARITY CALCULATIONS - active vs active clusters
		// ----------------------------------------------------------------------
		
		if (verbosity_ >= 10)
		{
			Log << "++ actives - actives ... ";
		}
		
		n_comparisons_ = (active_iids_.size() * active_iids_.size() + active_iids_.size()) / 2;
		
		for (unsigned int i=0; i!=n_threads_; ++i)
		{
			for (unsigned int j=0; j!=vec_actives_.size(); ++j)
			{
				thread_data_[i].float_array[j] = -1.0;
				thread_data_[i].uint_array[j] = 0;
			}
			
			threads_[i] = thread(boost::bind(&BinaryFingerprintMethods::calculateParallelSimilaritiesActivesThread, this, i));
		}
		
		for (unsigned int i=0; i!=n_threads_; ++i)
		{
			threads_[i].join();
		}
		
		if (verbosity_ >= 10)
		{
			Log << "done" << endl;
		}
		
		// Clean up
		for (unsigned int i=0; i!=active_iids_.size(); ++i)
		{
			for (unsigned int j=0; j!=active_iids_[i].first.size(); ++j)
			{
				destroyInvertedIndex(active_iids_[i].first[j]);
			}
			active_iids_[i].first.clear();
		}
		active_iids_.clear();
		
		
		for (unsigned int i=0; i!=vec_actives_.size(); ++i)
		{
			if (vec_inactives_.size() == 0)
			{
				vec_actives_[i]->nn = NULL;
				vec_actives_[i]->predecessor_sim = -1.0;
			}
			
			for (unsigned int j=0; j!=n_threads_; ++j)
			{
				if (fabs(thread_data_[j].float_array[i] - vec_actives_[i]->predecessor_sim) <= precision_)
				{
					if (vec_actives_[i]->nn->c_id > vec_actives_[thread_data_[j].uint_array[i]]->c_id)
					{
						vec_actives_[i]->nn = vec_actives_[thread_data_[j].uint_array[i]];
						vec_actives_[i]->predecessor_sim = thread_data_[j].float_array[i];
					}
				}
				else
				{
					if (thread_data_[j].float_array[i] > vec_actives_[i]->predecessor_sim)
					{
						vec_actives_[i]->nn = vec_actives_[thread_data_[j].uint_array[i]];
						vec_actives_[i]->predecessor_sim = thread_data_[j].float_array[i];
					}
				}
			}
		}
		
		vec_actives_.insert(vec_actives_.end(), vec_inactives_.begin(), vec_inactives_.end());
		vec_inactives_.clear();
	}
	
	// Clean up
	destroyThreadData();
	
	// ----------------------------------------------------------------------
	// SWITCH FROM STORED DATA TO STORED MATRIX APPROACH
	//
	// This position of the function is only reached when switching is enabled
	// and possible.
	// If RNN parallel algorithm has finished the clustering this position
	// is not reached.
	// ----------------------------------------------------------------------
	
	// Step 1: create feature InvertedIndices for every cluster
	tmp.clear();
	lib_iindices_.clear();
	
	for (unsigned int i=0; i!=vec_actives_.size(); ++i)
	{
		for (unsigned int j=0; j!=vec_actives_[i]->leaf_members.size(); ++j)
		{
// 			tmp.push_back(make_pair(vec_actives_[i]->leaf_members[j], i));
			tmp.push_back(make_pair(vec_actives_[i]->leaf_members[j], i));
		}
		
		createInvertedIndices(tmp, lib_iindices_);
		vec_actives_[i]->c_members.insert(lib_iindices_.begin(), lib_iindices_.end());
		lib_iindices_.clear();
		tmp.clear();
	}
	
	switchStorageMethod();
	
	// Step 3: start NNChainCore algorithm which is very fast now
	NNChainCore(root);
	
	return;
}


void BinaryFingerprintMethods::NNChainCore(Cluster*& root)
{
	if (verbosity_ >= 10)
	{
		Log << "++ Starting NNChain calculations" << endl;
	}
	
	if (threads_ == NULL)
	{
		createThreadData(blocksize_, 0, 0);
	}
	
	if (vec_actives_.size() == 0)
	{
		root = NULL;
		return;
	}
	
	// Indicates if rnn found
	bool rnn;
	
	// Empty NNChain at the beginning
	nn_chain_size_ = 0;
	
	// Counting available clusters
	unsigned int n_clusters = vec_actives_.size();
	
	// While there are clusters to merge
	while (n_clusters > 1)
	{
		// Check if NNChain is empty
		if (nn_chain_size_ == 0)
		{
			// Yes - so initialize
			nn_chain_tip_ = *vec_actives_.rbegin();
			nn_chain_tip_->predecessor = NULL;
			nn_chain_tip_->predecessor_sim = -1.0;
			nn_chain_tip_->predecessor_sim_sum = -1.0;
			
			nn_chain_size_ = 1;
			vec_actives_.pop_back();
		}
		
		rnn = false;
		
		while (!rnn)
		{
			// Find nearest neighbour to nn_chain_tip_
			nextNearestNeighbour();
			
			// Check if nn_chain_tip_ is more similar to nearest neighbour than to its predecessor
			if (fabs(current_nn_sim_ - nn_chain_tip_->predecessor_sim) <= precision_)
			{
				// Equally similar - so append to NNChain
				moveNearestNeighbour();
			}
			else
			{
				if (current_nn_sim_ < nn_chain_tip_->predecessor_sim)
				{
					// Less similar - thus RNN pair at the head of the NNChain
					rnn = true;
				}
				else
				{
					// More similar - so append to NNChain
					moveNearestNeighbour();
				}
			}
		}
		
		// Merge RNN pair heading the NNChain
		mergeClusters(nn_chain_tip_, nn_chain_tip_->predecessor, nn_chain_tip_->predecessor_sim_sum);
		
		// One cluster less to merge ...
		n_clusters -= 1;
	}
	
	// Done. 
	// Root cluster is the last remaining cluster
	root = vec_actives_[0];
	vec_actives_.clear();
	
	destroyThreadData();
	
	return;
}


void BinaryFingerprintMethods::clusterSelectionKGS(map<unsigned int, vector<unsigned int> >& cluster_selection)
{
	vector<Cluster*> clusters;
	vector<Cluster*>::iterator cl_iter;
	vector<Cluster*>::reverse_iterator r_iter;
	
	
	// ------------------------------------------------------------------------
	// STEP 0: Extract true cluster hierarchy
	// ------------------------------------------------------------------------
	for (unsigned int i=0; i!=leaf_clusters_.size(); ++i)
	{
		leaf_clusters_[i]->is_rnn = true;
	}
	
	unsigned int cluster_index = 0;
	map<float, vector<Cluster*> >::iterator it = internal_clusters_.begin();
	for (; it!=internal_clusters_.end(); ++it)
	{
		for (r_iter = it->second.rbegin(); r_iter!=it->second.rend(); ++r_iter)
		{
			(*r_iter)->is_rnn = false;
			(*r_iter)->c_index = cluster_index++;
			
			clusters.push_back(*r_iter);
			
// 			cerr << it->first << endl;
		}
	}
	
	unsigned int n_clust = 1;
	unsigned int n_singl = 0;
	vector<pair<unsigned int, unsigned int> > cluster_counts;
	for (unsigned int i=0; i!=clusters.size(); ++i)
	{
		cluster_counts.push_back(make_pair(n_clust, n_singl));
		
// 		cerr << n_clust << " " << n_singl << endl;
		
		if (clusters[i]->l_child->is_rnn || clusters[i]->r_child->is_rnn)
		{
			if (clusters[i]->l_child->is_rnn && clusters[i]->r_child->is_rnn)
			{
				// Two singletons, one cluster less.
				n_clust -= 1;
				n_singl += 2;
			}
			else
			{
				// One singleton, number of clusters has not changed.
				++n_singl;
			}
		}
		else
		{
			// No singletons produced
			++n_clust;
		}
	}
	
	for (unsigned int i=0; i!=leaf_clusters_.size(); ++i)
	{
		leaf_clusters_[i]->is_rnn = false;
	}
	
	
	// ------------------------------------------------------------------------
	// STEP 1: Calculate spread for every non-singleton cluster
	// ------------------------------------------------------------------------
	vector<double> cl_spreads(leaf_clusters_.size() - 1, 0.0);
	
	double spread;
	for (unsigned int i=0; i!=clusters.size(); ++i)
	{
		// Convert average similarity-spread to average dissimilarity-spread to match Kelley formulas
		// 1 - similarity = dissimilarity 
		spread = 1 - (clusters[i]->sim_sum / (double)((clusters[i]->c_size * clusters[i]->c_size - clusters[i]->c_size) / 2));
		cl_spreads[i] = spread;
	}
	
	
	// ------------------------------------------------------------------------
	// STEP 2: Calculate average spread for every hierarchy level
	// ------------------------------------------------------------------------
	vector<double> avg_spreads(leaf_clusters_.size() - 1, 0.0);
	
	double avg_spread;
	double spread_sum = 0.0;
	double max_avg_spread = 0.0;
	double min_avg_spread = FLT_MAX;
	
	for (int i=clusters.size()-1; i>=0; --i)
	{
		spread_sum += cl_spreads[i];
		avg_spread = spread_sum / (double)(cluster_counts[i].first);
		
		avg_spreads[i] = avg_spread;
// 		cerr << cluster_counts[i].first << " " << cluster_counts[i].second << endl;
// 		cerr << spread_sum << " " << avg_spread << endl;
		
		if (avg_spread > max_avg_spread)
		{
			max_avg_spread = avg_spread;
		}
		if (avg_spread < min_avg_spread)
		{
			min_avg_spread = avg_spread;
		}
	}
	cl_spreads.clear();
// 	cerr << max_avg_spread << " " << min_avg_spread << endl;
	
	// ------------------------------------------------------------------------
	// STEP 3: Normalize average spreads and calculate KGS penalty values
	// ------------------------------------------------------------------------
	vector<double> kgs_penalties = vector<double>(leaf_clusters_.size() - 1, 0.0);
	
	double avg_spread_norm;
	double scale = (leaf_clusters_.size() - 2) / (max_avg_spread - min_avg_spread);
	
	double kgs_min = FLT_MAX;
	double kgs_max = FLT_MIN;
	unsigned int kgs_min_index = 0;
	for (unsigned int i=0; i!=clusters.size(); ++i)
	{
		avg_spread_norm = scale * (avg_spreads[i] - min_avg_spread) + 1;
		
		kgs_penalties[i] = avg_spread_norm + cluster_counts[i].first + cluster_counts[i].second;
		
// 		cerr << avg_spread_norm << " " << kgs_penalties[i] << endl;
		
		if (kgs_penalties[i] < kgs_min)
		{
			kgs_min = kgs_penalties[i];
			kgs_min_index = i;
		}
		if (kgs_penalties[i] > kgs_max)
		{
			kgs_max = kgs_penalties[i];
		}
	}
	avg_spreads.clear();
// 	cerr << kgs_min << " " << kgs_max << endl;
// 	cerr << kgs_min_index << endl;
	
	
	// ------------------------------------------------------------------------
	// Apply KGS cutoff to create final cluster mapping
	// ------------------------------------------------------------------------
	unsigned int cluster_id = 0;
	for (unsigned int i=0; i!=clusters.size(); ++i)
	{
		if (i > kgs_min_index - 1)
		{
			if (!clusters[i]->is_rnn)
			{
				enumerateClusterMembers(clusters[i], cluster_id++);
			}
		}
		else
		{
			if (clusters[i]->l_child->l_child == NULL)
			{
				clusters[i]->l_child->c_index = cluster_id++;
			}
			if (clusters[i]->r_child->l_child == NULL)
			{
				clusters[i]->r_child->c_index = cluster_id++;
			}
		}
	}
	
	pair<map<unsigned int, vector<unsigned int> >::iterator, bool> cl_inserter;
	for (unsigned int i=0; i!=leaf_clusters_.size(); ++i)
	{
		cl_inserter = cluster_selection.insert(make_pair(leaf_clusters_[i]->c_index, vector<unsigned int>()));
		cl_inserter.first->second.push_back(leaf_clusters_[i]->c_id);
	}
}


void BinaryFingerprintMethods::enumerateClusterMembers(Cluster* cl, unsigned int cluster_id)
{
	cl->is_rnn = true;
	
	if (cl->l_child == NULL)
	{
		// Leaf
		cl->c_index = cluster_id;
		return;
	}
	
	enumerateClusterMembers(cl->l_child, cluster_id);
	enumerateClusterMembers(cl->r_child, cluster_id);
}


void BinaryFingerprintMethods::nextNearestNeighbour()
{
	current_nn_sim_ = -1.0;
	
	float current_sim;
	unsigned int tip_index = nn_chain_tip_->c_index;
	
	vector<Cluster*>::iterator cl_iter = vec_actives_.begin();
	for (; cl_iter!=vec_actives_.end(); ++cl_iter)
	{
		if ((*cl_iter)->c_index < tip_index)
		{
// 			cerr << strictUpperTriangularMatrixToArray((*cl_iter)->c_index, tip_index) << " " << 
// 			sim_matrix_[strictUpperTriangularMatrixToArray((*cl_iter)->c_index, tip_index)] << endl;
			current_sim = sim_matrix_[strictUpperTriangularMatrixToArray((*cl_iter)->c_index, tip_index)];
		}
		else
		{
// 			cerr << strictUpperTriangularMatrixToArray(tip_index, (*cl_iter)->c_index) << 
// 			sim_matrix_[strictUpperTriangularMatrixToArray(tip_index, (*cl_iter)->c_index)] << endl;
			current_sim = sim_matrix_[strictUpperTriangularMatrixToArray(tip_index, (*cl_iter)->c_index)];
		}
		
		if (fabs(current_sim - current_nn_sim_) <= precision_)
		{
			// EQUALLY SIMILAR: choose cluster with smaller cluster_id
			if ((*cl_iter)->c_id < (*current_nn_)->c_id)
			{
				current_nn_ = cl_iter;
				current_nn_sim_ = current_sim;
			}
		}
		else
		{
			// HIGHER SIMILARITY: new nearest neighbour
			if (current_sim > current_nn_sim_)
			{
				current_nn_ = cl_iter;
				current_nn_sim_ = current_sim;
			}
		}
		
	}
}


void BinaryFingerprintMethods::moveNearestNeighbour()
{
	Cluster* current_nn = *current_nn_;
	
	current_nn->predecessor = nn_chain_tip_;
	current_nn->predecessor_sim = current_nn_sim_;
	current_nn->predecessor_sim_sum = current_nn_sim_ * (nn_chain_tip_->c_size * current_nn->c_size);
	
	nn_chain_tip_ = current_nn;
	
	++nn_chain_size_;
	
	vec_actives_.erase(current_nn_);
}


void BinaryFingerprintMethods::switchStorageMethod()
{
	if (verbosity_ >= 10)
	{
		Log << "++ Switch to stored matrix method" << endl;
	}
	
	// Create cluster indices to address the similarity matrix
	unsigned int tmp_index = 0;
	BOOST_FOREACH(Cluster *c, vec_actives_)
	{
		c->c_index = tmp_index++;
	}
	
	unsigned long n_clusters = vec_actives_.size();
	n_comparisons_ = (n_clusters * n_clusters - n_clusters) / 2;
	
	// Allocate memory for similarity matrix
	sim_matrix_ = new float[n_comparisons_ + 1];
	sim_matrix_[n_comparisons_] = -1.0;
	
	if (threads_==NULL)
	{
		createThreadData(blocksize_, 0, 0);
	}
	
	for (unsigned int i=0; i!=n_threads_; ++i)
	{
		threads_[i] = thread(boost::bind(&BinaryFingerprintMethods::similarityMatrixFromClustersThread, this, i));
	}
	
	for (unsigned int i=0; i!=n_threads_; ++i)
	{
		threads_[i].join();
	}
	
	destroyThreadData();
	
	// Free memory
	BOOST_FOREACH(Cluster *c, vec_actives_)
	{
		BOOST_FOREACH(InvertedIndex *ii, c->c_members)
		{
			destroyInvertedIndex(ii);
		}
		c->c_members.clear();
	}
	
	clustering_method_ = STORED_MATRIX;
}


BinaryFingerprintMethods::Cluster* BinaryFingerprintMethods::createCluster()
{
	Cluster* cluster = new Cluster;
	
	cluster->c_id = n_clusters_;
	cluster->c_index = n_clusters_;
	cluster->c_size = 1;
	cluster->l_child = NULL;
	cluster->r_child = NULL;
	cluster->sim_sum = 0.0;
	cluster->predecessor = NULL;
	cluster->predecessor_sim = 0.0;
	cluster->predecessor_sim_sum = 0.0;
	
	++n_clusters_;
	
	return cluster;
}


void BinaryFingerprintMethods::finalizeClustering()
{
	if (clustering_method_ == STORED_MATRIX)
	{
		if (sim_matrix_ != NULL)
		{
			delete [] sim_matrix_;
			sim_matrix_ = NULL;
		}
	}
	
	destroyThreadData();
	
	vector<Cluster*>::iterator leaf_iter;
	for (leaf_iter=leaf_clusters_.begin(); leaf_iter!=leaf_clusters_.end(); ++leaf_iter)
	{
		BOOST_FOREACH(InvertedIndex *ii, (*leaf_iter)->c_members)
		{
			destroyInvertedIndex(ii);
		}
		(*leaf_iter)->c_members.clear();
		
		delete *leaf_iter;
	}
	leaf_clusters_.clear();
	
	boost::unordered_set<InvertedIndex*> c_members;
	map<float, vector<Cluster*> >::iterator internal_it;
	for (internal_it=internal_clusters_.begin(); internal_it!=internal_clusters_.end(); ++internal_it)
	{
		for (unsigned int i=0; i!= internal_it->second.size(); ++i)
		{
			c_members = internal_it->second[i]->c_members;
			BOOST_FOREACH(InvertedIndex *ii, c_members)
			{
				destroyInvertedIndex(ii);
			}
			c_members.clear();
			
			delete internal_it->second[i];
		}
	}
	internal_clusters_.clear();
	
	n_clusters_ = 0;
}


BinaryFingerprintMethods::Cluster* BinaryFingerprintMethods::mergeClusters(Cluster* c1, Cluster* c2, double sim_sum)
{
	// CREATE NEW CLUSTER
	Cluster* merged_cluster = createCluster();
	
	merged_cluster->l_child = c1;
	merged_cluster->r_child = c2;
	merged_cluster->c_size = merged_cluster->l_child->c_size + merged_cluster->r_child->c_size;
	
	if (clustering_method_ == STORED_DATA_PARALLEL)
	{
		sim_sum = sim_sum * (c1->c_size * c2->c_size);
	}
	
	merged_cluster->sim_sum = c1->sim_sum + c2->sim_sum + sim_sum;
	
// 	cerr << merged_cluster->c_id << " " << c1->c_id << " " << c2->c_id << " " << (sim_sum / (c1->c_size * c2->c_size)) << endl;
	
	double tmp_sim = sim_sum / (merged_cluster->l_child->c_size * merged_cluster->r_child->c_size);
	pair<map<float, vector<Cluster*> >::iterator, bool> ins;
	ins = internal_clusters_.insert(make_pair(tmp_sim, vector<Cluster*>()));
	ins.first->second.push_back(merged_cluster);
	
	
	if (clustering_method_ == STORED_DATA_PARALLEL)
	{
		merged_cluster->leaf_members.insert(merged_cluster->leaf_members.begin(), c1->leaf_members.begin(), c1->leaf_members.end());
		merged_cluster->leaf_members.insert(merged_cluster->leaf_members.begin(), c2->leaf_members.begin(), c2->leaf_members.end());
		c1->leaf_members.clear();
		c2->leaf_members.clear();
		
		sort(merged_cluster->leaf_members.begin(), merged_cluster->leaf_members.end());
		
		return merged_cluster;
	}
	else
	{
		// Insert merged cluster into active_clusters and all_clusters
		vec_actives_.push_back(merged_cluster);
		merged_cluster->c_index = c1->c_index;
		
		// Reset tip of NNChain
		nn_chain_size_ -= 2;
		
		if (nn_chain_size_==0)
		{
			nn_chain_tip_ = NULL;
		}
		else
		{
			nn_chain_tip_ = c2->predecessor;
		}
		
		// UPDATE SIMILARITY MATRIX USING LANCE and WILLIAMS FORMULA
		// Step 1: Update similarities to all clusters in active_clusters
		
		unsigned int n_updates = vec_actives_.size();
		
		unsigned int n_threads = n_threads_;
		if (n_updates < n_threads_)
		{
			n_threads = n_updates;
		}
		
		unsigned int batch_size = n_updates / n_threads;
		unsigned int last_batch_size = (n_updates % n_threads) + batch_size;
		
		for (unsigned int i=0; i!=n_threads; ++i)
		{
			if (i==n_threads - 1)
			{
				thread_data_[i].first = i * batch_size;
				thread_data_[i].last = thread_data_[i].first + last_batch_size;
			}
			else
			{
				thread_data_[i].first = i * batch_size;
				thread_data_[i].last = thread_data_[i].first + batch_size;
			}
			
			threads_[i] = thread(boost::bind(&BinaryFingerprintMethods::similarityUpdateAverageLinkageThread, this, i, merged_cluster));
		}
		
		for (unsigned int i=0; i!=n_threads; ++i)
		{
			threads_[i].join();
		}
		
		// Step 2: Update similarities to all clusters in NNChain
		Cluster *current = nn_chain_tip_;
		while (current)
		{
			similarityUpdateAverageLinkage(merged_cluster, current);
			current = current->predecessor;
		}
	}
	
	return 0;
}





















