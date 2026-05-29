// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.63 (v1.7.x-17b) — ValueRangeWidget: histogram + draggable
// range handles for value-based coloring (B-factor / Occupancy / Charge /
// Distance / Custom).
//
// ARCHITECTURE-CONTRACT.md §5 ("View = pure presentation"): the widget
// renders a Controller-derived BinSummary and emits user intent. It owns
// NO business logic and never walks the molecular hierarchy. Two draggable
// handles emit:
//   - rangeChanged(min, max)   while dragging  → render-only PREVIEW path
//   - rangeCommitted(min, max) on release       → the §2 MUTATION path
// The section maps the former onto ColoringController::previewRange() (a
// non-owner, render-only hint) and the latter onto setRange().apply().
//

#ifndef BALL_VIEW_WIDGETS_VALUERANGEWIDGET_H
#define BALL_VIEW_WIDGETS_VALUERANGEWIDGET_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtWidgets/QWidget>

#include <vector>

namespace BALL
{
	namespace VIEW
	{
		/**
		 * Bin-summary read-model produced by ColoringController::distribution()
		 * and consumed by ValueRangeWidget (ARCHITECTURE-CONTRACT.md §5). A
		 * plain value type — the widget cannot mutate the owner through it.
		 *
		 * Invariant: when @c valid is true, @c bin_edges has exactly
		 * <tt>bin_counts.size() + 1</tt> entries (the left/right edge of each
		 * bin), @c min == bin_edges.front(), @c max == bin_edges.back().
		 */
		struct BALL_VIEW_EXPORT BinSummary
		{
			bool               valid = false;  ///< false ⇒ empty selection / non-value method.
			float              min   = 0.0f;   ///< Smallest observed value (== bin_edges.front()).
			float              max   = 0.0f;   ///< Largest observed value  (== bin_edges.back()).
			std::vector<float> bin_edges;      ///< size == bin_counts.size() + 1.
			std::vector<Size>  bin_counts;     ///< Per-bin sample counts.

			/// True iff the summary describes a non-empty, well-formed distribution.
			bool isValid() const { return valid && !bin_counts.empty(); }
		};

		/**
		 * Histogram + dual-handle range widget for value-based coloring.
		 *
		 * Renders a 27-bin (default) histogram of the active selection's scalar
		 * value with the colormap painted behind the bars; in-range bins paint
		 * with the colormap, out-of-range bins are de-emphasised. Two draggable
		 * handles clamp the colormap; four preset buttons (Auto-fit / Full range
		 * / Robust 5–95% / Reset) sit in the footer.
		 *
		 * Pure presentation (§5): no statistics, no hierarchy walk. The
		 * distribution arrives via setDistribution(); user intent leaves via the
		 * rangeChanged / rangeCommitted / preset* signals.
		 */
		class BALL_VIEW_EXPORT ValueRangeWidget : public QWidget
		{
			Q_OBJECT

			public:
				explicit ValueRangeWidget(QWidget* parent = nullptr);
				~ValueRangeWidget() override;

				/// The current low handle value (clamped to [dataMin, dataMax]).
				float rangeMin() const { return range_min_; }
				/// The current high handle value (clamped to [dataMin, dataMax]).
				float rangeMax() const { return range_max_; }

				/// Smallest data value from the last setDistribution().
				float dataMin() const { return summary_.min; }
				/// Largest data value from the last setDistribution().
				float dataMax() const { return summary_.max; }

				/// True once a valid distribution has been installed.
				bool hasDistribution() const { return summary_.isValid(); }

				QSize sizeHint() const override;

			public Q_SLOTS:
				/**
				 * Install the histogram read-model and repaint. Resets the
				 * handles to the data extent only the first time a valid summary
				 * is seen or when the data extent changes; otherwise the existing
				 * handle positions are clamped into the new extent.
				 * @param summary the Controller-derived bin summary.
				 */
				void setDistribution(const BinSummary& summary);

				/**
				 * Move the handles WITHOUT emitting (programmatic set, e.g. a
				 * preset or a controller resync). Clamps to the data extent and
				 * repaints. Does NOT fire rangeChanged / rangeCommitted.
				 * @param min low handle value.
				 * @param max high handle value.
				 */
				void setRange(float min, float max);

				/**
				 * Escape hatch for tiny selections (≤ default bins): override the
				 * preferred bin count the section passes to distribution().
				 * @param n requested bin count (clamped ≥ 1).
				 */
				void setBinCount(int n);
				int  binCount() const { return bin_count_; }

			Q_SIGNALS:
				/// Emitted continuously while a handle is dragged (PREVIEW path).
				void rangeChanged(float min, float max);
				/// Emitted once on handle release (the §2 MUTATION path).
				void rangeCommitted(float min, float max);
				/// Footer preset buttons — each commits through the section.
				void autoFitRequested();
				void fullRangeRequested();
				void robustRangeRequested();
				void resetRequested();

			protected:
				void paintEvent(QPaintEvent* e) override;
				void mousePressEvent(QMouseEvent* e) override;
				void mouseMoveEvent(QMouseEvent* e) override;
				void mouseReleaseEvent(QMouseEvent* e) override;

			private:
				enum class Handle { None, Min, Max };

				/// The histogram plot rectangle (excludes the footer button row).
				QRect plotRect_() const;
				/// Map a data value to an x pixel inside plotRect_().
				int   valueToX_(float v) const;
				/// Map an x pixel to a data value, clamped to the data extent.
				float xToValue_(int x) const;
				/// Hit-test a press against the two handles (within a few px).
				Handle handleAt_(const QPoint& p) const;

				BinSummary summary_;
				float      range_min_;
				float      range_max_;
				int        bin_count_;
				Handle     dragging_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_VALUERANGEWIDGET_H
