#ifndef BALL_VIEW_WIDGETS_SHORTCUTTABLEVIEW_H
#define BALL_VIEW_WIDGETS_SHORTCUTTABLEVIEW_H

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_DIALOGS_EDITSINGLESHORTCUT_H
# include <BALL/VIEW/DIALOGS/editSingleShortcut.h>
#endif

#include <QtGui/QTableView>
#include <QtGui/QItemDelegate>
#include <QtGui/QPushButton>

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

				void setFilter(const QString& filter);

			protected slots:
				void editSuccess_();

			protected slots:
				void onDoubleClick(const QModelIndex& index);

			private:
				QSortFilterProxyModel* proxy_model_;
				EditSingleShortcut* editor_;
				int edited_row_;
		};
	}
}

#endif //BALL_VIEW_WIDGETS_SHORTCUTTABLEVIEW_H

