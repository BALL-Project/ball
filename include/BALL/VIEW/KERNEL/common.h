// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.14 2003/11/23 16:40:59 amoll Exp $
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

/** @name General enumerations, methods and typedefs
		These predefined types are used in VIEW for portability and
		comprehensibility.
		 \ingroup ViewKernelOther
*/
//@{

/// global variable, which defines, if DockWidgets are shown with a Label
extern bool BALL_VIEW_DOCKWINDOWS_SHOW_LABELS;

/** SceneHandle type.
		Use this type to represent <b>scene handles</b>. Scene handles are used
		for the non-ambiguous identification of scenes.
*/
typedef unsigned long    SceneHandle;      


/**	The default port for the VIEW server.
		Default port is 20000.
		\see  Server
*/
#define VIEW_DEFAULT_PORT 20000

/** Command enums.
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


/// Drawing Modes
enum DrawingMode
{
	///
	DRAWING_MODE_DOTS = 0,

	///
	DRAWING_MODE_WIREFRAME,

	///
	DRAWING_MODE_SOLID
};

// Number of drawing modes
#define BALL_VIEW_MAXIMAL_DRAWING_MODE 3


/// Drawing Precisions
enum DrawingPrecision
{
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
#define BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE  12  


/** This properties define the available models.
		Add new model entries before MODEL_LABEL if you want them to show up 
		in the DisplayProperties dialog!
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
	
	/// defines the property for the model: Label
	MODEL_LABEL,

	/// defines the property for the model: contour surface
	MODEL_CONTOUR_SURFACE
};


/** Coloring Methods
		Add new coloring methods before COLORING_CUSTOM
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
	COLORING_CUSTOM,

	///
	COLORING_UNKNOWN
};

///
String getModelName(ModelType type) 
	throw();

///
String getColoringName(ColoringMethod type) 
	throw();


/// Enumeration of Events
enum EventsIDs
{
	///
	SCENE_UPDATE_EVENT = 60000,

	///
	SIMULATION_THREAD_FINISHED_EVENT,

	///
	SIMULATION_OUTPUT_EVENT,

	///
	UPDATE_COMPOSITE_EVENT
};
	
//@}

} } //namespaces
#endif // BALL_VIEW_KERNEL_COMMON_H
