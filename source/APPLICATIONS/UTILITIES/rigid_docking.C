#include <iostream>
#include <set> 
#include <list>

#include <BALL/STRUCTURE/DOCKING/geometricFit.h>

////////////////
#include <BALL/SYSTEM/timer.h>

#include <BALL/COMMON/exception.h>

#include <BALL/FORMAT/PDBFile.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

using namespace std;
using namespace BALL;

int main( int argc, char **argv )
{

  Log << "be patient :)" << endl;
  // NEAR_RADIUS stands for the parameter r,
  // "Any grid point is considered inside the molecule if there is at least 
  // one atom nucleus within a distance r from it." 
  float NEAR_RADIUS = 1.8;

  // GRID_SPACING stands for the spacing between points in grid
  float GRID_SPACING = 1.0; // 1.0   

  int GRID_SIZE = 128;
  
  // SURFACE_THICKNESS stands for the thickness of the surface of the protein
  float SURFACE_THICKNESS = 1.0;
  
  // delta angle between every transformation
  // it is in unit degree
  double DEGREE_INTERVAL = 90.0;   // 

  // what surface definition should the program use.
  int SURFACE_TYPE = 1; // Connolly surface definition

  // how many peaks should the program keep for each rotation
  int TOP_N = 3;
  
  // how many overall top best peaks should the program keep 
  // after all rotations
  int BEST_NUM = 2000;

  // PDB file name
  string PDB_file_name_a = "";
  string PDB_file_name_b = "";

  // Chain name
  string CHAIN_A = "";
  string CHAIN_B = "";
  
  string REFERENCE_FILE_a = "";
  string REFERENCE_FILE_b = "";


	// Verbosity of the docking code
	int VERBOSITY = 6;

  // whether to add hydrogen atoms to the systems
  bool ADD_HYDROGENS = false;

  // usage string
  string USAGE     = "\
usage: docking -a <PDB File name for protein A>\n\
               -b <PDB File name for protein B>\n\
							[-c <chain name protein A>]\n\
							[-z <chain name protein B>]\n\
              [-r <NEAR_RADIUS>]\n\
						 	[-s <GRID_SPACING>]\n\
							[-t <SURFACE_THICKNESS>]\n\
						 	[-y <SURFACE_STYLE>]\n\
						 	[-e <GRID_SIZE>]\n\
              [-x <output_number>]\n\
							[-v <verbosity level>]\n";

  string SEPARATOR = "--------------------------------------------------";

  // command line options
  int arg_count;
  for ( arg_count = 1; arg_count < argc; ++arg_count )
  {
    if (argv[arg_count][0] == '-')
    {
      switch (argv[arg_count][1])
			{
				case 'a':  PDB_file_name_a   = string( argv[++arg_count] ); break;
				case 'c':  CHAIN_A           = string( argv[++arg_count] ); break;
				case 'b':  PDB_file_name_b   = string( argv[++arg_count] ); break;
				case 'z':  CHAIN_B           = string( argv[++arg_count] ); break;
				case 'r':  NEAR_RADIUS       = atof( argv[++arg_count] );   break;
				case 's':  GRID_SPACING      = atof( argv[++arg_count] );   break;
				case 'e':  GRID_SIZE         = atoi( argv[++arg_count] );   break;
				case 't':  SURFACE_THICKNESS = atof( argv[++arg_count] );   break;
				case 'y':  SURFACE_TYPE      = atoi( argv[++arg_count] );   break;
				case 'i':  DEGREE_INTERVAL   = atof( argv[++arg_count] );   break;
				case 'g':  ADD_HYDROGENS     = true;                        break;
				case 'x':  BEST_NUM          = atoi( argv[++arg_count] );   break;
				case 'n':  TOP_N             = atoi( argv[++arg_count] );   break;
				case 'v':  VERBOSITY         = atoi( argv[++arg_count] );   break;

				case 'h': 
									 cout << USAGE << endl;
									 exit(1);
				default:
									 cout << "invalid argument " << argv[arg_count] << endl;
			}
    }
    else
    {
      cout << USAGE << endl;
      exit(1);
    }
  }


  
  // Read some pdb file and calculate the atom number
  // and get the position of each atoms in the PDB file
  if ( (PDB_file_name_a == "") || (PDB_file_name_b == "") )
  {
    cout << "You didn't input PDB File name for protein A or B. " << endl;
    cout << USAGE << endl;
    exit(1);
  }

  System   pro_sys_a;
  Protein  pro_a;
  Molecule mol_a;
  PDBFile  infile_a( PDB_file_name_a ); 

  if ( !infile_a )
  {
     cout << "Can't open file: " << PDB_file_name_a << endl;
     return 0;
  }  
  
  if(CHAIN_A == "")  // To dock the whole protein
  {
    infile_a >> pro_sys_a;
  }
  else              // To dock some chain of the protein
  {
    infile_a >> pro_a;
    
    Chain *chn_a = NULL;
    int i = 0;
    for(ChainIterator ch_it = pro_a.beginChain(); +ch_it; ++ch_it)
    {
      if ( CHAIN_A == ch_it->getName() )
			{
				chn_a = pro_a.getChain(i);
				break;
			}
      i++;
    }
    
    if(chn_a == NULL)
		{
			cout << "no such a chain: " << CHAIN_A << " in file: " 
				<< PDB_file_name_a << "! Exit now ..." << endl;
			exit(1);
		}
    
    mol_a.insert( *chn_a );
    pro_sys_a.insert(mol_a);
  }
  infile_a.close();
  
  System   pro_sys_b;
  Protein  pro_b;
  Chain   *chn_b=0;
  Molecule mol_b;
  PDBFile  infile_b( PDB_file_name_b ); 
  
  if ( !infile_b )
  {
    cout << "Can't open file: " << PDB_file_name_b << endl;
    return 0;
  }
  
  if(CHAIN_B == "")
  {
    infile_b >> pro_sys_b;
  }
  else
  {
    infile_b >> pro_b;
    
    int i = 0;
    for(ChainIterator ch_it = pro_b.beginChain(); +ch_it; ++ch_it)
    {
      if ( CHAIN_B == ch_it->getName() )
      {
	chn_b = pro_b.getChain(i);
	break;
      }
      i++;
    }
    
    if(chn_b == NULL)
    {
      cout << "no such a chain: " << CHAIN_B << " in file: " 
	   << PDB_file_name_b << "! Exit now ..." << endl;
      exit(1);
    }
    
    mol_b.insert( *chn_b );
    pro_sys_b.insert(mol_b);    
  }
  infile_b.close();
   
  // add hydrogen atoms
  if( ADD_HYDROGENS )
  {
    FragmentDB fragment_db("");
    pro_sys_a.apply(fragment_db.add_hydrogens);
    pro_sys_b.apply(fragment_db.add_hydrogens);
  }
   
  GeometricFit geo_fit( pro_sys_a, pro_sys_b );
   
	geo_fit.options[GeometricFit::Option::NEAR_RADIUS] = NEAR_RADIUS;
	geo_fit.options[GeometricFit::Option::GRID_SPACING] = GRID_SPACING;
	geo_fit.options[GeometricFit::Option::GRID_SIZE] = GRID_SIZE;
	geo_fit.options[GeometricFit::Option::SURFACE_THICKNESS] = SURFACE_THICKNESS;
	geo_fit.options[GeometricFit::Option::DEGREE_INTERVAL] = DEGREE_INTERVAL;
	geo_fit.options[GeometricFit::Option::TOP_N] = TOP_N;
	geo_fit.options[GeometricFit::Option::BEST_NUM] = BEST_NUM;
	geo_fit.options[GeometricFit::Option::VERBOSITY] = VERBOSITY;

  // keep the larger protein in System A and the smaller one in System B
  if ( pro_sys_a.countAtoms() < pro_sys_b.countAtoms() )
  {
    // swap the systems
    pro_sys_a.swap( pro_sys_b );
    
    // swap the other info
    PDB_file_name_a.swap( PDB_file_name_b );
    CHAIN_A.swap( CHAIN_B );
  }
  
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   // echo the parameters
	 Options geo_opt = geo_fit.options;

   cout << "-----Parameters-----" << endl;
   cout << "NEAR_RADIUS       = " << NEAR_RADIUS       
		 		<< "|||" << geo_opt[GeometricFit::Option::NEAR_RADIUS] << endl;
   cout << "GRID_SPACING      = " << GRID_SPACING      
		 		<< "|||" << geo_opt[GeometricFit::Option::GRID_SPACING] << endl;
   cout << "GRID_SIZE         = " << GRID_SIZE         
		    << "|||" << geo_opt[GeometricFit::Option::GRID_SIZE] << endl;
   cout << "SURFACE_THICKNESS = " << SURFACE_THICKNESS 
		 		<< "|||" << geo_opt[GeometricFit::Option::SURFACE_THICKNESS] << endl;
   cout << "DEGREE_INTERVAL   = " << DEGREE_INTERVAL   
		 		<< "|||" << geo_opt[GeometricFit::Option::DEGREE_INTERVAL] << endl;
   cout << "TOP_N             = " << TOP_N             
		 		<< "|||" << geo_opt[GeometricFit::Option::TOP_N] << endl;
   cout << "BEST_NUM          = " << BEST_NUM          
		 		<< "|||" << geo_opt[GeometricFit::Option::BEST_NUM] << endl;
   cout << endl;
   cout << "------System A------" << endl;
   cout << "PDB File name a   = " << PDB_file_name_a   << endl;
   cout << "Chain name        = " << CHAIN_A           << endl;
   cout << "residue number    = " << pro_sys_a.countResidues() << endl;
   cout << "atom number       = " << pro_sys_a.countAtoms()    << endl;
   cout << endl;
   cout << "------System B------" << endl;
   cout << "PDB File name b   = " << PDB_file_name_b   << endl;  
   cout << "Chain name        = " << CHAIN_B           << endl;
   cout << "residue number    = " << pro_sys_b.countResidues() << endl;
   cout << "atom number       = " << pro_sys_b.countAtoms()    << endl;
   cout << endl;
   
	 geo_fit.setup(pro_sys_a, pro_sys_b);
	 geo_fit.start();

	 RankedConformations rc = geo_fit.getRankedConformations(BEST_NUM);
	 rc.closeTrajectoryFile();
	 System S = rc.getSystem();
	 
	 PDBFile outfile("docking.pdb", std::ios::out);
	 outfile << S;
	 outfile.close();

	 return 0;
}
