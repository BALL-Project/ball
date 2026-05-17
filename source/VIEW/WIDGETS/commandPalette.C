// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.46 (Handover §6.3) — CommandPalette impl.

#include <BALL/VIEW/WIDGETS/commandPalette.h>

#ifdef BALL_UI_V2

#include <QtCore/QEvent>
#include <QtCore/QSize>
#include <QtCore/QStringList>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QtGui/QShowEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QStyle>
#include <QtWidgets/QVBoxLayout>

namespace BALL
{
	namespace VIEW
	{
		// --- CommandPaletteModel ---------------------------------------

		CommandPaletteModel::CommandPaletteModel(QObject* parent)
			: QAbstractListModel(parent)
		{
		}

		CommandPaletteModel::~CommandPaletteModel() = default;

		int CommandPaletteModel::rowCount(const QModelIndex& parent) const
		{
			if (parent.isValid()) return 0;
			return rows_.size();
		}

		QVariant CommandPaletteModel::data(const QModelIndex& index, int role) const
		{
			if (!index.isValid() || index.row() < 0 || index.row() >= rows_.size())
				return QVariant();

			const Command& c = rows_.at(index.row());
			switch (role)
			{
				case Qt::DisplayRole:
				case Qt::EditRole:
					return c.title;
				case Qt::DecorationRole:
					return c.icon;
				case Qt::ToolTipRole:
					return c.description;
				case IdRole:          return c.id;
				case CategoryRole:    return c.category;
				case DescriptionRole: return c.description;
				case ShortcutRole:    return c.shortcut.toString(QKeySequence::NativeText);
				case IconRole:        return c.icon;
				default:              return QVariant();
			}
		}

		void CommandPaletteModel::setQuery(const QString& query)
		{
			beginResetModel();
			query_ = query;

			// Phase 999.46 §6.4 — prefix modes. The CommandPalette
			// strips the prefix before passing to setQuery and
			// also flips the delegate's help-mode; this model layer
			// only needs to filter `>` (File-only) since fuzzy
			// search itself handles plain + ? + : as "search all".
			QString effective = query;
			bool file_only = false;
			if (effective.startsWith(QLatin1Char('>')))
			{
				effective.remove(0, 1);
				file_only = true;
			}
			else if (effective.startsWith(QLatin1Char('?')) ||
			         effective.startsWith(QLatin1Char(':')))
			{
				effective.remove(0, 1);
			}

			QList<Command> hits = CommandRegistry::instance().search(effective.trimmed());

			if (file_only)
			{
				QList<Command> filtered;
				filtered.reserve(hits.size());
				for (const Command& c : hits)
				{
					if (c.category.compare(QStringLiteral("File"), Qt::CaseInsensitive) == 0)
						filtered.append(c);
				}
				rows_ = filtered;
			}
			else if (query.startsWith(QLatin1Char(':')))
			{
				QList<Command> filtered;
				filtered.reserve(hits.size());
				for (const Command& c : hits)
				{
					if (c.id.startsWith(QStringLiteral("view.workspace"))   ||
					    c.id.startsWith(QStringLiteral("view.inspector"))   ||
					    c.id.startsWith(QStringLiteral("file.recent"))      ||
					    c.id.startsWith(QStringLiteral("window."))          ||
					    c.id.startsWith(QStringLiteral("view.window.")))
						filtered.append(c);
				}
				rows_ = filtered;
			}
			else
			{
				rows_ = hits;
			}

			endResetModel();
		}

		Command CommandPaletteModel::commandAt(int row) const
		{
			if (row < 0 || row >= rows_.size()) return Command{};
			return rows_.at(row);
		}

		// --- CommandPaletteDelegate ------------------------------------

		CommandPaletteDelegate::CommandPaletteDelegate(QObject* parent)
			: QStyledItemDelegate(parent)
		{
		}

		CommandPaletteDelegate::~CommandPaletteDelegate() = default;

		QSize CommandPaletteDelegate::sizeHint(const QStyleOptionViewItem& option,
		                                      const QModelIndex& /*index*/) const
		{
			const int h = help_mode_ ? 56 : 36;
			return QSize(option.rect.width(), h);
		}

		void CommandPaletteDelegate::paint(QPainter* painter,
		                                  const QStyleOptionViewItem& option,
		                                  const QModelIndex& index) const
		{
			painter->save();

			if (option.state & QStyle::State_Selected)
				painter->fillRect(option.rect, option.palette.highlight());

			const int pad_x = 12;
			const int icon_size = 18;
			QRect r = option.rect.adjusted(pad_x, 0, -pad_x, 0);

			// Icon (left).
			QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
			QRect icon_r(r.left(), r.center().y() - icon_size / 2, icon_size, icon_size);
			if (!icon.isNull())
				icon.paint(painter, icon_r);

			const int text_x = icon_r.right() + 10;

			// Shortcut (right).
			QString shortcut = index.data(CommandPaletteModel::ShortcutRole).toString();
			int shortcut_w = 0;
			if (!shortcut.isEmpty())
			{
				QFont mono = option.font;
				mono.setStyleHint(QFont::Monospace);
				QFontMetrics fm(mono);
				shortcut_w = fm.horizontalAdvance(shortcut) + 8;
				QRect sc_r(r.right() - shortcut_w, r.top(), shortcut_w, r.height());
				painter->save();
				painter->setFont(mono);
				painter->setPen((option.state & QStyle::State_Selected)
				                ? option.palette.highlightedText().color()
				                : option.palette.windowText().color().lighter(150));
				painter->drawText(sc_r, Qt::AlignRight | Qt::AlignVCenter, shortcut);
				painter->restore();
			}

			// Category chip (right-of-title, before shortcut).
			QString category = index.data(CommandPaletteModel::CategoryRole).toString();
			int chip_w = 0;
			if (!category.isEmpty())
			{
				QFont cat_font = option.font;
				cat_font.setPointSizeF(option.font.pointSizeF() * 0.85);
				QFontMetrics fm(cat_font);
				chip_w = fm.horizontalAdvance(category) + 12;
				QRect chip_r(r.right() - shortcut_w - chip_w, r.center().y() - 9, chip_w, 18);
				painter->save();
				painter->setFont(cat_font);
				painter->setPen((option.state & QStyle::State_Selected)
				                ? option.palette.highlightedText().color()
				                : option.palette.windowText().color().lighter(160));
				painter->drawText(chip_r, Qt::AlignCenter, category);
				painter->restore();
			}

			// Title (bold, left).
			QString title = index.data(Qt::DisplayRole).toString();
			QFont title_font = option.font;
			title_font.setBold(true);
			QRect title_r(text_x, r.top(),
			              r.width() - text_x - chip_w - shortcut_w - 12,
			              help_mode_ ? r.height() / 2 : r.height());
			painter->save();
			painter->setFont(title_font);
			painter->setPen((option.state & QStyle::State_Selected)
			                ? option.palette.highlightedText().color()
			                : option.palette.windowText().color());
			painter->drawText(title_r, Qt::AlignLeft | Qt::AlignVCenter, title);
			painter->restore();

			// Description (help mode only).
			if (help_mode_)
			{
				QString desc = index.data(CommandPaletteModel::DescriptionRole).toString();
				if (!desc.isEmpty())
				{
					QFont desc_font = option.font;
					desc_font.setPointSizeF(option.font.pointSizeF() * 0.9);
					QRect desc_r(text_x, r.top() + r.height() / 2,
					             r.width() - text_x - 8, r.height() / 2);
					painter->save();
					painter->setFont(desc_font);
					painter->setPen((option.state & QStyle::State_Selected)
					                ? option.palette.highlightedText().color()
					                : option.palette.windowText().color().lighter(140));
					painter->drawText(desc_r, Qt::AlignLeft | Qt::AlignVCenter, desc);
					painter->restore();
				}
			}

			painter->restore();
		}

		// --- CommandPalette --------------------------------------------

		CommandPalette::CommandPalette(QWidget* parent)
			: QWidget(parent, Qt::Popup | Qt::FramelessWindowHint)
		{
			setObjectName("commandPalette");
			setFixedWidth(560);
			setAttribute(Qt::WA_TranslucentBackground, false);

			auto* layout = new QVBoxLayout(this);
			layout->setContentsMargins(8, 8, 8, 8);
			layout->setSpacing(6);

			line_ = new QLineEdit(this);
			line_->setPlaceholderText(tr("Type a command, or ?"));
			line_->setClearButtonEnabled(true);
			layout->addWidget(line_);

			list_ = new QListView(this);
			list_->setEditTriggers(QAbstractItemView::NoEditTriggers);
			list_->setSelectionMode(QAbstractItemView::SingleSelection);
			list_->setUniformItemSizes(true);
			list_->setMinimumHeight(36 * 8);  // 8 visible rows in non-help mode
			layout->addWidget(list_);

			model_ = new CommandPaletteModel(this);
			delegate_ = new CommandPaletteDelegate(this);
			list_->setModel(model_);
			list_->setItemDelegate(delegate_);

			connect(line_, &QLineEdit::textChanged, this, &CommandPalette::onQueryChanged);
			connect(list_, &QListView::activated, this, &CommandPalette::onActivated);

			// Keyboard plumbing: arrow up/down in the line-edit
			// navigates the list; Enter triggers; Esc closes. The
			// installed event filter on the line-edit picks the
			// nav keys before QLineEdit eats them.
			line_->installEventFilter(this);
		}

		CommandPalette::~CommandPalette() = default;

		void CommandPalette::open()
		{
			line_->clear();
			model_->setQuery(QString());
			if (model_->rowCount() > 0)
				list_->setCurrentIndex(model_->index(0));
			centerOnParent_();
			show();
			raise();
			activateWindow();
			line_->setFocus(Qt::ShortcutFocusReason);
		}

		void CommandPalette::close()
		{
			hide();
		}

		void CommandPalette::centerOnParent_()
		{
			QWidget* p = parentWidget();
			if (!p)
			{
				// Fall back to primary screen geometry center.
				QScreen* screen = QApplication::primaryScreen();
				if (screen)
				{
					const QRect g = screen->availableGeometry();
					move(g.center().x() - width() / 2,
					     g.top() + g.height() / 4);
				}
				return;
			}
			const QRect g = p->geometry();
			const QPoint top_left = p->mapToGlobal(QPoint(
				g.width() / 2 - width() / 2,
				g.height() / 4));
			move(top_left);
		}

		void CommandPalette::showEvent(QShowEvent* e)
		{
			QWidget::showEvent(e);
		}

		void CommandPalette::onQueryChanged(const QString& text)
		{
			// Phase 999.46 §6.4 — ?/>/: prefix mode detection.
			//
			//   ?  → help mode: delegate switches to 2-line rows
			//        with prominent description; the model uses
			//        a wider match (title + description).
			//   >  → file mode: only commands whose category is
			//        "File" (Open Recent etc.).
			//   :  → navigation mode: workspace presets +
			//        inspector jumps + recent files (commands
			//        whose id starts with view.workspace., window.,
			//        view.inspector., or file.recent.).
			//   (plain) → fuzzy search all commands.
			//
			// The model handles the search-set selection in
			// setQuery; here we flip the delegate render mode
			// based on the leading char so help mode gets
			// taller rows with the description visible.
			delegate_->setHelpMode(text.startsWith(QLatin1Char('?')));

			// Update the placeholder to hint at the active mode.
			if (text.startsWith(QLatin1Char('?')))
				line_->setToolTip(tr("Help mode — type to filter; descriptions shown"));
			else if (text.startsWith(QLatin1Char('>')))
				line_->setToolTip(tr("File mode — file actions only (Open / Recent / …)"));
			else if (text.startsWith(QLatin1Char(':')))
				line_->setToolTip(tr("Navigation — workspaces, inspector jumps, recent files"));
			else
				line_->setToolTip(QString());

			model_->setQuery(text);
			if (model_->rowCount() > 0)
				list_->setCurrentIndex(model_->index(0));
		}

		void CommandPalette::onActivated(const QModelIndex& index)
		{
			if (!index.isValid()) return;
			Command c = model_->commandAt(index.row());
			if (c.trigger)
			{
				c.trigger();
				CommandRegistry::instance().noteTriggered(c.id);
			}
			close();
		}

		void CommandPalette::triggerCurrent_()
		{
			QModelIndex idx = list_->currentIndex();
			if (idx.isValid()) onActivated(idx);
		}

		void CommandPalette::keyPressEvent(QKeyEvent* e)
		{
			switch (e->key())
			{
				case Qt::Key_Escape:
					close();
					return;
				case Qt::Key_Return:
				case Qt::Key_Enter:
					triggerCurrent_();
					return;
				default:
					break;
			}
			QWidget::keyPressEvent(e);
		}

		bool CommandPalette::eventFilter(QObject* watched, QEvent* event)
		{
			if (watched == line_ && event->type() == QEvent::KeyPress)
			{
				auto* ke = static_cast<QKeyEvent*>(event);
				QModelIndex cur = list_->currentIndex();
				const int row = cur.isValid() ? cur.row() : -1;
				const int n = model_->rowCount();
				switch (ke->key())
				{
					case Qt::Key_Down:
						if (n > 0)
							list_->setCurrentIndex(model_->index(qMin(row + 1, n - 1)));
						return true;
					case Qt::Key_Up:
						if (n > 0)
							list_->setCurrentIndex(model_->index(qMax(row - 1, 0)));
						return true;
					case Qt::Key_Return:
					case Qt::Key_Enter:
						triggerCurrent_();
						return true;
					case Qt::Key_Tab:
						// Handover §6.3 — Tab accepts current line text.
						// Phase 999.46: pass-through (no completion logic
						// this RC; deferred to v1.7.x patch).
						return true;
					case Qt::Key_Escape:
						close();
						return true;
					default:
						break;
				}
			}
			return QWidget::eventFilter(watched, event);
		}
	}
}

#endif // BALL_UI_V2
