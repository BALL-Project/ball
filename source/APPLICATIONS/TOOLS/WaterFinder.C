// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/DOCKING/COMMON/structurePreparer.h>
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include <BALL/COMMON/exception.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/KERNEL/molecularInteractions.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/SCORING/FUNCTIONS/PLPScoring.h>
#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/DOCKING/COMMON/gridAnalysis.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <set>
#include "version.h"

using namespace BALL;
using namespace std;

Residue* createNewWater(Vector3 center = Vector3(0, 0, 0))
{
	Residue* water = new Residue;
	water->setName("HOH");
	PDBAtom* oxygen = new PDBAtom;
	oxygen->setElement(PTE_::getElement("O"));
	oxygen->setPosition(Vector3(center.x, center.y, center.z));
	PDBAtom* h1 = new PDBAtom;
	h1->setElement(PTE_::getElement("H"));
	h1->setPosition(Vector3(center.x+0.2774, center.y+0.8929, center.z+0.2544));
	PDBAtom* h2 = new PDBAtom;
	h2->setElement(PTE_::getElement("H"));
	h2->setPosition(Vector3(center.x+0.6068, center.y-0.2383, center.z-0.7169));
	h2->createBond(*oxygen);
	h1->createBond(*oxygen);
	water->insert(*oxygen);
	water->insert(*h1);
	water->insert(*h2);
	return water;
}


// ab initio, grid-based, search
void findPutativeWaterPositions(ScoringFunction* sf, System& water_system, const double& threshold, StructurePreparer* sp, String& par_file)
{
	Protein* protein = new Protein;
	water_system.insert(*protein);

	Residue* water = createNewWater();
	sp->prepare(water, par_file);
	GridAnalysis ga(sf, water);
	ga.setResolution(1.4);
	cout<<"starting grid analysis ..."<<flush;
	RegularData3D* grid = ga.calculate();
	cout<<" finished."<<endl<<flush;
	delete water;
	RegularData3D::IndexType size = grid->getSize();
	multimap<double, Residue*> water_map;

	for (int i = 0; i < (int)size.x; i++)
	{
		for (int j = 0; j < (int)size.y; j++)
		{
			for (int k = 0; k < (int)size.z; k++)
			{
				RegularData3D::IndexType index(i, j, k);
				Vector3 position = grid->getCoordinates(index);

				double score = grid->getData(index);
				if (score < threshold)
				{
					// -- check for other, better scored water molecules near the current water molecule
					// -- if none are found, create new water molecule at current position
					//    and add it to water_system
					bool ok = 1;

					int i2 = i-1;
					if (i2 < 0) i2 = 0;
					for (; i2 < i+1 && i2 < (int)size.x; i2++)
					{
						int j2 = j-1;
						if (j2 < 0) j2 = 0;
						for (; j2 < j+1 && j2 < (int)size.y; j2++)
						{
							int k2 = k-1;
							if (k2 < 0) k2 = 0;
							for (; k2 < k+1 && k2 < (int)size.z; k2++)
							{
								if (i2 == i && j2 == j && k2 == k)
								{
									continue;
								}

								RegularData3D::IndexType index2(i2, j2, k2);
								double score2 = grid->getData(index2);
								if (score2 < score)
								{
									ok = false;
									break;
								}
							}
						}
					}

					if (ok)
					{
						Residue* new_water = createNewWater(position);
						sp->prepare(new_water, par_file);
						water_map.insert(make_pair(score, new_water));
						if (water_map.size() > 100)
						{
							water_map.erase(--water_map.end());
						}
					}
				}
			}
		}
	}

	// Add the 100 best putative water positions to water_system
	for (multimap < double, Residue* > ::iterator it = water_map.begin(); it != water_map.end(); it++)
	{
		protein->insert(*it->second);
	}

	cout<<"found "<<water_map.size()<<" putative water positions."<<endl<<flush;
	delete grid;
}


void rotateMolecule(Residue& mol, int axis, int degree, Vector3& center)
{
	TMatrix4x4<float> M;
	TAngle<float> angle(degree, false);
	Vector3 axis_vector(axis == 0, axis == 1, axis == 2);
	M.setRotation(angle, axis_vector);

	for (AtomIterator it = mol.beginAtom(); it != mol.endAtom(); it++)
	{
		it->setPosition(M*(it->getPosition()-center)+center);
	}
}


void optimizeWaterHydrogens(ResidueIterator& water, Chain& water_chain, System& receptor, Options& option)
{
	ResidueIterator next_res = water;
	next_res++;
	water_chain.remove(*water);

	MMScoring scoring_function(receptor, *water, option);
	scoring_function.setLigand(*water);
	Vector3 center = scoring_function.getLigandCenter();

	int best_angle_x = 0;
	int best_angle_y = 0;
	int best_angle_z = 0;
	double best_score = 1e12;
	for (Size i = 1; i < 36; i++)
	{
		rotateMolecule(*water, 0, 10, center);
		scoring_function.update();
		double score_i = scoring_function.updateScore();
		if (score_i < best_score)
		{
			best_score = score_i;
			best_angle_x = i*10;
		}
	}
	rotateMolecule(*water, 0, -350+best_angle_x, center);
	for (Size i = 1; i < 36; i++)
	{
		rotateMolecule(*water, 1, 10, center);
		scoring_function.update();
		double score_i = scoring_function.updateScore();
		if (score_i < best_score)
		{
			best_score = score_i;
			best_angle_y = i*10;
		}
	}
	rotateMolecule(*water, 1, -350+best_angle_y, center);
	for (Size i = 1; i < 36; i++)
	{
		rotateMolecule(*water, 2, 10, center);
		scoring_function.update();
		double score_i = scoring_function.updateScore();
		if (score_i < best_score)
		{
			best_score = score_i;
			best_angle_z = i*10;
		}
	}

	// rotate back to original orientation
	rotateMolecule(*water, 2, -350, center);
	rotateMolecule(*water, 1, -best_angle_y, center);
	rotateMolecule(*water, 0, -best_angle_x, center);

	// rotate to best orientation
	rotateMolecule(*water, 0, best_angle_x, center);
	rotateMolecule(*water, 1, best_angle_y, center);
	rotateMolecule(*water, 2, best_angle_z, center);

	 // re-insert water into protein
	if (next_res != water_chain.endResidue())
	{
		water_chain.insertBefore(*water, *next_res);
	}
	else water_chain.append(*water);
}


Vector3 calculateMoleculeCenter(const AtomContainer& molecule)
{
	Vector3 center(0, 0, 0);
 int no_atoms = 0;
	for (AtomConstIterator it = molecule.beginAtom(); +it; it++, no_atoms++)
	{
  center += it->getPosition();
	}
	center /= no_atoms;
	return center;
}


double calculateWaterBinding(ResidueIterator& water, Chain& water_chain, System& receptor, Options& option)
{
	ResidueIterator next_res = water;
	next_res++;
	water_chain.remove(*water);

	MMScoring scoring_function(receptor, *water, option);
	scoring_function.setLigand(*water);
	scoring_function.update();
	double score = scoring_function.updateScore();

	 // re-insert water into protein
	if (next_res != water_chain.endResidue())
	{
		water_chain.insertBefore(*water, *next_res);
	}
	else water_chain.append(*water);

	return score;
}


void removeUnboundWaterNetworks(Chain* water_chain, System& receptor, Options& option, const double& BRIDGE_REC_THRESHOLD)
{
	map<Residue*, double> receptor_scores; // binding-energy of water molecules to receptor
	list<list<Residue*> > connections; // each list contains one water-network
	typedef list<list<Residue*> >::iterator NetworkIterator;
	map<Residue*, NetworkIterator> connection_map; // maps each water to the network to which it belongs

	// Calculate binding of water molecules to the receptor.
	Protein* protein = receptor.getProtein(0);
	protein->remove(*water_chain);
	for (ResidueIterator res_it1 = water_chain->beginResidue(); res_it1 != water_chain->endResidue(); res_it1++)
	{
		double rec_score = calculateWaterBinding(res_it1, *water_chain, receptor, option);
		receptor_scores.insert(make_pair(&*res_it1, rec_score));
		//cout<<"receptor -- "<<res_it1->getID()<<" : "<<rec_score<<endl;
	}

	// Find out which water molecules are connected to each other.
	Chain tmp_chain;
	protein->append(tmp_chain);

	for (ResidueIterator res_it1 = water_chain->beginResidue(); res_it1 != water_chain->endResidue(); )
	{
		ResidueIterator next_res1 = res_it1;
		next_res1++;
		Residue* water1 = &*res_it1;
		tmp_chain.insert(*water1); // this also removes the water from 'water_chain' !

		for (ResidueIterator res_it2 = water_chain->beginResidue(); res_it2 != water_chain->endResidue(); )
		{
			if (&*res_it1 == &*res_it2)
			{
				res_it2++;
				continue;
			}

			ResidueIterator next_res2 = res_it2;
			next_res2++;
			Residue* water2 = &*res_it2;
			tmp_chain.insert(*water2); // this also removes the water from 'water_chain' !
			double score = calculateWaterBinding(res_it1, tmp_chain, receptor, option);
			tmp_chain.remove(*water2);
			if (next_res2 != water_chain->endResidue())
			{
				water_chain->insertBefore(*water2, *next_res2);
			}
			else water_chain->append(*water2);
			res_it2 = next_res2;

			double rec_score = receptor_scores.find(water1)->second;
			double water_score_res2 = score-rec_score;

			if (water_score_res2 < -1)
			{
				map<Residue*, NetworkIterator>::iterator search_it1 = connection_map.find(water1);
				NetworkIterator list_it;
				if (search_it1 != connection_map.end())
				{
					search_it1->second->push_back(water2);
					list_it = search_it1->second;
				}
				else
				{
					list<Residue*> clist;
					clist.push_back(water1);
					clist.push_back(water2);
					connections.push_back(clist);
					list_it = connections.end();
					list_it--;
					connection_map.insert(make_pair(water1, list_it));
				}

				map<Residue*, NetworkIterator>::iterator search_it2 = connection_map.find(water2);
				if (search_it2 != connection_map.end() && &*search_it1->second != &*search_it2->second)
				{
					list<Residue*> clist = *search_it2->second;
					for (list<Residue*>::iterator it = clist.begin();
						it!=clist.end(); it++)
					{
						Residue* res3 = *it;
						connection_map.erase(res3);
						connection_map.insert(make_pair(res3, list_it));
					}
					list_it->splice(list_it->begin(), clist);
				}
				connection_map.erase(water2);
				connection_map.insert(make_pair(water2, list_it));
			}
		}

		tmp_chain.remove(*water1);
		if (next_res1 != water_chain->endResidue())
		{
			water_chain->insertBefore(*water1, *next_res1);
		}
		else water_chain->append(*water1);
		res_it1 = next_res1;
	}
	protein->remove(tmp_chain);

	// Now, check that each water molecule is either bound strongly to the receptor or is part of a water network that has at least one water that is strongly bound to the receptor.
	String removed = "";
	for (ResidueIterator res_it = water_chain->beginResidue(); res_it != water_chain->endResidue(); )
	{
		// The current water itself is strongly bound to the receptor, so do nothing more here.
		if (receptor_scores.find(&*res_it)->second < BRIDGE_REC_THRESHOLD)
		{
			res_it++;
			continue;
		}

		bool receptor_bound = false; // has a connection of the water network to the receptor been found?

		// if the current water is part of a water network ..
		if (connection_map.find(&*res_it) != connection_map.end())
		{
			const list<Residue*>& res_connections = *connection_map.find(&*res_it)->second;

			for (list < Residue* > ::const_iterator lit = res_connections.begin(); lit != res_connections.end(); lit++)
			{
				if (receptor_scores.find(*lit)->second < BRIDGE_REC_THRESHOLD)
				{
					receptor_bound = true;
					break;
				}
			}
		}

		if (!receptor_bound)
		{
			ResidueIterator tmp_it = res_it;
			tmp_it++;
			removed += res_it->getFullName()+res_it->getID()+" ";
			water_chain->remove(*res_it);
			res_it = tmp_it;
		}
		else res_it++;
	}

	if (removed != "")
	{
		cout<<endl<<"The following water molecules have been removed because they are part of water networks that are not bound strongly to the receptor: "<<removed<<endl<<endl;
	}

	protein->append(*water_chain);
}



double calculateMinimalDistance(const AtomContainer& ref_ligand, const AtomContainer& water, double threshold)
{
	double min_dist = 9999;
	for (AtomConstIterator it = ref_ligand.beginAtom(); +it; it++)
	{
		for (AtomConstIterator it2 = water.beginAtom(); +it2; it2++)
		{
			double dist = it->getDistance(*it2);
			if (dist < threshold) return dist;
			if (dist < min_dist) min_dist = dist;
		}
	}
	return min_dist;
}



int main(int argc, char* argv[])
{
	CommandlineParser par("WaterFinder", "find strongly bound waters ", VERSION, String(__DATE__), "Docking");
	par.registerMandatoryInputFile("rec", "receptor pdb-file");
	par.registerMandatoryInputFile("rl", "reference-ligand");
	par.registerOptionalInputFile(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME, "configuration file");
	par.registerOptionalInputFile("wat", "input pdb-file containing water molecules (if not part of rec.-file)");
	par.registerFlag("ai", "use ab-initio water search (ignore water in pdb-file), experimental!");
	par.registerOptionalOutputFile("o", "output pdb file");
	par.registerOptionalOutputFile("write_ini", "write ini-file w/ default parameters (and don't do anything else)");

	String man = "This tool searches for crystal waters that\n\
    * either interact very strongly with the receptor\n\
    * or that interact strongly with receptor and reference ligand, \n\
      thus functioning as a water bridge.\n\nWater molecules in the pdb-structure (i.e. single oxygens) are automatically protonated and rotationally optimized before the search is done.\n\nAs input we need:\n\
    * a file containing a protonated protein in pdb-format.\n\
      This file should contain water molecules that are to be evaluated by this tool.\n\
      However, you can also use a separate pdb-file as input for the water molecules (see below).\n\
    * a file containing a reference ligand.\n\
      This reference ligand should be located in the binding pocket.\n\
      Supported formats are mol2, sdf or drf (DockResultFile, xml-based).\n\
    * optionally a file in pdb-format containing water molecules.\n\
      If you specify such a file , all water molecules appearing in the\n\
      protein input-file (if any) will be ignored.\n\nOutput of this tool is a pdb-file containing the protein and all detected strongly bound water molecules.";
	par.setToolManual(man);
	par.setSupportedFormats("rec","pdb");
	par.setSupportedFormats("wat","pdb");
	par.setSupportedFormats("rl",MolFileFactory::getSupportedFormats());
	par.setSupportedFormats(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME,"ini");
	par.setSupportedFormats("o","pdb");
	par.setSupportedFormats("write_ini","ini");
	Options default_options;
	ScoringFunction::getDefaultOptions(default_options);
	par.registerAdvancedParameters(default_options);
	par.setSupportedFormats(ScoringFunction::SUBCATEGORY_NAME, "filename","ini");
	par.parse(argc, argv);

	String default_inifile = par.get("write_ini");
	if (default_inifile != CommandlineParser::NOT_FOUND)
	{
		// if ini-file already exists, read its entries first
		list<Constraint*> clist;
		if (ifstream(default_inifile.c_str()))
		{
			DockingAlgorithm::readOptionFile(default_inifile, default_options, clist);
		}

		Options* scoring_options = default_options.getSubcategory(ScoringFunction::SUBCATEGORY_NAME);
		scoring_options->setDefault("scoring_type", "MM");
		scoring_options->setDefault("nonbonded_cutoff_precalculation", scoring_options->get("nonbonded_cutoff"));
		scoring_options->set("nonbonded_cutoff", 3.0);
		DockingAlgorithm::writeOptionFile(par.get("write_ini"), default_options, clist);
		Log << "Ini-file w/ default values has been written to file '"<<default_inifile<<"'. Goodbye!"<<endl;
		return 0;
	}

	Log.setMinLevel(cout, 10);

	GenericMolFile* ref_ligand_file = MolFileFactory::open(par.get("rl"));
	Molecule* ref_ligand = ref_ligand_file->read();
	ref_ligand_file->close();
	delete ref_ligand_file;

	Options option;
	par.copyAdvancedParametersToOptions(option);
	list<Constraint*> constraints;
	if (par.get(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME) != CommandlineParser::NOT_FOUND)
	{
		DockingAlgorithm::readOptionFile(par.get(DockingAlgorithm::OPTION_FILE_PARAMETER_NAME), option, constraints, ref_ligand);
	}
	Options* option_category = option.getSubcategory(ScoringFunction::SUBCATEGORY_NAME);
	if (!option_category) option_category = &option;
	String scoring_type = option.setDefault("scoring_type", "MM");

	// Support for using one and the same config-file for grid precalculation and docking
	String precalc_nonbonded_cuttoff = option_category->get("nonbonded_cutoff_precalculation");
	if (precalc_nonbonded_cuttoff != "")
	{
		option_category->set("nonbonded_cutoff", precalc_nonbonded_cuttoff);
	}

	if (scoring_type == "")
	{
		cout<<"[Error:] scoring_type must be specified in the config-file!"<<endl;
		exit(1);
	}
	if (scoring_type != "GridedMM" && scoring_type != "GridedPLP" && scoring_type != "MM" && scoring_type != "PLP")
	{
		cout<<"[Error:] no valid scoring_type specified !"<<endl;
		exit(1);
	}
	String par_file = option_category->get("filename");
	if (par_file == "") par_file="Amber/amber96-docking.ini";
	if (par_file == "" && !scoring_type.hasSubstring("GH"))
	{
		cout<<"[Error:] 'filename' for force-field parameter file must be specified in the config-file  !"<<endl;
		exit(1);
	}

	double DIRECT_REC_THRESHOLD = -5;  // threshold for water directly bound to receptor
	double BRIDGE_LIG_THRESHOLD = -1.5; // threshold for HOH--ligand interaction in case of water-bridges
	double BRIDGE_REC_THRESHOLD = -2; // threshold for HOH--receptor interaction in case of water-bridges

	StructurePreparer* sp;
	//bool use_PLP = 0;
	if (scoring_type.hasSubstring("PLP"))
	{
		//use_PLP = 1;
		sp = new StructurePreparer("PLP");
	}
	else
	{
		//use_PLP = 0;
		sp = new StructurePreparer;
	}

	sp->prepare(ref_ligand, par_file);

	System receptor;
	GenericMolFile* input = MolFileFactory::open(par.get("rec"));
	if (!input)
	{
		cerr<<"Format of receptor-file not supported!"<<endl;
		return 1;
	}
	*input >> receptor;
	sp->prepare(&receptor, par_file);
	delete input;

	// stores those water molecules that interact very strongly with the receptor
	set<Residue*> waters_strongly_recbound;

	bool found_water = 0;

	// Remove waters already in 'receptor'.
	// Else we would end up with two water molecules at the same position.
	for (ResidueIterator it = receptor.beginResidue(); +it; )
	{
		if (it->getName() == "HOH" || it->getName() == "WAT")
		{
			ResidueIterator tmp_it = it;
			tmp_it++;
			it->getChain()->remove(*it);
			it = tmp_it;
		}
		else it++;
	}

	/// ab initio search for putative water positions
	System water_system;
	bool ab_initio_search = (par.get("ai")!=CommandlineParser::NOT_FOUND);
	if (ab_initio_search)
	{
		ScoringFunction* scoring_function = 0;
		if (scoring_type == "GridedMM" || scoring_type == "MM")
		{
			scoring_function = new MMScoring(receptor, *ref_ligand, option);
		}
		else if (scoring_type == "GridedPLP" || scoring_type == "PLP")
		{
			scoring_function = new PLPScoring(receptor, *ref_ligand, option);
		}

		findPutativeWaterPositions(scoring_function, water_system, BRIDGE_REC_THRESHOLD, sp, par_file);

		delete scoring_function;
	}

	/// Read water molecules and append them to 'receptor'
	if (!ab_initio_search)
	{
		String water_file = par.get("wat");
		if (water_file == CommandlineParser::NOT_FOUND)
		{
			water_file = par.get("rec");
		}
		PDBFile water_pdb(water_file);
		water_pdb >> water_system;
	}
	Chain* water_chain = new Chain("Waters");
	Vector3 reflig_center = calculateMoleculeCenter(*ref_ligand);

	for (ResidueConstIterator it = water_system.beginResidue(); +it; it++)
	{
		String name = it->getName().trim();
		if (name.hasPrefix("HOH") || name.hasPrefix("WAT"))
		{
			if (calculateMinimalDistance(*ref_ligand, *it, 5) < 5)
			{
				Residue* residue_copy = new Residue(*it);
				water_chain->insert(*residue_copy);
				found_water = 1;
			}
		}
	}

	if (!found_water)
	{
		Log.level(20)<<"No water molecules found in input pdb-file, thus "<<endl;
		String outfile = par.get("o");
		if (outfile != CommandlineParser::NOT_FOUND) // write receptor with selected water molecules to output file
		{
			cout<<"writing unmodified protein to output file."<<endl;
			PDBFile output(outfile, ios::out);
			output << receptor;
		}
		else cout<<"doing nothing."<<endl;
		exit(0);
	}


	// For safety, remove water hydrogens that already exist in input file.
	// We will protonate the water molecules later, anyway.
	for (AtomIterator it = water_chain->beginAtom(); +it; it++)
	{
		if (it->getElement().getSymbol() == "H")
		{
			it->select();
		}
		else it->deselect();
	}
	water_chain->removeSelected();


	// add hydrogens to the new water molecules
	FragmentDB fragDB("fragments/Fragments.db");
	if (!ab_initio_search) water_chain->apply(fragDB.add_hydrogens);
	receptor.getProtein(0)->append(*water_chain);
	if (!ab_initio_search) sp->prepare(&receptor, par_file);

	receptor.insert(*ref_ligand); // include ref-ligand for optimization of hydrogens !

	cout<<"number of water molecules of be evaluated: "<<water_chain->countResidues()<<endl;


	/// Quickly optimize the hydrogen positions
	for (Size i = 0; i < 3; i++)
	{
		Size res_no = 1;
		for (ResidueIterator it = water_chain->beginResidue(); it != water_chain->endResidue(); )
		{
			cout<<"\r"<<res_no<<flush;
			res_no++;
			ResidueIterator tmp_it = it;
			tmp_it++;
			optimizeWaterHydrogens(it, *water_chain, receptor, option);
			it = tmp_it;
		}
		cout<<endl;
	}

	receptor.remove(*ref_ligand);

	/// Evaluate binding of water molecules to receptor *or* other water molecules
	for (ResidueIterator it = water_chain->beginResidue(); it != water_chain->endResidue(); )
	{
		ResidueIterator tmp_it = it;
		tmp_it++;
		double score = 	calculateWaterBinding(it, *water_chain, receptor, option);

		// remove those water molecules that are not bound to the receptor
		if (score > BRIDGE_REC_THRESHOLD)
		{
			water_chain->remove(*it);
			cout<<"removed water not bound to receptor,  score="<<score<<endl;
		}

		if (score < DIRECT_REC_THRESHOLD)
		{
			cout<<"found water strongly bound to receptor or other water molecules: "<<it->getName()<<it->getID()<<", "<<score<<" kJ/mol"<<endl;
			waters_strongly_recbound.insert(&*it);
		}
		it = tmp_it;
	}


	/// Remove waters that are part of water networks that are not connected to the receptor
	removeUnboundWaterNetworks(water_chain, receptor, option, BRIDGE_REC_THRESHOLD);


	/// Now evaluate binding of water molecules to the *ref-ligand*
	ScoringFunction* scoring_function = 0;
	if (scoring_type == "GridedMM" || scoring_type == "MM")
	{
		scoring_function = new MMScoring(receptor, *ref_ligand, option);
	}
	else if (scoring_type == "GridedPLP" || scoring_type == "PLP")
	{
		scoring_function = new PLPScoring(receptor, *ref_ligand, option);
	}
	scoring_function->enableStoreInteractions();
	scoring_function->setLigand(*ref_ligand);
	scoring_function->update();
	scoring_function->updateScore();
	scoring_function->printResult(1);

	set<Residue*> selected_waters;

	list<String> interaction_types;
	interaction_types.push_back("ES");
	interaction_types.push_back("HB");
	interaction_types.push_back("vdW");

	bool first = 1;

	for (ResidueIterator it = receptor.beginResidue(); +it; it++)
	{
		String name = it->getName().trim();
		if (!name.hasPrefix("HOH") && !name.hasPrefix("WAT"))
		{
			continue;
		}

		double score_sum = 0;

		for (AtomConstIterator it2 = it->beginAtom(); +it2; it2++)
		{
			if (it2->interactions)
			{
				score_sum += it2->interactions->getInteractionEnergy();
			}
		}

		if (fabs(score_sum) > 1.5 || waters_strongly_recbound.find(&*it) != waters_strongly_recbound.end())
		{
			if (first)
			{
				cout<<"Interactions between reference-ligand and water molecules:  ---- "<<endl;
				first = 0;
			}
			cout<<it->getName()<<" "<<it->getID()<<" : "<<score_sum<<" kJ/mol"<<endl;
		}

		// Remove water molecules that clash with the reference ligand and
		// keep those waters that interact significantly with the reference ligand and the receptor
		// OR that interact very strongly only with the receptor
		if (score_sum > 5 || (score_sum > BRIDGE_LIG_THRESHOLD && waters_strongly_recbound.find(&*it) == waters_strongly_recbound.end()))
		{
			it->select(); // mark for removal
		}
	}


	/// Write output and clean everthing up
	String outfile = par.get("o");
	if (outfile != CommandlineParser::NOT_FOUND)  // write receptor with selected water molecules to output file
	{
		receptor.removeSelected();
		PDBFile output(outfile, ios::out);
		output << receptor;
	}
	//delete scoring_function;
	delete sp;
	delete ref_ligand;
	return 0;
}
