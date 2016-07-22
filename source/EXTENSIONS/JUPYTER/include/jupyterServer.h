#ifndef JUPYTERSERVER_H
#define JUPYTERSERVER_H

#include <QtCore/QProcess>

namespace BALL
{
	namespace VIEW
	{
		class JupyterServer
		{
			public:
				JupyterServer(QObject* parent, unsigned int port, bool debug, QString nbdir);
				virtual ~JupyterServer();

				void start();
				void terminate();

			protected:
				unsigned int port_;
				bool debug_;
				QString nbdir_;
				QProcess* proc_;
		};
	}
}

#endif // JUPYTERSERVER_H
