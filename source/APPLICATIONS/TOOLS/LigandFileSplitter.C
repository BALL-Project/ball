// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include "version.h"

#include<math.h>


using namespace BALL;
using namespace std;


void validateParameters(CommandlineParser& parpars)
{
  if (parpars.has("o"))
	{
    if (parpars.has("output_name_pattern"))
    {
      Log.level(10) << "\rNOTE: Parameter 'output_name_pattern' is ignored because filenames are specified explicitly." << endl;
    }
    if (parpars.has("no"))
    {
      Log.level(10) << "\rNOTE: Parameter 'no' is ignored because filenames are specified explicitly." << endl;
    }
    if (parpars.has("ligands_per_file"))
    {
      Log.level(10) << "\rNOTE: Parameter 'mpf' is ignored because filenames are specified explicitly." << endl;
    }
	}
  else
  {
    if (parpars.has("no"))
    {

      if (parpars.has("o"))
      {
        Log.level(10) << "\rNOTE: Parameter 'o' is ignored because the number of output files is specified explicitly." << endl;
      }
      if (parpars.has("ligands_per_file"))
      {
        Log.level(10) << "\rNOTE: Parameter 'mpf' is ignored because the number of output files is specified explicitly." << endl;
      }
    }
    else
    {
      if (parpars.has("mpf"))
      {      
        if (parpars.has("no"))
        {
          Log.level(10) << "\rNOTE: Parameter 'no' is ignored because the number of ligands per output file is specified explicitly." << endl;
        }
        if (parpars.has("o"))
        {
          Log.level(10) << "\rNOTE: Parameter 'o' is ignored because the number of ligands per output file is specified explicitly." << endl;
        }
      }
      else
      {
        // No parameter passed that can be used to determine how to split.
        // EXIT
        
        Log.error() << "\rERROR: No parameter passed to specify how to split." << endl;
        Log.error() << "\r       Please set either 'no', 'o' or 'mpf'." << endl;
        Log.error() << "\r       Exit without producing output files." << endl;

        exit(1);
      }
    }
  }
  

	if (parpars.has("outname_pattern"))
	{
    String outname_pattern = parpars.get("outname_pattern");  
		
    // count the number of '%d' and exit if the name is different to one
		string::size_type placeholder_1 = outname_pattern.find("%d");
		string::size_type placeholder_2 = outname_pattern.rfind("%d");
		
    // if there is more than one %d, it means that the index of the first and last occurrences are different
		// if there is not a single %d, then the index of both first and last occurrences must be npos
		if (placeholder_1 != placeholder_2 || placeholder_1 == String::npos)
		{
			Log.error() << "Error: The provided value for outname_pattern '" << outname_pattern << "' is invalid." << endl;
      Log.error() << "       Exit without producing output files." << endl;
			exit(1);
		}
	}
}


String getOutputFileName(String& outname_base, bool is_pattern, String& outfile_type, unsigned int index)
{
	String outfile_name = outname_base;
  
	if (is_pattern)
	{
		outfile_name.substituteAll("%d", String(index));
	}
	else
	{
		outfile_name = outfile_name + String("_") + String(index)  + "." + outfile_type;
	}
	
	return outfile_name;
}


int main(int argc, char* argv[])
{
	CommandlineParser parpars("LigandFileSplitter", "split molecule files", VERSION, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "input molecule file");
	parpars.registerOptionalIntegerParameter("no", "Number of output files to be created");
	parpars.registerOptionalIntegerParameter("mpf", "Number of molecules per output file");
	parpars.registerOptionalStringParameter("outname_pattern", "Pattern that will be used to generate the names of the output files, see notes and examples below.");
	parpars.registerOptionalOutputFileList("o", "Output filenames. If none are specified, input filename postfixed with IDs will be used");
  
	String man =
	"LigandFileSplitter splits a molecule file into a given number of subsets.\n\n"

	"Examples:\n\n"

  "$ LigandFileSplitter -i Trypsin_actives.sdf -o batch_1 batch_2\n"
	"  will split the input file Trypsin_actives.sdf in the two output files batch_1.sdf and batch_2.sdf.\n\n"
	
  "$ LigandFileSplitter -i Trypsin_actives.sdf -no 3\n"
	"  will split the input file Trypsin_actives.sdf in three files named Trypsin_actives_0.sdf, Trypsin_actives_1.sdf and Trypsin_actives_2.sdf\n\n"
	
  "$ LigandFileSplitter -i ligands.sdf -ligands_per_file 4\n"
	"  will split the input file ligands.sdf in as many files needed to fit at most 4 ligands per file.\n"
	"  The files will be named ligands_0.sdf, ligands_1.sdf ... ligands_N.sdf\n\n"
	
  "$ LigandFileSplitter -i ligands.sdf -ligands_per_file 5 -outname_pattern split_ligands-%d.sdf\n"
	"  will split the input file ligands.sdf in as many files needed to fit at most 5 ligands per file.\n"
  "  The files will be named split_ligands-0.sdf, split_ligands-1.sdf, ... , split_ligands-N.sdf.\n\n"
			  
  "$ LigandFileSplitter -i ligands.sdf -outname_pattern split_ligands_%d.sdf -no 100\n"
	"  will split the input file ligands.sdf in 100 files using the following names:\n"
	"  split_ligands_0.sdf, split_ligands_1.sdf, ... , split_ligands_99.sdf.\n\n"
			
  "NOTES:\n"
	"- Molecules are not sorted in any way.\n"
	"- The tool is no format converter and the format of the output files will be the same as of the input file.\n"
	"- Output_name_pattern accepts a printf-like pattern, expecting exactly one decimal integer placeholder, %d.\n"
	"- The following are valid patterns: output_ligand.sdf_%d, split_%d.mol, %d_lig.drf\n"
	"- The following are invalid patterns: output_%f.sdf, ligands.drf_%u, %d_lig_%d.mol, molecules.sdf\n\n"
  
  "WARNING:\n"
	"- If the parameter outname_pattern is specified, the user is responsible for the occurrence of a valid file extension\n"
	"  in the outname_pattern, which has to be of the same file format as the input file.\n\n";
  
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i","mol2,sdf,drf");
	parpars.setSupportedFormats("o","mol2,sdf,drf");
 	parpars.parse(argc, argv);
  
  
  // Check if parameter setting is valid and/or useful  
 	validateParameters(parpars);

  
  unsigned int n_molecules = 0;
  unsigned int n_outfiles = 0;
  unsigned int mpf = 0;
  
  String infile = parpars.get("i");
  String infile_name = infile.substr(0, infile.find_last_of('.'));
  String infile_type = infile.substr(infile.find_last_of('.') + 1, infile.length() - infile.find_last_of('.') - 1);
  
  vector<String> outfile_names;
  HashSet <String> conformation_ids;
  
  Molecule* mol;
 	GenericMolFile* input;
  GenericMolFile* output;

  DockResultFile* drf_input;
  DockResultFile* drf_output;  
  

	// Determine number of molecules in input files.
	// In case of DockResultFiles, we do not need to process all contained molecules 
	// in order to achieve this; we can simply count the result-section entries.
  
  input = MolFileFactory::open(infile);
	
  drf_input = NULL;
  drf_input = dynamic_cast<DockResultFile*>(input);
	if (drf_input)
	{
		n_molecules = drf_input->countConformations();
	}
	else
	{
 		Log.level(10) << "\rCount number of molecules in input file ..." << endl;
		for (mol = input->read(); mol; mol = input->read())
		{
			++n_molecules;
			delete mol;
		}
	}	
	
	Log.level(10) << "\r" << n_molecules << " molecules found." << endl << endl;

	input->close();
	delete input;

	
	// Check which split method should be applied
	
	if (parpars.has("o"))
	{
		// Option 1:
		// Number of output files specified by explicit name passing.
		// Parameter 'o' is specified
    
		list<String> tmp = parpars.getList("o");
    for (list<String>::iterator iter = tmp.begin(); iter != tmp.end(); ++iter)
    {
      outfile_names.push_back(*iter + "." + infile_type);
    }
    
		n_outfiles = outfile_names.size();
  
    if (n_molecules >= n_outfiles)
    {
      mpf = floor((double)n_molecules / n_outfiles);
    }
    else
    {
      Log.level(10) << "\rNOTE: Number of molecules in input file is smaller than number of specified output files." << endl;
      n_outfiles = n_molecules;
      mpf = 1;
    }
	}
  else
  {
    if (parpars.has("no"))
    {
      // Option 2:
		  // Number of output files is specified directly.
		  // Parameter 'no' is specified
      
      n_outfiles = parpars.get("no").toInt();
      
      if (n_molecules >= n_outfiles)
      {
        mpf = floor((double)n_molecules / n_outfiles);
      }
      else
      {
        Log.level(10) << "\rNOTE: Number of molecules in input file is smaller than specified number of output files." << endl;
        n_outfiles = n_molecules;
        mpf = 1;
      }
    }
    else
    {
      if (parpars.has("mpf"))
      {
        // Option 3:
        // Number of molecules per output file is specified directly.
        // Parameter 'mpf' is specified
      
        mpf = parpars.get("mpf").toInt();
        n_outfiles = ceil((double)n_molecules / (double)mpf);
      }
    }
    
    
    // Generate output file names
    
    if (parpars.has("outname_pattern"))
    {
      // Option 1: Generate output file names from specified pattern

      String pattern = parpars.get("outname_pattern");
      
      for (unsigned int i=0; i!= n_outfiles; ++i)
      {
        outfile_names.push_back(getOutputFileName(pattern, true, infile_type, i));
      }
    }
    else
    {
      // Option 2: Simple indexing of input file name
      
      for (unsigned int i=0; i!= n_outfiles; ++i)
      {
        outfile_names.push_back(getOutputFileName(infile_name, false, infile_type, i));
      }
    }
      
  }
  
  
  // Now do the splitting
  
  input = MolFileFactory::open(infile);

  drf_input = NULL;
	drf_input = dynamic_cast<DockResultFile*>(input);
	if (drf_input) 
  {
    drf_input->selectAllResultsForInput();
  }
  
  for (unsigned int i=0; i!=outfile_names.size(); ++i)
  {
    conformation_ids.clear();
    
    output = MolFileFactory::open(outfile_names[i], ios::out, infile_type);
    drf_output = dynamic_cast<DockResultFile*>(output);
    if (drf_input && drf_output)
    {
      drf_output->disableAutomaticResultCreation();
    }

    
    if (i < outfile_names.size()-1)
    {
      // Not the last file - so number of molecules is standard
      
      for (unsigned int j=0; j!=mpf; ++j)
      {
        mol = input->read();
        if (drf_input && drf_output && mol->hasProperty("Conformation_input_UID"))
        {
          conformation_ids.insert(mol->getProperty("Conformation_input_UID").toString());
        }

        output->write(*mol);
        delete mol;
      }
    }
    else
    {
      // Last output file - so write remaining molecules into it
      
      mol = input->read();
      while (mol)
      {
        if (drf_input && drf_output && mol->hasProperty("Conformation_input_UID"))
        {
          conformation_ids.insert(mol->getProperty("Conformation_input_UID").toString());
        }

        output->write(*mol);
        delete mol;
        mol = input->read();
      }
    }

    if (drf_input && drf_output)
    {
      const vector<Result*>* results = drf_input->getResults();
      for (unsigned int i = 0; i < results->size(); ++i)
      {
        Result* new_res = new Result(*(*results)[i], conformation_ids);
        drf_output->addResult(new_res);
      }
    }
    
    output->close();
    delete output;
  }  

  input->close();
  delete input;
  
  
  return 0;
}
