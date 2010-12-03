// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/pubchemDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/STRUCTURE/sdGenerator.h>

#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QProgressBar>

#include <QtCore/QUrl>

#include <QtXml/QDomDocument>

#include <QtNetwork/QNetworkProxy>

#include <algorithm>

namespace BALL
{
	namespace VIEW
	{
		PubChemDialog::PubChemDialog(QWidget* parent, const char* name)
			: QDialog(parent),
				Ui_PubChemDialogData(),
				ModularWidget(name),
				action1_(0),
				action2_(0),
				esearch_base_url_("http://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi"), // TODO: make configurable
				esummary_base_url_("http://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi"), // TODO: make configurable
				progress_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new PubChemDialog " << this << std::endl;
			#endif

			setupUi(this);
			setObjectName(name);

			// register the widget with the MainControl
			ModularWidget::registerWidget(this);

			add_button_ = buttonBox->addButton(tr("Add"), QDialogButtonBox::AcceptRole);
			QPushButton* clear = buttonBox->addButton(tr("Clear Results"), QDialogButtonBox::ResetRole);

			connect(generate_button, SIGNAL(clicked()), this, SLOT(generateButtonClicked()));
			connect(search_button, SIGNAL(clicked()), this, SLOT(queryPubChem()));
			connect(clear, SIGNAL(clicked()), this, SLOT(clearEntries()));
			connect(queries, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(switchView(QTreeWidgetItem*, int)));

			queries->setColumnCount(1);
			queries->headerItem()->setText(0, tr("Results"));

			// connect the Http connectors...
			connect(&esearch_connector_,  SIGNAL(requestFinished(int, bool)), this, SLOT( esearchFinished(int, bool)));
			connect(&esummary_connector_, SIGNAL(requestFinished(int, bool)), this, SLOT(esummaryFinished(int, bool)));
		}
	
		PubChemDialog::~PubChemDialog()
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "deleting PubChemDialog " << this << std::endl;
#endif

			clearEntries();
		}

		void PubChemDialog::finished()
		{
			QTreeWidgetItem* item = queries->currentItem();
			// is this item connected with a system?
			if (sd_systems_.find(item) == sd_systems_.end()) return;

			hide();

			ParsedResult_& res = descriptions_.find(item)->second;

			System* org_system = sd_systems_.find(item)->second;
			System* new_system = new System(*org_system);

			new_system->setName(res.name);
			getMainControl()->insert(*new_system);
			getMainControl()->update(*new_system);
		}

		void PubChemDialog::clearEntries()
		{
			std::map<QTreeWidgetItem*, System*>::iterator it = sd_systems_.begin();
			for (; it != sd_systems_.end(); it++)
			{
				delete it->second;
			}

			it = original_systems_.begin();
			for (; it != original_systems_.end(); it++)
			{
				delete it->second;
			}

			sd_systems_.clear();
			original_systems_.clear();
			descriptions_.clear();
			queries->clear();
			sdwidget_->clear();
			text_field->clear();
		}

		void PubChemDialog::generateFromSMILES(const String& SMILES)
		{
			try
			{
				smiles_parser_.parse(SMILES);
			}
			catch(...)
			{
				setStatusbarText((String)tr("Invalid SMILES expression!"), true);
				return;
			}

			ParsedResult_ pr;
			pr.smiles = SMILES;
			pr.description = (String)tr("Generated from SMILES ")+SMILES;
			pr.name = SMILES;
			insert_(pr, 0, true);
		}

		void PubChemDialog::show()
		{
			QDialog::show();
			raise();
			add_button_->setDefault(false);
		}

		void PubChemDialog::generateButtonClicked()
		{
			generateFromSMILES(String(ascii(smiles_label->displayText())));
		}

		void PubChemDialog::switchView(QTreeWidgetItem* item, int /*column*/)
		{
			// is this item connected with a system?
			if (sd_systems_.find(item) != sd_systems_.end())
			{
				sdwidget_->plot(*sd_systems_[item], false);
			}

			// do we have a description for it?
			if (descriptions_.find(item) != descriptions_.end())
			{
				text_field->setText(QString(descriptions_[item].description.c_str()));
			}

			add_button_->setEnabled(true);
		}

		void PubChemDialog::queryPubChem()
		{
			String qt = ascii(pubchem_label->displayText());
			if (qt == "") return;

			// first, perform the search
			QUrl search_url(esearch_base_url_.c_str());
			search_url.addQueryItem("db", "pcsubstance");
			search_url.addQueryItem("usehistory", "n");
			search_url.addQueryItem("term", pubchem_label->displayText());

			esearch_connector_.setHost(QUrl::toAce(search_url.host()));
			current_request_id_ = esearch_connector_.get(search_url.encodedPath()+"?"+search_url.encodedQuery());
		}

		void PubChemDialog::insert_(ParsedResult_ pr, QTreeWidgetItem* parent, bool plot)
		{
			QTreeWidgetItem* current_item = new QTreeWidgetItem(parent, QStringList(pr.name.c_str()));
			descriptions_[current_item] = pr;
			System S;
			
			try
			{
				smiles_parser_.parse(pr.smiles);
				S = smiles_parser_.getSystem();

				original_systems_[current_item] = new System(S);

				SDGenerator sdg;
				sdg.generateSD(S);
			}
			catch(...)
			{
				Log.info() << (String)tr("Secondary structure plotting failed");
			}

			if (plot)
			{
				sdwidget_->plot(S, false);
				switchView(current_item, 0);
			}

			sd_systems_[current_item] = new System(S);
			if (parent == 0)
			{
				QList<QTreeWidgetItem *> structure;
				structure.append(current_item);
				queries->insertTopLevelItems(0, structure);
			}

			queries->setCurrentItem(current_item);
			current_item->setSelected(true);
			queries->update();
		}

		void PubChemDialog::parseItemRecursive_(const QDomNode& current_node, Position level, ParsedResult_& result)
		{
			// the special "Id" node is just ignored
			if (current_node.nodeName() == "Id")
				return;

			// are we finally at a leaf node?
			if (current_node.isText())
			{
				result.description += ascii(current_node.nodeValue())+"<br/>";	
			}

			// nope. we have children to cover.

			// first, determine if we have to put our own "Name"-attribute as a
			// description. We do this whenever it is not too generic (i.e. "string")
			QString node_name = "string";

			if (current_node.hasAttributes())
			{
				if (current_node.attributes().contains("Name"))
					node_name = current_node.attributes().namedItem("Name").nodeValue();
			}

			// two kinds of nodes are special: IUPACNames (we need them for the heading) and
			// CanonicalSmiles (to generate the molecule)
			if (node_name == "IUPACName")
			{
				result.name = ascii(current_node.firstChild().nodeValue());
			} 
			else if (node_name == "CanonicalSmile")
			{
				result.smiles = ascii(current_node.firstChild().nodeValue());
			}

			QDomNodeList children = current_node.childNodes();
			Size num_children = children.length();
			bool text_only = (num_children == 1) && children.item(0).isText();

			if (node_name != "string" && node_name != "int")
			{
				for (Position i=0; i<level-1; ++i) result.description += "&nbsp;&nbsp;";

				result.description += "<b>"+ascii(node_name)+": </b>";

				if (!text_only)
					result.description += "<br/>";
			}

			for (Position i=0; i<num_children; ++i)
			{
				if (!text_only)
					for (Position i=0; i<level; ++i) result.description += "&nbsp;&nbsp;";

				parseItemRecursive_(children.item(i), level+1, result);
			}
		}

		void PubChemDialog::initializeWidget(MainControl&)
		{
			action1_ = insertMenuEntry(MainControl::FILE_OPEN, (String)tr("PubChem"), this, SLOT(show()), "Shortcut|File|Open|PubChem");
			setMenuHint((String)tr("Download a file from pubchem"));
			setIcon("actions/download-pubchem", true);
			
			action2_ = insertMenuEntry(MainControl::BUILD, (String)tr("Build from SMILES"), this, SLOT(show()), "Shortcut|Build|Build_from_SMILES");
			setMenuHint((String)tr("Create a structure from a SMILES expression"));
		}

		void PubChemDialog::checkMenu(MainControl& main_control)
		{
			bool busy = main_control.compositesAreLocked();
			action1_->setEnabled(!busy);
			action2_->setEnabled(!busy);
		}

		void PubChemDialog::esearchFinished(int id, bool error)
		{
			if (error)
			{
				Log.error() << "Error while contacting PubChem! Reason given was: " << ascii(esearch_connector_.errorString()) << std::endl;
				getMainControl()->setStatusbarText((String)tr("Error in connecting to PubChem!"), true);

				return;
			}

			if (id == current_request_id_)
			{
				QString error_msg;
				int error_line, error_col;

				QByteArray buffer = esearch_connector_.readAll();

				QDomDocument dom;
				bool ok = dom.setContent(buffer, true, &error_msg, &error_line, &error_col);

				if (!ok)
				{
					Log.error() << "Error while parsing PubChem esearch results in line " << error_line << " col " << error_col << std::endl;
					Log.error() << "Reason given was: " << ascii(error_msg) << std::endl;

					getMainControl()->setStatusbarText((String)tr("Error in parsing PubChem esearch results!"), true);

					return;
				}

				QDomNode num_total_node = dom.elementsByTagName("Count").item(0);
				QDomNode num_download_node = dom.elementsByTagName("RetMax").item(0);

				Size num_total = 0;
				Size num_download = 0;

				if (!num_total_node.isNull() && !num_download_node.isNull())
				{
					 num_total    =    num_total_node.firstChild().nodeValue().toInt();
					 num_download = num_download_node.firstChild().nodeValue().toInt();

					getMainControl()->setStatusbarText(String(tr("Downloading "))+String(num_download)+" of "+String(num_total)+(String)tr(" entries"));
				}

				QDomNode idList = dom.elementsByTagName("IdList").item(0);

				if (idList.isNull())
				{
					Log.info() << (String)tr("No entries found!") << std::endl;

					return;
				}

				QDomNodeList entries = idList.toElement().elementsByTagName("Id");

				Size num_entries = entries.length();

				if (num_entries > 0)
				{
					if (progress_)
					{
						delete(progress_);
					}

					progress_ = new QProgressBar(queries);
					progress_->setRange(0, num_download);
					progress_->resize(queries->width(), progress_->height());
					progress_->move(0, queries->height()-progress_->height());
					progress_->show();

					QTreeWidgetItem* new_query_result = new QTreeWidgetItem((QTreeWidget*)0, 
					                                                        QStringList(pubchem_label->displayText()
																																		+" ("+QString::number(num_download)+"/"+QString::number(num_total)+")"));
					queries->insertTopLevelItem(0, new_query_result);
					queries->expandItem(new_query_result);

					for (Position i=0; i<num_entries; ++i)
					{
						if (!entries.item(i).firstChild().isNull())
						{
							callESummary(entries.item(i).firstChild().nodeValue(), new_query_result);
						}
					}
				}
			}
		}

		void PubChemDialog::callESummary(QString const& entry_id, QTreeWidgetItem* current_item)
		{
			// now prepare the download
			QUrl summary_url(esummary_base_url_.c_str());
			summary_url.addQueryItem("db", "pcsubstance");
			summary_url.addQueryItem("id", entry_id);

			esummary_connector_.setHost(QUrl::toAce(summary_url.host()));

			int id = esummary_connector_.get(summary_url.encodedPath()+"?"+summary_url.encodedQuery());
			esummary_request_ids_[id] = current_item;
		}

		void PubChemDialog::esummaryFinished(int id, bool error)
		{
			// see if we have been looking for this one...
			if (esummary_request_ids_.has(id) && !error)
			{
				if (progress_)
				{
					progress_->setValue(progress_->value()+1);
				}

				HashMap<int, QTreeWidgetItem*>::iterator list_it = esummary_request_ids_.find(id);
				QTreeWidgetItem* current_item = list_it->second;

				esummary_request_ids_.erase(list_it);
	
				// parse the file and insert results
				ParsedResult_ parsed_result;

				if (parseESummaryXml_(esummary_connector_.readAll(), parsed_result) && current_item)
					insert_(parsed_result, current_item, true);
			}

			if (esummary_request_ids_.size() == 0)
			{
				delete (progress_);
				progress_ = 0;
			}
		}

		bool PubChemDialog::parseESummaryXml_(const QByteArray& data, ParsedResult_& result)
		{
			QString error_msg;
			int error_line, error_col;

			QDomDocument esummary;
			bool ok = esummary.setContent(data, true, &error_msg, &error_line, &error_col);	

			if (!ok)
			{
				Log.error() << "Could not parse PubChem ESummary! Error in line " << error_line 
										<< " col " << error_col << std::endl;

				Log.error() << "Reason given was: " << ascii(error_msg) << std::endl;

				getMainControl()->setStatusbarText((String)tr("Error in parsing PubChem esummary results!"), true);

				return false;
			}

			QDomNodeList item_list = esummary.elementsByTagName("DocSum");

			for (Position i=0; i<item_list.length(); ++i)
				parseItemRecursive_(item_list.item(i), 0, result);

			if (result.name == "")
			{
				result.name = "(unknown)";
			}

			if (result.smiles == "")
			{
				Log.error() << "Error: PubChem ESummary contains no CanonicalSmile! Skipping this entry!"
				            << std::endl;

				getMainControl()->setStatusbarText((String)tr("Error in parsing PubChem esummary results!"), true);

				return false;
			}

			return true;
		}
	}
}
