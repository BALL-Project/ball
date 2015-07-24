// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/SYSTEM/file.h>

#include <BALL/DATATYPE/string.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3DBase.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/ioModule.h>
#include <BALL/STRUCTURE/fragmenter.h>
#include <BALL/STRUCTURE/rmsdBinner.h>
#include <BALL/STRUCTURE/UCK.h>

#include <BALL/QSAR/aromaticityProcessor.h>

#include <boost/unordered/unordered_set.hpp>

using namespace BALL;
using namespace std;


/// ################# Q u a l i t y F i l t e r #################
/// #################       M E T H O D S       #################
static unsigned int num_covalent_fault = 0;
static unsigned int num_vdw_fault      = 0;
static unsigned int num_elem_fault     = 0;

// covalent bond is valid if it does not derive more than 30 % from its ideal length
bool validCovalentBond(Atom& atm, Atom& btm)
{
	float ideal_dist = atm.getElement().getCovalentRadius() + btm.getElement().getCovalentRadius();
	
	float actual_dist = atm.getDistance( btm );
	float diff = std::abs(ideal_dist - actual_dist);
	
	if ( diff > (ideal_dist * 0.3) )
	{
		num_covalent_fault++;
		return false;
	}
	
	return true;
}

// vdw dist is okay, if dist is not *smaller* than 40 % of ideal
bool validVdwDist(Atom& atm, Atom& btm)
{
	float ideal_dist = atm.getElement().getVanDerWaalsRadius() + btm.getElement().getCovalentRadius();
	ideal_dist = ideal_dist - ideal_dist * 0.4;
	
	float actual_dist = atm.getDistance( btm );
	
	if ( actual_dist < ideal_dist)
	{
		num_vdw_fault++;
		return false;
	}
	
	return true;
}

bool hasValidGeometry(AtomContainer& mol)
{
	for (AtomIterator ati = mol.beginAtom(); +ati; ++ati)
	{
		Atom& atm = *ati;
		AtomIterator bti = ati;
		for (++bti; +bti; ++bti)
		{
			Atom& btm = *bti;
			
			// atm & btm are covalently bond:
			if ( atm.getBond( btm ) != 0)
			{
				if ( !validCovalentBond(atm, btm) )
					return false;
			}
			else
			{
				if (!validVdwDist(atm, btm) )
					return false;
			}
		}
	}
	return true;
}

bool containsUnknownElement(AtomContainer &ac)
{
	for( AtomIterator it = ac.beginAtom(); +it; ++it)
	{
		if( it->getElement().getSymbol() == "?")
			return true;
	}
	
	return false;
}

bool isValid(AtomContainer& mol)
{
	if( containsUnknownElement( mol ) )
	{
		num_elem_fault++;
		return false;
	}
	else
	{
		return hasValidGeometry( mol );
	}
}


/// ################# E l e m e n t F i l t e r #################
/// #################       M E T H O D         #################
bool isOfElementClass(AtomContainer& mol, boost::unordered_set< Element::AtomicNumber >& elems)
{
	for (AtomIterator ati = mol.beginAtom(); +ati; ++ati)
	{
		Atom& atm = *ati;
		if ( elems.find(atm.getElement().getAtomicNumber()) == elems.end() )
			return false;
	}
	return true;
}

/// ################# W r i t e L i n e    F i l e s #################
void writePositionLines(AtomContainer& mol, LineBasedFile& handle)
{
	handle <<"key "<< mol.getProperty("key").getString() <<endl;
	handle << String(mol.countAtoms()) << endl;
	
	AtomIterator ati = mol.beginAtom();
	for(; +ati; ati++)
	{
		handle << String(ati->getPosition().x) << " ";
		handle << String(ati->getPosition().y) << " ";
		handle << String(ati->getPosition().z) << endl;
	}
}

/// ################# M A I N #################
/// File names: 
/// - rejected_metal.sdf : contains all metal or metal-organic molecules
/// - rejected_invalid-organic.sdf : contains all organic molecules which have
///                                  an invalid structure (severe clashes)
/// - filtered_organic.sdf : contains all valid organic molecule structures
/// 
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Make 3D Fragment DBs", " Create 3D fragment templates from a collection of experimental structures",
														0.1, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "sdfile to be filtered");
	parpars.setSupportedFormats("i","sdf");

	String manual = "This tool collects 3D fragments from experimental molecule "
									"structures. These can be used for the Generate3DStructure "
									"and Generate3DCombinations tools as fragment template "
									"databases. \n\n"
									"Please provide as input a SDFile that contains separated "
									"molecules (e.g.: by applying SeparateMolecules from BALL/TOOLS\n"
									"The following steps will then be performed:\n"
									" 0.) Remove all structures that contain only 1 or less atoms.\n"
									" 1.) Remove all structures with atoms outside of the "
									"organic set: [As, B, Br, C, Cl, F, H, I, N, O, P, S, Se, Si]\n"
									" 2.) Remove all structures that contain an invalid geometry "
									"which is defined as having a vdw-Interaction with a distance"
									" of only 40% of the ideal distance or a covalent bond of "
									"only 30% of the standard distance.\n"
									" 3.) Split into site templates and rigid templates.\n\n"
									"The following files will be created:\n"
									"* rejected_metal.sdf - contains all structures that were removed in step 1\n"
									"* rejected_invalid-organic.sdf - contains all structures that were removed in step 2\n"
									"* filtered_organic.sdf - contains the remaining structures after step 0 to 2\n"
									"* unique_filtered_organic.sdf - same as above, but reduced to only unique topologies\n"
									"* templates_rigid.sdf - all rigid template fragments in a SDFile plus UCK in the 'key' property\n"
									"* templates_rigid.line - all rigid template fragments only as coordinates plus UCK\n"
									"* templates_sites.sdf - all site template fragments in a SDFile with the site-key stored in the 'key' property\n";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
	/// C O D E ##################################
	///
	// 1.) define the set of elements we want to keep if a molecule
	//     consists of only these:
	
	boost::unordered_set< Element::AtomicNumber > organic_elements;
	PTE_ pte;
	
	organic_elements.insert( pte[Element::ARSENIC].getAtomicNumber());
	organic_elements.insert( pte[Element::BORON].getAtomicNumber()   );
	organic_elements.insert( pte[Element::BROMINE].getAtomicNumber() );
	organic_elements.insert( pte[Element::CARBON ].getAtomicNumber());
	organic_elements.insert( pte[Element::CHLORINE].getAtomicNumber());
	organic_elements.insert( pte[Element::FLUORINE].getAtomicNumber());
	organic_elements.insert( pte[Element::HYDROGEN].getAtomicNumber());
	organic_elements.insert( pte[Element::IODINE  ].getAtomicNumber());
	organic_elements.insert( pte[Element::NITROGEN].getAtomicNumber());
	organic_elements.insert( pte[Element::OXYGEN  ].getAtomicNumber());
	organic_elements.insert( pte[Element::PHOSPHORUS].getAtomicNumber());
	organic_elements.insert( pte[Element::SULFUR  ].getAtomicNumber());
	organic_elements.insert( pte[Element::SELENIUM].getAtomicNumber());
	organic_elements.insert( pte[Element::SILICON ].getAtomicNumber());
	
	Log << "Reading molecules..."<<endl;
	
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile results_file("filtered_organic.sdf", ios::out);
	SDFile invalids_file("rejected_invalid-organic.sdf", ios::out);
	SDFile metals_file("rejected_metal.sdf", ios::out);
	SDFile rigids_file("template_rigids.sdf", ios::out);
	LineBasedFile rigids_file_line("template_rigids.line", ios::out);
	SDFile sites_file("template_sites.sdf", ios::out);
	SDFile uniques_file("unique_filtered_organic.sdf", ios::out);

	/// For uniqueness check
	HashSet<String> unique_ids;
	String unique_key="###";


	/// For filter counts
	AtomContainer* tmp_mol = 0;
	unsigned int num_failed_reads = 0;
	unsigned int num_sucess_reads = 0;
	unsigned int num_organic = 0;
	unsigned int num_metal = 0;
	unsigned int num_too_small = 0;
	unsigned int num_valid = 0;
	unsigned int num_invalid = 0;
	unsigned int cnt = 0;
	
	/// For fragmenting:
	ACVec fragments, dummy;
	ConnectList dummy2;
	int total_fragment_cnt=0;
	
	MoleculeFragmenter mol_fragmenter;
	Canonicalizer canoni;
	RMSDBinner rigid_binner(false);
	AromaticityProcessor arproc;
	
	int total_sites = 0;
	vector<pair<String, AtomContainer *> > temp_sites;
	RMSDBinner site_binner(true, 0.3, 100);
	
	do
	{
		// 1) try 'try_limit' times to load a molecule
		const int try_limit = 10000;
		int i = 0;
		for(; i < try_limit; ++i) // try reading untill you do not get any exception
		{
			// some user info every 1000 molecules:
			if( cnt % 1000 == 0)
			{
				cout << "\r" << flush;
				cout << "     Filtered: "<< cnt<<" structures to "<<num_valid<<" valid organic molecules";
			}
			cnt++;
			
			try
			{
				tmp_mol = in_file.read(); // EOF gives a successful read, thus also escaping the loop (sets tmp_mol to NULL)
				num_sucess_reads++;
				break;
			}
			catch (...)
			{
				num_failed_reads++;
			}
		}
		if (i >= try_limit)
		{
			cout<<"ERROR: Tried "<<try_limit<<" times to read a molecule from the input file"
				 << "but failed every time"<<endl;
			exit(EXIT_FAILURE);
		}
		
		// 2) If you've got a molecule: apply filters
		if( 0 != tmp_mol)
		{
			LigBase::removeHydrogens( *tmp_mol );
			
			if(tmp_mol->countAtoms() <= 1)
			{
				num_too_small++;
				num_invalid++;
				continue;
			}
			
			// Is Organic
			if( isOfElementClass(*tmp_mol, organic_elements) ) // is organic molecule
			{
				num_organic++;
				
				// Is Valid (and organic)
				if( isValid(*tmp_mol))
				{
//					canoni.canonicalize(*tmp_mol);
					AromaticityProcessor aroma;
					tmp_mol->apply(aroma);

					results_file << *tmp_mol;
					num_valid++;

					unique_key = Matcher::getUCK(*tmp_mol);

					// Is unique (and valid, organic)
					if( !unique_ids.has( unique_key ))
					{
						unique_ids.insert( unique_key );
						tmp_mol->setProperty("UCK-Hash", unique_key);
						uniques_file << *tmp_mol;
					}
					
					// Create Fragments from the Molecule:
					{
						tmp_mol->apply(arproc);

						mol_fragmenter.setMolecule(*tmp_mol);
						mol_fragmenter.fragmentToSites(temp_sites, false);
						mol_fragmenter.fragment(fragments, dummy, dummy2);

						total_sites += temp_sites.size();
						total_fragment_cnt += fragments.size();

						// Binning for each site:
						vector<pair<String, AtomContainer *> >::iterator ita;
						for(ita = temp_sites.begin(); ita != temp_sites.end(); ++ita)
							site_binner.addMolecule( ita->first, *ita->second );

						// Binning for each rigid fragment:
						vector<AtomContainer*>::iterator fit;
						for(fit = fragments.begin(); fit!=fragments.end(); fit++)
						{
							// canonicalise the atomlist & set UCK key
							canoni.canonicalize( **fit );

							unique_key = Matcher::getUCK( **fit);

							rigid_binner.addMolecule(unique_key, **fit);
						}
					}
					
				}
				// Else it is an in-valid organic molecule:
				else
				{
					num_invalid++;
					invalids_file << *tmp_mol;
				}
			}
			else // is metal-molecule:
			{
				num_metal++;
				metals_file << *tmp_mol;
			}
			
			delete tmp_mol;
		}
	} while( tmp_mol );
	cout << "\r" << flush;
	cout << "                                                                        " << endl;
	
	Log<< endl<< endl;
	Log<< "REPORT - Input Structures:"<<endl;
	Log<< " Successfully read structures: "<< num_sucess_reads - 1 << endl;
	Log<< " Failed read attempts:         "<< num_failed_reads << endl;
	Log<< endl;
	Log<< " after excluding single atom structures: " << num_sucess_reads - num_too_small<<endl;
	Log<< " after excluding non-organic structures: " << num_organic << endl;
	Log<< " after excluding invalid structures:     " << num_valid   << " (finally used structures)"<< endl;
	Log<< " after excluding duplicates:             " << unique_ids.getSize() << endl;
	Log<< endl;
	Log<< " Number of invalid Structures: " << num_invalid <<endl;
	Log<< " Found invalid types:"<<endl;
	Log<< "       Contained Unknown Element: " << num_elem_fault <<endl;
	Log<< "   Contained Covalent Bond Fault: " << num_covalent_fault <<endl;
	Log<< "          Contained Severe Clash: " << num_vdw_fault <<endl;
	Log<< "......done!" << endl;
	
	
	///3.) Write Site templates out
	map <String, vector< pair<AtomContainer*, int> > >::iterator it;
	for(it = site_binner.begin(); it != site_binner.end(); ++it)
	{
		unique_key = it->first;
		AtomContainer* ac = it->second[0].first;
		ac->setProperty("key", unique_key);
		
		sites_file << *ac;
	}
	
	///4.) Write Rigid fragments out
	map <String, vector< pair<AtomContainer*, int> > >::iterator bin_it;
	for(bin_it = rigid_binner.begin(); bin_it != rigid_binner.end(); ++bin_it)
	{
		AtomContainer& frag = * bin_it->second[0].first;
		frag.setProperty("key", bin_it->first);
		
		writePositionLines(frag, rigids_file_line);
		rigids_file << frag;
	}
}

