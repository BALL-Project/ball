#include <jupyterServer.h>

#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterServer::JupyterServer(
				QObject* parent,
				const QString& exe_path,
				unsigned int port,
				bool debug,
				const QString& nbdir,
				const QString& token
			)
			: exe_path_(exe_path),
			  port_(port),
			  debug_(debug),
			  nbdir_(nbdir),
			  token_(token),
			  proc_(new QProcess(parent))
		{
			connect(proc_, &QProcess::readyReadStandardOutput, this, &JupyterServer::readyReadStandardOutput);
			connect(proc_, &QProcess::readyReadStandardError,  this, &JupyterServer::readyReadStandardError);
			connect(proc_, &QProcess::stateChanged,            this, &JupyterServer::stateChanged);
			connect(proc_, &QProcess::started,                 this, &JupyterServer::started);
			connect(proc_, &QProcess::errorOccurred,           this, &JupyterServer::errorOccurred);
		}

		JupyterServer::~JupyterServer()
		{
			if(isRunning())
			{
				terminate();
			}
		}

		bool JupyterServer::isRunning() const
		{
			return proc_->state() != QProcess::ProcessState::NotRunning;
		}

		void JupyterServer::start()
		{
			if(isRunning())
			{
				Log.warn() << "[JupyterPlugin] The Jupyter server is already running." << std::endl;
				return;
			}
			QStringList args;
			args << "notebook"
				 << "--no-browser"                            // do not open the browser after starting
				 << QString("--port=%1").arg(port_)           // server port
				 << QString("--notebook-dir=%1").arg(nbdir_)  // notebook and kernel directory
				 << QString("--port-retries=0")               // prevent port changes (dashboard wouldn't notice!)
				 << QString("--NotebookApp.token=%1").arg(token_); // auth token (disabled if string empty)
			if(debug_)
			{
				args << "--debug";								// enable debug messages
			}
			Log.info() << "[JupyterPlugin] Starting Jupyter server..." << std::endl;
			proc_->start(exe_path_, args);
		}

		void JupyterServer::terminate(int kill_timer)
		{
			Log.info() << "[JupyterPlugin] Shutting down Jupyter server..." << std::endl;
			proc_->terminate(); // TODO check if this also works under Windows (c.f. QProcess documentation)

			// Kill server if it didn't manage to shut down within the specified grace period
			if(!proc_->waitForFinished(kill_timer))
			{
				proc_->kill();
			}
		}

		QByteArray JupyterServer::readStandardOutput()
		{
			return proc_->readAllStandardOutput();
		}

		QByteArray JupyterServer::readStandardError()
		{
			return proc_->readAllStandardError();
		}

		QProcess::ProcessState JupyterServer::state() const
		{
			return proc_->state();
		}
	}
}
