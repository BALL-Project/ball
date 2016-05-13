#include <BALLaxyInterface.h>

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/FORMAT/molFileFactory.h>

#include <QWebEngineProfile>

namespace BALL
{
	namespace VIEW
	{
		BALLaxyInterface::BALLaxyInterface(MainControl* parent)
			: HTMLView(parent),
			  Embeddable()
		{
			registerThis();
			connect(page()->profile(), SIGNAL(downloadRequested(QWebEngineDownloadItem*)), this, SLOT(verifyDownloadRequest(QWebEngineDownloadItem*)));

			context_submenu_ = new QMenu(tr("Send to ballaxy"), this);
			context_separator_      = MolecularControl::getInstance(0)->getContextMenu().addSeparator();
			context_submenu_action_ = MolecularControl::getInstance(0)->getContextMenu().addMenu(context_submenu_);

			context_action_pdb_  = context_submenu_->addAction(tr("as pdb"), this, SLOT(sendPDBToBallaxy()));
			context_action_mol2_ = context_submenu_->addAction(tr("as mol2"), this, SLOT(sendMOL2ToBallaxy()));
		}

		BALLaxyInterface::~BALLaxyInterface()
		{
			MolecularControl::getInstance(0)->getContextMenu().removeAction(context_separator_);
			context_submenu_->removeAction(context_action_pdb_);
			context_submenu_->removeAction(context_action_mol2_);
			MolecularControl::getInstance(0)->getContextMenu().removeAction(context_submenu_action_);
		}

		void BALLaxyInterface::setBALLaxyBaseUrl(const String& ballaxy_base)
		{
			ballaxy_base_.setUrl(ballaxy_base.c_str());
			page()->load(ballaxy_base_);
		}

		bool BALLaxyInterface::uploadToBallaxy(AtomContainer* ac, const String& format)
		{
			String tmp_filename = VIEW::createTemporaryFilename() + format;
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

			std::ifstream input_stream(tmp_filename.c_str());
			std::string content((std::istreambuf_iterator<char>(input_stream)), std::istreambuf_iterator<char>());
			std::stringstream js;
			js  << "function setTextareaVal(e) {"
				<< "	e.target.removeEventListener(e.type, arguments.callee);"
				<< "	e.target.contentDocument.querySelector('form#tool_form textarea').innerHTML = \"" << QString(content.c_str()).replace("\n", "\\n").toStdString() << "\";"
				<< "	e.target.contentDocument.querySelector('form#tool_form input[name=runtool_btn]').click();"
				<< "}"
				<< "var iframe = document.all['galaxy_main'];"
				<< "iframe.addEventListener('load', setTextareaVal);"
				<< "iframe.src = '/tool_runner?tool_id=upload1';";
			page()->runJavaScript(js.str().c_str());
			File::remove(tmp_filename);
			return true;
		}

		void BALLaxyInterface::contextMenuEvent(QContextMenuEvent*)
		{
			// here, we could, e.g., download structures to BALLView from a context menu in the web page
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

		void BALLaxyInterface::verifyDownloadRequest(QWebEngineDownloadItem* item)
		{
			if (item->url().fileName() == "display" && item->url().query().startsWith("to_ext=")) {
				item->accept();
				connect(item, SIGNAL(finished()), this, SLOT(openStructure()));
			}
			else
			{
				Log.error() << "BALLaxy plugin: Invalid download request" << std::endl;
				item->cancel();
			}
		}

		void BALLaxyInterface::openStructure()
		{
			QWebEngineDownloadItem* item = dynamic_cast<QWebEngineDownloadItem*>(sender());
			if (!item)
			{
				Log.error() << "BALLaxy plugin: Failed to open file (unknown)" << std::endl;
				return;
			}

			String filename = item->path().toStdString();
			QStringList args = item->url().query().split("to_ext=");
			if (args.size() == 2 && MolFileFactory::isFileExtensionSupported(args[1].toStdString()))
			{
				System* system = new System();

				GenericMolFile* molfile = MolFileFactory::open(filename, std::ios::in);
				*molfile >> *system;
				molfile->close();

				getMainControl()->setStatusbarText(String("read ") + String(system->countAtoms()) + " atoms from BALLaxy", true);

				// TODO there is currently (Qt 5.5) no simple way to retrieve the real filename from the headers using
				// QtWebEngine's download mechanism... So we set it to a static value for now.
				system->setName("System from BALLaxy");

				getMainControl()->insert(*system, system->getName());

				CompositeMessage* cm = new CompositeMessage(*system, CompositeMessage::CENTER_CAMERA);
				qApp->postEvent(parent(), new MessageEvent(cm));
			}
			else
			{
				Log.error() <<  "BALLaxy plugin: cannot open file " << item->url().toString().toStdString() << std::endl;
			}

			File::remove(filename);
		}
	}
}
