#ifndef BALL_GLRENDERER_GLRENDERER_H
#define BALL_GLRENDERER_GLRENDERER_H

#include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>
#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/stage.h>

#if defined(BALL_OS_DARWIN)
#	include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#else
#	include <GL/gl.h>
#	include <GL/glu.h>
#endif

#include <glDisplayList.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/VIEW/RENDERING/geometricObjectDispatcher.h>

#ifndef APIENTRY
#define APIENTRY
#endif

class QFont;

namespace BALL
{
// defines the maximal number of GL-objects, which can be selected in picking mode
// a number as big as 100.000 is needed for large molecules, just to be sure we use a million
#define BALL_GLRENDERER_PICKING_NUMBER_OF_MAX_OBJECTS 1000000

	namespace VIEW {
		class Scene;
		class GLDisplayList;
		class ColorMap;
		class RenderTarget;
	}

	namespace GLRenderer
	{
		class GLConfigDialog;
		class MeshBuffer;

		/** GLRenderer
		 		Renderer which provides hardware accelerated OPENGL rendering.
				\ingroup ViewRendering
		*/
		class BALL_PLUGIN_EXPORT GLRenderer :
			public BALL::VIEW::Renderer,
			public BALL::VIEW::GeometricObjectDispatcher
		{
			friend class BALL::VIEW::Scene;
			public:

			///
			enum RenderMode
			{
				///
				RENDER_MODE_UNDEFINED = 0,

				///
				RENDER_MODE_SOLID,

				///
				RENDER_MODE_TRANSPARENT,

				///
				RENDER_MODE_ALWAYS_FRONT
			};


			////
			enum BufferMode
			{
				/// Render without display lists, directly to the Scene
				DIRECT_RENDERING = 0,

				/// Render the contents of the display lists
				DISPLAY_LISTS_RENDERING,

				/// Rebuild the contents of the display lists and redraw them
				REBUILD_DISPLAY_LISTS
			};


			/** WrongModes Exception class.
					This exeption will be thrown if the <b> drawing_precision_</b> or
					<b> drawing_mode_</b> are not allowed.
					\see         GeneralException
			*/
			class BALL_VIEW_EXPORT WrongModes:	public BALL::Exception::GeneralException
			{
				public:

				WrongModes(const char* file, int line, int mode, int precision);
			};

			/// Typedef for OPENGL names
			typedef unsigned int Name;

			/// Default Constructor.
			explicit GLRenderer(GLConfigDialog* config_dialog = nullptr);

			/// Destructor
			~GLRenderer() override;

			/// Explicit default initialization.
			void clear() override;

			///
			void dump(std::ostream& s, BALL::Size depth) const;

			///
			inline Name getName(const BALL::VIEW::GeometricObject& object);

			///
			BALL::VIEW::GeometricObject* getObject(GLRenderer::Name name) const;

			using Renderer::init;

			/// Initialise the renderer, e.g. the display lists.
			bool init(const BALL::VIEW::Stage& stage, float width, float height) override;

			/// Set the light sources according to the stage
			void setLights(bool reset_all = false) override;

			///Should the lines in the line representation and the wireframe models
			///be smoothed?
			virtual void setSmoothLines(bool smooth_lines);
			virtual bool getSmoothLines();

			/** Pick all objects in the given screen rectangle.
			 */
			void pickObjects(BALL::Position x1, BALL::Position y1, BALL::Position x2, BALL::Position y2,
			                         std::list<BALL::VIEW::GeometricObject*>& objects) override;

			/** Pick geometric objects
			 		\param x1, y1, x2, y2 the rectangle of the selection
			*/
			void pickObjects1(BALL::Position x1, BALL::Position y1, BALL::Position x2, BALL::Position y2);

			/** Pick geometric objects method2.
			 		Call this method after pickObjects1 and rendering the representations.
					\param objects returns the picked objects
			*/
			void pickObjects2(std::list<BALL::VIEW::GeometricObject*>& objects);

			/** Set the GL_FOG intensity
			 */
			void setFogIntensity(float intensity);

			///
			void enterPickingMode();

			///
			void exitPickingMode();

			///
			void setSize(float width, float height) override;

			///
			float getXScale() const;

			///
			float getYScale() const;

			/// Update the background color from the stage
			void updateBackgroundColor() override;

			// Initialise transparent rendering
			void initTransparent();

			// Initialise solid rendering
			void initSolid();

			// Initialise always front rendering
			void initAlwaysFront();

			/// Enable or disable antialiasing
			void setAntialiasing(bool state);

			/// Remove all DisplayLists for the given Representation
			void removeRepresentation(const BALL::VIEW::Representation& rep) override;

			/// Buffer the visualisation for the given Representation into DisplayLists.
			void bufferRepresentation(const BALL::VIEW::Representation& rep) override;

			/// Draw the visualisation of the given Representation from a DisplayList.
			void drawBuffered(const BALL::VIEW::Representation& rep);

			/// Test if a Representation has a DisplayList.
			bool hasDisplayListFor(const BALL::VIEW::Representation& rep) const;

			///
			RenderMode getRenderMode() const;

			///
			void setRenderMode(RenderMode mode) { render_mode_ = mode;}

			/** This function renders into the buffer of the RenderTarget which has to be
			 *  made current before this function is called. Buffers are not automatically
			 *  swapped afterwards!
			 */
			virtual void renderToBuffer(BALL::VIEW::RenderTarget* renderTarget, BufferMode mode);

			void renderToBufferImpl(boost::shared_ptr<BALL::VIEW::FrameBuffer> /*buffer*/) override {
				renderToBuffer(0, DISPLAY_LISTS_RENDERING);
			}

			bool supports(const BALL::VIEW::PixelFormat&) const override { return true; }
			bool supports(const BALL::VIEW::FrameBufferFormat&) const override { return true; }
			void formatUpdated() override {}

			boost::shared_ptr<BALL::VIEW::RenderSetup> createRenderSetup(BALL::VIEW::RenderTarget* target,
				BALL::VIEW::Scene* scene) override;
			///
			virtual bool render(const BALL::VIEW::Representation& representation, bool for_display_list = false);

			virtual void bufferingDependentRender_(const BALL::VIEW::Representation& repr, BufferMode mode);

			/// @deprecated VBO support has been removed; does nothing
			BALL_DEPRECATED void clearVertexBuffersFor(BALL::VIEW::Representation& /* rep */) {}

			/// @deprecated VBO support has been removed; returns always false
			BALL_DEPRECATED bool vertexBuffersSupported() const { return false; };

			///
			String getVendor();

			///
			String getRenderer();

			///
			String getOpenGLVersion();

			///
			vector<String> getExtensions();

			///
			bool runningOnVirtualBox();

			/// @deprecated VBO support has been removed; returns always false
			BALL_DEPRECATED bool enableVertexBuffers(bool /* state */) { return false; }

			/// @deprecated VBO support has been removed; returns always false
			BALL_DEPRECATED bool vertexBuffersEnabled() const { return false; };

			///
			BALL::VIEW::DrawingMode getDrawingMode() const;

			///
			void initPerspective();

			///
			void setProjection();

			//_
			void setColorRGBA_(const BALL::VIEW::ColorRGBA& color);

			//_
			void vertexVector3_(const BALL::Vector3& v);

			//
			void updateCamera(const BALL::VIEW::Camera* camera = 0) override;

			//
			void setupStereo(float eye_separation, float focal_length) override;

			Position createTextureFromGrid(const BALL::VIEW::GridVisualisation& vis);
			void removeTextureFor_(const RegularData3D& grid);

			void getFrustum(float& near_f, float& far_f, float& left_f, float& right_f, float& top_f,
				float& bottom_f) override;

			void updateMaterialForRepresentation(BALL::VIEW::Representation const* rep) override
			{
				bufferRepresentation(*rep);
			}

	protected:
			bool isExtensionSupported(const char* extension) const;

			/** Maps the current viewplane to screen coordinates.
			 *  Returns false if the projection matrix is not correctly initialized.
			 */
			bool mapViewplaneToScreen_();

			void renderRepresentation_(const BALL::VIEW::Representation& representation, bool for_display_list);

			///
			void renderRepresentations_(BufferMode mode);

			///
			void renderLabel_(const BALL::VIEW::Label& /*label*/) override;

			///
			void renderLine_(const BALL::VIEW::Line& /*line*/) override;

			/// Render an illuminated line
			void renderMultiLine_(const BALL::VIEW::MultiLine& line) override;

			///
			void renderMesh_(const BALL::VIEW::Mesh& /*mesh*/) override;

			///
			void renderQuadMesh_(const BALL::VIEW::QuadMesh& /*mesh*/) override;

			///
			void initDrawingMeshes_();

			///
			void initDrawingOthers_();

			/** Render a ruler.
			 *
			 *  If supported by the renderer implementation, this function will produce
			 *  a simple ruler that is rendered together with the other representations.
			 *  The main use of this function is in the edit mode, where it can help to
			 *  straighten-up structures and to correctly estimate angles and distances.
			 */
			void renderRuler() override;

			///
			void renderPoint_(const BALL::VIEW::Point& /*point*/) override;

			///
			void renderSimpleBox_(const BALL::VIEW::SimpleBox& /*box*/) override;

			///
			void renderBox_(const BALL::VIEW::Box& /*box*/) override;

			///
			void renderSphere_(const BALL::VIEW::Sphere& /*sphere*/) override;

			///
			void renderDisc_(const BALL::VIEW::Disc& /*disc*/) override;

			///
			void renderTube_(const BALL::VIEW::Tube& /*tube*/) override;

			///
			void renderTwoColoredLine_(const BALL::VIEW::TwoColoredLine& /*two_colored_line*/) override;

			///
			void renderTwoColoredTube_(const BALL::VIEW::TwoColoredTube& /*two_colored_tube*/) override;

			///
			virtual void renderClippingPlane_(const BALL::VIEW::ClippingPlane& plane);

			/// Render a grid slice
			void renderGridVisualisation_(const BALL::VIEW::GridVisualisation& vol) override;

			//_
			void setColor4ub_(const BALL::VIEW::GeometricObject& object);

			//_
			void createSpheres_();

			//_
			void createTubes_();

			//_
			void createBoxes_();

			//_
			void createDottedSphere_(int precision);

			//_
			void subdivideTriangle_(BALL::Vector3& v1, BALL::Vector3& v2, BALL::Vector3& v3, int precision);

			//_
			void createLineBox_();

			//_
			void createDotBox_();

			//_
			void createSolidBox_();

			//_
			void clearNames_();

			//_
			void normalVector3_(const BALL::Vector3& v);


			//_
			void translateVector3_(const BALL::Vector3& v);

			//_
			void texCoordVector3_(const BALL::Vector3& v)
				{ glTexCoord3f(v.x, v.y, v.z); }

			//_
			void scaleVector3_(const BALL::Vector3& v);

			//_
			void rotateVector3Angle_(const BALL::Vector3& v, BALL::Real angle);

			//_
			void scale_(float f);


			void initGLU_(BALL::VIEW::DrawingMode mode);

			//_
			void setOrthographicZoom(float orthographic_zoom);

			//_
			float getOrthographicZoom(void);

			//_
			void generateIlluminationTexture_(float ka, float kd, float kr, float shininess);

			inline BALL::Position getTextureIndex_(BALL::Position x, BALL::Position y, BALL::Position z, BALL::Size width, BALL::Size height);
			void setupGridClipPlanes_(const BALL::VIEW::GridVisualisation& slice);

			// Sets the current OpenGL material
			void setMaterial_(const BALL::VIEW::Stage::Material& mat);
			///
			BALL::VIEW::DrawingMode 					drawing_mode_;

			///
			BALL::Index 								drawing_precision_;

			//_
			float									near_;
			//_
			float									far_;
			//_
			float									left_;
			//_
			float									right_;
			//_
			float									top_;
			//_
			float									bottom_;
			
			//_
			float 								x_scale_;

			//_
			float 								y_scale_;

			GLDisplayList* 				GL_spheres_list_;
			GLDisplayList* 				GL_tubes_list_;
			GLDisplayList* 				GL_boxes_list_;
			GLDisplayList* 				sphere_list_;
			GLDisplayList  				line_list_;
			GLuint 								line_texture_bind_;
			GLubyte*  						line_tex_;

			// naming of geometric objects
			typedef HashMap<const BALL::VIEW::GeometricObject*, Name> NameHashMap;
			typedef HashMap<Name, const BALL::VIEW::GeometricObject*> GeometricObjectHashMap;
			typedef HashMap<const BALL::VIEW::Representation*, GLDisplayList*> DisplayListHashMap;
			typedef HashMap<const BALL::VIEW::Representation*, vector<MeshBuffer*> > MeshBufferHashMap;

			GeometricObjectHashMap	name_to_object_;
			NameHashMap							object_to_name_;
			DisplayListHashMap 			display_lists_;
			MeshBufferHashMap 			rep_to_buffers_;
			Name 										all_names_;
			GLuint* 								object_buffer_;
			BALL::Vector3 								normal_vector_;

			RenderMode 							render_mode_;

			/// @deprecated VBO support has been removed; always false
			BALL_DEPRECATED bool 										use_vertex_buffer_ {false};
			bool smooth_lines_;
			bool 										picking_mode_;
			BALL::VIEW::ModelType 							model_type_;
			BALL::Position 								display_lists_index_;
			bool 										single_pick_;
			bool 										drawed_other_object_;
			bool 										drawed_mesh_;
			GLUquadricObj*  GLU_quadric_obj_;
			HashMap<const RegularData3D*, Position> grid_to_texture_;
			GLuint       						cel_texture_;

			float                   orthographic_zoom_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.iC>
#	endif

	} // namespace GLRenderer
} // namespace BALL

#endif // BALL_GLRENDERER_GLRENDERER_H
