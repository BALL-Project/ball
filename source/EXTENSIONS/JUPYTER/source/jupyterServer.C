#include <jupyterServer.h>
#include <BALL/common.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterServer::JupyterServer(QObject* parent, const QString& exe_path, unsigned int port, bool debug, const QString& nbdir)
			: exe_path_(exe_path),
			  port_(port),
			  debug_(debug),
			  nbdir_(nbdir),
			  proc_(new QProcess(parent))
		{
			connect(proc_, &QProcess::readyReadStandardOutput, this, &JupyterServer::readyReadStandardOutput);
			connect(proc_, &QProcess::readyReadStandardError,  this, &JupyterServer::readyReadStandardError);
			connect(proc_, &QProcess::stateChanged,            this, &JupyterServer::stateChanged);
			connect(proc_, &QProcess::started,                 this, &JupyterServer::started);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
			connect(proc_, &QProcess::errorOccurred,           this, &JupyterServer::errorOccurred);
#endif
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

#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
			// The QProcess::errorOccured signal is only available with Qt 5.6 or newer. We manually emit
			// the corresponding signal of our own class with older Qt versions.
			if(!proc_->waitForStarted())
				emit errorOccurred(QProcess::ProcessError::FailedToStart);
#endif
		}

		void JupyterServer::terminate(int kill_timer)
		{
			Log.info() << "Shutting down Jupyter server..." << std::endl;
			proc_->terminate(); // TODO check if this also works under Windows (c.f. QProcess documentation)

			// Kill server if it didn't manage to shut down within the specified grace period
			if(!proc_->waitForFinished(kill_timer))
			{
#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
				// This will happen automatically with Qt 5.6 or newer.
				emit errorOccurred(QProcess::ProcessError::Timedout);
#endif
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
