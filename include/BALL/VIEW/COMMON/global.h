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

		/**     Command enums.
						Use these enums in the client server classes.
						These enums specify the commands the server is able
						to understand.
		*/
		enum Command
		{
			///
			COMMAND__UNKOWN              = 0,
			///
			COMMAND__OPEN_SCENE          = 1,
			///
			COMMAND__CLOSE_SCENE         = 2,
			///
			COMMAND__RESIZE_SCENE        = 3,
			///
			COMMAND__SHOW_SCENE          = 4,
			///
			COMMAND__HIDE_SCENE          = 5,
			///
			COMMAND__DESTROY_SCENE       = 6,
			///
			COMMAND__SEND_OBJECT         = 7,
			///
			COMMAND__UPDATE_SCENE        = 8,
			///
			COMMAND__SET_CREATOR_VALUE   = 9,
			///
			COMMAND__GET_CREATOR_VALUE   = 10,
			///
			COMMAND__HAS_CREATOR_VALUE   = 11,
			///
			COMMAND__SET_PROCESSOR_VALUE = 12,
			///
			COMMAND__GET_PROCESSOR_VALUE = 13,
			///
			COMMAND__HAS_PROCESSOR_VALUE = 14,
			
			///
			NUMBER_OF_COMMANDS
		};	

		/**     Range enum.
		*/
		enum Range
		{
			///
			RANGE__ADDRESS = 50
		};

		/**     Value enum
		*/
		enum Value
		{
			///
			VALUE__INVALID              = -1
		};
		//@}

	} // namespace VIEW

} // namespace BALL

#endif
