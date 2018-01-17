// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_GLRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_GLRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
#	include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#endif

#ifndef BALL_MATHS_QUATERNION_H
# include <BALL/MATHS/quaternion.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#if defined(BALL_OS_DARWIN)
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

#ifndef BALL_VIEW_RENDERING_GLDISPLAYLIST_H
# include <glDisplayList.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
# include <BALL/DATATYPE/regularData3D.h>
#endif

#ifndef BALL_VIEW_RENDERING_GEOMETRICOBJECTDISPATCHER_H
# include <BALL/VIEW/RENDERING/geometricObjectDispatcher.h>
#endif

#ifndef APIENTRY
#define APIENTRY
#endif

class QFont;

namespace BALL
{
// defines the maximal number of GL-objects, which can be selected in picking mode
// a number as big as 100.000 is needed for large molecules, just to be sure we use a million
#define BALL_GLRENDERER_PICKING_NUMBER_OF_MAX_OBJECTS 1000000
	namespace VIEW
	{
		class Scene;
		class GLDisplayList;
		class MeshBuffer;
		class ColorMap;
		class RenderTarget;

		/** GLRenderer
		 		Renderer which provides hardware accelerated OPENGL rendering.
				\ingroup ViewRendering
		*/
		class BALL_PLUGIN_EXPORT GLRenderer
			: public Renderer, public GeometricObjectDispatcher
		{
			friend class Scene;
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
			class BALL_VIEW_EXPORT WrongModes:	public Exception::GeneralException
			{
				public:

				WrongModes(const char* file, int line, int mode, int precision);
			};

			/// Typedef for OPENGL names
			typedef unsigned int Name;

			/// Default Constructor.
			GLRenderer();

			/// Destructor
			virtual ~GLRenderer();

			/// Explicit default initialization.
			virtual void clear();

			///
			void dump(std::ostream& s, Size depth) const;

			///
			inline Name getName(const GeometricObject& object);

			///
			GeometricObject* getObject(GLRenderer::Name name) const;

			using Renderer::init;

			/// Initialise the renderer, e.g. the display lists.
			virtual bool init(const Stage& stage, float width, float height);

			/// Set the light sources according to the stage
			virtual void setLights(bool reset_all = false);

			///Should the lines in the line representation and the wireframe models
			///be smoothed?
			virtual void setSmoothLines(bool smooth_lines);
			virtual bool getSmoothLines();

			/** Pick all objects in the given screen rectangle.
			 */
			virtual void pickObjects(Position x1, Position y1, Position x2, Position y2, 
			                         std::list<GeometricObject*>& objects);

			/** Pick geometric objects
			 		\param x1, y1, x2, y2 the rectangle of the selection
			*/
			void pickObjects1(Position x1, Position y1, Position x2, Position y2);

			/** Pick geometric objects method2.
			 		Call this method after pickObjects1 and rendering the representations.
					\param objects returns the picked objects
			*/
			void pickObjects2(std::list<GeometricObject*>& objects);

			/** Set the GL_FOG intensity
			 */
			void setFogIntensity(float intensity);

			///
			void enterPickingMode();

			///
			void exitPickingMode();

			///
			void setSize(float width, float height);

			///
			float getXScale() const;

			///
			float getYScale() const;

			/// Update the background color from the stage
			virtual void updateBackgroundColor();

			// Initialise transparent rendering
			void initTransparent();

			// Initialise solid rendering
			void initSolid();

			// Initialise always front rendering
			void initAlwaysFront();

			/// Enable or disable antialiasing
			void setAntialiasing(bool state);

			/// Remove all VertexBuffer and DisplayLists for the given Representation
			void removeRepresentation(const Representation& rep);

			/// Buffer the visualisation for the given Representation into OpenGL VertexBuffer Objects and DisplayLists.
			void bufferRepresentation(const Representation& rep);

			/// Draw the visualisation of the given Representation from the VertexBuffers and a DisplayList.
			void drawBuffered(const Representation& rep);

			/// Test if a Representation has a DisplayList.
			bool hasDisplayListFor(const Representation& rep) const;

			///
			RenderMode getRenderMode() const;

			///
			void setRenderMode(RenderMode mode) { render_mode_ = mode;}

			/** This function renders into the buffer of the RenderTarget which has to be
			 *  made current before this function is called. Buffers are not automatically
			 *  swapped afterwards!
			 */
			virtual void renderToBuffer(RenderTarget* renderTarget, BufferMode mode);

			virtual void renderToBufferImpl(boost::shared_ptr<FrameBuffer> /*buffer*/) {
				renderToBuffer(0, DISPLAY_LISTS_RENDERING);
			}

			virtual bool supports(const BALL::VIEW::PixelFormat&) const { return true; }
			virtual bool supports(const BALL::VIEW::FrameBufferFormat&) const { return true; }
			virtual void formatUpdated() {}

			virtual boost::shared_ptr<RenderSetup> createRenderSetup(RenderTarget* target, Scene* scene);
			///
			virtual bool render(const Representation& representation, bool for_display_list = false);

			virtual void bufferingDependentRender_(const Representation& repr, BufferMode mode);

			///
			void clearVertexBuffersFor(Representation& rep);

			///
			bool vertexBuffersSupported() const;

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

			///
			bool enableVertexBuffers(bool state);

			///
			bool vertexBuffersEnabled() const;

			///
			DrawingMode getDrawingMode() const;

			///
			void initPerspective();

			///
			void setProjection();

			//_
			void setColorRGBA_(const ColorRGBA& color);

			//_
			void vertexVector3_(const Vector3& v);

			//
			void updateCamera(const Camera* camera = 0);

			//
			void setupStereo(float eye_separation, float focal_length);

			Position createTextureFromGrid(const GridVisualisation& vis);
			void removeTextureFor_(const RegularData3D& grid);

			virtual void getFrustum(float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f);

			void updateMaterialForRepresentation(Representation const* rep) { bufferRepresentation(*rep); }

	protected:
			bool isExtensionSupported(const char* extension) const;

			/** Maps the current viewplane to screen coordinates.
			 *  Returns false if the projection matrix is not correctly initialized.
			 */
			bool mapViewplaneToScreen_();

			void renderRepresentation_(const Representation& representation, bool for_display_list);

			///
			void renderRepresentations_(BufferMode mode);

			///
			virtual void renderLabel_(const Label& /*label*/);

			///
			virtual void renderLine_(const Line& /*line*/);

			/// Render an illuminated line
			virtual void renderMultiLine_(const MultiLine& line);

			///
			virtual void renderMesh_(const Mesh& /*mesh*/);

			///
			virtual void renderQuadMesh_(const QuadMesh& /*mesh*/);

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
			virtual void renderRuler();

			///
			virtual void renderPoint_(const Point& /*point*/);

			///
			virtual void renderSimpleBox_(const SimpleBox& /*box*/);

			///
			virtual void renderBox_(const Box& /*box*/);

			///
			virtual void renderSphere_(const Sphere& /*sphere*/);

			///
			virtual void renderDisc_(const Disc& /*disc*/);

			///
			virtual void renderTube_(const Tube& /*tube*/);

			///
			virtual void renderTwoColoredLine_(const TwoColoredLine& /*two_colored_line*/);

			///
			virtual void renderTwoColoredTube_(const TwoColoredTube& /*two_colored_tube*/);

			///
			virtual void renderClippingPlane_(const ClippingPlane& plane);

			/// Render a grid slice
			virtual void renderGridVisualisation_(const GridVisualisation& vol);

			//_
			void setColor4ub_(const GeometricObject& object);

			//_
			void createSpheres_();

			//_
			void createTubes_();

			//_
			void createBoxes_();

			//_
			void createDottedSphere_(int precision);

			//_
			void subdivideTriangle_(Vector3& v1, Vector3& v2, Vector3& v3, int precision);

			//_
			void createLineBox_();

			//_
			void createDotBox_();

			//_
			void createSolidBox_();

			//_
			void clearNames_();

			//_
			void normalVector3_(const Vector3& v);


			//_
			void translateVector3_(const Vector3& v);

			//_
			void texCoordVector3_(const Vector3& v)
				{ glTexCoord3f(v.x, v.y, v.z); }

			//_
			void scaleVector3_(const Vector3& v);

			//_
			void rotateVector3Angle_(const Vector3& v, Real angle);

			//_
			void scale_(float f);


			void initGLU_(DrawingMode mode);

			//_
			void setOrthographicZoom(float orthographic_zoom);

			//_
			float getOrthographicZoom(void);

			//_
			void generateIlluminationTexture_(float ka, float kd, float kr, float shininess);

			inline Position getTextureIndex_(Position x, Position y, Position z, Size width, Size height);
			void setupGridClipPlanes_(const GridVisualisation& slice);

			// Sets the current OpenGL material
			void setMaterial_(const Stage::Material& mat);
			///
			DrawingMode 					drawing_mode_;

			///
			Index 								drawing_precision_;

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
			typedef HashMap<const GeometricObject*, Name> NameHashMap;
			typedef HashMap<Name, const GeometricObject*> GeometricObjectHashMap;
			typedef HashMap<const Representation*, GLDisplayList*> DisplayListHashMap;
			typedef HashMap<const Representation*, vector<MeshBuffer*> > MeshBufferHashMap;

			GeometricObjectHashMap	name_to_object_;
			NameHashMap							object_to_name_;
			DisplayListHashMap 			display_lists_;
			MeshBufferHashMap 			rep_to_buffers_;
			Name 										all_names_;
			GLuint* 								object_buffer_;
			Vector3 								normal_vector_;

			RenderMode 							render_mode_;

			bool 										use_vertex_buffer_;
			bool smooth_lines_;
			bool 										picking_mode_;
			ModelType 							model_type_;
			Position 								display_lists_index_;
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

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_GLRENDERER_H
