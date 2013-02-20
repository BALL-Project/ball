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


// Format of fingerprint: 1 = Comma separated list of integer features, 2 = Fixed sized binay string
unsigned int fprint_format;

// Format of fingerprint: 1 = Comma separated list of integer features, 2 = Fixed sized binay string
unsigned int fixed_size_len;

// Limit of molecules to be read
unsigned int limit;

// Column number which contains the fingerprint
int fp_col;

// Column number which contains a identifier of the compounds
int id_col;

// True if input library file is in SD format
bool is_lib_sdf;

// SDF tag which contains the fingerprint
String fp_tag;

// SDF tag which contains a identifier of the compounds
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


void readMoleculeIdentifiers(unordered_map<unsigned int, set<String> >& mol_identifiers)
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
	
	unordered_map<unsigned int, set<String> > mol_identifiers;
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


int main(int argc, char* argv[])
{
	CommandlineParser parpars("FingerprintSimilarityClustering", "fast clustering of compounds using 2D binary fingerprints", VERSION, String(__DATE__), "Chemoinformatics");
	
	parpars.registerParameter("t", "Target library input file", INFILE, true);
	parpars.registerParameter("f", "Fingerprint format [1 = binary bitstring, 2 = comma separated feature list]", INT, true, "1");
	parpars.registerParameter("fp_col", "Column number for comma separated smiles input which contains the fingerprint", INT, false, -1);
	parpars.registerParameter("id_col", "Column number for comma separated smiles input which contains the molecule identifier", INT, false, -1);
	parpars.registerParameter("fp_tag", "Tag name for SDF input which contains the fingerprint", STRING, false, " ");
	parpars.registerParameter("id_tag", "Tag name for SDF input which contains the molecule identifier", STRING, false, " ");
	parpars.registerParameter("tc", "Tanimoto cutoff [default: 0.7]", DOUBLE, false, 0.7);
	parpars.registerParameter("cc", "Clustering size cutoff [default: 1000]", INT, false, 1000);
	parpars.registerParameter("l", "Number of fingerprints to read", INT, false, "0");
	parpars.registerParameter("nt", "Number of parallel threads to use. To use all possible threads enter <max> [default: 1]", STRING, false, "1");
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
		limit = Limits<unsigned int>::max();
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
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, 820);
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
	multimap<unsigned int, unsigned int> cc_sizes;
	vector<vector<pair<unsigned int, float> > > nn_data;
	
	// Add all input molecules for connected component calculation
	for (unsigned int i=0; i!=mol_features.size(); ++i)
	{
		m_indices.push_back(i);
	}
	
	BinaryFingerprintMethods bfm(options, mol_features);
	bool success = bfm.connectedComponents(m_indices, ccs, nn_data, sim_cutoff, true);
	
	if (!success)
	{
		Log.error() << "-- FAILED: fast compound clustering was not successful." << endl;
		Log.error() << endl;
		
		return 1;
	}
	
	// Determine sizes of connected components
	for (unsigned int i=0; i!=ccs.size(); ++i)
	{
		cc_sizes.insert(make_pair(ccs[i].size(), i));
	}
	
	writeConnectedComponents(m_indices, ccs, cc_sizes, nn_data, sim_cutoff);
	
	// ------------------------------------------------------------------------------------------
	// Clustering of connected components
	
	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ STEP 3: Average linkage clustering of connected components" << endl;
	
	vector<unsigned int> cl_indices;
	vector<vector<unsigned int> > clusters;
	map<unsigned int, vector<unsigned int> > cluster_selection;
	map<unsigned int, vector<unsigned int> >::iterator cl_iter;
	
	multimap<unsigned int, unsigned int>::iterator iter;
	for (iter=cc_sizes.begin(); iter!=cc_sizes.end(); ++iter)
	{
		if (iter->first > size_cutoff)
		{
			for (unsigned int i=0; i!=ccs[iter->second].size(); ++i)
			{
				cl_indices.push_back(m_indices[ccs[iter->second][i]]);
			}
			
			Log.level(10) << "++ CONNECTED COMPONENT SIZE: " << iter->first << endl;
			bfm.averageLinkageClustering(cl_indices, nn_data[iter->second], cluster_selection);
			Log.level(10) << "++"  << endl;
			
			for (cl_iter=cluster_selection.begin(); cl_iter!=cluster_selection.end(); ++cl_iter)
			{
				clusters.push_back(vector<unsigned int>());
				for (unsigned int i=0; i!=cl_iter->second.size(); ++i)
				{
					clusters[clusters.size() - 1].push_back(cl_indices[cl_iter->second[i]]);
				}
			}
			
			cl_indices.clear();
			cluster_selection.clear();
			nn_data[iter->second].clear();
		}
		else
		{
			nn_data[iter->second].clear();
			clusters.push_back(vector<unsigned int>());
			
			for (unsigned int i=0; i!=ccs[iter->second].size(); ++i)
			{
				clusters[clusters.size() -1 ].push_back(m_indices[ccs[iter->second][i]]);
			}
		}
	}
	
	// ------------------------------------------------------------------------------------------
	// Calculate Medoid of every cluster
	
	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ STEP 4: Calculate medoids for every cluster" << endl;
	
	bfm.setVerbosityLevel(0);
	
	unsigned int medoid_index;
	vector<float> tmp;
	vector<unsigned int> medoids;
	vector<vector<float> > avg_sims;
	
	for (unsigned int i=0; i!=clusters.size(); ++i)
	{
		if (bfm.calculateSelectionMedoid(clusters[i], medoid_index, tmp))
		{
			medoids.push_back(clusters[i][medoid_index]);
			avg_sims.push_back(tmp);
		}
		else
		{
			Log.error() << "-- WARNING: medoid calculation failed for unkown reason" << endl;
		}
	}
	
	
	// ------------------------------------------------------------------------------------------
	// Remap fingerprint duplicates
	
	Log.level(10) << "++ --------------------------------------------------------" << endl;
	Log.level(10) << "++ STEP 5: Remap fingerprint duplicates" << endl;
	
	
	String identifier;
	unordered_map<unsigned int, set<String> > identifiers;
	map<String, pair<unsigned int, pair<unsigned int, float> > > cluster_annotation;
	
	readMoleculeIdentifiers(identifiers);
	
	float avg_sim;
	unsigned int is_medoid;
	for (unsigned int i=0; i!=clusters.size(); ++i)
	{
		if (clusters[i].size() == 1)
		{
			is_medoid = 1;
			avg_sim = 1.0;
			
			for (set<String>::iterator id_iter=identifiers[clusters[i][0]].begin();id_iter!=identifiers[clusters[i][0]].end(); ++id_iter)
			{
				cluster_annotation[*id_iter] = make_pair(i+1, make_pair(is_medoid, avg_sim));
			}
		}
		else
		{
			for (unsigned int j=0; j!=clusters[i].size(); ++j)
			{
				if (clusters[i][j] == medoids[i])
				{
					is_medoid = 1;
				}
				else
				{
					is_medoid = 0;
				}
				
				avg_sim = avg_sims[i][j];
				
				for (set<String>::iterator id_iter=identifiers[clusters[i][j]].begin();id_iter!=identifiers[clusters[i][j]].end(); ++id_iter)
				{
					cluster_annotation[*id_iter] = make_pair(i+1, make_pair(is_medoid, avg_sim));
				}
			}
		}
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
		
		String tmp_nn, identifier;
		String cluster_tag = fp_tag + "_ClusterID";
		String medoid_tag = fp_tag + "_ClusterMedoid";
		String avg_sim_tag = fp_tag + "_AverageSim";
		Molecule* mol = lib_sdf->read();
		while (mol)
		{
			identifier = mol->getProperty(id_tag).getString();
			
			if (cluster_annotation.find(identifier)!=cluster_annotation.end())
			{
				mol->setProperty(cluster_tag, cluster_annotation[identifier].first);
				mol->setProperty(medoid_tag, cluster_annotation[identifier].second.first);
				mol->setProperty(avg_sim_tag, cluster_annotation[identifier].second.second);
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
		
		map<String, pair<unsigned int, pair<unsigned int, float> > >::iterator final_it = cluster_annotation.begin();
		for (; final_it!=cluster_annotation.end(); ++final_it)
		{
			out << final_it->first << " " << final_it->second.first << " " << final_it->second.second.first << " " << final_it->second.second.second << endl;
		}
		
		out.close();
	}
	
	
	Log.level(10) << "++" << endl;
	Log.level(10) << "++ DONE" << endl;
	Log.level(10) << "++" << endl;
	
	return 0;
}


























