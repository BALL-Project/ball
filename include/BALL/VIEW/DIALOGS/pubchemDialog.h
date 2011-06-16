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

#ifndef BALL_VIEW_WIDGETS_SDWIDGET_H
# include <BALL/VIEW/WIDGETS/SDWidget.h>
#endif

#ifndef BALL_STRUCTURE_SMILESPARSER_H
# include <BALL/STRUCTURE/smilesParser.h>
#endif

#ifndef BALL_FORMAT_PUBCHEMDOWNLOADER_H
# include <BALL/FORMAT/pubchemDownloader.h>
#endif

#include <BALL/VIEW/UIC/ui_pubchemDialog.h>

#include <QtGui/QDialog>

#include <map>

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

					///
					virtual void initializeWidget(MainControl& main_control);

					///
					virtual void checkMenu(MainControl& main_control);

					///
					void generateFromSMILES(const String& SMILES);
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

				protected:

					SmilesParser      smiles_parser_;

					struct ParsedResult_
					{
						String name;
						String description;
						String smiles;
					};

					void parseItemRecursive_(const QDomNode& current_node, Position level, ParsedResult_& result);
					 
					void insert_(ParsedResult_ d, QTreeWidgetItem* parent, bool plot);
					
					std::map<QTreeWidgetItem*, System*> 				sd_systems_;
					std::map<QTreeWidgetItem*, System*> 				original_systems_;
					std::map<QTreeWidgetItem*, ParsedResult_>  	descriptions_;

					QAction* action1_, *action2_;

					int current_request_id_;

					HashMap<int, QTreeWidgetItem*> esummary_request_ids_;
					QProgressBar *progress_;

					QPushButton* add_button_;
			};
	}
}
#endif
