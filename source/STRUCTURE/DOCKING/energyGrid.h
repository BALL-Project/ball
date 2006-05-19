// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: energyGrid.h,v 1.1.2.1 2006/05/19 14:18:21 leonhardt Exp $
// builds energy grid for receptor and evaluates ligand


#ifndef _ENERGY_GRID_
#define _ENERGY_GRID_

#include <vector>
#include <map>


#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/regularData3D.h>


namespace BALL
{
	class System;
	class Molecule;
	
	class EnergyGrid
	{
	public:
		/** default constructor
		 */
		EnergyGrid();
	
		EnergyGrid(const String& file, System& receptor, Molecule& ligand);
	
		/** copy constructor
		 */
		EnergyGrid(const EnergyGrid& eg);
		
		/** assignment operator
		 */
		EnergyGrid& operator=(const EnergyGrid& eg);
		
		/** default constructor
		 */
		~EnergyGrid();
		
		/** setup enegy grid with receptor, ligand, filename, orgin of the cube in space defined by receptor and extension
		 */
		void setup(const String& file, System& receptor, Molecule& ligand);
		
		void getGridInfo(Vector3& t_orig, Vector3& t_ext, bool& bound);

		/** returns energy for ligand
		 */
		double getEnergy(Molecule* mol);
		
		
	private:
		
		
		/** 3 dimensional vector to store one gridcontainer for each grid point
		 */
		vector<TRegularData3D<double>*> grid_;
		
		/** vector for all elements
		 */
		std::map<short,int> elements_;

		bool bound_;
		
	};
}

#endif
