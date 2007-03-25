// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include <BALL/VIEW/DIALOGS/pubchemDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/STRUCTURE/sdGenerator.h>

#include <QtGui/qpushbutton.h>
#include <QtGui/qtreewidget.h>

#include "../SOAP/soapeUtilsServiceSoapProxy.h" // get proxy 
#include "../SOAP/eUtilsServiceSoap.nsmap" // obtain the namespace mapping table 

namespace BALL
{
	namespace VIEW
	{
		PubChemDialog::PubChemDialog(QWidget* parent, const char* name)
			throw()
			: QDialog(parent),
				Ui_PubChemDialogData(),
				ModularWidget(name),
				action1_(0),
				action2_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new PubChemDialog " << this << std::endl;
			#endif

			setupUi(this);
			setObjectName(name);

			sdwidget_.setParent(SDFrame);
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			hide();
			connect(generate_button, SIGNAL(clicked()), this, SLOT(generateButtonClicked()));
			connect(cancel_button, SIGNAL(clicked()), this, SLOT(hide()));
			connect(search_button, SIGNAL(clicked()), this, SLOT(queryPubChem()));
			connect(clear_button, SIGNAL(clicked()), this, SLOT(clearEntries()));
			connect(ok_button, SIGNAL(clicked()), this, SLOT(finished()));
			connect(queries, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(switchView(QTreeWidgetItem*, int)));

			queries->setColumnCount(1);
			queries->headerItem()->setText(0, "Results");
		}
	
		PubChemDialog::~PubChemDialog()
			throw()
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "deleting PubChemDialog " << this << std::endl;
#endif

			clearEntries();
		}

		void PubChemDialog::finished()
		{
			hide();
			
			QTreeWidgetItem* item = queries->currentItem();
			// is this item connected with a system?
			if (sd_systems_.find(item) == sd_systems_.end()) return;

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
			sdwidget_.clear();
			text_field->clear();
		}

		void PubChemDialog::generateFromSMILES(const String& SMILES)
			throw()
		{
			try
			{
				smiles_parser_.parse(SMILES);
			}
			catch(...)
			{
				setStatusbarText("Invalid SMILES expression!", true);
				return;
			}

			ParsedResult_ pr;
			pr.smiles = SMILES;
			pr.description = "Generated from SMILES "+SMILES;
			pr.name = SMILES;
			insert_(pr, 0, true);
		}

		void PubChemDialog::show()
		{
			QDialog::show();
			raise();
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
				sdwidget_.plot(*sd_systems_[item], true, false);
			}

			// do we have a description for it?
			if (descriptions_.find(item) != descriptions_.end())
			{
				text_field->setText(QString(descriptions_[item].description.c_str()));
				ok_button->setEnabled(true);
			}
			else
			{
				ok_button->setEnabled(true);
			}
		}

		void PubChemDialog::queryPubChem()
		{
			String qt = ascii(pubchem_label->displayText());
			if (qt == "") return;

			eUtilsServiceSoap q; 
			std::string search_term(qt);
			std::string db_name = "pcsubstance";
			std::string usehistory = "y";

			_ns5__eSearchRequest*  search_request = new _ns5__eSearchRequest;
	
			search_request->db   = &db_name;
			search_request->term = &search_term;
			search_request->usehistory = &usehistory;

			_ns5__eSearchResult* search_result = new _ns5__eSearchResult;

			if (q.__ns1__run_USCOREeSearch(search_request, search_result) != SOAP_OK) 
			{
				Log.error() << "Query failed!" << std::endl;
				return;
			}

			// now call eSummary
			std::string webenv(*(search_result->WebEnv));
			_ns6__eSummaryRequest* summary_request = new _ns6__eSummaryRequest;
			summary_request->db 		 = &db_name;
			summary_request->WebEnv = search_result->WebEnv;
			summary_request->query_USCOREkey = search_result->QueryKey;
			_ns6__eSummaryResult*  summary_result  = new _ns6__eSummaryResult;

			if (q.__ns1__run_USCOREeSummary(summary_request, summary_result) != SOAP_OK) 
			{
				Log.info() << "ESummary failed!" << std::endl;
				return;
			}

			// generate the tree entries
			QList<QTreeWidgetItem *> query_results;
			QTreeWidgetItem* query_item = new QTreeWidgetItem((QTreeWidget*)0, QStringList(pubchem_label->displayText()));
			query_results.append(query_item);
			queries->insertTopLevelItems(0, query_results);
			queries->expandItem(query_item);

			Index max = (int)std::min(10,(int)summary_result->DocSum.size()) - 1;
			for (int i = 0; i <= max; i++)
			{
				ParsedResult_ pr = parseResultRecursive_(summary_result->DocSum[i]);

				// update the maps
				//std::cout << "smiles: " << std::endl;
				if (pr.smiles != "")
				{
					//std::cout << pr.smiles << std::endl;
					try 
					{
						smiles_parser_.parse(pr.smiles);
					} 
					catch (...) 
					{ 
						continue;
					}

					insert_(pr, query_item, i == max);
				}

			}
		}

		void PubChemDialog::insert_(ParsedResult_ pr, QTreeWidgetItem* parent, bool plot)
		{
			QTreeWidgetItem* current_item = new QTreeWidgetItem(parent, QStringList(pr.name.c_str()));
			descriptions_[current_item] = pr;
			System S = smiles_parser_.getSystem();
			original_systems_[current_item] = new System(S);

			SDGenerator sdg;
			sdg.generateSD(S);

			if (plot)
			{
				sdwidget_.plot(S, true, false);
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

		String PubChemDialog::parseItemRecursive_(ns6__ItemType* item, int level)
		{
			String result;

			for (int i=0; i<level; i++) result+="\t";

			if (item->__size_ItemType == 0)
			{
				if (level == 0) result+=item->Name+String(": ");

				result+=item->__mixed+String("\n");
			}
			else
			{
				result += item->Name+String(": ");
				for (int i=0; i<item->__size_ItemType; i++)
				{
					result+=parseItemRecursive_(item->__union_ItemType[i].union_ItemType.Item, level+1);
				}
			}

			return result;
		}

		PubChemDialog::ParsedResult_ PubChemDialog::parseResultRecursive_(ns6__DocSumType* result)
		{
			ParsedResult_ pr;
			for (int i=0; i<(int)result->Item.size(); i++)
			{
				if (result->Item[i]->Name == "IUPACName")
				{
					pr.name = String(result->Item[i]->__mixed);
				}
				else if (result->Item[i]->Name == "CanonicalSmile")
				{
					pr.smiles = String(result->Item[i]->__mixed);
				}
				else
				{
					pr.description += parseItemRecursive_(result->Item[i]);
				}
			}

			return pr;
		}

		void PubChemDialog::initializeWidget(MainControl&)
		{
			action1_ = insertMenuEntry(MainControl::FILE_OPEN, "PubChem", this, SLOT(show()));
			setMenuHint("Download a file from pubchem");
			setIcon("pubchem.png", true);

			action2_ = insertMenuEntry(MainControl::BUILD, "Build from SMILES", this, SLOT(show()));
			setMenuHint("Create a structure from a SMILES expression");
		}

		void PubChemDialog::checkMenu(MainControl& main_control)
			throw()
		{
			bool busy = main_control.compositesAreLocked();
			action1_->setEnabled(!busy);
			action2_->setEnabled(!busy);
		}

	}
}
