// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_XRAY_CRYSTALGENERATOR_H
#define BALL_XRAY_CRYSTALGENERATOR_H

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#include <BALL/DATATYPE/regularData3D.h>
#endif

#ifndef BALL_XRAY_CRYSTALINFO_H
#include <BALL/XRAY/crystalInfo.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICTRANSFORMATIONS_H
#include <BALL/STRUCTURE/geometricTransformations.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICPROPERTIES_H
# include <BALL/STRUCTURE/geometricProperties.h>
#endif

#ifndef BALL_MATHS_BOX3_H
# include <BALL/MATHS/box3.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
# include <BALL/DATATYPE/list.h>
#endif

namespace BALL
{
	/** CrystalGenerator Class
	 \ingroup XRAY
	*/
	class BALL_EXPORT CrystalGenerator
	{
		public:


			/** @name Enums
			*/
			//@{

			/**
			*/
			enum CellType
			{
				NCS_ASU 	= 0,
				ASU     	= 1,
				UNITCELL	= 2
			};


			/** @name constructors and Destructors
			*/
			//@{
			/**	Default Constructor.
			 		This methods creates a new CrystalGenerator object
				
			*/
			CrystalGenerator();
			
			/**	Copy Constructor.
			 		Creates a new CrystalGenerator object from another
					@param	cg	the CrystalGenerator to be copied from
				
			*/
			CrystalGenerator(const CrystalGenerator& cg);

			/**	Detailed Constructor.
					This methods creates a new CrystalGenerator object from a given System
					@param	system	a pointer to an existing System
			*/
			CrystalGenerator(System* system);
			
			/**	Destructor.
			*/
			~CrystalGenerator();
			//@}
	
			
			void setCrystalInfo(boost::shared_ptr<CrystalInfo> ci_ptr);
			void setSystem(System* system_ptr);	

			List<System*> generatePacking(Index a_loweridx, Index a_upperidx, Index b_loweridx, Index b_upperid, Index c_loweridx, Index c_upperid);	

			System* generateUnitCell(Index a, Index b, Index c);	
			System* generateUnitCell();	
			System* generateAsymmetricUnit();	
			List<System*> generateSymMoleculesWithinDistance(float angstrom);	
			
			Box3 getUnitCellBox(Index a, Index b, Index c);

		protected:
			
			bool buildUnitCell_();	
			bool buildASU_();
			bool correctASUPositions_(System* raw_cell);
			
			GeometricCenterProcessor center_processor_;
			TransformationProcessor transformer_;
			
			System* system_;
			
			System* asu_;
			System* unitcell_;
			boost::shared_ptr<CrystalInfo> ci_ptr_;
	};
}	// namespace BALL

#endif // BALL_XRAY_CRYSTALINFO_H
