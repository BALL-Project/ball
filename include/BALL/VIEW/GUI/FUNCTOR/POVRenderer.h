// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: POVRenderer.h,v 1.9 2003/08/26 08:04:59 oliver Exp $
//

#ifndef BALL_VIEW_GUI_FUNCTOR_POVRENDERER_H
#define BALL_VIEW_GUI_FUNCTOR_POVRENDERER_H

#ifndef BALL_VIEW_GUI_KERNEL_RENDERER_H
# include <BALL/VIEW/GUI/KERNEL/renderer.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
# include <BALL/MATHS/matrix44.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class ColorRGBA;

		/** POVRenderer class.
		 		This class walks over all the geometric primitives in a Scene
				and exports them into a data file in the POVRay 1.5 format, which can
				be used to render the same scene externally.
			\ingroup ViewGuiFunctors	
		class POVRenderer : public Renderer
		{
			public:

			/** @name Constructors and Destructors.
			 */
			//@{

			/// Default constructor.
			POVRenderer()
				throw();

			/** Detailed constructor.
			 		\param name The name of the file we will create
			 */
			POVRenderer(const String& name)
				throw(Exception::FileNotFound);
			
			/// Destructor.
			virtual ~POVRenderer()
				throw();

			/// Clear method.
			virtual void clear()
				throw();

			//@}
			/** @name Accessors
			 */
			//@{

			/** Sets the name of the file we will create.
			 		\param name The file name
			 */
			void setFileName(const String& name)
				throw(Exception::FileNotFound);

			/** Converts a ColorRGBA into a String in POVRay format.
			 */
			String POVColorRGBA(const ColorRGBA& input)
				throw();

			/** Returns the corresponding BALLFinish declaration
			 */
			String POVFinish(const String& object, const ColorRGBA& input)
				throw();

			/** Converts a Vector3 into a String in POVRay format.
			 */
			String POVVector3(Vector3 input)
				throw();

			//@}
			
			/** @name Processor specific methods
			 */
			//@{
			/** Start method. 
			    This method creates the file and writes the header.
			 */
			virtual bool init(const Stage& stage)
				throw();

			/** Finish method.
			 		This method writes the ending of the file and closes it.
			 */
			virtual bool finish()
				throw();

			void renderSphere_(const Sphere& sphere)
				throw();
			
			void renderDisc_(const Disc& disc)
				throw();

			void renderTube_(const Tube& tube)
				throw();

			void renderTwoColoredTube_(const TwoColoredTube& tube)
				throw();

			void renderMesh_(const Mesh& mesh)
				throw();
			//@}

				Size width, height;
			protected:
				
				File outfile_;

				Vector3   origin_;
				Matrix4x4 rotation_;

		};
  
	} // namespace BALL
} // namespace VIEW

#endif // BALL_VIEW_GUI_FUNCTOR_POVRENDERER_H
