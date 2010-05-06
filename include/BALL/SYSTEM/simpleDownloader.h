#ifndef BALL_SYSTEM_SIMPLEDOWNADER_H
#define BALL_SYSTEM_SIMPLEDOWNLOADER_H

#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#include <QtCore/QThread>
#include <QtCore/QFile>
#include <QtNetwork/QNetworkReply>

class QByteArray;

namespace BALL
{
	namespace SimpleDownloaderHelper
	{
		class DLThread;
	}

	/**
	 * This class provides an interface for synchronous downloads.
	 * It currently relies on QNetworkAccessManager and thus on the existence of
	 * a QCoreApplication object. If no global instance is existing a local application
	 * object is created. For this reason you must not use this class in any thread but
	 * the main thread if you did not create a QCoreApplication object yourself.
	 */
	class BALL_EXPORT SimpleDownloader
	{
		public:
			/*
			 * Default Constructor.
			 *
			 * @param url The URL to download.
			 * @param timeout The maximum number of milliseconds the download is allowed to take.
			 *                default: infinite
			 */
			SimpleDownloader(const String& url, unsigned int timeout = UINT_MAX);

			/**
			 * If the content of the download should be kept in memory you can use this function
			 * that stores the received bytes in the provides vector. Any previous content is overwritten.
			 *
			 * @param buffer The buffer vector to use.
			 * @return Non-zero if an error occurred while downloading
			 */
			int downloadToBuffer(std::vector<char>& buffer);

			/**
			 * Download the url and store the contents to a file.
			 *
			 * @param path The path to the file the content is stored in.
			 * @return Non-zero if an error occurred while downloading
			 */
			int downloadToFile(const String& path);

			/**
			 * Sets the maximum amount of time a download my take. The time is specified in
			 * milliseconds.
			 *
			 * @param timeout The time in milliseconds.
			 */
			void setTimeout(unsigned int timeout);

			/**
			 * Sets the url of the download.
			 *
			 * @param url The URL to download
			 */
			void setURL(const String& url);

			/**
			 * Returns the current URL.
			 *
			 * @return The currently set URL
			 */
			const String& getURL() const;

		private:
			int download_(SimpleDownloaderHelper::DLThread& thread);

			String url_;
			unsigned int timeout_;
	};

	namespace SimpleDownloaderHelper
	{
		class DLThread : public QThread
		{
			public:
				DLThread(const String& url, QByteArray* result);
				DLThread(const String& url, const String& path);

				int getStatus();

			protected:
				void run();

				int err_;
				String url_;
				QByteArray* result_;
				String path_;
		};

		class BasicHelper : public QObject
		{
			Q_OBJECT

			public:
				BasicHelper(DLThread* caller, QNetworkReply* reply);
				virtual ~BasicHelper(){}

			public slots:
				void error(QNetworkReply::NetworkError error);
				void sslErrors(const QList<QSslError>& errors);
				virtual void finished() = 0;

			protected:
				DLThread* caller_;
				QNetworkReply* reply_;
		};

		class DLArrayHelper : public BasicHelper
		{
			Q_OBJECT

			public:
				DLArrayHelper(DLThread* caller, QNetworkReply* reply, QByteArray* result);

			public slots:
				void finished();

			private:
				QByteArray* result_;
		};

		class DLHelper : public BasicHelper
		{
			Q_OBJECT

			public:
				DLHelper(DLThread* caller, QNetworkReply* reply, const String& path);

			public slots:
				void finished();
				void receivedData();

			private:
				QFile file_;
		};
	}
}

#endif //BALL_SYSTEM_SIMPLEDOWNLOADER_H
