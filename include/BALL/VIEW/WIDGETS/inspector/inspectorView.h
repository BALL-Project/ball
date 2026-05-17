// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorView: root widget hosted inside
// InspectorDock. Owns InspectorTabs (top) + InspectorBody (below);
// reads + writes the [Inspector] INI group for visible-state /
// current-tab / per-section collapsed persistence.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORVIEW_H
#define BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORVIEW_H

// Phase 999.44: header unconditional for AUTOMOC; implementation
// wraps body with #ifdef BALL_UI_V2.

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorTabs.h>

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{
		class InspectorBody;
		class InspectorSection;

		/**
		 * Root content widget hosted inside the InspectorDock. Owns the
		 * tab strip + body stack + INI persistence.
		 *
		 * The view subscribes to the existing BALLView mainframe (no
		 * direct dependency — the dock parent is responsible for forwarding
		 * scene/selection changes). State persistence runs through a
		 * 250ms debounced writer to avoid INI churn during drag/scroll.
		 *
		 * QSS objectName: `inspectorView`.
		 */
		class BALL_VIEW_EXPORT InspectorView : public QWidget
		{
			Q_OBJECT

			public:
				explicit InspectorView(QWidget* parent = nullptr);
				~InspectorView() override;

				/** Accessor for the body so the parent dock or future sections can install themselves. */
				InspectorBody* body() const { return body_; }
				InspectorTabs* tabs() const { return tabs_; }

				/**
				 * Append a section to the given tab. The view tracks the
				 * section and persists its expanded/collapsed state under
				 * [Inspector] <section->stateKey()>.
				 */
				void addSection(InspectorTabs::TabIndex idx, InspectorSection* section);

				/**
				 * Set (or clear, with nullptr) the empty-state widget for
				 * the given tab. Replaces any sections previously on that
				 * tab.
				 */
				void setEmptyState(InspectorTabs::TabIndex idx, QWidget* emptyState);

				/** Read [Inspector] group from QSettings into the view. */
				void loadState();

				/** Write [Inspector] group from the view into QSettings (debounced via state_writer_). */
				void scheduleStateWrite();

			private Q_SLOTS:
				void onTabChanged_(int index);
				void onSectionToggled_(bool expanded);
				void flushStateWrite_();

			private:
				InspectorTabs* tabs_;
				InspectorBody* body_;
				QTimer state_writer_;
				bool loading_;  // suppress writes during loadState()

				// (We track section pointers indirectly via Qt's object-tree;
				// expanded-state writes walk children via findChildren<>().)
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORVIEW_H
