// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: energyGrid.C,v 1.1.2.5 2006/08/24 16:42:37 leonhardt Exp $


#include <BALL/KERNEL/system.h>
#include <BALL/MATHS/vector3.h>
#include <fstream>
#include <BALL/FORMAT/HINFile.h>

#include "energyGrid.h"
#include "assignGridTypes.h"


#include "convert.h"


namespace BALL
{

	EnergyGrid::EnergyGrid()
		:bound_(false)
	{
	}
	
	EnergyGrid::EnergyGrid(const String& file,System& receptor, Molecule& ligand)
		:bound_(false)
	{
		setup(file, receptor, ligand);
	}
	

	EnergyGrid::EnergyGrid(const EnergyGrid& eg)
		:elements_(eg.elements_),
		 bound_(eg.bound_)
	{
		for (uint x = 0; x < eg.grid_.size(); x++)
			grid_.push_back(new TRegularData3D<double>(*(eg.grid_[x])));
	}  
	
	EnergyGrid& EnergyGrid::operator=(const EnergyGrid& eg)
	{
		if (this == &eg)
			return *this;
		
		for (uint x = 0; x < eg.grid_.size(); x++)
			grid_.push_back(new TRegularData3D<double>(*(eg.grid_[x])));
			
		elements_ = eg.elements_;

		bound_ = eg.bound_;
				
		return *this;
	}
	
	EnergyGrid::~EnergyGrid()
	{
		for (uint x = 0; x < grid_.size(); x++)
			delete grid_[x];
	}
	
	void EnergyGrid::setup(const String& file,System& receptor, Molecule& ligand)
	{
		
		/**open file to read elements in the gridfile
		 */
		//ifstream infile((file + ".gr").c_str());
		std::ifstream infile((file).c_str());
		
		/**vector for elements in the gridfile
		 */
		if (infile)
			{
				
				//HINFile h_infile((file + ".gr").c_str());
				HINFile h_infile((file).c_str());
				h_infile >> receptor;
				
				std::string s;
				
				while (s != "--DATA--")
					getline(infile,s);
				
				/** read elements
				 */
				uint el_size;
				infile >> el_size;
				for (uint x = 0; x < el_size ; x++)
					{
						short type;
						infile >> type;
						elements_[type]=x+1;
					}
				infile >> bound_;
			}
		else
			{
				Log.error() << "can't load grid file " << __FILE__ << " " << __LINE__<< std::endl;
				exit(1);
			}
		
		//assignGridTypes(ligand);
		
		for (AtomConstIterator ai = ligand.beginAtom(); +ai; ++ai)
			{
				//cout << ai->getType() << endl;
				std::map<short,int>::iterator it = elements_.find(gridType(*ai));
				
				if (it == elements_.end())
					{
						Log.error() << "grid file doesen't contain type " << gridType(*ai) << " " << __FILE__ << " " << __LINE__<< std::endl;
						exit(1);
					}
			}

		std::string s;
		while (s!= "--GRID--")
			getline(infile,s);
			
		for (uint e = 0; e < elements_.size() + 1; e++)
			{
				/** update screen
				 */
				Log.info() << "loading gridfile: "<< e + 1 << " of " << elements_.size()+ 1 << " elements" << std::endl; 
				
				TRegularData3D<double>* trg = new TRegularData3D<double>;
				
				std::string s = "x";
				
				std::ofstream out("aux.trg");

				while (s != "")
					{
						getline(infile,s);
						out << s << std::endl;
					}
					
				std::ifstream infile_b("aux.trg");
				
				infile_b >> *trg;
				
				grid_.push_back(trg);
			
				if (e == elements_.size())
					break;

				while (s!= "--GRID--")
					getline(infile,s);

			}
		
	}
	
	
	double EnergyGrid::getEnergy(Molecule* mol)
	{
		
		double energy = 0;

		for (AtomConstIterator ai = mol->beginAtom(); +ai; ++ai)
			{
				try
					{
						energy += grid_[0]->getInterpolatedValue(ai->getPosition()) * ai->getCharge();
				
					}
				
				catch (Exception::OutOfGrid)
					{
						if (bound_)
							energy += 100;
						
						continue;
					}
				
				vector<double> gridpoints(8);
	
				grid_[elements_[gridType(*ai)]]->getEnclosingValues(ai->getPosition(),
																														gridpoints[0],
																														gridpoints[1],
																														gridpoints[2],
																														gridpoints[3],
																														gridpoints[4],
																														gridpoints[5],
																														gridpoints[6],
																														gridpoints[7]);
				

				energy += *min_element(gridpoints.begin(),gridpoints.end());
			}
		
		return energy;
	}
	
	
	void EnergyGrid::getGridInfo(Vector3& t_orig, Vector3& t_ext, bool& bound)
	{
		if (grid_.size() == 0)
			return;
		
		t_orig = grid_[0]->getOrigin();
		t_ext = grid_[0]->getDimension();
		bound = bound_;
	}
	
}
