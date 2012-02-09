#include <iostream>
#include <cstdlib>

// BALL includes
#include <BALL/kernel.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/STRUCTURE/sideChainPlacementProcessor.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/STRUCTURE/atomBijection.h> 
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/PDBFile.h>

//#define DEBUG 1
#undef DEBUG

using namespace std;

namespace BALL 
{	
	const String SideChainPlacementProcessor::Method::SCWRL_4_0 = "scwrl_4_0";
	//const String SideChainPlacementProcessor::Method::SCWRL_SERVER = "scwrl_server";
	//const String SideChainPlacementProcessor::Method::ILP= "ilp";
	
	const char*  SideChainPlacementProcessor::Option::METHOD = "method";
	const String SideChainPlacementProcessor::Default::METHOD = SideChainPlacementProcessor::Method::SCWRL_4_0;

	const char* SideChainPlacementProcessor::Option::SCWRL_BINARY_PATH = "scwrl_binary_path";
	const String  SideChainPlacementProcessor::Default::SCWRL_BINARY_PATH = "Scwrl4";
	
	const char* SideChainPlacementProcessor::Option::MUTATE_SELECTED_SIDE_CHAINS = "mutate_sidechains";
	const bool  SideChainPlacementProcessor::Default::MUTATE_SELECTED_SIDE_CHAINS = false;

	const char* SideChainPlacementProcessor::Option::SCWRL_INPUT_FILE = "scwrl_input_file";
	const String SideChainPlacementProcessor::Default::SCWRL_INPUT_FILE = "";

	const char* SideChainPlacementProcessor::Option::SCWRL_SEQUENCE_FILE = "scwrl_input_sequence_file";
	const String SideChainPlacementProcessor::Default::SCWRL_SEQUENCE_FILE = "";

	const char* SideChainPlacementProcessor::Option::SCWRL_OUTPUT_FILE = "scwrl_output_file";
	const String SideChainPlacementProcessor::Default::SCWRL_OUTPUT_FILE = "";

	SideChainPlacementProcessor::SideChainPlacementProcessor()
		: UnaryProcessor<AtomContainer>(),
			options(),
			mutated_sequence_(),
			valid_(true)
	{
		setDefaultOptions();
	}

	SideChainPlacementProcessor::SideChainPlacementProcessor(const SideChainPlacementProcessor& scpp)
		:	UnaryProcessor<AtomContainer>(scpp),
			options(scpp.options),
			mutated_sequence_(scpp.mutated_sequence_),
			valid_(scpp.valid_)
	{
	}

	SideChainPlacementProcessor::~SideChainPlacementProcessor()
	{
		clear();
	}

	SideChainPlacementProcessor& SideChainPlacementProcessor::operator = (const SideChainPlacementProcessor& scpp)
	{
		// prevent self assignment
		if (&scpp == this)
			return *this;
		
		options = scpp.options;
		mutated_sequence_ = scpp.mutated_sequence_;
		valid_ = scpp.valid_;
		return *this;
	}

	void SideChainPlacementProcessor::clear()
	{
		//NOTE: options should remain!!
		valid_ = true;	
		mutated_sequence_ = "";
	}	
	
	bool SideChainPlacementProcessor::readOptions_()
	{
		bool ret = true;	
		// TODO: test if the Scwrl binary works

		return ret;
	}

	bool SideChainPlacementProcessor::start()
	{	
		String scwrl_binary_path    = options[Option::SCWRL_BINARY_PATH];
		String method               = options[Option::METHOD];
		bool   mutate_residues			= options.getBool(Option::MUTATE_SELECTED_SIDE_CHAINS);

		if (   (method == Method::SCWRL_4_0)
				&& (options[Option::SCWRL_OUTPUT_FILE] == "")
			 )
		{
			Path path;	
			String dirp = path.find(scwrl_binary_path);
		  if (dirp == "")
			{
				valid_= false;
				Log.error() << "SideChainPlacementProcessor: No valid path to SCWRL binary given!" << endl; 
				Log.error() << "Check option Option::SCWRL_BINARY_PATH." << endl;
			}	
		}
	 	else if (method != Method::SCWRL_4_0)
		{
			Log.error() << "SideChainPlacementProcessor: Invalid option Option::METHOD." << endl; 
			valid_ = false;
		}	
		
		if (mutate_residues && (mutated_sequence_.size() == 0))
		{
			valid_ = false;	
			Log.error() << "SideChainPlacementProcessor: option OPTIONS::MUTATE_SELECTED_SIDE_CHAINS "
				          << "is activated, but mutated sequence is empty!" << endl; 
		}
		return valid_;
	}
	
	Processor::Result SideChainPlacementProcessor::operator () (AtomContainer& ac)
	{
#ifdef DEBUG 
cout << " SideChainPlacementProcessor: OPTIONS:" << endl;
cout << " \t Method: " <<  options[Option::METHOD] << endl;
cout << " \t SCWRL binary path: "  << options[Option::SCWRL_BINARY_PATH] << endl;
cout << " \t mutate side chains: " << options.getBool(Option::MUTATE_SELECTED_SIDE_CHAINS);
cout << endl;
#endif
		
		// Speed up the code by temporarily storing the options locally 
		String scwrl_binary_path    = options[Option::SCWRL_BINARY_PATH];
		bool   mutate_residues			= options.getBool(Option::MUTATE_SELECTED_SIDE_CHAINS);

		if (valid_)		
		{
			// What kind of composite do we have?
			// Do we have a system, a molecule or a chain? 
			// Nothing else is allowed ...
			if (   RTTI::isKindOf<Chain>(ac) 
					|| RTTI::isKindOf<Protein>(ac) 
					|| RTTI::isKindOf<System>(ac)) 
			{
				//System* sys = RTTI::castTo<System>(ac);
				AtomContainer* sys = RTTI::castTo<AtomContainer>(ac);
				bool has_selection = sys->containsSelection();

				// sequence string indicating which residues shall be considered
				// in the side chain computation
				String apply_restriction = "";
				
				String PDB_temp_in  = options[Option::SCWRL_INPUT_FILE];
				String PDB_temp_out = options[Option::SCWRL_OUTPUT_FILE];
				String seq_in       = options[Option::SCWRL_SEQUENCE_FILE];
			
				bool has_output    = (PDB_temp_out != "");
				bool keep_input    = (PDB_temp_in != "");
				bool keep_sequence = (seq_in != "");

				// if we are not given a Scwrl results file, we have to
				// actually call Scwrl.
				int error_code = -1;
				if (!has_output)
				{
					// do we need a temporary input?
					if (!keep_input)
					{
						File::createTemporaryFilename(PDB_temp_in);			
					}

					PDBFile PDB_infile(PDB_temp_in, std::ios::out);	
					Protein* prot  = NULL;
					Chain*   chain = NULL;
					System*  in_sys;

					if (RTTI::isKindOf<System>(ac))
					{
						in_sys = RTTI::castTo<System>(ac);
						if (in_sys->countProteins() == 0)
						{
							Log.warn() << "SideChainPlacementProcessor::operator(): can only adapt proteins!" << endl;
							return Processor::BREAK;
						}
						else if (in_sys->countProteins() > 1)
						{
							Log.warn() << "SideChainPlacementProcessor::operator(): Scwrl can only handle one protein, but" << endl;
							Log.warn() << "this system contains several. Using only the first!" << endl;
						}
						prot = &(*(in_sys->beginProtein()));
					}
					else
					{
						if (RTTI::isKindOf<Protein>(ac))
							prot = RTTI::castTo<Protein>(ac);
						if (RTTI::isKindOf<Chain>(ac))
						{
							chain =  RTTI::castTo<Chain>(ac);
							prot = chain->getProtein();
						}
					}
					PDB_infile << *prot;
					PDB_infile.close();

					// create the scwrl command line	
					String scwrl_command_line = scwrl_binary_path + " -i " + PDB_temp_in; 

					// create an output file
					// Note: we have to create the file right after creating the filename
					// because otherwise the filename may be reused.
					File::createTemporaryFilename(PDB_temp_out);
					PDBFile PDB_outfile(PDB_temp_out, std::ios::out);
					PDB_infile.close();

					// add the outfile to the scwrl command line
					scwrl_command_line += " -o " + PDB_temp_out;

					// create the sequence file	
					// SCWRL supports application to selected objects via the sequence file.
					// NOTE: For the unconsidered residues SCWRL might update the hydrogens!
					String next_aa;
					Size residue_counter = 0;
					for (ResidueIterator rit = prot->beginResidue(); +rit; ++rit)
					{
						if (rit->isAminoAcid())
						{	
							next_aa = Peptides::OneLetterCode(rit->getName()); 
							String mutated_res_upper = mutated_sequence_[residue_counter];
							mutated_res_upper.toUpper();

							/*
cout << true << " chain:" << !chain << " " << (rit->getChain() == chain) 
	   << " sel:" << (!has_selection || rit->isSelected())
		 << " mut:" << ( mutate_residues && (mutated_sequence_.size() > residue_counter))
		 						<< "," << mutated_sequence_.size() << "," << residue_counter
		 << " seq:" << Peptides::OneLetterCode(rit->getName()) << " " << mutated_res_upper
		 << endl; */
							
							// We need upper case letters for:
							// - chains, whose proteins have additional chains
							// - selected residues 
							// - mutation of residues
/*							if (   (!chain || (rit->getChain() == chain)) //TODO
									&& (   (!has_selection || rit->isSelected())
										  || (     mutate_residues && (mutated_sequence_.size() > residue_counter)
										       && (Peptides::OneLetterCode(rit->getName()) != mutated_res_upper)))
								 )	*/
							if (   (!chain || (rit->getChain() == chain))
									&& (!has_selection   || rit->isSelected())
									&& (!mutate_residues || (    (mutated_sequence_.size() > residue_counter)
										       								  && (Peptides::OneLetterCode(rit->getName()) != mutated_res_upper)))
								 )	
							{	
								// An UPPER-CASE letter specifies that the 
								// corresponding residue's side-chain conformation
								// should be predicted for the amino acid
								// type given in the sequence file.				
								if (    mutate_residues 
										&& (Peptides::OneLetterCode(rit->getName()) != mutated_res_upper))
								{
									next_aa =  mutated_res_upper;
								}		
								next_aa.toUpper();
							}	
							else
							{	
								// Lower case letter means that the input
								// conformation of a side-chain should be preserved.
								next_aa.toLower();
							}
							apply_restriction += next_aa;
						}
						++residue_counter; 
					}

#ifdef DEBUG
					cout << "SideChainPlacementProcessor: apply restriction string: " <<  apply_restriction << endl;
#endif

					//      create and fill a restriction seq_in file
					// do we need a temporary file?
					if (!keep_sequence)
					{
						File::createTemporaryFilename(seq_in);
					}
					File seq_file(seq_in, std::ios::out);
					seq_file << apply_restriction;
					seq_file.close();

					// add the command for restricted computation 
					scwrl_command_line += " -s " + seq_in; 

#ifdef DEBUG
					cout << "SideChainPlacementProcessor: scwrl command: " << scwrl_command_line << endl;
#endif
					// execute scwrl
					error_code = system(scwrl_command_line.c_str());  
				}
				if (error_code == 0)
				{
					// read the resulting pdb file
					PDBFile SCWRL_result(PDB_temp_out);
					System temp_sys;
					SCWRL_result >> temp_sys;
	

					// we want to maintain as much of the original data as possible 
					// to support pointers and properties 

					// Map between original and mutated system
					AtomBijection mapper;

					mapper.assignByName(*sys, temp_sys);

					// we have to consider three cases:
					// - atoms of original residues with changed position
					// - atoms of mutated residues with changed position
					// - atoms added by scwrl 

					// iterate over the mapping and update the side chains
					for (Size i=0; i<mapper.size(); ++i)
					{
						Atom* orig_atom = mapper[i].first;
						Atom* scwrled_atom = mapper[i].second;
						if (orig_atom && scwrled_atom)
						{
							orig_atom->setPosition(scwrled_atom->getPosition());
							// mark atom as handled 
							scwrled_atom->setProperty("SideChainPlacementProcessor::ALREADY_UPDATED", true);
						}
					}
						
 					// now add the "new" atoms
					// first collect, than add
					ResidueIterator res_it_ori = ((System*)sys)->beginResidue();
					ResidueIterator res_it_scwrled = temp_sys.beginResidue();
					PDBAtomIterator at_it_ori;
					PDBAtomIterator at_it_scwrled;

					// collect while iterating over all residues
					vector<pair<Residue*, PDBAtom*> > to_add;
					vector<pair<Residue*, Residue*> > to_mutate;
					for ( ; +res_it_ori && +res_it_scwrled;  ++res_it_ori, ++res_it_scwrled)
					{	
						// the current residue is mutated
						if (res_it_ori->getName() != res_it_scwrled->getName())
						{
#ifdef DEBUG
cout << "SideChainPlacementProcessor: Mutate " << res_it_ori->getName() << " to " 
	    << res_it_scwrled->getName() << endl;
#endif
								to_mutate.push_back(pair<Residue*, Residue*>(&*res_it_ori, &*res_it_scwrled));
						}
						else // the current residue is original
						{
							for (	at_it_scwrled=res_it_scwrled->beginPDBAtom();
									 +at_it_scwrled; ++at_it_scwrled)
							{	
								// the current atom was added by scwrl
								if (!at_it_scwrled->hasProperty("SideChainPlacementProcessor::ALREADY_UPDATED"))
								{
									// create a new Atom
									PDBAtom* a = new PDBAtom(*at_it_scwrled);   
									// store for later adding
									to_add.push_back(pair<Residue*, PDBAtom*>(&*res_it_ori, a));
								}
								// if not: atom should already be covered by the mapping!
							}
						}
					}
					
					// first add the "new" atoms
					for (Size j=0; j<to_add.size(); ++j)
					{
						to_add[j].first->insert(*to_add[j].second);
					}
					
					// now consider the new atoms of the mutated residues
					// initialize a fragment DB
					FragmentDB fdb = FragmentDB("");

					//    mutate the residues
					for (Size j=0; j<to_mutate.size(); ++j)
					{
						vector<Atom*> to_delete;
						// delete all side chain atoms
						for (	at_it_ori = to_mutate[j].first->beginPDBAtom();
								  +at_it_ori; 
									++at_it_ori)
						{	
							// is it a backbone atom? 
							if ( !( (at_it_ori->getName()== "C") ||
										(at_it_ori->getName()== "O") ||
										(at_it_ori->getName()== "N") ||
										(at_it_ori->getName()== "HA") 
										)
								 )
							{
								to_delete.push_back(&*at_it_ori);
							}
						}
						// really delete them
						for (Size k=0; k<to_delete.size(); ++k)
						{
							to_delete[k]->destroy(); 
						}

						// rename the residue for the fragmentDB
						to_mutate[j].first->setName(to_mutate[j].second->getName());

						// apply the fragmentDB
						to_mutate[j].first->apply(fdb.normalize_names);
						to_mutate[j].first->apply(fdb.add_hydrogens);
						to_mutate[j].first->apply(fdb.build_bonds);

						//    move the atoms to the scwrled positions 
						// first map atoms between new and scwrled residue 
						AtomBijection res_mapper;
						res_mapper.assignByName(*to_mutate[j].first, *to_mutate[j].second);

						// iterate over the mapping and update the atoms
						for (Size i=0; i<res_mapper.size(); ++i)
						{
							Atom* orig_atom    = res_mapper[i].first;
							Atom* scwrled_atom = res_mapper[i].second;
							// then move
							orig_atom->setPosition(scwrled_atom->getPosition());
							// mark these atoms
							orig_atom->setProperty("SideChainPlacementProcessor::HAS_SCWRL_CORR", true);
						}

						//    now delete all atoms that have __no__ corresponding scwrl atom
						to_delete.clear();
						//NOTE: we have to use an AtomIterator here since
						//      the fragmentdb only adds atoms not PDBAtoms.
						for (	AtomIterator at_it_ori2 = to_mutate[j].first->beginAtom();
								+at_it_ori2; ++at_it_ori2)
						{	
							if (at_it_ori2->hasProperty("SideChainPlacementProcessor::HAS_SCWRL_CORR"))
							{
								at_it_ori2->clearProperty("SideChainPlacementProcessor::HAS_SCWRL_CORR");
							}
							else
							{	
								to_delete.push_back(&*at_it_ori2);
							}	
						}
						// now delete	
						for (Size k=0; k<to_delete.size(); ++k)
						{
							to_delete[k]->destroy(); 
						}
					} // end of mutate residues

					// now create the missing bonds for newly added atoms
					sys->apply(fdb.normalize_names);
					//to_mutate[j].first->apply(fdb.add_hydrogens);
					sys->apply(fdb.build_bonds);
				}
				else
				{
					Log.error() << "SideChainPlacementProcessor: call to Scwrl returned an error!" << endl; 
				}

				// we have to delete the tmp files 
				if (!keep_input)
					File::remove(PDB_temp_in);
				if (!has_output)
					File::remove(PDB_temp_out);
				if (!keep_sequence)
					File::remove(seq_in);   

			} // end of if molecule
		}
		return Processor::BREAK;
	}	

	bool SideChainPlacementProcessor::finish()
	{
		return true;
	}

	void SideChainPlacementProcessor::setDefaultOptions()
	{		
		options.setDefault(SideChainPlacementProcessor::Option::METHOD,
											 SideChainPlacementProcessor::Default::METHOD);			
		options.setDefault(SideChainPlacementProcessor::Option::SCWRL_BINARY_PATH,
											 SideChainPlacementProcessor::Default::SCWRL_BINARY_PATH);
		options.setDefaultBool(SideChainPlacementProcessor::Option::MUTATE_SELECTED_SIDE_CHAINS,
													 SideChainPlacementProcessor::Default::MUTATE_SELECTED_SIDE_CHAINS);
		options.setDefault(Option::SCWRL_INPUT_FILE, Default::SCWRL_INPUT_FILE);
		options.setDefault(Option::SCWRL_SEQUENCE_FILE, Default::SCWRL_SEQUENCE_FILE);
		options.setDefault(Option::SCWRL_OUTPUT_FILE, Default::SCWRL_OUTPUT_FILE);
	}

} // namespace BALL
