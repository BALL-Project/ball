#include <jupyterServer.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterServer::JupyterServer(QObject* parent, unsigned int port, bool debug, QString nbdir)
			: port_(port),
			  debug_(debug),
			  nbdir_(nbdir),
			  proc_(new QProcess(parent))
		{ }

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
			proc_->start("jupyter", args);
		}

		void JupyterServer::terminate()
		{
			proc_->terminate();
			proc_->waitForFinished(); // TODO timeout as member
		}
	}
}
