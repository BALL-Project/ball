// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "basic.h"
#include "matcher.h"

using namespace OpenBabel;
using namespace BALL;
using namespace std;





///####################### F R A G M E N T I N G ##############################
void assignFragments(OBMol& ob_mol, 
										 Molecule& ball_mol, 
										 vector <Molecule*>& rigid_fragments, 
										 vector <Molecule*>& linker_fragments, 
										 list< pair<Atom*, Atom*> >& connections)
{
	typedef boost::disjoint_sets < int*, int*, boost::find_with_full_path_compression > DisjointSet;
	int num_atoms = ob_mol.NumAtoms();

	// create 2 empty disjoint sets:
	std::vector <int> fixed_rank(num_atoms);
	std::vector <int> fixed_parent(num_atoms);
	DisjointSet dset_rigid(&fixed_rank[0], &fixed_parent[0]);
	
	std::vector <int> linker_rank(num_atoms);
	std::vector <int> linker_parent(num_atoms);
	DisjointSet dset_linker(&linker_rank[0], &linker_parent[0]);
	
	std::vector <bool> is_rigid(num_atoms, 0); // bitVec indicating rigid atoms
	std::vector <bool> is_linker(num_atoms, 0); // bitVec indicating linker atoms

	/// iterate over all bonds and split into rigid and linker fragments:
	OBBondIterator b_it = ob_mol.BeginBonds();
	
//	Log <<"Creating fragments from your queryStructure..."<<endl;
	for (; b_it != ob_mol.EndBonds(); b_it++)
	{
//		Log << "getting OB Atoms and IDs" << endl;
		OBAtom* atm1 = (*b_it)->GetBeginAtom();
		OBAtom* atm2 =  (*b_it)->GetEndAtom();
		int id1 = atm1->GetId();
		int id2 = atm2->GetId();

//		Log << "is Bond rotor?" << endl;
		// for all rotable bonds:
		if ( (*b_it)->IsRotor() )
		{
//			cout<<"Rotor: "<<atm1->GetType()<<" - "<<atm2->GetType();
			bool isRigid_atm1 = isAtomRigid(atm1);
			bool isRigid_atm2 = isAtomRigid(atm2);
			if( !(is_linker[id1] || isRigid_atm1) ) // do not double assign (deletes disjointset-parent status)
			{
				dset_linker.make_set(id1);
				is_linker[id1]=1;
			}
			if( !(is_linker[id2] || isRigid_atm2) ) // do not double assign (deletes disjointset-parent status)
			{
				dset_linker.make_set(id2);
				is_linker[id2]=1;
			}
			// add union atoms if both are not fixed:
			if( !(isRigid_atm1 || isRigid_atm2) )// both need to be linker atoms (checked via isAtomRigid(atm) )
			{
//				cout<<" bond added! ";
				dset_linker.union_set(id1, id2);
			}
//			cout<<endl;
			
			// rotables to a rigid fragment define connection points:
			if( isAtomRigid((*b_it)->GetBeginAtom()) || isAtomRigid((*b_it)->GetEndAtom()) )
			{
//				Log << "found a connection" <<endl;
				Atom* atm1 = ball_mol.getAtom(id1);
				Atom* atm2 = ball_mol.getAtom(id2);
				connections.push_back(make_pair(atm1, atm2));
			}
		}
		// for all fixed bonds:
		else
		{
//			Log << " Got a fixed bond!"<<endl;
			if( !is_rigid[id1] ) // do not double assign (deletes disjointset-parent status)
			{
				dset_rigid.make_set(id1); 
				is_rigid[id1]=1;
			}
			if( !is_rigid[id2] ) // do not double assign (deletes disjointset-parent status)
			{
				dset_rigid.make_set(id2); 
				is_rigid[id2]=1;
			}
			dset_rigid.union_set(id1,id2);
		}
	}
	
	///	iterate over all atoms, sorting out the  fixed and linker fragments
	std::vector <int> link_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	std::vector <int> rigid_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	
//	Log<< "I am going over to fragment assignment:"<<endl;
	int parent_id = -1;
	// loop over the atoms, sort fragments out
	for(int i = 0 ; i < num_atoms; i++)
	{
		Atom* atm = ball_mol.getAtom(0); // simply pick the atoms from the top
		// atom is linker, extend or create a linker fragment
		if ( is_linker[i] )
		{
			parent_id = dset_linker.find_set(i);
			
			// if the atoms fragment does not exist, create it:
			if(link_groups[parent_id]<0)
			{
				link_groups[parent_id] = linker_fragments.size();
				Molecule* dummy = new Molecule();
				
				dummy->insert(*atm);
//				dummy->setName("Fragment_"+toString(linker_fragments.size()));
//				dummy->setProperty("isRigid", false);
				
				linker_fragments.push_back(dummy);
			}
			// atom is part of existing fragment:
			else
			{
				linker_fragments[ link_groups[parent_id] ]->insert(*atm);
			}
		}
		// Atom is a rigid, extend or create a rigid fragment
		else
		{
			parent_id = dset_rigid.find_set(i);
			// if fragment does not exist, create it:
			if(rigid_groups[parent_id]<0)
			{
				rigid_groups[parent_id] = rigid_fragments.size();
				Molecule* dummy = new Molecule();
				
				dummy->insert(*atm);
//				dummy->setName("Fragment_"+toString(rigid_fragments.size()));
//				dummy->setProperty("isRigid", true);
				rigid_fragments.push_back(dummy);
			}
			else
			{
				rigid_fragments[ rigid_groups[parent_id] ]->insert(*atm);
			}
		}
//		cout<<"END OF LOOP"<<endl;
	}
	
//	//iterate over all connections and present them to the user
//	cout << endl;
//	cout << "following inter fragment bonds exist:"<<endl;
//	list< pair<Atom*, Atom*> >::iterator coit;
//	for(coit=connections.begin(); coit!=connections.end(); coit++)
//	{
//		Atom* atm1 = coit->first;
//		Atom* atm2 = coit->second;
//		cout<<((Molecule*)atm1->getParent())->getName();
//		cout<< " to "<< ((Molecule*)atm2->getParent())->getName();
//		cout<<endl;
//	}
//	cout<<endl;
}






///####################### 3 D    A S S E M B L Y ##############################
	
/// ------ check if for every atom in list1 a matching atom in list2 
/// ------ can be found
bool allMatch(Molecule* li1, Molecule* li2)
{
	AtomIterator at1 = li1->beginAtom();
	for (; +at1 ; at1++)
	{
		bool b = false;
		
		AtomIterator at2 = li2->beginAtom();

		for (; +at2; at2++)
		{
			if (at1->getDistance(*at2) < 0.8){ // epsilon set to 0.8 A
				b = true;
				
				break;
			}
		}
		if (!b)
			return false;
	}
	return true;
}


// compare element+bondorder annotations:
bool compare(pair<String,Atom*>& a, pair<String,Atom*>& b)
{
	return a.first < b.first;
}


/// ------- get connection site
/// 'atm' the atom spanning the site and 'partner' the atom
/// on the other side of the bond that is to be formed.
/// 
int getSite(Atom* atm, vector< Atom* >& site, Atom* partner, String& key)
{
	key = atm->getElement().getSymbol();
	
	// insert central atom as first:
	site.push_back(atm);
	
	// determine the key and the order for atoms
	Atom::BondIterator b_it = atm->beginBond();
	vector< pair<String,Atom*> > elements;
	
	for(; b_it != atm->endBond(); b_it++)
	{
		Atom* tmp_atm = b_it->getBoundAtom(*atm); // get neighbors
		
		String elem = tmp_atm->getElement().getSymbol();
		elem += String(b_it->getOrder());
		
		elements.push_back( make_pair( elem, tmp_atm) );
	}
	
	// also add the partner atom for the new bond:
	elements.push_back( make_pair( String(partner->getElement().getSymbol())+ String(1), partner) );
	
	// sort identifers, but keep central atom as first:
	sort( elements.begin(), elements.end(), compare);
	
	// set the next two atoms that are NOT the partner
	// as additional points for a 3 point match site:
	int pos = -1;
	for(int i = 0; i < elements.size(); i++)
	{
		Atom* tmp = elements[i].second;

		if(tmp != partner)
			site.push_back(tmp);
		
		if(tmp == partner)
		{
			pos = i+1;
		}
	}
	
	// create the key:
	vector< pair<String, Atom*> >::iterator el_it = elements.begin();
	for(; el_it !=elements.end(); el_it++)
	{
		key += ((*el_it).first);
	}
	return pos;
}


/// get transformation vector to move atom 2 so that it has the correct distance
/// to atom 1
Vector3 getDiffVec(Atom* atm1, Atom* atm2, boost::unordered_map <String, float > std_bonds)
{
	String key = atm1->getElement().getSymbol();
	key += atm2->getElement().getSymbol();
	float bond_len = std_bonds[key];
	
	Vector3 diff_vec = atm1->getPosition() - atm2->getPosition();
	float diff_len = diff_vec.getLength() - bond_len;
	
	diff_vec.normalize();
	return (diff_vec * diff_len);
}


/// ------- merge two connection templates to a final template
/// the final template will only contain 6 atoms, 3 for each end
/// starting at position 0 and 3 with the molecules that are to be
/// connected, and then the ordered next two neighbors
void mergeTemplates(Molecule* mol1, int pos1, Molecule* mol2, int pos2, boost::unordered_map<String, float> std_bonds)
{
	Atom* aTarget = mol1->getAtom(pos1);
	Atom* bTarget = mol2->getAtom(pos2);
	Atom* atm1 = mol1->getAtom(0);
	Atom* atm2 = mol2->getAtom(0);

	// got no time to take care of the possible sign errors thus simply use 3-point-matching:
	Vector3& ptA1 = atm1->getPosition();
	Vector3& ptA2 = aTarget->getPosition();
	Vector3& ptB1 = bTarget->getPosition();
	Vector3& ptB2 = atm2->getPosition();

	Matrix4x4 rot_matrix = StructureMapper::matchPoints(ptB1, ptB2, Vector3(), ptA1, ptA2, Vector3());
	
	// transfrom the 2nd template
	TransformationProcessor transformer(rot_matrix);
	mol2->apply(transformer);
	
	// fix bond length
	Vector3 bond_fix = getDiffVec(atm1, atm2, std_bonds);
	TranslationProcessor t_later(bond_fix);
	mol2->apply(t_later);

	// remove the two connection partner atoms, they are redundant now:
	mol1->remove(*aTarget);
	mol2->remove(*bTarget);
}

/// Helper to get a single key component
String getBondName(Atom* atm, Atom* partner)
{
	String name = atm->getElement().getSymbol();
	name += atm->getBond(*partner)->getOrder();
	return name;
}

/// TODO: improve! use the already better version of 'align' from connectionRMSDFilter
/// (Structurally) align a connection site to a template and return the 
/// transformation matrix
/// 
/// What it does: finds a transformation from 'site' to 'templ', that 
/// fits all atoms in both sets. For that a 3 point match searched that fulfills
/// the condition
/// 
/// NOTE: 'all atoms fit' currently means only that positions match, order 
///       and element are neglected for now, but this seems to be still a good
///       approximation.
Matrix4x4 align(vector< Atom* >& site, Molecule* templ)
{
	Matrix4x4 result;
	Vector3 frag1, frag2, frag3, tem1, tem2, tem3;
	
	// set the two center atoms:
	frag1 = site[0]->getPosition();
	tem1  = templ->getAtom(0)->getPosition();
	
	/// simple solution for only one neighbor:
	if(site.size() == 2)
	{
		// got no time to take care of the possible sign errors thus simply use 3-point-matching:
		frag2 = site[1]->getPosition();
		tem2  = templ->getAtom(1)->getPosition();
		return StructureMapper::matchPoints(frag1, frag2, Vector3(), tem1, tem2, Vector3());
	}
	
	/// see if some atoms differ in element and order:
	// find the unique elements, via their 'bondName'
	boost::unordered_map<String, int> el_map;
	for (int i = 1; i < site.size(); i++)
	{
		el_map[ getBondName(site[i], site[0]) ] += 1;
	}
	
	/// try to assign the unique atom if one exists:
	if (el_map.size() != 1)
	{
		vector< Atom* > unique_atm;
		vector< int > unique_pos;
		vector< String > unique_names;
		boost::unordered_map<String, int>::iterator mit = el_map.begin();
		
		// find all unique identifiers:
		for (; mit != el_map.end(); mit++)
		{
			if(mit->second == 1)
				unique_names.push_back(mit->first);
		}
		
		/// a not yet captured case, but examples for this are probably very rare,
		/// because the connection to the partner is excluded, thus for all carbons
		/// we either have 3 identical bonds or at least one bond that is unique.
		if(unique_names.size() < 1)
		{
			cout<<"ERROR: currently we can't handle cases where different but"<<endl;
			cout<<" not unique bonds to a central atom exist"<<endl<<endl;
			exit(EXIT_FAILURE);
		}
		
		/// get all unique atoms (element+order) and positions 
		/// (for the matches in the template)
		for(int j = 0; j < unique_names.size(); j++)
		{
			for (int i = 1; i < site.size(); i++)
			{
				if ( getBondName(site[i], site[0]) == unique_names[j])
				{
					unique_atm.push_back(site[i]);
					unique_pos.push_back(i);
				}
			}
		}
		
		// Find mapping using the found unique atoms.
		// Take the first two, get matrix and return early:
		if(unique_atm.size() >= 2)
		{
			frag2 = unique_atm[0]->getPosition();
			frag3 = unique_atm[1]->getPosition();
			
			tem2 = templ->getAtom(unique_pos[0])->getPosition();
			tem3 = templ->getAtom(unique_pos[1])->getPosition();
			
			return StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		}
		// only one unique does exist, set it and continue with the last step
		else
		{
			frag2 = unique_atm[0]->getPosition();
			tem2 = templ->getAtom(unique_pos[0])->getPosition();
		}
	}
	
	/// all atoms have same element and order:
	/// this means we may choose one match arbitrarily and then test the 
	/// assignment for a second one iteratively
	/// OR: it might be that in the previous step we found already a single unique
	/// atom mapping
	if( frag2.isZero() ) // if frag2 was not yet initialized, arbitrarily choose
											 // the two second atoms to as matching pair
	{
		frag2 = site[1]->getPosition();
		tem2  = templ->getAtom(1)->getPosition();
	}
	
	frag3 = site[2]->getPosition();
	
	/// test all remaining assignments:
	// create test molecules:
	Molecule* dummy_frag;
	Molecule ref_frag;
	for(int i = 0; i < site.size(); i++)
		ref_frag.insert( *(new Atom (*site[i])) );

	dummy_frag = new Molecule(ref_frag, true);
	
	// assignment search loop (because a canonical ordering is not always possible for star-graphs)
	AtomIterator ati = templ->beginAtom();
	ati++; // we used first atom already
	for(; ati != templ->endAtom(); ati++ )
	{
		tem3 = ati->getPosition();
		
		if(tem2 == tem3) // it's possible that our unique assignment did set these coordinates already for tmp2
			continue;      // in that case cycle!
		
		result = StructureMapper::matchPoints(frag1, frag2, frag3, tem1, tem2, tem3);
		TransformationProcessor transformer(result);
		dummy_frag->apply(transformer);
		
		if ( allMatch(dummy_frag, templ) )
		{
			delete dummy_frag;
			return result;
		}
		else
		{
			delete dummy_frag;
			dummy_frag = new Molecule(ref_frag, true);
		}
	}
	
	cout<<"ERROR: could not match"<<endl;
	exit(EXIT_FAILURE);
	return result;
}


/// Handle connections where at least one fragment contains only one atom
void handleSimpleConnections( Atom* atm1, Atom* atm2,
															boost::unordered_map <String, float >& bondLib,
															boost::unordered_map <String, Molecule* >& connectLib)
{
	Molecule* frag1 = (Molecule*)atm1->getParent();
	Molecule* frag2 = (Molecule*)atm2->getParent();
	Molecule* single_frag;
	
	Atom* con1=atm1;
	Atom* con2=atm2;
	Bond* bnd = new Bond();
	bnd->setOrder(1);
	
	String bond_key = atm1->getElement().getSymbol();
	bond_key += atm2->getElement().getSymbol();
	
	// both fragments have just one atom
	if( frag1->countAtoms() + frag2->countAtoms() == 2)
	{
		atm1->setPosition(Vector3(0,0,0));
		atm2->setPosition(Vector3(bondLib[bond_key],0,0));

		frag1->insert(*atm2);
		return; // return early
	}
	// frag 1 is single
	else if(frag1->countAtoms() == 1)
	{
		// insert the single atom1 into frag2
		con1 = atm2;
		con2 = atm1;
		single_frag = frag2;
	}
	// frag 2 is single
	else
	{
		// insert the single atom2 into frag1
		con1 = atm1;
		con2 = atm2;
		single_frag = frag1;
	}
	
	vector< Atom* > site; String key;
	
	int pos = getSite(con1, site, con2, key);
	Molecule* templ = new Molecule(*connectLib[key]);
	
	con2->setPosition( templ->getAtom(pos)->getPosition() );
	templ->remove(*templ->getAtom(pos));
	
	// rotate the single_frag so that it aligns with the template:
	Matrix4x4 trans = align(site, templ);
	TransformationProcessor transformer(trans);
	single_frag->apply(transformer);
	
	single_frag->insert(*con2);
	con1->createBond(*bnd, *con2);
}


///  connectFragments, thereby merge connected fragments
///----------------------------------------------------------------------------
void connectFragments(Molecule* mol,
											list< pair<Atom*, Atom*> >& connections,
											boost::unordered_map <String, Molecule* >& connectLib,
											boost::unordered_map <String, float >& bondLib)
{
	list< pair<Atom*, Atom*> >::iterator con_it = connections.begin();
	
	Atom* anchor_atom = 0;
	for(; con_it != connections.end(); con_it++)
	{
		Atom* con1 = (*con_it).first;
		Atom* con2 = (*con_it).second;
		anchor_atom = con1;
		
		Molecule* frag1 = (Molecule*)con1->getParent();
		Molecule* frag2 = (Molecule*)con2->getParent();
		
//		cout<<"Connecting Atoms "<<con1<< " and "<< con2<<endl;
//		cout<<"Connecting Fragments "<<frag1<< " and "<< frag2<<endl;
		
		///0) Check for trivial cases of one being a single atom or both being single atoms
		if(frag1->countAtoms() == 1 || frag2->countAtoms() == 1)
		{
			handleSimpleConnections(con1, con2, bondLib, connectLib);
		}
		
//		cout<<"searching sites..."<<endl<<endl;
		///1) find connection sites from the two atom pointers:
		vector< Atom* > site1, site2;
		String key1, key2;
		int pos1 = getSite(con1, site1, con2, key1);
		int pos2 = getSite(con2, site2, con1, key2);
		
//		cout<<"site key1: "<<key1<<endl;
//		cout<<"site key2: "<<key2<<endl<<endl;
//		cout<<"......done!"<<endl<<endl;
		
//		cout<<"searching templates..."<<endl;
		///2) find the corresponding templates
		// create working_copies to keep the originals save!
		Molecule* templ1 = new Molecule(*connectLib[key1]);
		Molecule* templ2 = new Molecule(*connectLib[key2]);
	
//		cout<< "found templates: "<< connectLib[key1] <<" "<<templ1->countAtoms() <<" - "<<connectLib[key2]<<" "<<templ2->countAtoms()<<endl;
//		cout<<"......done!"<<endl<<endl;
		
//		cout<<"creating connected template..."<<endl;
		///3) connect the two templates to one new template
		mergeTemplates(templ1, pos1, templ2, pos2, bondLib);
//		cout<<"......done!"<<endl<<endl;
		
//		cout<<"calculating transformations for fragments to the connected template..."<<endl;
		///4) rotate both sites to match the template
		Matrix4x4 trans1 = align(site1, templ1);
		Matrix4x4 trans2 = align(site2, templ2);
//		cout<<"......done!"<<endl<<endl;
		
//		cout<<"applying the transformations to both fragments..."<<endl;
		TransformationProcessor transformer;
		transformer.setTransformation(trans1);
		frag1->apply(transformer);
		
		transformer.setTransformation(trans2);
		frag2->apply(transformer);
//		cout<<"......done!"<<endl<<endl;
		
		
//		cout<<"writing correct molecule type..."<<endl;
		///5) finishing molecule connection on data-type level:
		// form new bond:
		Bond* bnd = new Bond;
		bnd->setOrder(1);
		con1->createBond( *bnd,*(con2) );
		
		// transfer atoms to frag1:
		transferMolecule(frag1, frag2);
//		cout<<"......done!"<<endl<<endl;
		
		///6) clean up:
		delete templ1;
		delete templ2;
	}
	
	// move atoms from the last fragment back to 'mol'
	transferMolecule(mol, (Molecule*)anchor_atom->getParent() );
}



/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("3D structure generation of ligands", " generate coordinates for a combiLib", 0.1, String(__DATE__), "Prediction");
	parpars.registerParameter("i", "combiLib as CONF file", INFILE, false);
	parpars.registerParameter("o", "output molecule with 3D coordinates SDF", OUTFILE, true);
	parpars.registerParameter("l", "library configuration file", INFILE, false);
	
	parpars.setSupportedFormats("i","conf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("l","conf");
	parpars.setOutputFormatSource("i","o");

	String manual = 
			"Generate a valid (not optimized) 3D structure for structures of an"
			"entire combinatorial library which is defined in a CONF file (either "
			"containing SIMLES for each group or giving the paths multi line SMILES "
			"files).";
	
	parpars.setToolManual(manual);
	parpars.parse(argc, argv);
	

///L O A D    L I B s
	// Load all template libs:
	vector <String> libPathes(3); //0=fragments, 1=bondlenths, 2=connections
	if ( parpars.has("c") )
	{
		getLibraryPathes(libPathes, parpars.get("l"));
	}
	else
	{
		getLibraryPathes(libPathes, "/Users/pbrach/files/projects/Master-2014_2015/1_code/ball_ligandStruct/source/APPLICATIONS/LigStruct/libraries.conf");
	}
	Log<<"Configuration is:"<<endl;
	Log<<"FRAGMENTS are in:"<<endl<<libPathes[0]<<endl;
	Log<<"BONDS are in:"<<endl<<libPathes[1]<<endl;
	Log<<"CONNECTIONS are in:"<<endl<<libPathes[2]<<endl;
	
	boost::unordered_map <String, Molecule*> fragmentLib;
	boost::unordered_map <String, float > bondLib;
	boost::unordered_map <String, Molecule* > connectLib;

	Log << "loading template libs...";
	readFragmentLib(libPathes[0], fragmentLib);
	readBondLib(libPathes[1], bondLib);
	readConnectionLib(libPathes[2], connectLib);
	Log <<"done!"<<endl<<endl;
	
	
/// F R A G M E N T I N G
	Log << "Reading query molecule..."<<endl;
	OBMol ob_mol; // input query molecule
	Molecule* ball_mol = 0;
	readOBMolecule(parpars.get("i"), ob_mol);
	ball_mol = MolecularSimilarity::createMolecule(ob_mol, true);// convert from Babel to BALL molecule
	Log << "......done!"<<endl<<endl;
	
	vector <Molecule*> rigid_fragments;
	vector <Molecule*> linker_fragments;
	list< pair<Atom*, Atom*> > connections;
	
	Log << "Fragmenting the input molecule..."<<endl;
	assignFragments(ob_mol, *ball_mol, rigid_fragments, linker_fragments, connections);
	Log << "......done!"<<endl<<endl;
	
	
/// M A T C H I N G
	Log << "Matching fragments against FragmentLib..."<<endl;
	canonicalize(rigid_fragments);
	canonicalize(linker_fragments);
	
	matchFragments(fragmentLib, rigid_fragments);
	Log << "......done!"<<endl<<endl;
	
	
///// A S S E M B L E   3 D
	if( ball_mol->countAtoms() > 0 )
	{
		Log << "ERROR: not all atoms were correctly fragmented!!!"<< endl;
		exit(EXIT_FAILURE);
	}
	Log << "Connecting 3D fragments..."<<endl;
	connectFragments(ball_mol, connections, connectLib, bondLib);
	Log << "......done!"<<endl<<endl;
	
	// memory clean up (the fragment entries should now all be empty)
	checkAndDeleteFragments(rigid_fragments);
	checkAndDeleteFragments(linker_fragments);
	rigid_fragments.clear();
	linker_fragments.clear();
	
	
/// F I N I S H
/// write output
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	outfile << *ball_mol;
	Log << "wrote structure to file: "<< endl << outfile_name << endl<<endl;
	
/// Clean up
	// clean up:
	delete ball_mol;
	outfile.close();
	
	// clean db
	Log << "Removing loaded libs from memory..."<<endl;
	boost::unordered_map <BALL::String, Molecule*>::iterator map_it;
	for(map_it = fragmentLib.begin(); map_it!=fragmentLib.end(); map_it++)
		delete map_it->second;
	
	// bonds are of primitive type and on the stack
	
	for(map_it = connectLib.begin(); map_it != connectLib.end(); map_it++)
		delete map_it->second;
	
	Log << "......done!"<<endl;
}

