#include "../include/BALLViewPluginConfiguration.h"
#include <QtCore/QObject>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/INPUT/inputPlugin.h>
#include <BALL/SYSTEM/networking.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/SYSTEM/mutex.h>

#include <queue>
#include <QtCore/qthread.h>

#include "../include/MolecularStructureContainer.h"
#include "../include/MolecularModelingContainer.h"
#include "../include/MolecularDynamicsContainer.h"




namespace BALL
{
	namespace VIEW
	{
		class BALLViewOpenSimPlugin
			: public QObject,
			  public ModularWidget,
			  public BALLPlugin,
			  public VIEWPlugin,
			  public InputPlugin
			  
		{
			Q_OBJECT
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::InputPlugin)

			BALL_EMBEDDABLE(BALLViewOpenSimPlugin, ModularWidget)


			public:

				class BVWorkerThread: public QThread
				{

					
					public:
						void run();
	
						BVWorkerThread(BALLViewOpenSimPlugin* plugin);

						~BVWorkerThread();

					protected:
						BALLViewOpenSimPlugin* bvplugin_;
						


				};


				class BVCommandExecutionThread: public QThread
				{

					
					public:
						void run();
	
						BVCommandExecutionThread(BALLViewOpenSimPlugin* plugin);

						~BVCommandExecutionThread();

					protected:
						BALLViewOpenSimPlugin* bvcmdplugin_;
						

				};


				class BVOSServer: public TCPServerThread
				{
					public :
						
						bool is_acknowledged_ ;

						bool is_Process_Done_ ;

						ReadWriteLock rwLock_;

						queue<std::vector<String>> incomingmessage_queue_;

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

						BVOSServer(Size port, BALLViewOpenSimPlugin* plugin);

					
						~BVOSServer();

						virtual void handleAsyncConnection();

			
						void checkClientStatus();

						
						void sendMessageString(const String& to_send);

					protected:
						
						BALLViewOpenSimPlugin* plugin_;
						
						BVWorkerThread* funcThread_;

						

				};


				

				enum BallviewMessageEnum
				{
					NEW_COMPOSITE,
					CHANGED_COMPOSITE_HIERARCH,
					REMOVED_COMPOSITE,
					REPRESENTATION
				};

				struct BondStruct
				{

					const Bond* bond;
					Handle atom_one_handle;
					Handle atom_two_handle;

					BondStruct(const Bond* bond_,Handle atom_one_handle_,Handle atom_two_handle_)
					{
						bond = bond_;
						atom_one_handle_ = atom_one_handle;
						atom_two_handle_ = atom_two_handle;

					}

					BondStruct()
					{
					}

					

				};
				typedef BondStruct bondStructType;

				struct BallviewMessage
				{
					

					BallviewMessageEnum bwmessageEnum;
					AtomContainer* container;
					
					HashMap<Handle, const Atom*> handle_to_atom_;
					
					HashMap<Handle, BondStruct> handle_to_bond_;
					
				
					BallviewMessage(BallviewMessageEnum bwmessageEnum_,AtomContainer* container_,
									HashMap<Handle, const Atom*> handle_to_atom,
									HashMap<const Atom*, Handle> atom_to_handle,
									HashMap<Handle, bondStructType> handle_to_bond)
					{
						bwmessageEnum = bwmessageEnum_;
						container = container_;
						handle_to_atom_ = handle_to_atom;
						handle_to_bond_ = handle_to_bond;
					


					}

					BallviewMessage(){}

				};

		
				typedef BallviewMessage ballviewMessageType;

				queue<ballviewMessageType> ballviewmessage_queue_;

			
				BALLViewOpenSimPlugin();
				const char* configFilePath_;

				BVCommandExecutionThread* const cmdThread_;

				ReadWriteLock pluginrwLock_;


				virtual QString getName() const { return "BALLViewOpenSimPlugin"; }
				virtual QString getDescription() const { return "An interesting plugin"; }
				virtual bool isActive() { return is_active_; };
				virtual bool activate();
				virtual bool deactivate();

				virtual QPixmap* getIcon() const { return NULL; }
				virtual QDialog* getConfigDialog() { return NULL; }

				virtual void setReceiver(QWidget*) { }

				virtual InputDeviceDriver* startDriver() { return NULL; }

				virtual void onNotify(Message* message);

				void handleMolecularModeling(std::vector<String> message);

				void sendAcknowledgement(const String& message);


				void handleNewComposite(BallviewMessage message);

				void handleChangedComposite(BallviewMessage message);
				
				void handleRemovedComposite(BallviewMessage message);
				
				void handleRepresentation(BallviewMessage message);

				
				
			   

			protected:
				
				bool is_active_;

				BVOSServer*  server_;

				ElementColorProcessor ecp_;

				MolecularStructureContainer* const molStructPlugin_;

				MolecularModelingContainer* const molModelingPlugin_;

				MolecularDynamicsContainer* const molDynamicsPlugin_;

			
				CommunicationConfiguration*  config_;


				

				
				

				
		};
	}

}
