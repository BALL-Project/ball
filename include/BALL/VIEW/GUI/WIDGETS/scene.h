// $Id: scene.h,v 1.3 2000/09/23 14:15:07 hekl Exp $

#ifndef BALL_VIEW_GUI_WIDGETS_SCENE_H
#define BALL_VIEW_GUI_WIDGETS_SCENE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <qgl.h>
#include <iostream>

#ifndef BALL_CONCEPT_NOTIFICATION_H
#	include <BALL/CONCEPT/notification.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif


#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_MATHS_QUATERNION_H
#	include <BALL/MATHS/quaternion.h>
#endif

#ifndef BALL_VIEW_FUNCTOR_GEOMETRICCOLLECTOR_H
#	include <BALL/VIEW/FUNCTOR/geometricCollector.h>
#endif

#ifndef BALL_VIEW_GUI_FUNCTOR_GLOBJECTCOLLECTOR_H
#	include <BALL/VIEW/GUI/FUNCTOR/glObjectCollector.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_MAINCONTROL_H
#	include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_EVENTS_H
#	include <BALL/VIEW/GUI/WIDGETS/events.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
#	include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H
#	include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h>
#endif

using namespace BALL;
using namespace BALL::VIEW;

namespace BALL
{

	namespace VIEW
	{

		/**
		*/
		class Scene
			: public QGLWidget,
				public ModularWidget
		{
			friend class Events;

			Q_OBJECT

			public:
			
			BALL_EMBEDDABLE(Scene)
			
			/**	@name	Enums
			*/
			//@{

			enum Phase
			{
				PHASE__STATIC_SCENE   = 0,
				PHASE__DYNAMIC_SCENE  = 1
			};

			enum RenderMode
			{
				RENDER_MODE__COMPILE        = 0,
				RENDER_MODE__DO_NOT_COMPILE = 1
			};

			enum Event
			{
				EVENT__UNKOWN       = 0,
				EVENT__PRESSED      = 1,
				EVENT__RELEASED     = 2,
				EVENT__MOVED        = 3
			};

			enum KeyPressed
			{
				KEY_PRESSED__NONE          = 0,
				KEY_PRESSED__SHIFT         = 1,
				KEY_PRESSED__CONTROL       = 2,
				KEY_PRESSED__SHIFT_CONTROL = 3
			};

			enum MouseButton
			{
				MOUSE_BUTTON__NONE  = 0,
				MOUSE_BUTTON__ONE   = 1,
				MOUSE_BUTTON__TWO   = 2,
				MOUSE_BUTTON__THREE = 3
			};
			//@}

			/**	@name	Type Definitions
			*/
			//@{
			typedef unsigned long Width;
			typedef unsigned long Height;
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			/**	Default constructor
			*/
			Scene();

			Scene
				(QWidget* parent_widget,
				 const char* name = NULL,
				 WFlags w_flags = 0);

			Scene
				(const Scene& scene,
				 bool deep = true,
				 QWidget* parent_widget = NULL,
				 const char* name = NULL,
				 WFlags wflags = 0);

			virtual ~Scene();

			virtual void clear();
			//@}
		
			/**	@name Exceptions
			*/
			//@{
 			class MainControlMissing
				: public Exception::GeneralException
			{
  			public:
	   			MainControlMissing(const char* file, int line, const string& data);
			};
			//@}

			// --- CONVERTERS

			// --- ASSIGNMENT METHODS

			void set
				(const Scene& scene,
				 bool deep = true);

			Scene& operator =
				(const Scene& scene);

			void get
				(Scene& scene,
				 bool deep = true) const;

			void swap
				(Scene& scene);

			// --- ACCESSORS: INSPECTORS and MUTATORS

			void setRenderMode(RenderMode mode);

			RenderMode getRenderMode() const;

			/**	@name	Camera access functions
			*/
			//@{
			bool setCamera
				(const Vector3& look_at,
				 const Vector3& view_point,
				 bool set_origin = true);
			
			void setViewPointPosition(const Vector3& v);

			const Vector3& getViewPointPosition() const;

			void setViewPointPosition
				(const Real x, const Real y, const Real z);		
			
			void getViewPointPosition(Vector3& v) const;
			
			void getViewPointPosition
				(Real &x, Real &y, Real &z) const;
			
			void setLookAtPosition
				(const Vector3& v, bool set_origin = true);
			
			const Vector3& getLookAtPosition() const;

			void setLookAtPosition
				(const Real x, const Real y, const Real z, 
				 bool set_origin = true);
			
			void getLookAtPosition(Vector3& v) const;

			void getLookAtPosition
				(Real& x, Real& y, Real& z) const;
			
			const Vector3& getLastLookAtPosition() const;
			
			void getLastLookAtPosition(Vector3& v) const;
			
			void getLastLookAtPosition
				(Real& x, Real& y, Real& z) const;
			
			Real getDistance() const;
			
			void getDistance(Real& distance) const;

			const Vector3& getZoomVector() const;

			const Vector3& getUpVector() const;

			const Vector3& getRightVector() const;

			const Vector3& getLookUpVector() const;
			//@}			


			Events events;
			

			void registerGLObjectCollector
				(const GLObjectCollector& globject_collector);

			void unregisterGLObjectCollector();

			const GLObjectCollector *getGLObjectCollector() const;

			bool update(bool rebuild_displaylists = false);
			
			/**	ModularWidget methods.
			*/
			//@{
			/**
			*/
			virtual void initializeWidget(MainControl& main_control);
		
			/**
			*/
			virtual void finalizeWidget(MainControl& main_control);
		
			/**
			*/
			virtual void checkMenu(MainControl& main_control);
			//@}

			virtual void onNotify(Message *message);
			// --- PREDICATES

			bool hasDefaultGLObjectCollector() const;

			bool hasOtherGLObjectCollector() const;

			// --- DEBUGGERS and DIAGNOSTICS

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;

			// --- STORERS

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;

			// --- EXTERNAL ITERATORS

/*
			signals:

			// must be updated => new event handler

			void clickedButton
				(Scene::MouseButton,
				 Scene::Event);

			void clickedMouseButtonOne
				(Scene::Event);

			void clickedMouseButtonTwo
				(Scene::Event);

			void clickedMouseButtonThree
				(Scene::Event);

			void releasedButton
				(Scene::MouseButton,
				 Scene::Event);

			void releasedMouseButtonOne
				(Scene::Event);

			void releasedMouseButtonTwo
				(Scene::Event);

			void releasedMouseButtonThree
				(Scene::Event);
*/

			protected:

			virtual void paintGL();

			virtual void resizeGL
				(int width, int height);

			virtual void keyPressEvent
				(QKeyEvent* qkey_event);

			virtual void keyReleaseEvent
				(QKeyEvent* qkey_event);

			virtual void mouseMoveEvent
				(QMouseEvent* qmouse_event);

			virtual void mousePressEvent
				(QMouseEvent* qmouse_event);

			virtual void mouseReleaseEvent
				(QMouseEvent* qmouse_event);


			private:

			void renderView_(bool woth_names = false);

			// --- registerable functions

			void rotateSystem_(Scene* scene);

			void translateSystem_(Scene* scene);

			void zoomSystem_(Scene* scene);

			void selectionPressed_(Scene* scene);
			void selectionReleased_(Scene* scene);
			void selectionPressedMoved_(Scene* scene);

			void deselectionPressed_(Scene* scene);
			void deselectionReleased_(Scene* scene);
			void deselectionPressedMoved_(Scene* scene);


			void initializeOpenGL_();

			Vector3 translateObjectXY_(const Real distance);

			Vector3 translateObjectZ_(const Real distance);

			Vector3 calculateRotatedVector_
				(const Vector3& v, const Quaternion& q);

			void calculateQuaternion_
				(Quaternion& quaternion, const Quaternion& rotation_quaternion);

			Real sphereproject_(Real radius, Real x, Real y);

			void convertMatrix_(const Matrix4x4& m, GLfloat glfloat_array[4][4]);

			void selectObjects_(bool select = true);

			GLObjectCollector *GL_object_collector_;
			GLPrimitiveManager GL_primitive_manager_;

			Vector3 system_origin_;
			Quaternion quaternion_;

			Scene::Width	width_;
			Scene::Height height_;

			Scene::Phase				phase_;
			Scene::RenderMode		render_mode_;
			Scene::KeyPressed		key_pressed_;
			Scene::MouseButton	actual_mouse_button_;

			bool was_picking_;

			bool need_update_;

			Real x_window_pos_old_;
			Real y_window_pos_old_;

			Real x_window_pos_new_;
			Real y_window_pos_new_;

			Real x_window_pick_pos_first_;
			Real y_window_pick_pos_first_;

			Real x_window_pick_pos_second_;
			Real y_window_pick_pos_second_;

			GLfloat x_scale_;
			GLfloat y_scale_;

			bool open_gl_initialized_;


			// Camera section -----

			void initViewVectors_();

			void setCamera_
				(bool set_origin = false);

			bool setCameraPosition_
				(const Vector3& look_at,
				 const Vector3& view_point,
				 bool set_origin);

			Vector3 zoom_;
			Vector3 position_;
			Vector3 look_at_position_;
			Vector3 last_look_at_position_;
			Vector3 look_up_;
			Vector3 right_;
			Vector3 up_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/scene.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_SCENE_H_
