// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_XRAY_CRYSTALINFO_H
#define BALL_XRAY_CRYSTALINFO_H

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H
#include <BALL/CONCEPT/persistentObject.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICTRANSFORMATIONS_H
#include <BALL/STRUCTURE/geometricTransformations.h>
#endif

#include <string>

namespace BALL
{
	/** CrystalInfo class.
			
			\ingroup XRAY
	*/
	class BALL_EXPORT CrystalInfo
		: public PersistentObject
	{
		public:

		/**	@name Constant Definitions
		*/
		//@{

			/** Option Names
			 */
			struct BALL_EXPORT Option
			{
				/** The file name for the space group to symmetry operations mappings file
				 */
				static const char* SPACE_GROUP_FILE;
			};
			
			/** Default Names
			 */
			struct BALL_EXPORT Default
			{
				/** The default filename for the space group mappings file
				 */
				static const char* SPACE_GROUP_FILE;
			};

			//@}
			/** @name	Constructors and Destructor
			*/
			//@{
			//
			
			/** Default constructor
			 */
			CrystalInfo();

			/** Constructor
			 */
			CrystalInfo(string group, Vector3 dim, Angle Alpha, Angle Beta, Angle Gamma);
			
			/** Default Constructor
			 */
			~CrystalInfo() throw ();
	
			bool setSpaceGroup(string sg);
			string getSpaceGroup();
			
			void setCellDimensions(Vector3 dim);
			void setCellEdgeLengthA(float a);
			void setCellEdgeLengthB(float b);
			void setCellEdgeLengthC(float c);
			void setCellAngles(Angle alpha, Angle beta, Angle gamma);
			void setCellAngleAlpha(Angle alpha);
			void setCellAngleBeta(Angle beta);
			void setCellAngleGamma(Angle gamma);

			Size getNumberOfSymOps();
			Matrix4x4 getSymOp(Position p);

			Size getNumberOfNCSSymOps();
			Matrix4x4 getNCS(Position p);
			void pushbackNCS(Matrix4x4 ncsm);
			void dropNCS(Position p);

			Matrix4x4 getCart2Fract();	
			Matrix4x4 getFract2Cart();	

		protected:

			void calculateMatrices_();
			bool retrieveSymOps_();

			Matrix4x4 cart2frac_;
			Matrix4x4 frac2cart_;

			vector<Matrix4x4> ncs_symops_;
			vector<Matrix4x4> sg_symops_;

			Vector3 cell_dimensions_;
			Angle alpha_, beta_, gamma_;
			string space_group_;
			string filename_;

	};
}	// namespace BALL

#endif // BALL_XRAY_CRYSTALINFO_H
