#ifndef BALL_PYTHON_PYSERVER_H
#define BALL_PYTHON_PYSERVER_H

#include <QtNetwork/QTcpServer>

namespace BALL
{
	class PyServer : public QObject
	{
		Q_OBJECT

		public:
			/**
			 * Starts a new Python server and listens for requests.
			 */
			PyServer();

			~PyServer();

		public slots:
			/**
			 * Processes a single request and sends a suitable response.
			 */
			void processRequest();

		private:
			/**
			 * Processes a single `execute_request`, that is, runs the received Python code through PyInterpreter
			 * and sends the result to the requesting client.
			 *
			 * @param client
			 * @param request
			 */
			void processExecuteRequest(QTcpSocket* client, const QString& request);

			/**
			 * Utility function to create a JSON-encoded message to a client.
			 *
			 * @param msg_type `execute_result` or `error`
			 * @param content actual message
			 * @return JSON-encoded message
			 */
			QByteArray createMessage(QString msg_type, QString content);

			/**
			 * Disconnects a client connection (after all messages to the same client have been sent).
			 *
			 * @param client
			 */
			void disconnectClient(QTcpSocket* client);

			QTcpServer* server_;
	};
}

#endif // BALL_PYTHON_PYSERVER_H