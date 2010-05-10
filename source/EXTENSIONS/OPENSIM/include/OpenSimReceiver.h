#ifndef BALL_EXTENSIONS_OPENSIM_OPENSIMRECEIVER_H
#define BALL_EXTENSIONS_OPENSIM_OPENSIMRECEIVER_H

#include <BALL/SYSTEM/networking.h>
#include <BALL/SYSTEM/mutex.h>

#include <OpenSimWorkerThread.h>

#include <queue>

namespace BALL
{
	namespace VIEW
	{
		class OpenSimPlugin;

		class OpenSimReceiver : public TCPServerThread
		{
			Q_OBJECT

			public :

				bool is_acknowledged_ ;

				bool is_Process_Done_ ;

				ReadWriteLock rwLock_;

				std::queue<std::vector<String> > incomingmessage_queue_;

				enum {
					ADD_ATOM = 0,
					ADD_BOND,
					REMOVE_ATOM,
					REMOVE_BOND,
					RUN_MINIMIZATION,      
					FULLUPDATE,
					MOVE_ATOM,						
					UPDATE_POSITIONS,           
					SATURATE_FULL_WITH_HYDROGENS,
					ACKNOWLEDGEMENT,  
					UPDATE_ATOM,			 
					UPDATE_BOND,
					SELECT_ATOM,
					SELECT_BOND,
					DESELECT_ATOM,
					DESELECT_BOND,
					CHANGE_FORCE_FIELD,
					SATURATE_PARTIAL_WITH_HYDROGENS,
					CHECK_STATUS,
					MD_SIMULATION,
					ADD_MOLECULE,
					STOP_SIMULATION,
					SINGLE_POINT_CALCULATION,
					ACKNOWLEDGE_ADD_ATOM,
					ACKNOWLEDGE_ADD_BOND,
					ACKNOWLEDGE_REMOVE_ATOM,
					ACKNOWLEDGE_REMOVE_BOND,
					ACKNOWLEDGE_UPDATE_ATOM,
					ACKNOWLEDGE_UPDATE_BOND,
					ACKNOWLEDGE_SATURATE_F_HYD,
					ACKNOWLEDGE_RUN_MINI,
					ACKNOWLEDGE_CHANGE_FF,
					ACKNOWLEDGE_S_PNT_CAL,
					ACKNOWLEDGE_MD_SIMULATION,
					ACKNOWLEDGE_ADD_MOLECULE,
					ACKNOWLEDGE_STOP_SIMULATION,
					ADD_ATOM_CONTAINER
				};

				OpenSimReceiver(Size port, OpenSimPlugin* plugin);
				~OpenSimReceiver();

				virtual void run();
				virtual void handleAsyncConnection();
				virtual void deactivate();

				void checkClientStatus();

				void sendMessageString(const String& to_send);

				bool hasMessage();

			protected:

				OpenSimPlugin* plugin_;

				OpenSimWorkerThread* funcThread_;

		};
	}
}

#endif
