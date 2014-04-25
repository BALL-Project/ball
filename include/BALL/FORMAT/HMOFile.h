// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_HMOFILE_H
#define BALL_FORMAT_HMOFILE_H

#include <BALL/FORMAT/lineBasedFile.h>

#ifndef BALL_KERNEL_ATOMCONTAINER_H
# include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
# include <BALL/MATHS/surface.h>
#endif

namespace BALL
{
	/** HMO mesh file format.
   *
	 *  This class enables BALL to read and write the HMO format for
	 *  polygonal meshes of surfaces. For the moment, we do not implement
	 *  all variants allowed by HMO, but rather focus on triangular surface
	 *  meshes. In addition, we read and write an extension to HMO (CHARGE_DATA), 
	 *  which contains information about positions and magnitudes of partially
	 *  charged atoms in the system.
	 *
	 *  \ingroup Format
	 *
	 */
	class BALL_EXPORT HMOFile 
		: public LineBasedFile
	{
		public:
		  /** @name Nested classes and enums
			 */
		  //@{
			class HMOCharge
			{
				public:
					Vector3 position;
					float   value;

					bool operator == (const HMOCharge& c) const
					{
						return ((position == c.position) && (value == c.value));
					}
			};

			enum ElementType
			{
				L2   = 60,          // line element with two nodes
				L3   = 63,          // line element with three nodes
				T3   = 103,         // triangular element with three nodes
				T6   = 106,         // triangular element with six nodes
				Q4   = 104,         // quadrilateral element with four nodes
				Q8   = 108,         // quadrilateral element with eight nodes
				TH4  = 204,         // tetrahedral element with four nodes
				TH10 = 210,         // tetrahedral element with ten nodes
				P6   = 206,         // pentahedral element with six nodes
				P15  = 215,         // pentahedral element with fifteen nodes
				H8   = 208,         // hexaedral element with eight nodes
				H20  = 220          // hexaedral element with twenty nodes
			};
		  //@}

		  /** @name Constructors and Destructor
			 */
		  //@{
			
			/// Default constructor	
			HMOFile();

			/** Detailed constructor
		   *  @throw Exception::FileNotFound if the file could not be opened
			 */
			HMOFile(const String& filename, File::OpenMode open_mode = std::ios::in);

			/// Destructor
			virtual ~HMOFile();

			//@}

			/** @name Assignment
			 */
			//@{

			/// Clear method
			virtual void clear();

			//@}
			
			/** @name Predicates
			 */
			//@{

			/// Equality operator
			bool operator == (const HMOFile& file);

			//@}

			/** @name Accessors
			 */
		  //@{

			/// Test for the presence of charges
			bool hasCharges() const
			{
				return (charges_.size() > 0);
			}

			/// Accessor for the vector of charges
			std::vector<HMOCharge>& getCharges()
			{
				return charges_;
			}

			/// Accessor for the vector of charges, const version
			std::vector<HMOCharge> const& getCharges() const
			{
				return charges_;
			}

			/// Test for the presence of comments
			bool hasComments() const
			{
				return (comments_.size() > 0);
			}

			/// Accessor for the vector of comments
			std::vector<String>& getComments()
			{
				return comments_;
			}

			/// Accessor for the vector of comments, const version
			std::vector<String> const& getComments() const
			{
				return comments_;
			}
			//@}

			/** @name Public methods for file handling
			 */
			//@{

			/** Open an HMOFile
			 *  @throw Exception::FileNotFound if the file could not be opened
			 */
			virtual bool open(const String& name, File::OpenMode open_mode = std::ios::in);

			/** Read the file into a Surface
			 *
			 *  @throw Exception::ParseError if the file was invalid
			 */
			virtual bool read(Surface& surface);

		  /** Write a given surface to an HMO file
			 *
			 *  If the vector of comments is not empty, it is prepended to the written file.
			 *  Otherwise, a generic comment is used.
			 *
			 *  Return true if the file could be written succesfully, false otherwise
			 */
			virtual bool write(Surface const& surface);

			/** Write a given surface and the charges contained in a given AtomContainer to an HMO file
			 *
			 *  If the vector of comments is not empty, it is prepended to the written file.
			 *  Otherwise, a generic comment is used.
			 *
			 *  Return true if the file could be written successfully, false otherwise
			 */
		  virtual bool write(Surface const& surface, AtomContainer const& ac);

			//@}

		protected:
			// The vector of charge positions and magnitudes
			std::vector<HMOCharge> charges_;

			// The comments contained in the original file
			std::vector<String> comments_;

			// read the node data
			void readNodeData_(Surface& surface);

			// read the element data
			void readElementData_(Surface& surface);

			// read the charge data
			void readChargeData_();

			// read until a given text is found, and store all comments along the way
			bool readUntil_(String const& pattern);

			// write the node section of the HMO file
			void writeNodes_(Surface const& surface);

			// write the elements section of the HMO file
			void writeElements_(Surface const& surface);

			// write the charges section of the HMO file
			void writeCharges_(AtomContainer const& ac);

		private:
			const HMOFile& operator = (const HMOFile& file);
	};
}

#endif // BALL_FORMAT_HMOFILE_H
