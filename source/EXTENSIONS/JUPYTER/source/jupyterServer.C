#include <jupyterServer.h>
#include <BALL/common.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterServer::JupyterServer(QObject* parent, unsigned int port, bool debug, QString nbdir)
			: port_(port),
			  debug_(debug),
			  nbdir_(nbdir),
			  proc_(new QProcess(parent))
		{
			connect(proc_, SIGNAL(readyReadStandardOutput()), this, SIGNAL(readyReadStandardOutput()));
			connect(proc_, SIGNAL(readyReadStandardError()), this, SIGNAL(readyReadStandardError()));
			connect(proc_, SIGNAL(stateChanged(QProcess::ProcessState)), this, SIGNAL(stateChanged(QProcess::ProcessState)));
			connect(proc_, SIGNAL(started()), this, SIGNAL(started()));
		}

		JupyterServer::~JupyterServer()
		{ }

		void JupyterServer::start()
		{
			QStringList args;
			args << "notebook"
				 << "--no-browser"								// do not open the browser after starting
				 << QString("--port=%1").arg(port_)				// server port
				 << QString("--notebook-dir=%1").arg(nbdir_);	// notebook and kernel directory
			if(debug_)
			{
				args << "--debug";								// enable debug messages
			}
			proc_->start("jupyter", args); // TODO exe as member
		}

		void JupyterServer::terminate()
		{
			proc_->terminate();
			proc_->waitForFinished(); // TODO timeout as member
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
