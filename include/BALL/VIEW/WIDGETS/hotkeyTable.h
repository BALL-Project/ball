#ifndef BALL_VIEW_WIDGETS_HOTKEY_TABLE_H
#define BALL_VIEW_WIDGETS_HOTKEY_TABLE_H

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_WIDGETS_PYWIDGET_H
# include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif

#include <QtGui/QTableWidget>
#include <QtGui/QItemDelegate>

namespace BALL
{
	namespace VIEW
	{
		class ComboBoxDelegate
			: public QItemDelegate
		{
			public:

				ComboBoxDelegate(QObject* parent = 0);

				QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
										          const QModelIndex &index) const;

				void setEditorData(QWidget *editor, const QModelIndex &index) const;
				void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

				void updateEditorGeometry(QWidget *editor,
																	const QStyleOptionViewItem &option, const QModelIndex &index) const;
			private:

				QStringList sl_keys_;
				QStringList sl_modifier_;
		};

		/** Class for storing Python hotkeys in a GUI table
				\ingroup ViewDialogs
		*/
		class HotkeyTable
			:	public QTableWidget,
				public PreferencesEntry::ExtendedPreferencesObject
		{
			Q_OBJECT

			public:

				///
				enum Modifier
				{
					NONE = 0,
					SHIFT
					// ALT // currently not working!
				};

				///
				HotkeyTable(QWidget* parent = 0, const char* name = "PythonSettings");

				///
				virtual bool getValue(String& value) const;

				///
				virtual bool setValue(const String& value);

				///
				std::list<Hotkey> getContent() const;

				///
				void setContent(const std::list<Hotkey>& hotkeys);

			public slots:

				///
				virtual void addEmptyRow();

				///
				virtual void removeSelection();

				/** Append a hotkey
						F_key: 1-12 for the 12 F-keys
				*/
				virtual void appendHotkey(const String& modif, const String& F_key,
																	const String& command, String comment = "");

			private:
				QStringList modifier_, keys_;
				ComboBoxDelegate delegate_;
		};
	}
}

#endif //BALL_VIEW_WIDGETS_HOTKEY_TABLE_H
