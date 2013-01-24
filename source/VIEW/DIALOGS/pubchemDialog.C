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
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QProgressBar>
#include <QtGui/QMessageBox>

#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QTemporaryFile>

#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <algorithm>
#include <BALL/VIEW/UIC/ui_pubchemDialog.h>

namespace BALL
{
	namespace VIEW
	{
		PubChemDialog::PubChemDialog ( QWidget* parent, const char* name )
			: QDialog ( parent ),
			  ModularWidget ( name ),
			  action1_ ( 0 ),
			  action2_ ( 0 ),
			  network_manager_(0)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new PubChemDialog " << this << std::endl;
#endif
			ui_ = new Ui::PubChemDialogData;
			ui_->setupUi ( this );

			setObjectName ( name );

			ui_->progress_bar->hide();

			// register the widget with the MainControl
			ModularWidget::registerWidget ( this );

			add_button_ = ui_->buttonBox->addButton ( tr ( "Add" ), QDialogButtonBox::AcceptRole );
			QPushButton* clear = ui_->buttonBox->addButton ( tr ( "Clear Results" ), QDialogButtonBox::ResetRole );

			connect ( ui_->generate_button, SIGNAL ( clicked() ), this, SLOT ( generateButtonClicked() ) );
			connect ( ui_->search_button, SIGNAL ( clicked() ), this, SLOT ( queryPubChem() ) );
			connect ( clear, SIGNAL ( clicked() ), this, SLOT ( clearEntries() ) );
			connect ( ui_->queries, SIGNAL ( itemActivated ( QTreeWidgetItem*, int ) ), this, SLOT ( switchView ( QTreeWidgetItem*, int ) ) );

			ui_->queries->setColumnCount ( 1 );
			ui_->queries->headerItem()->setText ( 0, tr ( "Results" ) );
			//
			// TODO: re-enable the status bar...
			buildInformationTemplates_();
		}

		PubChemDialog::~PubChemDialog()
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "deleting PubChemDialog " << this << std::endl;
#endif
			clearEntries();

			delete ui_;
		}

		void PubChemDialog::finished()
		{
			QTreeWidgetItem* item = ui_->queries->currentItem();

			// is this item connected with a system?
			if ( sd_systems_.find ( item ) == sd_systems_.end() ) return;

			hide();

			ParsedResult_& res = descriptions_.find ( item )->second;

			System* org_system = sd_systems_.find ( item )->second;
			System* new_system = new System ( *org_system );

			new_system->setName ( res.name.toStdString() );
			getMainControl()->insert ( *new_system );
			getMainControl()->update ( *new_system );
		}

		void PubChemDialog::clearEntries()
		{
			std::map<QTreeWidgetItem*, System*>::iterator it = sd_systems_.begin();

			for ( ; it != sd_systems_.end(); it++ )
			{
				delete it->second;
			}

			it = original_systems_.begin();

			for ( ; it != original_systems_.end(); it++ )
			{
				delete it->second;
			}

			sd_systems_.clear();
			original_systems_.clear();
			descriptions_.clear();
			ui_->queries->clear();
			ui_->sdwidget_->clear();
			ui_->text_field->clear();
		}

		void PubChemDialog::generateFromSMILES ( const String& SMILES )
		{
			try
			{
				smiles_parser_.parse ( SMILES );
			}
			catch ( ... )
			{
				setStatusbarText ( ( String ) tr ( "Invalid SMILES expression!" ), true );
				return;
			}

			ParsedResult_ pr;
			pr.smiles = SMILES;
			pr.description = tr ( "Generated from SMILES " ) + SMILES.c_str();
			pr.name = SMILES.c_str();
			insert_ ( pr, 0, true );
		}

		void PubChemDialog::show()
		{
			QDialog::show();
			raise();
			add_button_->setDefault ( false );
		}

		void PubChemDialog::generateButtonClicked()
		{
			generateFromSMILES ( String ( ascii ( ui_->smiles_label->displayText() ) ) );
		}

		void PubChemDialog::switchView ( QTreeWidgetItem* item, int /*column*/ )
		{
			// is this item connected with a system?
			if ( sd_systems_.find ( item ) != sd_systems_.end() )
			{
				ui_->sdwidget_->plot ( *sd_systems_[item], false );
			}

			// do we have a description for it?
			if ( descriptions_.find ( item ) != descriptions_.end() )
			{
				ui_->text_field->setText ( descriptions_[item].description );
			}

			add_button_->setEnabled ( true );
		}

		void PubChemDialog::queryPubChem()
		{
			QString qt = ui_->pubchem_label->displayText();

			if ( qt == "" ) return;
			QString pug_query_("http://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/%1/%2/SDF");

			if(!network_manager_) {
				network_manager_ = new QNetworkAccessManager(this);

				connect(network_manager_, SIGNAL(finished(QNetworkReply*)),
				        this, SLOT(downloadFinished(QNetworkReply*)));
			}

			getMainControl()->setStatusbarText ( String ( tr ( "Performing query..." ) ) );

			QNetworkReply* reply  = network_manager_->get(QNetworkRequest(QUrl(pug_query_.arg(ui_->query_type_box->currentText().toLower()).arg(qt))));

			connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
			        this, SLOT(updateDownloadProgress(qint64,qint64)));
	
			ui_->progress_bar->setMaximum(0);
			ui_->progress_bar->show();

			ui_->search_button->setEnabled ( false );
		}

		bool PubChemDialog::handleDownloadError_(QNetworkReply* reply)
		{
			QString errormessage;

			switch(reply->error()) {
				case QNetworkReply::NoError:
					return true;
				case QNetworkReply::ContentNotFoundError:
					errormessage = tr("Could not find the requested entry!");
					break;
				default:
					errormessage = tr("Unknown error!");
			}

			QMessageBox::critical(this, tr( "Download failed"), errormessage + "\n\nThe Server responded:\n" + reply->errorString());

			return false;
		}

		void PubChemDialog::downloadFinished ( QNetworkReply* reply )
		{
			disconnect(reply);

			if(!handleDownloadError_(reply))
			{
				ui_->search_button->setEnabled ( true );
				ui_->progress_bar->hide();

				return;
			}

			SDFile sdf;

			QTemporaryFile outfile(QDir::tempPath() + "/XXXXXX.sdf");

			if(!outfile.open()) {
				QMessageBox::critical ( this, tr ( "Download failed" ),
				                        tr ( "Could not open download destination (%1) for query %2").arg(outfile.fileName()).arg(ui_->pubchem_label->displayText()));
				ui_->search_button->setEnabled ( true );
				ui_->progress_bar->hide();

				return;
			}
			outfile.write(reply->readAll());
			outfile.close();

			// now, try to read the SD File
			sdf.open(outfile.fileName().toStdString(), std::ios::in);

			// iterate over the molecules in the SD File and build a system for each
			Size count = 0;

			sdf.enableAtoms();

			Molecule* current_molecule;
			std::deque<System*> sd_systems;

			try
			{
				while( (current_molecule = sdf.read()) && count < 20)
				{
					if ( current_molecule )
					{
						String qt = ascii ( ui_->pubchem_label->displayText() );
						System* new_system = new System ( qt );

						new_system->insert ( *current_molecule );

						if ( current_molecule->hasProperty ( "PUBCHEM_IUPAC_NAME" ) )
						{
							current_molecule->setName ( current_molecule->getProperty ( "PUBCHEM_IUPAC_NAME" ).toString() );
						}

						sd_systems.push_back ( new_system );
					}

					++count;
				}
			}
			catch ( Exception::ParseError& exn )
			{
				QMessageBox::critical ( this, tr ( "Download failed" ), tr ( "Error while parsing downloaded SD file. Aborting" ) );
				Log.error() << exn.getMessage() << std::endl;
				return;
			}

			sdf.close();

			QTreeWidgetItem* new_query_result = new QTreeWidgetItem ( ( QTreeWidget* ) 0,
			        QStringList ( ui_->pubchem_label->displayText()
			                      +" ("+QString::number ( sd_systems.size() ) + ")" ) );
			ui_->queries->insertTopLevelItem ( 0, new_query_result );
			ui_->queries->expandItem ( new_query_result );

			QTreeWidgetItem* first_item = NULL;

			for ( std::deque<System*>::iterator sys_it  = sd_systems.begin();
			        sys_it != sd_systems.end(); ++sys_it )
			{
				ParsedResult_ pr;
				pr.name = ( *sys_it )->beginMolecule()->getName().c_str();

				NamedPropertyIterator npi = ( *sys_it )->beginMolecule()->beginNamedProperty();

				for ( ; npi != ( *sys_it )->beginMolecule()->endNamedProperty(); ++npi )
				{
					String prop_name = npi->getName();
					prop_name.trim();

					boost::unordered_map<String, QString>::iterator it = information_templates_.find(prop_name);
					if ( it != information_templates_.end() )
					{
						pr.description += it->second.arg(npi->toString().c_str());
					}
				}

				QTreeWidgetItem* current_item = new QTreeWidgetItem ( new_query_result, QStringList ( pr.name) );
				descriptions_[current_item] = pr;

				ui_->sdwidget_->plot ( **sys_it, false );
				sd_systems_[current_item] = *sys_it;

				ui_->queries->setCurrentItem ( current_item );
				current_item->setSelected ( true );
				switchView ( current_item, 0 );
				ui_->queries->setCurrentItem ( current_item );
				current_item->setSelected ( true );

				if ( !first_item )
				{
					first_item = current_item;
				}

				ui_->queries->update();
			}

			if ( first_item )
			{
				first_item->setSelected ( true );
				ui_->queries->setCurrentItem ( first_item );
				first_item->setSelected ( true );
				switchView ( first_item, 0 );
			}

			ui_->search_button->setEnabled ( true );
			ui_->progress_bar->hide();
		}

		void PubChemDialog::buildInformationTemplates_()
		{
			information_templates_.insert(std::make_pair("PUBCHEM_IUPAC_NAME"           , buildHeaderTemplate_(LONG, "IUPAC Name:")));
			information_templates_.insert(std::make_pair("PUBCHEM_OPENEYE_CAN_SMILES"   , buildHeaderTemplate_(LONG, "Canonical SMILES:")));
			information_templates_.insert(std::make_pair("PUBCHEM_MOLECULAR_FORMULA"    , buildHeaderTemplate_(LONG, "Molecular Formula:")));
			information_templates_.insert(std::make_pair("PUBCHEM_COMPOUND_CID"         , buildHeaderTemplate_(SHORT, "Pubchem Database Key:")));
			information_templates_.insert(std::make_pair("PUBCHEM_XLOGP3"               , buildHeaderTemplate_(SHORT, "Predicted logP:")));
			information_templates_.insert(std::make_pair("PUBCHEM_CACTVS_TPSA"          , buildHeaderTemplate_(SHORT, "Predicted Polar Surface Area (TPSA):")));
			information_templates_.insert(std::make_pair("PUBCHEM_CACTVS_HBOND_DONOR"   , buildHeaderTemplate_(SHORT, "Number of H-Bond Donors:")));
			information_templates_.insert(std::make_pair("PUBCHEM_CACTVS_HBOND_ACCEPTOR", buildHeaderTemplate_(SHORT, "Number of H-Bond Acceptors:")));
			information_templates_.insert(std::make_pair("PUBCHEM_CACTVS_ROTATABLE_BOND", buildHeaderTemplate_(SHORT, "Number of Rotatable Bonds:")));
			information_templates_.insert(std::make_pair("PUBCHEM_TOTAL_CHARGE"         , buildHeaderTemplate_(SHORT, "Total Charge:")));
			information_templates_.insert(std::make_pair("PUBCHEM_ATOM_DEF_STEREO_COUNT", buildHeaderTemplate_(SHORT, "Number of Chiral Atoms:")));
			information_templates_.insert(std::make_pair("PUBCHEM_HEAVY_ATOM_COUNT"     , buildHeaderTemplate_(SHORT, "Number of Heavy Atoms:")));
			information_templates_.insert(std::make_pair("PUBCHEM_EXACT_MASS"           , buildHeaderTemplate_(SHORT, "Exact Mass (Most Intense MS Peak):")));
			information_templates_.insert(std::make_pair("PUBCHEM_MONOISOTOPIC_WEIGHT"  , buildHeaderTemplate_(SHORT, "Monoisotopic Weight (Most abundant isotopes):")));
			information_templates_.insert(std::make_pair("PUBCHEM_MOLECULAR_WEIGHT"     , buildHeaderTemplate_(SHORT, "Molecular Weight (Average weight):")));
		}

		QString PubChemDialog::buildHeaderTemplate_(InfoDisplayStyle style, const char* str) const
		{
			return "<b>" + tr(str) + "</b>" + ((style == LONG) ? "<br/>" : " ") + "%1<br/><br/>";
		}

		void PubChemDialog::insert_ ( ParsedResult_ pr, QTreeWidgetItem* parent, bool plot )
		{
			QTreeWidgetItem* current_item = new QTreeWidgetItem ( parent, QStringList ( pr.name ) );
			descriptions_[current_item] = pr;
			System S;

			try
			{
				smiles_parser_.parse ( pr.smiles );
				S = smiles_parser_.getSystem();

				original_systems_[current_item] = new System ( S );

				SDGenerator sdg;
				sdg.generateSD ( S );
			}
			catch ( ... )
			{
				Log.info() << ( String ) tr ( "Secondary structure plotting failed" );
			}

			if ( plot )
			{
				ui_->sdwidget_->plot ( S, false );
				switchView ( current_item, 0 );
			}

			sd_systems_[current_item] = new System ( S );

			if ( parent == 0 )
			{
				QList<QTreeWidgetItem*> structure;
				structure.append ( current_item );
				ui_->queries->insertTopLevelItems ( 0, structure );
			}

			ui_->queries->setCurrentItem ( current_item );
			current_item->setSelected ( true );
			ui_->queries->update();
		}


		void PubChemDialog::initializeWidget ( MainControl& )
		{
			action1_ = insertMenuEntry ( MainControl::FILE_OPEN, tr ( "PubChem" ), this,
			                             SLOT ( show() ), "Shortcut|File|Open|PubChem", QKeySequence(),
			                             tr ( "Download a file from pubchem" ),
			                             UIOperationMode::MODE_ADVANCED );
			setIcon ( action1_, "actions/download-pubchem", true );

			action2_ = insertMenuEntry ( MainControl::BUILD, tr ( "Build from SMILES" ), this,
			                             SLOT ( show() ), "Shortcut|Build|Build_from_SMILES", QKeySequence(),
			                             tr ( "Create a structure from a SMILES expression" ),
			                             UIOperationMode::MODE_ADVANCED );
		}

		void PubChemDialog::checkMenu ( MainControl& main_control )
		{
			bool busy = main_control.compositesAreLocked();

			if ( action1_ )
				action1_->setEnabled ( !busy );

			if ( action2_ )
				action2_->setEnabled ( !busy );
		}

		void PubChemDialog::updateDownloadProgress ( qint64 done, qint64 total )
		{
			ui_->progress_bar->setMaximum ( std::max ( total, ( qint64 ) 0 ) );
			ui_->progress_bar->setValue ( done );
		}

	}
}
