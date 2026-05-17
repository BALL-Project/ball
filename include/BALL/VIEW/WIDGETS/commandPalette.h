// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.46 (Handover §6.3) — CommandPalette: Cmd/Ctrl+K
// floating sheet that fuzzy-matches commands from CommandRegistry.
//
// Whole class is BALL_UI_V2-only — gating the header (not just
// the impl) keeps AUTOMOC from generating moc symbols in the OFF
// cell that would dangling-link against an empty TU.

#ifndef BALL_VIEW_WIDGETS_COMMANDPALETTE_H
#define BALL_VIEW_WIDGETS_COMMANDPALETTE_H

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#ifdef BALL_UI_V2

#include <BALL/VIEW/KERNEL/commandRegistry.h>

#include <QtCore/QAbstractListModel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QStyledItemDelegate>

class QLineEdit;
class QListView;
class QSortFilterProxyModel;

namespace BALL
{
	namespace VIEW
	{
		/** Phase 999.46 §6.3 internal model — adapter from
		 *  CommandRegistry to QListView. Re-runs search() each
		 *  time setQuery() is called and stores the result list
		 *  as the row source.
		 */
		class CommandPaletteModel : public QAbstractListModel
		{
			Q_OBJECT

			public:
				enum Roles
				{
					IdRole = Qt::UserRole + 1,
					CategoryRole,
					DescriptionRole,
					ShortcutRole,
					IconRole
				};

				explicit CommandPaletteModel(QObject* parent = nullptr);
				~CommandPaletteModel() override;

				int rowCount(const QModelIndex& parent = QModelIndex()) const override;
				QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

				/** Re-run the search; emit modelReset. */
				void setQuery(const QString& query);

				/** Return the Command at row, or empty Command if out of range. */
				Command commandAt(int row) const;

				/** Last-set query (for the delegate's `?` mode rendering). */
				const QString& currentQuery() const { return query_; }

			private:
				QString query_;
				QList<Command> rows_;
		};

		/** Phase 999.46 §6.3 delegate — renders title (bold) +
		 *  category chip + shortcut + icon per row. In `?` mode
		 *  (query starts with `?`) the description text is rendered
		 *  prominently below the title.
		 */
		class CommandPaletteDelegate : public QStyledItemDelegate
		{
			Q_OBJECT

			public:
				explicit CommandPaletteDelegate(QObject* parent = nullptr);
				~CommandPaletteDelegate() override;

				QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
				void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

				/** When set, the delegate switches to "help row" rendering. */
				void setHelpMode(bool on) { help_mode_ = on; }

			private:
				bool help_mode_ = false;
		};

		/** Phase 999.46 §6.3 — floating Cmd/Ctrl+K command palette.
		 *
		 *  Usage: construct once parented to the main window, then
		 *  call open() (typically wired to a QShortcut on Ctrl+K).
		 *  open() centers the palette on the parent, clears the
		 *  query, focuses the line edit, and shows.
		 *
		 *  Modes (§6.4):
		 *   - `>` prefix: file actions only.
		 *   - `?` prefix: help-row rendering.
		 *   - `:` prefix: navigation / inspector / workspace jumps.
		 *   - (plain text): fuzzy search all commands.
		 */
		class BALL_VIEW_EXPORT CommandPalette : public QWidget
		{
			Q_OBJECT

			public:
				explicit CommandPalette(QWidget* parent = nullptr);
				~CommandPalette() override;

			public Q_SLOTS:
				/** Show + center + clear + focus query line. */
				void open();

				/** Hide. */
				void close();

			protected:
				void keyPressEvent(QKeyEvent* e) override;
				void showEvent(QShowEvent* e) override;
				bool eventFilter(QObject* watched, QEvent* event) override;

			private Q_SLOTS:
				void onQueryChanged(const QString& text);
				void onActivated(const QModelIndex& index);

			private:
				void triggerCurrent_();
				void centerOnParent_();

				QLineEdit* line_ = nullptr;
				QListView* list_ = nullptr;
				CommandPaletteModel* model_ = nullptr;
				CommandPaletteDelegate* delegate_ = nullptr;
		};
	}
}

#endif // BALL_UI_V2

#endif // BALL_VIEW_WIDGETS_COMMANDPALETTE_H
