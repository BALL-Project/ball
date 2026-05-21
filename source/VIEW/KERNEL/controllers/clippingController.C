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
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/common.h>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		ClippingController::ClippingController(Scene* scene, QObject* parent)
			: QObject(parent), scene_(scene),
				enabled_(false), offset_(0.0f), capped_(true)
		{
		}

		ClippingController::~ClippingController() = default;

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

		void ClippingController::apply()
		{
			// UFG-11 cut-over (v1.7.x-01 / Phase 999.51) — the Inspector now
			// hosts a ClippingSection, so apply() functionally mutates a
			// ClippingPlane through the RepresentationManager and triggers
			// the same redraw the legacy ClippingDialog uses. The legacy
			// ClippingDialog + SetClippingPlane dialogs remain reachable and
			// drive the SAME clipping planes (deletion deferred to 999.53).
			MainControl* mc = MainControl::getInstance(0);
			if (mc == nullptr)
			{
				Log.warn() << "[ClippingController::apply] no MainControl available — skipping." << std::endl;
				return;
			}

			// Busy guard — mirrors ModelController/MaterialController. If the
			// renderer or another modal is doing work, defer rather than
			// racing the scene mutation. NOTE: unlike Model/Material, clipping
			// apply does not trigger a synchronous re-entrant controller
			// notification (the redraw path emits no *Changed back into this
			// controller), so the ControllerApplyGuard re-entrancy shield is
			// intentionally omitted here.
			if (mc->isBusy())
			{
				Log.info() << "[ClippingController::apply] MainControl busy — deferring." << std::endl;
				return;
			}

			RepresentationManager& pm = mc->getRepresentationManager();

			// Find-or-create the Inspector's clipping plane. The Inspector
			// owns a single plane: reuse the first plane in the manager, or
			// create one on first enable (mirror scene.C:3448 setupViewVolume).
			ClippingPlane* plane = nullptr;
			const vector<ClippingPlane*>& planes = pm.getClippingPlanes();
			if (!planes.empty())
			{
				plane = planes.front();
			}

			if (plane == nullptr)
			{
				if (!enabled_)
				{
					// Nothing to disable and nothing requested — no-op.
					Q_EMIT appliedStub();
					return;
				}
				plane = new ClippingPlane;

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

				// Clip every current Representation (mirror ClippingDialog::accept
				// which rebuilds plane->getRepresentations()).
				RepresentationList::const_iterator it = pm.getRepresentations().begin();
				for (; it != pm.getRepresentations().end(); ++it)
				{
					plane->getRepresentations().insert(*it);
				}

				pm.insertClippingPlane(plane);
			}

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

			// Trigger the same redraw the legacy ClippingDialog uses.
			mc->redrawAllRepresentations();

			Q_EMIT appliedStub();
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

