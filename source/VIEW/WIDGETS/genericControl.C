// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericControl.C,v 1.17.2.1 2006/01/13 15:36:05 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/genericControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

namespace BALL
{
	namespace VIEW
	{

		GenericControl::GenericControl(QWidget* parent, const char* name)
			throw()
				:	DockWidget(parent, name),
 					context_item_(0),
					listview(new QTreeWidget(this))
		{
			// appearance
//   			listview->setSorting(-1);
 			setGuest(*listview);	
			listview->setSelectionMode(QAbstractItemView::ExtendedSelection);
			connect(listview, SIGNAL(itemSelectionChanged()), this, SLOT(updateSelection()));
			connect(listview, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onItemClicked(QTreeWidgetItem*)));
		}


		GenericControl::~GenericControl()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class GenericControl" << endl;
			#endif 
		}


		GenericControl::ItemList GenericControl::getSelectedItems()
			throw()
		{
			QList<QTreeWidgetItem*> result;
			result = listview->selectedItems();
			if (result.size() > 0)
			{
				context_item_ = *result.begin();
			}
			return result;
		}

		void GenericControl::deselectOtherControls_()
		{
			notify_(new DeselectControlsMessage);
		}

		void GenericControl::updateSelection() 
		{
			if (getSelectedItems().size() != 0) deselectOtherControls_();
		}

		void GenericControl::onNotify(Message *message)
			throw()
		{
			if (!RTTI::isKindOf<DeselectControlsMessage>(*message))
			{
				return;
			}

			listview->setCurrentItem(0);
//   			listview->selectAll(false);
		}

		void GenericControl::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.insertDeleteEntry();
			DockWidget::initializeWidget(main_control);
		} 

		QTreeWidgetItem* GenericControl::addRow(const QStringList& entries)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(listview, entries);

			if (checkable_) item->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
			return item;
		}

		void GenericControl::removeItem_(QTreeWidgetItem* item)
			throw()
		{
			while (item->child(0) != 0)
			{
				removeItem_(item->child(0));
			}

			delete item;
		}


	} // namespace VIEW
} // namespace BALL
