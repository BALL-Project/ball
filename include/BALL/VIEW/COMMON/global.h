#ifndef BALL_VIEW_COMMON_GLOBAL_H
#define BALL_VIEW_COMMON_GLOBAL_H

#ifndef BALL_CONFIG_CONFIG_H
# include <BALL/CONFIG/config.h>
#endif
 
namespace BALL
{
	namespace VIEW
	{
		/**     @name Type aliases defined by VIEW
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

		/**			@name Constant definition
		*/	
		//@{

		/**	The default port for the VIEW server.
				Default port is 20000.
				@see  Server
		*/
		#define VIEW_DEFAULT_PORT 20000

		//@}

		/**     @name Enums defined by VIEW
		*/
		//@{

		/**     Command enums.
						These enums are used in the client and server classes.
						These enums specify the commands the server is able
						to understand (at the moment).
						@see  Server
						@see  Client
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
		//@}

	} // namespace VIEW

} // namespace BALL

#endif
