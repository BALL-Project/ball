// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.40.16.4 2007/04/20 14:16:50 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_COMMON_H
#define BALL_VIEW_KERNEL_COMMON_H

#ifndef BALL_DATATYPE_STRING_H
 #include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_MATHS_VECTOR3
 #include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
 #include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
 #include <BALL/DATATYPE/regularData3D.h>
#endif

#include <QtCore/QEvent>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QLabel>
#include <QtGui/QDropEvent>

namespace BALL
{
	namespace VIEW
	{

		class GeometricObject;
		class MainControl;
		class ColorRGBA;
		class Camera;

		/** @name defines
				\ingroup ViewKernelOther
		*/
		//@{

		/**	The default port for the VIEW::Server.
				Default port is 20000.
				\see  Server
		*/
		#define VIEW_DEFAULT_PORT 20000

		/** Assign the name of a QObject to its variable name, needed e.g. for the GUI testing
		*/
		#define BALL_ASSIGN_NAME(OBJ)\
			OBJ->setObjectName(#OBJ);
	
		//@}
		/** @name General enumerations, methods and typedefs
				These predefined types are used in VIEW for portability and
				comprehensibility.
				 \ingroup ViewKernelOther
		*/
		//@{

		/** Enumeration of Events
				These events are used to communicate between different threads.
				Have a look at QWidget::customEvent(QEvent*) to learn about
				QT and its events concept. This enum is used to give every type
				of derived QEvent its individual type id.
		*/
		enum EventsIDs
		{
			/// An event with a contained VIEW message
			MESSAGE_EVENT = 60000,

			/// Event to print output from a Thread
			LOG_EVENT,

			/// Event that encodes a 6 dimensional transformation
			TRANSFORMATION_EVENT_6D,
			
			/// Event that encodes a head tracker position change
			HEADTRACKING_EVENT,

			/// Event that encodes a motion tracker position change
			MOTIONTRACKING_EVENT,

			/// Event for button press events of custom input devices
			BUTTON_PRESS_EVENT,

			/// Event for button press events of custom input devices
			BUTTON_RELEASE_EVENT,

			/// Event for passing BALL Messages accross threads
			NOTIFICATION_EVENT,

			/// Event posted by renderers after a new buffer is available
			RENDER_TO_BUFFER_FINISHED_EVENT
		};

		//@}
		/** @name Enumerations for Representations and Renderer
				Enums and methods to describe the models and drawing methods.
				 \ingroup ViewKernelOther
		*/
		//@{

		/// Enumeration for Drawing Modes
		enum DrawingMode
		{
			///
			DRAWING_MODE_DOTS = 0,

			///
			DRAWING_MODE_WIREFRAME,

			///
			DRAWING_MODE_SOLID,
			
			///
			DRAWING_MODE_TOON
		};

		/// Number of drawing modes
		#define BALL_VIEW_MAXIMAL_DRAWING_MODE 4

		/// Enumeration for Drawing Precisions.
		enum DrawingPrecision
		{
			///
			DRAWING_PRECISION_INVALID = -1,

			///
			DRAWING_PRECISION_LOW = 0,

			///
			DRAWING_PRECISION_MEDIUM,

			///
			DRAWING_PRECISION_HIGH,

			/// 
			DRAWING_PRECISION_ULTRA
		};
		
		/// Number of drawing precisions
		#define BALL_VIEW_MAXIMAL_DRAWING_PRECISION 4

		/// MAXIMAL_DRAWING_PRECISION * MAXIMAL_DRAWING_MODE
		#define BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE  BALL_VIEW_MAXIMAL_DRAWING_PRECISION * BALL_VIEW_MAXIMAL_DRAWING_MODE

		/// Mapping from the enum to the float values of Surfaces
		BALL_VIEW_EXPORT extern float SurfaceDrawingPrecisions[4];

		/** This properties define the available models.
				Add new model entries directly before MODEL_LABEL if you want them to show up 
				in the DisplayProperties dialog!
				You may have to modify the following methods, if you add a new ModelType:
				@see getModelName(ModelType type)
				@see getColoringName(ColoringMethod type)
				@see isSurfaceModel(ModelType type)
				@see modelMuteableByDisplayProperties(ModelType type)
				@see modelMustBeRebuild(ModelType type)
		*/
		enum ModelType
		{
			/// defines the property for the model: Lines
			MODEL_LINES = 0,

			/// defines the property for the model: Stick
			MODEL_STICK,

			/// defines the property for the model: Ball and Stick
			MODEL_BALL_AND_STICK,
			
			/// defines the property for the model: Van der Waals
			MODEL_VDW,

			/// defines the property for the model: solvent-excluded surface
			MODEL_SE_SURFACE,

			/// defines the property for the model: solvent-accessible surface
			MODEL_SA_SURFACE,

			/// defines the property for the model: Backbone
			MODEL_BACKBONE,

			/// defines the property for the model: Cartoon
			MODEL_CARTOON,

			/// defines the property for the model: Ribbon
			MODEL_RIBBON,

			/// defines the property for the model: H-Bonds
			MODEL_HBONDS,

			/// 
			MODEL_FORCES,

			// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			// add new model types before this comment!!!!
			// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			
			/// defines the property for the model: Label
			MODEL_LABEL = 100,

			/// defines the property for the model: contour surface
			MODEL_CONTOUR_SURFACE,

			///
			MODEL_GRID_SLICE,

			///
			MODEL_GRID_VOLUME,

			///
			MODEL_FIELD_LINES,

			///
			MODEL_INFORMATIONS,

			///
			MODEL_COORDINATE_SYSTEM,
			
			/// defines the property for the model: proximity light
			MODEL_PROXIMITY_LIGHT,

			///
			MODEL_PLANE,

			///
			MODEL_SPHERE,

			/// unkown property
			MODEL_UNKNOWN = 200
		};


		/** Enumeration for Coloring Methods.
				<b>Add new coloring methods before COLORING_CUSTOM.</b>
		*/
		enum ColoringMethod
		{
			///
			COLORING_ELEMENT          = 0,

			///
			COLORING_RESIDUE_INDEX,

			///
			COLORING_RESIDUE_NAME,

			///
			COLORING_SECONDARY_STRUCTURE,

			///
			COLORING_ATOM_CHARGE,

			///
			COLORING_DISTANCE,

			///
			COLORING_TEMPERATURE_FACTOR,

			///
			COLORING_OCCUPANCY,

			///
			COLORING_FORCES,

			///
			COLORING_RESIDUE_TYPE,

			///
			COLORING_CHAIN,
			
			///
			COLORING_MOLECULE,

			// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			// add new coloring types before this comment!!!!
			// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			
			///
			COLORING_CUSTOM,

			///
			COLORING_UNKNOWN = 200
		};

		/** A special Composite, which is ignored by all ColoringProcessors,
		 		the GeometricObjects, which have this Composite set, will be colored
				by the default color.
		*/
		BALL_VIEW_EXPORT extern Composite composite_to_be_ignored_for_colorprocessors_;

		/////////////////////////////////////////////////////////

		///
		BALL_VIEW_EXPORT String vector3ToString(const Vector3& v);

		///
		BALL_VIEW_EXPORT bool stringToVector3(const String& data, Vector3& v);

		/// create a string from a float and cut after a given number of digits after the dot
		BALL_VIEW_EXPORT String createFloatString(float value, Size precision);

		/** Get MainControl
		 		This method is only to be used for code, which is not part of 
				a ModularWidget.
				On Windows, it uses the following cast:\\
				MainControl* mc = dynamic_cast<MainControl*>(qApp->mainWidget());\\
				On all other platforms, it uses MainControl::getInstance(0);
		*/
		BALL_VIEW_EXPORT MainControl* getMainControl();

		/// Create a temporary filename in the users home dir
		BALL_VIEW_EXPORT String createTemporaryFilename();

		///
		BALL_VIEW_EXPORT Vector3 getNormal(const Vector3& v);

		/// Event class used for thread safe output to logview
		class BALL_VIEW_EXPORT LogEvent
			: public QEvent
		{
			public:

				///
				LogEvent();

				///
				void setMessage(const String& msg) {message_ = msg;}

				///
				String getMessage() {return message_;}

				/// will allways be shown in Statusbar or just when no other message shown?
				bool isImportant() { return important_;}

				///
				void setImportant(bool state) { important_ = state;}

				/// only show in logview, no change to status bar
				bool showOnlyInLogView() const { return only_log_;}

				///
				void setShowOnlyInLogView(bool state) { only_log_ = state;}

			protected:
				String message_;
				bool   important_;
				bool   only_log_;
		};

		/// thread safe output to logview
		BALL_VIEW_EXPORT void logString(const String& data);

		/// BALLView Debug macro
		#define BALLVIEW_DEBUG logString(String("A problem occured in ") + __FILE__ + " " + \
													 String(__LINE__) + ".  Please notify us per mail: ball@bioinf.uni-sb.de\n");

	
		BALL_VIEW_EXPORT void processDropEvent(QDropEvent* e);

		/** Focus the camera on a Composite
				Sends a SceneMessage.
		*/
		BALL_VIEW_EXPORT Camera focusCamera(Composite* composite);

		/** Focus the camera on a list of points, e.g. atoms or geometric objects.
				Sends a SceneMessage.
		*/
		BALL_VIEW_EXPORT Camera focusCamera(const std::list<Vector3>& points);

		///
		BALL_VIEW_EXPORT String ascii(const QString& str);
		
		///
		BALL_VIEW_EXPORT ColorRGBA getColor(const QLabel* label);
		
		///
 		BALL_VIEW_EXPORT void setColor(const QLabel* label, const ColorRGBA& color);

		///
		BALL_VIEW_EXPORT QColor chooseColor(QLabel* label);

		///
		BALL_VIEW_EXPORT void setTextColor(QLabel* label, const ColorRGBA& color);

		/** Uses the de-Casteljou algorithm to evalute a cubic Hermite interpolation
		 *  polynomial at interpolated_values.size() equidistant values.
		 */
		BALL_VIEW_EXPORT void cubicInterpolation(const Vector3& a, const Vector3& b,
																	 const Vector3& tangent_a, const Vector3& tangent_b,
																	 std::vector<Vector3>& interpolated_values);

		//////////////////////////////////////////////////////////////////////////////
		/// Code to refine a icosaeder:
		extern float 		icosaeder_vertices[12][3];
		extern Position icosaeder_indices[20][3];

		BALL_VIEW_EXPORT void subdivideTriangle(vector<Vector3>& results, Vector3& v1, Vector3& v2, Vector3& v3, Size precision);

		BALL_VIEW_EXPORT vector<Vector3> createSphere(Size precision);

		BALL_VIEW_EXPORT void calculateHistogramEqualization(const vector<float>& values, vector<float>& normalized_values, bool use_absolute_values = false);

		/// calcualte a random set of points, dependening of the field strength of a grid
		BALL_VIEW_EXPORT void calculateRandomPoints(const RegularData3D& grid, Size nr_points, vector<Vector3>& resulting_points);

		//@}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COMMON_H
