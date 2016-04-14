#ifndef BALL_VIEW_WIDGETS_SHORTCUTTABLEVIEW_H
#define BALL_VIEW_WIDGETS_SHORTCUTTABLEVIEW_H

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_DIALOGS_EDITSINGLESHORTCUT_H
# include <BALL/VIEW/DIALOGS/editSingleShortcut.h>
#endif

#include <QtWidgets/QTableView>
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QPushButton>

class QSortFilterProxyModel;

namespace BALL
{
	namespace VIEW
	{
		class BALL_VIEW_EXPORT ShortcutTableView : public QTableView
		{
			Q_OBJECT

			public:
				ShortcutTableView(QWidget* parent);
				virtual ~ShortcutTableView();

				void setFilter(const QString& filter);

			signals:
				void shortcutChanged();

			protected slots:
				void editSuccess_();

			protected slots:
				void onClick(const QModelIndex& index);

			private:
				QSortFilterProxyModel* proxy_model_;
				EditSingleShortcut* editor_;
				int edited_row_;
		};

		class ShortcutTableModel : public QAbstractTableModel
		{
			Q_OBJECT

			public:
				ShortcutTableModel(ShortcutRegistry* reg);
				int rowCount(const QModelIndex& parent = QModelIndex()) const;
				int columnCount(const QModelIndex& parent = QModelIndex()) const;

				QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
				QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
				Qt::ItemFlags flags(const QModelIndex& index) const;
				bool isValid(const QKeySequence& seq) const;
				bool setData(const QModelIndex& index, const QVariant& data, int role = Qt::EditRole);
			private:
				ShortcutRegistry* registry_;
		};

	}
}

#endif //BALL_VIEW_WIDGETS_SHORTCUTTABLEVIEW_H

