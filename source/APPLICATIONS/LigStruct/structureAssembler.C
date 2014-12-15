#ifndef STRCUTUREASSEMBLER_H
#include "structureAssembler.h"
#endif

#ifndef MATCHER_H
#include "matcher.h"
#endif

using namespace OpenBabel;
using namespace BALL;
using namespace std;


StructureAssembler::StructureAssembler()
{
	
}

// TODO: delete the libs!
StructureAssembler::~StructureAssembler()
{
	// clean db
	Log << "Removing loaded libs from memory...";
	CoordinateMap::iterator tmp_it;
	for(tmp_it = fragment_lib.begin(); tmp_it!=fragment_lib.end(); tmp_it++)
		delete tmp_it->second; // TODO: this line does not delete the contained molecules!!!!
	
	// bonds are of primitive type and on the stack
	
	ConnectionMap::iterator mit;
	for(mit = connect_lib.begin(); mit != connect_lib.end(); mit++)
		delete mit->second;
}




void StructureAssembler::setLibsFromConfig(const String& path)
{
	// get the pathes from config file
	libraryPathesFromConfig(path);
	
	// read lib files:
//	readSDFFragmentLib();
	readFragmentLib();
	readBondLib();
	readConnectionLib();
}


void StructureAssembler::assembleStructure(GroupFragment* gmol, OBMol* ob_mol)
{
	assemble_(gmol->molecule, ob_mol, gmol->frag_con2, gmol->linker_lst, gmol->rigid_lst);
}

void StructureAssembler::assembleStructure(Molecule* mol, OBMol* ob_mol)
{
	list< pair < Atom*, Atom* > > dummy_connections;
	vector< Fragment* > dummy_linker;
	vector< Fragment* > dummy_rigids;
//	list< Bond* > dummy_rotors;
	
	assemble_(mol, ob_mol, dummy_connections, dummy_linker, dummy_rigids);
}

/*
 * Private underlying method to the public assembly methods
 */
void StructureAssembler::assemble_ (Molecule* mol, OBMol* ob_mol,
																		list< pair < Atom*, Atom* > >& connections,
																		vector< Fragment* >& linker_lst, vector<Fragment *> &rigid_lst)
{
	cout<<"Fragmenting molecule"<<endl;
	OBAtom* at = ob_mol->GetAtom(1);
	cout<<at->GetType()<<endl;//DEBUG
	cout <<at->GetHyb()<<endl;
	OBBondIterator obit = at->BeginBonds();
	for (; obit != at->EndBonds(); ++obit)
	{
		cout<< (*obit)->GetBO()<<endl;
	}
	fragmentMolecule(*ob_mol, *mol, rigid_lst, linker_lst, connections);
	cout<<"Rigid: "<<rigid_lst.size()<<" inker:"<<linker_lst.size()<<endl;
	
	// canonicalize and match rigid fragments
	cout<<"canonicalizing rigid fragments"<<endl;
	canonicalize(rigid_lst);
	cout<<"matching rigid fragments"<<endl;
	matchRigidFragments(fragment_lib, rigid_lst);
	
	cout<<"building linker frgaments"<<endl;
	// build every linker fragment from standard torsions
	vector< Fragment* >::iterator lit = linker_lst.begin();
	for(; lit != linker_lst.end(); ++lit)
	{
		AssemblerFunctions::buildLinker(**lit, connect_lib);
	}
	
	cout<<"connecting the frgaments"<<endl;
	// connect the individual fragments
	list< pair < Atom*, Atom* > >::iterator cit = connections.begin();
	for(; cit != connections.end(); ++cit)
	{
		AssemblerFunctions::connectFragments(cit->first, cit->second, connect_lib, bond_lib);
	}
}


void StructureAssembler::readBondLib()
{
	LineBasedFile bondFile(bondlenth_lib_path, ios::in);
	
	while( bondFile.readLine() )
	{
		String st_ar[2];
		bondFile.getLine().split(st_ar, 2);
		
		bond_lib[st_ar[0]] = st_ar[1].toFloat();
		
		// generate also the reversed label, if both differ
		if( (st_ar[0])[0] != (st_ar[0])[1] )
		{
			String altKey = (st_ar[0])[1];
			altKey += (st_ar[0])[0];
			bond_lib[altKey] = st_ar[1].toFloat();
		}
	}
}



void StructureAssembler::readConnectionLib()
{
	connect_lib.clear();
	
	SDFile handle(connection_lib_path, ios::in); //open the lib file as sdf-file
	
	// read all lib molecules and save them with their key:
	Fragment* tmp_mol;
	tmp_mol = (Fragment*)handle.read(); // first entry
	
	int cnt = 0;
	while(tmp_mol)
	{
		BALL::String key = tmp_mol->getProperty("key").getString();
		connect_lib[key] = tmp_mol;
		
		tmp_mol = (Fragment*)handle.read();
		cnt++;
	}
	handle.close();
}




void StructureAssembler::readFragmentLib()
{
	//DEBUG:
	cout<<"reading fragment lib from: "<<fragment_lib_path<<endl;
	fragment_lib.clear();
	LineBasedFile libFile(fragment_lib_path, ios::in);
	
	// read in fragmentLib and create hash-map from that:
	String key;
	while( libFile.readLine() )
	{
		TemplateCoord* tmp_frag=0;
		if ( libFile.getLine().hasPrefix("key ") )
		{
			// get key:
			key = libFile.getLine().after("key ");
			
			// get number of positions:
			libFile.readLine();
			Size size = libFile.getLine().toUnsignedInt();
			
			// get positions:
			tmp_frag = new TemplateCoord(size);
			Size i;
			for(i = 0; i < size; i++)
			{
				libFile.readLine();
				String coords[3];
				libFile.getLine().split(coords, 3);
				Vector3& vec = (*tmp_frag)[i];
				vec.set(coords[0].toFloat(), coords[1].toFloat(), coords[2].toFloat());
			}
			
			// append to hash map
			fragment_lib[key] = tmp_frag;
		}
		else
		{
			Log << "WARNING: missed in the template coordinate lib file a line!!!"<<endl;
		}
	}
	libFile.close();
}




void StructureAssembler::readSDFFragmentLib()
{
	fragment_lib.clear();
	SDFile libFile(fragment_lib_path, ios::in);
	
	Molecule* tmp_mol;
	TemplateCoord* tmp_frag=0;
	tmp_mol = libFile.read();
	
	// read in fragmentLib and create hash-map from that:
	while(tmp_mol)
	{
		String key = tmp_mol->getProperty("key").getString(); // get key
		
		Size size = tmp_mol->countAtoms();
		tmp_frag = new TemplateCoord(size);
		fragment_lib[key] = tmp_frag; // add template to the lib
		
		Index i = 0;
		for (AtomIterator it = tmp_mol->beginAtom(); +it; ++i, ++it)
		{
			tmp_frag->get(i).set( it->getPosition() );
		}
		
		delete tmp_mol; // free memory
		
		tmp_mol = libFile.read(); //read for next iteration
	}
	libFile.close();
}


void StructureAssembler::libraryPathesFromConfig(const String& config_path)
{
	LineBasedFile configFile(config_path, ios::in);
	
	String tmp="";
	while( configFile.readLine() )
	{
		tmp = configFile.getLine();
		if(tmp.hasPrefix("#"))
			continue;
		
		if(tmp.hasPrefix("fragments=")){
			tmp = tmp.after("fragments=");
			fragment_lib_path = tmp.trim();
		}
		else if(tmp.hasPrefix("bondlenths=")){
			tmp = tmp.after("bondlenths=");
			bondlenth_lib_path = tmp.trim();
		}
		else if(tmp.hasPrefix("connections=")){
			tmp = tmp.after("connections=");
			connection_lib_path = tmp.trim();
		}
		else
			continue;
	}
}
