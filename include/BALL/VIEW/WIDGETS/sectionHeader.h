// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: SectionHeader shared widget.
//
// Collapsible section header used in dialogs and (load-bearing) the
// Phase 999.44 Inspector. Renders a chevron toggle + 11px uppercase
// tracked label + hairline rule. State is a single bool (expanded?);
// the *content* the header gates lives outside this widget — owners
// connect to `expandedChanged(bool)` and show/hide their content.
//

#ifndef BALL_VIEW_WIDGETS_SECTIONHEADER_H
#define BALL_VIEW_WIDGETS_SECTIONHEADER_H

#ifndef BALL_COMMON_GLOBAL_
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QString>
#include <QtWidgets/QWidget>

class QLabel;
class QToolButton;

namespace BALL
{
	namespace VIEW
	{
		/**
		 * Collapsible section-header widget for the BALLView Refresh
		 * design system. Composed of a chevron toggle button, a small
		 * uppercase-tracked title label, and a hairline horizontal rule.
		 *
		 * The widget owns its expanded/collapsed state and emits
		 * `expandedChanged` on transitions. It does not own or manage
		 * the section content — the caller is responsible for showing /
		 * hiding the content widget(s) in response to the signal.
		 *
		 * QSS objectName: `sectionHeader` (root), `sectionHeaderTitle`
		 * (the QLabel), `sectionHeaderChevron` (the QToolButton).
		 */
		class BALL_VIEW_EXPORT SectionHeader : public QWidget
		{
			Q_OBJECT

			public:
				explicit SectionHeader(const QString& title, QWidget* parent = nullptr);
				~SectionHeader() override;

				/** Current expanded state (true = expanded, false = collapsed). */
				bool isExpanded() const { return expanded_; }

				/** Section title text. */
				QString title() const;
				void setTitle(const QString& title);

			public Q_SLOTS:
				/**
				 * Set the expanded state. If the new state differs from
				 * the current one, updates the chevron orientation and
				 * emits `expandedChanged`.
				 */
				void setExpanded(bool expanded);

				/** Toggle expanded state (convenience). */
				void toggle();

			Q_SIGNALS:
				/** Emitted whenever the expanded state changes. */
				void expandedChanged(bool expanded);

			private Q_SLOTS:
				void onChevronClicked();

			private:
				bool expanded_;
				QToolButton* chevron_;
				QLabel* title_label_;

				void updateChevronIcon_();
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_SECTIONHEADER_H
