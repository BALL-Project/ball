#ifndef BALL_VIEW_COMMON_GLOBAL_H
#define BALL_VIEW_COMMON_GLOBAL_H

#ifndef BALL_CONFIG_CONFIG_H
# include <BALL/CONFIG/config.h>
#endif
 
namespace BALL
{
	namespace VIEW
	{
		/**     @name Type aliases defined by VIEW.
						These predefined types are used in VIEW for portability and
						comprehensibility.
						{\bf Definition:} \URL{BALL/VIEW/COMMON/global.h}
		*/
		//@{
		
		/**     SceneHandle type.
						Use this type to represent {\bf scene handles}. Scene handles are used
						for the non-ambiguous identification of scenes.
		*/
		typedef unsigned long    SceneHandle;      
		//@}

		/**     @name Enums defined by VIEW.
		*/
		//@{

		/**     Command enums
						Use this enums in the client server classes.
						These enums specify the commands the server is able
						to understand.
		*/
		enum Command
		{
			COMMAND__UNKOWN         = 0,
			COMMAND__OPEN_SCENE     = 1,
			COMMAND__CLOSE_SCENE    = 2,
			COMMAND__RESIZE_SCENE   = 3,
			COMMAND__SHOW_SCENE     = 4,
			COMMAND__HIDE_SCENE     = 5,
			COMMAND__DESTROY_SCENE  = 6,
			COMMAND__SEND_OBJECT    = 7,
			COMMAND__UPDATE_SCENE   = 8,
			
			NUMBER_OF_COMMANDS
		};	
		//@}

	} // namespace VIEW
} // namespace BALL

#endif
