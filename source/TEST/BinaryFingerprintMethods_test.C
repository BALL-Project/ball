// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

/////////////////////////////////////////////////////////////

#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/SYSTEM/file.h>

#include <boost/unordered_map.hpp>

/////////////////////////////////////////////////////////////
// To be tested ...
#include <BALL/STRUCTURE/binaryFingerprintMethods.h>
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;
using namespace boost;

START_TEST(BinaryFingerprintMethods)

PRECISION(1e-5)

Log.disableOutput();

/////////////////////////////////////////////////////////////
// Fingerprint reading

vector<String> cmpd_ids;
vector<vector<unsigned short> > fprints;

vector<String> lib_ids;
vector<String> query_ids;
vector<vector<unsigned short> > lib;
vector<vector<unsigned short> > query;

CHECK(parseFingerprintBitstring())
	unsigned short fp1_tmp[] = {13,40,63,82,97,103,119,123,129,181,215,222,236,244,271,286,299,301,342,359,363,413,419,457,486,499,506,537,544,553,557,570,589,605,609,615,640,669,701,747,777,792,793,809,824,892,901,933,942,958,980};
	unsigned short fpN_tmp[] = {2,13,21,29,47,82,102,110,116,119,138,141,153,160,196,217,218,227,232,234,236,268,271,295,306,334,336,342,352,359,364,502,506,522,537,553,586,628,676,677,705,707,712,729,731,765,792,793,809,829,830,839,851,856,858,867,882,895,901,903,935,988,989};
	vector<unsigned short> fp1(fp1_tmp, fp1_tmp + sizeof(fp1_tmp) / sizeof(unsigned short));
	vector<unsigned short> fpN(fpN_tmp, fpN_tmp + sizeof(fpN_tmp) / sizeof(unsigned short));
	
	reverse(fp1.begin(), fp1.end());
	reverse(fpN.begin(), fpN.end());
	
	Molecule* m;
	vector<unsigned short> tmp;
	SDFile f(BALL_TEST_DATA_PATH(BinaryFingerprintMethods_test.sdf), File::MODE_IN);
	while ((m = f.read()))
	{
		BinaryFingerprintMethods::parseBinaryFingerprint(m->getProperty("ECFP4_1024").getString(), tmp, 1);
		fprints.push_back(tmp);
		delete m;
	}
	delete m;
	f.close();
	
	TEST_EQUAL(fp1.size(), fprints[0].size());
	for (unsigned int i=0; i!=fp1.size(); ++i)
	{
		TEST_EQUAL(fp1[i], fprints[0][i]);
	}
	
	TEST_EQUAL(fpN.size(), fprints[fprints.size()-1].size());
	for (unsigned int i=0; i!=fpN.size(); ++i)
	{
		TEST_EQUAL(fpN[i], fprints[fprints.size()-1][i]);
	}
	
	String fp_test = "";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 1), true);
	TEST_EQUAL(tmp.size(), 0);
	
	fp_test = "10";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 1), true);
	TEST_EQUAL(tmp.size(), 1);
	
	fp_test = "aa";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 1), false);
	TEST_EQUAL(tmp.size(), 0);
	
	fp_test = "a1";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 1), false);
	TEST_EQUAL(tmp.size(), 0);
	
	fp_test = "1a";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 1), false);
	TEST_EQUAL(tmp.size(), 0);
RESULT

fprints.clear();

CHECK(parseFingerprintFeatureList())
	unsigned short fp1_tmp[] = {13,40,63,82,97,103,119,123,129,181,215,222,236,244,271,286,299,301,342,359,363,413,419,457,486,499,506,537,544,553,557,570,589,605,609,615,640,669,701,747,777,792,793,809,824,892,901,933,942,958,980};
	unsigned short fpN_tmp[] = {2,13,21,29,47,82,102,110,116,119,138,141,153,160,196,217,218,227,232,234,236,268,271,295,306,334,336,342,352,359,364,502,506,522,537,553,586,628,676,677,705,707,712,729,731,765,792,793,809,829,830,839,851,856,858,867,882,895,901,903,935,988,989};
	vector<unsigned short> fp1(fp1_tmp, fp1_tmp + sizeof(fp1_tmp) / sizeof(unsigned short));
	vector<unsigned short> fpN(fpN_tmp, fpN_tmp + sizeof(fpN_tmp) / sizeof(unsigned short));
	
	reverse(fp1.begin(), fp1.end());
	reverse(fpN.begin(), fpN.end());
	
	Molecule* m;
	vector<unsigned short> tmp;
	SDFile f(BALL_TEST_DATA_PATH(BinaryFingerprintMethods_test.sdf), File::MODE_IN);
	while ((m = f.read()))
	{
		cmpd_ids.push_back(m->getProperty(("CMPD_ID")).getString());
		BinaryFingerprintMethods::parseBinaryFingerprint(m->getProperty("ECFP4").getString(), tmp, 2);
		fprints.push_back(tmp);
		delete m;
	}
	delete m;
	f.close();
	
	for (unsigned int i=0; i!=fprints.size(); ++i)
	{
		if (i < 6)
		{
			query.push_back(fprints[i]);
			query_ids.push_back(cmpd_ids[i]);
		}
		else
		{
			lib.push_back(fprints[i]);
			lib_ids.push_back(cmpd_ids[i]);
		}
	}
	
	TEST_EQUAL(fp1.size(), fprints[0].size());
	for (unsigned int i=0; i!=fp1.size(); ++i)
	{
		TEST_EQUAL(fp1[i] + 1, fprints[0][i]);
	}
	
	TEST_EQUAL(fpN.size(), fprints[fprints.size()-1].size());
	for (unsigned int i=0; i!=fpN.size(); ++i)
	{
		TEST_EQUAL(fpN[i] + 1, fprints[fprints.size()-1][i]);
	}
	
	String fp_test = "";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 2), true);
	TEST_EQUAL(tmp.size(), 0);
	
	fp_test = "5";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 2), true);
	TEST_EQUAL(tmp.size(), 1);
	
	fp_test = "5,3";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 2), true);
	TEST_EQUAL(tmp.size(), 2);
	
	fp_test = "5,3,";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 2), true);
	TEST_EQUAL(tmp.size(), 2);
	
	fp_test = "a";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 2), false);
	TEST_EQUAL(tmp.size(), 0);
	
	fp_test = "5,a";
	TEST_EQUAL(BinaryFingerprintMethods::parseBinaryFingerprint(fp_test, tmp, 2), false);
	TEST_EQUAL(tmp.size(), 0);
RESULT


/////////////////////////////////////////////////////////////
// Constructors and Destructors

BinaryFingerprintMethods *b;
CHECK(BinaryFingerprintMethods())
	b = new BinaryFingerprintMethods();
	
	TEST_NOT_EQUAL(b, 0)
	
	const Options o1 = b->getOptions();
	
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE), BinaryFingerprintMethods::Default::BLOCKSIZE);
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::SIM_CUTOFF), BinaryFingerprintMethods::Default::SIM_CUTOFF);
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::PRECISION), BinaryFingerprintMethods::Default::PRECISION);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::N_THREADS), BinaryFingerprintMethods::Default::N_THREADS);
	TEST_EQUAL(o1.getBool(BinaryFingerprintMethods::Option::STORE_NN), BinaryFingerprintMethods::Default::STORE_NN);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::VERBOSITY), BinaryFingerprintMethods::Default::VERBOSITY);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS), BinaryFingerprintMethods::Default::MAX_CLUSTERS);
	
	TEST_EQUAL(b->getTargetLibrarySize(), 0);
	TEST_EQUAL(b->getQueryLibrarySize(), 0);
RESULT

CHECK(~BinaryFingerprintMethods())
	delete b;
RESULT

CHECK(BinaryFingerprintMethods(Options))
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 13);
	options.setDefaultReal(BinaryFingerprintMethods::Option::SIM_CUTOFF, 0.5);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 1);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 5);
	
	BinaryFingerprintMethods bfm(options);
	
	const Options o1 = bfm.getOptions();
	
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE), 13);
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::SIM_CUTOFF), 0.5);
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::PRECISION), BinaryFingerprintMethods::Default::PRECISION);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::N_THREADS), 1);
	TEST_EQUAL(o1.getBool(BinaryFingerprintMethods::Option::STORE_NN), BinaryFingerprintMethods::Default::STORE_NN);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::VERBOSITY), 5);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS), BinaryFingerprintMethods::Default::MAX_CLUSTERS);
	
	TEST_EQUAL(bfm.getTargetLibrarySize(), 0);
	TEST_EQUAL(bfm.getQueryLibrarySize(), 0);
	
RESULT

CHECK(BinaryFingerprintMethods(Options, LibraryFeatures))
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 13);
	options.setDefaultReal(BinaryFingerprintMethods::Option::SIM_CUTOFF, 0.5);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 1);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 5);
	
	BinaryFingerprintMethods bfm(options, lib);
	
	const Options o1 = bfm.getOptions();
	
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE), 13);
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::SIM_CUTOFF), 0.5);
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::PRECISION), BinaryFingerprintMethods::Default::PRECISION);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::N_THREADS), 1);
	TEST_EQUAL(o1.getBool(BinaryFingerprintMethods::Option::STORE_NN), BinaryFingerprintMethods::Default::STORE_NN);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::VERBOSITY), 5);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS), BinaryFingerprintMethods::Default::MAX_CLUSTERS);
	
	TEST_EQUAL(bfm.getTargetLibrarySize(), lib.size());
	TEST_EQUAL(bfm.getQueryLibrarySize(), 0);
RESULT

CHECK(BinaryFingerprintMethods(Options, LibraryFeatures, QueryFeatures))
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 13);
	options.setDefaultReal(BinaryFingerprintMethods::Option::SIM_CUTOFF, 0.5);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 1);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 5);
	
	BinaryFingerprintMethods bfm(options, lib, query);
	
	const Options o1 = bfm.getOptions();
	
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE), 13);
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::SIM_CUTOFF), 0.5);
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::PRECISION), BinaryFingerprintMethods::Default::PRECISION);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::N_THREADS), 1);
	TEST_EQUAL(o1.getBool(BinaryFingerprintMethods::Option::STORE_NN), BinaryFingerprintMethods::Default::STORE_NN);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::VERBOSITY), 5);
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS), BinaryFingerprintMethods::Default::MAX_CLUSTERS);
	
	TEST_EQUAL(bfm.getTargetLibrarySize(), lib.size());
	TEST_EQUAL(bfm.getQueryLibrarySize(), query.size());
RESULT

CHECK(BinaryFingerprintMethods(const BinaryFingerprintMethods& bfm))
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 13);
	options.setDefaultReal(BinaryFingerprintMethods::Option::SIM_CUTOFF, 0.5);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 1);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 5);
	
	BinaryFingerprintMethods bfm(options, lib, query);
	BinaryFingerprintMethods bfm_copy(bfm);
	
	TEST_EQUAL(bfm_copy.getTargetLibrarySize(), lib.size());
	TEST_EQUAL(bfm_copy.getQueryLibrarySize(), query.size());
	
	const Options o1 = bfm.getOptions();
	const Options o2 = bfm_copy.getOptions();
	
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE), o2.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE));
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::SIM_CUTOFF), o2.getReal(BinaryFingerprintMethods::Option::SIM_CUTOFF));
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::PRECISION), o2.getReal(BinaryFingerprintMethods::Option::PRECISION));
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::N_THREADS), o2.getInteger(BinaryFingerprintMethods::Option::N_THREADS));
	TEST_EQUAL(o1.getBool(BinaryFingerprintMethods::Option::STORE_NN), o2.getBool(BinaryFingerprintMethods::Option::STORE_NN));
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::VERBOSITY), o2.getInteger(BinaryFingerprintMethods::Option::VERBOSITY));
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS), o2.getInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS));
RESULT

CHECK(Assignment)
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 9);
	options.setDefaultReal(BinaryFingerprintMethods::Option::SIM_CUTOFF, 0.2);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 1);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 3);
	
	BinaryFingerprintMethods bfm(options, lib, query);
	BinaryFingerprintMethods bfm_copy = bfm;
	
	TEST_EQUAL(bfm_copy.getTargetLibrarySize(), lib.size());
	TEST_EQUAL(bfm_copy.getQueryLibrarySize(), query.size());
	
	const Options o1 = bfm.getOptions();
	const Options o2 = bfm_copy.getOptions();
	
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE), o2.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE));
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::SIM_CUTOFF), o2.getReal(BinaryFingerprintMethods::Option::SIM_CUTOFF));
	TEST_REAL_EQUAL(o1.getReal(BinaryFingerprintMethods::Option::PRECISION), o2.getReal(BinaryFingerprintMethods::Option::PRECISION));
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::N_THREADS), o2.getInteger(BinaryFingerprintMethods::Option::N_THREADS));
	TEST_EQUAL(o1.getBool(BinaryFingerprintMethods::Option::STORE_NN), o2.getBool(BinaryFingerprintMethods::Option::STORE_NN));
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::VERBOSITY), o2.getInteger(BinaryFingerprintMethods::Option::VERBOSITY));
	TEST_EQUAL(o1.getInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS), o2.getInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS));
RESULT


/////////////////////////////////////////////////////////////
// Fingerprint Similarity Search

CHECK(cutoffSearch())
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 13);
	options.setDefaultReal(BinaryFingerprintMethods::Option::SIM_CUTOFF, 0.5);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 1);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 0);
	
	BinaryFingerprintMethods bfm(options);
	
	float sim;
	String key;
	boost::unordered_map<string, float> results;
	LineBasedFile lbf(BALL_TEST_DATA_PATH(BinaryFingerprintMethods_SimSearchResults.csv), File::MODE_IN);
	while(lbf.readLine())
	{
		sim = lbf.getField(2).toFloat();
		key = lbf.getField(0) + "_" + lbf.getField(1);
		
		results.insert(make_pair(key.c_str(), sim));
	}
	lbf.close();
	
	bfm.setLibraryFeatures(lib);
	bfm.setQueryFeatures(query);
	
	float cutoff = 0.0;
	String outfile_name = "_BALL_CUTOFF_SEARCH_TEST.tmp";
	boost::unordered_map<string, float>::iterator it;
	boost::unordered_map<string, float> results_subset;
	
	while (cutoff <= 1.0)
	{
		results_subset.clear();
		
		bfm.cutoffSearch(cutoff, outfile_name);
		
		for (it=results.begin(); it!=results.end(); ++it)
		{
			if (it->second >= cutoff)
			{
				results_subset.insert(*it);
			}
		}
		
		LineBasedFile lbf(outfile_name, File::MODE_IN);
		while(lbf.readLine())
		{
			sim = lbf.getField(2).toFloat();
			key = query_ids[lbf.getField(0).toUnsignedInt()] + "_" + lib_ids[lbf.getField(1).toUnsignedInt()];
			
			TEST_REAL_EQUAL(results_subset[key.c_str()], sim);
			results_subset.erase(key.c_str());
		}
		lbf.close();
		
		File::remove(outfile_name);
		
		TEST_EQUAL(results_subset.size(), 0);
		
		cutoff += 0.1;
	}
RESULT

/////////////////////////////////////////////////////////////
// Connected Components Decomposition

CHECK(connectedComponents(store_nns=true))
	String key;
	unsigned int id, min_id, int_cutoff;
	map<string, float> tmp_cc;
	map<string, float>::iterator cc_it;
	boost::unordered_map<unsigned int, map<string, float> > ccs_results;
	boost::unordered_map<unsigned int, map<string, float> >::iterator ccs_it;
	boost::unordered_map<unsigned int, boost::unordered_map<unsigned int, map<string, float> > > all_ccs;
	
	LineBasedFile lbf(BALL_TEST_DATA_PATH(BinaryFingerprintMethods_ConnectedComponents.csv), File::MODE_IN);
	while(lbf.readLine())
	{
		if (lbf.getField(0) == "#")
		{
			if (!tmp_cc.empty())
			{
				all_ccs[int_cutoff].insert(make_pair(min_id, tmp_cc));
				tmp_cc.clear();
				
				min_id = fprints.size() + 1;
			}
			
			int_cutoff = lbf.getField(1).toUnsignedInt();
			all_ccs[int_cutoff] = boost::unordered_map<unsigned int, map<string, float> >();
			
			continue;
		}
		if (lbf.getField(0) == "//")
		{
			all_ccs[int_cutoff].insert(make_pair(min_id, tmp_cc));
			tmp_cc.clear();
			
			min_id = fprints.size() + 1;
			
			continue;
		}
		
		id = lbf.getField(0).toUnsignedInt();
		key = String(id) + "_" + lbf.getField(1);
		tmp_cc.insert(make_pair(key, lbf.getField(2).toFloat()));
		
		if (id < min_id)
		{
			min_id = id;
		}
	}
	lbf.close();
	
	
// 	unordered_map<unsigned int, unordered_map<unsigned int, map<string, float> > >::iterator it = all_ccs.begin();
// 	for (; it!=all_ccs.end(); ++it)
// 	{
// 		if (it->first == 9)
// 		{
// 			for (ccs_it=it->second.begin(); ccs_it!=it->second.end(); ++ccs_it)
// 			{
// 				cerr << "# " << ccs_it->first << endl;
// 				for (cc_it=ccs_it->second.begin(); cc_it!=ccs_it->second.end(); ++cc_it)
// 				{
// 					cerr << cc_it->first << " " << cc_it->second << endl;
// 				}
// 			}
// 		}
// 	}
	
	
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 27);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 2);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 0);
	
	BinaryFingerprintMethods bfm(options);
	
	vector<unsigned int> m_indices;
	vector<vector<unsigned int> > ccs;
	vector<vector<pair<unsigned int, float> > > nn_data;
	
	float cutoff = 0.5;
	bool store_nns = true;
	
	TEST_EQUAL(bfm.connectedComponents(m_indices, ccs, nn_data, cutoff, store_nns), false);
	
	for (unsigned int i=0; i!=fprints.size(); ++i)
	{
		m_indices.push_back(i);
	}
	
	TEST_EQUAL(bfm.connectedComponents(m_indices, ccs, nn_data, cutoff, store_nns), false);
	
	bfm.setLibraryFeatures(fprints);
	
	m_indices.clear();
	TEST_EQUAL(bfm.connectedComponents(m_indices, ccs, nn_data, cutoff, store_nns), false);
	
	for (unsigned int i=0; i!=fprints.size(); ++i)
	{
		m_indices.push_back(i);
	}
	
	cutoff = 0.0;
	while (cutoff < 0.95)
	{
		TEST_EQUAL(bfm.connectedComponents(m_indices, ccs, nn_data, cutoff, store_nns), true);
		
		int_cutoff = int(cutoff * 10);
		ccs_results.clear();
		
		for (unsigned int i=0; i!=ccs.size(); ++i)
		{
			tmp_cc.clear();
			min_id = fprints.size() + 1;
			
			for (unsigned int j=0; j!=ccs[i].size(); ++j)
			{
				if (ccs[i][j] < min_id)
				{
					min_id = ccs[i][j];
				}
				
				key = String(ccs[i][j]) + "_" + String(ccs[i][nn_data[i][j].first]);
				tmp_cc[key] = nn_data[i][j].second;
			}
			
			ccs_results[min_id] = tmp_cc;
		}
		
		TEST_EQUAL(all_ccs[int_cutoff].size(), ccs_results.size());
		
		for (ccs_it=ccs_results.begin(); ccs_it!=ccs_results.end(); ++ccs_it)
		{
			id = ccs_it->first;
			
			TEST_EQUAL(all_ccs[int_cutoff][id].size(), ccs_it->second.size());
			
			for (cc_it=ccs_it->second.begin(); cc_it!=ccs_it->second.end(); ++cc_it)
			{
				TEST_REAL_EQUAL(all_ccs[int_cutoff][id][cc_it->first], cc_it->second);
				all_ccs[int_cutoff][id].erase(cc_it->first);
			}
			
			TEST_EQUAL(all_ccs[int_cutoff][id].size(), 0);
		}
		
		cutoff += 0.1;
	}
RESULT

CHECK(connectedComponents(store_nns=false))
	String key;
	unsigned int min_id = 0;
	unsigned int id, int_cutoff;
	map<string, float> tmp_cc;
	map<string, float>::iterator cc_it;
	boost::unordered_map<unsigned int, map<string, float> > ccs_results;
	boost::unordered_map<unsigned int, map<string, float> >::iterator ccs_it;
	boost::unordered_map<unsigned int, boost::unordered_map<unsigned int, map<string, float> > > all_ccs;
	
	LineBasedFile lbf(BALL_TEST_DATA_PATH(BinaryFingerprintMethods_ConnectedComponents.csv), File::MODE_IN);
	while(lbf.readLine())
	{
		if (lbf.getField(0) == "#")
		{
			if (!tmp_cc.empty())
			{
				all_ccs[int_cutoff].insert(make_pair(min_id, tmp_cc));
				tmp_cc.clear();
				
				min_id = fprints.size() + 1;
			}
			
			int_cutoff = lbf.getField(1).toUnsignedInt();
			all_ccs[int_cutoff] = boost::unordered_map<unsigned int, map<string, float> >();
			
			continue;
		}
		if (lbf.getField(0) == "//")
		{
			all_ccs[int_cutoff].insert(make_pair(min_id, tmp_cc));
			tmp_cc.clear();
			
			min_id = fprints.size() + 1;
			
			continue;
		}
		
		id = lbf.getField(0).toUnsignedInt();
		tmp_cc.insert(make_pair(String(id), 0.0));
		
		if (id < min_id)
		{
			min_id = id;
		}
	}
	lbf.close();
	
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 27);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 2);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 0);
	
	BinaryFingerprintMethods bfm(options);
	
	vector<unsigned int> m_indices;
	vector<vector<unsigned int> > ccs;
	vector<vector<pair<unsigned int, float> > > nn_data;
	
	float cutoff = 0.5;
	bool store_nns = false;
	
	TEST_EQUAL(bfm.connectedComponents(m_indices, ccs, nn_data, cutoff, store_nns), false);
	
	for (unsigned int i=0; i!=fprints.size(); ++i)
	{
		m_indices.push_back(i);
	}
	
	TEST_EQUAL(bfm.connectedComponents(m_indices, ccs, nn_data, cutoff, store_nns), false);
	
	bfm.setLibraryFeatures(fprints);
	
	m_indices.clear();
	TEST_EQUAL(bfm.connectedComponents(m_indices, ccs, nn_data, cutoff, store_nns), false);
	
	for (unsigned int i=0; i!=fprints.size(); ++i)
	{
		m_indices.push_back(i);
	}
	
	cutoff = 0.0;
	while (cutoff < 0.95)
	{
		TEST_EQUAL(bfm.connectedComponents(m_indices, ccs, nn_data, cutoff, store_nns), true);
		
		int_cutoff = int(cutoff * 10);
		ccs_results.clear();
		
		for (unsigned int i=0; i!=ccs.size(); ++i)
		{
			tmp_cc.clear();
			min_id = fprints.size() + 1;
			
			for (unsigned int j=0; j!=ccs[i].size(); ++j)
			{
				if (ccs[i][j] < min_id)
				{
					min_id = ccs[i][j];
				}
				
				tmp_cc[String(ccs[i][j])] = 0.0;
			}
			
			ccs_results[min_id] = tmp_cc;
		}
		
		TEST_EQUAL(all_ccs[int_cutoff].size(), ccs_results.size());
		
		for (ccs_it=ccs_results.begin(); ccs_it!=ccs_results.end(); ++ccs_it)
		{
			id = ccs_it->first;
			
			TEST_EQUAL(all_ccs[int_cutoff][id].size(), ccs_it->second.size());
			
			for (cc_it=ccs_it->second.begin(); cc_it!=ccs_it->second.end(); ++cc_it)
			{
				TEST_REAL_EQUAL(all_ccs[int_cutoff][id][cc_it->first], cc_it->second);
				all_ccs[int_cutoff][id].erase(cc_it->first);
			}
			
			TEST_EQUAL(all_ccs[int_cutoff][id].size(), 0);
		}
		
		cutoff += 0.1;
	}
RESULT

/////////////////////////////////////////////////////////////
// Calculate Medoid of a compound selection

CHECK(calculateSelectionMedoid())
	float lib_medoid_avg_sim = 0.0;
	float query_medoid_avg_sim = 0.0;
	unsigned int lib_medoid_index = 0;
	unsigned int query_medoid_index = 0;
	vector<float> tmp;
	vector<float> lib_avg_sims;
	vector<float> query_avg_sims;
	
	LineBasedFile lbf(BALL_TEST_DATA_PATH(BinaryFingerprintMethods_MedoidResults.csv), File::MODE_IN);
	while(lbf.readLine())
	{
		if (lbf.getField(0) == "#")
		{
			if (lbf.getField(1) == "QUERY")
			{
				query_avg_sims = tmp;
				query_medoid_index = lbf.getField(2).toUnsignedInt();
				query_medoid_avg_sim = lbf.getField(3).toFloat();
			}
			if (lbf.getField(1) == "LIB")
			{
				lib_avg_sims = tmp;
				lib_medoid_index = lbf.getField(2).toUnsignedInt();
				lib_medoid_avg_sim = lbf.getField(3).toFloat();
			}
			tmp.clear();
		}
		else
		{
			tmp.push_back(lbf.getField(1).toFloat());
		}
	}
	lbf.close();
	
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 27);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 1);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 0);
	
	vector<float> avg_sims;
	unsigned int medoid_index;
	vector<unsigned  int> m_indices;
	
	BinaryFingerprintMethods bfm(options);
	
	for (unsigned int i=0; i!=query.size(); ++i)
	{
		m_indices.push_back(i);
	}
	
	bfm.setLibraryFeatures(query);
	TEST_EQUAL(bfm.calculateSelectionMedoid(m_indices, medoid_index, avg_sims), true);
	
	TEST_EQUAL(avg_sims.size(), query.size());
	TEST_EQUAL(medoid_index, query_medoid_index);
	TEST_REAL_EQUAL(avg_sims[medoid_index], query_medoid_avg_sim);
	for (unsigned int i=0; i!=avg_sims.size(); ++i)
	{
		TEST_REAL_EQUAL(avg_sims[i], query_avg_sims[i]);
	}
	
	avg_sims.clear();
	m_indices.clear();
	for (unsigned int i=0; i!=lib.size(); ++i)
	{
		m_indices.push_back(i);
	}
	
	bfm.setLibraryFeatures(lib);
	TEST_EQUAL(bfm.calculateSelectionMedoid(m_indices, medoid_index, avg_sims), true);
	
	TEST_EQUAL(avg_sims.size(), lib.size());
	TEST_EQUAL(medoid_index, lib_medoid_index);
	TEST_REAL_EQUAL(avg_sims[medoid_index], lib_medoid_avg_sim);
	for (unsigned int i=0; i!=avg_sims.size(); ++i)
	{
		TEST_REAL_EQUAL(avg_sims[i], lib_avg_sims[i]);
	}
RESULT


/////////////////////////////////////////////////////////////
// Reciprocal Nearest Neighbour based average linkage clustering

CHECK(averageLinkageClustering( RNN_PARALLEL ))
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 35);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 1);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS, 0);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 0);
	
	vector<vector<unsigned short> > target;
	target.push_back(fprints[0]);
	target.push_back(fprints[1]);
	target.push_back(fprints[2]);
	target.push_back(fprints[56]);
	target.push_back(fprints[57]); 
	target.push_back(fprints[58]);
	target.push_back(fprints[91]);
	target.push_back(fprints[92]);
	target.push_back(fprints[108]);
	target.push_back(fprints[147]);
	
	vector<unsigned  int> m_indices;
	for (unsigned int i=0; i!=target.size(); ++i)
	{
		m_indices.push_back(i);
	}
	
	vector<pair<unsigned int, float> > nn_data;
	map<unsigned int, vector<unsigned int> > cluster_selection;
	
	BinaryFingerprintMethods bfm(options, target);
	bfm.averageLinkageClustering(m_indices, nn_data, cluster_selection);
	
	TEST_EQUAL(cluster_selection.size(), 3);
	
	TEST_EQUAL(cluster_selection[0][0], 3);
	TEST_EQUAL(cluster_selection[0][1], 4);
	TEST_EQUAL(cluster_selection[0][2], 5);
	TEST_EQUAL(cluster_selection[0][3], 8);
	TEST_EQUAL(cluster_selection[0][4], 9);
	TEST_EQUAL(cluster_selection[1][0], 0);
	TEST_EQUAL(cluster_selection[1][1], 1);
	TEST_EQUAL(cluster_selection[1][2], 2);
	TEST_EQUAL(cluster_selection[2][0], 6);
	TEST_EQUAL(cluster_selection[2][1], 7);
RESULT

CHECK(averageLinkageClustering( RNN_SWITCHED ))
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 2);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, 4);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS, 100);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 0);
	
	vector<unsigned  int> m_indices;
	m_indices.push_back(0);
	m_indices.push_back(1);
	m_indices.push_back(2);
	m_indices.push_back(56);
	m_indices.push_back(57);
	m_indices.push_back(58);
	m_indices.push_back(91);
	m_indices.push_back(92);
	m_indices.push_back(108);
	m_indices.push_back(147);
	
	vector<pair<unsigned int, float> > nn_data;
	map<unsigned int, vector<unsigned int> > cluster_selection;
	
	BinaryFingerprintMethods bfm(options, fprints);
	bfm.averageLinkageClustering(m_indices, nn_data, cluster_selection);
	
	TEST_EQUAL(cluster_selection.size(), 3);
	
	TEST_EQUAL(m_indices[cluster_selection[0][0]], 56);
	TEST_EQUAL(m_indices[cluster_selection[0][1]], 57);
	TEST_EQUAL(m_indices[cluster_selection[0][2]], 58);
	TEST_EQUAL(m_indices[cluster_selection[0][3]], 108);
	TEST_EQUAL(m_indices[cluster_selection[0][4]], 147);
	TEST_EQUAL(m_indices[cluster_selection[1][0]], 0);
	TEST_EQUAL(m_indices[cluster_selection[1][1]], 1);
	TEST_EQUAL(m_indices[cluster_selection[1][2]], 2);
	TEST_EQUAL(m_indices[cluster_selection[2][0]], 91);
	TEST_EQUAL(m_indices[cluster_selection[2][1]], 92);
RESULT

/////////////////////////////////////////////////////////////
END_TEST






























