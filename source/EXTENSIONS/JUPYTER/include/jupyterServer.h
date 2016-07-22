#ifndef JUPYTERSERVER_H
#define JUPYTERSERVER_H

#include <QtCore/QProcess>

namespace BALL
{
	namespace VIEW
	{
		class JupyterServer: public QObject
		{
			Q_OBJECT

			public:
				JupyterServer(QObject* parent, unsigned int port, bool debug, QString nbdir);
				virtual ~JupyterServer();

				void start();
				void terminate();
				QByteArray readStandardOutput();
				QByteArray readStandardError();
				QProcess::ProcessState state();

			signals:
				void readyReadStandardOutput();
				void readyReadStandardError();
				void stateChanged(QProcess::ProcessState);
				void started();

			protected:
				unsigned int port_;
				bool debug_;
				QString nbdir_;
				QProcess* proc_;
		};
	}
}

#endif // JUPYTERSERVER_H
