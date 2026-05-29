// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.63 (v1.7.x-17b) — ValueRangeWidget implementation.
//
// Pure presentation (ARCHITECTURE-CONTRACT.md §5): paints the histogram +
// colormap + two draggable handles, and emits rangeChanged (drag, preview)
// / rangeCommitted (release, mutate) + the four preset signals. No
// statistics, no hierarchy walk — the BinSummary arrives via setDistribution().
//

#include <BALL/VIEW/WIDGETS/valueRangeWidget.h>

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <algorithm>
#include <cmath>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			constexpr int   DEFAULT_BINS    = 27;   // mockups.html board 05.
			constexpr int   FOOTER_HEIGHT   = 34;   // preset button row.
			constexpr int   READOUT_HEIGHT  = 16;   // min/max text below the plot.
			constexpr int   HANDLE_HALF_W   = 4;     // handle grab half-width (px).
			constexpr int   PLOT_MARGIN     = 6;     // left/right inset for the plot.

			// Colormap sample used both behind the in-range bars and across the
			// gradient strip — a simple blue→white→red interpolation matching the
			// InterpolateColorProcessor default ramp. (Render-only; no contract
			// dependency on the exact colors.)
			QColor colormapAt_(float t)
			{
				t = std::max(0.0f, std::min(1.0f, t));
				if (t < 0.5f)
				{
					float u = t / 0.5f;
					return QColor(static_cast<int>(60 + u * 195),
					              static_cast<int>(90 + u * 165),
					              255);
				}
				float u = (t - 0.5f) / 0.5f;
				return QColor(255,
				              static_cast<int>(255 - u * 165),
				              static_cast<int>(255 - u * 195));
			}
		}

		ValueRangeWidget::ValueRangeWidget(QWidget* parent)
			: QWidget(parent),
				range_min_(0.0f),
				range_max_(1.0f),
				bin_count_(DEFAULT_BINS),
				dragging_(Handle::None)
		{
			setObjectName(QStringLiteral("valueRangeWidget"));
			setMouseTracking(false);
			setMinimumHeight(120);

			// Footer preset buttons. They sit in a child layout pinned to the
			// bottom; the histogram is painted in the area above them.
			QVBoxLayout* root = new QVBoxLayout(this);
			root->setContentsMargins(0, 0, 0, 0);
			root->setSpacing(0);
			root->addStretch(1);                      // plot area (painted, no child).

			QWidget* footer = new QWidget(this);
			footer->setObjectName(QStringLiteral("valueRangeFooter"));
			footer->setFixedHeight(FOOTER_HEIGHT);
			QHBoxLayout* frow = new QHBoxLayout(footer);
			frow->setContentsMargins(PLOT_MARGIN, 2, PLOT_MARGIN, 6);
			frow->setSpacing(4);

			QPushButton* auto_fit = new QPushButton(tr("Auto-fit"), footer);
			QPushButton* full     = new QPushButton(tr("Full range"), footer);
			QPushButton* robust   = new QPushButton(tr("Robust 5–95%"), footer);
			QPushButton* reset    = new QPushButton(tr("Reset"), footer);
			auto_fit->setObjectName(QStringLiteral("valueRangePresetAutoFit"));
			full->setObjectName(QStringLiteral("valueRangePresetFull"));
			robust->setObjectName(QStringLiteral("valueRangePresetRobust"));
			reset->setObjectName(QStringLiteral("valueRangePresetReset"));
			for (QPushButton* b : { auto_fit, full, robust, reset })
			{
				b->setFlat(true);
				b->setCursor(Qt::PointingHandCursor);
				frow->addWidget(b);
			}

			root->addWidget(footer);

			connect(auto_fit, &QPushButton::clicked,
			        this, &ValueRangeWidget::autoFitRequested);
			connect(full, &QPushButton::clicked,
			        this, &ValueRangeWidget::fullRangeRequested);
			connect(robust, &QPushButton::clicked,
			        this, &ValueRangeWidget::robustRangeRequested);
			connect(reset, &QPushButton::clicked,
			        this, &ValueRangeWidget::resetRequested);
		}

		ValueRangeWidget::~ValueRangeWidget() = default;

		QSize ValueRangeWidget::sizeHint() const
		{
			return QSize(260, 150);
		}

		void ValueRangeWidget::setBinCount(int n)
		{
			bin_count_ = std::max(1, n);
		}

		void ValueRangeWidget::setDistribution(const BinSummary& summary)
		{
			const bool extent_changed =
				(summary.min != summary_.min) || (summary.max != summary_.max);
			const bool was_valid = summary_.isValid();

			summary_ = summary;

			if (summary_.isValid() && (!was_valid || extent_changed))
			{
				// First valid summary, or the data extent moved — snap the
				// handles to the full data extent. (A preset / explicit setRange
				// overrides this afterwards.)
				range_min_ = summary_.min;
				range_max_ = summary_.max;
			}
			else if (summary_.isValid())
			{
				// Same extent — keep the handles but clamp them back in-bounds.
				range_min_ = std::max(summary_.min, std::min(range_min_, summary_.max));
				range_max_ = std::max(summary_.min, std::min(range_max_, summary_.max));
			}

			update();
		}

		void ValueRangeWidget::setRange(float min, float max)
		{
			if (max < min) std::swap(min, max);
			if (summary_.isValid())
			{
				min = std::max(summary_.min, std::min(min, summary_.max));
				max = std::max(summary_.min, std::min(max, summary_.max));
			}
			range_min_ = min;
			range_max_ = max;
			update();
		}

		QRect ValueRangeWidget::plotRect_() const
		{
			// The plot occupies everything above the footer, minus a small
			// readout strip at the bottom for the min/max labels.
			int top    = PLOT_MARGIN;
			int bottom = height() - FOOTER_HEIGHT - READOUT_HEIGHT;
			return QRect(PLOT_MARGIN, top,
			             std::max(1, width() - 2 * PLOT_MARGIN),
			             std::max(1, bottom - top));
		}

		int ValueRangeWidget::valueToX_(float v) const
		{
			QRect r = plotRect_();
			if (!summary_.isValid() || summary_.max <= summary_.min)
				return r.left();
			float t = (v - summary_.min) / (summary_.max - summary_.min);
			t = std::max(0.0f, std::min(1.0f, t));
			return r.left() + static_cast<int>(std::lround(t * r.width()));
		}

		float ValueRangeWidget::xToValue_(int x) const
		{
			QRect r = plotRect_();
			if (r.width() <= 0 || !summary_.isValid())
				return summary_.min;
			float t = static_cast<float>(x - r.left()) / static_cast<float>(r.width());
			t = std::max(0.0f, std::min(1.0f, t));
			return summary_.min + t * (summary_.max - summary_.min);
		}

		ValueRangeWidget::Handle ValueRangeWidget::handleAt_(const QPoint& p) const
		{
			if (!summary_.isValid()) return Handle::None;
			int min_x = valueToX_(range_min_);
			int max_x = valueToX_(range_max_);
			int dmin = std::abs(p.x() - min_x);
			int dmax = std::abs(p.x() - max_x);
			if (dmin <= HANDLE_HALF_W + 3 && dmin <= dmax) return Handle::Min;
			if (dmax <= HANDLE_HALF_W + 3)                 return Handle::Max;
			return Handle::None;
		}

		void ValueRangeWidget::paintEvent(QPaintEvent*)
		{
			QPainter g(this);
			g.setRenderHint(QPainter::Antialiasing, false);

			QRect r = plotRect_();

			// Background.
			g.fillRect(rect(), palette().color(QPalette::Base));

			if (!summary_.isValid())
			{
				g.setPen(palette().color(QPalette::Disabled, QPalette::Text));
				g.drawText(r, Qt::AlignCenter,
				           tr("No value distribution\n(select a value-based coloring method)"));
				return;
			}

			const std::size_t nbins = summary_.bin_counts.size();
			Size max_count = 0;
			for (Size c : summary_.bin_counts) max_count = std::max(max_count, c);
			if (max_count == 0) max_count = 1;

			const int min_x = valueToX_(range_min_);
			const int max_x = valueToX_(range_max_);

			// Histogram bars. In-range bins paint with the colormap; out-of-range
			// bins are de-emphasised (grey).
			for (std::size_t i = 0; i < nbins; ++i)
			{
				float lo = summary_.bin_edges[i];
				float hi = summary_.bin_edges[i + 1];
				int x0 = valueToX_(lo);
				int x1 = valueToX_(hi);
				int bw = std::max(1, x1 - x0 - 1);
				float frac = static_cast<float>(summary_.bin_counts[i]) /
				             static_cast<float>(max_count);
				int bh = static_cast<int>(std::lround(frac * r.height()));
				QRect bar(x0, r.bottom() - bh, bw, bh);

				float center = 0.5f * (lo + hi);
				bool in_range = (center >= range_min_ && center <= range_max_);
				if (in_range)
				{
					float t = (summary_.max > summary_.min)
						? (center - summary_.min) / (summary_.max - summary_.min)
						: 0.5f;
					g.fillRect(bar, colormapAt_(t));
				}
				else
				{
					g.fillRect(bar, palette().color(QPalette::Disabled, QPalette::Mid));
				}
			}

			// Colormap gradient strip just under the bars (the clamp preview).
			QRect strip(min_x, r.bottom() + 1,
			            std::max(1, max_x - min_x), 4);
			QLinearGradient grad(strip.topLeft(), strip.topRight());
			grad.setColorAt(0.0, colormapAt_(0.0f));
			grad.setColorAt(0.5, colormapAt_(0.5f));
			grad.setColorAt(1.0, colormapAt_(1.0f));
			g.fillRect(strip, grad);

			// Two handles.
			g.setPen(palette().color(QPalette::Highlight));
			g.setBrush(palette().color(QPalette::Highlight));
			for (int hx : { min_x, max_x })
			{
				g.drawRect(QRect(hx - HANDLE_HALF_W, r.top(),
				                 HANDLE_HALF_W * 2, r.height()));
			}

			// Min/max readouts in the strip below the plot.
			g.setPen(palette().color(QPalette::Text));
			QRect readout(PLOT_MARGIN, r.bottom() + 6,
			              width() - 2 * PLOT_MARGIN, READOUT_HEIGHT);
			g.drawText(readout, Qt::AlignLeft | Qt::AlignVCenter,
			           QString::number(range_min_, 'g', 4));
			g.drawText(readout, Qt::AlignRight | Qt::AlignVCenter,
			           QString::number(range_max_, 'g', 4));
		}

		void ValueRangeWidget::mousePressEvent(QMouseEvent* e)
		{
			if (e->button() != Qt::LeftButton) { QWidget::mousePressEvent(e); return; }
			Handle h = handleAt_(e->pos());
			if (h == Handle::None) { QWidget::mousePressEvent(e); return; }
			dragging_ = h;
			e->accept();
		}

		void ValueRangeWidget::mouseMoveEvent(QMouseEvent* e)
		{
			if (dragging_ == Handle::None) { QWidget::mouseMoveEvent(e); return; }

			float v = xToValue_(e->pos().x());
			if (dragging_ == Handle::Min)
				range_min_ = std::min(v, range_max_);
			else
				range_max_ = std::max(v, range_min_);

			update();
			// Drag-time intent — the RENDER-ONLY preview path. The section maps
			// this onto ColoringController::previewRange(): NO apply(), NO owner
			// mutation, NO typed event.
			Q_EMIT rangeChanged(range_min_, range_max_);
			e->accept();
		}

		void ValueRangeWidget::mouseReleaseEvent(QMouseEvent* e)
		{
			if (dragging_ == Handle::None) { QWidget::mouseReleaseEvent(e); return; }
			dragging_ = Handle::None;
			update();
			// Release — the §2 MUTATION path. The section maps this onto
			// setRange().apply().
			Q_EMIT rangeCommitted(range_min_, range_max_);
			e->accept();
		}

	} // namespace VIEW
} // namespace BALL
