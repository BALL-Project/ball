#include <BALL/SYSTEM/simpleDownloader.h>

#include <QtCore/QCoreApplication>
#include <QtNetwork/QSslError>
#include <QtCore/QBuffer>

namespace BALL
{
	namespace SimpleDownloaderHelper
	{
		HelperThread::HelperThread(const QUrl& url, QByteArray* result, SimpleDownloader* parent)
			: err_(0), url_(url), result_(result), parent_(parent)
		{
		}

		HelperThread::HelperThread(const QUrl& url, const String& path, SimpleDownloader* parent)
			: err_(0), url_(url), result_(0), path_(path), parent_(parent)
		{
		}

		int HelperThread::getStatus()
		{
			return err_;
		}

		void HelperThread::run()
		{
			QNetworkAccessManager* man = new QNetworkAccessManager();
			QNetworkReply* reply = getReply_(man);

			BasicHelper* dl;
			if(path_ != "") {
				dl = new DLHelper(this, reply, path_);
			} else {
				dl = new DLArrayHelper(this, reply, result_);
			}

			err_ = exec();

			if(reply->error() != QNetworkReply::NoError) {
				err_ = reply->error();
			}

			delete dl;
			delete reply;
			delete man;
		}

		DLThread::DLThread(const QUrl& url, QByteArray* result, SimpleDownloader* parent)
			: HelperThread(url, result, parent)
		{
		}

		DLThread::DLThread(const QUrl& url, const String& path, SimpleDownloader* parent)
			: HelperThread(url, path, parent)
		{
		}

		QNetworkReply* DLThread::getReply_(QNetworkAccessManager* man)
		{
			return man->get(QNetworkRequest(url_));
		}

		UPThread::UPThread(const QUrl& url, const QByteArray* data, QByteArray* result, SimpleDownloader* parent)
			: HelperThread(url, result, parent), data_(data), file_(0)
		{
		}

		UPThread::UPThread(const QUrl& url, const QByteArray* data, const String& path, SimpleDownloader* parent)
			: HelperThread(url, path, parent), data_(data), file_(0)
		{
		}

		UPThread::UPThread(const QUrl& url, QIODevice* file, QByteArray* result, SimpleDownloader* parent)
			: HelperThread(url, result, parent), data_(0), file_(file)
		{
		}

		UPThread::UPThread(const QUrl& url, QIODevice* file, const String& path, SimpleDownloader* parent)
			: HelperThread(url, path, parent), data_(0), file_(file)
		{
		}

		QNetworkReply* UPThread::getReply_(QNetworkAccessManager* man)
		{
			if(data_)
			{
				return man->post(QNetworkRequest(url_), *data_);
			}
			else
			{
				return man->post(QNetworkRequest(url_), file_);
			}
		}

		BasicHelper::BasicHelper(HelperThread* caller, QNetworkReply* reply)
			: caller_(caller), reply_(reply)
		{
			QObject::connect(reply, SIGNAL(finished()),
												this,   SLOT(finished()));
			QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
												this,   SLOT(error(QNetworkReply::NetworkError)));
#ifndef QT_NO_OPENSSL
			QObject::connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),
												this,   SLOT(sslErrors(const QList<QSslError>&)));
#endif
		}

		void BasicHelper::error(QNetworkReply::NetworkError error)
		{
			Log.error() << "Error while downloading. Error code is: " << error << "\n";
			caller_->exit(error);
		}

#ifndef QT_NO_OPENSSL
		void BasicHelper::sslErrors(const QList<QSslError>& errors)
		{
			Log.error() << "SSL error(s) while downloading. Errors are:\n";

			QSslError ssl_error;
			foreach(ssl_error, errors) {
				Log.error() << ssl_error.errorString().toAscii().data() << "\n";
			}

			caller_->exit(-1);
		}
#endif

		DLHelper::DLHelper(HelperThread* caller, QNetworkReply* reply, const String& path)
			: BasicHelper(caller, reply), file_(path.c_str())
		{
			file_.open(QIODevice::WriteOnly);
			QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
												this,   SLOT(receivedData()));
		}

		void DLHelper::finished()
		{
			file_.write(reply_->readAll());
			file_.close();
			caller_->quit();
		}

		void DLHelper::receivedData()
		{
			file_.write(reply_->readAll());
		}

		DLArrayHelper::DLArrayHelper(HelperThread* caller, QNetworkReply* reply, QByteArray* result)
			: BasicHelper(caller, reply), result_(result)
		{
		}

		void DLArrayHelper::finished()
		{
			*result_ = reply_->readAll();
			caller_->quit();
		}

		//Hack Thread
		QFtpHackThread::QFtpHackThread(const QUrl& url, QIODevice* iodev, SimpleDownloader* parent)
			: ftp_(0),
			  helper_(0),
			  url_(url),
			  iodev_(iodev),
				parent_(parent)
		{
		}

		QFtpHackThread::~QFtpHackThread()
		{
			if(ftp_) ftp_->deleteLater();
			if(helper_) helper_->deleteLater();
		}

		void QFtpHackThread::run()
		{
			ftp_ = new QFtp;
			helper_ = new QFtpHackHelper(this);
			connect(ftp_,    SIGNAL(done(bool)), helper_, SLOT(done(bool)));
			
			ftp_->connectToHost(url_.host(), url_.port(21));
			ftp_->login();
			ftp_->get(url_.path(), iodev_);
			ftp_->close();

			exec();
		}

		//Hack Helper
		QFtpHackHelper::QFtpHackHelper(QFtpHackThread* th)
			: th_(th)
		{
		}

		void QFtpHackHelper::done( bool error)
		{
			if(error)
				std::cout << "error while downloading" << std::endl;
			th_->exit(error);
		}
	}

	SimpleDownloader::SimpleDownloader(const String& url, unsigned int timeout)
		: url_(url.c_str()), timeout_(timeout)
	{
	}

	SimpleDownloader::SimpleDownloader(const QUrl& url, unsigned int timeout)
		: url_(url), timeout_(timeout)
	{
	}

	int SimpleDownloader::qftpDownloadHack_(QIODevice* iodev)
	{
		QCoreApplication* app = 0;
		if(!QCoreApplication::instance()) {
			int tmp = 0;
			app = new QCoreApplication(tmp, 0);
		}

		SimpleDownloaderHelper::QFtpHackThread th_(url_, iodev, this);
		th_.start();

		if(!th_.wait(timeout_)) {
			Log.error() << "Error while downloading file" << std::endl;
		}

		delete app;

		return 0;
	}

	int SimpleDownloader::downloadToBuffer(std::vector<char>& array)
	{
		QByteArray tmp_array;

		int result;

		//This is only necessary as long as QNetworkAccessManager is buggy
		if(url_.scheme() == "ftp")
		{
			QBuffer buf(&tmp_array);
			result = qftpDownloadHack_(&buf);
		} else {
			SimpleDownloaderHelper::DLThread th(url_, &tmp_array, this);

			result = download_(th);
		}
		array.resize(tmp_array.count());
		std::copy(tmp_array.data(), tmp_array.data() + tmp_array.count(), &array[0]);

		return result;
	}

	int SimpleDownloader::downloadToFile(const String& path)
	{
		//This is only necessary as long as QNetworkAccessManager is buggy
		if(url_.scheme() == "ftp")
		{
			QFile file(path.c_str());
			file.open(QIODevice::WriteOnly);
			return qftpDownloadHack_(&file);
		}
		else
		{
			SimpleDownloaderHelper::DLThread th(url_, path, this);
			return download_(th);
		}
	}

	int SimpleDownloader::uploadStringToBuffer(const String& data, std::vector<char>& response)
	{
		QByteArray tmp_array;
		QByteArray data_array(data.c_str());
		SimpleDownloaderHelper::UPThread th(url_, &data_array, &tmp_array, this);

		int result = download_(th);
		response.resize(tmp_array.count());
		std::copy(tmp_array.data(), tmp_array.data() + tmp_array.count(), &response[0]);

		return result;
	}

	int SimpleDownloader::uploadStringToFile(const String& data, const String& path)
	{
		QByteArray data_array(data.c_str());
		SimpleDownloaderHelper::UPThread th(url_, &data_array, path, this);
		return download_(th);
	}

	int SimpleDownloader::uploadFileToBuffer(const String& path, std::vector<char>& response)
	{
		QByteArray tmp_array;
		QFile file(path.c_str());

		if(!file.open(QIODevice::ReadOnly)) {
			return -1;
		}

		SimpleDownloaderHelper::UPThread th(url_, &file, &tmp_array, this);

		int result = download_(th);
		response.resize(tmp_array.count());
		std::copy(tmp_array.data(), tmp_array.data() + tmp_array.count(), &response[0]);

		return result;
	}

	int SimpleDownloader::uploadFileToFile(const String& in_path, const String& out_path)
	{
		QFile file(in_path.c_str());

		if(!file.open(QIODevice::ReadOnly)) {
			return -1;
		}

		SimpleDownloaderHelper::UPThread th(url_, &file, out_path, this);

		return download_(th);
	}

	int SimpleDownloader::download_(SimpleDownloaderHelper::HelperThread& th)
	{
		QCoreApplication* app = 0;
		if(!QCoreApplication::instance()) {
			int tmp = 0;
			app = new QCoreApplication(tmp, 0);
		}

		th.start();

		if (!th.wait(timeout_))
		{
			Log.error() << "SimpleDownloader::download_: Download request \"" << (String)(url_.toString()) << "\" timed out.\n";
			return -1;
		}

		delete app;

		return th.getStatus();
	}

	void SimpleDownloader::setTimeout(unsigned int timeout)
	{
		timeout_ = timeout;
	}

	void SimpleDownloader::setURL(const String& url)
	{
		url_ = QUrl(url.c_str());
	}

	void SimpleDownloader::setURL(const QUrl& url)
	{
		url_ = url;
	}

	const QUrl& SimpleDownloader::getURL() const
	{
		return url_;
	}
}
