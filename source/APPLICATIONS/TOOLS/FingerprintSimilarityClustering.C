// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/COMMON/limits.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/binaryFingerprintMethods.h>
#include <BALL/SYSTEM/sysinfo.h>

#include "version.h"

#include <boost/unordered_map.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/stream.hpp>

#include <locale>
#include <map>
#include <vector>


using namespace BALL;
using namespace boost;
using namespace std;


typedef map<unsigned int, map<unsigned int, vector<unsigned int> > > ClusterMap;
typedef map<unsigned int, map<unsigned int, vector<unsigned int> > >::iterator ClusterMapIterator;
typedef map<unsigned int, map<unsigned int, vector<unsigned int> > >::reverse_iterator ClusterMapReverseIterator;
typedef map<unsigned int, vector<pair<unsigned int, float> > > NNData;


// Fingerprint format
// 1: Comma separated list of integer features
// 2: Fixed-length binay string
unsigned int fprint_format;

// Fingerprint length
unsigned int fixed_size_len;

// Limit number of molecules to read
unsigned int limit;

// Column number of the fingerprint
int fp_col;

// Column number of a ompound identifier
int id_col;

// True iff input library file is in SD format
bool is_lib_sdf;

// SDF tag of the fingerprint
String fp_tag;

// SDF tag of a ompound identifier
String id_tag;


void readFingerprintsCSV(LineBasedFile& fprints_in, vector<vector<unsigned short> >& mol_features, vector<String>& mol_identifiers)
{
	String fprint;
	String identifier;
	vector<unsigned short> features;
	unsigned int mol_count = 0;
	
	mol_features.clear();
	mol_identifiers.clear();
	
	fprints_in.readLine();
	while (fprints_in.startsWith("#"))
	{
		fprints_in.readLine();
	}
	
	fprint = fprints_in.getField(fp_col).trim();
	while (!fprint.isEmpty())
	{
		if (id_col!=-1)
		{
			identifier = fprints_in.getField(id_col).trim();
		}
		else
		{
			identifier = String(mol_count);
		}
		
		if (mol_count == 0 && fprint_format == 2)
		{
			fixed_size_len = fprint.size();
		}
		
		if (BinaryFingerprintMethods::parseBinaryFingerprint(fprint, features, fprint_format))
		{
			mol_features.push_back(features);
			mol_identifiers.push_back(identifier);
		}
		else
		{
			Log.error() << "-- WARNING: Fingerprint could not be read" << endl;
		}
		
		++mol_count;
		
		if (mol_count == limit)
		{
			break;
		}
		
		fprints_in.readLine();
		fprint = fprints_in.getField(fp_col).trim();
	}
	
	Log.level(10) << "++ Molecules read:     " << mol_count << endl;
}


void readFingerprintsSDF(SDFile* fprints_in, vector<vector<unsigned short> >& mol_features, vector<String>& mol_identifiers)
{
	String fprint;
	String identifier;
	vector<unsigned short> features;
	unsigned int mol_count = 0;
	
	mol_features.clear();
	mol_identifiers.clear();
	
	Molecule* mol = fprints_in->read();
	while (mol)
	{
		if (mol->hasProperty(id_tag))
		{
			if (mol->hasProperty(fp_tag))
			{
				identifier = mol->getProperty(id_tag).getString();
				fprint = mol->getProperty(fp_tag).getString().trim();
				
				if (mol_count == 0 && fprint_format == 2)
				{
					fixed_size_len = fprint.size();
				}
				
				if (BinaryFingerprintMethods::parseBinaryFingerprint(fprint, features, fprint_format))
				{
					mol_features.push_back(features);
					mol_identifiers.push_back(identifier);
					
					++mol_count;
					
					if (mol_count==limit)
					{
						break;
					}
				}
			}
		}
		
		delete mol;
		mol = fprints_in->read();
	}
	delete mol;
	
	Log.level(10) << "++ Molecules read:     " << mol_count << endl;
}


void uniqueFingerprintsFilter(vector<vector<unsigned short> >& mol_features, const vector<String>& mol_identifiers)
{
	locale loc;
	const collate<char>& coll = use_facet<collate<char> >(loc);
	
	String fp;
	long fp_hash;
	
	map<long, unsigned int> fp_hash_map;
	vector<vector<unsigned int> > duplicates;
	for (unsigned int i=0; i!=mol_features.size(); ++i)
	{
		fp = "";
		for (unsigned int j=0; j!=mol_features[i].size(); ++j)
		{
			fp += String(mol_features[i][j]);
		}
		
		fp_hash = coll.hash(fp.data(), fp.data() + fp.length());
		
		if (fp_hash_map.find(fp_hash)==fp_hash_map.end())
		{
			fp_hash_map.insert(make_pair(fp_hash, duplicates.size()));
			duplicates.push_back(vector<unsigned int>(1, i));
		}
		else
		{
			duplicates[fp_hash_map[fp_hash]].push_back(i);
		}
	}
	
	
	File out("FFC_1_fingerprint_duplicates.txt", File::MODE_OUT);
	
	out << "# FFC FINGERPRINT DUPLICATES MAPPING" << endl;
	out << "# Eeach line is a blank separated list of identical fingerprints and has at least two columns." << endl;
	out << "# The first column is a global internal ID [0, n-1] where n is the number of unique fingerprints." << endl;
	out << "# The second and possibly following columns list the external molecule identifiers of the fingerprint duplicates. " << endl;
	
	vector<vector<unsigned short> > tmp_features;
	for (unsigned int i=0; i!=duplicates.size(); ++i)
	{
		// Keep first occurrence of new feature list
		tmp_features.push_back(mol_features[duplicates[i][0]]);
		
		out << tmp_features.size() - 1;
		
		for (unsigned int j=0; j!=duplicates[i].size(); ++j)
		{
			out << " " << mol_identifiers[duplicates[i][j]];
		}
		
		out << endl;
	}
	
	out.close();
	mol_features = tmp_features;
	
	return;
}


void readMoleculeIdentifiers(boost::unordered_map<unsigned int, set<String> >& mol_identifiers)
{
	File in("FFC_1_fingerprint_duplicates.txt", File::MODE_IN);
	
	String line;
	unsigned int parent;
	vector<String> line_split;
	
	while(getline(in, line))
	{
		if (!line.hasPrefix("#"))
		{
			line.split(line_split);
			
			parent = line_split[0].toInt();
			mol_identifiers.insert(make_pair(parent, set<String>()));
			mol_identifiers[parent].insert((++line_split.begin()), line_split.end());
		}
	}
	
	in.close();
}



void writeConnectedComponents(const vector<unsigned int>& m_indices, 
			      const vector<vector<unsigned int> >& ccs,
			      const multimap<unsigned int, unsigned int>& cc_sizes,
			      const vector<vector<pair<unsigned int, float> > >& nn_data, 
			      const float sim_cutoff)
{
	File out("FFC_2_connected_components.txt", File::MODE_OUT);
	
	out << "# FFC CONNECTED COMPONENTS" << endl;
	out << "# at similarity cutoff: " << sim_cutoff << endl;
	out << "# Connected components are separated by a '//' prefixed line." << endl;
	out << "# SRC_NODE_ID:    global internal (fingerprint unique) id of source node." << endl;
	out << "# DST_NODE_INDEX: index of nearest neighbour within connected component vector." << endl;
	out << "# TANIMOTO_SIM:   tanimoto similarity. If TANIMOTO_SIM == -1.0, no nearest neighbour information hass been calculated." << endl;
	out << "# CMPD_IDS:   Comma separated list of original compound id(s) which map onto this SRC_NODE_ID." << endl;
	out << "SRC_NODE_ID DST_NODE_INDEX TANIMOTO_SIM CMPD_IDS" << endl;
	
	boost::unordered_map<unsigned int, set<String> > mol_identifiers;
	readMoleculeIdentifiers(mol_identifiers);

	String cids;
	set<String>::iterator it;
	multimap<unsigned int, unsigned int>::const_reverse_iterator iter;
	for (iter=cc_sizes.rbegin(); iter!=cc_sizes.rend(); ++iter)
	{
		vector<unsigned int> cc = ccs[iter->second];
		vector<pair<unsigned int, float> > nnd = nn_data[iter->second];
		
		for (unsigned int i=0; i!=cc.size(); ++i)
		{
			it=mol_identifiers[m_indices[cc[i]]].begin();

			cids = *it;
			for (++it; it!=mol_identifiers[m_indices[cc[i]]].end(); ++it)
			{
				cids += "," + *it;
			}

			out << m_indices[cc[i]] << " " << m_indices[nnd[i].first] << " " << nnd[i].second << " " << cids << endl;
		}
		
		out << "//" << endl;
	}
	
	out.close();
}

void writeConnectedComponents(const vector<unsigned int>& m_indices,
							  const ClusterMap& clmap,
							  const NNData& nn,
							  const float sim_cutoff)
{
	File out("FFC_2_connected_components.txt", File::MODE_OUT);

	out << "# FFC CONNECTED COMPONENTS" << endl;
	out << "# at similarity cutoff: " << sim_cutoff << endl;
	out << "# Connected components are separated by a '//' prefixed line." << endl;
	out << "# SRC_NODE_ID:    global internal (fingerprint unique) id of source node." << endl;
	out << "# DST_NODE_INDEX: index of nearest neighbour within connected component vector." << endl;
	out << "# TANIMOTO_SIM:   tanimoto similarity. If TANIMOTO_SIM == -1.0, no nearest neighbour information hass been calculated." << endl;
	out << "# CMPD_IDS:   Comma separated list of original compound id(s) which map onto this SRC_NODE_ID." << endl;
	out << "SRC_NODE_INDEX DST_NODE_INDEX TANIMOTO_SIM CMPD_IDS" << endl;

	boost::unordered_map<unsigned int, set<String> > mol_identifiers;
	readMoleculeIdentifiers(mol_identifiers);

	set<String>::iterator it;
	ClusterMap::const_reverse_iterator clmap_iter = clmap.rbegin();
	for (; clmap_iter!=clmap.rend(); ++clmap_iter)
	{
		map<unsigned int, vector<unsigned int> >::const_iterator iter;
		for (iter=clmap_iter->second.begin(); iter!=clmap_iter->second.end(); ++iter)
		{
			vector<unsigned int> cc = iter->second;
			vector<pair<unsigned int, float> > nn_tmp = (nn.find(iter->first))->second;

			for (unsigned int i=0; i!=cc.size(); ++i)
			{
				it = mol_identifiers[m_indices[cc[i]]].begin();

				String cids = *it;
				for (++it; it!=mol_identifiers[m_indices[cc[i]]].end(); ++it)
				{
					cids += "," + *it;
				}

				out << m_indices[cc[i]] << " " << m_indices[nn_tmp[i].first] << " " << nn_tmp[i].second << " " << cids << endl;
			}

			out << "//" << endl;
		}
	}

	out.close();

	return;
}


bool readFingerprints(const String& input_file, vector<vector<unsigned short> >& mol_features, vector<String>& mol_identifiers)
{
	if (!MolFileFactory::isFileExtensionSupported(input_file))
	{
		// Assuming space separated CSV file which contains fingerprints
		
		if (fp_col == -1)
		{
			Log.error() << "-- FAILED: For comma separated smiles input files please specify the column number (fp_col > 0) which contains the fingerprints." << endl;
			Log.error() << endl;
			
			return false;
		}
		
		if (input_file.hasSuffix(".gz"))
		{
			String tmp_unzipped;
			File::createTemporaryFilename(tmp_unzipped);
			LineBasedFile unzipped(tmp_unzipped, File::MODE_OUT);
			File zipped(input_file, File::MODE_IN | File::MODE_BINARY);
			
			iostreams::filtering_streambuf<iostreams::input> gzip_in;
			gzip_in.push(iostreams::gzip_decompressor());
			gzip_in.push(zipped);
			iostreams::copy(gzip_in, unzipped);
			
			zipped.close();
			
			unzipped.reopen(File::MODE_IN);
			readFingerprintsCSV(unzipped, mol_features, mol_identifiers);
			unzipped.close();
			
			File::remove(tmp_unzipped);
		}
		else
		{
			LineBasedFile lbf(input_file, File::MODE_IN);
			
			readFingerprintsCSV(lbf, mol_features, mol_identifiers);
			
			lbf.close();
		}
	}
	else
	{
		if (fp_tag == " ")
		{
			Log.error() << "-- FAILED: For SDF input files please specify the tag which contains the fingerprints." << endl;
			Log.error() << endl;
			
			return false;
		}
		if (id_tag == " ")
		{
			Log.error() << "-- FAILED: For SDF input files please specify the tag which contains the molecule identifier." << endl;
			Log.error() << endl;
			
			return false;
		}
		
		GenericMolFile* lib = MolFileFactory::open(input_file, File::MODE_IN);
		if (lib == NULL)
		{
			Log.error() << "-- FAILED: Specified library input could not be opened." << endl;
			Log.error() << "--" << endl;
			
			return false;
		}
		
		SDFile* lib_sdf = dynamic_cast<SDFile*>(lib);
		if (lib_sdf)
		{
			is_lib_sdf = true;
			lib_sdf->disableAtoms();
			
			readFingerprintsSDF(lib_sdf, mol_features, mol_identifiers);
			
			lib_sdf->close();
			delete lib_sdf;
		}
		else
		{
			Log.error() << "-- FAILED: Input file has an unsupported format." << endl;
			Log.error() << endl;
			
			lib->close();
			delete lib;
			
			return false;
		}
	}
	
	if (mol_features.empty())
	{
		Log.error() << "-- FAILED: Input file empty or wrong fingerprint tags or columns specified." << endl;
		Log.error() << endl;
		
		return false;
	}
	
	return true;
}


bool connectedComponentsMemoryEstimation(const unsigned int n_items, const unsigned int n_threads, const unsigned int b_size)
{
	LongIndex threaded_nn_data_size = (n_threads + 1) * n_items * ( sizeof(unsigned int) + sizeof(float) );
	LongIndex cc_matrices_size = n_threads * sizeof(unsigned short) * b_size * (b_size + 1);
	LongIndex mem_Bytes = threaded_nn_data_size + cc_matrices_size;
	LongIndex total_mem = SysInfo::getTotalMemory();
	
	bool proceed = true;
	if (total_mem == -1 || mem_Bytes / (double)total_mem > 0.5)
	{
		Log << "\n++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++" << endl;
		Log << "++ WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING" << endl;
		Log << "++" << endl;
		Log << "++ The application will use at least >> " << (mem_Bytes / 1.074e+9) << " GB << of memory" << endl;
		Log << "++ DO YOU WANT TO PROCEED AT YOUR OWN RISK? [yes | no]" << endl;
		Log << "++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++\n" << endl;
		
		String proceed = "no";
		getline(cin, proceed);
		
		String decision = "no";
		getline(cin, decision);
		
		if (decision != "yes")
		{
			proceed = false;
		}
	}
	
	return proceed;
}


bool clusteringMemoryEstimation(const LongSize cc_max, const unsigned int n_threads, const unsigned int b_size)
{
	LongIndex cluster_data_size = (2 * cc_max - 1)  * 100;
	LongIndex thread_data_size = (n_threads + 1) * cc_max * ( sizeof(unsigned int) + sizeof(float) );
	thread_data_size += n_threads * sizeof(unsigned short) * b_size * (b_size + 1);
	thread_data_size += (n_threads) * 100 * cc_max * sizeof(double);
	LongIndex mem_Bytes = cluster_data_size + thread_data_size;
	LongIndex total_mem = SysInfo::getTotalMemory();
	
	bool proceed = true;
	if (total_mem == -1 || mem_Bytes / (double)total_mem > 0.5)
	{
		Log << "\n++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++" << endl;
		Log << "++ WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING" << endl;
		Log << "++" << endl;
		Log << "++ The largest connected component has >> " << cc_max << " members <<" << endl;
		Log << "++ Clustering of this component using current settings will use at least >> " << (mem_Bytes / 1.074e+9) << " GB << of memory" << endl;
		Log << "++ DO YOU WANT TO PROCEED AT YOUR OWN RISK? [yes | no]" << endl;
		Log << "++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++\n" << endl;
		
		String decision = "no";
		getline(cin, decision);
		
		if (decision != "yes")
		{
			proceed = false;
		}
	}
	
	return proceed;
}


unsigned int getIDMin(vector<unsigned int>& ids)
{
	unsigned int min = UINT_MAX;
	for (unsigned int i=0; i!=ids.size(); ++i)
	{
		if (ids[i] < min)
		{
			min = ids[i];
		}
	}

	return min;
}


int main(int argc, char* argv[])
{
	CommandlineParser parpars("FingerprintSimilarityClustering", "fast clustering of compounds using 2D binary fingerprints", VERSION, String(__DATE__), "Chemoinformatics");
	
	parpars.registerMandatoryInputFile("t", "Target library input file");
	parpars.registerMandatoryIntegerParameter("f", "Fingerprint format [1 = binary bitstring, 2 = comma separated feature list]");
	parpars.registerOptionalIntegerParameter("fp_col", "Column number for comma separated smiles input which contains the fingerprint", -1);
	parpars.registerOptionalIntegerParameter("id_col", "Column number for comma separated smiles input which contains the molecule identifier", -1);
	parpars.registerOptionalStringParameter("fp_tag", "Tag name for SDF input which contains the fingerprint", " ");
	parpars.registerOptionalStringParameter("id_tag", "Tag name for SDF input which contains the molecule identifier", " ");
	parpars.registerOptionalDoubleParameter("tc", "Tanimoto cutoff [default: 0.7]", 0.7);
	parpars.registerOptionalIntegerParameter("cc", "Clustering size cutoff [default: 1000]", 1000);
	parpars.registerOptionalIntegerParameter("l", "Number of fingerprints to read", 0);
	parpars.registerOptionalStringParameter("nt", "Number of parallel threads to use. To use all possible threads enter <max> [default: 1]", "1");
	parpars.registerFlag("sdf_out", "If input file has SD format, this flag activates writing of clustering information as new tags in a copy of the input SD file.");
	
	parpars.setSupportedFormats("t","smi, smi.gz, csv, csv.gz, txt, txt.gz, sdf, sdf.gz");
	parpars.setParameterRestrictions("f", 1, 2);
	
	String man = "This tool performs a fast and deterministic semi-hierarchical clustering of input compounds encoded as 2D binary fingerprints.\n\n\
The method is a multistep workflow which first reduces the number of input fingerprints by removing duplicates. This unique set is forwarded to connected\n\
components decomposition by calculating all pairwise Tanimoto similarities and application of a similarity cutoff value. As a third step, all connected components\n\
which exceed a predefined size are hierarchically clustered using the average linkage clustering criterion. The Kelley method is applied on every hierarchical clustering\n\
to determine a level for cluster selection. Finally, the fingerprint duplicates are remapped onto the final clusters which contain their representatives. \n\n\
For every final cluster a medoid is calulated. For a single cluster multiple medoids are possible because fingerprint duplicates of a medoid are also marked as medoid.\n\n\
For every compound the output yields a cluster ID, a medoid tag where '1' indicates the cluster medoid(s) and the average similarity of the compound to all other \n\
cluster members. If the output format is SD, these properties are added as new tags.\n\n\
======================================================================================================================================================\n\n\
Examples:\n\n\
$ FingerprintSimilarityClustering -t target.sdf -fp_tag FPRINT -f 1 -id_tag NAME\n\
  tries to read fingerprints as binary bitstrings (-f 1) from tag <FPRINT> and compound IDs from tag <NAME> of target.sdf input file.\n\
  The clustering workflow described is executed on the input molecules with default values.\n\n\
$ FingerprintSimilarityClustering -t target.csv -fp_col 3 -f 2 -id_col 1\n\
  tries to read fingerprints as comma separated integer feature list (-f 2) from column 3 and IDs from column 1 out of a space separated CSV file.\n\
  The clustering workflow described is executed on the input molecules with default values.\n\n\
$ FingerprintSimilarityClustering -t target.sdf -fp_tag FPRINT -f 1 -id_tag NAME -nt max\n\
  Same as first example but executed in parallel mode using as many threads as available.\n\n\
$ FingerprintSimilarityClustering -t target.sdf -fp_tag FPRINT -f 1 -id_tag NAME -tc 0.5 -cc 50\n\
  Same as first example but using modified parameters for similarity network generation (tc 0.5) and size of connected components to be clustered (-cc 50).";
	
	parpars.setToolManual(man);
	parpars.parse(argc, argv);
	
	// Set read and parameters
	fprint_format = parpars.get("f").toInt();
	float sim_cutoff = parpars.get("tc").toFloat();
	unsigned int size_cutoff = parpars.get("cc").toInt();
	
	limit = parpars.get("l").toInt();
	if (limit == 0)
	{
		limit = std::numeric_limits<unsigned int>::max();
	}
	
	unsigned int n_threads = 1;
	if (parpars.get("nt") != "1")
	{
		if (parpars.get("nt") == "max")
		{
			n_threads = SysInfo::getNumberOfProcessors();
		}
		else
		{
			if (parpars.get("nt").toInt() > SysInfo::getNumberOfProcessors())
			{
				n_threads = SysInfo::getNumberOfProcessors();
				Log.info() << "++ INFO: Specified number of threads exceeds available threads. Setting number to available threads." << endl;
			}
			else
			{
				n_threads = parpars.get("nt").toInt();
			}
		}
	}
	
	if (parpars.get("fp_col") != "-1")
	{
		fp_col = parpars.get("fp_col").toInt() - 1;
	}
	else
	{
		fp_col = -1;
	}
	if (parpars.get("id_col") != "-1")
	{
		id_col = parpars.get("id_col").toInt() - 1;
	}
	else
	{
		id_col = -1;
	}
	
	if (parpars.get("fp_tag") != " ")
	{
		fp_tag = parpars.get("fp_tag");
	}
	if (parpars.get("id_tag") != " ")
	{
		id_tag = parpars.get("id_tag");
	}
	
	// Read library fingerprints
	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ Read target library ... " << endl;
	
	is_lib_sdf = false;
	vector<String> mol_identifiers;
	vector<vector<unsigned short> > mol_features;
	
	bool read_success = readFingerprints(parpars.get("t"), mol_features, mol_identifiers);
	
	if (!read_success)
	{
		return 1;
	}
	
	Log.level(10) << "++" << endl;
	
	
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 500);
	options.setDefaultReal(BinaryFingerprintMethods::Option::SIM_CUTOFF, sim_cutoff);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, n_threads);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::MAX_CLUSTERS, 1000);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 15);
	
	
	// ------------------------------------------------------------------------------------------
	// Create unique fingerprint set
	
	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ STEP 1: Unique Fingerprints Filter" << endl;
	Log.level(10) << "++ Fingerprints total : " << mol_features.size() << endl;
	
	uniqueFingerprintsFilter(mol_features, mol_identifiers);
	mol_identifiers.clear();
	
	Log.level(10) << "++ Fingerprints unique: " <<  mol_features.size() << endl;
	Log.level(10) << "++" << endl;
	
	
	// ------------------------------------------------------------------------------------------
	// Connected components decomposition
	
	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ STEP 2: Connected components decomposition" << endl;

	vector<unsigned int> m_indices;
	vector<vector<unsigned int> > ccs;
	vector<vector<pair<unsigned int, float> > > nn_tmp;

	// Add all input molecules for connected component calculation
	for (unsigned int i=0; i!=mol_features.size(); ++i)
	{
		m_indices.push_back(i);
	}

	// MAKE ESTIMATION FOR SYSTEM RESOURCE DEMANDS
	bool proceed = connectedComponentsMemoryEstimation(m_indices.size(), n_threads, options.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE));
	if (!proceed)
	{
		Log << "++" << endl;
		Log << "++ DONE" << endl;
		Log << "++" << endl;

		return 0;
	}


	BinaryFingerprintMethods bfm(options, mol_features);

	bool success = bfm.connectedComponents(m_indices, ccs, nn_tmp, sim_cutoff, true);

	if (!success)
	{
		Log.error() << "-- FAILED: fast compound clustering was not successful." << endl;
		Log.error() << endl;

		return 1;
	}


	// Store connected components in final cluster data structure
	// Additionally map the nearest neighbour data appropriately.

	ClusterMap clmap;
	NNData nn;

	for (unsigned int i=0; i!=ccs.size(); ++i)
	{
		unsigned int size = ccs[i].size();
		unsigned int idmin = getIDMin(ccs[i]);

		// Insert connected components into ClusterMap
		map<unsigned int, vector<unsigned int> >& sizemap = clmap[size];
		sizemap[idmin] = ccs[i];

		// Insert nearest neighbour data
		nn.insert(make_pair(idmin, nn_tmp[i]));
	}

	// Write connected components to file
	writeConnectedComponents(m_indices, clmap, nn, sim_cutoff);

	ccs.clear();
	nn_tmp.clear();


	// ------------------------------------------------------------------------------------------
	// Clustering of connected components

	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ STEP 3: Average linkage clustering of connected components" << endl;

	// MAKE ESTIMATION FOR SYSTEM RESOURCE DEMANDS
	LongSize cc_max = clmap.rbegin()->first;
	proceed = clusteringMemoryEstimation(cc_max, n_threads, options.getInteger(BinaryFingerprintMethods::Option::BLOCKSIZE));
	if (!proceed)
	{
		Log << "++" << endl;
		Log << "++ DONE" << endl;
		Log << "++" << endl;

		return 0;
	}

	ClusterMap tmp_clmap;
	vector<unsigned int> cl_indices;
	map<unsigned int, vector<unsigned int> > cluster_selection;

	// Connected components that will be clustered must subsequently be removed from the main ClusterMap (clmap).
	// The corresponding keys are stored here:
	map<unsigned int, vector<unsigned int> > remove;

	for (ClusterMapIterator size_iter=clmap.begin(); size_iter!=clmap.end(); ++size_iter)
	{
		for (map<unsigned int, vector<unsigned int> >::iterator cl_iter=size_iter->second.begin(); cl_iter!=size_iter->second.end(); ++cl_iter)
		{
			if (size_iter->first > size_cutoff)
			{
				vector<unsigned int>& tmp_cl = cl_iter->second;

				cl_indices.clear();
				for (unsigned int i=0; i!=tmp_cl.size(); ++i)
				{
					cl_indices.push_back(m_indices[tmp_cl[i]]);
				}

				Log.level(10) << "++ CONNECTED COMPONENT SIZE: " << size_iter->first << endl;
				bfm.averageLinkageClustering(cl_indices, nn[cl_iter->first], cluster_selection);
				Log.level(10) << "++"  << endl;

				// Retrieve every cluster
				for (map<unsigned int, vector<unsigned int> >::iterator sel_iter=cluster_selection.begin(); sel_iter!=cluster_selection.end(); ++sel_iter)
				{
					unsigned int idmin = UINT_MAX;

					tmp_cl.clear();
					for (unsigned int i=0; i!=sel_iter->second.size(); ++i)
					{
						tmp_cl.push_back(cl_indices[sel_iter->second[i]]);

						if (tmp_cl[tmp_cl.size() - 1] < idmin)
						{
							idmin = tmp_cl[tmp_cl.size() - 1];
						}
					}

					// Insert clusters into tmp_map
					map<unsigned int, vector<unsigned int> >& sizemap = tmp_clmap[tmp_cl.size()];
					sizemap[idmin] = tmp_cl;
				}

				// Store just processed connected component as remove candidate from main clmap
				vector<unsigned int>& tmp = remove[size_iter->first];
				tmp.push_back(cl_iter->first);
			}

			nn.erase(cl_iter->first);
		}
	}


	// Update main ClusterMap clmap in two steps:
	// Step 1: remove connected components that were clustered
	for (map<unsigned int, vector<unsigned int> >::iterator it=remove.begin(); it!=remove.end(); ++it)
	{
		map<unsigned int, vector<unsigned int> >& tmp = clmap[it->first];
		for (unsigned int i=0; i!=it->second.size(); ++i)
		{
			tmp.erase(it->second[i]);
		}

		if (tmp.empty())
		{
			clmap.erase(it->first);
		}
	}

	// Step 2: insert new clusters
	for (ClusterMap::iterator it=tmp_clmap.begin(); it!=tmp_clmap.end(); ++it)
	{
		map<unsigned int, vector<unsigned int> >& tmp = clmap[it->first];
		tmp.insert(it->second.begin(), it->second.end());
	}

	// Clean up
	tmp_clmap.clear();


	// ------------------------------------------------------------------------------------------
	// Calculate Medoid of every cluster

	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ STEP 4: Calculate medoids for every cluster" << endl;

	bfm.setVerbosityLevel(0);

	unsigned int medoid_index;
	vector<float> tmp_avg_sims;
	map<unsigned int, pair<unsigned int, vector<float> > > medoids_avg_sims;

	for (ClusterMap::iterator size_iter=clmap.begin(); size_iter!=clmap.end(); ++size_iter)
	{
		for (map<unsigned int, vector<unsigned int> >::iterator cl_iter=size_iter->second.begin(); cl_iter!=size_iter->second.end(); ++cl_iter)
		{
			if (bfm.calculateSelectionMedoid(cl_iter->second, medoid_index, tmp_avg_sims))
			{
				medoids_avg_sims[cl_iter->first] = make_pair(medoid_index, tmp_avg_sims);
				tmp_avg_sims.clear();
			}
			else
			{
				Log.error() << "-- WARNING: medoid calculation failed for unkown reason" << endl;
			}
		}
	}


	// ------------------------------------------------------------------------------------------
	// Remap fingerprint duplicates

	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ STEP 5: Remap fingerprint duplicates" << endl;


	boost::unordered_map<unsigned int, set<String> > identifiers;
	readMoleculeIdentifiers(identifiers);

	// Store final cluster information
	map<String, pair<unsigned int, pair<unsigned int, float> > > final_clusters;

	unsigned int is_medoid;
	unsigned int cluster_id = 1;
	for (ClusterMap::iterator size_iter=clmap.begin(); size_iter!=clmap.end(); ++size_iter)
	{
		for (map<unsigned int, vector<unsigned int> >::iterator cl_iter=size_iter->second.begin(); cl_iter!=size_iter->second.end(); ++cl_iter)
		{

			if (cl_iter->second.size() == 1)
			{
				// Singleton cluster

				is_medoid = 1;
				float avg_sim = 1.0;

				for (set<String>::iterator id_iter=identifiers[cl_iter->second[0]].begin(); id_iter!=identifiers[cl_iter->second[0]].end(); ++id_iter)
				{
					final_clusters[*id_iter] = make_pair(cluster_id, make_pair(is_medoid, avg_sim));
				}
			}
			else
			{
				medoid_index = medoids_avg_sims[cl_iter->first].first;
				tmp_avg_sims = medoids_avg_sims[cl_iter->first].second;

				for (unsigned int i=0; i!=cl_iter->second.size(); ++i)
				{
					if (i == medoid_index)
					{
						is_medoid = 1;
					}
					else
					{
						is_medoid = 0;
					}

					for (set<String>::iterator id_iter=identifiers[cl_iter->second[i]].begin(); id_iter!=identifiers[cl_iter->second[i]].end(); ++id_iter)
					{
						final_clusters[*id_iter] = make_pair(cluster_id, make_pair(is_medoid, tmp_avg_sims[i]));
					}
				}
			}

			++cluster_id;
			cl_iter->second.clear();
		}

		size_iter->second.clear();
	}


	// ------------------------------------------------------------------------------------------
	// Write final clustering
	
	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ STEP 6: Write final clustering" << endl;
	
	if (parpars.has("sdf_out") && is_lib_sdf)
	{
		GenericMolFile *lib = MolFileFactory::open(parpars.get("t"), File::MODE_IN);
		SDFile *lib_sdf = dynamic_cast<SDFile*>(lib);
		
		GenericMolFile* tmp_out = MolFileFactory::open("FFC_3_final_clustering.sdf", File::MODE_OUT);
		SDFile* out_sdf = dynamic_cast<SDFile*>(tmp_out);
		
		String identifier;
		String cluster_tag = fp_tag + "_ClusterID";
		String medoid_tag = fp_tag + "_ClusterMedoid";
		String avg_sim_tag = fp_tag + "_AverageSim";
		Molecule* mol = lib_sdf->read();
		while (mol)
		{
			identifier = mol->getProperty(id_tag).getString();
			
			if (final_clusters.find(identifier)!=final_clusters.end())
			{
				mol->setProperty(cluster_tag, final_clusters[identifier].first);
				mol->setProperty(medoid_tag, final_clusters[identifier].second.first);
				mol->setProperty(avg_sim_tag, final_clusters[identifier].second.second);
			}
			else
			{
				mol->setProperty(cluster_tag, "");
				mol->setProperty(medoid_tag, 0);
				mol->setProperty(avg_sim_tag, 0.0);
			}
			
			out_sdf->write(*mol);
			
			delete mol;
			mol = lib_sdf->read();
		}
		
		out_sdf->close();
		delete out_sdf;
		
		lib_sdf->close();
		delete lib_sdf;
	}
	else
	{
		File out("FFC_3_final_clustering.csv", File::MODE_OUT);
		
		out << "# FFC FINAL CLUSTERING" << endl;
		out << "# Similarity cutoff: " << sim_cutoff << endl;
		out << "# Size cutoff: " << size_cutoff << endl;
		out << "# MolID:         external molecule identifier." << endl;
		out << "# ClusterID:     cluster identifier is a integer value [1-n] where n is the total number of clusters." << endl;
		out << "# ClusterMedoid: 1 = molecule is medoid of its cluster. 0 = not medoid of cluster." << endl;
		out << "#                Multiple medoids are possible due to duplicate fingerprints. All duplicates of a medoid are also marked as medoids." << endl;
		out << "# AverageSim:    Average similarity of fingerprint to all others in cluster." << endl;
		out << "MolID ClusterID ClusterMedoid AverageSim" << endl;
		
		map<String, pair<unsigned int, pair<unsigned int, float> > >::iterator final_it = final_clusters.begin();
		for (; final_it!=final_clusters.end(); ++final_it)
		{
			out << final_it->first << " " << final_it->second.first << " " << final_it->second.second.first << " " << final_it->second.second.second << endl;
		}
		
		out.close();
	}


	Log << "++" << endl;
	Log << "++ DONE" << endl;
	Log << "++" << endl;
	
	return 0;
}


























