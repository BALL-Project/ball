// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: global.h,v 1.15 2003/08/26 08:04:56 oliver Exp $
//

#ifndef BALL_VIEW_COMMON_GLOBAL_H
#define BALL_VIEW_COMMON_GLOBAL_H

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
    
	} // namespace VIEW

} // namespace BALL

#endif
