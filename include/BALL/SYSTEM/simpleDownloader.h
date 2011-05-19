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
		class HelperThread;
	}

	/**
	 * This class provides an interface for synchronous downloads.
	 * It currently relies on QNetworkAccessManager and thus on the existence of
	 * a QCoreApplication object. If no global instance is existing a local application
	 * object is created. For this reason you must not use this class in any thread but
	 * the main thread if you did not create a QCoreApplication object yourself.
	 *
	 * REMARK: this class is considered experimental and its interface
	 * can be a subject to redesign.
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
			 * Upload the passed string to the specified URL and save the response
			 * to the specified buffer
			 *
			 * @param data the string which is uploaded
			 * @param response the response of the server
			 * @return Non-zero if an error occured while uploading
			 */
			int uploadStringToBuffer(const String& data, std::vector<char>& response);

			/**
			 * Upload the passed string to the specified URL and save the response
			 * to the specified file
			 *
			 * @param data the string which is uploaded
			 * @param response the file the response is saved to
			 * @return Non-zero if an error occured while uploading
			 */
			int uploadStringToFile(const String& data, const String& response);

			/**
			 * Upload the passed string to the specified URL and save the response
			 * to the specified file
			 *
			 * @param path the path of the file to upload
			 * @param response the response of the server
			 * @return Non-zero if an error occured while uploading
			 */
			int uploadFileToBuffer(const String& path, std::vector<char>& response);

			/**
			 * Upload the passed string to the specified URL and save the response
			 * to the specified file
			 *
			 * @param path the path of the file to upload
			 * @param response the file the response is saved to
			 * @return Non-zero if an error occured while uploading
			 */
			int uploadFileToFile(const String& path, const String& response);

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
			int download_(SimpleDownloaderHelper::HelperThread& thread);

			String url_;
			unsigned int timeout_;
	};

	namespace SimpleDownloaderHelper
	{
		class HelperThread : public QThread
		{
			public:
				HelperThread(const String& url, QByteArray* result);
				HelperThread(const String& url, const String& path);

				int getStatus();

			protected:
				virtual QNetworkReply* getReply_(QNetworkAccessManager* man) = 0;

				void run();

				int err_;
				String url_;
				QByteArray* result_;
				String path_;

		};

		class DLThread : public HelperThread
		{
			public:
				DLThread(const String& url, QByteArray* result);
				DLThread(const String& url, const String& path);

			protected:
				virtual QNetworkReply* getReply_(QNetworkAccessManager* man);
		};

		class UPThread : public HelperThread
		{
			public:
				UPThread(const String& url, const QByteArray* data, QByteArray* result);
				UPThread(const String& url, const QByteArray* data, const String& path);
				UPThread(const String& url, QIODevice* file, QByteArray* result);
				UPThread(const String& url, QIODevice* file, const String& path);

			protected:
				virtual QNetworkReply* getReply_(QNetworkAccessManager* man);

				const QByteArray* data_;
				QIODevice* file_;
		};

		class BasicHelper : public QObject
		{
			Q_OBJECT

			public:
				BasicHelper(HelperThread* caller, QNetworkReply* reply);
				virtual ~BasicHelper(){}

			public slots:
				void error(QNetworkReply::NetworkError error);
#ifndef QT_NO_OPENSSL
				void sslErrors(const QList<QSslError>& errors);
#endif
				virtual void finished() = 0;

			protected:
				HelperThread* caller_;
				QNetworkReply* reply_;
		};

		class DLArrayHelper : public BasicHelper
		{
			Q_OBJECT

			public:
				DLArrayHelper(HelperThread* caller, QNetworkReply* reply, QByteArray* result);

			public slots:
				void finished();

			private:
				QByteArray* result_;
		};

		class DLHelper : public BasicHelper
		{
			Q_OBJECT

			public:
				DLHelper(HelperThread* caller, QNetworkReply* reply, const String& path);

			public slots:
				void finished();
				void receivedData();

			private:
				QFile file_;
		};
	}
}

#endif //BALL_SYSTEM_SIMPLEDOWNLOADER_H
