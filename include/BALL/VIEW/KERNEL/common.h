// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.36.2.14 2005/10/17 14:51:42 amoll Exp $
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

#ifndef BALL_DATATYPE_LIST_H
 #include <BALL/DATATYPE/list.h>
#endif


#include <qevent.h>
#include <qcolordialog.h>
#include <qlabel.h>

namespace BALL
{
	namespace VIEW
	{

		class GeometricObject;
		class MainControl;

		/** @name defines
				\ingroup ViewKernelOther
		*/
		//@{

		/**	The default port for the VIEW::Server.
				Default port is 20000.
				\see  Server
		*/
		#define VIEW_DEFAULT_PORT 20000

		//@}
		/** @name General enumerations, methods and typedefs
				These predefined types are used in VIEW for portability and
				comprehensibility.
				 \ingroup ViewKernelOther
		*/
		//@{

		/** Command enums for the VIEW::Server.
				These enums specify the commands the server is able
				to understand (at the moment).
				\see  Server
				\see  Client
		*/
		enum Command
		{
			/// unknown command.
			COMMAND__UNKOWN              = 0,

			/// sent command. This command will be used for indicating a new object that is about to be received
			COMMAND__SEND_OBJECT         = 1,

			/// next free command.
			NUMBER_OF_COMMANDS
		};	

		/** Enumeration of Events
				These events are used to communicate between different threads.
				Have a look at QWidget::customEvent(QCustomEvent) to learn about
				QT and its events concept. This enum is used to give every type
				of derived QCustomEvent its individual type id.
		*/
		enum EventsIDs
		{
			///
			SCENE_EXPORTPNG_EVENT = 60000,

			///
			SCENE_EXPORTPOV_EVENT,

			///
			SCENE_SETCAMERA_EVENT,

			/// see Mainframe::SimulationThreadFinished
			SIMULATION_THREAD_FINISHED_EVENT,

			/// see SimulationOutput
			LOG_EVENT,

			/// see UpdateCompositeEvent
			UPDATE_COMPOSITE_EVENT,

			/// see FinishedRepresentionUpdateEvent
			FINISHED_REPRESENTATION_UPDATE_EVENT
		};

		/// global variable, which defines, if DockWidgets are shown with a Label
		BALL_VIEW_EXPORT extern bool BALL_VIEW_DOCKWINDOWS_SHOW_LABELS;

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
			DRAWING_MODE_SOLID
		};

		/// Number of drawing modes
		#define BALL_VIEW_MAXIMAL_DRAWING_MODE 3

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

			/// defines the property for the model: H-Bonds
			MODEL_HBONDS,

			/// 
			MODEL_FORCES,

			// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			// add new model types before this comment!!!!
			// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			
			/// defines the property for the model: Label
			MODEL_LABEL,

			/// defines the property for the model: contour surface
			MODEL_CONTOUR_SURFACE,

			/// unkown property
			MODEL_UNKNOWN
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
			COLORING_UNKNOWN
		};

		/** A special Composite, which is ignored by all ColoringProcessors,
		 		the GeometricObjects, which have this Composite set, will be colored
				by the default color.
		*/
		BALL_VIEW_EXPORT extern Composite composite_to_be_ignored_for_colorprocessors_;


		/// Enumeration of GeometricObject Types
		enum GeometricObjectType
		{
			/// Unknown type 
			TYPE__UNKNOWN              = -1,

			/// Line type 
			TYPE__LINE                 = 0,

			/// Sphere type 
			TYPE__SPHERE               = 1,

			/// Tube type 
			TYPE__TUBE                 = 2,

			/// Point type
			TYPE__POINT                = 3,

			/// Mesh type
			TYPE__MESH                 = 4,

			/// Box type
			TYPE__BOX           			 = 5
		};

		/** Get a name for a ModelType
		*/
		BALL_VIEW_EXPORT String getModelName(ModelType type) 
			throw();

		/** Get a name for a ColoringMethod
		*/
		BALL_VIEW_EXPORT String getColoringName(ColoringMethod type) 
			throw();

		/** Define, which Models are Surfaces.
				Add new kinds of Surfaces to this function!
		*/
		BALL_VIEW_EXPORT bool isSurfaceModel(ModelType type)
			throw();

		/** Model can be modified with DisplayProperitesDialog
		*/
		BALL_VIEW_EXPORT bool modelMuteableByDisplayProperties(ModelType type)
			throw();

		/** Model must be rebuild, if Composite changes
		 		but hierarchy is unchanged.
		*/
		BALL_VIEW_EXPORT bool modelMustBeRebuild(ModelType type)
			throw();

		/////////////////////////////////////////////////////////

		///
		BALL_VIEW_EXPORT String getTypeName(GeometricObjectType type);

		///
		BALL_VIEW_EXPORT GeometricObjectType getGeometricObjectType(const GeometricObject& object);

		
		/////////////////////////////////////////////////////////

		///
		BALL_VIEW_EXPORT String vector3ToString(const Vector3& v)
			throw();

		///
		BALL_VIEW_EXPORT bool stringToVector3(const String& data, Vector3& v)
			throw();

		/// create a string from a float and cut after a given number of digits after the dot
		BALL_VIEW_EXPORT String createFloatString(float value, Size precision)
			throw();

		/** Get MainControl
		 		This method is only to be used for code, which is not part of 
				a ModularWidget.
				On Windows, it uses the following cast:\\
				MainControl* mc = dynamic_cast<MainControl*>(qApp->mainWidget());\\
				On all other platforms, it uses MainControl::getInstance(0);
		*/
		BALL_VIEW_EXPORT MainControl* getMainControl()
			throw();

		/// Create a temporary filename in the users home dir
		BALL_VIEW_EXPORT String createTemporaryFilename()
			throw();

		///
		BALL_VIEW_EXPORT Vector3 getNormal(const Vector3& v)
			throw();

		/// Event class used for thread safe output to logview
		class BALL_VIEW_EXPORT LogEvent
			: public QCustomEvent
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
													 String(__LINE__) + ".  Please notify us per mail: ball@bioinf.uni-sb.de");

	
		/** Choose a color.
		 		The colordialog is initialised with the background color of the label.
				If the colordialog returns a new color, this becomes the new backbground color of the label.
		*/
		BALL_VIEW_EXPORT QColor chooseColor(QLabel* label);

		BALL_VIEW_EXPORT void processDropEvent(QDropEvent* e);

		/** focus the camera on a list of points, e.g. atoms or geometric objects.
				Sends a SceneMessage.
		*/
		BALL_VIEW_EXPORT void focusCamera(const List<Vector3>& points);

		/** Get the first defined data path, this should be the content of the environment variable BALLVIEW_DATA_PATH.
		 		We also sort out double slashes and make sure a slash is at the end.
		*/
		BALL_VIEW_EXPORT String getDataPath();
		

		//@}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COMMON_H
