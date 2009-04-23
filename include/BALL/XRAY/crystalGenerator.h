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

namespace BALL
{
	/** CrystalGenerator Class
	 \ingroup XRAY
	*/
	class BALL_EXPORT CrystalGenerator
	{
		public:

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
	
			
			void setCrystalInfo(CrystalInfo& ci);
			void setSystem(System* system_ptr);	

			System* generatePacking(Index a, Index b, Index c);	

			System* generateUnitCell(Index a, Index b, Index c);	
			System* generateUnitCell();	
			System* generateAsymmetricUnit();	
			System* generateSymMoleculesWithinDistance(float angstrom);	
			
			Box3 getUnitCellBox(Index a, Index b, Index c);

		protected:
			
			bool buildUnitCell_();	
			bool buildASU_();
			
			GeometricCenterProcessor center_processor_;
			TransformationProcessor transformer_;
			
			System* system_;
			
			System* asu_;
			System* unitcell_;
			CrystalInfo* ci_ptr_;
	};
}	// namespace BALL

#endif // BALL_XRAY_CRYSTALINFO_H
