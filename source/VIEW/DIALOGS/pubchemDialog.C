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
#include <QtGui/QMessageBox>

#include <QtCore/QUrl>

#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <algorithm>

namespace BALL
{
	namespace VIEW
	{
		PubChemDialog::PubChemDialog ( QWidget* parent, const char* name )
			: QDialog ( parent ),
			  Ui_PubChemDialogData(),
			  ModularWidget ( name ),
			  action1_ ( 0 ),
			  action2_ ( 0 ),
			  network_manager_(0)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new PubChemDialog " << this << std::endl;
#endif

			setupUi ( this );
			setObjectName ( name );

			progress_bar->hide();

			// register the widget with the MainControl
			ModularWidget::registerWidget ( this );

			add_button_ = buttonBox->addButton ( tr ( "Add" ), QDialogButtonBox::AcceptRole );
			QPushButton* clear = buttonBox->addButton ( tr ( "Clear Results" ), QDialogButtonBox::ResetRole );

			connect ( generate_button, SIGNAL ( clicked() ), this, SLOT ( generateButtonClicked() ) );
			connect ( search_button, SIGNAL ( clicked() ), this, SLOT ( queryPubChem() ) );
			connect ( clear, SIGNAL ( clicked() ), this, SLOT ( clearEntries() ) );
			connect ( queries, SIGNAL ( itemActivated ( QTreeWidgetItem*, int ) ), this, SLOT ( switchView ( QTreeWidgetItem*, int ) ) );

			queries->setColumnCount ( 1 );
			queries->headerItem()->setText ( 0, tr ( "Results" ) );
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
			if ( sd_systems_.find ( item ) == sd_systems_.end() ) return;

			hide();

			ParsedResult_& res = descriptions_.find ( item )->second;

			System* org_system = sd_systems_.find ( item )->second;
			System* new_system = new System ( *org_system );

			new_system->setName ( res.name );
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
			queries->clear();
			sdwidget_->clear();
			text_field->clear();
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
			pr.description = ( String ) tr ( "Generated from SMILES " ) +SMILES;
			pr.name = SMILES;
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
			generateFromSMILES ( String ( ascii ( smiles_label->displayText() ) ) );
		}

		void PubChemDialog::switchView ( QTreeWidgetItem* item, int /*column*/ )
		{
			// is this item connected with a system?
			if ( sd_systems_.find ( item ) != sd_systems_.end() )
			{
				sdwidget_->plot ( *sd_systems_[item], false );
			}

			// do we have a description for it?
			if ( descriptions_.find ( item ) != descriptions_.end() )
			{
				text_field->setText ( QString ( descriptions_[item].description.c_str() ) );
			}

			add_button_->setEnabled ( true );
		}

		void PubChemDialog::queryPubChem()
		{
			QString qt = pubchem_label->displayText();

			if ( qt == "" ) return;
			QString pug_name_query_("http://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/%1/SDF");

			if(!network_manager_) {
				network_manager_ = new QNetworkAccessManager(this);

				connect(network_manager_, SIGNAL(finished(QNetworkReply*)),
				        this, SLOT(downloadFinished(QNetworkReply*)));
			}

			getMainControl()->setStatusbarText ( String ( tr ( "Performing query..." ) ) );

			QNetworkReply* reply  = network_manager_->get(QNetworkRequest(QUrl(pug_name_query_.arg(qt))));

			connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
			        this, SLOT(updateDownloadProgress(qint64,qint64)));
	
			progress_bar->setMaximum(0);
			progress_bar->show();

			search_button->setEnabled ( false );
		}

		bool PubChemDialog::downloadError_(QNetworkReply* reply)
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

			if(!downloadError_(reply))
			{
				search_button->setEnabled ( true );
				progress_bar->hide();

				return;
			}

			SDFile sdf;

			String fname;
			File::createTemporaryFilename(fname, ".sdf");

			// Write the received data into a buffer
			QFile outfile(fname.c_str());

			if(!outfile.open(QIODevice::WriteOnly)) {
				QMessageBox::critical ( this, tr ( "Download failed" ),
				                        tr ( "Could not open download destination (%1) for query %2").arg(fname.c_str()).arg(pubchem_label->displayText()));
				search_button->setEnabled ( true );
				progress_bar->hide();

				return;
			}
			outfile.write(reply->readAll());
			outfile.close();

			// now, try to read the SD File
			sdf.open(fname, std::ios::in);

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
						String qt = ascii ( pubchem_label->displayText() );
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
			File::remove (fname);

			QTreeWidgetItem* new_query_result = new QTreeWidgetItem ( ( QTreeWidget* ) 0,
			        QStringList ( pubchem_label->displayText()
			                      +" ("+QString::number ( sd_systems.size() ) + ")" ) );
			queries->insertTopLevelItem ( 0, new_query_result );
			queries->expandItem ( new_query_result );

			QTreeWidgetItem* first_item = NULL;

			for ( std::deque<System*>::iterator sys_it  = sd_systems.begin();
			        sys_it != sd_systems.end(); ++sys_it )
			{
				ParsedResult_ pr;
				pr.name = ( *sys_it )->beginMolecule()->getName();

				NamedPropertyIterator npi = ( *sys_it )->beginMolecule()->beginNamedProperty();

				for ( ; npi != ( *sys_it )->beginMolecule()->endNamedProperty(); ++npi )
				{
					String prop_name = npi->getName();

					if ( prop_name.hasPrefix ( "PUBCHEM_" ) )
					{
						pr.description += String ( "<b>" ) +prop_name.after ( "PUBCHEM_" ) +"</b><br/>";
						pr.description += npi->toString() +"<br/>";
					}
				}

				QTreeWidgetItem* current_item = new QTreeWidgetItem ( new_query_result, QStringList ( QString ( pr.name.c_str() ) ) );
				descriptions_[current_item] = pr;

				sdwidget_->plot ( **sys_it, false );
				sd_systems_[current_item] = *sys_it;

				queries->setCurrentItem ( current_item );
				current_item->setSelected ( true );
				switchView ( current_item, 0 );
				queries->setCurrentItem ( current_item );
				current_item->setSelected ( true );

				if ( !first_item )
				{
					first_item = current_item;
				}

				queries->update();
			}

			if ( first_item )
			{
				first_item->setSelected ( true );
				queries->setCurrentItem ( first_item );
				first_item->setSelected ( true );
				switchView ( first_item, 0 );
			}

			search_button->setEnabled ( true );
			progress_bar->hide();
		}

		void PubChemDialog::insert_ ( ParsedResult_ pr, QTreeWidgetItem* parent, bool plot )
		{
			QTreeWidgetItem* current_item = new QTreeWidgetItem ( parent, QStringList ( pr.name.c_str() ) );
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
				sdwidget_->plot ( S, false );
				switchView ( current_item, 0 );
			}

			sd_systems_[current_item] = new System ( S );

			if ( parent == 0 )
			{
				QList<QTreeWidgetItem*> structure;
				structure.append ( current_item );
				queries->insertTopLevelItems ( 0, structure );
			}

			queries->setCurrentItem ( current_item );
			current_item->setSelected ( true );
			queries->update();
		}


		void PubChemDialog::initializeWidget ( MainControl& )
		{
			action1_ = insertMenuEntry(MainControl::FILE_OPEN, (String)tr("PubChem"), this, SLOT(show()), "Shortcut|File|Open|PubChem");
			setMenuHint((String)tr("Download a file from pubchem"));
			setIcon("actions/download-pubchem", true);
			
			action2_ = insertMenuEntry(MainControl::BUILD, (String)tr("Build from SMILES"), this, SLOT(show()), "Shortcut|Build|Build_from_SMILES");
			setMenuHint((String)tr("Create a structure from a SMILES expression"));
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
			progress_bar->setMaximum ( std::max ( total, ( qint64 ) 0 ) );
			progress_bar->setValue ( done );
		}

	}
}
