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

// Phase 999.44: gated by BALL_UI_V2 (see inspectorSection.h).

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorTabs.h>

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>

#include <list>

namespace BALL
{
	class Composite;

	namespace VIEW
	{
		class InspectorBody;
		class InspectorSection;
		class SelectionSummarySection;
		class PropertiesSection;
		class QuickActionsSection;
		class MainControl;
		class Representation;
		class RepHeaderSection;
		class ModelSection;
		class ColoringSection;
		class MaterialSection;
		class ModelController;
		class ColoringController;
		class MaterialController;

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

				/**
				 * Phase 999.44 Plan 03 — Selection tab population.
				 * Construct (lazily on first call) the 3 Selection-tab
				 * sections and bind them to @p main_control. After this
				 * call, setSelection(...) routes selection updates to
				 * those sections.
				 */
				void attachSelectionTab(MainControl* main_control);

				/**
				 * Update the Selection-tab sections with the given
				 * composite selection. Empty list → restore empty
				 * state; non-empty → swap sections in and update
				 * their per-section content.
				 */
				void setSelection(const std::list<Composite*>& selection);

				/**
				 * Phase 999.44 Plan 04 — Representation-tab population.
				 * Constructs the Representation tab sections on first
				 * call (lazy) and binds them to read-only mirror
				 * Controllers. The active Representation feeds into
				 * each Controller's setRepresentation slot.
				 */
				void attachRepresentationTab();

				/**
				 * Replace the Representation-tab content with the
				 * given list of representations + the chosen active
				 * Representation. Empty list restores the noRepresentation
				 * empty state.
				 */
				void setRepresentations(const std::list<Representation*>& reps,
				                        Representation* active);

				/**
				 * Phase 999.44 Plan 04 — Switch which Representation
				 * the Model/Coloring/Material sections edit. Called by
				 * the RepHeaderSection picker.
				 */
				void setActiveRepresentation(Representation* rep);

			private Q_SLOTS:
				void onTabChanged_(int index);
				void onSectionToggled_(bool expanded);
				void flushStateWrite_();

			private:
				InspectorTabs* tabs_;
				InspectorBody* body_;
				QTimer state_writer_;
				bool loading_;  // suppress writes during loadState()

				// Phase 999.44 Plan 03 — Selection-tab section pointers.
				// Created lazily in attachSelectionTab; nullptr until then.
				SelectionSummarySection* selection_summary_;
				PropertiesSection*       selection_properties_;
				QuickActionsSection*     selection_actions_;
				bool                     selection_sections_added_;

				// Phase 999.44 Plan 04 — Representation-tab sections +
				// Controllers. Lazy-built in attachRepresentationTab.
				RepHeaderSection*        rep_header_;
				ModelSection*            model_section_;
				ColoringSection*         coloring_section_;
				MaterialSection*         material_section_;
				ModelController*         model_controller_;
				ColoringController*      coloring_controller_;
				MaterialController*      material_controller_;
				bool                     representation_sections_added_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORVIEW_H
