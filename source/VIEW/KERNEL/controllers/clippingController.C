// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ClippingController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/clippingController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/representationManager.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/common.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/CONCEPT/timeStamp.h>

#include <sstream>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Compact owner-state snapshot for the ApplyPayload (§2 step 3).
			// Reads the Inspector's clipping plane (first plane in the manager).
			String snapshotClippingState_()
			{
				MainControl* mc = MainControl::getInstance(0);
				if (mc == nullptr) return String("clipping{no-mc}");
				const vector<ClippingPlane*>& planes =
					mc->getRepresentationManager().getClippingPlanes();
				std::ostringstream s;
				if (planes.empty() || planes.front() == nullptr)
				{
					s << "clipping{none}";
				}
				else
				{
					ClippingPlane* p = planes.front();
					s << "active=" << (p->isActive() ? 1 : 0)
					  << ";capped=" << (p->cappingEnabled() ? 1 : 0);
				}
				return String(s.str());
			}
		}

		ClippingController::ClippingController(Scene* scene, QObject* parent)
			: Controller(parent), scene_(scene),
				enabled_(false), offset_(0.0f), capped_(true)
		{
		}

		ClippingController::~ClippingController()
		{
			// Release the headless fallback plane if we created one (the
			// RepresentationManager owns production planes, not this).
			delete owned_plane_;
			owned_plane_ = nullptr;
		}

		void ClippingController::setScene(Scene* scene)
		{
			if (scene_ == scene) return;
			scene_ = scene;
			revert();
		}

		void ClippingController::revert()
		{
			// Mirror the live clipping state into the controller so the
			// Inspector widgets resync. The Inspector owns a single
			// clipping plane (the first plane in the RepresentationManager,
			// created on first enable); read its active/capping state back.
			MainControl* mc = MainControl::getInstance(0);
			if (mc == nullptr) return;

			const vector<ClippingPlane*>& planes =
				mc->getRepresentationManager().getClippingPlanes();
			if (planes.empty()) return;

			ClippingPlane* plane = planes.front();
			if (plane == nullptr) return;

			bool live_enabled = plane->isActive();
			if (live_enabled != enabled_)
			{
				enabled_ = live_enabled;
				Q_EMIT enabledChanged(enabled_);
			}
			bool live_capped = plane->cappingEnabled();
			if (live_capped != capped_)
			{
				capped_ = live_capped;
				Q_EMIT cappedChanged(capped_);
			}
		}

		bool ClippingController::apply()
		{
			// 999.59-03 cut-over to the §2 `bool apply()` command contract.
			// The Inspector hosts a ClippingSection, so apply() mutates a
			// ClippingPlane through the RepresentationManager and triggers the
			// same redraw the legacy ClippingDialog uses. The legacy
			// ClippingDialog + SetClippingPlane dialogs remain reachable and
			// drive the SAME clipping planes (deletion deferred to 999.53).
			//
			// 999.59-03 CLOSES the §4 omission: the prior void body explicitly
			// SKIPPED the ControllerApplyGuard (clippingController.C ~line 88
			// comment, Codex HIGH #6). The cut-over now takes it like every
			// other apply().

			// ── 1. Preconditions (§2 step 1) ─────────────────────────────────
			// A MainControl is the production owner of the clip plane, but is
			// NOT required: headless (no MainControl) we mutate a controller-
			// owned fallback plane so the command still has a single funnel.
			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[ClippingController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// ── 2. Re-entrancy shield (§2 step 2 / §4) — THE OMISSION CLOSED ─
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "clipping.setEnabled";
			payload.before     = snapshotClippingState_();
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotClippingState_();

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT applied();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			invalidateDeclared_();               // soft refresh

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
		}

		bool ClippingController::applyInternal_()
		{
			// §13 cookbook step 2 — mutation body, moved out of apply().
			// Find-or-create the Inspector's clipping plane: through the
			// RepresentationManager in production, or a controller-owned
			// fallback plane when no MainControl is reachable (headless).
			MainControl* mc = MainControl::getInstance(0);
			RepresentationManager* pm =
				(mc != nullptr) ? &mc->getRepresentationManager() : nullptr;

			ClippingPlane* plane = nullptr;
			if (pm != nullptr)
			{
				const vector<ClippingPlane*>& planes = pm->getClippingPlanes();
				if (!planes.empty())
				{
					plane = planes.front();
				}
			}
			else
			{
				// Headless fallback — reuse our own plane across applies.
				plane = owned_plane_;
			}

			bool created = false;
			if (plane == nullptr)
			{
				plane = new ClippingPlane;
				created = true;

				// Seed the plane's point/normal from the camera, reusing the
				// legacy SetClippingPlane semantics: point at the look-at
				// position, normal along the view vector (default Vector3(1,0,0)
				// fallback per setClippingPlane.C:75 if no camera is reachable).
				Vector3 base(0.0f, 0.0f, 0.0f);
				Vector3 normal(1.0f, 0.0f, 0.0f);
				Scene* scene = (scene_ != nullptr) ? scene_ : Scene::getInstance(0);
				if (scene != nullptr && scene->getStage() != nullptr)
				{
					const Camera& camera = scene->getStage()->getCamera();
					base = camera.getLookAtPosition();
					Vector3 n(-camera.getViewVector());
					if (!Maths::isZero(n.getLength())) { n.normalize(); normal = n; }
				}
				plane->setPoint(base);
				plane->setNormal(normal);

				if (pm != nullptr)
				{
					// Clip every current Representation (mirror
					// ClippingDialog::accept which rebuilds
					// plane->getRepresentations()).
					RepresentationList::const_iterator it = pm->getRepresentations().begin();
					for (; it != pm->getRepresentations().end(); ++it)
					{
						plane->getRepresentations().insert(*it);
					}
					pm->insertClippingPlane(plane);
				}
				else
				{
					// Headless — keep ownership on the controller.
					owned_plane_ = plane;
				}
			}
			(void)created;

			// Push mirrored state onto the plane.
			plane->setActive(enabled_);
			plane->setCappingEnabled(capped_);

			// Apply offset_ along the plane normal from the seed point. The
			// offset is a unit-fraction (0..1) of a fixed shift distance,
			// mirroring SetClippingPlane's point/normal handling: shift the
			// plane point along its normal by offset_ * SHIFT.
			Vector3 n = plane->getNormal();
			if (Maths::isZero(n.getLength()))
			{
				n = Vector3(1.0f, 0.0f, 0.0f);
				plane->setNormal(n);
			}
			else
			{
				n.normalize();
			}
			constexpr float SHIFT = 20.0f;
			Scene* scene = (scene_ != nullptr) ? scene_ : Scene::getInstance(0);
			Vector3 base = plane->getPoint();
			if (scene != nullptr && scene->getStage() != nullptr)
			{
				base = scene->getStage()->getCamera().getLookAtPosition();
			}
			plane->setPoint(base - n * (offset_ * SHIFT));

			return true;
		}

		void ClippingController::invalidateDeclared_()
		{
			// §2a Clipping row — SOFT refresh. A clip-plane change re-draws the
			// EXISTING geometry via the same redraw the legacy ClippingDialog
			// uses; the primitive set is unchanged. Headless (no MainControl)
			// there is nothing to redraw.
			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr)
			{
				mc->redrawAllRepresentations();
			}
		}

		void ClippingController::reset()
		{
			// §2 reset path (999.64 consumes) — single-pass re-sync from owner.
			revert();
		}


		void ClippingController::setEnabled(bool b)
		{
			if (b == enabled_) return;
			enabled_ = b;
			Q_EMIT enabledChanged(b);
		}

		void ClippingController::setOffset(float v)
		{
			if (v == offset_) return;
			offset_ = v;
			Q_EMIT offsetChanged(v);
		}

		void ClippingController::setCapped(bool b)
		{
			if (b == capped_) return;
			capped_ = b;
			Q_EMIT cappedChanged(b);
		}

	} // namespace VIEW
} // namespace BALL

