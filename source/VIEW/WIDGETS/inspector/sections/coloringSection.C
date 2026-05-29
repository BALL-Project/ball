// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringSection implementation.
//
// Phase 999.63 (v1.7.x-17b) — the value-based methods' Min/Max sliders are
// replaced by the ValueRangeWidget (histogram + draggable handles + 4
// presets). The section is pure presentation (ARCHITECTURE-CONTRACT.md §5):
//   - rangeCommitted (handle release) → setRange(min,max).apply()  [§2 MUTATION]
//   - rangeChanged   (drag)           → previewRange(min,max)       [§5 RENDER]
//   - presets                         → setRange(...).apply() / reset()
// The histogram data comes from ColoringController::distribution(); the
// section never walks the Composite tree.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/coloringSection.h>


#include <BALL/VIEW/KERNEL/controllers/coloringController.h>
#include <BALL/VIEW/WIDGETS/valueRangeWidget.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtCore/QSignalBlocker>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include <algorithm>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			void populateMethods_(QComboBox* c)
			{
				c->addItem(QStringLiteral("Element"),             COLORING_ELEMENT);
				c->addItem(QStringLiteral("Residue index"),       COLORING_RESIDUE_INDEX);
				c->addItem(QStringLiteral("Residue name"),        COLORING_RESIDUE_NAME);
				c->addItem(QStringLiteral("Secondary structure"), COLORING_SECONDARY_STRUCTURE);
				c->addItem(QStringLiteral("Atom charge"),         COLORING_ATOM_CHARGE);
				c->addItem(QStringLiteral("Distance"),            COLORING_DISTANCE);
				c->addItem(QStringLiteral("Temperature factor"),  COLORING_TEMPERATURE_FACTOR);
				c->addItem(QStringLiteral("Occupancy"),           COLORING_OCCUPANCY);
				c->addItem(QStringLiteral("Forces"),              COLORING_FORCES);
				c->addItem(QStringLiteral("Residue type"),        COLORING_RESIDUE_TYPE);
				c->addItem(QStringLiteral("Chain"),               COLORING_CHAIN);
				c->addItem(QStringLiteral("Molecule"),            COLORING_MOLECULE);
				c->addItem(QStringLiteral("Custom"),              COLORING_CUSTOM);
			}

			int findIndexForData_(QComboBox* c, int value)
			{
				for (int i = 0; i < c->count(); ++i)
					if (c->itemData(i).toInt() == value) return i;
				return -1;
			}

			bool isValueBasedMethod_(int method)
			{
				switch (method)
				{
					case COLORING_TEMPERATURE_FACTOR:
					case COLORING_OCCUPANCY:
					case COLORING_ATOM_CHARGE:
					case COLORING_DISTANCE:
					case COLORING_FORCES:
					case COLORING_CUSTOM:
						return true;
					default:
						return false;
				}
			}
		}

		ColoringSection::ColoringSection(ColoringController* controller, QWidget* parent)
			: InspectorSection(tr("Coloring"),
			                   QStringLiteral("Representation/Coloring"),
			                   parent),
				controller_(controller),
				method_(nullptr),
				type_options_(nullptr),
				range_widget_(nullptr),
				page_empty_(0), page_value_range_(0)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			method_ = new QComboBox(content);
			populateMethods_(method_);
			col->addWidget(new FormRow(tr("Method"), method_, content));

			// v1.7.x-17 → 999.63 — per-method options. One QStackedWidget page
			// per option group; an empty page for methods without extra options.
			// The visible page tracks the selected method. The value-based
			// methods share the histogram + range-handle widget page.
			type_options_ = new QStackedWidget(content);

			// Empty page (default for methods without extra options).
			QWidget* empty_page = new QWidget(type_options_);
			page_empty_ = type_options_->addWidget(empty_page);

			// Value-range histogram page (shared by the value-based methods).
			range_widget_ = new ValueRangeWidget(type_options_);
			page_value_range_ = type_options_->addWidget(range_widget_);

			col->addWidget(type_options_);

			setContent(content);

			if (controller_)
			{
				int i = findIndexForData_(method_, controller_->coloringMethod());
				if (i >= 0) method_->setCurrentIndex(i);
			}

			connect(method_,
			        QOverload<int>::of(&QComboBox::currentIndexChanged),
			        this, &ColoringSection::onMethodChosen_);

			// 999.63 — histogram widget wiring.
			connect(range_widget_, &ValueRangeWidget::rangeChanged,
			        this, &ColoringSection::onRangeChanged_);
			connect(range_widget_, &ValueRangeWidget::rangeCommitted,
			        this, &ColoringSection::onRangeCommitted_);
			connect(range_widget_, &ValueRangeWidget::autoFitRequested,
			        this, &ColoringSection::onAutoFit_);
			connect(range_widget_, &ValueRangeWidget::fullRangeRequested,
			        this, &ColoringSection::onFullRange_);
			connect(range_widget_, &ValueRangeWidget::robustRangeRequested,
			        this, &ColoringSection::onRobustRange_);
			connect(range_widget_, &ValueRangeWidget::resetRequested,
			        this, &ColoringSection::onWidgetReset_);

			// Show the page matching the initial coloring method.
			showPageForMethod_(controller_ ? controller_->coloringMethod()
			                                : method_->currentData().toInt());

			if (controller_)
			{
				connect(controller_, &ColoringController::coloringMethodChanged,
				        this, &ColoringSection::onControllerMethodChanged_);
				connect(controller_, &ColoringController::valueMinChanged,
				        this, &ColoringSection::onControllerValueMinChanged_);
				connect(controller_, &ColoringController::valueMaxChanged,
				        this, &ColoringSection::onControllerValueMaxChanged_);

				// 999.64 — per-section reset via the single-call Controller::reset()
				// (sets method-defined defaults, one §2 apply, one reversible
				// payload), replacing the legacy two-step revert(); apply().
				setResettable(true, tr("Reset Coloring settings to defaults?"));
				connect(this, &InspectorSection::resetRequested, this, [this]() {
					if (controller_) controller_->reset();
				});
			}
		}

		ColoringSection::~ColoringSection() = default;

		void ColoringSection::showPageForMethod_(int method)
		{
			if (!type_options_) return;
			int page = isValueBasedMethod_(method) ? page_value_range_ : page_empty_;
			type_options_->setCurrentIndex(page);
			if (page == page_value_range_) refreshDistribution_();
		}

		void ColoringSection::refreshDistribution_()
		{
			if (!controller_ || !range_widget_) return;
			// §5 — the histogram data is the Controller's read-model; the section
			// never walks the Composite tree. distribution() is cached.
			BinSummary s = controller_->distribution(range_widget_->binCount());
			range_widget_->setDistribution(s);
			// Reflect the controller's currently-staged range onto the handles.
			QSignalBlocker b(range_widget_);
			range_widget_->setRange(controller_->valueMin(), controller_->valueMax());
		}

		void ColoringSection::fullRangeLimitsForMethod_(int method,
		                                                float& lo, float& hi) const
		{
			// Method-defined full-range limits for the Full range preset.
			switch (method)
			{
				case COLORING_TEMPERATURE_FACTOR:
				case COLORING_OCCUPANCY:
					lo = 0.0f;   hi = 100.0f; break;
				case COLORING_ATOM_CHARGE:
					lo = -5.0f;  hi = 5.0f;   break;
				case COLORING_FORCES:
					lo = 0.0f;   hi = 1000.0f; break;
				default:
					// Distance / Custom — fall back to the observed data extent.
					if (range_widget_ && range_widget_->hasDistribution())
					{ lo = range_widget_->dataMin(); hi = range_widget_->dataMax(); }
					else { lo = 0.0f; hi = 100.0f; }
					break;
			}
		}

		void ColoringSection::onMethodChosen_(int idx)
		{
			if (idx < 0 || !controller_) return;
			int m = method_->itemData(idx).toInt();
			controller_->setColoringMethod(m);
			showPageForMethod_(m);
			// A method change commits immediately through the §2 path.
			controller_->apply();
			if (isValueBasedMethod_(m)) refreshDistribution_();
		}

		// ── 999.63 — histogram widget intent ───────────────────────────────────

		void ColoringSection::onRangeChanged_(float min, float max)
		{
			// Drag-time — the RENDER-ONLY preview path (§5). NO apply(), NO owner
			// mutation, NO typed event. previewRange() is explicitly non-owner.
			if (controller_) controller_->previewRange(min, max);
		}

		void ColoringSection::onRangeCommitted_(float min, float max)
		{
			// Handle release — the ONLY mutation path (§2): setRange().apply().
			if (!controller_) return;
			controller_->setRange(min, max);
			controller_->apply();
		}

		void ColoringSection::onAutoFit_()
		{
			if (!controller_ || !range_widget_) return;
			refreshDistribution_();
			if (range_widget_->hasDistribution())
			{
				controller_->setRange(range_widget_->dataMin(), range_widget_->dataMax());
				controller_->apply();
			}
		}

		void ColoringSection::onFullRange_()
		{
			if (!controller_) return;
			float lo = 0.0f, hi = 100.0f;
			fullRangeLimitsForMethod_(controller_->coloringMethod(), lo, hi);
			controller_->setRange(lo, hi);
			controller_->apply();
		}

		void ColoringSection::onRobustRange_()
		{
			if (!controller_ || !range_widget_) return;
			// Robust 5–95% — percentile bin edges from the (cached) distribution.
			BinSummary s = controller_->distribution(range_widget_->binCount());
			if (!s.isValid()) return;

			Size total = 0;
			for (Size c : s.bin_counts) total += c;
			if (total == 0) return;

			Size lo_target = static_cast<Size>(0.05 * total);
			Size hi_target = static_cast<Size>(0.95 * total);
			Size cum = 0;
			float lo = s.min, hi = s.max;
			bool lo_set = false;
			for (std::size_t i = 0; i < s.bin_counts.size(); ++i)
			{
				cum += s.bin_counts[i];
				if (!lo_set && cum >= lo_target) { lo = s.bin_edges[i]; lo_set = true; }
				if (cum >= hi_target) { hi = s.bin_edges[i + 1]; break; }
			}
			controller_->setRange(lo, hi);
			controller_->apply();
		}

		void ColoringSection::onWidgetReset_()
		{
			// 999.64 — the ValueRangeWidget's own reset button. Controller::reset()
			// is now the SINGLE-call reset: it sets the method-defined coloring
			// defaults AND runs ONE §2 apply() itself, so we must NOT call apply()
			// again here (that would emit a second event / second payload, the very
			// dual-path the contract closes). Just refresh the histogram afterwards.
			if (!controller_) return;
			controller_->reset();
			refreshDistribution_();
		}

		// ── controller → section resync ─────────────────────────────────────────

		void ColoringSection::onControllerMethodChanged_(int m)
		{
			int i = findIndexForData_(method_, m);
			if (i >= 0 && method_->currentIndex() != i)
			{
				QSignalBlocker b(method_);
				method_->setCurrentIndex(i);
			}
			showPageForMethod_(m);
		}

		void ColoringSection::onControllerValueMinChanged_(float v)
		{
			if (!range_widget_) return;
			QSignalBlocker b(range_widget_);
			range_widget_->setRange(v, controller_ ? controller_->valueMax() : range_widget_->rangeMax());
		}

		void ColoringSection::onControllerValueMaxChanged_(float v)
		{
			if (!range_widget_) return;
			QSignalBlocker b(range_widget_);
			range_widget_->setRange(controller_ ? controller_->valueMin() : range_widget_->rangeMin(), v);
		}

	} // namespace VIEW
} // namespace BALL
