// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glRenderer.h,v 1.16 2004/05/27 19:49:50 oliver Exp $
//

#ifndef BALL_VIEW_RENDERING_GLRENDERER_H
#define BALL_VIEW_RENDERING_GLRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERER_H
#	include <BALL/VIEW/RENDERING/renderer.h>
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

#include <qgl.h>

namespace BALL
{
// defines the maximal number of GL-objects, which can be selected in picking mode
// a number as big as 100.000 is needed for large molecules, just to be sure we use a million
#define BALL_GLRENDERER_PICKING_NUMBER_OF_MAX_OBJECTS 1000000
	namespace VIEW
	{
		class GLDisplayList;

		/** GLRenderer
		 		Renderer which provides hardware accelerated OPENGL rendering.
				\ingroup ViewRendering
		*/
		class BALL_EXPORT GLRenderer: public Renderer
		{
			public:

			/** WrongModes Exception class.
					This exeption will be thrown if the <b> drawing_precision_</b> or
					<b> drawing_mode_</b> are not allowed. 
					\see         GeneralException			
			*/
			class WrongModes:	public Exception::GeneralException
			{
				public:

				WrongModes(const char* file, int line, int mode, int precision)
					throw();
			};

			/** @name Type Definitions
			*/
			//@{
			
			/// Typedef for OPENGL names
			typedef unsigned int Name;
			
			//@}
			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			GLRenderer()
				throw();

			/** Destructor
			*/
			virtual ~GLRenderer()
				throw();

			/** Explicit default initialization.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{		

			///
			void dump(std::ostream& s, Size depth) const
				throw();

			///
			Name getName(const GeometricObject& object)
				throw();

			///
			GeometricObject* getObject(GLRenderer::Name name) const
				throw();

			/** Initialise the display lists.
			 		This method is called by Scene::initializeGL.
			*/
			virtual bool init(const Stage& stage, float width, float height)
				throw();

			/// Set the light sources according to the stage
			virtual void setLights()
				throw();

			/** Pick geometric objects
			 		\param x1, y1, x2, y2 the rectangle of the selection
			*/
			void pickObjects1(float x1, float y1, float x2, float y2)
				throw();

			/** Pick geometric objects method2.
			 		Call this method after pickObjects1 and rendering the representations.
					\param objects returns the picked objects
					\param width the width
					\param height the height
			*/
			void pickObjects2(List<GeometricObject*>& objects, int width, int height)
				throw();

			/**
			 */
			void setSize(float width, float height)
				throw();

			///
			float getXScale() const
				throw() { return x_scale_;}

			///
			float getYScale() const
				throw() { return y_scale_;}

			///
			void updateCamera()
				throw();

			/// Update the background color from the stage
			void updateBackgroundColor()
				throw();

			// Initialise transparent rendering
			void initTransparent() 
				throw();

			// Initialise solid rendering
			void initSolid()
				throw();
			
			///
			void buildDisplayListFor(const Representation& rep)
				throw();

			///
			void removeDisplayListFor(const Representation& rep)
				throw();

			///
			void rebuildDisplayListFor(const Representation& rep)
				throw();

			///
			void drawFromDisplayList(const Representation& rep)
				throw();
			
			///
			void setStereoMode(bool on)
				throw();

			///
			bool isInStereoMode() const
				throw() { return stereo_;}
			
			///
			virtual bool render(const Representation& representation)
				throw();

			//@}
			protected:

			///
			virtual void render_(const GeometricObject* object)
				throw();

			///
			virtual void renderLabel_(const Label& /*label*/)
				throw();

			///
			virtual void renderLine_(const Line& /*line*/)
				throw();

			///
			virtual void renderMesh_(const Mesh& /*mesh*/)
				throw();

			///
			virtual void renderPoint_(const Point& /*point*/)
				throw();

			///
			virtual void renderSimpleBox_(const SimpleBox& /*box*/)
				throw();

			///
			virtual void renderBox_(const Box& /*box*/)
				throw();

			///
			virtual void renderSphere_(const Sphere& /*sphere*/)
				throw();

			///
			virtual void renderDisc_(const Disc& /*disc*/)
				throw();

			///
			virtual void renderTube_(const Tube& /*tube*/)
				throw();

			///
			virtual void renderTwoColoredLine_(const TwoColoredLine& /*two_colored_line*/)
				throw();

			///
			virtual void renderTwoColoredTube_(const TwoColoredTube& /*two_colored_tube*/)
				throw();

			//_
			void setColor4ub_(const GeometricObject& object)
				throw(){ setColorRGBA_(object.getColor()); }

			//_
			GLubyte* generateBitmapFromText_(const String& text, int& width, int& height) const
				throw();

			//_
			void createSpheres_()
				throw();
			
			//_
			void createTubes_()
				throw();

			//_
			void createBoxes_()
				throw();

			//_
			void createDottedSphere_(int precision)
				throw();
			
			//_
			void subdivideTriangle_(Vector3& v1, Vector3& v2, Vector3& v3, int precision)
				throw();

			//_
			void createLineBox_()
				throw();

			//_
			void createDotBox_()
				throw();

			//_
			void createSolidBox_()
				throw();

			//_
			void clearNames_()
				throw();

			//_
			void normalVector3_(const Vector3& v) 
				throw() { glNormal3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);}

			//_
			void vertexVector3_(const Vector3& v)
				throw() { glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z); }

			//_
			void translateVector3_(const Vector3& v)
				throw() { glTranslatef((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z); }

			//_
			void scaleVector3_(const Vector3& v)
				throw() { glScalef((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z); }

			//_
			void rotateVector3Angle_(const Vector3& v, Real angle)
				throw() { glRotatef(angle, v.x, v.y, v.z); }

			//_
			void scale_(float f)
				throw() { glScalef(f, f, f);}

			//_
			void setColorRGBA_(const ColorRGBA& color)
				throw() { glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());}
			
			///
			Index 								drawing_mode_;

			///
			Index 								drawing_precision_;

			//_
			float 								x_scale_;

			//_
			float 								y_scale_;

			GLDisplayList* 				GL_spheres_list_;
			GLDisplayList* 				GL_tubes_list_;
			GLDisplayList* 				GL_boxes_list_;

			/* static array of vertices for sphere dots */
			static const float sphere_vertices_[12][3];
			static const int 		sphere_indices_[20][3];

			// naming of geometric objects
			typedef HashMap<GeometricObject*, Name> NameHashMap;
			typedef HashMap<Name, GeometricObject*> GeometricObjectHashMap;
			typedef HashMap<const Representation*, GLDisplayList*> DisplayListHashMap;

			GeometricObjectHashMap	name_to_object_;
			NameHashMap							object_to_name_;
			DisplayListHashMap 			display_lists_;
			Name 										all_names_;
			GLuint 									object_buffer_[BALL_GLRENDERER_PICKING_NUMBER_OF_MAX_OBJECTS];
			Vector3 								normal_vector_;

			bool stereo_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_GLRENDERER_H
