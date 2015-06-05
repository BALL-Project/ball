// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/COMMON/limits.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/binaryFingerprintMethods.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/SYSTEM/sysinfo.h>

#include "version.h"

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/stream.hpp>

#include <map>
#include <vector>


using namespace BALL;
using namespace boost;
using namespace std;


// Format of fingerprint: 1 = Comma separated list of integer features, 2 = Fixed sized binay string
unsigned int fprint_format;

// Format of fingerprint: 1 = Comma separated list of integer features, 2 = Fixed sized binay string
unsigned int fixed_size_len;


// Column number which contains the fingerprint
int fp_col;

// Column number which contains a identifier of the compounds
int id_col;

// True if input query file is in SD format
bool is_query_sdf;

// SDF tag which contains the fingerprint
String fp_tag;

// SDF tag which contains a identifier of the compounds
String id_tag;


void readFingerprintsCSV(LineBasedFile& fprints_in, vector<vector<unsigned short> >& molecule_features, vector<String>& identifiers)
{
	String fprint;
	String identifier;
	vector<unsigned short> features;
	unsigned int mol_count = 0;
	
	molecule_features.clear();
	identifiers.clear();
	
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
			molecule_features.push_back(features);
			identifiers.push_back(identifier);
		}
		else
		{
			Log.error() << "-- WARNING: Fingerprint could not be read" << endl;
		}
		
		++mol_count;
		
		fprints_in.readLine();
		fprint = fprints_in.getField(fp_col).trim();
	}
	
	Log.level(10) << "++ Molecules read:     " << mol_count << endl;
}


void readFingerprintsSDF(SDFile* fprints_in, vector<vector<unsigned short> >& molecule_features, vector<String>& identifiers)
{
	String fprint;
	String identifier;
	vector<unsigned short> features;
	unsigned int mol_count = 0;
	
	molecule_features.clear();
	identifiers.clear();
	
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
					molecule_features.push_back(features);
					identifiers.push_back(identifier);
					
					++mol_count;
				}
			}
		}
		
		delete mol;
		mol = fprints_in->read();
	}
	delete mol;
	
	Log.level(10) << "++ Molecules read:     " << mol_count << endl;
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
			is_query_sdf = true;
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
	CommandlineParser parpars("FingerprintSimilaritySearch", "calculate similar molecules in a library", VERSION, String(__DATE__), "Chemoinformatics");
	
	parpars.registerMandatoryInputFile("t", "Target library input file");
	parpars.registerMandatoryInputFile("q", "Query library input file");
	parpars.registerMandatoryOutputFile("o", "Result output file");
	parpars.registerMandatoryIntegerParameter("f", "Fingerprint format [1 = binary bitstring, 2 = comma separated feature list]");
	parpars.registerOptionalIntegerParameter("fp_col", "Column number for comma separated smiles input which contains the fingerprint", -1);
	parpars.registerOptionalIntegerParameter("id_col", "Column number for comma separated smiles input which contains the molecule identifier", -1);
	parpars.registerOptionalStringParameter("fp_tag", "Tag name for SDF input which contains the fingerprint", " ");
	parpars.registerOptionalStringParameter("id_tag", "Tag name for SDF input which contains the molecule identifier", " ");
	parpars.registerOptionalDoubleParameter("tc", "Tanimoto cutoff [default: 0.7]", 0.7);
	parpars.registerOptionalStringParameter("nt", "Number of parallel threads to use. To use all possible threads enter <max> [default: 1]", "1");
	parpars.registerOptionalIntegerParameter("bs", "Block size [default: 500]", 500);
	parpars.registerFlag("sdf_out", "If query file has SD format, this flag activates writing of nearest neighbours as a new CSV tag in a copy of the query SD file.");
	
	parpars.setParameterRestrictions("f", 1, 2);
	parpars.setSupportedFormats("t","smi, smi.gz, csv, csv.gz, txt, txt.gz, sdf, sdf.gz");
	parpars.setSupportedFormats("q","smi, smi.gz, csv, csv.gz, txt, txt.gz, sdf, sdf.gz");
	parpars.setSupportedFormats("o","smi, smi.gz, csv, csv.gz, txt, txt.gz, sdf, sdf.gz");
	
	String man = "This tool calculates all nearest neighbours above a similarity cutoff for given query molecules in a compound library on the basis of 2D binary fingerprints.\n\
The first library to specify (i1) is the compound library to be searched, the second library (i2) is conseiderd as the query compounds.\n\
Both files have to be comma separated values (csv) files and the binary fingerprints have to be encoded as feature lists or as binary bit strings.\n\n\
WARNING: If similarity cutoff is chosen to be 0.0, the output will be the entire similarity matrix and has a size of n*m with n=|i1| and m=|i2|. \n\n\
======================================================================================================================================================\n\n\
Examples:\n\n\
$ FingerprintSimilaritySearch -t target.sdf -q query.sdf -o results -fp_tag FPRINT -f 1 -id_tag NAME\n\
  tries to extract fingerprints as binary bitstrings (-f 1) from tag <FPRINT> and compound IDs from tag <NAME> of target.sdf and query.sdf.\n\
  A similarity search is performed for all query molecules against all target molecules and pairs with similarity above Tanimoto cutoff 0.7 are written to outfile (results).\n\n\
$ FingerprintSimilaritySearch -t target.sdf -q query.sdf -o results -fp_tag FPRINT -f 1 -id_tag NAME -sdf_out\n\
  tries to extract fingerprints as binary bitstrings (-f 1) from tag <FPRINT> and compound IDs from tag <NAME> of target.sdf and query.sdf.\n\
  A similarity search is performed for all query molecules against all target molecules and pairs with similarity above Tanimoto cutoff 0.7\n\
  are added as a new SD tag to output file 'NN_TAGGED_query.sdf' as a list of TargetID:Similarity pairs.\n\n\
$ FingerprintSimilaritySearch -t target.sdf -q query.smi -o results -fp_tag FPRINT -f 1 -id_tag NAME -fp_col 2\n\
  tries to extract fingerprints as binary bitstrings (-f 1) from tag <FPRINT> and compound IDs from tag <NAME> of target.sdf\n\
  and fingerprints as binary bitstrings of space separated query file from column 2 (-fp_col 2).\n\
  A similarity search is performed for all query molecules against all target molecules and pairs with similarity above Tanimoto cutoff 0.7 are written to outfile (results).";
	
	parpars.setToolManual(man);
	parpars.parse(argc, argv);
	
	// Set read and parameters
	fprint_format = parpars.get("f").toInt();
	float sim_cutoff = parpars.get("tc").toFloat();
	unsigned int bs = parpars.get("bs").toInt();

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
	
	if (File::isAccessible(parpars.get("o")))
	{
		Log.error() << "-- FAILED: Specified output file already exists: " << parpars.get("o") << endl;
		Log.error() << endl;
		
		return 1;
	}
	
	// Read library fingerprints
	Log.level(10) << "++" << endl;
	Log.level(10) << "++ Read target library ... " << endl;
	
	bool read_success;
	vector<String> lib_identifiers;
	vector<vector<unsigned short> > lib_features;
	
	read_success = readFingerprints(parpars.get("t"), lib_features, lib_identifiers);
	
	if (!read_success)
	{
		return 1;
	}
	
	
	// Read query fingerprints
	Log.level(10) << "++ Read query library ... " << endl;
	
	is_query_sdf = false;
	vector<String> query_identifiers;
	vector<vector<unsigned short> > query_features;
	
	read_success = readFingerprints(parpars.get("q"), query_features, query_identifiers);
	
	if (!read_success)
	{
		return 1;
	}
	Log.level(10) << "++" << endl;
	
	Options options;
	options.setDefaultInteger(BinaryFingerprintMethods::Option::BLOCKSIZE, bs);
	options.setDefaultReal(BinaryFingerprintMethods::Option::SIM_CUTOFF, sim_cutoff);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::N_THREADS, n_threads);
	options.setDefaultInteger(BinaryFingerprintMethods::Option::VERBOSITY, 6);
	
	
	String outfile_name = "_BALL_CUTOFF_SEARCH.tmp";
	
	BinaryFingerprintMethods bfm(options, lib_features, query_features);
	bool success = bfm.cutoffSearch(sim_cutoff, outfile_name);
	
	if (!success)
	{
		Log.error() << "-- FAILED: Similarity calculations not successful." << endl;
		Log.error() << endl;
		
		File::remove(outfile_name);
		
		return 1;
	}
	
	LineBasedFile outfile;
	try
	{
		outfile.open(outfile_name, File::MODE_IN);
	}
	catch (Exception::FileNotFound& e)
	{
		Log.error() << "-- FAILED: Similarity calculations not successful." << endl;
		Log.error() << endl;
		
		return 1;
	}
	
	String query_index, lib_index;
	map<String, map<String, String> > result;
	
	while (outfile.readLine())
	{
		query_index = query_identifiers[outfile.getField(0).toUnsignedInt()];
		lib_index = lib_identifiers[outfile.getField(1).toUnsignedInt()];
		
		if (result.find(query_index)==result.end())
		{
			result.insert(make_pair(query_index, map<String, String>()));
		}
		result[query_index].insert(make_pair(lib_index, outfile.getField(2)));
	}
	outfile.close();
	File::remove(outfile_name);
	
	if (parpars.has("sdf_out") && is_query_sdf)
	{
		GenericMolFile *query = MolFileFactory::open(parpars.get("q"), File::MODE_IN);
		SDFile *query_sdf = dynamic_cast<SDFile*>(query);
		
		String sdf_out_name = "NN_TAGGED_" + parpars.get("q");
		GenericMolFile* tmp_out = MolFileFactory::open(sdf_out_name, File::MODE_OUT);
		SDFile* out_sdf = dynamic_cast<SDFile*>(tmp_out);
		
		String tmp_nn, identifier;
		String nn_tag_name = fp_tag + "_NearestNeighbours:Tanimoto";
		Molecule* mol = query_sdf->read();
		while (mol)
		{
			identifier = mol->getProperty(id_tag).getString();
			
			if (result.find(identifier)!=result.end())
			{
				map<String, String>::iterator nn_iter = result[identifier].begin();
				tmp_nn = nn_iter->first + ":" + nn_iter->second;
				++nn_iter;
				for (; nn_iter!=result[identifier].end(); ++nn_iter)
				{
					tmp_nn +=  "," + nn_iter->first + ":" + nn_iter->second;
				}
				
				mol->setProperty(nn_tag_name, tmp_nn);
			}
			else
			{
				mol->setProperty(nn_tag_name, "");
			}
			
			out_sdf->write(*mol);
			
			delete mol;
			mol = query_sdf->read();
		}
		
		out_sdf->close();
		delete out_sdf;
		
		query_sdf->close();
		delete query_sdf;
	}
	else
	{
		LineBasedFile result_file(parpars.get("o"), File::MODE_OUT);
		result_file << "Query_ID Library_ID Tanimoto_Similarity" << endl;
		
		map<String, map<String, String> >::iterator result_it1 = result.begin();
		for (; result_it1!=result.end(); ++result_it1)
		{
			map<String, String>::iterator result_it2 = result_it1->second.begin();
			for (; result_it2!=result_it1->second.end(); ++result_it2)
			{
				result_file << result_it1->first << " " << result_it2->first << " " << result_it2->second << endl;
			}
		}
		
		result_file.close();
	}
	
	Log.level(10) << "++ Finished " << endl;
	Log.level(10) << "++" << endl;
	
	return 0;
}

