/* TRANSLATOR BALL::VIEW::PubChemDialog

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/pubchemDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/FORMAT/SDFile.h>

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
			//
			// TODO: re-enable the status bar...
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

			String filename;
			File::createTemporaryFilename(filename, ".sdf");

			getMainControl()->setStatusbarText(String(tr("Performing query...")));
			PubChemDownloader pcd;
			pcd.downloadSDF(qt, filename);

			// now, try to read the SD File
			SDFile sdf(filename, std::ios::in);

			// iterate over the molecules in the SD File and build a system for each
			Size count = 0;

			Molecule *current_molecule;
			std::deque<System*> sd_systems;
			do {
				current_molecule = sdf.read();	

				if (current_molecule)
				{
					System* new_system = new System(qt);
					new_system->insert(*current_molecule);

					if (current_molecule->hasProperty("PUBCHEM_IUPAC_NAME"))
					{
						current_molecule->setName(current_molecule->getProperty("PUBCHEM_IUPAC_NAME").toString());
					}

					sd_systems.push_back(new_system);
				}
				++count;
			} while(current_molecule && (count < 20)); // TODO: find a way to limit the *search* directly
			sdf.close();
			File::remove(filename);

			QTreeWidgetItem* new_query_result = new QTreeWidgetItem((QTreeWidget*)0, 
																														 QStringList(pubchem_label->displayText()
																														             +" ("+QString::number(sd_systems.size())+ ")"));
			queries->insertTopLevelItem(0, new_query_result);
			queries->expandItem(new_query_result);

			QTreeWidgetItem* first_item = NULL;
			for (std::deque<System*>::iterator sys_it  = sd_systems.begin(); 
					                               sys_it != sd_systems.end(); ++sys_it)
			{
				ParsedResult_ pr;
				pr.name = (*sys_it)->beginMolecule()->getName();

				NamedPropertyIterator npi = (*sys_it)->beginMolecule()->beginNamedProperty();
				for (; npi != (*sys_it)->beginMolecule()->endNamedProperty(); ++npi)
				{
					String prop_name = npi->getName();
					if (prop_name.hasPrefix("PUBCHEM_"))
					{
						pr.description += String("<b>")+prop_name.after("PUBCHEM_")+"</b><br/>";
						pr.description += npi->toString()+"<br/>";
					}
				}

				QTreeWidgetItem* current_item = new QTreeWidgetItem(new_query_result, QStringList(QString(pr.name.c_str())));
				descriptions_[current_item] = pr;
				
				sdwidget_->plot(**sys_it, false);
				sd_systems_[current_item] = *sys_it;

				queries->setCurrentItem(current_item);
				current_item->setSelected(true);
				switchView(current_item, 0);
				queries->setCurrentItem(current_item);
				current_item->setSelected(true);

				if (!first_item)
				{
					first_item = current_item;
				}
				queries->update();
			}

			if (first_item)
			{
				first_item->setSelected(true);
				queries->setCurrentItem(first_item);
				first_item->setSelected(true);
				switchView(first_item, 0);
			}
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
	}
}
