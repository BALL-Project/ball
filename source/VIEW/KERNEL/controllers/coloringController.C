// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringController implementation.
// UFG-03 cut-over (v1.7-modernization) — apply() mutates the attached
// Representation through the per-ColoringMethod processor factory and
// the Representation's own setters.
//
// Phase 999.57 Plan 03 (VIEW-CLEAN-02 render-path cut-over): apply()
// no longer reaches through DisplayProperties' owned
// ColoringSettingsDialog for the processor factory. It now constructs
// the color processor via the relocated ColorProcessorFactory::create
// with EMPTY ColoringOverrides (compiled defaults — the controller does
// not expose per-element/residue color overrides), then applies
// value_min_/value_max_ to the InterpolateColorProcessor exactly as
// before — making the controller fully dialog-independent.
//

#include <BALL/VIEW/KERNEL/controllers/coloringController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/MODELS/representationBuilder.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/MODELS/colorProcessorFactory.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/CONCEPT/timeStamp.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/COMMON/rtti.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/forEach.h>

#include <algorithm>
#include <cmath>
#include <functional>
#include <sstream>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Compact owner-state snapshot for the ApplyPayload before/after
			// blobs (§2 step 3).
			String snapshotColoringState_(Representation* rep)
			{
				if (rep == nullptr) return String("coloring{null}");
				std::ostringstream s;
				s << "coloring=" << static_cast<int>(rep->getColoringMethod());
				InterpolateColorProcessor* icp =
					dynamic_cast<InterpolateColorProcessor*>(rep->getColorProcessor());
				if (icp != nullptr)
				{
					s << ";min=" << icp->getMinValue()
					  << ";max=" << icp->getMaxValue();
				}
				return String(s.str());
			}
		}

		ColoringController::ColoringController(Representation* rep, QObject* parent)
			: Controller(parent), rep_(rep), coloring_method_(0),
				value_min_(0.0f), value_max_(100.0f)
		{
			revert();
		}

		ColoringController::~ColoringController() = default;

		void ColoringController::setRepresentation(Representation* rep)
		{
			if (rep_ == rep) return;
			rep_ = rep;
			revert();
		}

		void ColoringController::revert()
		{
			if (rep_ == nullptr) return;
			int cm = static_cast<int>(rep_->getColoringMethod());
			if (cm != coloring_method_)
			{
				coloring_method_ = cm;
				Q_EMIT coloringMethodChanged(cm);
			}

			// Pull the value range from the attached processor if it is
			// one of the value-based (interpolating) color processors.
			InterpolateColorProcessor* icp =
				dynamic_cast<InterpolateColorProcessor*>(rep_->getColorProcessor());
			if (icp != nullptr)
			{
				float vmin = icp->getMinValue();
				if (vmin != value_min_) { value_min_ = vmin; Q_EMIT valueMinChanged(vmin); }
				float vmax = icp->getMaxValue();
				if (vmax != value_max_) { value_max_ = vmax; Q_EMIT valueMaxChanged(vmax); }
			}
		}

		void ColoringController::reset()
		{
			// 999.64 — SINGLE-call reset to the METHOD-DEFINED defaults (NOT the
			// legacy two-step revert(); apply()). Default coloring method (0 =
			// element) and the default 0..100 value range; emit the change
			// notifications so the Inspector + histogram resync, then run the
			// SINGLE §2 apply() (captures current owner as `before`, installs the
			// defaults as `after`, one event, one reversible payload).
			coloring_method_ = 0;
			value_min_       = 0.0f;
			value_max_       = 100.0f;

			Q_EMIT coloringMethodChanged(coloring_method_);
			Q_EMIT valueMinChanged(value_min_);
			Q_EMIT valueMaxChanged(value_max_);

			apply();
		}

		void ColoringController::setRange(float min, float max)
		{
			// 999.63 — stage both endpoints atomically. NOT a mutation of the
			// owner (apply() does that); just updates the staged mirror state and
			// emits the per-endpoint change notifications the section listens for.
			if (max < min) std::swap(min, max);
			setValueMin(min);
			setValueMax(max);
		}

		void ColoringController::previewRange(float min, float max)
		{
			// 999.63 / ARCHITECTURE-CONTRACT.md §5 — RENDER-ONLY drag-preview hint.
			//
			// This is explicitly NON-OWNER. It MUST NOT:
			//   - create an ApplyPayload,
			//   - emit a typed event,
			//   - call any owner setter (Representation::update / RepresentationBuilder),
			//   - take a ControllerApplyGuard,
			//   - touch the staged mutation state (coloring_method_ / value_min_ /
			//     value_max_) — otherwise a subsequent apply() would silently
			//     commit the dragged value, turning this into a second mutation
			//     path (exactly the regression the contract test guards against).
			//
			// In v1.7.4 the render-side overlay falls back to the cheaper
			// "recolor the histogram bars in-range during drag" hint owned by the
			// widget itself (see the plan's Risk note), so there is no scene-side
			// owner touch here. The method is the explicit, named, non-mutating
			// render seam: idempotent, freely re-callable per drag-frame, and a
			// no-op against the owner. The diagnostic log makes the render-only
			// intent observable without any state change.
			if (max < min) std::swap(min, max);
			Log.info() << "[ColoringController::previewRange] render-only hint ["
			           << min << ", " << max << "] — no mutation." << std::endl;
		}

		bool ColoringController::isValueBasedMethod_(int method)
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

		float ColoringController::atomValue_(const Atom& atom) const
		{
			switch (coloring_method_)
			{
				case COLORING_ATOM_CHARGE:
					return static_cast<float>(atom.getCharge());
				case COLORING_FORCES:
					return static_cast<float>(atom.getForce().getLength());
				case COLORING_TEMPERATURE_FACTOR:
				{
					const PDBAtom* pa = dynamic_cast<const PDBAtom*>(&atom);
					return (pa != nullptr) ? pa->getTemperatureFactor() : 0.0f;
				}
				case COLORING_OCCUPANCY:
				{
					const PDBAtom* pa = dynamic_cast<const PDBAtom*>(&atom);
					return (pa != nullptr) ? pa->getOccupancy() : 0.0f;
				}
				case COLORING_DISTANCE:
				case COLORING_CUSTOM:
				default:
					// Distance / custom carry their scalar on a per-atom named
					// property the InterpolateColorProcessor reads. Fall back to 0
					// when the property is absent (e.g. distances not yet computed).
					return atom.hasProperty("ColorValue")
						? static_cast<float>(atom.getProperty("ColorValue").getFloat())
						: 0.0f;
			}
		}

		std::size_t ColoringController::compositeHash_() const
		{
			if (rep_ == nullptr) return 0;
			std::size_t h = 1469598103934665603ull;     // FNV-ish seed.
			const std::list<const Composite*>& comps = rep_->getComposites();
			for (const Composite* c : comps)
			{
				h ^= reinterpret_cast<std::size_t>(c);
				h *= 1099511628211ull;
			}
			h ^= comps.size();
			return h;
		}

		void ColoringController::rebuildDistribution_(int bins)
		{
			BinSummary s;
			s.valid = false;

			if (rep_ == nullptr || !isValueBasedMethod_(coloring_method_) || bins < 1)
			{
				dist_cache_ = s;
				return;
			}

			// O(N) single pass: collect the per-method scalar for every atom in
			// the Representation's composites, find the extent, then bin.
			std::vector<float> values;
			const std::list<const Composite*>& comps = rep_->getComposites();
			for (const Composite* c : comps)
			{
				if (c == nullptr) continue;
				if (RTTI::isKindOf<AtomContainer>(c))
				{
					const AtomContainer* ac = dynamic_cast<const AtomContainer*>(c);
					if (ac == nullptr) continue;
					AtomConstIterator it;
					BALL_FOREACH_ATOM(*ac, it)
					{
						values.push_back(atomValue_(*it));
					}
				}
				else if (RTTI::isKindOf<Atom>(c))
				{
					const Atom* a = dynamic_cast<const Atom*>(c);
					if (a != nullptr) values.push_back(atomValue_(*a));
				}
			}

			if (values.empty())
			{
				dist_cache_ = s;
				return;
			}

			float vmin = values.front();
			float vmax = values.front();
			for (float v : values)
			{
				vmin = std::min(vmin, v);
				vmax = std::max(vmax, v);
			}

			// Guard against a degenerate (all-equal) extent so the bins are valid.
			if (vmax <= vmin) vmax = vmin + 1.0f;

			s.valid = true;
			s.min = vmin;
			s.max = vmax;
			s.bin_edges.resize(static_cast<std::size_t>(bins) + 1);
			s.bin_counts.assign(static_cast<std::size_t>(bins), 0);
			float width = (vmax - vmin) / static_cast<float>(bins);
			for (int i = 0; i <= bins; ++i)
				s.bin_edges[static_cast<std::size_t>(i)] = vmin + width * static_cast<float>(i);

			for (float v : values)
			{
				int idx = static_cast<int>((v - vmin) / width);
				if (idx < 0) idx = 0;
				if (idx >= bins) idx = bins - 1;
				++s.bin_counts[static_cast<std::size_t>(idx)];
			}

			dist_cache_ = s;
		}

		BinSummary ColoringController::distribution(int bins)
		{
			std::size_t chash = compositeHash_();
			bool hit = dist_cache_valid_
				&& dist_cache_composite_hash_ == chash
				&& dist_cache_method_ == coloring_method_
				&& dist_cache_bins_ == bins;

			if (!hit)
			{
				rebuildDistribution_(bins);
				dist_cache_composite_hash_ = chash;
				dist_cache_method_ = coloring_method_;
				dist_cache_bins_ = bins;
				dist_cache_valid_ = true;
			}
			return dist_cache_;
		}

		bool ColoringController::apply()
		{
			// ── 1. Preconditions (§2 step 1) ─────────────────────────────────
			if (rep_ == nullptr)
			{
				Log.warn() << "[ColoringController::apply] no Representation attached — skipping." << std::endl;
				return false;                    // rejected — nothing to mutate
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[ColoringController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// ── 2. Re-entrancy shield (§2 step 2 / §4) — nest-aware drop ─────
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "coloring.setMethod";
			payload.before     = snapshotColoringState_(rep_);
			payload.target     = rep_;
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotColoringState_(rep_);

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT appliedStub();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			invalidateDeclared_();               // soft refresh

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
		}

		bool ColoringController::applyInternal_()
		{
			// §13 cookbook step 2 — existing mutation body, moved out of
			// apply(). Precondition (rep_ != nullptr) guaranteed by apply().
			// Does NOT invalidate (that is invalidateDeclared_()).
			ColoringMethod new_method = static_cast<ColoringMethod>(coloring_method_);
			bool method_changed = (rep_->getColoringMethod() != new_method);

			if (method_changed || rep_->getColorProcessor() == nullptr)
			{
				// Phase 999.57 Plan 03: build the color processor through the
				// headless ColorProcessorFactory with EMPTY ColoringOverrides.
				// An empty override set means "keep the processor's compiled
				// defaults" (CONFIG-01), which is exactly what this controller
				// wants — it exposes only coloring_method_ + value_min_/value_max_
				// and never carried per-element/residue color overrides. The
				// value range is applied below, unchanged.
				ColorProcessor* cp = ColorProcessorFactory::create(new_method);
				if (cp == nullptr)
				{
					// Defensive fallback — install a baseline processor so the
					// renderer always has something to invoke.
					cp = new ColorProcessor();
				}
				// §3c — Representation mutation flows through RepresentationBuilder.
				RepresentationBuilder::setColorProcessor(*rep_, cp);
				RepresentationBuilder::setColoringMethod(*rep_, new_method);
			}

			// Value-range parameters are live setters on the processor,
			// so a min/max-only change needs no recreate — push them onto
			// whatever processor is currently attached (newly created or
			// pre-existing) before the color re-walk.
			InterpolateColorProcessor* icp =
				dynamic_cast<InterpolateColorProcessor*>(rep_->getColorProcessor());
			if (icp != nullptr)
			{
				icp->setMinValue(value_min_);
				icp->setMaxValue(value_max_);
			}

			// 999.63 fix — a value-range-only change swaps no processor, so
			// Representation's changed_color_processor_ gate would stay false and
			// the declared soft update(false) would SKIP the recolor (the
			// histogram drag then had no visible effect). Mark the processor dirty
			// so the soft refresh re-walks colors over the existing geometry —
			// cheap, no SE-surface rebuild (preserves the <50ms target).
			RepresentationBuilder::markColorProcessorChanged(*rep_);

			return true;
		}

		void ColoringController::invalidateDeclared_()
		{
			// §2a Coloring row — SOFT refresh. A coloring-method / value-range
			// change re-walks the color processor over the EXISTING geometry;
			// the primitive set is unchanged, so update(false) (no display-list
			// rebuild) is correct. Contrast ModelController, which rebuilds
			// because a model-processor swap changes the primitives.
			if (rep_ != nullptr) rep_->update(false);
		}


		void ColoringController::setColoringMethod(int m)
		{
			if (m == coloring_method_) return;
			coloring_method_ = m;
			Q_EMIT coloringMethodChanged(m);
		}

		void ColoringController::setValueMin(float v)
		{
			if (v == value_min_) return;
			value_min_ = v;
			Q_EMIT valueMinChanged(v);
		}

		void ColoringController::setValueMax(float v)
		{
			if (v == value_max_) return;
			value_max_ = v;
			Q_EMIT valueMaxChanged(v);
		}

	} // namespace VIEW
} // namespace BALL

