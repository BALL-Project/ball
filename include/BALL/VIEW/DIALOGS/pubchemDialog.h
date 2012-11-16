// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGSPUBCHEMDIALOG_H
#define BALL_VIEW_DIALOGSPUBCHEMDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_STRUCTURE_SMILESPARSER_H
# include <BALL/STRUCTURE/smilesParser.h>
#endif

#include <QtGui/QDialog>

#include <map>
#include <boost/unordered_map.hpp>

class QNetworkAccessManager;
class QNetworkReply;

class QTreeWidgetItem;

namespace Ui
{
	class PubChemDialogData;
}

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for querying PubChem and generating structures from a SMILES string
		 */
		class BALL_VIEW_EXPORT PubChemDialog
			: public QDialog,
			  public ModularWidget
		{
			Q_OBJECT

			public:

				BALL_EMBEDDABLE ( PubChemDialog,ModularWidget )

				/// Default Constructor.
				PubChemDialog ( QWidget* parent = NULL, const char* name = "PubChemDialog" );

				/// Destructor.
				virtual ~PubChemDialog();

				///
				virtual void initializeWidget ( MainControl& main_control );

				///
				virtual void checkMenu ( MainControl& main_control );

				///
				void generateFromSMILES ( const String& SMILES );
			public slots:

				/// Show and raise dialog
				void show();

				/// Generate a structure from SMILES
				void generateButtonClicked();

				/// Query PubChem
				void queryPubChem();

				///
				void switchView ( QTreeWidgetItem* item, int column );

				///
				void clearEntries();

				///
				void finished();

				///
				void updateDownloadProgress ( qint64 done, qint64 total );

				///
				void downloadFinished ( QNetworkReply* reply );

			protected:
				Ui::PubChemDialogData* ui_;

				SmilesParser smiles_parser_;

				struct ParsedResult_
				{
					QString name;
					QString description;
					String smiles;
				};

				enum InfoDisplayStyle{
					LONG, SHORT
				};

				void insert_ ( ParsedResult_ d, QTreeWidgetItem* parent, bool plot );
				bool handleDownloadError_ ( QNetworkReply* reply);
				void buildInformationTemplates_();
				QString buildHeaderTemplate_(InfoDisplayStyle style, const char* str) const;

				std::map<QTreeWidgetItem*, System*> sd_systems_;
				std::map<QTreeWidgetItem*, System*> original_systems_;
				std::map<QTreeWidgetItem*, ParsedResult_> descriptions_;

				QAction* action1_, *action2_;

				HashMap<int, QTreeWidgetItem*> esummary_request_ids_;
				boost::unordered_map<String, QString> information_templates_;

				QPushButton* add_button_;
				QNetworkAccessManager* network_manager_;
		};

	}
}
#endif
