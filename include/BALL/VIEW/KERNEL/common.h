// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.3 2003/09/03 13:03:51 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_COMMON_H
#define BALL_VIEW_KERNEL_COMMON_H

#ifndef BALL_DATATYPE_STRING_H
 #include <BALL/DATATYPE/string.h>
#endif

/*
#define BALL_VIEW_PRINT_PROPERTY(property) \
		(hasProperty(property) ? "yes" : "no")
*/
namespace BALL
{
	namespace VIEW
	{
		/** @name Type aliases defined by VIEW
				These predefined types are used in VIEW for portability and
				comprehensibility.
		*/
		//@{
		
		/**     SceneHandle type.
						Use this type to represent <b>scene handles</b>. Scene handles are used
						for the non-ambiguous identification of scenes.
		*/
		typedef unsigned long    SceneHandle;      

		/**			@name Constant definition
		 \ingroup ViewCommon
		*/	
		//@{

		/**	The default port for the VIEW server.
				Default port is 20000.
				\see  Server
		*/
		#define VIEW_DEFAULT_PORT 20000

		//@}

		/**     @name Enums defined by VIEW
		 \ingroup ViewCommon
		*/
		//@{

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

		//@}
    
		/** Predefined properties.
				Enumeration of all properties that are used by the VIEW kernel.
				This properties define the models available in VIEW.
		*/
		enum ModelTypes
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

			/// defines the property for the model: Surface
			MODEL_SURFACE,

			/// defines the property for the model: Backbone
			MODEL_BACKBONE,

			/// defines the property for the model: Cartoon
			MODEL_CARTOON,

			/// defines the property for the model: H-Bonds
			MODEL_HBONDS,
			
			/// defines the property for the model: Label
			MODEL_LABEL

		};


		/// Coloring Methods
		enum ColoringMethod
		{
			///
			COLORING_ELEMENT          = 0,

			///
			COLORING_RESIDUE_TYPE,

			///
			COLORING_RESIDUE_NAME,

			///
			COLORING_ATOM_CHARGE,

			///
			COLORING_DISTANCE,

			///
			COLORING_CUSTOM
		};

		String getModelName(ModelTypes type) 
			throw();

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COMMON_H
