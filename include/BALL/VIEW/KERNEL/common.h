// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.34 2005/02/13 16:42:42 amoll Exp $
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

#include <qevent.h>

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

		/// Number of drawing modes
		#define BALL_VIEW_MAXIMAL_DRAWING_MODE 3

		/// Number of drawing precisions
		#define BALL_VIEW_MAXIMAL_DRAWING_PRECISION 4

		/// MAXIMAL_DRAWING_PRECISION * MAXIMAL_DRAWING_MODE
		#define BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE  12  

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
		extern bool BALL_VIEW_DOCKWINDOWS_SHOW_LABELS;

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


		/// Enumeration for Drawing Precisions.
		enum DrawingPrecision
		{
			///
			DRAWING_PRECISION_LOW = 0,

			///
			DRAWING_PRECISION_MEDIUM,

			///
			DRAWING_PRECISION_HIGH,

			/// UNUSED AT THE MOMENT !
			DRAWING_PRECISION_ULTRA
		};


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

			/// Clipping Planes, e.g. in OpenGL
			MODEL_CLIPPING_PLANE,

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
		extern Composite composite_to_be_ignored_for_colorprocessors_;


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
		String getModelName(ModelType type) 
			throw();

		/** Get a name for a ColoringMethod
		*/
		String getColoringName(ColoringMethod type) 
			throw();

		/** Define, which Models are Surfaces.
				Add new kinds of Surfaces to this function!
		*/
		bool isSurfaceModel(ModelType type)
			throw();

		/** Model can be modified with DisplayProperitesDialog
		*/
		bool modelMuteableByDisplayProperties(ModelType type)
			throw();

		/** Model must be rebuild, if Composite changes
		*/
		bool modelMustBeRebuild(ModelType type)
			throw();


		/////////////////////////////////////////////////////////

		///
		String getTypeName(GeometricObjectType type);

		///
		GeometricObjectType getGeometricObjectType(const GeometricObject& object);

		
		/////////////////////////////////////////////////////////

		///
		String vector3ToString(const Vector3& v)
			throw();

		///
		bool stringToVector3(const String& data, Vector3& v)
			throw();

		/// create a string from a float and cut after a given number of digits after the dot
		String createFloatString(float value, Size precision)
			throw();

		/** Get MainControl
		 		This method is only to be used for code, which is not part of 
				a ModularWidget.
				On Windows, it uses the following cast:\\
				MainControl* mc = dynamic_cast<MainControl*>(qApp->mainWidget());\\
				On all other platforms, it uses MainControl::getInstance(0);
		*/
		MainControl* getMainControl()
			throw();

		/// Create a temporary filename in the users home dir
		String createTemporaryFilename()
			throw();

		///
		Vector3 getNormal(const Vector3& v)
			throw();

		/// Event class used for thread safe output to logview
		class BALL_EXPORT LogEvent
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
		void logString(const String& data);
		
		//@}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COMMON_H
