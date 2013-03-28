// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: POVRenderer.h,v 1.12.16.1 2007/03/25 21:26:13 oliver Exp $
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_POVRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_POVRENDERER_H

#ifndef BALL_VIEW_RENDERING_SCENEEXPORTER_H
# include <BALL/VIEW/RENDERING/sceneExporter.h>
#endif

#ifndef BALL_VIEW_RENDERING_GEOMETRICOBJECTDISPATCHER_H
# include <BALL/VIEW/RENDERING/geometricObjectDispatcher.h>
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
		class ClippingPlane;

		/** POVRenderer class.
		 		This class walks over all the geometric primitives in a Scene
				and exports them into a data file in the POVRay 1.5 format, which can
				be used to render the same scene externally.
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT POVRenderer : public SceneExporter, public GeometricObjectDispatcher
		{
			public:

			struct POVRendererClippingPlane
			{
				public:
					Vector3 normal;
					float translation;
//					 Vector3 translation;
			};

			/** @name Constructors and Destructors.
			 */
			//@{

			/// Default constructor.
			POVRenderer(std::ostream* strm);

			/** Detailed constructor.
			 		\param name The name of the file we will create
			 */
			POVRenderer(const String& name);

			/// Destructor.
			virtual ~POVRenderer();

			/// Clear method.
			virtual void clear();

			//@}
			/** @name Accessors
			 */
			//@{

			/** Converts a ColorRGBA into a String in POVRay format.
			 */
			String POVColorRGBA(const ColorRGBA& input);

			/** Returns the corresponding BALLFinish declaration
			 */
			String POVFinish(const String& object, const ColorRGBA& input);

			/** Converts a Vector3 into a String in POVRay format.
			 */
			String POVVector3(Vector3 input);

			virtual bool exportOneRepresentation(const Representation* representation);

			//@}
			
			/** @name Processor specific methods
			 */
			//@{

			/** Start method. 
			    This method creates the file and writes the header.
			 */
			virtual bool init(const Stage* stage, float width, float height);

			//@}
			protected:
				/**
				 * Finish method.
				 * This method writes the ending of the file and closes it.
				 */
				virtual bool finishImpl_();

				void renderSphere_(const Sphere& sphere);

				void renderDisc_(const Disc& disc);

				void renderTube_(const Tube& tube);

				void renderTwoColoredTube_(const TwoColoredTube& tube);

				void renderMesh_(const Mesh& mesh);

				void renderTwoColoredLine_(const TwoColoredLine& line);

				void renderLine_(const Line& line);

				void renderPoint_(const Point& point);

				// do nothing
				void renderLabel_(const Label&);

				/// Render an illuminated line
				virtual void renderMultiLine_(const MultiLine& line);

				const ColorRGBA& getColor_(const GeometricObject& object);
				String trimFloatValue_(float value);
				void storeColor_(const GeometricObject& object);
				String getColorIndex_(const ColorRGBA& color);

				Vector3   origin_;
				Matrix4x4 rotation_;
				vector<ClippingPlane*> clipping_planes_;

				typedef HashMap<String, Size> ColorMap;
				ColorMap color_map_;
				vector<const Representation*> representations_;
				HashSet<const Mesh*> wireframes_;
				HashSet<String> color_strings_;
				double m_[12];
				Position color_index_;

				float width_;
				float height_;
				const Stage* stage_;
		};
  
	} // namespace BALL
} // namespace VIEW

#endif // BALL_VIEW_RENDERING_POVRENDERER_H
