// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.17.2.2 2004/05/18 16:18:43 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_COMMON_H
#define BALL_VIEW_KERNEL_COMMON_H

#ifndef BALL_DATATYPE_STRING_H
 #include <BALL/DATATYPE/string.h>
#endif

namespace BALL
{
	namespace VIEW
	{


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
	/// see SceneUpdateEvent
	SCENE_UPDATE_EVENT = 60000,

	/// see Mainframe::SimulationThreadFinished
	SIMULATION_THREAD_FINISHED_EVENT,

	/// see SimulationOutput
	SIMULATION_OUTPUT_EVENT,

	/// see UpdateCompositeEvent
	UPDATE_COMPOSITE_EVENT
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
	/// unkown property
	MODEL_UNKNOWN = -1,
	
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

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// add new model types before this comment!!!!
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	/// defines the property for the model: Label
	MODEL_LABEL,

	/// defines the property for the model: contour surface
	MODEL_CONTOUR_SURFACE
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

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// add new coloring types before this comment!!!!
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	///
	COLORING_CUSTOM,

	///
	COLORING_UNKNOWN
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

//@}

} } //namespaces
#endif // BALL_VIEW_KERNEL_COMMON_H
