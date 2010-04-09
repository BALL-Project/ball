#ifndef BALL_EXTENSIONS_OPENSIM_OPENSIMPLUGIN_H
#define BALL_EXTENSIONS_OPENSIM_OPENSIMPLUGIN_H

#include <OpenSimPluginConfiguration.h>
#include <OpenSimReceiver.h>
#include <OpenSimTask.h>
#include <OpenSimWorkerThread.h>
#include <OpenSimCommandExecutionThread.h>

#include <QtCore/QObject>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/INPUT/inputPlugin.h>
#include <BALL/SYSTEM/networking.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/SYSTEM/mutex.h>

#include <queue>
#include <QtCore/qthread.h>

#include <MolecularStructureContainer.h>
#include <MolecularModelingContainer.h>
#include <MolecularDynamicsContainer.h>


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

				friend class OpenSimWorkerThread;
				friend class OpenSimCommandExecutionThread;

				queue<OpenSimTask> ballviewmessage_queue_;
			
				BALLViewOpenSimPlugin();
				~BALLViewOpenSimPlugin();

				OpenSimCommandExecutionThread* const cmdThread_;

				ReadWriteLock pluginrwLock_;


				virtual QString getName() const { return "BALLViewOpenSimPlugin"; }
				virtual QString getDescription() const { return "An interesting plugin"; }
				virtual bool isActive() { return is_active_; };
				virtual bool activate();
				virtual bool deactivate();

				virtual const QPixmap* getIcon() const;
				virtual QDialog* getConfigDialog(); 

				virtual void setReceiver(QWidget*);

				virtual InputDeviceDriver* startDriver() { return NULL; }

				virtual void onNotify(Message* message);

				void handleMolecularModeling(std::vector<String> message);

				void sendAcknowledgement(const String& message);


				void handleNewComposite(OpenSimTask message);

				void handleChangedComposite(OpenSimTask message);
				
				void handleRemovedComposite(OpenSimTask message);
				
				void handleRepresentation(OpenSimTask message);

				bool hasMessage();

				bool checkConfig();

				void demandConfig();

				const String& getRemoteHost() const { return remote_host_; }
				const String& getLocalHost()  const { return local_host_;  }

				Size getRemotePort() const { return remote_port_; }
				Size getLocalPort()  const { return local_port_;  }
				
			public slots:
				virtual void settingsChanged();

			protected:
				
				OpenSimPluginConfiguration* settings_;

				bool is_active_;

				OpenSimReceiver*  server_;

				ElementColorProcessor ecp_;

				QPixmap icon_;
				QWidget* receiver_;		

				MolecularStructureContainer* const molStructPlugin_;

				MolecularModelingContainer* const molModelingPlugin_;

				MolecularDynamicsContainer* const molDynamicsPlugin_;

			
				String remote_host_;
				Size   remote_port_;
				String local_host_;
				Size   local_port_;
		};
	}

}

#endif
