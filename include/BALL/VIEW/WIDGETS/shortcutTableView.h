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

namespace BALL
{
	namespace VIEW
	{
		class BALL_VIEW_EXPORT ShortcutTableView : public QTableView
		{
			Q_OBJECT

			public:
				ShortcutTableView(QWidget* parent);
		};

		class ShortcutInserter : public  EditSingleShortcut
		{
			Q_OBJECT
			public:
				ShortcutInserter(QWidget* parent);
				virtual ~ShortcutInserter();

				QKeySequence const& getKeySequence() const { return new_sequence_; }

			protected:
				unsigned int modifiers_;
				int key_;
				QKeySequence new_sequence_;
				
				void updateText_();

				void keyPressEvent(QKeyEvent* evt);
				void keyReleaseEvent(QKeyEvent* evt);
		};

		// TODO MODEL

		class ShortcutTableDelegate : public QItemDelegate
		{
			Q_OBJECT

			public:
				QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option,
														 const QModelIndex& index) const;

				void setEditorData(QWidget* editor, const QModelIndex& index) const;
				void setModelData(QWidget* editor, QAbstractItemModel* model,
												 const QModelIndex& index) const;

		//		QSize sizeHint (const QStyleOptionViewItem& option, const QModelIndex & index ) const;
		//		void paint (QPainter* painter, const QStyleOptionViewItem& option, 
		//																			 const QModelIndex& index ) const;  

				void updateEditorGeometry(QWidget* editor,
				                          const QStyleOptionViewItem& option,
				                          const QModelIndex& index) const;
		};

	}
}

#endif //BALL_VIEW_WIDGETS_SHORTCUTTABLEVIEW_H
