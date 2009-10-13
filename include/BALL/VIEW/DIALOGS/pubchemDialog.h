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

#ifndef BALL_VIEW_WIDGETS_SDWIDGET_H
# include <BALL/VIEW/WIDGETS/SDWidget.h>
#endif

#include <BALL/VIEW/UIC/ui_pubchemDialog.h>

#include <QtGui/QDialog>
#include <QtNetwork/QHttp>

#include <map>

class QDomNode;
class QProgressBar;

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for querying PubChem and generating structures from a SMILES string
			 */
			class BALL_VIEW_EXPORT PubChemDialog :
				public QDialog,
				public Ui_PubChemDialogData,
				public ModularWidget
			{
				Q_OBJECT

				public:

					BALL_EMBEDDABLE(PubChemDialog,ModularWidget)

					/// Default Constructor.
					PubChemDialog(QWidget *parent = NULL, const char *name = "PubChemDialog");

					/// Destructor.
					virtual ~PubChemDialog();

					/** Generate a new molecule from a SMILES string. The molecule is displayed in the SDWidget
							widget but not yet inserted into BALLView.
					*/
					void generateFromSMILES(const String& SMILES);

					///
					virtual void initializeWidget(MainControl& main_control);

					///
					virtual void checkMenu(MainControl& main_control);

					/// Calls the esummary web service from entrez to download the molecule with given pcsubstance id
					void callESummary(QString const& entry_id, QTreeWidgetItem* current_item = NULL);

				public slots:
					
					/// Show and raise dialog
					void show();

					/// Generate a structure from SMILES
					void generateButtonClicked();

					/// Query PubChem
					void queryPubChem();

					///
					void switchView(QTreeWidgetItem* item, int column);

					///
					void clearEntries();

					///
					void finished();

					/// An entrez esearch has finished
					void esearchFinished(int id, bool error);

					/// A pubchem download has finished
					void esummaryFinished(int id, bool error);

				protected:

					struct ParsedResult_
					{
						String name;
						String description;
						String smiles;
					};

					void parseItemRecursive_(const QDomNode& current_node, Position level, ParsedResult_& result);
					 
					void insert_(ParsedResult_ d, QTreeWidgetItem* parent, bool plot);
					
					bool parseESummaryXml_(const QByteArray& data, ParsedResult_& result);

					SDWidget sdwidget_;

					std::map<QTreeWidgetItem*, System*> 				sd_systems_;
					std::map<QTreeWidgetItem*, System*> 				original_systems_;
					std::map<QTreeWidgetItem*, ParsedResult_>  	descriptions_;

					SmilesParser smiles_parser_;

					QAction* action1_, *action2_;

					String esearch_base_url_;
					String esummary_base_url_;

					int current_request_id_;

					HashMap<int, QTreeWidgetItem*> esummary_request_ids_;

					QHttp esearch_connector_;
					QHttp esummary_connector_;

					QProgressBar *progress_;
			};
	}
}
#endif
