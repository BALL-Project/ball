// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/editOperationDialog.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <QtGui/QToolTip>
#include <QtGui/QButtonGroup>
#include <QtGui/QPushButton>
#include <QtGui/QListBox>

namespace BALL
{
	namespace VIEW
	{
		EditOperationDialog::EditOperationDialog(QWidget* parent, const char* name, Qt::WFlags fl)
			: QWidget(parent, name, fl),
				Ui_EditOperationDialogData(),
				PreferencesEntry(),
				ModularWidget(name)
		{	
			setupUi(this);

			Scene* scene = Scene::getInstance(0);

			if (scene == 0)
			{
				Log.error() << "Expected an Scene, but found none!" << std::endl;
			}
			else
			{ 
				connect(scene, SIGNAL(newEditOperation(Scene::EditOperation&)),
								this,    SLOT(addEditOperation(Scene::EditOperation&)));
			}
			undo_operation_list->setSelectionMode(Q3ListBox::Extended);	
		}


		EditOperationDialog::~EditOperationDialog()
		{
		}

			// ------------------------- SLOTS ------------------------------------------------
			// --------------------------------------------------------------------------------


		void EditOperationDialog::undo(int number_of_undo_steps)
		{
			// TODO: Implementation :-)
		}

		void EditOperationDialog::operationSelected(int operation)
		{
			int number_of_items = undo_operation_list->count(); 
			for(Position i = number_of_items-1; i > (Position) operation; i--)
			{	
				Log.info() << "Hallo: " << i<<std::endl;
				undo_operation_list->setSelected(i, true);
			}
		}

		void EditOperationDialog::addEditOperation(Scene::EditOperation& eo)
		{
			undo_operation_list->insertItem(eo.description.c_str());
		}


	}
}
