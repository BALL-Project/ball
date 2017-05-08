#include <jupyterServer.h>

#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterServer::JupyterServer(QObject* parent)
			: exe_path_(),
			  port_(0ul),
			  debug_(false),
			  nbdir_(),
			  token_(),
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
			if(isRunning()) terminate();
			if(proc_) delete proc_;
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
			     << "--no-browser"                                 // do not open the browser after starting
			     << QString("--port=%1").arg(port_)                // server port
			     << QString("--notebook-dir=%1").arg(nbdir_)       // notebook and kernel directory
			     << QString("--port-retries=0")                    // prevent port changes (dashboard wouldn't notice!)
			     << QString("--NotebookApp.token=%1").arg(token_); // auth token (disabled if string empty)
			if (debug_) args << "--debug";                         // enable debug messages
			Log.info() << "[JupyterPlugin] Starting Jupyter server..." << std::endl;
			proc_->start(exe_path_, args);
		}

		void JupyterServer::terminate(int kill_timer)
		{
			Log.info() << "[JupyterPlugin] Shutting down Jupyter server..." << std::endl;
#ifdef BALL_OS_WINDOWS
			// Since the Jupyter Notebook server does not respond to the WM_CLOSE message that is sent by
			// QProcess::terminate, and does not respond well to the QProcess::kill (only closing the wrapper
			// process rather than the Tornado server), we have to kill the process tree manually...
			QProcess pkill;
			QStringList args;
			args << "/f"   // force kill; required to kill Notebook server
			     << "/t"   // kill tree; also required to stop all server processes
			     << "/pid" << QString::number(proc_->processId());
			pkill.start("taskkill", args);

			// ... and if even this approach fails, we're virtually out of options. Resign and let JupyterServerTab
			// inform the user that they should terminate the process(es) manually.
			if (!pkill.waitForFinished(kill_timer))
			{
				pkill.kill();
			}
#else
			proc_->terminate();

			// Kill server if it didn't manage to shut down within the specified grace period
			if (!proc_->waitForFinished(kill_timer))
			{
				proc_->kill();
			}
#endif
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
