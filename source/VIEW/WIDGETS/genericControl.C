// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericControl.C,v 1.17.16.1 2007/03/25 21:56:47 oliver Exp $
//

#include <BALL/VIEW/WIDGETS/genericControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

namespace BALL
{
	namespace VIEW
	{

		TreeWidget::TreeWidget(QWidget* parent)
			: QTreeWidget(parent)
		{
		}

		void TreeWidget::selectItems(const list<QTreeWidgetItem*>& items)
		{
			QItemSelection qis;
			list<QTreeWidgetItem*>::const_iterator cit = items.begin();
			for (; cit != items.end(); cit++)
			{
				QItemSelectionRange qsr(indexFromItem(*cit));
				qis.push_back(qsr);
			}

	    selectionModel()->select(qis, QItemSelectionModel::Select
					                            |QItemSelectionModel::Rows);
		}

		GenericControl::GenericControl(QWidget* parent, const char* name)
				:	DockWidget(parent, name),
 					context_item_(0),
					listview(new TreeWidget(this)),
					checkable_(false)
		{
 			setGuest(*listview);	
			listview->setSelectionMode(QAbstractItemView::ExtendedSelection);
			connect(listview, SIGNAL(itemSelectionChanged()), this, SLOT(updateSelection()));
			connect(listview, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
		}


		GenericControl::~GenericControl()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class GenericControl" << endl;
			#endif 
		}


		GenericControl::ItemList GenericControl::getSelectedItems()
		{
			QList<QTreeWidgetItem*> result = listview->selectedItems();
			if (result.size() > 0)
			{
				context_item_ = *result.begin();
			}
			else
			{
				context_item_ = 0;
			}

			return result;
		}

		void GenericControl::deselectOtherControls_()
		{
			notify_(new DeselectControlsMessage);
		}

		void GenericControl::updateSelection() 
		{
			if (getSelectedItems().size() > 0) deselectOtherControls_();
		}

		void GenericControl::onNotify(Message *message)
		{
			if (!RTTI::isKindOf<DeselectControlsMessage>(*message)) return;

			listview->clearSelection();
		}

		void GenericControl::initializeWidget(MainControl& main_control)
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
		{
			while (item->child(0) != 0)
			{
				removeItem_(item->child(0));
			}

			delete item;
		}

	} // namespace VIEW
} // namespace BALL
