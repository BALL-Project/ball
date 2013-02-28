#include <BALLaxyInterface.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/FORMAT/molFileFactory.h>

#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QWebElement>
#include <QWebFrame>
#include <QHttpMultiPart>
#include <QSslError>

#include <QtCore/QDebug>

namespace BALL
{
	namespace VIEW
	{

		void BALLaxyInterfaceAction::execute(const QList<QPair<QString, QString> >& parameters)
		{
			executeImpl_(parameters);

			emit finishedExecution();
		}

		BALLaxyInterface::BALLaxyInterface(MainControl* parent)
			: HTMLView(parent),
				Embeddable()
		{
			registerThis();

// NOTE: we cannot currently use link delegation, since qt does not provide information about the target frame of
//       the link
//			page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
//			connect(this, SIGNAL(linkClicked(const QUrl&)), this, SLOT(handleLinkClicked(const QUrl&)));
//			connect(this, SIGNAL( urlChanged(const QUrl&)), this, SLOT(executeLink(const QUrl&)));

			connect(page()->networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> & )),
							this, SLOT(handleSslErrors(QNetworkReply*, const QList<QSslError> & ))); 


			connect(page(), SIGNAL(unsupportedContent(QNetworkReply*)),
					    this, SLOT(handleDownload(QNetworkReply*)));

			page()->setForwardUnsupportedContent(true);

			// Here, we can register python scripts with which we could interact with BALLView from the HTML side
			Path p;
			script_base_ = p.find("BALLaxyInterface/scripts") + "/";

			context_submenu_ = new QMenu(tr("Send to ballaxy"), this);
			context_separator_      = MolecularControl::getInstance(0)->getContextMenu().addSeparator();
			context_submenu_action_ = MolecularControl::getInstance(0)->getContextMenu().addMenu(context_submenu_);

			context_action_pdb_  = context_submenu_->addAction(tr("as pdb"), this, SLOT(sendPDBToBallaxy()));
			context_action_mol2_ = context_submenu_->addAction(tr("as mol2"), this, SLOT(sendMOL2ToBallaxy()));
		}

		BALLaxyInterface::~BALLaxyInterface()
		{
			for(QHash<QString, BALLaxyInterfaceAction*>::iterator it = action_registry_.begin(); it != action_registry_.end(); ++it)
			{
				delete it.value();
			}

			MolecularControl::getInstance(0)->getContextMenu().removeAction(context_separator_);
			context_submenu_->removeAction(context_action_pdb_);
			context_submenu_->removeAction(context_action_mol2_);
			MolecularControl::getInstance(0)->getContextMenu().removeAction(context_submenu_action_);
		}

		void BALLaxyInterface::setBALLaxyBaseUrl(String const& ballaxy_base)
		{
			ballaxy_base_.setUrl(ballaxy_base.c_str());
			setUrl(ballaxy_base_.toString());
		}

		void BALLaxyInterface::registerAction(BALLaxyInterfaceAction* action)
		{
			if(!action)
			{
				return;
			}

			QHash<QString, BALLaxyInterfaceAction*>::iterator it = action_registry_.find(action->getName());

			if(it != action_registry_.end())
			{
				delete *it;
			}

			action_registry_.insert(action->getName(), action);
		}

		bool BALLaxyInterface::uploadToBallaxy(AtomContainer* ac, const String& format)
		{
			String tmp_filename;
			File::createTemporaryFilename(tmp_filename, format);

			GenericMolFile* mol_file = MolFileFactory::open(tmp_filename, std::ios::out);

			// currently, we can only handle Molecules and Systems
			if (dynamic_cast<System*>(ac) != 0)
			{
				*mol_file << *dynamic_cast<System*>(ac);
				mol_file->close();
			}
			else if (dynamic_cast<Molecule*>(ac) != 0)
			{
				*mol_file << *dynamic_cast<Molecule*>(ac);
				mol_file->close();
			}
			else
			{
				mol_file->close();
				File::remove(tmp_filename);

				return false;
			}

			// find main frame
			QWebFrame* page_main_frame   = page()->mainFrame();
			QWebFrame* galaxy_main_frame = page_main_frame;

			if (galaxy_main_frame->frameName() != "galaxy_main") // try its children
			{
				foreach(QWebFrame* child_frame, page_main_frame->childFrames())
				{
					if (child_frame->frameName() == "galaxy_main")
					{
						galaxy_main_frame = child_frame;
					}
				}
			}

			galaxy_main_frame->load(QUrl(ballaxy_base_.toString()+"/tool_runner?tool_id=upload1"));
			QEventLoop loop;
			QObject::connect(this, SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
			loop.exec();

			// now, try to find the submit button in the page
			QWebElement run_button = galaxy_main_frame->findFirstElement("input[name=runtool_btn]");

			// also, find the input area
			QWebElement text_field = galaxy_main_frame->findFirstElement("textarea");

			std::ifstream input_stream(tmp_filename);
			std::string content((std::istreambuf_iterator<char>(input_stream)), std::istreambuf_iterator<char>());

			text_field.setPlainText(content.c_str());

			run_button.evaluateJavaScript("this.click()");

			File::remove(tmp_filename);

			return true;
		}

		void BALLaxyInterface::handleLinkClicked(const QUrl& url)
		{
			if (!getMainControl()->isBusy())
			{
				load(url);
			}
		}

		void BALLaxyInterface::contextMenuEvent(QContextMenuEvent*)
		{
			// here, we could, e.g., download structures to BALLView from a context menu in the web page
		}

		void BALLaxyInterface::executeLink(const QUrl& url)
		{
			QString action_name = url.queryItemValue("action");
			if(action_name == QString::null)
			{
				return;
			}

			QString method_type = url.queryItemValue("method");
			QString parameters  = url.queryItemValue("parameters");

			//Ideally this if should be converted into another registry
			if(method_type == "native")
			{
				QHash<QString, BALLaxyInterfaceAction*>::iterator it = action_registry_.find(action_name);

				if(it != action_registry_.end())
				{
					(*it)->execute(url.queryItems());
				}
			}
			else if(method_type == "" || method_type == "python")
			{
					executePython_(action_name, url.queryItems());
			}
		}

		void BALLaxyInterface::executePython_(const QString& action, const ParameterList& parameters)
		{
			//Ensure, that the module search path is registered
			static bool added_module_path = false;

			if(!added_module_path)
			{
				PyInterpreter::run(String("sys.path.append(\"") + script_base_ + "\")", added_module_path);

				if(!added_module_path)
				{
					Log.error() << "Could not add module path" << std::endl;
					return;
				}
			}

			//Search the module to load
			QString load_module = "__main__";
			for(ParameterList::const_iterator it = parameters.begin(); it != parameters.end(); ++it)
			{
				if(it->first == "module")
				{
					load_module = it->second;
					break;
				}
			}

			try
			{
				PyInterpreter::execute(load_module, action, parameters);
			}
			catch(Exception::FileNotFound)
			{
				Log.error() << "Could not execute action " << action.toAscii().data() << "\n No such file or directory." << std::endl;
			}
		}

		// TODO: ask the user what to do (for the moment, we just ignore any ssl errors)
		void BALLaxyInterface::handleSslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
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

		void BALLaxyInterface::sendPDBToBallaxy()
		{
			AtomContainer* ac = dynamic_cast<AtomContainer*>(MolecularControl::getInstance(0)->getContextComposite());

			if (ac)
			{
				uploadToBallaxy(ac, ".pdb");
			}
		}

		void BALLaxyInterface::sendMOL2ToBallaxy()
		{
			AtomContainer* ac = dynamic_cast<AtomContainer*>(MolecularControl::getInstance(0)->getContextComposite());

			if (ac)
			{
				uploadToBallaxy(ac, ".mol2");
			}
		}

		void BALLaxyInterface::handleDownload(QNetworkReply* reply)
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

					filename.split(split, ".");

					String extension = split.back();

					if (MolFileFactory::isFileExtensionSupported(extension))
					{
						String tmp_filename;
						File::createTemporaryFilename(tmp_filename, extension);

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
				}
			}
		}

	}
}
