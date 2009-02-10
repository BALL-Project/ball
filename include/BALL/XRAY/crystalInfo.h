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
			//CrystalInfo(string group, Vector3 dim, Angle Alpha, Angle Beta, Angle Gamma);
			
			/** Default Constructor
			 */
			~CrystalInfo() throw ();
	
			bool setSpaceGroup(const string& sg);
			const string& getSpaceGroup() const;
			
			void setCellDimensions(const Vector3& dim);
			void setCellEdgeLengthA(const float& a);
			void setCellEdgeLengthB(const float& b);
			void setCellEdgeLengthC(const float& c);
			void setCellAngles(const Angle& alpha, const Angle& beta, const Angle& gamma);
			void setCellAngleAlpha(const Angle& alpha);
			void setCellAngleBeta(const Angle& beta);
			void setCellAngleGamma(const Angle& gamma);

			Size getNumberOfSymOps() const ;
			const Matrix4x4& getSymOp(Position p) const;

			Size getNumberOfNCSSymOps() const;
			const Matrix4x4& getNCS(Position p) const;
			bool insertNCS(Position p, Matrix4x4 ncsm);
			void pushbackNCS(Matrix4x4 ncsm);
			bool eraseNCS(Position p);

			const Matrix4x4& getCart2Frac() const;	
			const Matrix4x4& getFrac2Cart() const;	

		protected:

			void calculateMatrices_();
			bool retrieveSymOps_(const string& sg);

			string space_group_;
			Vector3 cell_dimensions_;
			Angle alpha_, beta_, gamma_;
			string filename_;
			
			Matrix4x4 cart2frac_;
			Matrix4x4 frac2cart_;

			vector<Matrix4x4> ncs_symops_;
			vector<Matrix4x4> sg_symops_;

	};
}	// namespace BALL

#endif // BALL_XRAY_CRYSTALINFO_H
