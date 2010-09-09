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

#ifndef BALL_DATATYPE_STRING_H
#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#	include <BALL/CONCEPT/persistenceManager.h>
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
				static const string SPACE_GROUP_FILE;
			};
			
			/** Default Names
			 */
			struct BALL_EXPORT Default
			{
				/** The default filename for the space group mappings file
				 */
				static const string SPACE_GROUP_FILE;
			};

			//@}
			/** @name	Constructors and Destructor
			*/
			//@{
			//
			
			/** Default constructor
			 */
			CrystalInfo();

			/** Detailed Constructor
			 */
			CrystalInfo(String group, Vector3 dim, Angle alpha, Angle beta, Angle gamma);
			
			/** Copy Constructor
			 */
			CrystalInfo(const CrystalInfo& ci);
		
			BALL_CREATE(CrystalInfo)
			/** Default Constructor
			 */
			~CrystalInfo();
			//@}
	
			bool setSpaceGroup(const String& sg);
			const String& getSpaceGroup() const;
			
			void setCellDimensions(const Vector3& dim);
			
			void setCellEdgeLengthA(const float& a);
			const float& getCellEdgeLengthA() const;
			
			void setCellEdgeLengthB(const float& b);
			const float& getCellEdgeLengthB() const;
			
			void setCellEdgeLengthC(const float& c);
			const float& getCellEdgeLengthC() const;
			
			void setCellAngles(const Angle& alpha, const Angle& beta, const Angle& gamma);
			
			void setCellAngleAlpha(const Angle& alpha);
			const Angle& getCellAngleAlpha() const;
			
			void setCellAngleBeta(const Angle& beta);
			const Angle& getCellAngleBeta() const;
			
			void setCellAngleGamma(const Angle& gamma);
			const Angle& getCellAngleGamma() const;

			void setZScore(const int& zscore);
			const int& getZScore() const;
			
			Size getNumberOfSymOps() const ;
			const Matrix4x4& getSymOp(Position p) const;

			Size getNumberOfNCSSymOps() const;

			/** Returns the p-th NCS
			 *  @throw Exception::IndexOverflow if p >= getNumberOfNCSSymOps()
			 */
			const Matrix4x4& getNCS(Position p) const;
			
			/** Returns a mutable reference to the p-th NCS
			 *  @throw Exception::IndexOverflow if p >= getNumberOfNCSSymOps()
			 */
			Matrix4x4& getNCS(Position p);
			
			/** Returns true is the p-th NCS is given
			 *  @throw Exception::IndexOverflow if p >= getNumberOfNCSSymOps()
			 */
			bool isgivenNCS(Position p) const;
			
			/** Insert an NCS
			 *  @throw Exception::IndexOverflow if p >= getNumberOfNCSSymOps()
			 */
			bool insertNCS(Position p, Matrix4x4 ncsm, bool is_given = 0);
			
			void pushbackNCS(Matrix4x4 ncsm, bool is_given = 0);
			
			/** Erase an NCS
			 *  @throw Exception::IndexOverflow if p >= getNumberOfNCSSymOps()
			 */
			bool eraseNCS(Position p);

			const Matrix4x4& getCart2Frac() const;	
			const Matrix4x4& getFrac2Cart() const;	
			
			/** Resets the options to default values.
			*/
			void setDefaultOptions();
			
			/** @name Public Attributes
			*/
			//@
			/// options
			Options options;

			//@}

			/** @name Storable Interface
			*/
			//@{

			/** Persistent stream writing.
			*/
			//void write(PersistenceManager& pm) const;
			void persistentWrite(PersistenceManager& pm, const char* name) const;

			/** Persistent stream reading.
			*/
			//bool read(PersistenceManager& pm);
			void persistentRead(PersistenceManager& pm);

			//@}


		protected:

			void calculateMatrices_();
			bool retrieveSymOps_(const String& sg);
			
			/** Reads, checks and stores the options.   
		  	*
				* @return bool - false if one of the options got an invalid value.
				* @return bool - true otherwise
				*/
			//bool readOptions_();

			String space_group_;
			Vector3 cell_dimensions_;
			Angle alpha_, beta_, gamma_;
			int z_score_;
			String filename_;
			
			Matrix4x4 cart2frac_;
			Matrix4x4 frac2cart_;

			vector<Matrix4x4> ncs_symops_;
			vector<bool> ncs_isgiven_;
			vector<Matrix4x4> sg_symops_;

	};
}	// namespace BALL

#endif // BALL_XRAY_CRYSTALINFO_H
