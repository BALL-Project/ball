#include <jupyterServer.h>
#include <BALL/common.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterServer::JupyterServer(QObject* parent, const QString& exe_path, unsigned int port, bool autostart, bool debug, const QString& nbdir)
			: exe_path_(exe_path),
			  port_(port),
			  debug_(debug),
			  nbdir_(nbdir),
			  proc_(new QProcess(parent))
		{
			connect(proc_, SIGNAL(readyReadStandardOutput()), this, SIGNAL(readyReadStandardOutput()));
			connect(proc_, SIGNAL(readyReadStandardError()), this, SIGNAL(readyReadStandardError()));
			connect(proc_, SIGNAL(stateChanged(QProcess::ProcessState)), this, SIGNAL(stateChanged(QProcess::ProcessState)));
			connect(proc_, SIGNAL(started()), this, SIGNAL(started()));

			if(autostart)
			{
				start();
			}
		}

		JupyterServer::~JupyterServer()
		{
			if(isRunning())
			{
				terminate();
			}
		}

		const QString& JupyterServer::getExePath()
		{
			return exe_path_;
		}

		void JupyterServer::setExePath(const QString &exe_path)
		{
			exe_path_ = exe_path;
		}

		unsigned int JupyterServer::getPort()
		{
			return port_;
		}

		void JupyterServer::setPort(unsigned int port)
		{
			port_ = port;
		}

		bool JupyterServer::getDebug()
		{
			return debug_;
		}

		void JupyterServer::setDebug(bool debug)
		{
			debug_ = debug;
		}

		const QString& JupyterServer::getNbdir()
		{
			return nbdir_;
		}

		void JupyterServer::setNbdir(const QString &nbdir)
		{
			nbdir_ = nbdir;
		}

		bool JupyterServer::isRunning()
		{
			return proc_->state() != QProcess::ProcessState::NotRunning;
		}

		void JupyterServer::start()
		{
			if(isRunning())
			{
				Log.warn() << "The Jupyter server is already running." << std::endl;
				return;
			}
			QStringList args;
			args << "notebook"
				 << "--no-browser"								// do not open the browser after starting
				 << QString("--port=%1").arg(port_)				// server port
				 << QString("--notebook-dir=%1").arg(nbdir_)	// notebook and kernel directory
				 << QString("--port-retries=0");				// prevent port changes (dashboard wouldn't notice!)
			if(debug_)
			{
				args << "--debug";								// enable debug messages
			}
			Log.info() << "Starting Jupyter server..." << std::endl;
			proc_->start(exe_path_, args);
			// TODO check fail state
		}

		void JupyterServer::terminate(int kill_timer)
		{
			Log.info() << "Shutting down Jupyter server..." << std::endl;
			proc_->terminate(); // TODO check if this also works under Windows (c.f. QProcess documentation)
			proc_->waitForFinished(kill_timer);

			// Kill server if it didn't manage to shut down within the specified grade period
			if(isRunning())
			{
				Log.warn() << "The Jupyter server didn't shut down within " << kill_timer/1000
						   << "seconds. Killing..." << std::endl;
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

		QProcess::ProcessState JupyterServer::state()
		{
			return proc_->state();
		}
	}
}
