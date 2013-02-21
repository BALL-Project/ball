#include <BALLaxyInterface.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QWebElement>
#include <QWebFrame>
#include <QHttpMultiPart>
#include <QSslError>

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

			// Here, we can register python scripts with which we could interact with BALLView from the HTML side
			Path p;
			script_base_ = p.find("BALLaxyInterface/scripts") + "/";

			context_separator_ = MolecularControl::getInstance(0)->getContextMenu().addSeparator();
			context_action_    = MolecularControl::getInstance(0)->getContextMenu().addAction(tr("Send to ballaxy"), this, SLOT(sendToBallaxy()));
		}

		BALLaxyInterface::~BALLaxyInterface()
		{
			for(QHash<QString, BALLaxyInterfaceAction*>::iterator it = action_registry_.begin(); it != action_registry_.end(); ++it)
			{
				delete it.value();
			}

			MolecularControl::getInstance(0)->getContextMenu().removeAction(context_separator_);
			MolecularControl::getInstance(0)->getContextMenu().removeAction(context_action_);
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

		bool BALLaxyInterface::uploadToBallaxy(AtomContainer* ac)
		{
			String tmp_pdb_filename;
			File::createTemporaryFilename(tmp_pdb_filename);
			PDBFile tmp_pdb_file(tmp_pdb_filename, std::ios::out);

			// currently, we can only handle Molecules and Systems
			if (dynamic_cast<System*>(ac) != 0)
			{
				tmp_pdb_file << *dynamic_cast<System*>(ac);
				tmp_pdb_file.close();
			}
			else if (dynamic_cast<Molecule*>(ac) != 0)
			{
				tmp_pdb_file << *dynamic_cast<Molecule*>(ac);
				tmp_pdb_file.close();
			}
			else
			{
				tmp_pdb_file.close();
				File::remove(tmp_pdb_filename);

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

			std::ifstream input_stream(tmp_pdb_filename);
			std::string content((std::istreambuf_iterator<char>(input_stream)), std::istreambuf_iterator<char>());

			text_field.setPlainText(content.c_str());

			run_button.evaluateJavaScript("this.click()");

			File::remove(tmp_pdb_filename);

			return true;
		}

		void BALLaxyInterface::handleLinkClicked(const QUrl& url)
		{
			if(!getMainControl()->isBusy())
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
			Log.warn() << "Warning: got an ssl error: " << std::endl;
			foreach (QSslError e, errors)
			{
				Log.warn() << ascii(e.errorString()) << std::endl;
			}

			reply->ignoreSslErrors();
		}

		void BALLaxyInterface::sendToBallaxy()
		{
			AtomContainer* ac = dynamic_cast<AtomContainer*>(MolecularControl::getInstance(0)->getContextComposite());

			if (ac)
			{
				uploadToBallaxy(ac);
			}
		}

	}
}
