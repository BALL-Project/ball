// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.4 2003/09/04 23:14:10 amoll Exp $
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

		//@}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COMMON_H
