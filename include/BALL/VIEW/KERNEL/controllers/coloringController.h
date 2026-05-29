// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/coloringSettingsDialog.C (~878 LOC).
//
// Phase 999.59-02 (v1.7.x-24) — cut over to the §2 `bool apply()` command
// contract (ARCHITECTURE-CONTRACT.md §2 / §2a / §13). ColoringController now
// inherits the base Controller and overrides bool apply() in the §2 shape.
// Its §2a declared invalidation is a SOFT refresh
// (representation_->update(false)): a coloring-method change rewalks the
// color processor over the existing geometry but leaves the primitive set
// intact, so no display-list rebuild is needed.
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLER_H
# include <BALL/VIEW/KERNEL/controllers/controller.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
# include <BALL/VIEW/KERNEL/controllers/applyPayload.h>
#endif

#include <QtCore/QObject>

#include <vector>

// ---------------------------------------------------------------------------
// 999.59-02 per-controller contract-fixture activation symbol (Codex MEDIUM
// #7) — flips coloring_contract_test's parity block live (see
// test/contract/contractFixtureActivation.h). Defined ONLY because apply()
// below returns bool.
// ---------------------------------------------------------------------------
#define BALL_VIEW_COLORING_APPLY_BOOL 1

namespace BALL
{
	class Atom;

	namespace VIEW
	{
		class Representation;

		/**
		 * Bin-summary read-model produced by ColoringController::distribution()
		 * (ARCHITECTURE-CONTRACT.md §5). A plain, copy-constructible value type:
		 * the widget reads it but cannot mutate the owner through it.
		 *
		 * Invariant: when @c valid is true, @c bin_edges has exactly
		 * <tt>bin_counts.size() + 1</tt> entries (left/right edge of each bin),
		 * @c min == bin_edges.front(), @c max == bin_edges.back().
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
		 * Per-Representation Coloring settings — the coloringMethod
		 * enum + any per-method parameters. Cut over to the §2 `bool apply()`
		 * command contract in 999.59-02 (declared §2a invalidation: SOFT
		 * refresh — a color change leaves the primitive set intact).
		 */
		class BALL_VIEW_EXPORT ColoringController : public Controller
		{
			Q_OBJECT

			Q_PROPERTY(int coloringMethod READ coloringMethod WRITE setColoringMethod NOTIFY coloringMethodChanged)
			Q_PROPERTY(float valueMin READ valueMin WRITE setValueMin NOTIFY valueMinChanged)
			Q_PROPERTY(float valueMax READ valueMax WRITE setValueMax NOTIFY valueMaxChanged)

			public:
				explicit ColoringController(Representation* rep = nullptr,
				                            QObject* parent = nullptr);
				~ColoringController() override;

				Representation* representation() const { return rep_; }
				void setRepresentation(Representation* rep);

				int coloringMethod() const { return coloring_method_; }
				float valueMin() const { return value_min_; }
				float valueMax() const { return value_max_; }
				// 999.59-02 — re-entrancy state now lives in the base depth
				// counter (applying_depth_).
				bool isApplying() const { return applying_depth_ > 0; }

				/**
				 * Apply the staged coloring settings to the attached
				 * Representation, command-shaped per §2. Returns true when the
				 * Representation was mutated, false when dropped (re-entry) or
				 * rejected (no Representation / busy). Overrides
				 * Controller::apply(). Called directly by the Inspector section
				 * (controller_->apply()), not as a string-based Qt slot, so the
				 * bool return is binding-surface-compatible with prior void.
				 * @return true if the owner was mutated, false otherwise.
				 */
				bool apply() override;

				/**
				 * Reset the mirror to the owner's current state (999.64 reset
				 * path). Overrides Controller::reset(); delegates to revert().
				 */
				void reset() override;

				/**
				 * Stage both value-range endpoints in one call (999.63). A
				 * convenience over setValueMin()/setValueMax() so the histogram
				 * widget commits an atomic (min, max) through a single
				 * setRange(min, max); apply() — the §2 mutation path. Swaps the
				 * pair if @p min > @p max. Emits valueMinChanged/valueMaxChanged
				 * for any endpoint that actually moved; does NOT mutate the owner
				 * (apply() does that).
				 * @param min low endpoint.
				 * @param max high endpoint.
				 */
				void setRange(float min, float max);

				/**
				 * RENDER-ONLY drag-preview hint (999.63, ARCHITECTURE-CONTRACT.md
				 * §5 "render-only hints are NOT mutations").
				 *
				 * Paints a transient colormap-clamp hint for a candidate (min,
				 * max) while the user drags a histogram handle. It is explicitly
				 * <b>non-owner</b>: it creates NO ApplyPayload, emits NO typed
				 * event, calls NO owner setter, and takes NO ControllerApplyGuard.
				 * It does NOT touch coloring_method_ / value_min_ / value_max_ (the
				 * staged mutation state). It is a second RENDER path, never a
				 * second MUTATION path — commit happens only on handle release
				 * through setRange().apply(). Idempotent and freely re-callable
				 * per drag-frame. A contract test
				 * (coloring_preview_nonmutation_test) asserts it mutates nothing.
				 * @param min candidate low endpoint (render-only).
				 * @param max candidate high endpoint (render-only).
				 */
				void previewRange(float min, float max);

				/**
				 * Cached bin-summary read-model for the current selection +
				 * coloring method (999.63, §5). The Controller computes the
				 * histogram once (O(N) over the attached Representation's
				 * composites) and caches it against (composite-set, method,
				 * bin-count); the widget reads the returned value type and never
				 * walks the Composite tree itself. Re-binning is triggered only
				 * when the cache key changes. Returns an invalid BinSummary
				 * (isValid() == false) for an empty selection or a non
				 * value-based coloring method.
				 * @param bins requested bin count (default 27, mockups board 05).
				 * @return the (possibly cached) bin summary by value.
				 */
				BinSummary distribution(int bins = 27);

			public Q_SLOTS:
				void revert();
				void setColoringMethod(int m);
				void setValueMin(float v);
				void setValueMax(float v);

			Q_SIGNALS:
				void coloringMethodChanged(int m);
				void valueMinChanged(float v);
				void valueMaxChanged(float v);
				void appliedStub();

			protected:
				/**
				 * §2a declared invalidation — SOFT refresh
				 * (representation_->update(false)). A coloring-method or
				 * value-range change re-walks the color processor over the
				 * existing primitives; the primitive set is unchanged, so no
				 * display-list rebuild is required (contrast ModelController,
				 * which rebuilds). Overrides Controller::invalidateDeclared_().
				 */
				void invalidateDeclared_() override;

			private:
				/**
				 * §13-cookbook mutation body, moved out of apply(): rebuilds
				 * the color processor when the method changed and pushes the
				 * value range. Precondition rep_ != nullptr guaranteed by
				 * apply(). Does NOT invalidate (that is invalidateDeclared_()).
				 * @return true (it always installs the staged coloring state).
				 */
				bool applyInternal_();

				/**
				 * True iff @p method is a value-based (interpolating) coloring
				 * method — the ones the histogram + range widget apply to
				 * (temperature factor / occupancy / atom charge / distance /
				 * forces / custom). Non-value methods (element, chain, …) have no
				 * scalar distribution and yield an invalid BinSummary.
				 */
				static bool isValueBasedMethod_(int method);

				/**
				 * Extract the per-atom scalar value for the current coloring
				 * method from @p atom. Mirrors the per-method field the matching
				 * InterpolateColorProcessor walks (temperature factor / occupancy
				 * / charge / …), so the histogram bins the same values the
				 * colormap paints. @return the scalar, or 0 for unknown methods.
				 */
				float atomValue_(const Atom& atom) const;

				/**
				 * Recompute the bin summary for the current composites + method +
				 * @p bins and refresh the cache key. O(N) over the attached
				 * Representation's composites; called only on a cache miss.
				 */
				void rebuildDistribution_(int bins);

				/// Cheap content-hash of the attached Representation's composite
				/// set (pointer identities + count) — the selection component of
				/// the distribution cache key.
				std::size_t compositeHash_() const;

				/// Capture-only reversible intent (§2 step 7; v2.0 UndoStack).

				Representation* rep_;
				int coloring_method_;
				float value_min_;
				float value_max_;

				// 999.63 — distribution() cache. Key = (composite-hash, method,
				// bin-count). Invalidated lazily on the next distribution() call
				// when any key component differs (selection-change / method-change).
				BinSummary  dist_cache_;
				std::size_t dist_cache_composite_hash_ = 0;
				int         dist_cache_method_ = -1;
				int         dist_cache_bins_ = -1;
				bool        dist_cache_valid_ = false;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H
