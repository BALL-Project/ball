#include <iPythonWidget.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{

		IPythonWidget::IPythonWidget(MainControl* parent, const char* title)
			: DockWidget(parent, title)
		{
			registerThis();

			tab_view_ = new QTabWidget(this);
			BALL_ASSIGN_NAME(tab_view_);
			setGuest(*tab_view_);

			IPythonHTMLView* main_view = new IPythonHTMLView(tab_view_, this);
			
			main_view->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
			main_view->settings()->setAttribute(QWebSettings::JavascriptCanCloseWindows, true);
			main_view->settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);

			tab_view_->addTab(main_view, "iPython");
//			connect(main_view, SIGNAL(linkClicked(const QUrl&)), this, SLOT(handleLinkClicked(const QUrl&)));
		}

		IPythonWidget::~IPythonWidget()
		{
		}

		IPythonWidget::IPythonHTMLView::IPythonHTMLView(QWidget* parent, IPythonWidget* base)
			: HTMLView(parent),
				base_(base)
		{
		}

		QWebView* IPythonWidget::IPythonHTMLView::createWindow(QWebPage::WebWindowType type)
		{
			return base_->createWindow(type);
		}

		QWebView* IPythonWidget::createWindow(QWebPage::WebWindowType type)
		{
			IPythonHTMLView *result = new IPythonHTMLView(tab_view_, this);
			
			result->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
			result->settings()->setAttribute(QWebSettings::JavascriptCanCloseWindows, true);
			result->settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);

			result->load(ipython_url);

			tab_view_->addTab(result, "iPython");

			return result;
		}

		void IPythonWidget::setIPythonURL(String const& url)
		{
			QWriteLocker lock(&page_lock_);
			ipython_url.setUrl(url.c_str());
			reinterpret_cast<IPythonHTMLView*>(tab_view_->currentWidget())->load(ipython_url.toString());
		}

		void IPythonWidget::handleLinkClicked(const QUrl& url)
		{
		}

		void IPythonWidget::contextMenuEvent(QContextMenuEvent*)
		{
			// here, we could, e.g., download structures to BALLView from a context menu in the web page
		}

		// TODO: ask the user what to do (for the moment, we just ignore any ssl errors)
		void IPythonWidget::handleSslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
		{
			// TODO: reenable display of ssl errors
			/*
			Log.warn() << "Warning: got an ssl error: " << std::endl;
			foreach (QSslError e, errors)
			{
				Log.warn() << ascii(e.errorString()) << std::endl;
			}
			*/
			reply->ignoreSslErrors();
		}

		void IPythonWidget::loadFinished(bool ok)
		{
		}

		/*
		void IPythonWidget::handleDownload(QNetworkReply* reply)
		{
			// if qt wants to signal that the user wants to download something,
			// it generates an unsupportedContent-signal with the raw header
			// "Content-Disposition" set
			if (reply->hasRawHeader("Content-Disposition"))
			{
				// we need the filename to extract the type
				String content_disposition = ascii(reply->rawHeader("Content-Disposition"));

				// the string should have the format "attachment; filename"
				if (content_disposition.hasPrefix("attachment;"))
				{
					String filename = content_disposition.after("attachment;");
					std::vector<String> split;

					// remove potential quotes introduced by the web server
					filename.substituteAll("\"", "");
					filename.substituteAll("\'", "");

					filename.split(split, ".");

					String extension = split.back();
					if (MolFileFactory::isFileExtensionSupported(extension))
					{
						String tmp_filename = VIEW::createTemporaryFilename() + "." + extension;

						// write the data to a file
						QFile outfile(tmp_filename.c_str());
						outfile.open(QIODevice::WriteOnly);
						outfile.write(reply->readAll());
						outfile.close();

						// and read it back
						System* system = new System();

						GenericMolFile* molfile = MolFileFactory::open(tmp_filename, std::ios::in);
						*molfile >> *system;
						molfile->close();

						File::remove(tmp_filename);

						getMainControl()->setStatusbarText(String("read ")
								      + String(system->countAtoms()) + " atoms from BALLaxy", true);

						if (system->getName() == "")
						{
							system->setName(filename);
						}

						getMainControl()->insert(*system, system->getName());

						CompositeMessage* cm = new CompositeMessage(*system, CompositeMessage::CENTER_CAMERA);
						qApp->postEvent(parent(), new MessageEvent(cm));
					}
					else
					{
						Log.error() << "BALLaxy plugin: cannot download file of unsupported extension " 
												<< extension << std::endl;
					}
				}
			}
		}
		*/

	}
}
