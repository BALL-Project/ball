// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALL_VIEW_DIALOGSPUBCHEMDIALOG_H
#define BALL_VIEW_DIALOGSPUBCHEMDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_STRUCTURE_SMILESPARSER_H
# include <BALL/STRUCTURE/smilesParser.h>
#endif

#ifndef BALL_VIEW_WIDGETS_SDWIDGET_H
# include <BALL/VIEW/WIDGETS/SDWidget.h>
#endif

#include <BALL/VIEW/UIC/pubchemDialogData.h>
#include <QtGui/qdialog.h>
#include <map>

class ns6__ItemType;
class ns6__DocSumType;

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
					PubChemDialog(QWidget *parent = NULL, const char *name = "PubChemDialog")
						throw();

					/// Destructor.
					virtual ~PubChemDialog() throw();

					/** Generate a new molecule from a SMILES string. The molecule is displayed in the SDWidget
							widget but not yet inserted into BALLView.
					*/
					void generateFromSMILES(const String& SMILES) throw();

					///
					virtual void initializeWidget(MainControl& main_control);

					///
					virtual void checkMenu(MainControl& main_control)
						throw();

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

					struct ParsedResult_
					{
						String name;
						String description;
						String smiles;
					};

					String				parseItemRecursive_(ns6__ItemType* item, int level=0);
					ParsedResult_ parseResultRecursive_(ns6__DocSumType* result);
					void insert_(ParsedResult_ d, QTreeWidgetItem* parent, bool plot);
					
					SDWidget sdwidget_;

					std::map<QTreeWidgetItem*, System*> 				sd_systems_;
					std::map<QTreeWidgetItem*, System*> 				original_systems_;
					std::map<QTreeWidgetItem*, ParsedResult_>  	descriptions_;

					SmilesParser smiles_parser_;

					QAction* action1_, *action2_;
			};
	}
}
#endif
