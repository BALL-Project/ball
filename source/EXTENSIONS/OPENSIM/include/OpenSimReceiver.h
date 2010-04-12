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
					ADD_BOND = 1,
					REMOVE_ATOM = 2,
					REMOVE_BOND = 3,
					RUN_MINIMIZATION = 4,      
					FULLUPDATE = 5,
					MOVE_ATOM = 6,						
					UPDATE_POSITIONS = 7,           
					SATURATE_FULL_WITH_HYDROGENS = 8,
					ACKNOWLEDGEMENT = 9,  
					UPDATE_ATOM = 10,			 
					UPDATE_BOND = 11,
					SELECT_ATOM = 12,
					SELECT_BOND = 13,
					DESELECT_ATOM = 14,
					DESELECT_BOND = 15,
					CHANGE_FORCE_FIELD = 16,
					SATURATE_PARTIAL_WITH_HYDROGENS = 17,
					CHECK_STATUS =18,
					MD_SIMULATION = 19,
					ADD_MOLECULE = 20,
					STOP_SIMULATION = 21,
					SINGLE_POINT_CALCULATION = 22,
					ACKNOWLEDGE_ADD_ATOM = 23,
					ACKNOWLEDGE_ADD_BOND = 24,
					ACKNOWLEDGE_REMOVE_ATOM = 25,
					ACKNOWLEDGE_REMOVE_BOND = 26,
					ACKNOWLEDGE_UPDATE_ATOM = 27,
					ACKNOWLEDGE_UPDATE_BOND = 28,
					ACKNOWLEDGE_SATURATE_F_HYD = 29,
					ACKNOWLEDGE_RUN_MINI = 30,
					ACKNOWLEDGE_CHANGE_FF =31,
					ACKNOWLEDGE_S_PNT_CAL = 32,
					ACKNOWLEDGE_MD_SIMULATION = 33,
					ACKNOWLEDGE_ADD_MOLECULE = 34,
					ACKNOWLEDGE_STOP_SIMULATION = 35

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
