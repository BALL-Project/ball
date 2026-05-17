// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 03 — SelectionSummarySection: Selection-tab
// Inspector section showing aggregate counts of selected atoms /
// residues / chains / molecules.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_SELECTIONSUMMARYSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_SELECTIONSUMMARYSECTION_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <list>

class QLabel;

namespace BALL
{
	class Composite;

	namespace VIEW
	{

		/**
		 * Selection-tab section: aggregate counts of the current
		 * composite selection. Updated by InspectorView::setSelection
		 * which walks the list of selected composites and tallies
		 * Atom / Residue / Chain / Molecule sub-counts.
		 */
		class BALL_VIEW_EXPORT SelectionSummarySection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit SelectionSummarySection(QWidget* parent = nullptr);
				~SelectionSummarySection() override;

			public Q_SLOTS:
				/** Recompute and render the summary for @p selection. */
				void updateForSelection(const std::list<Composite*>& selection);

			private:
				QLabel* atoms_label_;
				QLabel* residues_label_;
				QLabel* chains_label_;
				QLabel* molecules_label_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_SELECTIONSUMMARYSECTION_H
