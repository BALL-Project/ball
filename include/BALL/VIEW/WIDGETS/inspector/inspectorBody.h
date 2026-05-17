// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorBody: QStackedWidget under a QScrollArea
// hosting one page per Inspector tab. Each page is a vertical stack
// of InspectorSections plus an optional empty state.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORBODY_H
#define BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORBODY_H

// Phase 999.44: gated by BALL_UI_V2 (see inspectorSection.h).

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorTabs.h>

#include <QtWidgets/QWidget>

class QStackedWidget;
class QVBoxLayout;

namespace BALL
{
	namespace VIEW
	{
		class InspectorSection;

		/**
		 * Body host for the Inspector. A QStackedWidget switched by
		 * InspectorTabs; each page wraps a vertical layout inside a
		 * QScrollArea so sections that overflow the dock height scroll
		 * cleanly.
		 *
		 * Add sections via addSection(TabIndex, InspectorSection*).
		 * Replace a page's content with an empty-state widget via
		 * setEmptyState(TabIndex, QWidget*) when the tab has nothing to
		 * show.
		 *
		 * QSS objectName: `inspectorBody`.
		 */
		class BALL_VIEW_EXPORT InspectorBody : public QWidget
		{
			Q_OBJECT

			public:
				explicit InspectorBody(QWidget* parent = nullptr);
				~InspectorBody() override;

				/** Show the page for the given tab. */
				void setCurrentTab(InspectorTabs::TabIndex idx);

				/**
				 * Append a section to the given tab's section list. Takes
				 * ownership. Sections accumulate top-to-bottom.
				 */
				void addSection(InspectorTabs::TabIndex idx, InspectorSection* section);

				/**
				 * Replace the page contents with the given empty-state
				 * widget. Removes all previously added sections from that
				 * tab (they are deleted). Passing nullptr restores an empty
				 * sectionless page so subsequent addSection() calls work.
				 */
				void setEmptyState(InspectorTabs::TabIndex idx, QWidget* emptyState);

				/** Remove (delete) all sections from the given tab. */
				void clearSections(InspectorTabs::TabIndex idx);

			private:
				struct TabPage
				{
					QWidget* page = nullptr;       // QStackedWidget child
					QVBoxLayout* layout = nullptr; // section list layout (inside scroll area's contents)
					QWidget* emptyState = nullptr; // current empty state widget (may be null)
				};

				QStackedWidget* stack_;
				TabPage pages_[3];

				QWidget* makePage_(TabPage& tp);
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORBODY_H
