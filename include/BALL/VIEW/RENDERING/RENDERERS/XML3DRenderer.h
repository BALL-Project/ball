// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_XML3DRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_XML3DRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
# include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
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

#ifndef BALL_MATHS_SURFACE_H
# include <BALL/MATHS/surface.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class ColorRGBA;
		class ClippingPlane;

		/** XML3DRenderer class.
		 		This class walks over all the geometric primitives in a Scene
				and exports them into a data file in the XML3DRay 1.5 format, which can
				be used to render the same scene externally.
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT XML3DRenderer : public Renderer
		{
			public:

			BALL_CREATE(XML3DRenderer)

			struct XML3DRendererClippingPlane
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
			XML3DRenderer();

			/** Detailed constructor.
			 		\param name The name of the file we will create
			 */
			XML3DRenderer(const String& name)
				throw(Exception::FileNotFound);
			
			XML3DRenderer(std::ostream& name);
			
			// Only for Python
			XML3DRenderer(const XML3DRenderer& renderer);


			/// Destructor.
			virtual ~XML3DRenderer();

			/// Clear method.
			virtual void clear();

			//@}
			/** @name Accessors
			 */
			//@{

			virtual void setSize(float width, float height);

			/** Sets the name of the file we will create.
			 		\param name The file name
			 */
			void setFileName(const String& name)
				throw(Exception::FileNotFound);

			/// Set a stream as output device
			void setOstream(std::ostream& out_stream);

			/// 
			void setHumanReadable(bool state)
				{ human_readable_ = state;}

			///
			bool isHumanReadable() const
				{ return human_readable_;}

			/** Converts a ColorRGBA into a String in XML3D format.
			 */
			String XML3DColorRGBA(const ColorRGBA& input, const String& name);

			/** Returns the corresponding BALLFinish declaration
			 */
			String XML3DFinish(const String& object, const ColorRGBA& input);

			/** Converts an Material into the corresponding shader properties.
			 */
			String XML3DRaytracingMaterial(const Stage::Material& input);

			/** Converts a Vector3 into a String in XML3DRay format.
			 */
			String XML3DVector3(Vector3 input);

			/** Prepare Strings for XML3D
			 */
			String XML3DString(const String& input);

			virtual bool renderOneRepresentation(const Representation& representation);

			//@}
			
			/** @name Processor specific methods
			 */
			//@{

			/** Initialization routine.
			*/
			virtual bool init(Scene& scene);

			/** Start method. 
			    This method creates the file and writes the header.
			 */
			virtual bool init(const Stage& stage, float width, float height);

			/** Finish method.
			 		This method writes the ending of the file and closes it.
			 */
			virtual bool finish();
			
			void createXHTMLHeader();

			void createXHTMLFooter();

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

			//@}

			protected:

				const ColorRGBA& getColor_(const GeometricObject& object);
			
				std::ostream* outfile_;
				String trimFloatValue_(float value);
				void storeColor_(const GeometricObject& object);
				String getColorIndex_(const ColorRGBA& color);
				void createTubeTransform_(const TwoColoredTube& tube);
				void createSphereTemplate_();


				Vector3   origin_;
				Matrix4x4 rotation_;
				vector<ClippingPlane*> clipping_planes_;
				bool human_readable_;

				typedef HashMap<String, Size> ColorMap;
				ColorMap color_map_;
				vector<const Representation*> representations_;
				HashSet<const Mesh*> wireframes_;
				HashSet<String> color_strings_;
				double m_[12];
				Position color_index_;
				bool create_XHTML_;

				Surface sphere_template_;
				Surface tube_template_;

				Stage::Material rt_material_;

				Index current_sphere_number_;
				Index current_tube_number_;

				float fov_x_;
				float fov_y_;
		};
  
	} // namespace BALL
} // namespace VIEW

#endif // BALL_VIEW_RENDERING_XML3DRENDERER_H
